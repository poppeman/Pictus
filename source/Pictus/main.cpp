#include "main.h"
#include "viewer.h"
#include "lang_val.h"
#include "registry.h"
#include "config.h"
#include "illa/codecmgr.h"
#include "orz/intl.h"
#include "control.h"
#include "w32_assure_folder.h"
#include "orz/logger.h"

int start_app(const std::wstring& params) {
	if (params == L"--cleanup") {
		// If the user refused to uninstall the previous version when upgrading, the uninstaller will likely call pictus.exe with --cleanup
		// as parameter. Let's not throw up a blank Pictus windows in that situation.
		// This applies when 1.1.4.0 or older was installed.
		return EXIT_SUCCESS;
	}

	Img::CodecFactoryStore cfs;
	cfs.AddBuiltinCodecs();

	try {
		auto cfg = Reg::Load(App::cg_SettingsLocation);

		// TODO: Control logging by some other mechanism, such as an .ini setting
		//Log.SetOutput(assure_folder(App::cg_RunLogLocation));

		Intl::LanguageTable(c_lang_strings);
		Intl::CurrentLanguage(cfg.View.Language);

		App::Viewer viewer(&cfs, cfg, params.c_str());

		// Attempt to display viewer
		if (viewer.Create(0)) {
			viewer.Show(true);
		}

		viewer.StartApplication();
	}
	catch(Err::DuplicateInstance&) {
		// This is not something out of the ordinary.
	}
	catch(Err::Exception& e) {
		Log << L"Exception caught:\n" << UTF8ToWString(e.what()) << L"\n";
		MessageBoxW(nullptr, (L"An error occurred: " + UTF8ToWString(e.what())).c_str(), L"Pictus Error", MB_OK);
		//throw;	// Good for debugging
	}

	Win::Control::DestructControls();

	return EXIT_SUCCESS;
}
