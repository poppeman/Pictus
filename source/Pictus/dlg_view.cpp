#include "dlg_view.h"
#include "registry.h"
#include "wintypes.h"
#include <wx/sizer.h>
#include <wx/statbox.h>
#include "settings_layout.h"

namespace App
{
	enum {
		AdaptWindowSizeId
	};

	class FilterModeClientData:public wxClientData
	{
	public:
		Filter::Mode Mode;

		FilterModeClientData(Filter::Mode mode):Mode(mode) {}
	};

	wxBEGIN_EVENT_TABLE(SetView, wxPanel)
		EVT_CHECKBOX(AdaptWindowSizeId, SetView::OnClickAdaptWindowSize)
	wxEND_EVENT_TABLE()

	std::string SetView::Caption()
	{
		return Intl::GetString(App::SIDSettingsViewer);
	}

	void SetView::PerformUpdateFromSettings(const Reg::Settings &settings)
	{
		m_cbDefaultZoom->SetSelection(settings.View.DefaultZoomMode);
		m_cbResizeBehavior->SetSelection(settings.View.ResizeBehaviour);
		SetFilterBoxValue(m_cbMagFilter, settings.Render.MagFilter);
		SetFilterBoxValue(m_cbMinFilter, settings.Render.MinFilter);

		m_wrapAround->SetValue(settings.View.BrowseWrapAround);
		m_resetZoom->SetValue(settings.View.ResetZoom);
		m_resetPan->SetValue(settings.View.ResetPan);
		m_adaptWindowSize->SetValue(settings.View.ResizeWindow);

		DoToggleResizeWindow(settings.View.ResizeWindow);

		// Position method
		auto posmethod = settings.View.ResizePositionMethod;
		if (posmethod == ResizePositionMethod::PositionToScreen)
		{
			m_positionMethod[0]->SetValue(true);
		}

		if (posmethod == ResizePositionMethod::PositionToCurrent)
		{
			m_positionMethod[1]->SetValue(true);
		}

		if (posmethod == ResizePositionMethod::PositionNothing)
		{
			m_positionMethod[2]->SetValue(true);
		}
	}

	void SetView::onWriteSettings(Reg::Settings &settings)
	{
		settings.View.BrowseWrapAround = m_wrapAround->GetValue();
		settings.View.ResetZoom = m_resetZoom->GetValue();
		settings.View.ResetPan = m_resetPan->GetValue();
		settings.View.ResizeWindow = m_adaptWindowSize->GetValue();
		settings.Render.MinFilter = GetFilterBoxValue(m_cbMinFilter);
		settings.Render.MagFilter = GetFilterBoxValue(m_cbMagFilter);

		settings.View.DefaultZoomMode = App::ZoomMode(m_cbDefaultZoom->GetSelection());
		settings.View.ResizeBehaviour = App::ResizeBehaviour(m_cbResizeBehavior->GetSelection());

		auto posmethod = settings.View.ResizePositionMethod;
		if (m_positionMethod[0]->GetValue())
		{
			posmethod = ResizePositionMethod::PositionToScreen;
		}
		else if (m_positionMethod[1]->GetValue())
		{
			posmethod = ResizePositionMethod::PositionToCurrent;
		}
		else
		{
			posmethod = ResizePositionMethod::PositionNothing;
		}

		settings.View.ResizePositionMethod = posmethod;
	}

	void SetView::DoToggleResizeWindow(bool newState)
	{
		for (auto pm : m_positionMethod)
		{
			pm->Enable(newState);
		}
	}

	wxChoice* SetView::SetupFilterBox(wxWindow* parent)
	{
		auto box = new wxChoice(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize);
		box->Append(Win::GetStringWx(SIDResampleModeNearestNeighbor), new FilterModeClientData(Filter::Mode::NearestNeighbor));
		box->Append(Win::GetStringWx(SIDResampleModeBilinear), new FilterModeClientData(Filter::Mode::Bilinear));
		box->Append(Win::GetStringWx(SIDResampleModeLanczos3), new FilterModeClientData(Filter::Mode::Lanczos3));
		return box;
	}

	Filter::Mode SetView::GetFilterBoxValue(wxChoice* filterBox)
	{
		auto index = filterBox->GetSelection();
		if(index < 0)
		{
			DO_THROW(Err::CriticalError, "Filter box had no selection, this should not be possible!");
		}

		auto data = dynamic_cast<FilterModeClientData*>(filterBox->GetClientObject(index));
		if(data == nullptr)
		{
			DO_THROW(Err::CriticalError, "Filter box selection did not have any data.");
		}
		return data->Mode;
	}

	SetView::SetView(wxWindow* parent) :
		App::SettingsPage(parent),
		m_cbDefaultZoom{nullptr},
		m_cbResizeBehavior{nullptr},
		m_cbMinFilter{nullptr},
		m_cbMagFilter{nullptr}
	{
		auto topSizer = new wxBoxSizer(wxVERTICAL);
		topSizer->Add(CreateZoomBox(), StaticBoxOuterPadding(0));
		topSizer->Add(CreateViewerBox(), StaticBoxOuterPadding(0));
		SetSizerAndFit(topSizer);
	}

	void SetView::OnClickAdaptWindowSize(wxCommandEvent &event)
	{
		DoToggleResizeWindow(m_adaptWindowSize->GetValue());
	}

	wxSizer* SetView::CreateZoomBox()
	{
		auto topmostBox = new wxStaticBoxSizer(wxVERTICAL, this, Win::GetStringWx(SIDSettingsViewerZoom));
		m_resetZoom = new wxCheckBox(topmostBox->GetStaticBox(), wxID_ANY, wxString::FromUTF8(Intl::GetString(SIDSettingsViewerResetZoom)));

		topmostBox->Add(CreateResizeAlgoSizer(topmostBox->GetStaticBox()), StaticBoxInnerPadding(0));
		topmostBox->Add(m_resetZoom, StaticBoxInnerPadding(0));
		topmostBox->Add(CreateModeSizer(topmostBox->GetStaticBox()), StaticBoxInnerPadding(0));

		return topmostBox;
	}


	wxSizer *SetView::CreateResizeAlgoSizer(wxWindow* parent)
	{
		auto topmostSizer= new wxBoxSizer(wxHORIZONTAL);
		auto magBox= new wxStaticBoxSizer(wxVERTICAL, parent, Win::GetStringWx(SIDSettingsViewerMagnification));
		auto minBox= new wxStaticBoxSizer(wxVERTICAL, parent, Win::GetStringWx(SIDSettingsViewerMinification));
		m_cbMagFilter = SetupFilterBox(magBox->GetStaticBox());
		magBox->Add(m_cbMagFilter, StaticBoxInnerPadding(1));
		m_cbMinFilter = SetupFilterBox(minBox->GetStaticBox());
		minBox->Add(m_cbMinFilter, StaticBoxInnerPadding(1));
		topmostSizer->Add(magBox, wxSizerFlags(1).Expand().Border(wxRIGHT, GetPadding() / 2));
		topmostSizer->Add(minBox, wxSizerFlags(1).Expand().Border(wxLEFT, GetPadding() / 2));
		return topmostSizer;
	}

	wxSizer *SetView::CreateModeSizer(wxWindow* parent)
	{
		auto topmostSizer = new wxBoxSizer(wxHORIZONTAL);
		auto defaultZoomModeBoxer = new wxStaticBoxSizer(wxVERTICAL, parent, Win::GetStringWx(SIDSettingsViewerDefaultZoomMode));
		auto resizeBox = new wxStaticBoxSizer(wxVERTICAL, parent, Win::GetStringWx(SIDSettingsViewerResizeBehavior));

		wxArrayString zoomChoices;
		zoomChoices.Add(Win::GetStringWx(SIDActionZoomFree));
		zoomChoices.Add(Win::GetStringWx(SIDActionZoomFitImage));
		m_cbDefaultZoom = new wxChoice(defaultZoomModeBoxer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, zoomChoices);
		defaultZoomModeBoxer->Add(m_cbDefaultZoom, StaticBoxInnerPadding(1));

		wxArrayString behaviors;
		behaviors.Add(Win::GetStringWx(SIDSettingsViewerReduceOrEnlarge));
		behaviors.Add(Win::GetStringWx(SIDSettingsViewerEnlargeOnly));
		behaviors.Add(Win::GetStringWx(SIDSettingsViewerReduceOnly));
		m_cbResizeBehavior = new wxChoice(resizeBox->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, behaviors);
		resizeBox->Add(m_cbResizeBehavior, StaticBoxInnerPadding(1));

		topmostSizer->Add(defaultZoomModeBoxer, wxSizerFlags(1).Expand().Border(wxRIGHT, GetPadding() / 2));
		topmostSizer->Add(resizeBox, wxSizerFlags(1).Expand().Border(wxLEFT, GetPadding() / 2));
		return topmostSizer;
	}

	wxSizer *SetView::CreateViewerBox()
	{
		auto viewerBox = new wxStaticBoxSizer(wxVERTICAL, this, Win::GetStringWx(SIDSettingsViewer));

		m_positionMethod.push_back(new wxRadioButton(viewerBox->GetStaticBox(), wxID_ANY, Win::GetStringWx(SIDSettingsViewerCenterScreen)));
		m_positionMethod.push_back(new wxRadioButton(viewerBox->GetStaticBox(), wxID_ANY, Win::GetStringWx(SIDSettingsViewerCenterPosition)));
		m_positionMethod.push_back(new wxRadioButton(viewerBox->GetStaticBox(), wxID_ANY, Win::GetStringWx(SIDSettingsViewerNoReposition)));

		m_wrapAround = new wxCheckBox(viewerBox->GetStaticBox(), wxID_ANY, wxString::FromUTF8(Intl::GetString(SIDSettingsViewerWrapAround)));
		m_resetPan = new wxCheckBox(viewerBox->GetStaticBox(), wxID_ANY, wxString::FromUTF8(Intl::GetString(SIDSettingsViewerResetPan)));
		m_adaptWindowSize = new wxCheckBox(viewerBox->GetStaticBox(), AdaptWindowSizeId, Win::GetStringWx(SIDSettingsViewerAdaptWindowSize));

		viewerBox->Add(m_wrapAround, HorizontalPaddingFirst(0));
		viewerBox->Add(m_resetPan, HorizontalPadding(0));
		viewerBox->Add(m_adaptWindowSize, HorizontalPadding(0));
		viewerBox->Add(m_positionMethod[0], HorizontalPadding(0, true));
		viewerBox->Add(m_positionMethod[1], HorizontalPadding(0, true));
		viewerBox->Add(m_positionMethod[2], HorizontalPaddingLast(0, true));

		return viewerBox;
	}

	void SetView::SetFilterBoxValue(wxChoice* filterBox, Filter::Mode mode)
	{
		for(unsigned i=0;i<filterBox->GetCount();i++)
		{
			auto data = dynamic_cast<FilterModeClientData*>(filterBox->GetClientObject(i));
			if(data == nullptr)
			{
				DO_THROW(Err::CriticalError, "Filter box selection did not have any data.");
			}
			if(data->Mode == mode)
			{
				filterBox->SetSelection(i);
				return;
			}
		}
		DO_THROW(Err::CriticalError, ("Filter box did not contain filter mode: " + ToAString(mode)).c_str());
	}

}
