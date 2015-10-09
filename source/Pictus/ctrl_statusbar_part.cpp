#include "ctrl_statusbar_part.h"
#include "ctrl_statusbar.h"

namespace Win {
	void StatusBarPart::Text(const std::wstring newText) {
		m_text = newText;
		Update();
	}

	const std::wstring& StatusBarPart::Text() const {
		return m_text;
	}

	StatusBarPart::StatusBarPart():
		m_statusBar{ 0 },
		m_setWidth{ 0 },
		m_autoSize{ AutoSizeNone }
	{}

	void StatusBarPart::AttachTo(StatusBar* target) {
		m_statusBar = target;
		Update();
	}

	void StatusBarPart::Update() {
		m_statusBar->Update();
	}

	void StatusBarPart::AutoSize(AutoSizeType ast) {
		m_autoSize = ast;
	}

	StatusBarPart::AutoSizeType StatusBarPart::AutoSize() const {
		return m_autoSize;
	}

	void StatusBarPart::Width(int size) {
		m_setWidth = size;
		m_autoSize = AutoSizeNone;
	}

	int StatusBarPart::CalculateAutoSize() const {
		HDC dcStatusBar = GetDC(m_statusBar->Handle());
		SIZE sz;
		GetTextExtentPoint32(dcStatusBar, m_text.c_str(), m_text.length(), &sz);
		ReleaseDC(m_statusBar->Handle(), dcStatusBar);
		return sz.cx;
	}

	int StatusBarPart::Width() const {
		return m_autoSize?CalculateAutoSize():m_setWidth;
	}
}
