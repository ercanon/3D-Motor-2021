#include "Application.h"
#include "ModuleInput.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	App->editor->LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		App->editor->LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
			{
				keyboard[i] = KEY_DOWN;
				LogInput(i, KEY_DOWN);
			}
			else
			{
				keyboard[i] = KEY_REPEAT;
				LogInput(i, KEY_REPEAT);
			}
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
			{
				keyboard[i] = KEY_UP;
				LogInput(i, KEY_UP);
			}
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	wheel_mov = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if (mouse_buttons[i] == KEY_IDLE)
			{
				mouse_buttons[i] = KEY_DOWN;
				LogInput(1000 + i, KEY_DOWN);
				LogInput(1000 + i, KEY_REPEAT);
			}
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if (mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
			{
				mouse_buttons[i] = KEY_UP;
				LogInput(1000 + i, KEY_UP);
			}
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	// Pointer for directory of dropped file
	char* dropped_filedir;
	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			wheel_mov = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
				break;
			}

			case SDL_DROPFILE: 
			{
				dropped_filedir = e.drop.file;
				SDL_ShowSimpleMessageBox(
					SDL_MESSAGEBOX_INFORMATION,
					"File dropped on window",
					dropped_filedir,
					App->window->window
				);
				App->fbx->LoadFbx(dropped_filedir);
				App->texture->LoadTextures(dropped_filedir);
				SDL_free(dropped_filedir);    // Free dropped_filedir memory
				break;
			}
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	App->editor->LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

void ModuleInput::LogInput(uint key, uint state)
{
	static char entry[512];
	static const char* states[] = { "IDLE", "DOWN", "REPEAT", "UP" };

	if (key < 1000) sprintf_s(entry, 512, "Key: %02u - %s\n", key, states[state]);
	else sprintf_s(entry, 512, "Mouse: %02u - %s\n", key - 1000, states[state]);

	App->editor->input.appendf(entry);
	App->editor->scroll = true;
}