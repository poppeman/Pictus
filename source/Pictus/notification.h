#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <mutex>
#include <string>

#include <windows.h>
#include <shellapi.h>

namespace Win {
	class Notification {
	public:
		enum BalloonStandardIcon {
			BSIError,
			BSIInfo,
			BSIWarning,
		};

		void Show(HWND hwndOwner);
		void Remove();

		void Icon(HICON icon);
		void ToolTip(const std::wstring& text);

		void Balloon(const std::wstring& text, const std::wstring& caption, DWORD msTimeout);

		void BalloonIcon(HICON icon);
		void BalloonIcon(BalloonStandardIcon icon);

		DWORD ID() const { return m_nid.uID; }

		Notification();
		~Notification();

	private:
		NOTIFYICONDATA m_nid;

		static std::mutex m_mutexCounter;
		static DWORD m_idCounter;
	};
}

#endif
