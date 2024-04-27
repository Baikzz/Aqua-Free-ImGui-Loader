#include "imgui.h"
#include "aqua_var.h"
#include <D3dx9tex.h>
#include <iostream>
#include <ctime>
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3dx9")

using namespace ImGui;


namespace c_gui
{

    static bool logged = false;
    static int page = 1;

    static char user[64] = "";
    static char pass[64] = "";

    bool button1Active = true;
    bool button2Active = false;

    float rounding = 20.0f;
    static float textopp = 1;
    static float mtextopp = 1;
    static float oppacity = 0;
    static float itemopp = 0;
    static float childopp = 0;
    static float mchildopp = 0;
    static float buttonopp = 0;
    static float opacityText = 0;

    static ImVec2 start = { 50, 50 };
    static ImVec2 img_size = { 30, 30 };
    static ImVec4 blue = { 0.0157, 0.3608, 0.5882, textopp };
    static ImVec4 white = { 1, 1, 1, textopp };
    static ImVec4 red = { 0.9804, 0.2235, 0.2118, mtextopp };
    static ImVec4 green = { 0.0745, 0.7176, 0.4431, mtextopp };

    class gui 
{
    public:

        DWORD window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings;
        ImGuiStyle& style = ImGui::GetStyle();

        void sws(float X, float Y)
        {
            SetWindowSize({ X, Y });
        }

        void animation()
        {
            static float X = 350;
            static float Y = 400;

            if (X <= 500)
            {
                X += 2;
                sws(X, Y);
            }
            sws(X, Y);
        }

        void opp()
        {
            if (oppacity <= 0.8)
            {
                oppacity += 0.008;
            }

            if (itemopp <= 0.85)
            {
                itemopp += 0.004;
            }

            if (childopp <= 0.3)
            {
                childopp += 0.0006;
            }

            if (buttonopp <= 0.85)
            {
                buttonopp += 0.003;
            }

            if (mchildopp <= 0.3)
            {
                mchildopp += 0.001;
            }

            if (textopp <= 0.3)
            {
                textopp += 0.001;
                textopp = 1;
            }
        }

        void noopp()
        {
            mchildopp = 0;
            buttonopp = 0;
        }

        void begin(const char* name, float X, float Y) 
        {
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0941, 0.0941, 0.1059, oppacity));
            style.WindowRounding = 10.0f; //WINDOW ROUNDING
            style.FrameRounding = 5.0f; //FRAME ROUNDING FOR ALL ITEMS
            ImGui::Begin(name, NULL, window_flags);
            sws(X, Y);
        }
        
        void end()
        {
            ImGui::End();
        }

        void cText(const char* text, float posX, float posY, ImVec4 colortest, float op)
        {
            setpos(posX, posY);
            ImGui::TextColored(colortest, text);
        }
        
        void textc(std::string text, float posY) {
            auto windowWidth = ImGui::GetWindowSize().x;
            auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

            ImGui::SetCursorPos(ImVec2((windowWidth - textWidth) * 0.5f, posY));
            ImGui::Text(text.c_str());
        }

        bool button(const char* name, float sizeX, float sizeY, float posX, float posY)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0157, 0.3608, 0.5882, itemopp)); //NORMAL BUTTON COLOR
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0118, 0.5765, 0.8392, itemopp)); //HOVERED BUTTON COLOR
            SetCursorPos({ posX, posY });
            return ImGui::Button(name, ImVec2(sizeX, sizeY));
        }

        bool ibutton(const char* name, float posX, float posY, float sizeX, float sizeY, IDirect3DTexture9* texture, bool& isActive)
        {
            if (isActive) 
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0118, 0.5765, 0.8392, buttonopp));
            }

            else
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0157, 0.3608, 0.5882, buttonopp));
            }

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(sizeX, sizeY));
            setpos(posX, posY);
            bool pressed = ImGui::ImageButton(name, texture, ImVec2(32, 32));
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();

            if (pressed)
            {
                isActive = true;
            }

            return pressed;
        }
        
        void hintInput(const char* name, const char* hint, char test[], size_t size, float posX, float posY)
        {
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.23f, 0.23f, 0.23f, childopp)); //FOR INPUT TEXT BG
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 10));
            SetCursorPos({ posX, posY });

            ImGui::InputTextWithHint(name, hint, test, sizeof(test));
            TreePop();
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
        }
        
        void hintInputPass(const char* name, const char* hint, char test[], size_t size, float posX, float posY)
        {
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.23f, 0.23f, 0.23f, childopp)); //FOR INPUT TEXT BG
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 10));
            SetCursorPos({ posX, posY });

            ImGui::InputTextWithHint(name, hint, test, sizeof(test), ImGuiInputTextFlags_Password);
            TreePop();
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
        }

        void child(const char* name, float sizeX, float sizeY, float posX, float posY, float op)
        {
            style.ChildRounding = 10.0f; // CHILD ROUNDING
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.23, 0.23, 0.23, op)); //CHILD BG
            SetCursorPos({ posX, posY });
            ImGui::BeginChild(name, {sizeX, sizeY});
        }

        void endChild()
        {
            ImGui::EndChild();
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
        void particles() // <- pasted from https://github.com/dannybanno/imgui-particle-background
        {
            ImDrawList* drawList = ImGui::GetWindowDrawList();

            static const int numParticles = 140;
            static ImVec2 particlePositions[numParticles];
            static ImVec2 particleVelocities[numParticles];
            static bool initialized = false;

            static const ImVec2 drawAreaPos(10, 10); 
            static const ImVec2 drawAreaSize(1280, 960);

            if (!initialized) {
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
                for (int j = i + 1; j < numParticles; ++j) {
                    float distance = std::hypotf(particlePositions[j].x - particlePositions[i].x, particlePositions[j].y - particlePositions[i].y);
                    float opacity = 1.0f - (distance / 2.0f);

                    if (opacity > 0.0f) {
                        ImU32 lineColor = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, opacity));
                        drawList->AddLine(particlePositions[i], particlePositions[j], lineColor);
                    }
                }

                float distanceToCursor = std::hypotf(cursorPos.x - particlePositions[i].x, cursorPos.y - particlePositions[i].y);
                float opacityToCursor = 1.0f - (distanceToCursor / 30.0f); 

                if (opacityToCursor > 0.0f) {
                    ImU32 lineColorToCursor = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, opacityToCursor));
                    drawList->AddLine(cursorPos, particlePositions[i], lineColorToCursor);
                }
            }

            float deltaTime = ImGui::GetIO().DeltaTime;
            for (int i = 0; i < numParticles; ++i) {
                particlePositions[i].x += particleVelocities[i].x * deltaTime;
                particlePositions[i].y += particleVelocities[i].y * deltaTime;

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
