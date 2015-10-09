#ifndef PICTUS_WINTYPES_H
#define PICTUS_WINTYPES_H

#include "app_types.h"
#include <windows.h>

namespace Win {
    enum MouseButton {
        NoButton,
        Left,
        Right,
        Middle,
        Extra1,
        Extra2
    };

    class MouseEvent {
    public:
        MouseButton Button;
        Geom::PointInt Position;
        int WheelTicks;

        MouseEvent() : Button(Left), Position(0, 0), WheelTicks(0) {
        }

        MouseEvent(LPARAM lParam)
                : Position(static_cast<SHORT>(LOWORD(lParam)), static_cast<SHORT>(HIWORD(lParam))), WheelTicks(0) {
        }
    };

    class KeyEvent {
    public:
        WPARAM Key;
        bool AltPressed;
        bool CtrlPressed;
        bool ShiftPressed;

        bool operator<(const KeyEvent &rhs) const;

        KeyEvent(WPARAM key, bool isAltPressed, bool isCtrlPressed, bool isShiftPressed);
    };

    std::wstring LongPath(const std::wstring &path);

    Geom::RectInt RECTToRect(const RECT &rect);

    RECT RectToRECT(const Geom::RectInt &rect);

    POINT PointToPOINT(const Geom::PointInt &point);
}

#endif
