#include "EventHandler.h"
#include "GameEngine.h"

EventHandler* EventHandler::sInstance = nullptr;
EventHandler::EventHandler()
{
	mKeyStates = SDL_GetKeyboardState(nullptr);
	mKeyBindings[MOVE_LEFT] = SDL_Scancode::SDL_SCANCODE_LEFT;
	mKeyBindings[MOVE_RIGHT] = SDL_Scancode::SDL_SCANCODE_RIGHT;
	mKeyBindings[JUMP] = SDL_Scancode::SDL_SCANCODE_A;
	mKeyBindings[DOWN] = SDL_Scancode::SDL_SCANCODE_DOWN;
}

void EventHandler::ListenForEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_EventType::SDL_QUIT: GameEngine::GetInstance()->QuitEngine(); break;
	case SDL_EventType::SDL_KEYDOWN: KeyPressed(); break;
	case SDL_EventType::SDL_KEYUP: KeyReleased(); break;
	default:
		break;
	}
}
bool EventHandler::GetAction(RegisteredAction action)
{
	SDL_Scancode keyToCheck = mKeyBindings[action];
	if (mKeyStates[keyToCheck] == 1)
		return true;
	return false;
}

void EventHandler::KeyPressed(){ mKeyStates = SDL_GetKeyboardState(nullptr);}
void EventHandler::KeyReleased(){ mKeyStates = SDL_GetKeyboardState(nullptr); }