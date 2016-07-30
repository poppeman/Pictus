#include "main.h"
#include "viewer.h"
#include "lang_val.h"
#include "registry.h"
#include "config.h"
//#include "control.h"
#include "orz/logger.h"
#include <boost/locale.hpp>
#include <wx/app.h>
#include <wx/msgdlg.h>

class MyApp:public wxApp
{
public:
	bool OnInit() override
	{
		if(argc > 1)
		{
			return start_app(std::string(argv[1].mbc_str())) == EXIT_SUCCESS;
		}
		else
		{
			return start_app("") == EXIT_SUCCESS;
		}
	}

private:
	App::Viewer* m_viewer;
	Img::CodecFactoryStore cfs;

	int start_app(std::string params) {
		if (params == "--cleanup") {
			// If the user refused to uninstall the previous version when upgrading, the uninstaller will likely call pictus.exe with --cleanup
			// as parameter. Let's not throw up a blank Pictus windows in that situation.
			// This applies when 1.1.4.0 or older was installed.
			return EXIT_SUCCESS;
		}

		params = "/home/pontus/test.jpg";

		// Make sure that boost (and boost.filesystem) uses UTF-8 on Windows whenever possible.
		std::locale::global(boost::locale::generator().generate(""));
		boost::filesystem::path::imbue(std::locale());

		cfs.AddBuiltinCodecs();

		try {
			auto cfg = Reg::Load(App::cg_SettingsLocation);

			// TODO: Control logging by some other mechanism, such as an .ini setting
			//Log.SetOutput(assure_folder(App::cg_RunLogLocation));

			Intl::LanguageTable(c_lang_strings);
			Intl::CurrentLanguage(cfg.View.Language);

			m_viewer = new App::Viewer(&cfs, cfg, params.c_str());

			// Attempt to display viewer
			if (m_viewer->Init())
			{
				m_viewer->Show(true);
			}

			//viewer.StartApplication();
		}
		catch(Err::DuplicateInstance&) {
			// This is not something out of the ordinary.
		}
		catch(std::exception& e) {
			Log << "Exception caught:\n" << e.what() << "\n";
			wxMessageBox((L"An error occurred: " + UTF8ToWString(e.what())).c_str(), L"Pictus Error", wxOK);
			//throw;	// Good for debugging
		}

		//Win::Control::DestructControls();

		return EXIT_SUCCESS;
	}

};

wxIMPLEMENT_APP(MyApp);
