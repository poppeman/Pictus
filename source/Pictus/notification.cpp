#include "notification.h"
#include "basewin.h"

#define SET_OR_UNSET(c, v, m) if (c) v |= m; else v &= ~m;

namespace Win {
	DWORD Notification::m_idCounter = 0;
	std::mutex Notification::m_mutexCounter;

	void Notification::Show(HWND hwndOwner) {
		m_nid.hWnd = hwndOwner;
		Shell_NotifyIcon(NIM_ADD, &m_nid);
	}

	void Notification::Remove() {
		Shell_NotifyIcon(NIM_DELETE, &m_nid);
	}

	void Notification::Icon(HICON icon) {
		m_nid.hIcon = icon;
		SET_OR_UNSET(icon, m_nid.uFlags, NIF_ICON);
	}

	void Notification::ToolTip(const std::wstring& text) {
        wcscpy_s(m_nid.szTip, 128, text.c_str());
		SET_OR_UNSET(text != L"", m_nid.uFlags, NIF_TIP);
	}

	Notification::Notification() {
		ZeroMemory(&m_nid, sizeof(m_nid));
		m_nid.cbSize = sizeof(m_nid);
		m_nid.uCallbackMessage = WWM_NOTIFICATIONAREAICON_CLICK;
		m_nid.uFlags = NIF_MESSAGE;

		std::lock_guard<std::mutex> l(m_mutexCounter);
		m_nid.uID = m_idCounter++;
	}

	Notification::~Notification() {
		Remove();
	}

	void Notification::Balloon(const std::wstring& text, const std::wstring& caption, DWORD msTimeout) {
		m_nid.uFlags |= NIF_INFO;
        wcscpy_s(m_nid.szInfo, 256, text.c_str());
        wcscpy_s(m_nid.szInfoTitle, 64, caption.c_str());
		m_nid.dwInfoFlags = NIIF_NONE;
		m_nid.uTimeout = msTimeout;
		m_nid.uVersion = NOTIFYICON_VERSION;
	}

	void Notification::BalloonIcon(HICON icon) {
		m_nid.dwInfoFlags |= NIIF_USER;
		m_nid.hIcon = icon;
	}

	void Notification::BalloonIcon(BalloonStandardIcon icon) {
		if (icon == BSIWarning)
			m_nid.dwInfoFlags = NIIF_WARNING;
		else if (icon == BSIError)
			m_nid.dwInfoFlags = NIIF_ERROR;
		else if (icon == BSIInfo)
			m_nid.dwInfoFlags = NIIF_INFO;
		else
			DO_THROW(Err::InvalidParam, "Unknown standard icon param: " + ToAString(icon));
	}
}
