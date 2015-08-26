#include "StdAfx.h"
#include "registry.h"
#include "w32_assure_folder.h"
#include "reg_keyboard_binding_translator.h"
#include "reg_language_translator.h"
#include "reg_mouse_action_translator.h"
#include "reg_resize_behavior_translator.h"
#include "reg_resize_position_method_translator.h"

namespace Reg {
	Reg::Settings Load(const std::wstring& name) {
		Reg::Settings cfg;
		std::wstring full_name = assure_folder(name);

		IO::FileReader r(full_name);
		r.Open();
		auto sz = r.Size();
		std::string data;
		data.resize(sz);
		r.ReadFull(&data[0], sz);
		r.Close();
		std::stringstream ss(data);
		boost::property_tree::ptree pt;

		try {
			boost::property_tree::ini_parser::read_ini(ss, pt);
		}
		catch (std::exception const& ex) {
			Log << L"(Reg::Load) Failed parsing configuration file: " << ex.what() << L"\n";
		}

		cfg.Cache.DoAutoMemoryLimit = pt.get<bool>("Settings.AutoMemoryLimit", true);
		cfg.Cache.ManualMemoryLimit = pt.get<bool>("Settings.ManualMemoryLimit", 0);

		//cfg.Mouse.
		cfg.Mouse.OnMouseLeft = pt.get<App::MouseAction>("Settings.OnMouseLeft", App::MouseAction::MousePan);
		cfg.Mouse.OnMouseLeftDbl = pt.get<App::MouseAction>("Settings.OnMouseLeftDbl", App::MouseAction::MouseFullscreen);
		cfg.Mouse.OnMouseMiddle = pt.get<App::MouseAction>("Settings.OnMouseMioddle", App::MouseAction::MouseToggleFullSizeDefaultZoom);
		cfg.Mouse.OnMouseMiddleDbl = pt.get<App::MouseAction>("Settings.OnMouseMiddleDbl", App::MouseAction::MouseDisable);
		cfg.Mouse.OnMouseRight = pt.get<App::MouseAction>("Settings.OnMouseRight", App::MouseAction::MouseContext);
		cfg.Mouse.OnMouseRightDbl = pt.get<App::MouseAction>("Settings.OnMouseRightDbl", App::MouseAction::MouseDisable);
		cfg.Mouse.OnMouseWheelDown = pt.get<App::MouseAction>("Settings.OnMouseWheelDown", App::MouseAction::MouseNextImage);
		cfg.Mouse.OnMouseWheelUp = pt.get<App::MouseAction>("Settings.OnMouseWheelUp", App::MouseAction::MousePrevImage);


		cfg.Mouse.OnMouseRight = pt.get<App::MouseAction>("Settings.OnMouseRight", App::MouseAction::MouseContext);

		cfg.Render.BackgroundColor = Img::Color::FromDWord(pt.get<uint32_t>("Settings.BackgroundColor", 0x00ccccff)); // TODO: Handle #ff00ff representation
		cfg.Render.MagFilter = Filter::Mode(pt.get<uint32_t>("Settings.MagFilter", static_cast<int>(Filter::Mode::Bilinear)));
		cfg.Render.MinFilter = Filter::Mode(pt.get<uint32_t>("Settings.MinFilter", static_cast<int>(Filter::Mode::Bilinear)));

		// cfg.View.
		cfg.View.AlwaysOnTop = pt.get<bool>("Settings.AlwaysOnTop", false);
		cfg.View.BrowseWrapAround = pt.get<bool>("Settings.BrowseWrapAround", false);
		cfg.View.DefaultZoomMode = App::ZoomMode(pt.get<uint32_t>("Settings.DefaultZoom", static_cast<int>(App::ZoomFitImage)));
		cfg.View.Language = pt.get<Intl::Language>("Settings.Language", Intl::Language::English);
		cfg.View.Maximized = pt.get<bool>("Settings.Maximized", false);
		cfg.View.MultipleInstances = pt.get<bool>("Settings.MultipleInstances", false);
		cfg.View.ResetPan = pt.get<bool>("Settings.ResetPan", true);
		cfg.View.ResetZoom = pt.get<bool>("Settings.ResetZoom", true);
		cfg.View.ResizeBehaviour = pt.get<App::ResizeBehaviour>("Settings.ResizeBehaviour", App::ResizeBehaviour::ResizeReduceOnly);

		// TODO: Probably wrong constant here
		cfg.View.ResizePositionMethod = pt.get<App::ResizePositionMethod>("Settings.ResizePositionMethod", App::ResizePositionMethod::PositionNothing);

		// TODO: Set up some defaults if no shortcuts are defined.
		// cfg.Keyboard.
		int index = 0;
		while (true) {
			std::stringstream ss;
			ss << "Keyboard." << index++;
			auto inBinding = pt.get_optional<KeyboardBinding>(ss.str());
			if (!inBinding) break;

			cfg.Keyboard.Bindings.push_back(inBinding.get());
		}

		return cfg;
	}

	void Save(const std::wstring& name, const Reg::Settings& data) {
		std::wstring full_name = assure_folder(name);
		std::stringstream ss;
		ss << (char)0xef << (char)0xbb << (char)(0xbf);
		boost::property_tree::ptree pt;

		return;

		/*for (int i = 0; i < Reg::Keys::FinalDWORD; ++i) {
			std::string name = std::string("Settings.") + c_DWordSettings[i].Name;
			pt.put<int>(name.c_str(), c_DWordSettings[i].Value);
		}*/

		boost::property_tree::ini_parser::write_ini(ss, pt);
		IO::FileWriter wrt;
		if (wrt.Open(full_name)) {
			auto str = ss.str();

			// Although technically valid, the implementation of write_ini doesn't produce good old \r\n linefeeds that
			// notepad and other shoddy programs expect. Therefore we'll convert the linefeeds to that format.
			boost::replace_all(str, "\n", "\r\n");
			wrt.Write(str.c_str(), sizeof(char), str.size());
		}
	}
}
