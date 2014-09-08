#ifndef STATUSBAR_PART_H
#define STATUSBAR_PART_H

namespace Win {
	class StatusBar;

	class StatusBarPart {
	public:
		void Text(const std::wstring newText);
		const std::wstring& Text() const;

		enum AutoSizeType {
			AutoSizeNone = 0,
			AutoSizeByContent,
			AutoSizeShare
		};
		void AutoSize(AutoSizeType ast);
		AutoSizeType AutoSize() const;
		void Width(int size);

		int Width() const;

		StatusBarPart();

	private:
		void Update();
		void AttachTo(StatusBar* target);
		int CalculateAutoSize() const;

		friend class StatusBar;

		std::wstring m_text;
		StatusBar* m_statusBar;
		int m_setWidth;
		AutoSizeType m_autoSize;
	};
}

#endif
