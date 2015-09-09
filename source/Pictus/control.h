#ifndef CONTROL_H
#define CONTROL_H

#include "illa/geom.h"

namespace Win {
	class Window;

	class Control:boost::noncopyable {
	public:
		typedef std::shared_ptr<Control> Ptr;

		void Visible(bool isVisible);
		bool Visible() const;

		void Enabled(bool doEnable);

		HWND Handle();
		const HWND Handle() const;

		HWND DialogHandle();

		Geom::RectInt Position();
		void Position(const Geom::RectInt& newPosition);

		boost::optional<LRESULT> OnNotify(LPNMHDR lParam);

		Control(int id, HWND hwnd);
		virtual ~Control()=0;

		static Ptr GetControl(HWND hwnd);
		static void DestructControls();

		Control();

	private:
		void AttachTo(Window* owner);
		
		virtual boost::optional<LRESULT> PerformOnNotify(LPNMHDR lParam);
		virtual HWND PerformDynamicCreate();

		friend class Window;

		void operator =(const Control&) = delete;
		Control(const Control&) = delete;

		int m_id;
		HWND m_dlgHwnd;
		HWND m_ctrlHwnd;

		// HWND<->Control mapping
		typedef std::pair<HWND, Ptr> PairHWNDControl;
		typedef std::map<HWND, Ptr> ControlMap;
		static ControlMap m_controls;
	};
}

#endif
