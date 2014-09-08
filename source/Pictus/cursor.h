#ifndef CURSOR_H
#define CURSOR_H

namespace Win {
	class Window;

	class Cursor {
	public:
		Cursor()
			:m_hCursor(0)
		{}

		Cursor(DWORD id)
			:m_hCursor(LoadCursor(GetModuleHandle(0), MAKEINTRESOURCE(id)))
		{}

	protected:
		bool SetCursor() {
			if (!m_hCursor) return false;
			::SetCursor(m_hCursor);
			return true;
		}

		friend class Win::Window;

	private:
		HCURSOR						m_hCursor;
	};
}

#endif
