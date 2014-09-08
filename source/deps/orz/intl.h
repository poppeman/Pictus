#ifndef INTL_H
#define INTL_H

namespace Intl {
	enum class Language {
		English = 0,
		Swedish,
		Undefined,
	};

	struct TableEntry {
		const wchar_t* langs[static_cast<int>(Language::Undefined)];
	};

	void LanguageTable(_In_ const TableEntry* pTable, _In_ int minID, _In_ int maxID);
	void CurrentLanguage(_In_ const Language& lang);
	_Check_return_ const Language CurrentLanguage();
	_Check_return_ Language SystemLanguage();
	_Ret_z_ PCWSTR GetWString(_In_ int id);
	_Ret_z_ PCWSTR GetWStringLang(_In_ int id, _In_ Language lang);

	extern boost::signals2::signal<void()> OnLanguageChanged;
};

#endif
