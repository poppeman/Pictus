#include "StdAfx.h"
#include "viewer_keyboard.h"
#include "viewer.h"
#include "registry.h"

namespace App {
	void ViewerKeyboard::Construct(Viewer* owner) {
		m_owner = owner;

		m_owner->OnKeyDown.connect([&](Win::KeyEvent e) {
			for (auto x : m_cfg.Bindings) {
				if (x.Key.Key == e.Key && x.Key.Alt == e.AltPressed && x.Key.Shift == e.ShiftPressed && x.Key.Ctrl == e.CtrlPressed) {
					switch (x.Action) {
					case KeyAction::FirstImage:
						m_owner->ToStart();
						break;
					case KeyAction::LastImage:
						m_owner->ToEnd();
						break;;
					case KeyAction::NextImage:
						m_owner->ImageNext();
						break;
					case KeyAction::NextSkipImage:
						m_owner->ImageNext(Viewer::ShiftSkipLength);
						break;
					case KeyAction::PreviousImage:
						m_owner->ImagePrev();
						break;
					case KeyAction::PreviousSkipImage:
						m_owner->ImagePrev(Viewer::ShiftSkipLength);
						break;
					case KeyAction::PanDown:
						m_owner->PanDown();
						break;
					case KeyAction::PanUp:
						m_owner->PanUp();
						break;
					case KeyAction::PanLeft:
						m_owner->PanLeft();
						break;
					case KeyAction::PanRight:
						m_owner->PanRight();
						break;
					case KeyAction::RandomImage:
						m_owner->ImageRandom();
						break;
					case KeyAction::RecycleCurrentFile:
						m_owner->RemoveImage(Viewer::RemoveRecycle);
						break;
					case KeyAction::RemoveCurrentImage:
						m_owner->RemoveImage(Viewer::RemoveOnly);
						break;
					case KeyAction::RenameFile:
						m_owner->RenameCurrent();
						break;
					case KeyAction::ToggleFullscreen:
						m_owner->ToggleFullscreenMode();
						break;
					case KeyAction::ZoomDefault:
						m_owner->ZoomDefault();
						break;
					case KeyAction::ZoomFree:
						m_owner->ZoomMode(ZoomMode::ZoomFree);
						break;
					case KeyAction::ZoomFull:
						m_owner->ZoomMode(ZoomMode::ZoomFullSize);
						break;
					case KeyAction::ZoomIn:
						m_owner->ZoomIn();
						break;
					case KeyAction::ZoomOut:
						m_owner->ZoomOut();
						break;
					case KeyAction::CloseApplication:
						m_owner->Close();
						break;
					case KeyAction::OpenSettings:
						m_owner->ShowSettings();
						break;
					case KeyAction::CopyImage:
						m_owner->CopyToClipboard();
						break;
					}
					return true;
				}
			}
			return false;
		});
	}

	void ViewerKeyboard::SetBindings(Reg::KeyboardSettings cfg) {
		m_cfg = cfg;
	}
}
