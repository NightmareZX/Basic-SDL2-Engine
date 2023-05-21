#include "EventHandler.h"
#include "GameEngine.h"

EventHandler* EventHandler::sInstance = nullptr;
EventHandler::EventHandler()
{
	mKeyStates = SDL_GetKeyboardState(nullptr);
	mLastKeyStates = new Uint8[512];
	for (size_t i = 0; i < 512; i++)
		mLastKeyStates[i] = 0;
	mKeyBindings[MOVE_LEFT] = SDL_Scancode::SDL_SCANCODE_LEFT;
	mKeyBindings[MOVE_RIGHT] = SDL_Scancode::SDL_SCANCODE_RIGHT;
	mKeyBindings[JUMP] = SDL_Scancode::SDL_SCANCODE_A;
	mKeyBindings[DOWN] = SDL_Scancode::SDL_SCANCODE_DOWN;
	mKeyBindings[DEBUG_LOG_SDL_ERROR] = SDL_Scancode::SDL_SCANCODE_P;
	mKeyBindings[DEBUG_RESTART_MUSIC] = SDL_Scancode::SDL_SCANCODE_K;
	mKeyBindings[DEBUG_ENABLE_FRAME_ADVANCE] = SDL_Scancode::SDL_SCANCODE_M;
	mKeyBindings[DEBUG_ADVANCE_FRAME] = SDL_Scancode::SDL_SCANCODE_B;
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
	case SDL_EventType::SDL_WINDOWEVENT: WindowEvent(event); break;
	default:
		break;
	}
}
//Returns true as long as the button is held
bool EventHandler::GetAction(RegisteredAction action)
{
	SDL_Scancode keyToCheck = mKeyBindings[action];
	if (mKeyStates[keyToCheck] == 1)
		return true;
	return false;
}
//It returns true on the first frame the button is pressed, if the button is held then it returns false
bool EventHandler::GetActionOnce(RegisteredAction action)
{
	SDL_Scancode keyToCheck = mKeyBindings[action];
	if (mKeyStates[keyToCheck] == 1 && mLastKeyStates[keyToCheck] == 0)
	{
		mLastKeyStates[keyToCheck] = 1;
		return true;
	}
	else if (mKeyStates[keyToCheck] == 1 && mLastKeyStates[keyToCheck] == 1)
	{
		return false;
	}
	mLastKeyStates[keyToCheck] = 0;
	return false;
}
void EventHandler::WindowEvent(SDL_Event event)
{
	if (event.window.event == SDL_WINDOWEVENT_RESIZED)
	{
		RenderManager::GetInstance()->SetWindowDimensions(event.window.data1, event.window.data2);
	}
}
void EventHandler::KeyPressed()
{ 
	mKeyStates = SDL_GetKeyboardState(nullptr);
}
void EventHandler::KeyReleased()
{
	mKeyStates = SDL_GetKeyboardState(nullptr);
}