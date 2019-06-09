#ifndef __j1INPUT_H__
#define __j1INPUT_H__

#include "j1Module.h"
#include "SDL/include/SDL_gamecontroller.h"
#include "SDL/include/SDL_haptic.h"
#include <map>

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
//#define LAST_KEYS_PRESSED_BUFFER 50
#define DEAD_ZONE 8000

struct SDL_Rect;

enum j1EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum j1KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum j1JoyDir
{
	JOYSTICK_DIR_LEFT, // neg
	JOYSTICK_DIR_RIGHT, // pos
	JOYSTICK_DIR_UP, // neg
	JOYSTICK_DIR_DOWN, // pos
	JOYSTICK_DIR_MAX
};

enum j1JoyStickSide
{
	JOY_STICK_LEFT,
	JOY_STICK_RIGHT,
	JOY_MAX
};

struct ControllerPressData
{
	/*bool pressed = false;
	bool isButton = false;*/
	SDL_GameControllerButton button = SDL_GameControllerButton(-1);
	SDL_GameControllerAxis axis = SDL_GameControllerAxis(-1);
};

struct SharedControlScheme
{
	ControllerPressData interact;
	ControllerPressData swap_next;
	ControllerPressData swap_prev;
};

struct CharacterControlScheme
{
	ControllerPressData basic;
	ControllerPressData dodge;
	ControllerPressData special1;
	ControllerPressData special2;
	ControllerPressData ultimate;
	ControllerPressData aim;
};

struct GamepadControlScheme
{
	SharedControlScheme sharedInput;
	CharacterControlScheme marche;
	CharacterControlScheme ritz;
	CharacterControlScheme shara;
};

class j1Input : public j1Module
{

public:

	j1Input();

	// Destructor
	virtual ~j1Input();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Gather relevant win events
	bool GetWindowEvent(j1EventWindow ev);

	// Check key states (includes mouse and joy buttons)
	j1KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	j1KeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	Sint16 GetControllerAxis(SDL_GameControllerAxis axis);

	j1KeyState GetControllerButton(int id) const {
		return controller[id];
	}

	void SetControllerButtonState(int button, j1KeyState state);

	j1KeyState GetControllerAxisPulsation(int id) const
	{
		return controller_axis[id];
	}

	j1KeyState GetJoystickPulsation(j1JoyStickSide joystickSide, j1JoyDir joyButtonDir) const;

	j1KeyState GetControllerGeneralPress(ControllerPressData mappedButtonData) const;


	// Check if a certain window event happened
	bool GetWindowEvent(int code);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

	uint GetCurrentMouseButtonDown(); 

	void DoGamePadRumble(float strength, uint32 duration) const;

	const ControllerPressData& CheckGamepadWTFPressedInput() const;

	bool LoadGamepadMapScheme(const char* path, const char* scheme = "current");
	bool SaveGamepadMapScheme(const char* path);
	const SDL_Rect& GetSectionForElement(std::string name);
	void ListeningInputFor(std::string name); // listen and change mapping input for gui controls keybinding
	bool IsAimToggled() const;
	void ToggleAimON();
	void ToggleAimOFF();

	const SDL_Rect GetAssociatedRectForThisGamepadInput(SDL_GameControllerButton button = SDL_GameControllerButton(-1),
		SDL_GameControllerAxis axis = SDL_GameControllerAxis(-1));

private:
	bool GenerateGuiButtonsRectMapping();
	bool GenerateMapping();
	bool GenerateGuiElemMapping();

private:
	bool aimToggled = false;
	bool buttonPermittedMatrix[SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_MAX] = { false };
	bool axisPermittedMatrix[SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_MAX] = { false };
	bool		windowEvents[WE_COUNT];
	j1KeyState*	keyboard = nullptr;
	j1KeyState*	mouse_buttons = nullptr;
	j1KeyState* controller = nullptr;
	j1KeyState* controller_axis = nullptr;
	j1KeyState* joystick = nullptr;
	int			mouse_motion_x;
	int			mouse_motion_y;
	int			mouse_x;
	int			mouse_y;

	SDL_GameController* gamePad1 = nullptr;
	SDL_Haptic* haptic = nullptr;
private:
	std::map<std::string, std::map<std::string, ControllerPressData&>&> comparerGuiMapForRepeteadOnGroup;
	std::map<SDL_GameControllerButton, SDL_Rect> guiButtonRectsMap;
	std::map<SDL_GameControllerAxis, SDL_Rect> guiAxisRectsMap;
	std::map<std::string, ControllerPressData&> guiElemMapInput;
	std::map<std::string, ControllerPressData&> generalMapInput;
	std::map<std::string, ControllerPressData&> marcheMapInput;
	std::map<std::string, ControllerPressData&> ritzMapInput;
	std::map<std::string, ControllerPressData&> sharaMapInput;
	std::map<std::string, std::map<std::string, ControllerPressData&>&> characterNameToMapData;
public:
	GamepadControlScheme gamepadScheme;
};

#endif // __j1INPUT_H__