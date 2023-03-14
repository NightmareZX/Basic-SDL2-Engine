#pragma once
#include "SDL.h"
#include "STL_Components.h"

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
};

class EventHandler
{
private:
	EventHandler();
	static EventHandler* sInstance;
	const Uint8* mKeyStates;
	map<RegisteredAction, SDL_Scancode> mKeyBindings;
public:
	inline static EventHandler* GetInstance() { if (sInstance == nullptr) sInstance = new EventHandler(); return sInstance; }
	void ListenForEvents();
	void KeyPressed();
	void KeyReleased();
	bool GetAction(RegisteredAction action);
};

