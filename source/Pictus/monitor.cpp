#include "monitor.h"

namespace Win {
	class MonitorStore {
	private:
		typedef std::vector<Monitor> MonitorList;

	public:
		void Probe() {
			EnumDisplayMonitors(0, 0, MonitorProbeProc, (LPARAM)this);
		}

		const Monitor* FindAt(const Geom::PointInt& coordinate) {
			auto found = performSearch(coordinate);
			if ((found == 0) && (coordinate.X < 0 || coordinate.Y < 0)) {
				Geom::PointInt cappedCoordinate = Maximum(coordinate, Geom::PointInt(0, 0));
				found = performSearch(cappedCoordinate);
			}

			if (found == nullptr) {
				DO_THROW(Err::InvalidParam, L"Couldn't find a monitor for virtual screen coordinate: " + ToWString(coordinate));
			}

			return found;
		}

	private:
		const Monitor* performSearch(const Geom::PointInt& coordinate) {
			for (const auto& i : m_monitors) {
				if (i.Region().Contains(coordinate)) return &i;
			}

			return 0;
		}

		void AddMonitor(HMONITOR hMonitor) {
			m_monitors.push_back(Monitor(hMonitor));
		}

		static BOOL CALLBACK MonitorProbeProc(HMONITOR hMonitor, HDC, LPRECT, LPARAM data) {
			MonitorStore* saver = reinterpret_cast<MonitorStore*>(data);
			saver->AddMonitor(hMonitor);
			return TRUE;
		}

		MonitorList m_monitors;
	};

	MonitorStore g_store;

	void PollMonitors() {
		g_store.Probe();
	}

	_Check_return_ _Ret_ const Monitor* FindMonitorAt( _In_ Geom::PointInt coordinate ) {
		PollMonitors();
		return g_store.FindAt(coordinate);
	}

	Monitor::Monitor(_In_ HMONITOR hMonitor):m_hMonitor(hMonitor) {
		refreshMonitorInfo();
	}

	void Monitor::refreshMonitorInfo() {
		MONITORINFOEX mi;
		ZeroMemory(&mi, sizeof(mi));
		mi.cbSize = sizeof(mi);
		GetMonitorInfo(m_hMonitor, &mi);

		m_region	= RECTToRect(mi.rcMonitor);
		m_workArea	= RECTToRect(mi.rcWork);
	}

	_Check_return_ Geom::RectInt Monitor::Region() const {
		return m_region;
	}

	_Check_return_ Geom::RectInt Monitor::WorkArea() const {
		return m_workArea;
	}
}
