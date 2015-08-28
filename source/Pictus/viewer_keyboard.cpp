#include "StdAfx.h"
#include "viewer_keyboard.h"
#include "viewer.h"
#include "registry.h"

namespace App {
	void ViewerKeyboard::Construct(Viewer* owner, Reg::KeyboardSettings cfg) {
		m_cfg = cfg;

		owner->OnKeyDown.connect([=](Win::KeyEvent e) {
			for (auto x : m_cfg.Bindings) {
				if (x.Key == e.Key && x.Alt == e.AltPressed && x.Shift == e.ShiftPressed && x.Ctrl == e.CtrlPressed) {
					switch (x.Action) {
					case KeyAction::FirstImage:
						owner->ToStart();
						break;
					case KeyAction::LastImage:
						owner->ToEnd();
						break;;
					case KeyAction::NextImage:
						owner->ImageNext();
						break;
					case KeyAction::NextSkipImage:
						owner->ImageNext(Viewer::ShiftSkipLength);
						break;
					case KeyAction::PreviousImage:
						owner->ImagePrev();
						break;
					case KeyAction::PreviousSkipImage:
						owner->ImagePrev(Viewer::ShiftSkipLength);
						break;
					case KeyAction::PanDown:
						owner->PanDown();
						break;
					case KeyAction::PanUp:
						owner->PanUp();
						break;
					case KeyAction::PanLeft:
						owner->PanLeft();
						break;
					case KeyAction::PanRight:
						owner->PanRight();
						break;
					case KeyAction::RandomImage:
						owner->ImageRandom();
						break;
					case KeyAction::RecycleFile:
						owner->RemoveImage(Viewer::RemoveRecycle);
						break;
					case KeyAction::RemoveImage:
						owner->RemoveImage(Viewer::RemoveOnly);
						break;
					case KeyAction::RenameFile:
						owner->RenameCurrent();
						break;
					case KeyAction::ToggleFullscreen:
						owner->ToggleFullscreenMode();
						break;
					case KeyAction::ZoomDefault:
						owner->ZoomDefault();
						break;
					case KeyAction::ZoomFree:
						owner->ZoomMode(ZoomMode::ZoomFree);
						break;
					case KeyAction::ZoomFull:
						owner->ZoomMode(ZoomMode::ZoomFullSize);
						break;
					case KeyAction::ZoomIn:
						owner->ZoomIn();
						break;
					case KeyAction::ZoomOut:
						owner->ZoomOut();
						break;
					case KeyAction::CloseApplication:
						owner->Close();
						break;
					case KeyAction::OpenSettings:
						owner->ShowSettings();
						break;
					}
					return true;
				}
			}
			return false;
		});
	}
}
