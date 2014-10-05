#ifndef MONITOR_H
#define MONITOR_H

#include "wintypes.h"

namespace Win {
	class Monitor {
	public:
		_Check_return_ Geom::RectInt Region() const;
		_Check_return_ Geom::RectInt WorkArea() const;

		Monitor(_In_ HMONITOR hMonitor);

	private:
		void refreshMonitorInfo();

		Geom::RectInt m_region;
		Geom::RectInt m_workArea;
		HMONITOR m_hMonitor;
	};

	_Check_return_ _Ret_ const Monitor* FindMonitorAt(_In_ Geom::PointInt coordinate);
}

#endif
