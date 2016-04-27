#include "orz/file_reader.h"
#include "orz/logger.h"
#include "registry.h"
#include "w32_assure_folder.h"
#include "reg_color_translator.h"
#include "reg_filter_mode_translator.h"
#include "reg_keyboard_binding_translator.h"
#include "reg_language_translator.h"
#include "reg_mouse_action_translator.h"
#include "reg_resize_behavior_translator.h"
#include "reg_resize_position_method_translator.h"
#include "reg_zoom_mode_translator.h"
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <windows.h>

namespace Reg {
	Settings Load(const std::string& name) {
		Reg::Settings cfg;
		auto full_name = assure_folder(name);

		boost::property_tree::ptree pt;

		// Scope ensures that file is closed
		{
			IO::FileReader r(full_name);
			if (r.Open()) {
				auto sz = r.Size();
				std::string data;
				data.resize(sz);
				r.ReadFull(&data[0], sz);

				std::stringstream ss(data);

				try {
					boost::property_tree::ini_parser::read_ini(ss, pt);
				}
				catch (std::exception const& ex) {
					Log << "(Reg::Load) Failed parsing configuration file: " << ex.what() << "\n";
				}
			}
		}

		cfg.Cache.DoAutoMemoryLimit = pt.get<bool>("Settings.AutoMemoryLimit", true);
		cfg.Cache.ManualMemoryLimit = pt.get<size_t>("Settings.ManualMemoryLimit", 0);

		//cfg.Mouse.
		cfg.Mouse.OnMouseLeft = pt.get<App::MouseAction>("Settings.OnMouseLeft", App::MouseAction::MousePan);
		cfg.Mouse.OnMouseLeftDbl = pt.get<App::MouseAction>("Settings.OnMouseLeftDbl", App::MouseAction::MouseFullscreen);
		cfg.Mouse.OnMouseMiddle = pt.get<App::MouseAction>("Settings.OnMouseMioddle", App::MouseAction::MouseToggleFullSizeDefaultZoom);
		cfg.Mouse.OnMouseMiddleDbl = pt.get<App::MouseAction>("Settings.OnMouseMiddleDbl", App::MouseAction::MouseDisable);
		cfg.Mouse.OnMouseRight = pt.get<App::MouseAction>("Settings.OnMouseRight", App::MouseAction::MouseContext);
		cfg.Mouse.OnMouseRightDbl = pt.get<App::MouseAction>("Settings.OnMouseRightDbl", App::MouseAction::MouseDisable);
		cfg.Mouse.OnMouseWheelDown = pt.get<App::MouseAction>("Settings.OnMouseWheelDown", App::MouseAction::MouseNextImage);
		cfg.Mouse.OnMouseWheelUp = pt.get<App::MouseAction>("Settings.OnMouseWheelUp", App::MouseAction::MousePrevImage);
		cfg.Mouse.OnMouseExtra1 = pt.get<App::MouseAction>("Settings.OnMouseExtra1", App::MouseAction::MousePrevImage);
		cfg.Mouse.OnMouseExtra2 = pt.get<App::MouseAction>("Settings.OnMouseExtra2", App::MouseAction::MouseNextImage);
		cfg.Mouse.OnMouseExtra1Dbl = pt.get<App::MouseAction>("Settings.OnMouseExtra1Dbl", App::MouseAction::MouseDisable);
		cfg.Mouse.OnMouseExtra2Dbl = pt.get<App::MouseAction>("Settings.OnMouseExtra2Dbl", App::MouseAction::MouseDisable);

		cfg.Mouse.OnMouseRight = pt.get<App::MouseAction>("Settings.OnMouseRight", App::MouseAction::MouseContext);

		cfg.Render.BackgroundColor = pt.get<Img::Color>("Settings.BackgroundColor", { 0, 0xcc, 0xcc, 0xff });
		cfg.Render.MagFilter = pt.get<Filter::Mode>("Settings.MagFilter", Filter::Mode::Bilinear);
		cfg.Render.MinFilter = pt.get<Filter::Mode>("Settings.MinFilter", Filter::Mode::Bilinear);

		// cfg.View.
		cfg.View.AlwaysOnTop = pt.get<bool>("Settings.AlwaysOnTop", false);
		cfg.View.BrowseWrapAround = pt.get<bool>("Settings.BrowseWrapAround", false);
		cfg.View.DefaultZoomMode = pt.get<App::ZoomMode>("Settings.DefaultZoom", App::ZoomFitImage);
		cfg.View.Language = pt.get<Intl::Language>("Settings.Language", Intl::Language::English);
		cfg.View.Maximized = pt.get<bool>("Settings.Maximized", false);
		cfg.View.MultipleInstances = pt.get<bool>("Settings.MultipleInstances", false);
		cfg.View.ResetPan = pt.get<bool>("Settings.ResetPan", true);
		cfg.View.ResetZoom = pt.get<bool>("Settings.ResetZoom", true);
		cfg.View.ResizeBehaviour = pt.get<App::ResizeBehaviour>("Settings.ResizeBehaviour", App::ResizeBehaviour::ResizeReduceOnly);
		cfg.View.ResizePositionMethod = pt.get<App::ResizePositionMethod>("Settings.ResizePositionMethod", App::ResizePositionMethod::PositionNothing);
		cfg.View.ResizeWindow = pt.get<bool>("Settings.ResizeWindow", true);
		cfg.View.ShowStatusBar = pt.get<bool>("Settings.ShowStatusBar", true);
		cfg.View.WindowAnchorCenterX = pt.get("Settings.CAnchorX", 100);
		cfg.View.WindowAnchorCenterY = pt.get("Settings.CAnchorY", 100);
		cfg.View.WindowAnchorTLX = pt.get("Settings.TLAnchorX", 100);
		cfg.View.WindowAnchorTLY = pt.get("Settings.TLAnchorY", 100);
		cfg.View.WindowPosX = pt.get("Settings.PosX", 100);
		cfg.View.WindowPosY = pt.get("Settings.PosY", 100);
		cfg.View.WindowSizeWidth = pt.get("Settings.WindowWidth", 640);
		cfg.View.WindowSizeHeight = pt.get("Settings.WindowHeight", 480);


		// cfg.Keyboard.
		int index = 0;
		for (;;) {
			std::stringstream ss;
			ss << "Keyboard." << index++;
			auto inBinding = pt.get_optional<KeyboardBinding>(ss.str());
			if (!inBinding) break;

			cfg.Keyboard.Bindings.push_back(inBinding.get());
		}
		if (cfg.Keyboard.Bindings.empty()) {
			using App::KeyAction;
			cfg.Keyboard.Bindings = {
				// Alt, shift, control
				{ { VK_F2, false, false, false }, KeyAction::RenameFile },
				{ { VK_ESCAPE, false, false, false }, KeyAction::CloseApplication },
				{ { VK_OEM_PLUS, false, false, false }, KeyAction::ZoomIn },
				{ { VK_ADD, false, false, false }, KeyAction::ZoomIn },
				{ { VK_NUMPAD0, false, false, false }, KeyAction::ZoomIn },

				{ { VK_OEM_MINUS, false, false, false }, KeyAction::ZoomOut },
				{ { VK_SUBTRACT, false, false, false }, KeyAction::ZoomOut },
				{ { VK_NUMPAD1, false, false, false }, KeyAction::ZoomOut },

				{ { L'0', false, false, false }, KeyAction::ZoomDefault },
				{ { L'1', false, false, false }, KeyAction::ZoomFull },
				{ { VK_MULTIPLY, false, false, false }, KeyAction::ZoomFree },

				{ { VK_UP, false, false, false }, KeyAction::PanUp },
				{ { VK_DOWN, false, false, false }, KeyAction::PanDown },
				{ { VK_LEFT, false, false, false }, KeyAction::PanLeft },
				{ { VK_RIGHT, false, false, false }, KeyAction::PanRight },

				{ { VK_HOME, false, false, false }, KeyAction::FirstImage },
				{ { VK_END, false, false, false }, KeyAction::LastImage },
				
				{ { VK_SPACE, false, false, false }, KeyAction::NextImage },
				{ { VK_NEXT, false, false, false }, KeyAction::NextImage },

				{ { VK_SPACE, false, true, false }, KeyAction::NextSkipImage },
				{ { VK_NEXT, false, true, false }, KeyAction::NextSkipImage },
				{ { VK_RIGHT, true, false, false }, KeyAction::NextSkipImage },

				{ { VK_BACK, false, false, false }, KeyAction::PreviousImage },
				{ { VK_PRIOR, false, false, false }, KeyAction::PreviousImage },

				{ { VK_BACK, false, true, false }, KeyAction::PreviousSkipImage },
				{ { VK_PRIOR, false, true, false }, KeyAction::PreviousSkipImage },
				{ { VK_LEFT, true, false, false }, KeyAction::PreviousSkipImage },

				{ { VK_DELETE, false, false, false }, KeyAction::RecycleCurrentFile },
				{ { VK_DELETE, false, true, false }, KeyAction::DeleteCurrentFile },
				{ { VK_DELETE, false, false, true }, KeyAction::RemoveCurrentImage },
				
				{ { L'R', false, false, false }, KeyAction::RandomImage },
				{ { L'O', false, false, false }, KeyAction::OpenSettings },
				{ { VK_RETURN, true, false, false }, KeyAction::ToggleFullscreen },
				{ { L'C', false, false, true}, KeyAction::CopyImage }
			};
		}

		return cfg;
	}

	void Save(const std::string& name, const Settings& settings) {
		auto full_name = assure_folder(name);
		std::stringstream ss;
		//ss << (char)0xef << (char)0xbb << (char)(0xbf);
		boost::property_tree::ptree pt;

		pt.put("Settings.AutoMemoryLimit", settings.Cache.DoAutoMemoryLimit);
		pt.put("Settings.ManualMemoryLimit", settings.Cache.ManualMemoryLimit);

		//cfg.Mouse.
		pt.put("Settings.OnMouseLeft", settings.Mouse.OnMouseLeft);
		pt.put("Settings.OnMouseLeftDbl", settings.Mouse.OnMouseLeftDbl);
		pt.put("Settings.OnMouseMioddle", settings.Mouse.OnMouseMiddle);
		pt.put("Settings.OnMouseMiddleDbl", settings.Mouse.OnMouseMiddleDbl);
		pt.put("Settings.OnMouseRight", settings.Mouse.OnMouseRight);
		pt.put("Settings.OnMouseRightDbl", settings.Mouse.OnMouseRightDbl);
		pt.put("Settings.OnMouseWheelDown", settings.Mouse.OnMouseWheelDown);
		pt.put("Settings.OnMouseWheelUp", settings.Mouse.OnMouseWheelUp);
		pt.put("Settings.OnMouseExtra1", settings.Mouse.OnMouseExtra1);
		pt.put("Settings.OnMouseExtra2", settings.Mouse.OnMouseExtra2);
		pt.put("Settings.OnMouseExtra1Dbl", settings.Mouse.OnMouseExtra1Dbl);
		pt.put("Settings.OnMouseExtra2Dbl", settings.Mouse.OnMouseExtra2Dbl);

		pt.put("Settings.BackgroundColor", settings.Render.BackgroundColor);
		pt.put("Settings.MagFilter", settings.Render.MagFilter);
		pt.put("Settings.MinFilter", settings.Render.MinFilter);

		// cfg.View.
		pt.put("Settings.AlwaysOnTop", settings.View.AlwaysOnTop);
		pt.put("Settings.BrowseWrapAround", settings.View.BrowseWrapAround);
		pt.put("Settings.DefaultZoom", settings.View.DefaultZoomMode);
		pt.put("Settings.Language", settings.View.Language);
		pt.put("Settings.Maximized", settings.View.Maximized);
		pt.put("Settings.MultipleInstances", settings.View.MultipleInstances);
		pt.put("Settings.ResetPan", settings.View.ResetPan);
		pt.put("Settings.ResetZoom", settings.View.ResetZoom);
		pt.put("Settings.ResizeBehaviour", settings.View.ResizeBehaviour);
		pt.put("Settings.ResizePositionMethod", settings.View.ResizePositionMethod);
		pt.put("Settings.ResizeWindow", settings.View.ResizeWindow);
		pt.put("Settings.ShowStatusBar", settings.View.ShowStatusBar);
		pt.put("Settings.CAnchorX", settings.View.WindowAnchorCenterX);
		pt.put("Settings.CAnchorY", settings.View.WindowAnchorCenterY);
		pt.put("Settings.TLAnchorX", settings.View.WindowAnchorTLX);
		pt.put("Settings.TLAnchorY", settings.View.WindowAnchorTLY);
		pt.put("Settings.PosX", settings.View.WindowPosX);
		pt.put("Settings.PosY", settings.View.WindowPosY);
		pt.put("Settings.WindowWidth", settings.View.WindowSizeWidth);
		pt.put("Settings.WindowHeight", settings.View.WindowSizeHeight);



		int index = 0;
		for (auto binding : settings.Keyboard.Bindings) {
			std::stringstream ss;
			ss << "Keyboard." << index++;
			pt.put(ss.str(), binding);
		}

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
