#include "StdAfx.h"
#include "control.h"
#include "window.h"

namespace Win {
	HWND Control::Handle() {
		return m_ctrlHwnd;
	}

	const HWND Control::Handle() const {
		return m_ctrlHwnd;
	}

	HWND Control::DialogHandle() {
		return m_dlgHwnd;
	}

	Control::Control(int id, HWND hwnd):m_id(id), m_dlgHwnd(hwnd) {
		if (hwnd == 0) throw Err::InvalidParam(L"hwnd was null");

		m_ctrlHwnd = GetDlgItem(hwnd, id);
		m_controls[m_ctrlHwnd] =  Ptr(this);
	}

	Control::Control():m_ctrlHwnd(0), m_dlgHwnd(0), m_id(0) {}
	Control::~Control() {}

	Control::Ptr Control::GetControl(HWND hwnd) {
		if (hwnd == 0) throw Err::InvalidParam(L"hwnd was null");

		auto i = m_controls.find(hwnd);

		if (i != m_controls.end())
			return i->second;

		return Control::Ptr();
	}

	void Control::DestructControls() {
		m_controls.clear();
	}

	void Control::Visible(bool isVisible) {
		ShowWindow(Handle(), (isVisible ? SW_SHOW : SW_HIDE));
	}

	bool Control::Visible() const {
		return IsWindowVisible(Handle()) != 0;
	}

	void Control::AttachTo(Window* owner) {
		COND_STRICT(owner, Err::InvalidParam, TX("owner was Null."));
		m_dlgHwnd = owner->Handle();
		m_ctrlHwnd = PerformDynamicCreate();
	}

	HWND Control::PerformDynamicCreate() {
		return 0;
	}

	Geom::RectInt Control::Position() {
		RECT r;
		GetWindowRect(Handle(), &r);
		return RECTToRect(r);
	}

	boost::optional<LRESULT> Control::PerformOnNotify(LPNMHDR lParam) {
		return nullptr;
	}

	boost::optional<LRESULT> Control::OnNotify(LPNMHDR lParam) {
		return PerformOnNotify(lParam);
	}

	Control::ControlMap Control::m_controls;
}
