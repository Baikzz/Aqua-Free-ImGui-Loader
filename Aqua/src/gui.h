#include "imgui.h"
#include <iostream>
#include "aqua_var.h"
#include <D3dx9tex.h>
#include "imgui_notify.h"
#pragma comment(lib, "dxguid.lib")
#include <ctime>
#pragma comment(lib, "D3dx9")

using namespace ImGui;

static char user[20] = "";
static char pass[20] = "";
static bool logged = false;
float rounding = 20.0f;
ImVec2 start = { 50, 50 };
ImVec2 img_size = { 30, 30 };

namespace c_gui {

    class gui {
    public:
        DWORD window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings;
        ImGuiStyle& style = ImGui::GetStyle();

        char user[20] = "";

        void sws(float X, float Y)
        {
            SetWindowSize({ X, Y });
        }

        void begin(const char* name, float X, float Y) 
        {
            ImGui::Begin(name, NULL, window_flags);
            sws(X, Y);
        }
        
        void end()
        {
            ImGui::End();
        }
        
        void text(const char* text, float posX, float posY)
        {
            setpos(posX, posY);
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), text);
        }

        void bText(const char* text, float posX, float posY)
        {
            setpos(posX, posY);
            ImGui::TextColored(ImVec4(0.0157, 0.3608, 0.5882, 1.0), text);
        }

        bool button(const char* name, float sizeX, float sizeY, float posX, float posY)
        {
            SetCursorPos({ posX, posY });

            return ImGui::Button(name, ImVec2(sizeX, sizeY));
            ImGui::PopStyleColor(2);
        }
        
        void hintInput(const char* name, const char* hint, char test[20], float posX, float posY) 
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 10));
            SetCursorPos({ posX, posY });

            ImGui::InputTextWithHint(name, hint, test, sizeof(test));
            TreePop();
            ImGui::PopStyleVar();
        }
        
        void child(const char* name, float sizeX, float sizeY, float posX, float posY)
        {
            SetCursorPos({ posX, posY });
            ImGui::BeginChild(name, {sizeX, sizeY});
        }

        void endChild()
        {
            ImGui::EndChild();
        }

        void hintInputPass(const char* name, const char* hint, char test[20], float posX, float posY) 
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 10));
            SetCursorPos({ posX, posY });

            ImGui::InputTextWithHint(name, hint, test, sizeof(test), ImGuiInputTextFlags_Password);
            TreePop();
            ImGui::PopStyleVar();
        }
        
        void setpos(float X, float Y) 
        {
            SetCursorPos({ X, Y });
        }
        
        void ppFont() 
        {
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        }
        
        void psFont() 
        {
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
        }
        
        void stpFont()
        {
            ImGui::PopFont();
        }
        
        void applyStyle() 
        {
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.23f, 0.23f, 0.23f, 0.3f)); //FOR INPUT TEXT BG
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0157, 0.3608, 0.5882, 1.0)); //NORMAL BUTTON COLOR
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0118, 0.5765, 0.8392, 1.0)); //HOVERED BUTTON COLOR
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.23, 0.23, 0.23, 0.3)); //CHILD BG
            ImGui::RenderNotifications(); // Renders the notifications, i used https://github.com/patrickcjk/imgui-notify
            style.FrameRounding = 5.0f; //FRAME ROUNDING FOR ALL ITEMS
            style.WindowRounding = 10.0f; //WINDOW ROUNDING
            style.ChildRounding = 10.0f; // CHILD ROUNDING
        }
        
        void particles() // <- pasted from https://github.com/dannybanno/imgui-particle-background
        {
            ImDrawList* drawList = ImGui::GetWindowDrawList();

            static const int numParticles = 140;
            static ImVec2 particlePositions[numParticles];
            static ImVec2 particleVelocities[numParticles];
            static bool initialized = false;

            // Dimensiones fijas del área de dibujo
            static const ImVec2 drawAreaPos(10, 10);  // Posición superior izquierda del área de dibujo
            static const ImVec2 drawAreaSize(1280, 960); // Tamaño del área de dibujo

            if (!initialized) {
                // Inicializar las partículas dentro del área de dibujo
                for (int i = 0; i < numParticles; ++i) {
                    particlePositions[i] = ImVec2(
                        drawAreaPos.x + drawAreaSize.x * static_cast<float>(rand()) / RAND_MAX,
                        drawAreaPos.y + drawAreaSize.y * static_cast<float>(rand()) / RAND_MAX
                    );

                    particleVelocities[i] = ImVec2(
                        static_cast<float>((rand() % 11) - 5),
                        static_cast<float>((rand() % 11) - 5)
                    );
                }
                initialized = true;
            }

            ImVec2 cursorPos = ImGui::GetIO().MousePos;

            for (int i = 0; i < numParticles; ++i) {
                // Dibujar líneas a otras partículas
                for (int j = i + 1; j < numParticles; ++j) {
                    float distance = std::hypotf(particlePositions[j].x - particlePositions[i].x, particlePositions[j].y - particlePositions[i].y);
                    float opacity = 1.0f - (distance / 2.0f);  // Cambiar opacidad

                    if (opacity > 0.0f) {
                        ImU32 lineColor = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, opacity));
                        drawList->AddLine(particlePositions[i], particlePositions[j], lineColor);
                    }
                }

                // Dibujar líneas al cursor
                float distanceToCursor = std::hypotf(cursorPos.x - particlePositions[i].x, cursorPos.y - particlePositions[i].y);
                float opacityToCursor = 1.0f - (distanceToCursor / 30.0f);  // Ajustar divisor para controlar cambio de opacidad

                if (opacityToCursor > 0.0f) {
                    ImU32 lineColorToCursor = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, opacityToCursor));
                    drawList->AddLine(cursorPos, particlePositions[i], lineColorToCursor);
                }
            }

            // Actualizar y renderizar partículas
            float deltaTime = ImGui::GetIO().DeltaTime;
            for (int i = 0; i < numParticles; ++i) {
                particlePositions[i].x += particleVelocities[i].x * deltaTime;
                particlePositions[i].y += particleVelocities[i].y * deltaTime;

                // Mantener dentro del área de dibujo
                if (particlePositions[i].x < drawAreaPos.x)
                    particlePositions[i].x = drawAreaPos.x + drawAreaSize.x;
                else if (particlePositions[i].x > drawAreaPos.x + drawAreaSize.x)
                    particlePositions[i].x = drawAreaPos.x;

                if (particlePositions[i].y < drawAreaPos.y)
                    particlePositions[i].y = drawAreaPos.y + drawAreaSize.y;
                else if (particlePositions[i].y > drawAreaPos.y + drawAreaSize.y)
                    particlePositions[i].y = drawAreaPos.y;

                ImU32 particleColour = ImGui::ColorConvertFloat4ToU32(settings::particleColour);

                drawList->AddCircleFilled(particlePositions[i], 1.5f, particleColour);
            }
        }
    };
}