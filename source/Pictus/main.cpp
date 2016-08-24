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

#ifdef _WIN32
#include <windows.h>
#endif

class MyApp:public wxApp
{
public:
	bool OnInit() override
	{
		SetVendorName("Poppeman");

		if(!wxApp::OnInit())
		{
			return false;
		}

		if(argc > 1)
		{
			return start_app(std::string(argv[1].mbc_str())) == EXIT_SUCCESS;
		}
		else
		{
			return start_app("") == EXIT_SUCCESS;
		}
	}

	MyApp() {
#ifdef _WIN32
		m_singleMutex = nullptr;
#endif
	}

	~MyApp() {
#ifdef _WIN32
		if (m_singleMutex != nullptr) {
			ReleaseMutex(m_singleMutex);
			CloseHandle(m_singleMutex);
		}
#endif
	}

private:
	App::Viewer* m_viewer;
	Img::CodecFactoryStore cfs;

#ifdef _WIN32
	HANDLE m_singleMutex;
#endif

	void EnsureSingleInstance() {
		// Look for another process (disallow if the setting requires that)
#ifdef _WIN32
		m_singleMutex = CreateMutex(0, true, (std::wstring(L"Local\\") + ClassName).c_str());

		if ((GetLastError() == ERROR_ALREADY_EXISTS) && (cfg.View.MultipleInstances == false)) {
				if (HWND hwnd = ::FindWindow(ClassName, 0)) {
						SetForegroundWindow(hwnd);
						if (IsIconic(hwnd)) {
								ShowWindow(hwnd, SW_RESTORE);
						}

						// Tell the instance to open the new location/file (if applicable)
						if (params.empty() == false) {
								auto wide = UTF8ToWString(params);

								// Dodge const-incorrectness
								boost::scoped_array<wchar_t> pStrData(new wchar_t[wide.length() + 1]);
								wcscpy_s(pStrData.get(), (wide.length() + 1), wide.c_str());
								COPYDATASTRUCT cds;
								cds.dwData = 0;
								cds.cbData = static_cast<DWORD>((params.length() + 1) * sizeof(wchar_t));
								cds.lpData = reinterpret_cast<void*>(pStrData.get());
								SendMessage(hwnd, WM_COPYDATA, (WPARAM)Handle(), (LPARAM)&cds);
						}

						throw Err::DuplicateInstance();
				}
		}
#else
		// TODO: Posix'ish implementation (pidfile)
#endif
	}

	int start_app(std::string params) {
		if (params == "--cleanup") {
			// If the user refused to uninstall the previous version when upgrading, the uninstaller will likely call pictus.exe with --cleanup
			// as parameter. Let's not throw up a blank Pictus windows in that situation.
			// This applies when 1.1.4.0 or older was installed.
			return EXIT_SUCCESS;
		}

		params = "/media/Misc/tmp/In/";

		// Make sure that boost (and boost.filesystem) uses UTF-8 on Windows whenever possible.
		std::locale::global(boost::locale::generator().generate(""));
		boost::filesystem::path::imbue(std::locale());

		cfs.AddBuiltinCodecs();

		try {
			auto cfg = Reg::Load(App::cg_SettingsLocation);

			// TODO: Control logging by some other mechanism, such as an .ini setting
			//Log.SetOutput(assure_folder(App::cg_RunLogLocation));

			EnsureSingleInstance();

			Intl::LanguageTable(c_lang_strings);
			Intl::CurrentLanguage(cfg.View.Language);

			m_viewer = new App::Viewer(&cfs, cfg);

			// Attempt to display viewer
			if (m_viewer->Init(params.c_str()))
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
