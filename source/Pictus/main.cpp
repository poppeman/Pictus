#include "main.h"
#include "viewer.h"
#include "lang_val.h"
#include "registry.h"
#include "config.h"
//#include "control.h"
#include "orz/logger.h"
#include "ipc_client.h"
#include "ipc_server.h"
#include <boost/filesystem.hpp>
#include <boost/locale.hpp>
#include <boost/scoped_array.hpp>
#include <wx/app.h>
#include <wx/msgdlg.h>
#include <wx/snglinst.h>

const wchar_t* ClassName = L"PictusMainMutex";

class MyApp:public wxApp
{
public:
	bool OnInit() override
	{
		SetVendorName("Poppeman");

		if(argc > 1)
		{
			// Workaround for bug or weird wxWidgets behavior.
			// "something.exe" "some long argument" results in the following:
			//    argv[0] = something.exe
			//    argv[1] = some long argument"
			std::string params(argv[1].ToUTF8());
			if (params[params.size() - 1] == '"' && params[0] != '"') {
				params = params.substr(0, params.size() - 1);
			}

			return start_app(params) == EXIT_SUCCESS;
		}
		else
		{
			return start_app("") == EXIT_SUCCESS;
		}
	}

	MyApp():
		m_singleMutex(ClassName)
	{}

	~MyApp() {}

private:
	App::Viewer* m_viewer = nullptr;
	Img::CodecFactoryStore cfs;
	wxSingleInstanceChecker m_singleMutex;
	std::shared_ptr<IPC::OpenFileServer> m_server;

	int start_app(std::string params) {
		if (params == "--cleanup") {
			// If the user refused to uninstall the previous version when upgrading, the uninstaller will likely call pictus.exe with --cleanup
			// as parameter. Let's not throw up a blank Pictus windows in that situation.
			// This applies when 1.1.4.0 or older was installed.
			return EXIT_SUCCESS;
		}

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

			m_viewer = new App::Viewer(&cfs, cfg);
			m_viewer->CreateWindow();
			if (cfg.View.MultipleInstances == false && m_singleMutex.IsAnotherRunning())
			{
				auto client = std::make_shared<IPC::OpenFileClient>();
				auto con = dynamic_cast<IPC::OpenFileConnection*>(client->MakeConnection(L"localhost", ClassName, L"OPENFILE"));
				if (con != nullptr)
				{
					con->SendOpenFile(params);
					con->Disconnect();
				}
				return EXIT_FAILURE;
			}

			m_server = std::make_shared<IPC::OpenFileServer>(m_viewer);
			m_server->Create(ClassName);

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
