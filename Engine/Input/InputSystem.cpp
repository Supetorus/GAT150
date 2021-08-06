#include "InputSystem.h"
namespace nc
{
	void InputSystem::Startup()
	{
		// get pointer to sdl keyboard states
		const Uint8* keyboardStateSDL = SDL_GetKeyboardState(&numKeys);

		// resize of keyboard state using numKeys for size
		keyboardState.resize(numKeys);

		// copy sdl keyboard state to keyboard state vector
		std::copy(keyboardStateSDL, keyboardStateSDL + numKeys, keyboardState.begin());

		// set previous keyboard state to keyboard state
		prevKeyboardState = keyboardState;

	}

	void InputSystem::Shutdown()
	{
	}

	void InputSystem::Update(float dt)
	{
		// set the previous keyboard state to keyboard state
		prevKeyboardState = keyboardState;
		// copy the current SDL keyboard state to keyboard state
		const Uint8* keyboardStateSDL = SDL_GetKeyboardState(nullptr);
		std::copy(keyboardStateSDL, keyboardStateSDL + numKeys, keyboardState.begin());

	}

	InputSystem::eKeyState InputSystem::GetKeyState(int id)
	{
		eKeyState state = eKeyState::Idle;

		bool keyDown = IsKeyDown(id);
		bool prevKeyDown = IsPreviousKeyDown(id);

		if (keyDown)
		{
			state = (prevKeyDown) ? eKeyState::Held : eKeyState::Pressed;
		}
		else
		{
			state = (prevKeyDown) ? eKeyState::Release : eKeyState::Idle;
		}

		return state;

	}

	bool InputSystem::IsKeyDown(int id)
	{
		return keyboardState[id];
	}

	bool InputSystem::IsPreviousKeyDown(int id)
	{
		return prevKeyboardState[id];
	}
}