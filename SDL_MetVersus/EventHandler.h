#pragma once
#include "SDL.h"
#include "Containers.h"

enum RegisteredAction
{
	MOVE_LEFT,
	MOVE_RIGHT,
	AIM_UP,
	DOWN,
	AIM_DIAGONAL_UP,
	AIM_DIAGONAL_DOWN,
	JUMP,
	PRIMARY_ATTACK,
	SECONDARY_ATTACK,
	SELECT,
	SELECT_CANCEL,
	PAUSE,
	MENU_ACCEPT,
	MENU_CANCEL,

	DEBUG_LOG_SDL_ERROR,
	DEBUG_RESTART_MUSIC,
	DEBUG_ENABLE_FRAME_ADVANCE,
	DEBUG_ADVANCE_FRAME,
};

class EventHandler final
{
private:
	const Uint8* mKeyStates;
	Uint8* mLastKeyStates;
	map<RegisteredAction, SDL_Scancode> mKeyBindings;
	bool mQuitStatus;
public:
	EventHandler();
	void ListenForEvents();
	void KeyPressed();
	void KeyReleased();
	void WindowEvent(SDL_Event event);
	bool GetAction(RegisteredAction action);
	bool GetActionOnce(RegisteredAction action);
	inline bool GetQuitStatus() { return mQuitStatus; }
};

