#include "ui.h"
#include "byte.h"
#include "elements.h"

namespace fonts {
    ImFont* medium;
    ImFont* semibold;
    ImFont* logo;
}

// String arrays for UI elements
static const char* teams[] = { "Attackers", "Defenders" };
static const char* attackers[] = { "Ace", "Ash", "Blackbeard", "Blitz", "Buck", "Capitao" };
static const char* defenders[] = { "Alibi", "Aruni", "Bandit", "Castle", "Caveira", "Doc" };
static const char* primaryWeapons[] = { "Auto-detect", "AK-12", "M4", "MP5" };
static const char* secondaryWeapons[] = { "Auto-detect", "P12", "5.7 USG", "PMM" };
static const char* macroKeys[] = { "C", "V", "X", "Alt", "Shift" };
static const char* mouseMethods[] = { "Logitech", "SendInput", "KMBox" };
static const char* configs[] = { "Default", "Ragge", "Good Chair", "250MPH" };
static const char* triggerModes[] = { "Pixel", "AI", "Glaz" };
static const char* triggerKeys[] = { "RMB", "LMB", "Ctrl", "X", "MMB" };
static const char* quickPeekTypes[] = { "Normal", "Shaiko", "Fast" };
static const char* crosshairShapes[] = { "Cross", "Dot", "T-Shape" };

UIState state;  // Default constructor will handle initialization

void StyleColorsByTheme() {
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.19f, 0.19f, 0.19f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.19f, 0.19f, 0.19f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));
}

void PopStyle() {
    ImGui::PopStyleColor(6);
    ImGui::PopStyleVar(2);
}

void InitializeUI() {
    ImGuiIO& io = ImGui::GetIO();
    
    ImFontConfig font_config;
    font_config.PixelSnapH = false;
    font_config.OversampleH = 5;
    font_config.OversampleV = 5;
    font_config.RasterizerMultiply = 1.2f;

    static const ImWchar ranges[] = {
        0x0020, 0x00FF,
        0x0400, 0x052F,
        0x2DE0, 0x2DFF,
        0xA640, 0xA69F,
        0xE000, 0xE226,
        0,
    };

    font_config.GlyphRanges = ranges;

    fonts::medium = io.Fonts->AddFontFromMemoryTTF(InterMedium, sizeof(InterMedium), 15.0f, &font_config, ranges);
    fonts::semibold = io.Fonts->AddFontFromMemoryTTF(InterSemiBold, sizeof(InterSemiBold), 17.0f, &font_config, ranges);
    fonts::logo = io.Fonts->AddFontFromMemoryTTF(catrine_logo, sizeof(catrine_logo), 17.0f, &font_config, ranges);
}

void RenderAimbotTab(ImDrawList* draw, const ImVec2& pos) {
    const char* aimPart[5] = { "Head", "Body", "Leg", "Mixed", "Custom" };
    const char* aimKeybind[5] = { "RMB", "LMB", "Ctrl", "X", "MMB" };
    const char* modelselect[3] = { "Eco", "Balanced", "Extreme" };
    const char* fovShapes[3] = { "Circle", "Rectangle", "Square" };

    draw->AddText(fonts::medium, 14.0f, ImVec2(pos.x + 25, pos.y + 60), 
        ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Aimbot Settings");
    draw->AddText(fonts::medium, 14.0f, ImVec2(pos.x + 285, pos.y + 60), 
        ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Model Settings");

    // Left column
    ImGui::SetCursorPos({ 25, 85 });
    ImGui::BeginChild("##leftcol", ImVec2(190, 240), false, ImGuiWindowFlags_NoScrollbar);
    {
        ImGui::Checkbox("Active", &state.active);
        ImGui::Checkbox("Show Fov", &state.showFov);
        ImGui::Combo("Aim part", &state.aimPartcombo, aimPart, IM_ARRAYSIZE(aimPart));
        
        if (state.aimPartcombo == 4) {
            ImGui::SliderFloat("X Offset %", &state.customXOffset, -100.0f, 100.0f, "%.1f%%");
            ImGui::SliderFloat("Y Offset %", &state.customYOffset, -100.0f, 100.0f, "%.1f%%");
        }
        
        ImGui::Combo("Aim keybind", &state.aimKeybindcombo, aimKeybind, IM_ARRAYSIZE(aimKeybind));
        ImGui::SliderFloat("Smoothing", &state.smoothing, 0.0f, 10.0f, "%.1f");
    }
    ImGui::EndChild();

    // Right column
    ImGui::SameLine(285);
    ImGui::BeginChild("##rightcol", ImVec2(190, 240), false, ImGuiWindowFlags_NoScrollbar);
    {
        ImGui::Combo("Model choice", &state.comboModelcombobox, modelselect, IM_ARRAYSIZE(modelselect));
        ImGui::SliderFloat("Aim speed", &state.aimSpeed, 0.0f, 100.0f, "%.1f");
        ImGui::SliderFloat("Aim strength", &state.aimStrength, 0.0f, 100.0f, "%.1f");
        ImGui::Separator();
        ImGui::Text("FOV Settings");
        ImGui::Combo("FOV Shape", (int*)&state.fovShape, fovShapes, IM_ARRAYSIZE(fovShapes));
        ImGui::SliderFloat("FOV Size", &state.fovSize, 10.0f, 500.0f, "%.1f");
        ImGui::ColorEdit4("FOV Color", (float*)&state.fovColor, 
            ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
    }
    ImGui::EndChild();
}

void RenderUI(bool&) {
    static heads head_selected = HEAD_1;
    
    ImGui::SetNextWindowSize({ 500, 370 });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    // Apply background color
    ImGui::PushStyleColor(ImGuiCol_WindowBg, state.backgroundColor);
    ImGui::PushStyleColor(ImGuiCol_Text, state.textColor);

    ImGui::Begin("Starlit", nullptr, ImGuiWindowFlags_NoDecoration);
    {
        auto draw = ImGui::GetWindowDrawList();
        auto pos = ImGui::GetWindowPos();
        auto size = ImGui::GetWindowSize();

        // Background and header
        draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + 51), 
            ImGui::ColorConvertFloat4ToU32(state.tabBackgroundColor), 9.0f, ImDrawFlags_RoundCornersTop);
        draw->AddRectFilledMultiColorRounded(pos, ImVec2(pos.x + 55, pos.y + 51), 
            ImColor(1.0f, 1.0f, 1.0f, 0.00f), ImColor(1.0f, 1.0f, 1.0f, 0.05f), 
            ImColor(1.0f, 1.0f, 1.0f, 0.00f), ImColor(1.0f, 1.0f, 1.0f, 0.00f), 
            ImColor(1.0f, 1.0f, 1.0f, 0.05f), 9.0f, ImDrawFlags_RoundCornersTopLeft);

        // Logo and text alignment
        draw->AddText(fonts::logo, 17.0f, ImVec2(pos.x + 25, pos.y + 19), 
            ImGui::ColorConvertFloat4ToU32(state.menuColors), "A");
        draw->AddText(fonts::semibold, 17.0f, ImVec2(pos.x + 49, pos.y + 19), 
            ImGui::ColorConvertFloat4ToU32(state.textColor), "Starlit");

        // Main tabs
        ImGui::SetCursorPos({ 125, 19 });
        ImGui::BeginGroup();
        {
            if (elements::tab("Aim", head_selected == HEAD_1)) head_selected = HEAD_1;
            ImGui::SameLine();
            if (elements::tab("RCS", head_selected == HEAD_2)) head_selected = HEAD_2;
            ImGui::SameLine();
            if (elements::tab("Misc", head_selected == HEAD_3)) head_selected = HEAD_3;
            ImGui::SameLine();
            if (elements::tab("Macros", head_selected == HEAD_4)) head_selected = HEAD_4;
            ImGui::SameLine();
            if (elements::tab("Settings", head_selected == HEAD_5)) head_selected = HEAD_5;

            // Draw subtle line under selected tab
            ImVec2 tabPos = ImGui::GetItemRectMin();
            ImVec2 tabSize = ImGui::GetItemRectSize();
            draw->AddRectFilled(
                ImVec2(tabPos.x, tabPos.y + tabSize.y),
                ImVec2(tabPos.x + tabSize.x, tabPos.y + tabSize.y + 1),
                ImGui::ColorConvertFloat4ToU32(ImVec4(state.menuColors.x, state.menuColors.y, state.menuColors.z, 0.3f))
            );
        }
        ImGui::EndGroup();

        StyleColorsByTheme();

        switch (head_selected) {
        case HEAD_1: // Aimbot
            ImGui::SetCursorPos({ 25, 60 });
            RenderAimbotTab(draw, pos);
            break;

        case HEAD_2: // RCS
            draw->AddText(fonts::medium, 14.0f, ImVec2(pos.x + 25, pos.y + 60), 
                ImGui::ColorConvertFloat4ToU32(state.textColor), "Recoil Control");

            ImGui::SetCursorPos({ 25, 85 });
            ImGui::BeginChild("##container", ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::Checkbox("Enable RCS", &state.rcsEnabled);
                ImGui::SliderFloat("RCS Strength", &state.rcsStrength, 0.0f, 10.0f, "%.1f");
                ImGui::SliderFloat("RCS Smoothing", &state.rcsSmoothing, 0.0f, 10.0f, "%.1f");
                ImGui::SliderFloat("Randomization", &state.rcsRandomization, 0.0f, 1.0f, "%.2f");
                ImGui::SliderFloat("Delay (ms)", &state.rcsDelay, 0.0f, 100.0f, "%.0f");
                ImGui::Checkbox("Random First Shot", &state.rcsFirstShotRandom);
                ImGui::Checkbox("Auto Pistol", &state.autoPistol);
            }
            ImGui::EndChild();

            draw->AddText(fonts::medium, 14.0f, ImVec2(pos.x + 285, pos.y + 60), 
                ImGui::ColorConvertFloat4ToU32(state.textColor), "Auto Detection");

            ImGui::SetCursorPos({ 285, 85 });
            ImGui::BeginChild("##container1", ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::Checkbox("Enable detection", &state.autoDetectionAlwaysOn);
                ImGui::SliderFloat("HUD Size", &state.autoDetectionHudSize, 0.0f, 100.0f, "%.0f");
                
                ImGui::Separator();
                ImGui::Text("Operator Selection");
                ImGui::Combo("Team", &state.teamSelection, teams, IM_ARRAYSIZE(teams));
                if (state.teamSelection == 0) {
                    ImGui::Combo("Operator", &state.operatorSelection, attackers, IM_ARRAYSIZE(attackers));
                } else {
                    ImGui::Combo("Operator", &state.operatorSelection, defenders, IM_ARRAYSIZE(defenders));
                }
                ImGui::Checkbox("Auto-detect loadout", &state.autoDetectLoadout);
                if (!state.autoDetectLoadout) {
                    ImGui::Combo("Primary", &state.primaryWeapon, primaryWeapons, IM_ARRAYSIZE(primaryWeapons));
                    ImGui::Combo("Secondary", &state.secondaryWeapon, secondaryWeapons, IM_ARRAYSIZE(secondaryWeapons));
                }
            }
            ImGui::EndChild();
            break;

        case HEAD_3: // Misc
            draw->AddText(fonts::medium, 14.0f, ImVec2(pos.x + 25, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Triggerbot");

            ImGui::SetCursorPos({ 25, 85 });
            ImGui::BeginChild("##container", ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::Checkbox("Active", &state.triggerActive);
                ImGui::Combo("Mode", (int*)&state.triggerMode, triggerModes, IM_ARRAYSIZE(triggerModes));
                ImGui::Combo("Key", &state.triggerKey, triggerKeys, IM_ARRAYSIZE(triggerKeys));
                ImGui::Checkbox("Use RCS", &state.triggerUseRCS);
                
                if (state.triggerMode == TRIGGER_PIXEL) {
                    ImGui::SliderFloat("Threshold", &state.triggerThreshold, 0.0f, 10.0f, "%.1f");
                    ImGui::SliderFloat("Delay (ms)", &state.triggerDelay, 0.0f, 100.0f, "%.0f");
                }
            }
            ImGui::EndChild();

            draw->AddText(fonts::medium, 14.0f, ImVec2(pos.x + 285, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Visuals");

            ImGui::SetCursorPos({ 285, 85 });
            ImGui::BeginChild("##container1", ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::Checkbox("Show Visuals", &state.showVisuals);
                ImGui::Checkbox("Head ESP", &state.showHeadESP);
                ImGui::Checkbox("Player ESP", &state.showPlayerESP);
                
                ImGui::Separator();
                ImGui::Text("Crosshair");
                ImGui::Combo("Shape", (int*)&state.crosshairShape, crosshairShapes, IM_ARRAYSIZE(crosshairShapes));
                ImGui::SliderFloat("Size", &state.crosshairSize, 2.0f, 20.0f, "%.0f");
            }
            ImGui::EndChild();
            break;

        case HEAD_4: // Macros
            draw->AddText(fonts::medium, 14.0f, ImVec2(pos.x + 25, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Quick Peek");

            ImGui::SetCursorPos({ 25, 85 });
            ImGui::BeginChild("##container", ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::Checkbox("Enable Quick Peek", &state.quickPeekEnabled);
                ImGui::Combo("Type", &state.quickPeekType, quickPeekTypes, IM_ARRAYSIZE(quickPeekTypes));
                ImGui::Combo("Key", &state.quickPeekKey, macroKeys, IM_ARRAYSIZE(macroKeys));
                ImGui::Checkbox("Repeat on Hold", &state.quickPeekRepeat);
                ImGui::Separator();
                ImGui::Checkbox("Enable Drop Shot", &state.dropShotEnabled);
                ImGui::Combo("Key", &state.dropShotKey, macroKeys, IM_ARRAYSIZE(macroKeys));
            }
            ImGui::EndChild();

            draw->AddText(fonts::medium, 14.0f, ImVec2(pos.x + 285, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Additional Macros");

            ImGui::SetCursorPos({ 285, 85 });
            ImGui::BeginChild("##container1", ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::Checkbox("Enable Quick Lean", &state.quickLeanEnabled);
                ImGui::Combo("Key", &state.quickLeanKey, macroKeys, IM_ARRAYSIZE(macroKeys));
                ImGui::SliderFloat("Macro Speed", &state.macroSpeed, 0.1f, 2.0f, "%.1fx");
                ImGui::SliderFloat("Delay (ms)", &state.macroDelay, 0.0f, 100.0f, "%.0f");
            }
            ImGui::EndChild();
            break;

        case HEAD_5: // Settings
            draw->AddText(fonts::medium, 14.0f, ImVec2(pos.x + 25, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Menu Settings");

            ImGui::SetCursorPos({ 25, 85 });
            ImGui::BeginChild("##container", ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::ColorEdit4("Accent##color", (float*)&state.menuColors, 
                    ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
                ImGui::ColorEdit4("Text##color", (float*)&state.textColor,
                    ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
                ImGui::ColorEdit4("Background##color", (float*)&state.backgroundColor,
                    ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
                ImGui::ColorEdit4("Tab Background##color", (float*)&state.tabBackgroundColor,
                    ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
                
                ImGui::Separator();
                ImGui::Combo("Config", &state.configSelection, configs, IM_ARRAYSIZE(configs));
                
                if (ImGui::Button("Save Config", ImVec2(-1, 0))) {
                    // Save config logic
                }
                if (ImGui::Button("Load Config", ImVec2(-1, 0))) {
                    // Load config logic
                }
            }
            ImGui::EndChild();

            draw->AddText(fonts::medium, 14.0f, ImVec2(pos.x + 285, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Additional Settings");

            ImGui::SetCursorPos({ 285, 85 });
            ImGui::BeginChild("##container1", ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::Combo("Mouse Method", &state.mouseMethod, mouseMethods, IM_ARRAYSIZE(mouseMethods));
                
                ImGui::Separator();
                ImGui::Text("Text to Speech");
                ImGui::Checkbox("Enable TTS", &state.textToSpeechEnabled);
                ImGui::SliderFloat("Volume", &state.textToSpeechVolume, 0.0f, 1.0f, "%.2f");
                
                ImGui::Separator();
                ImGui::Text("Created by fizz");
                ImGui::Text("Discord: YoungFizzler");
                ImGui::Text("Version: V1.0.1");
            }
            ImGui::EndChild();
            break;
        }

        PopStyle();
    }
    ImGui::End();
    ImGui::PopStyleColor(2);  // Pop background and text colors
    ImGui::PopStyleVar();
}

void CleanupUI() {
    // Ensure proper cleanup order
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    
    // Clear font pointers
    fonts::medium = nullptr;
    fonts::semibold = nullptr;
    fonts::logo = nullptr;
} 
