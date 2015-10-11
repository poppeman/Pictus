#include "basewin.h"
#include "orz/fileops.h"
#include "orz/sysinfo.h"

#include "ctrl_button.h"
#include "ctrl_editbox.h"
#include "ctrl_listview.h"
#include "ctrl_combobox.h"

#include <boost/scoped_array.hpp>
#include <mutex>

#include <commdlg.h>

namespace Win {
    using std::recursive_mutex;
    recursive_mutex g_mut;
    bool BaseWindow::m_isCursorVisible = true;
    const wchar_t *BaseWindow::ClassPtrProp = L"ClassPointer";
    using namespace Geom;

    BaseWindow::BaseWindow()
            : m_hwnd(0),
              m_pParent(0),
              m_isMoveByUser(false),
              m_hInstance(GetModuleHandle(0)),
              m_sidCaption(-1) {
        m_lang = Intl::OnLanguageChanged.connect([&]() {
            UpdateCaption();
        });
    }

    BaseWindow::~BaseWindow() {
        if (m_hwnd)
            EnumProps(m_hwnd, DelPropProc);
    }


    EditBox *BaseWindow::CreateEditBox(const DWORD id) {
        return new EditBox(id, Handle());
    }

    ListView *BaseWindow::CreateListView(const DWORD id) {
        return new ListView(id, Handle());
    }

    ComboBox *BaseWindow::CreateComboBox(const DWORD id) {
        Control::Ptr p = Control::GetControl(GetDlgItem(Handle(), id));
        if (p) return dynamic_cast<ComboBox *>(p.get());

        return new ComboBox(id, Handle());
    }

    Button *BaseWindow::CreateButton(DWORD id) {
        Control::Ptr p = Control::GetControl(GetDlgItem(Handle(), id));
        if (p) return dynamic_cast<Button *>(p.get());

        return new Button(id, Handle());
    }


    bool BaseWindow::Show(bool doShow) {
        ShowWindow(m_hwnd, (doShow ? SW_SHOW : SW_HIDE));
        return true;
    }

    RectInt BaseWindow::ClientRect() {
        RECT rect;
        GetClientRect(Handle(), &rect);
        return RECTToRect(rect);
    }

    PointInt BaseWindow::PositionScreen() {
        RECT rect;
        GetWindowRect(m_hwnd, &rect);
        return PointInt(rect.left, rect.top);
    }

    PointInt BaseWindow::CenterPositionScreen() {
        RECT rect;
        GetWindowRect(m_hwnd, &rect);
        auto w = rect.right - rect.left;
        auto h = rect.bottom - rect.top;
        return PointInt(rect.left + w / 2, rect.top + h / 2);
    }

    PointInt BaseWindow::PositionToParent() {
        RECT rect;
        GetWindowRect(Handle(), &rect);
        POINT pt = {rect.left, rect.top};

        ScreenToClient(Parent()->Handle(), &pt);
        return PointInt(pt.x, pt.y);
    }

    BaseWindow *BaseWindow::Parent() {
        return m_pParent;
    }

    HWND &BaseWindow::Handle() {
        return m_hwnd;
    }

    const HWND &BaseWindow::Handle() const {
        return m_hwnd;
    }

    void BaseWindow::Redraw() {
        performRedraw();
    }

    void BaseWindow::Caption(const std::wstring &newcapt) {
        if (Handle()) SetWindowText(Handle(), newcapt.c_str());
    }

    void BaseWindow::Caption(const int sidCaption) {
        m_sidCaption = sidCaption;
        Caption(UTF8ToWString(Intl::GetString(m_sidCaption)));
    }

    const std::wstring BaseWindow::Caption() const {
        if (Handle()) {
            // SEXY!
            int len = GetWindowTextLength(Handle()) + 1;
            boost::scoped_array<wchar_t> str(new wchar_t[len]);
            GetWindowText(Handle(), str.get(), len);
            return std::wstring(str.get());
        }
        return L"";
    }

    void BaseWindow::Resize(const SizeInt &newSize) {
        RECT rect;
        GetWindowRect(Handle(), &rect);
        POINT pt = {rect.left, rect.top};

        if (m_pParent) ScreenToClient(Handle(), &pt);

        m_isMoveByUser = false;
        ::MoveWindow(Handle(), pt.x, pt.y, newSize.Width, newSize.Height, true);
    }

    void BaseWindow::Move(const PointInt &newPos) {
        m_isMoveByUser = false;
        ::MoveWindow(Handle(), newPos.X, newPos.Y, Width(), Height(), true);
    }

    void BaseWindow::MoveResize(const PointInt &newPos, const SizeInt &newSize) {
        m_isMoveByUser = false;
        ::MoveWindow(Handle(), newPos.X, newPos.Y, newSize.Width, newSize.Height, true);
        ::UpdateWindow(Handle());
    }

    PointInt BaseWindow::MouseCursorPos() {
        POINT pt;
        GetCursorPos(&pt);
        return PointInt(pt.x, pt.y);
    }


    void BaseWindow::MoveResize(const RectInt &newRect) {
        MoveResize(newRect.TopLeft(), newRect.Dimensions());
    }

    bool BaseWindow::DisplayContextMenu(DWORD idMenu, const PointInt &pos, bool absoluteCoords) {
        HMENU hMenu = LoadMenu(m_hInstance, MAKEINTRESOURCE(idMenu));
        HMENU hPopup = GetSubMenu(hMenu, 0);
        OnCreateContextMenu(hMenu);

        // Draw and track the shortcut menu.
        if (!absoluteCoords) ClientToScreen(Handle(), (LPPOINT) &pos);

        TrackPopupMenu(hPopup, TPM_LEFTALIGN | TPM_LEFTBUTTON, pos.X, pos.Y, 0, Handle(), NULL);
        DestroyMenu(hMenu);
        return true;
    }

    bool BaseWindow::DisplayContextMenu(HMENU hMenu, const PointInt &pos, bool absoluteCoords) {
        if (!absoluteCoords)
            ClientToScreen(m_hwnd, (LPPOINT) &pos);

        TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, pos.X, pos.Y, 0, Handle(), NULL);
        return true;
    }

    void BaseWindow::QueueMessage(unsigned message, WPARAM wParam, LPARAM lParam) {
        PostMessage(Handle(), message, wParam, lParam);
    }

    void BaseWindow::UpdateCaption() {
        if (m_sidCaption != -1)
            Caption(UTF8ToWString(Intl::GetString(m_sidCaption)));
    }

    const std::wstring BaseWindow::OpenFileDialog(const wchar_t *title, const wchar_t *filters, size_t defaultFilterIndex) {
        const int MaxPath = 4096;
        wchar_t filename[MaxPath] = L"";
        OPENFILENAME ofn;

        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = Handle();
        ofn.hInstance = m_hInstance;
        ofn.lpstrFilter = filters;
        ofn.nFilterIndex = static_cast<DWORD>(defaultFilterIndex);
        ofn.lpstrFile = filename;
        ofn.nMaxFile = MaxPath;
        ofn.lpstrTitle = title;
        ofn.Flags = OFN_HIDEREADONLY;

        BOOL ret = GetOpenFileName(&ofn);

        SetCurrentDirectory(IO::GetPath(UTF8ToWString(Sys::Info::ExePath())).c_str());

        if (ret) return filename;
        return L"";
    }

    bool BaseWindow::OnClose() {
        return PerformOnClose();
    }

    bool BaseWindow::OnPaint() {
        return PerformOnPaint();
    }

    bool BaseWindow::OnEraseBackground(HDC hdc) {
        return PerformOnEraseBackground(hdc);
    }

    bool BaseWindow::OnContextMenu(const PointInt &pt) {
        return PerformOnContextMenu(pt);
    }

    bool BaseWindow::Onchar(uint32_t key, int count, bool pressed) {
        return PerformOnchar(key, count, pressed);
    }

    bool BaseWindow::OnCommand(int id, WPARAM wp, LPARAM lp) {
        if (lp == 0)
            return PerformOnCommand(id, wp, lp);

        switch (HIWORD(wp)) {
            case BN_CLICKED:
                OnButtonClicked(LOWORD(wp));
                return true;

            case CBN_SELENDOK:
                OnComboSelectionChanged(LOWORD(wp));
                return true;
        }
        return false;
    }

    bool BaseWindow::OnSysCommand(int id, LPARAM lp) {
        // I'm starting to hate all the quirky shit in windows.
        return (id == SC_KEYMENU && lp == 0x0d);
    }

    bool BaseWindow::OnMove(const PointInt &pt) {
        bool ret = PerformOnMove(pt, m_isMoveByUser);
        m_isMoveByUser = true;
        return ret;
    }

    bool BaseWindow::OnSize(const SizeInt &sz) {
        return PerformOnSize(sz);
    }

    bool BaseWindow::OnHScroll(WPARAM wp, LPARAM lp) {
        return PerformOnHScroll(wp, lp);
    }

    bool BaseWindow::OnVScroll(WPARAM wp, LPARAM lp) {
        return PerformOnVScroll(wp, lp);
    }

    bool BaseWindow::OnNotificationAreaIconClick(size_t id) {
        return PerformOnNotificationAreaIconClick(id);
    }

    bool BaseWindow::OnApp(int index, WPARAM wp, LPARAM lp) {
        return PerformOnApp(index, wp, lp);
    }

    bool BaseWindow::OnCopyData(const COPYDATASTRUCT *pcds) {
        return PerformOnCopyData(pcds);
    }

    void BaseWindow::OnCreateContextMenu(HMENU hMenu) {
        PerformOnCreateContextMenu(hMenu);
    }

    bool BaseWindow::m_exceptionOcurred = false;
    std::wstring BaseWindow::m_exceptionDescription;

    void BaseWindow::performRedraw() {
        if (m_hwnd)
            RedrawWindow(m_hwnd, 0, 0, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
    }

    LRESULT CALLBACK BaseWindow::WndProcDelegate(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        std::lock_guard<std::recursive_mutex> l(g_mut);
        if (msg == WM_NCCREATE) {
            LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
            BaseWindow *pWin = reinterpret_cast<BaseWindow *>(lpcs->lpCreateParams);

            SetProp(hwnd, ClassPtrProp, reinterpret_cast<HANDLE>(pWin));
            pWin->m_hwnd = hwnd;
        }
        else if (msg == WM_INITDIALOG) {
            // Slightly different behavior for dialogs.

            BaseWindow *pWin = reinterpret_cast<BaseWindow *>(lParam);
            SetProp(hwnd, ClassPtrProp, reinterpret_cast<HANDLE>(pWin));

            pWin->m_hwnd = hwnd;
        }

        {
            BaseWindow *pWin = reinterpret_cast<BaseWindow *>(GetProp(hwnd, ClassPtrProp));

            if (pWin != 0) {
                // 64-bit windows has a tendency to eat exceptions in WndProc, so we'll need to be take the long route.
                try {
                    return pWin->wndProc(hwnd, msg, wParam, lParam);
                }
                catch (Err::Exception &e) {
					pWin->m_exceptionDescription = UTF8ToWString(e.what());
                    pWin->m_exceptionOcurred = true;
                    PostQuitMessage(0);
                }
                catch (...) {
                    pWin->m_exceptionDescription = L"Unknown Exception";
                    pWin->m_exceptionOcurred = true;
                    PostQuitMessage(0);
                }
            }
        }

        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    // Shared calls
    LRESULT BaseWindow::baseWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, bool doDefProc) {
        if ((msg >= WM_APP) && (msg < WM_APP + 16)) if (OnApp(msg - WM_APP, wParam, lParam)) return 0;

        switch (msg) {
            case WWM_REGISTERMODELESS:
            case WWM_UNREGISTERMODELESS:
                if (Parent() == 0) break;
                return SendMessage(Parent()->Handle(), msg, wParam, lParam);

            case WWM_NOTIFICATIONAREAICON_CLICK:
                if ((lParam == WM_LBUTTONUP) && (OnNotificationAreaIconClick(wParam)))
                    return 0;
                break;

            case WM_XBUTTONDOWN:
            case WM_XBUTTONUP:
            case WM_XBUTTONDBLCLK:
            case WM_LBUTTONUP:
            case WM_MBUTTONUP:
            case WM_RBUTTONUP:
            case WM_LBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_LBUTTONDBLCLK:
            case WM_MBUTTONDBLCLK:
            case WM_RBUTTONDBLCLK:
            case WM_MOUSEMOVE: {
                boost::optional<bool> rcv;
                MouseEvent e(lParam);
                e.Button = getButton(wParam);
                switch (msg) {
                    case WM_XBUTTONUP:
                    case WM_LBUTTONUP:
                    case WM_MBUTTONUP:
                    case WM_RBUTTONUP:
                        if ((rcv = OnMouseButtonUp(e)) && *rcv) return 0;
                        break;
                    case WM_LBUTTONDOWN:
                    case WM_MBUTTONDOWN:
                    case WM_RBUTTONDOWN:
                    case WM_XBUTTONDOWN:
                        if ((rcv = OnMouseButtonDown(e)) && *rcv) return 0;
                        break;
                    case WM_LBUTTONDBLCLK:
                    case WM_MBUTTONDBLCLK:
                    case WM_RBUTTONDBLCLK:
                    case WM_XBUTTONDBLCLK:
                        if ((rcv = OnMouseButtonDoubleClick(e)) && *rcv) return 0;
                        break;
                    case WM_MOUSEMOVE:
                        if ((rcv = OnMouseMove(e)) && *rcv) return 0;
                        break;
                }
                if (Parent()) {
                    PointInt p = PositionToParent();
                    LPARAM newLP = ((e.Position.Y + p.Y) << 16) + e.Position.X + p.X;
                    return SendMessage(Parent()->Handle(), msg, wParam, newLP);
                }
            }
                break;
            case WM_MOUSEWHEEL:
            case WM_MOUSEHWHEEL: {
                MouseEvent e(lParam);
                e.Button = getButton(wParam);
                e.WheelTicks = GET_WHEEL_DELTA_WPARAM(wParam);
                if (OnMouseWheel(e)) return 0;
                if (Parent()) {
                    return SendMessage(Parent()->Handle(), msg, wParam, lParam);
                }
            }
            case WM_COMMAND: {
                if (OnCommand(LOWORD(wParam), wParam, lParam)) return 0;
                break;
            }
            case WM_SYSCOMMAND: {
                if (OnSysCommand(wParam, lParam)) return 0;
                break;
            }
            case WM_HSCROLL:
                if (OnHScroll(wParam, lParam)) return 0;
                break;
            case WM_VSCROLL:
                if (OnVScroll(wParam, lParam)) return 0;
                break;

            case WM_COPYDATA:
                if (OnCopyData((COPYDATASTRUCT *) lParam)) return 0;
                break;

            case WM_CLOSE:
                if (!OnClose()) return 0;  // Abort window closing
                break;

            case WM_CHAR:
                if (Onchar(static_cast<uint32_t>(wParam), LOWORD(lParam), (lParam & 0x80000000) != 0)) return 0;
                break;

            case WM_DESTROY:
                EnumProps(hwnd, DelPropProc);
                break;
        }
        if (!doDefProc) return 0;                            // Dialogs should return 0
        return DefWindowProc(hwnd, msg, wParam, lParam);    // Windows should run the default wndproc
    }

    MouseButton BaseWindow::getButton(WPARAM wParam) {
        if (wParam & MK_LBUTTON) return Left;
        else if (wParam & MK_RBUTTON) return Right;
        else if (wParam & MK_MBUTTON) return Middle;
        else if (wParam & MK_XBUTTON1) return Extra1;
        else if (wParam & MK_XBUTTON2) return Extra2;
        return NoButton;
    }

    BOOL CALLBACK BaseWindow::DelPropProc(HWND hwnd, LPCTSTR propName, HANDLE) {
        RemoveProp(hwnd, propName);
        return TRUE;
    }

    const Geom::RectInt BaseWindow::WindowRect() const {
        RECT winapiRect;
        GetWindowRect(Handle(), &winapiRect);
        return RECTToRect(winapiRect);
    }

    const Geom::SizeInt BaseWindow::GetSize() const {
        return WindowRect().Dimensions();
    }

    void BaseWindow::ShowCursor(bool doShow) {
        if (doShow && !m_isCursorVisible)
            ::ShowCursor(true);
        else if (!doShow && m_isCursorVisible)
            ::ShowCursor(false);

        m_isCursorVisible = doShow;
    }

    bool BaseWindow::UnhandledExceptionOcurred() {
        return m_exceptionOcurred;
    }

    std::wstring BaseWindow::UnhandledExceptionDescription() {
        return m_exceptionDescription;
    }

    void BaseWindow::Update() {
        if (m_hwnd)
            RedrawWindow(m_hwnd, 0, 0, RDW_INTERNALPAINT | RDW_ALLCHILDREN);
    }

    void BaseWindow::OnButtonClicked(int id) {
        CreateButton(id)->OnClick();
    }

    void BaseWindow::OnComboSelectionChanged(int id) {
        CreateComboBox(id)->OnSelectionChanged();
    }

    void BaseWindow::Close() {
        PostMessage(Handle(), WM_CLOSE, 0, 0);
    }

    const int BaseWindow::Width() const {
        return GetSize().Width;
    }

    const int BaseWindow::Height() const {
        return GetSize().Height;
    }

    HINSTANCE &BaseWindow::GetHINSTANCE() {
        return m_hInstance;
    }

    bool BaseWindow::CursorVisible() {
        return m_isCursorVisible;
    }

    void BaseWindow::Handle(HWND hwnd) {
        m_hwnd = hwnd;
    }

    void BaseWindow::Parent(Win::BaseWindow *pParent) {
        m_pParent = pParent;
    }

    bool BaseWindow::PerformOnClose() {
        return true;
    }

    bool BaseWindow::PerformOnPaint() {
        return false;
    }

    bool BaseWindow::PerformOnEraseBackground(HDC hdc) {
        hdc;
        return false;
    }

    bool BaseWindow::PerformOnContextMenu(const Geom::PointInt &pt) {
        pt;
        return false;
    }

    bool BaseWindow::PerformOnchar(uint32_t key, int count, bool pressed) {
        (key, count, pressed);
        return false;
    }

    bool BaseWindow::PerformOnCommand(int id, WPARAM wp, LPARAM lp) {
        (id, wp, lp);
        return false;
    }

    bool BaseWindow::PerformOnMove(const Geom::PointInt &pt, bool isByUser) {
        (pt, isByUser);
        return false;
    }

    bool BaseWindow::PerformOnSize(const Geom::SizeInt &sz) {
        sz;
        return false;
    }

    bool BaseWindow::PerformOnHScroll(WPARAM wp, LPARAM lp) {
        (wp, lp);
        return false;
    }

    bool BaseWindow::PerformOnVScroll(WPARAM wp, LPARAM lp) {
        (wp, lp);
        return false;
    }

    bool BaseWindow::PerformOnNotificationAreaIconClick(size_t id) {
        (id);
        return false;
    }

    bool BaseWindow::PerformOnApp(int index, WPARAM wp, LPARAM lp) {
        (index, wp, lp);
        return false;
    }

    bool BaseWindow::PerformOnCopyData(const COPYDATASTRUCT *pcds) {
        pcds;
        return false;
    }

    void BaseWindow::PerformOnCreateContextMenu(HMENU hMenu) {
        hMenu;
    }
}
