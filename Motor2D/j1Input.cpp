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

	GenerateMapping();
	LoadGamepadMapScheme("config/controllerMapping.xml");
	GenerateGuiElemMapping();
	GenerateGuiButtonsRectMapping();

	// define desired permitted keybinding from user input GUI
	// buttons
	buttonPermittedMatrix[SDL_CONTROLLER_BUTTON_A] = true;
	buttonPermittedMatrix[SDL_CONTROLLER_BUTTON_B] = true;
	buttonPermittedMatrix[SDL_CONTROLLER_BUTTON_X] = true;
	buttonPermittedMatrix[SDL_CONTROLLER_BUTTON_Y] = true;
	buttonPermittedMatrix[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER] = true;
	buttonPermittedMatrix[SDL_CONTROLLER_BUTTON_LEFTSHOULDER] = true;
	buttonPermittedMatrix[SDL_CONTROLLER_BUTTON_RIGHTSTICK] = true;
	buttonPermittedMatrix[SDL_CONTROLLER_BUTTON_LEFTSTICK] = true;
	// axis
	axisPermittedMatrix[SDL_CONTROLLER_AXIS_TRIGGERLEFT] = true;
	axisPermittedMatrix[SDL_CONTROLLER_AXIS_TRIGGERRIGHT] = true;

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

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if (mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}

	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
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

	// testing --------------

	//ListeningInputFor("marche_dash_button");
	//ListeningInputFor("interact");
	//ListeningInputFor("swap_next");
	//ListeningInputFor("ritz_basic_button");

	//if (App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_DOWN)
	//{
	//	//gamepadScheme.marche.dodge.button = SDL_CONTROLLER_BUTTON_A;
	//	//SaveGamepadMapScheme("config/controllerMapping.xml");
	//	GetSectionForElement("marche_dash_button");
	//}

	/*if (GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		gamepadScheme.marche.dodge.button = SDL_CONTROLLER_BUTTON_A;
	}*/

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

	// free maps
	generalMapInput.clear();
	marcheMapInput.clear();
	ritzMapInput.clear();
	sharaMapInput.clear();
	characterNameToMapData.clear();
	
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
const ControllerPressData& j1Input::CheckGamepadWTFPressedInput() const
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
	if (!detectedPress)
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

bool j1Input::GenerateMapping()
{
	// character name to map data maps
	characterNameToMapData.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("marche", marcheMapInput));
	characterNameToMapData.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("ritz", ritzMapInput));
	characterNameToMapData.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("shara", sharaMapInput));
	characterNameToMapData.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("general", generalMapInput));

	// for shared buttons
	generalMapInput.insert(std::pair<std::string, ControllerPressData&>("interact", gamepadScheme.sharedInput.interact));
	generalMapInput.insert(std::pair<std::string, ControllerPressData&>("swap_next", gamepadScheme.sharedInput.swap_next));
	generalMapInput.insert(std::pair<std::string, ControllerPressData&>("swap_prev", gamepadScheme.sharedInput.swap_prev));

	// marche
	marcheMapInput.insert(std::pair<std::string, ControllerPressData&>("basic", gamepadScheme.marche.basic));
	marcheMapInput.insert(std::pair<std::string, ControllerPressData&>("dodge", gamepadScheme.marche.dodge));
	marcheMapInput.insert(std::pair<std::string, ControllerPressData&>("special1", gamepadScheme.marche.special1));
	marcheMapInput.insert(std::pair<std::string, ControllerPressData&>("special2", gamepadScheme.marche.special2));
	marcheMapInput.insert(std::pair<std::string, ControllerPressData&>("ultimate", gamepadScheme.marche.ultimate));
	marcheMapInput.insert(std::pair<std::string, ControllerPressData&>("aim", gamepadScheme.marche.aim));

	// ritz
	ritzMapInput.insert(std::pair<std::string, ControllerPressData&>("basic", gamepadScheme.ritz.basic));
	ritzMapInput.insert(std::pair<std::string, ControllerPressData&>("dodge", gamepadScheme.ritz.dodge));
	ritzMapInput.insert(std::pair<std::string, ControllerPressData&>("special1", gamepadScheme.ritz.special1));
	ritzMapInput.insert(std::pair<std::string, ControllerPressData&>("special2", gamepadScheme.ritz.special2));
	ritzMapInput.insert(std::pair<std::string, ControllerPressData&>("ultimate", gamepadScheme.ritz.ultimate));
	ritzMapInput.insert(std::pair<std::string, ControllerPressData&>("aim", gamepadScheme.ritz.aim));

	// shara
	sharaMapInput.insert(std::pair<std::string, ControllerPressData&>("basic", gamepadScheme.shara.basic));
	sharaMapInput.insert(std::pair<std::string, ControllerPressData&>("dodge", gamepadScheme.shara.dodge));
	sharaMapInput.insert(std::pair<std::string, ControllerPressData&>("special1", gamepadScheme.shara.special1));
	sharaMapInput.insert(std::pair<std::string, ControllerPressData&>("special2", gamepadScheme.shara.special2));
	sharaMapInput.insert(std::pair<std::string, ControllerPressData&>("ultimate", gamepadScheme.shara.ultimate));
	sharaMapInput.insert(std::pair<std::string, ControllerPressData&>("aim", gamepadScheme.shara.aim));

	return true;
}

bool j1Input::LoadGamepadMapScheme(const char* path)
{
	pugi::xml_document inputDoc;
	pugi::xml_node node;

	pugi::xml_parse_result result = inputDoc.load_file(path);

	if (result == NULL)
		LOG("Could not load map xml file config/controllerMapping.xml pugi error: %s", result.description());
	else
		node = inputDoc.child("controller_mapping").child("current");

	// LOAD shared scheme
	pugi::xml_node general_node = node.child("general");
	if (general_node != NULL)
	{
		for (pugi::xml_node sharedButtons = general_node.child("button"); sharedButtons; sharedButtons = sharedButtons.next_sibling("button"))
		{
			std::map<std::string, ControllerPressData&>::iterator it;
			it = generalMapInput.find(sharedButtons.attribute("function").as_string());

			if (it != generalMapInput.end()) // if found
			{
				// write values to data map
				std::string buttonType = sharedButtons.attribute("type").as_string();

				if (buttonType == "button")
					(*it).second.button = SDL_GameControllerButton(sharedButtons.attribute("id").as_int());
				else if (buttonType == "axis")
					(*it).second.axis = SDL_GameControllerAxis(sharedButtons.attribute("id").as_int());
			}
		}
	}
	
	// LOAD SPECIFIC characters schemes
	for (std::map<std::string, std::map<std::string, ControllerPressData&>&>::iterator it = characterNameToMapData.begin(); it != characterNameToMapData.end(); ++it )
	{
		pugi::xml_node charNode = node.child((*it).first.data()); // loads node with the same mapped name
		if (charNode != NULL)
		{
			for (pugi::xml_node charButtons = charNode.child("button"); charButtons; charButtons = charButtons.next_sibling("button"))
			{
				std::map<std::string, ControllerPressData&>::iterator it2;
				it2 = (*it).second.find(charButtons.attribute("function").as_string());

				if (it2 != (*it).second.end())
				{
					// write values to data map
					std::string buttonType = charButtons.attribute("type").as_string();

					if (buttonType == "button")
						(*it2).second.button = SDL_GameControllerButton(charButtons.attribute("id").as_int());
					else if (buttonType == "axis")
						(*it2).second.axis = SDL_GameControllerAxis(charButtons.attribute("id").as_int());
				}
			}
		}
	}

	return true;
}

bool j1Input::SaveGamepadMapScheme(const char* path)
{
	pugi::xml_document inputDoc;
	pugi::xml_node node;

	pugi::xml_parse_result result = inputDoc.load_file("config/controllerMapping.xml");

	if (result == NULL)
	{
		LOG("Could not load map xml file config/controllerMapping.xml pugi error: %s", result.description());
		return false;
	}
	else
		node = inputDoc.child("controller_mapping").child("current"); // only overwrite the current scheme, never the default

	// if the current node its not found, create it
	if (node == NULL)
	{
		node = inputDoc.child("controller_mapping").append_child("current");
		if (node.empty())
			return false;
	}

	// SAVE ALL input ---------------------------------------------
	// iterates all the mappings and so on
	for (std::map<std::string, std::map<std::string, ControllerPressData&>&>::iterator it = characterNameToMapData.begin(); it != characterNameToMapData.end(); ++it)
	{
		// search and remove duplicates
		if (node.child((*it).first.data()))
			node.remove_child((*it).first.data());

		// adds new node
		pugi::xml_node newButtonGroupNode = node.append_child((*it).first.data());
		
		// iterate thorught all current mapping
		for (std::map<std::string, ControllerPressData&>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2)
		{
			// new button action
			pugi::xml_node thisButton = newButtonGroupNode.append_child("button");
			thisButton.append_attribute("function") = (*it2).first.data();

			// check type of button
			if ((*it2).second.button  != -1)
			{
				thisButton.append_attribute("type") = "button";
				thisButton.append_attribute("id").set_value((*it2).second.button);
			}
			else if ((*it2).second.axis != -1)
			{
				thisButton.append_attribute("type") = "axis";
				thisButton.append_attribute("id").set_value((*it2).second.axis);
			}
		}
	}

	inputDoc.save_file("config/controllerMapping.xml");

	return true;
}

const SDL_Rect& j1Input::GetSectionForElement(std::string name)
{
	SDL_Rect ret = { 0,0,0,0 };

	// get the associated button or axis
	std::map<std::string, ControllerPressData&>::iterator it;
	it = guiElemMapInput.find(name);

	if (it != guiElemMapInput.end())
	{
		// get associated rect to this button
		if ((*it).second.button != -1)
		{
			ret = GetAssociatedRectForThisGamepadInput((*it).second.button, SDL_GameControllerAxis(-1));
		}
		else if ((*it).second.axis != -1)
		{
			ret = GetAssociatedRectForThisGamepadInput(SDL_GameControllerButton(-1), (*it).second.axis);
		}
	}
	
	return ret;
}

bool j1Input::GenerateGuiElemMapping() // to get associated controllerpress data to this actions
{
	// shared input relative to gui nomenclature
	guiElemMapInput.insert(std::pair<std::string, ControllerPressData&>("interact", gamepadScheme.sharedInput.interact));
	guiElemMapInput.insert(std::pair<std::string, ControllerPressData&>("swap_next", gamepadScheme.sharedInput.swap_next));
	guiElemMapInput.insert(std::pair<std::string, ControllerPressData&>("swap_prev", gamepadScheme.sharedInput.swap_prev));

	// all pj input relative to gui nomenclature
	// marche
	guiElemMapInput.insert(std::pair<std::string, ControllerPressData&>("marche_basic_button", gamepadScheme.marche.basic));
	guiElemMapInput.insert(std::pair<std::string, ControllerPressData&>("marche_dash_button", gamepadScheme.marche.dodge));
	guiElemMapInput.insert(std::pair<std::string, ControllerPressData&>("marche_special1_button", gamepadScheme.marche.special1));
	guiElemMapInput.insert(std::pair<std::string, ControllerPressData&>("marche_special2_button", gamepadScheme.marche.special2));
	guiElemMapInput.insert(std::pair<std::string, ControllerPressData&>("marche_ultimate_button", gamepadScheme.marche.ultimate));
	guiElemMapInput.insert(std::pair<std::string, ControllerPressData&>("marche_aim_button", gamepadScheme.marche.aim));

	// ritz
	guiElemMapInput.insert(std::pair<std::string, ControllerPressData&>("ritz_basic_button", gamepadScheme.ritz.basic));
	guiElemMapInput.insert(std::pair<std::string, ControllerPressData&>("ritz_dash_button", gamepadScheme.ritz.dodge));
	guiElemMapInput.insert(std::pair<std::string, ControllerPressData&>("ritz_special1_button", gamepadScheme.ritz.special1));
	guiElemMapInput.insert(std::pair<std::string, ControllerPressData&>("ritz_special2_button", gamepadScheme.ritz.special2));
	guiElemMapInput.insert(std::pair<std::string, ControllerPressData&>("ritz_ultimate_button", gamepadScheme.ritz.ultimate));
	guiElemMapInput.insert(std::pair<std::string, ControllerPressData&>("ritz_aim_button", gamepadScheme.ritz.aim));

	// shara
	guiElemMapInput.insert(std::pair<std::string, ControllerPressData&>("shara_basic_button", gamepadScheme.shara.basic));
	guiElemMapInput.insert(std::pair<std::string, ControllerPressData&>("shara_dash_button", gamepadScheme.shara.dodge));
	guiElemMapInput.insert(std::pair<std::string, ControllerPressData&>("shara_special1_button", gamepadScheme.shara.special1));
	guiElemMapInput.insert(std::pair<std::string, ControllerPressData&>("shara_special2_button", gamepadScheme.shara.special2));
	guiElemMapInput.insert(std::pair<std::string, ControllerPressData&>("shara_ultimate_button", gamepadScheme.shara.ultimate));
	guiElemMapInput.insert(std::pair<std::string, ControllerPressData&>("shara_aim_button", gamepadScheme.shara.aim));

	// map gui nomenclature to specific groups to compare repeteads when input is coming from gui menu

	// marche
	comparerGuiMapForRepeteadOnGroup.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("marche_basic_button", marcheMapInput));
	comparerGuiMapForRepeteadOnGroup.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("marche_dash_button", marcheMapInput));
	comparerGuiMapForRepeteadOnGroup.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("marche_special1_button", marcheMapInput));
	comparerGuiMapForRepeteadOnGroup.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("marche_special2_button", marcheMapInput));
	comparerGuiMapForRepeteadOnGroup.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("marche_ultimate_button", marcheMapInput));
	comparerGuiMapForRepeteadOnGroup.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("marche_aim_button", marcheMapInput));

	// ritz
	comparerGuiMapForRepeteadOnGroup.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("ritz_basic_button", ritzMapInput));
	comparerGuiMapForRepeteadOnGroup.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("ritz_dash_button", ritzMapInput));
	comparerGuiMapForRepeteadOnGroup.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("ritz_special1_button", ritzMapInput));
	comparerGuiMapForRepeteadOnGroup.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("ritz_special2_button", ritzMapInput));
	comparerGuiMapForRepeteadOnGroup.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("ritz_ultimate_button", ritzMapInput));
	comparerGuiMapForRepeteadOnGroup.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("ritz_aim_button", ritzMapInput));
	
	// shara
	comparerGuiMapForRepeteadOnGroup.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("shara_basic_button", sharaMapInput));
	comparerGuiMapForRepeteadOnGroup.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("shara_dash_button", sharaMapInput));
	comparerGuiMapForRepeteadOnGroup.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("shara_special1_button", sharaMapInput));
	comparerGuiMapForRepeteadOnGroup.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("shara_special2_button", sharaMapInput));
	comparerGuiMapForRepeteadOnGroup.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("shara_ultimate_button", sharaMapInput));
	comparerGuiMapForRepeteadOnGroup.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("shara_aim_button", sharaMapInput));
	
	// shared input
	comparerGuiMapForRepeteadOnGroup.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("interact", generalMapInput));
	comparerGuiMapForRepeteadOnGroup.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("swap_next", generalMapInput));
	comparerGuiMapForRepeteadOnGroup.insert(std::pair<std::string, std::map<std::string, ControllerPressData&>&>("swap_prev", generalMapInput));

return true;
}

bool j1Input::GenerateGuiButtonsRectMapping()
{
	// buttons
	guiButtonRectsMap.insert(std::pair<SDL_GameControllerButton, SDL_Rect>(SDL_CONTROLLER_BUTTON_A, { 93,31,30,30 }));
	guiButtonRectsMap.insert(std::pair<SDL_GameControllerButton, SDL_Rect>(SDL_CONTROLLER_BUTTON_B, { 31,0,30,30 }));
	guiButtonRectsMap.insert(std::pair<SDL_GameControllerButton, SDL_Rect>(SDL_CONTROLLER_BUTTON_X, { 62,0,30,30 }));
	guiButtonRectsMap.insert(std::pair<SDL_GameControllerButton, SDL_Rect>(SDL_CONTROLLER_BUTTON_Y, { 63,31,30,30 }));
	guiButtonRectsMap.insert(std::pair<SDL_GameControllerButton, SDL_Rect>(SDL_CONTROLLER_BUTTON_LEFTSHOULDER, { 124,0,30,30 }));
	guiButtonRectsMap.insert(std::pair<SDL_GameControllerButton, SDL_Rect>(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, { 155,0,30,30 }));
	guiButtonRectsMap.insert(std::pair<SDL_GameControllerButton, SDL_Rect>(SDL_CONTROLLER_BUTTON_RIGHTSTICK, { 93,0,30,30 }));
	guiButtonRectsMap.insert(std::pair<SDL_GameControllerButton, SDL_Rect>(SDL_CONTROLLER_BUTTON_LEFTSTICK, { 0,31,30,30 }));

	// axis
	guiAxisRectsMap.insert(std::pair<SDL_GameControllerAxis, SDL_Rect>(SDL_CONTROLLER_AXIS_TRIGGERLEFT, { 0,0,30,30 }));
	guiAxisRectsMap.insert(std::pair<SDL_GameControllerAxis, SDL_Rect>(SDL_CONTROLLER_AXIS_TRIGGERRIGHT, { 31,31,30,30 }));

	return true;
}

const SDL_Rect j1Input::GetAssociatedRectForThisGamepadInput(SDL_GameControllerButton button, SDL_GameControllerAxis axis)
{
	SDL_Rect ret = { 0,0,0,0 };

	if (button != -1)
	{
		// protection if not found
		std::map<SDL_GameControllerButton, SDL_Rect>::iterator it = guiButtonRectsMap.find(button);
		if (it != guiButtonRectsMap.end())
			ret = (*it).second;
	}
	else if (axis != -1)
	{
		// protection if not found
		std::map<SDL_GameControllerAxis, SDL_Rect>::iterator it = guiAxisRectsMap.find(axis);
		if (it != guiAxisRectsMap.end())
			ret = (*it).second;
	}

	return ret;
}

void j1Input::ListeningInputFor(std::string gui_elem_name)
{
	// if any input is pressed
	const ControllerPressData newMapping = CheckGamepadWTFPressedInput();
	bool acceptedInput = false;

	// if receives a possible valid mapping
	// filter with permitted input expected to change
	if (newMapping.button != -1)
	{
		acceptedInput = buttonPermittedMatrix[newMapping.button];
	}
	else if (newMapping.axis != -1)
	{
		acceptedInput = axisPermittedMatrix[newMapping.axis];
	}

	if (acceptedInput)
	{
		// gets the specific data to be changed
		std::map<std::string, ControllerPressData&>::iterator it = guiElemMapInput.find(gui_elem_name);

		if (it != guiElemMapInput.end()) // if the element is found
		{
			// first check if the input to be changed is from shared scheme, or characters scheme
			// if are coming from characters, only we need to check the shared and specific character
			// but if we are coming from shared, we need to check itself, and all characters
			std::map<std::string, std::map<std::string, ControllerPressData&>&>::iterator genIt;
			genIt = comparerGuiMapForRepeteadOnGroup.find(gui_elem_name);

			if (&(*genIt).second == &generalMapInput) // if coming from shared
			{
				// check all characters and itself
				// characterNameToMapData contains all that we need
				for (std::map<std::string, std::map<std::string, ControllerPressData&>&>::iterator it2 = characterNameToMapData.begin(); it2 != characterNameToMapData.end(); ++it2)
				{
					// iterate all groups: general, marche,ritz and shara
					for (std::map<std::string, ControllerPressData&>::iterator it3 = (*it2).second.begin(); it3 != (*it2).second.end(); ++it3)
					{
						if (&(*it).second == &(*it3).second) // if the previous to be modified "button" is the same that we are currently checking, skip
							continue;

						// check for all buttons and axis, if found repetead, write the old one
						if ((*it3).second.button == newMapping.button && newMapping.button != -1)
							(*it3).second = (*it).second;

						if ((*it3).second.axis == newMapping.axis && newMapping.axis != -1)
							(*it3).second = (*it).second;
					}
				}
				// and finally update the desired one
				(*it).second.button = newMapping.button;
				(*it).second.axis = newMapping.axis;
			}
			else
			{
				bool inputFoundOnShared = false;
				// check for possible repetead key on shared scheme
				for (std::map<std::string, ControllerPressData&>::iterator sharedIt = generalMapInput.begin(); sharedIt != generalMapInput.end(); ++sharedIt)
				{
					// check for all buttons and axis, if found repetead, write the old one
					if ((*sharedIt).second.button == newMapping.button && newMapping.button != -1)
					{
						(*sharedIt).second = (*it).second;
						inputFoundOnShared = true;
					}

					if ((*sharedIt).second.axis == newMapping.axis && newMapping.axis != -1)
					{
						(*sharedIt).second = (*it).second;
						inputFoundOnShared = true;
					}

					
				}
				// check if enters on any condition above, if this, recheck all the players, except the shared scheme (already checked)
				if (inputFoundOnShared)
				{
					for (std::map<std::string, std::map<std::string, ControllerPressData&>&>::iterator it2 = characterNameToMapData.begin(); it2 != characterNameToMapData.end(); ++it2)
					{
						if (&(*it2).second == &generalMapInput) // skip the general input scheme
							continue;

						// iterate through accepted map (characters input map)
						for (std::map<std::string, ControllerPressData&>::iterator it3 = (*it2).second.begin(); it3 != (*it2).second.end(); ++it3)
						{
							if (&(*it).second == &(*it3).second) // if the previous modified "button" is the same that we are currently checking, skip
								continue;

							// check for all buttons and axis, if found repetead, write the old one
							if ((*it3).second.button == (*it).second.button && (*it).second.button != -1)
								(*it3).second = newMapping;

							if ((*it3).second.axis == (*it).second.axis && (*it).second.axis != -1)
								(*it3).second = newMapping;
						}

					}
				}
				else // if not, recheck only the target scheme
				{
					// and checks for possible repetead key on this mapped scheme
					std::map<std::string, std::map<std::string, ControllerPressData&>&>::iterator it2;
					it2 = comparerGuiMapForRepeteadOnGroup.find(gui_elem_name);

					if (it2 != comparerGuiMapForRepeteadOnGroup.end())
					{
						for (std::map<std::string, ControllerPressData&>::iterator it3 = (*it2).second.begin(); it3 != (*it2).second.end(); ++it3)
						{
							if (&(*it).second == &(*it3).second) // if the previous modified "button" is the same that we are currently checking, skip
								continue;

							// check for all buttons and axis, if found repetead, write the old one
							if ((*it3).second.button == newMapping.button && newMapping.button != -1)
								(*it3).second = (*it).second;
							

							if ((*it3).second.axis == newMapping.axis && newMapping.axis != -1)
								(*it3).second = (*it).second;
						}
					}
				}

				// and finally update the desired one
				(*it).second.button = newMapping.button;
				(*it).second.axis = newMapping.axis;
			}
		}
	}
}