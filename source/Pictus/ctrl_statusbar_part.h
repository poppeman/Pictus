#ifndef PICTUS_STATUSBAR_PART_H
#define PICTUS_STATUSBAR_PART_H

#include <string>

namespace Win {
	class StatusBar;

	class StatusBarPart {
	public:
		void Text(const std::string newText);
		const std::string& Text() const;

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

		std::string m_text;
		StatusBar* m_statusBar;
		int m_setWidth;
		AutoSizeType m_autoSize;
	};
}

#endif
