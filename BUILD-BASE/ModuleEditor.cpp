#include "Application.h"
#include "ModuleEditor.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
	bool ret = true;

    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Setup window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    gl_context = SDL_GL_CreateContext(App->window->window);
    SDL_GL_MakeCurrent(App->window->window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    io_copy = io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(App->window->window, gl_context);
    ImGui_ImplOpenGL3_Init();

    // Our state
    clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

    // Frame MAX init
    App->maxFPS = 60;

    // Init Window
    SDL_GetCurrentDisplayMode(0, &DM);

    bright = 1;
    width = SCREEN_WIDTH;
    height = SCREEN_HEIGHT;

	return ret;
}

update_status ModuleEditor::PreUpdate(float dt)
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Refresh fps/ms log
    // FPS
    if (fps_log.size() <= 100) fps_log.push_back((1.f / dt));
    else
    {
        fps_log.erase(fps_log.begin());
        fps_log.push_back((1.f / dt));
    }

    // MS
    if (ms_log.size() <= 100) ms_log.push_back(dt);
    else
    {
        ms_log.erase(ms_log.begin());
        ms_log.push_back(dt);
    }

    // Add Log
    //buffer.appendf(entry);

    return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float dt)
{
    SDL_GetWindowPosition(App->window->window, &posWindowX, &posWindowY);
    SDL_GetWindowSize(App->window->window, &actualwidth, &actualheight);
    LogToConsole();

    // Menu Window
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Menu"))
            {
                if (ImGui::MenuItem("Configuration")) configB = !configB;
                if (ImGui::MenuItem("Console")) consoleB = !consoleB;
                if (ImGui::MenuItem("About")) aboutB = !aboutB;
                ImGui::EndMenu();
            }
        }

        ImGui::EndMainMenuBar();
    }

    // Configuration Window
    if (configB)
    {
        ImGui::Begin("Configuration", &configB);
        ImGui::Text("Options");

        if (ImGui::TreeNode("Application"))
        {
            ImGui::InputText("App Name", "JaggyJiggy", 20);
            ImGui::InputText("Organization", "UPC Stundets", 20);

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Window"))
        {
            ImGui::Text("Brightness");
            if (ImGui::SliderFloat(" ", &bright, 0.f, 1.f)) SDL_SetWindowBrightness(App->window->window, bright);

            if (resizable)
            {
                ImGui::Text("Size");
                if (ImGui::SliderInt("Width", &width, 640, SCREEN_WIDTH)) SDL_SetWindowSize(App->window->window, width, height);
                if (ImGui::SliderInt("Height", &height, 480, SCREEN_HEIGHT)) SDL_SetWindowSize(App->window->window, width, height);
              
                SDL_GetWindowSize(App->window->window, &modifywidth, &modifyheight);
                if (modifywidth != width || modifyheight != height) SDL_SetWindowSize(App->window->window, width, height);

                if (!fullscreen)
                {
                    glViewport(0, 0, width, height);
                    Resize(0);
                }
            }

            if (ImGui::Checkbox("Windowed", &fullscreen))
            {
                if (fulldesktop)
                {
                    SDL_SetWindowFullscreen(App->window->window, 0);
                    Resize(0);
                    //resizeActive = true;
                    fulldesktop = false;
                    borderless = true;
                }

                if (fullscreen)
                {
                    borderless = true;
                    resizable = false;
                    SDL_GetWindowPosition(App->window->window, &posX, &posY);
                    SDL_SetWindowPosition(App->window->window, 0, 30);
                    SDL_SetWindowSize(App->window->window, DM.w, DM.h - 71);
                    Resize(0);
                    //resizeActive = true;
                    glViewport(0, 0, DM.w, DM.h);
                    SDL_GetWindowSize(App->window->window, &modifywidth, &modifyheight);
                    SDL_SetWindowBordered(App->window->window, SDL_bool(borderless));
                }
                else
                {
                    resizable = true;
                    SDL_SetWindowSize(App->window->window, width, height);
                    SDL_SetWindowPosition(App->window->window, posX, posY);
                    Resize(0);
                    //resizeActive = true;
                }

            }

            if (fullscreen && (modifywidth != width || modifyheight != height))
            {
                SDL_SetWindowSize(App->window->window, DM.w, DM.h - 71);
                glViewport(0, 0, DM.w, DM.h);
            }

            ImGui::SameLine();
            if (!fullscreen && !fulldesktop) ImGui::Checkbox("Resizable", &resizable);

            if (!fullscreen && !fulldesktop) if (ImGui::Checkbox("Border", &borderless)) SDL_SetWindowBordered(App->window->window, SDL_bool(borderless));
           
            ImGui::SameLine();
            if (ImGui::Checkbox("Fullscreen", &fulldesktop))
            {
                if (fulldesktop)
                {
                    SDL_GetWindowPosition(App->window->window, &posX, &posY);
                    SDL_SetWindowPosition(App->window->window, 0, 30);
                    SDL_SetWindowSize(App->window->window, DM.w, DM.h - 71);
                    glViewport(0, 0, DM.w, DM.h);
                    SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN);
                    Resize(0);
                    //resizeActive = true;
                    borderless = false;
                    resizable = false;
                    fullscreen = false;
                }
                else
                {
                    SDL_SetWindowFullscreen(App->window->window, 0);
                    SDL_SetWindowSize(App->window->window, width, height);
                    SDL_SetWindowPosition(App->window->window, posX, posY);
                    Resize(0);
                    //resizeActive = true;
                    borderless = true;
                    resizable = true;
                }
                SDL_SetWindowBordered(App->window->window, SDL_bool(borderless));
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Render"))
        {
            // Frame Rate
            ImGui::Text("Limit Framerate: ");
            ImGui::SameLine();
            if (App->maxFPS > 0) ImGui::Text("%.1f", App->maxFPS);
            else ImGui::Text("VSync");
            ImGui::SliderFloat("Max FPS", &App->maxFPS, 0, 120);

            ImGui::Text("FPS");
            char title[25];
            sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
            ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
            ImGui::Text("MS");
            sprintf_s(title, 25, "Milliseconds %.3f", ms_log[ms_log.size() - 1]);
            ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 0.024f, ImVec2(310, 100));

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Input"))
        {
            ImGui::Text("Mouse Position: %i, %i", App->input->mouse_x, App->input->mouse_y);
            ImGui::Text("Mouse Movement: %i, %i", App->input->mouse_x_motion, App->input->mouse_y_motion);
            ImGui::Text("Mouse Wheel: %i", App->input->mouse_z);

            ImGui::BeginChild("Input Log");
            ImGui::TextUnformatted(input.begin());
            if (scroll) ImGui::SetScrollHereY(1);
            scroll = false;
            ImGui::EndChild();

            ImGui::TreePop();
        }

        if (ImGui::GetWindowPos().x - posWindowX < 0 ||
            ImGui::GetWindowPos().y - posWindowY < 0 ||
            ImGui::GetWindowPos().x - posWindowX + ImGui::GetWindowSize().x > actualwidth ||
            ImGui::GetWindowPos().y - posWindowY + ImGui::GetWindowSize().y > actualheight)
        {
            Resize(0);
        }

        ImGui::End();
    }

    // Console/Debug Window
    if (consoleB)
    {
        ImGui::Begin("Console", &consoleB);

        ImGui::TextUnformatted(buffer.begin());
        if (scrollC) ImGui::SetScrollHereY(1);
        scrollC = false;

        if (ImGui::GetWindowPos().x - posWindowX < 0 ||
            ImGui::GetWindowPos().y - posWindowY < 0 ||
            ImGui::GetWindowPos().x - posWindowX + ImGui::GetWindowSize().x > actualwidth ||
            ImGui::GetWindowPos().y - posWindowY + ImGui::GetWindowSize().y > actualheight) 
            //resizeActive)
        {
            Resize(1);
        }

        ImGui::End();
    }

    // About Window
    if (aboutB)
    {
        ImGui::Begin("About", &aboutB);

        ImGui::Text("JaggyJiggy v0.1");
        ImGui::Text("3D Game Engine");
        ImGui::Text("Autor:");
        ImGui::BulletText("Alex Gesti: https://github.com/alexgesti");
        ImGui::BulletText("Raul Cano: https://github.com/ercanon");
        ImGui::NewLine();
        ImGui::Text("3rd Party Libraries used:");
        ImGui::BulletText("SDL 2.0.16");
        ImGui::BulletText("Glew 2.1.0");
        ImGui::BulletText("ImGui 1.84.2");
        ImGui::BulletText("MathGeoLib 1.5");
        ImGui::BulletText("OpenGL 3.1");
        // Habrá que añadir más
        ImGui::NewLine();
        ImGui::Text("MIT License");
        ImGui::NewLine();
        ImGui::Text("Copyright (c) 2021 ercanon & pixelcuak");
        ImGui::NewLine();
        ImGui::Text("The above copyright notice and this permission");
        ImGui::Text("notice shall be included in all copies or");
        ImGui::Text("substantial portions of the Software.");
        ImGui::NewLine();
        ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT");
        ImGui::Text("WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,");
        ImGui::Text("INCLUDING BUT NOT LIMITED TO THE WARRANTIES");
        ImGui::Text("OF MERCHANTABILITY, FITNESS FOR A PARTICULAR");
        ImGui::Text("PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL");
        ImGui::Text("THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR");
        ImGui::Text("ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER");
        ImGui::Text("IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,");
        ImGui::Text("ARISING FROM, OUT OF OR IN CONNECTION WITH THE");
        ImGui::Text("SOFTWARE OR THE USE OR OTHER DEALINGS IN THE");
        ImGui::Text("SOFTWARE.");

        ImGui::End();
    }

    return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{
    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io_copy.DisplaySize.x, (int)io_copy.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    //glClear(GL_COLOR_BUFFER_BIT);
    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
    if (io_copy.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }

    return UPDATE_CONTINUE;
}

void ModuleEditor::Resize(int num)
{
    if (num == 0)
    {
        if (fullscreen) ImGui::SetWindowSize({ float(DM.w / 4),  float(DM.h / 1.5f) });
        else if (fulldesktop) ImGui::SetWindowSize({ float(DM.w / 4),  float(DM.h / 1.25f) });
        else ImGui::SetWindowSize({ float(width / 4),  float(height / 1.41f) });
        ImGui::SetWindowPos({ float(posWindowX), float(posWindowY) + 19 });
    }

    if (num == 1)
    {
        if (fullscreen)
        {
            ImGui::SetWindowSize({ float(DM.w),  float((DM.h - 120) / 3.75f) });
            ImGui::SetWindowPos({ float(posWindowX), float(posWindowY) + 19 + float(DM.h / 1.5f) });
        }
        else
        {
            ImGui::SetWindowSize({ float(width),  float(height / 3.75f) });
            ImGui::SetWindowPos({ float(posWindowX), float(posWindowY) + 48 + float(height / 1.5f) });
        }

        //resizeActive = false;
    }
}

void ModuleEditor::LogToConsole()
{
    /*va_list args;
    va_start(args, fmt);
    buffer.appendv(fmt, args);    
    va_end(args);
    scrollC = true;*/
}

bool ModuleEditor::CleanUp()
{
	bool ret = true;

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    fps_log.clear();
    ms_log.clear();
    buffer.clear();

    SDL_GL_DeleteContext(gl_context);

	return ret;
}