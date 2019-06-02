#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Window.h"
#include "SDL/include/SDL.h"
//#include "SDL/include/SDL_gamecontroller.h"

#define MAX_KEYS 300

j1Input::j1Input() : j1Module()
{
	name.assign("input");

	keyboard = DBG_NEW j1KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(j1KeyState) * MAX_KEYS);
	mouse_buttons = DBG_NEW j1KeyState[NUM_MOUSE_BUTTONS];
	memset(mouse_buttons, KEY_IDLE, sizeof(j1KeyState) * NUM_MOUSE_BUTTONS);
	controller = DBG_NEW j1KeyState[SDL_CONTROLLER_BUTTON_MAX];
	memset(controller, KEY_IDLE, sizeof(j1KeyState) * SDL_CONTROLLER_BUTTON_MAX);
	controller_axis = DBG_NEW j1KeyState[SDL_CONTROLLER_AXIS_MAX];
	memset(controller_axis, KEY_IDLE, sizeof(j1KeyState)* SDL_CONTROLLER_AXIS_MAX);

	joystick = DBG_NEW j1KeyState[(int)j1JoyStickSide::JOY_MAX * (int)j1JoyDir::JOYSTICK_DIR_MAX];
	memset(joystick, KEY_IDLE, sizeof(j1KeyState) * ((int)j1JoyStickSide::JOY_MAX * (int)j1JoyDir::JOYSTICK_DIR_MAX));
}

// Destructor
j1Input::~j1Input()
{
	delete[] keyboard;
	delete[] mouse_buttons;
	delete[] controller;
	delete[] controller_axis;
	delete[] joystick;
}

// Called before render is available
bool j1Input::Awake(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	// gamepad controller ----
	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0) {
		LOG("SDL_GAMECONTROLLER could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	if (SDL_InitSubSystem(SDL_INIT_HAPTIC) < 0)
	{
		LOG("SDL_GAMECONTROLLER HAPTIC could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	// ------------------------

	return ret;
}

// Called before the first frame
bool j1Input::Start()
{

	SDL_StopTextInput();
	return true;
}

// Called each loop iteration
bool j1Input::PreUpdate()
{
	static SDL_Event event;

	// gamepad input -------------------------------

	// BUTTONS
	for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i) {
		if (SDL_GameControllerGetButton(gamePad1, (SDL_GameControllerButton)i) == 1) {
			if (controller[i] == KEY_IDLE) {
				controller[i] = KEY_DOWN;
			}
			else {
				controller[i] = KEY_REPEAT;
			}
		}
		else
		{
			if (controller[i] == KEY_REPEAT || controller[i] == KEY_DOWN) {
				controller[i] = KEY_UP;
			}
			else {
				controller[i] = KEY_IDLE;
			}
		}
	}
	// TRIGGERS/JOYSTICKS GENERAL STATE (for joysticks better use GetJoystickPulsation to differentiate directions
	// for triggers this is good
	for (int i = 0; i < SDL_CONTROLLER_AXIS_MAX; ++i)
	{
		//if (SDL_GameControllerGetAxis(gamePad1, (SDL_GameControllerAxis)i) > 0)
		if(GetControllerAxis((SDL_GameControllerAxis)i)) // filtered with dead zone too
		{
			if (controller_axis[i] == KEY_IDLE)
				controller_axis[i] = KEY_DOWN;
			else
				controller_axis[i] = KEY_REPEAT;
		}
		else
		{
			if (controller_axis[i] == KEY_REPEAT || controller_axis[i] == KEY_DOWN)
				controller_axis[i] = KEY_UP;
			else
				controller_axis[i] = KEY_IDLE;
		}
		
	}

	// joystick axis pulsation -----------------------------------------------
	/* Original order of sdl_gamecontrolleraxis enum
	SDL_CONTROLLER_AXIS_INVALID = -1,

	SDL_CONTROLLER_AXIS_LEFTX,
	SDL_CONTROLLER_AXIS_LEFTY,
	SDL_CONTROLLER_AXIS_RIGHTX,
	SDL_CONTROLLER_AXIS_RIGHTY,

	SDL_CONTROLLER_AXIS_TRIGGERLEFT,
	SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
	SDL_CONTROLLER_AXIS_MAX */

	int axis_direction_counter = 0; // relative to joydir enum order, this is good because we only "track" pairs for each dir (+/-)
	for (int i = SDL_CONTROLLER_AXIS_LEFTX; i < SDL_CONTROLLER_AXIS_TRIGGERLEFT; ++i)
	{
		// we need to track for each axis two directions -/+
		Sint16 joy_value = GetControllerAxis((SDL_GameControllerAxis)i); // filtered with dead zone too
		
		// negative values for "this" axis, ie: SDL_CONTROLLER_AXIS_LEFTX, directly to joydir enum order, dir "left"
		// ie: SDL_CONTROLLER_AXIS_RIGHTX, axis direction counter = 2, pointing to joydir UP, wich needs a negative value from getaxis
		// etc

		if (joy_value < 0) 
		{
			if (joystick[axis_direction_counter] == KEY_IDLE)
				joystick[axis_direction_counter] = KEY_DOWN;
			else
				joystick[axis_direction_counter] = KEY_REPEAT;
		}
		else
		{
			if (joystick[axis_direction_counter] == KEY_REPEAT || joystick[axis_direction_counter] == KEY_DOWN)
				joystick[axis_direction_counter] = KEY_UP;
			else
				joystick[axis_direction_counter] = KEY_IDLE;
		}

		++axis_direction_counter;

		// positive values  for "this" axis, ie: SDL_CONTROLLER_AXIS_LEFTX, dir "right" (axis direction counter 1)
		if (joy_value > 0)
		{
			if (joystick[axis_direction_counter] == KEY_IDLE)
				joystick[axis_direction_counter] = KEY_DOWN;
			else
				joystick[axis_direction_counter] = KEY_REPEAT;
		}
		else
		{
			if (joystick[axis_direction_counter] == KEY_REPEAT || joystick[axis_direction_counter] == KEY_DOWN)
				joystick[axis_direction_counter] = KEY_UP;
			else
				joystick[axis_direction_counter] = KEY_IDLE;
		}

		++axis_direction_counter;
	}

	// --------------------------------------------------------------
	
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for(int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if(mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if(mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}

	while(SDL_PollEvent(&event) != 0)
	{
		switch(event.type)
		{
			case SDL_QUIT:
				windowEvents[WE_QUIT] = true;
			break;

			case SDL_WINDOWEVENT:
				switch(event.window.event)
				{
					//case SDL_WINDOWEVENT_LEAVE:
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_MINIMIZED:
					case SDL_WINDOWEVENT_FOCUS_LOST:
					windowEvents[WE_HIDE] = true;
					break;

					//case SDL_WINDOWEVENT_ENTER:
					case SDL_WINDOWEVENT_SHOWN:
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_MAXIMIZED:
					case SDL_WINDOWEVENT_RESTORED:
					windowEvents[WE_SHOW] = true;
					break;
				}
			break;

			case SDL_MOUSEBUTTONDOWN:
				mouse_buttons[event.button.button - 1] = KEY_DOWN;
				//LOG("Mouse button %d down", event.button.button-1);
			break;

			case SDL_MOUSEBUTTONUP:
				mouse_buttons[event.button.button - 1] = KEY_UP;
				//LOG("Mouse button %d up", event.button.button-1);
			break;

			case SDL_MOUSEMOTION:
			{
				int scale = App->win->GetScale();
				mouse_motion_x = event.motion.xrel / scale;
				mouse_motion_y = event.motion.yrel / scale;
				mouse_x = event.motion.x / scale;
				mouse_y = event.motion.y / scale;
				//LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);
			}
			break;

			case SDL_CONTROLLERDEVICEADDED:
			{
				//Open the first available controller
				for (int i = 0; i < SDL_NumJoysticks(); ++i) {
					if (SDL_IsGameController(i)) {
						gamePad1 = SDL_GameControllerOpen(i);
						if (gamePad1) {

							if (SDL_JoystickIsHaptic(SDL_GameControllerGetJoystick(gamePad1)) > 0)
							{
								haptic = SDL_HapticOpenFromJoystick(SDL_GameControllerGetJoystick(gamePad1));

								if (haptic != nullptr)
								{
									LOG("HAPTIC SUCCESS");
									//Get initialize rumble 
									if (SDL_HapticRumbleInit(haptic) < 0) // initialize simple rumble
									{
										LOG("Warning: Unable to initialize rumble! SDL Error: %s\n", SDL_GetError());
									}

									if (SDL_HapticRumblePlay(haptic, 0.3f, 1000) < 0)
									{
										LOG("rumble play error");
									}
								}
							}
							else
							{
								LOG("haptic error! SDL_Error: %s\n", SDL_GetError());
							}
						}
						else {
							LOG("gamepad awake assign failed");
						}
					}
				}
				break;
			}

			case SDL_CONTROLLERDEVICEREMOVED:
				LOG("disconnected gamepad");
				if (gamePad1 != nullptr)
				{
					SDL_HapticClose(haptic);
					haptic = nullptr;
					SDL_GameControllerClose(gamePad1);
					gamePad1 = nullptr;
					break;
				}
		}
	}

	CheckGamepadWTFPressedInput();

	return true;
}

// Called before quitting
bool j1Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	SDL_HapticStopAll(haptic);
	SDL_HapticClose(haptic);
	SDL_QuitSubSystem(SDL_INIT_HAPTIC);
	//SDL_GameControllerClose(gamePad1);
	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
	
	return true;
}

// ---------
bool j1Input::GetWindowEvent(j1EventWindow ev)
{
	return windowEvents[ev];
}

void j1Input::GetMousePosition(int& x, int& y)
{
	x = mouse_x;
	y = mouse_y;
}

void j1Input::GetMouseMotion(int& x, int& y)
{
	x = mouse_motion_x;
	y = mouse_motion_y;
}

Sint16 j1Input::GetControllerAxis(SDL_GameControllerAxis axis) {
	Sint16 axisValue = SDL_GameControllerGetAxis(gamePad1, axis);
	//Avoid clunky zone for older or used gamepads precision
	if (axisValue < -DEAD_ZONE || axisValue > DEAD_ZONE) {
		return axisValue;
	}
	return 0;
}


uint j1Input::GetCurrentMouseButtonDown()
{
	for (uint iter = 0; iter < NUM_MOUSE_BUTTONS; iter++)
	{
		if (mouse_buttons[iter] == KEY_DOWN)
		{
			return iter + 1;
		}
	}
	return 0;
}

void j1Input::DoGamePadRumble(float strength, uint32 duration) const
{
	//SDL_GameControllerRumble() // TO bypass haptic etc and do the order directly to gamecontroller (for simple rumble)
	// for simple rumble too (for now)
	SDL_HapticRumblePlay(haptic, strength, duration);

}

j1KeyState j1Input::GetJoystickPulsation(j1JoyStickSide joystickSide, j1JoyDir joyButtonDir) const
{
	return joystick[(int)joystickSide * ((int)j1JoyDir::JOYSTICK_DIR_MAX) + (int)joyButtonDir];
}

// testing
ControllerPressData j1Input::CheckGamepadWTFPressedInput()
{
	bool detectedPress = false;
	bool isButton = false;
	ControllerPressData ret;

	// iterate throught all gamepad buttons and axis, on the first we found "down", return
	// axis (this way only checks correctly the triggers, no joystick mapping is needed for remap player scheme)
	int i = 0;
	for (; i < SDL_CONTROLLER_AXIS_MAX; ++i)
	{
		if (controller_axis[i] == KEY_DOWN)
		{
			detectedPress = true;
			break;
		}
	}

	// buttons
	if (detectedPress != true)
	{
		i = 0;
		for (; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
		{
			if (controller[i] == KEY_DOWN)
			{
				detectedPress = true;
				isButton = true;
				break;
			}
		}
	}

	if (detectedPress)
	{
		//ret.pressed = true;

		if (!isButton)
		{
			ret.axis = SDL_GameControllerAxis(i);
			LOG("axis pressed found");
		}
		else
		{
			//ret.isButton = true;
			ret.button = SDL_GameControllerButton(i);
			LOG("button pressed found");
		}
	}

	return ret;
}

j1KeyState j1Input::GetControllerGeneralPress(ControllerPressData mappedButtonData) const // priority to assigned buttons first
{
	if (mappedButtonData.button != -1)
		return controller[mappedButtonData.button];
	else if (mappedButtonData.axis != -1)
		return controller_axis[mappedButtonData.axis];
	else
		return j1KeyState::KEY_IDLE;
}