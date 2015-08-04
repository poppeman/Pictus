#include "StdAfx.h"
#include "viewer_keyboard.h"
#include "viewer.h"
#include "registry.h"

namespace App {

	void ViewerKeyboard::Construct(Viewer* owner) {
		owner->OnKeyDown.connect([&](Win::KeyEvent e) { return m_keyMap.Execute(e); });

		/*AddMapping(VK_F2, [=]() { owner->RenameCurrent(); });
		AddMapping(VK_ESCAPE, [=]() { owner->Close(); });

		KeyActionMap::Function_Type ZoomIn = [=]() { owner->ZoomIn(); };
		AddMapping(VK_OEM_PLUS, ZoomIn);
		AddMapping(VK_ADD, ZoomIn);
		AddMapping(VK_NUMPAD0, ZoomIn);

		KeyActionMap::Function_Type ZoomOut = [=]() { owner->ZoomOut(); };
		AddMapping(VK_OEM_MINUS, ZoomOut);
		AddMapping(VK_SUBTRACT, ZoomOut);
		AddMapping(VK_NUMPAD1, ZoomOut);

		AddMapping('0', [=]() { owner->ZoomMode(App::ZoomMode(Reg::Key(Reg::Keys::DWDefaultZoomMode))); });
		AddMapping('1', [=]() { owner->ZoomMode(ZoomFullSize); });

		AddMapping(VK_MULTIPLY, [=]() { owner->ZoomMode(ZoomFree); });
		AddMapping(VK_UP, [=]() { owner->PanUp(); });
		AddMapping(VK_LEFT, [=]() { owner->PanLeft(); });
		AddMapping(VK_DOWN, [=]() { owner->PanDown(); });
		AddMapping(VK_RIGHT, [=]() { owner->PanRight(); });
		AddMapping(VK_HOME, [=]() { owner->ToStart(); });
		AddMapping(VK_END, [=]() { owner->ToEnd(); });

		KeyActionMap::Function_Type ImageNext = [=]() { owner->ImageNext(1); };
		KeyActionMap::Function_Type ImageNextSkip = [=]() { owner->ImageNext(Viewer::ShiftSkipLength); };
		AddMapping(VK_SPACE, ImageNext);
		AddMapping(VK_NEXT, ImageNext);
		AddMapping(VK_SPACE, ImageNextSkip, Shift);
		AddMapping(VK_NEXT, ImageNextSkip, Shift);
		AddMapping(VK_RIGHT, ImageNext, Alt);

		KeyActionMap::Function_Type ImagePrev = [=]() { owner->ImagePrev(1); };
		KeyActionMap::Function_Type ImagePrevSkip = [=]() { owner->ImagePrev(Viewer::ShiftSkipLength); };
		AddMapping(VK_BACK, ImagePrev);
		AddMapping(VK_PRIOR, ImagePrev);
		AddMapping(VK_BACK, ImagePrevSkip, Shift);
		AddMapping(VK_PRIOR, ImagePrevSkip, Shift);
		AddMapping(VK_LEFT, ImagePrev, Alt);


		AddMapping(VK_DELETE, [=]() { owner->RemoveImage(Viewer::RemoveRecycle); });
		AddMapping(VK_DELETE, [=]() { owner->RemoveImage(Viewer::RemoveDelete); }, Shift);
		AddMapping(VK_DELETE, [=]() { owner->RemoveImage(Viewer::RemoveOnly); }, Ctrl);

		KeyActionMap::Function_Type ImageRandom = [=]() { owner->ImageRandom(); };
		AddMapping('R', ImageRandom);
		AddMapping('r', ImageRandom);*/

		KeyActionMap::Function_Type ShowSettings = [=]() { owner->ShowSettings(); };
		AddMapping('O', ShowSettings);
		AddMapping('o', ShowSettings);

		//AddMapping(VK_RETURN, [=]() { owner->ToggleFullscreenMode(); }, Alt);
	}

	void ViewerKeyboard::AddMapping(WPARAM key, KeyActionMap::Function_Type f, int flags) {
		m_keyMap.AddAction(Win::KeyEvent(
			key,
			(flags & Alt) != 0,
			(flags & Ctrl) != 0,
			(flags & Shift) != 0),
			f);
	}
}
