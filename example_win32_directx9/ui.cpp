#include "ui.h"
#include "byte.h"
#include "elements.h"

namespace fonts {
    ImFont* medium;
    ImFont* semibold;
    ImFont* logo;
}

static UIState state = {
    false, false, 0, 0, 0,
    8.0f, 8.0f, 8.0f,
    {1.0f, 1.0f, 1.0f},
    false, 5.0f, 3.0f,
    {false, false, false},
    0,
    0, 0, 0, 0, true,
    false, 0, false, 0, false, 0, 1.0f
};

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

void RenderUI(bool&) {
    static heads head_selected = HEAD_1;
    const char* aimPart[4] = { "Head", "Body", "Leg", "Mixed" };
    const char* aimKeybind[5] = { "RMB", "LMB", "Ctrl", "X", "MMB" };
    const char* modelselect[3] = { "Eco", "Balanced", "Extreme" };
    const char* configs[4] = { "Default", "Aggressive", "Legit", "Custom" };
    const char* teams[2] = { "Attackers", "Defenders" };
    const char* attackers[6] = { "Ace", "Ash", "Blackbeard", "Blitz", "Buck", "Capitao" };
    const char* defenders[6] = { "Alibi", "Aruni", "Bandit", "Castle", "Caveira", "Doc" };
    const char* primaryWeapons[4] = { "Auto-detect", "AK-12", "M4", "MP5" };
    const char* secondaryWeapons[4] = { "Auto-detect", "P12", "5.7 USG", "PMM" };
    const char* macroKeys[5] = { "C", "V", "X", "Alt", "Shift" };

    ImGui::SetNextWindowSize({ 500, 370 });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    ImGui::Begin("Starlit", nullptr, ImGuiWindowFlags_NoDecoration);
    {
        auto draw = ImGui::GetWindowDrawList();
        auto pos = ImGui::GetWindowPos();
        auto size = ImGui::GetWindowSize();

        draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + 51), ImColor(24, 24, 24), 9.0f, ImDrawFlags_RoundCornersTop);
        draw->AddRectFilledMultiColorRounded(pos, ImVec2(pos.x + 55, pos.y + 51), ImColor(1.0f, 1.0f, 1.0f, 0.00f), ImColor(1.0f, 1.0f, 1.0f, 0.05f), ImColor(1.0f, 1.0f, 1.0f, 0.00f), ImColor(1.0f, 1.0f, 1.0f, 0.00f), ImColor(1.0f, 1.0f, 1.0f, 0.05f), 9.0f, ImDrawFlags_RoundCornersTopLeft);

        draw->AddText(fonts::logo, 17.0f, ImVec2(pos.x + 25, pos.y + 17), ImColor(192, 203, 229), "A");
        draw->AddText(fonts::semibold, 17.0f, ImVec2(pos.x + 49, pos.y + 18), ImColor(192, 203, 229), "Starlit");

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
        }
        ImGui::EndGroup();

        switch (head_selected) {
        case HEAD_1: // Aimbot
            draw->AddText(fonts::medium, 14.0f, ImVec2(pos.x + 25, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Aimbot");

            ImGui::SetCursorPos({ 25, 85 });
            ImGui::BeginChild("##container", ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::Checkbox("Active", &state.active);
                ImGui::Checkbox("Show Fov", &state.showFov);
                ImGui::Combo("Aim part", &state.aimPartcombo, aimPart, IM_ARRAYSIZE(aimPart));
                ImGui::Combo("Aim keybind", &state.aimKeybindcombo, aimKeybind, IM_ARRAYSIZE(aimKeybind));
                ImGui::SliderFloat("Smoothing", &state.smoothing, 0.0f, 10.0f, "%.1f");
                ImGui::Text("Made by fizz :3");
            }
            ImGui::EndChild();

            draw->AddText(fonts::medium, 14.0f, ImVec2(pos.x + 285, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Advanced options");

            ImGui::SetCursorPos({ 285, 85 });
            ImGui::BeginChild("##container1", ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::Combo("Model choice", &state.comboModelcombobox, modelselect, IM_ARRAYSIZE(modelselect));
                ImGui::SliderFloat("Aim speed", &state.aimSpeed, 0.0f, 100.0f, "%.1f");
                ImGui::SliderFloat("Aim strength", &state.aimStrength, 0.0f, 100.0f, "%.1f");
            }
            ImGui::EndChild();
            break;

        case HEAD_2: // RCS
            draw->AddText(fonts::medium, 14.0f, ImVec2(pos.x + 25, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Recoil Control");

            ImGui::SetCursorPos({ 25, 85 });
            ImGui::BeginChild("##container", ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::Checkbox("Enable RCS", &state.rcsEnabled);
                ImGui::SliderFloat("RCS Strength", &state.rcsStrength, 0.0f, 10.0f, "%.1f");
                ImGui::SliderFloat("RCS Smoothing", &state.rcsSmoothing, 0.0f, 10.0f, "%.1f");
            }
            ImGui::EndChild();

            draw->AddText(fonts::medium, 14.0f, ImVec2(pos.x + 285, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Operator Selection");

            ImGui::SetCursorPos({ 285, 85 });
            ImGui::BeginChild("##container1", ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar);
            {
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
            draw->AddText(fonts::medium, 14.0f, ImVec2(pos.x + 25, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Features");

            ImGui::SetCursorPos({ 25, 85 });
            ImGui::BeginChild("##container", ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::Checkbox("No Flash", &state.miscFeatures[0]);
                ImGui::Checkbox("Bunny Hop", &state.miscFeatures[1]);
                ImGui::Checkbox("Radar", &state.miscFeatures[2]);
            }
            ImGui::EndChild();

            draw->AddText(fonts::medium, 14.0f, ImVec2(pos.x + 285, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Information");

            ImGui::SetCursorPos({ 285, 85 });
            ImGui::BeginChild("##container1", ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
                ImGui::Text("Latency: 15ms");
                ImGui::Separator();
                ImGui::Text("Status: Undetected");
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Last updated: Today");
            }
            ImGui::EndChild();
            break;

        case HEAD_4: // Macros
            draw->AddText(fonts::medium, 14.0f, ImVec2(pos.x + 25, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Quick Peek");

            ImGui::SetCursorPos({ 25, 85 });
            ImGui::BeginChild("##container", ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::Checkbox("Enable Quick Peek", &state.quickPeekEnabled);
                ImGui::Combo("Key##qp", &state.quickPeekKey, macroKeys, IM_ARRAYSIZE(macroKeys));
                ImGui::Checkbox("Enable Drop Shot", &state.dropShotEnabled);
                ImGui::Combo("Key##ds", &state.dropShotKey, macroKeys, IM_ARRAYSIZE(macroKeys));
                ImGui::Checkbox("Enable Quick Lean", &state.quickLeanEnabled);
                ImGui::Combo("Key##ql", &state.quickLeanKey, macroKeys, IM_ARRAYSIZE(macroKeys));
                ImGui::SliderFloat("Macro Speed", &state.macroSpeed, 0.1f, 2.0f, "%.1fx");
            }
            ImGui::EndChild();

            draw->AddText(fonts::medium, 14.0f, ImVec2(pos.x + 285, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Macro Status");

            ImGui::SetCursorPos({ 285, 85 });
            ImGui::BeginChild("##container1", ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::Text("Active Macros:");
                if (state.quickPeekEnabled)
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Quick Peek (%s)", macroKeys[state.quickPeekKey]);
                if (state.dropShotEnabled)
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Drop Shot (%s)", macroKeys[state.dropShotKey]);
                if (state.quickLeanEnabled)
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Quick Lean (%s)", macroKeys[state.quickLeanKey]);
            }
            ImGui::EndChild();
            break;

        case HEAD_5: // Settings
            draw->AddText(fonts::medium, 14.0f, ImVec2(pos.x + 25, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Menu Settings");

            ImGui::SetCursorPos({ 25, 85 });
            ImGui::BeginChild("##container", ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::Text("Menu Colors");
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.19f, 0.19f, 0.19f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
                ImGui::ColorEdit3("##Accent", state.menuColors, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                ImGui::PopStyleColor(3);
                ImGui::Separator();
                ImGui::Text("Configuration");
                ImGui::Combo("##Config", &state.configSelection, configs, IM_ARRAYSIZE(configs));
                
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.19f, 0.19f, 0.19f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
                if (ImGui::Button("Save Config", ImVec2(-1, 0))) {
                    // Save config logic
                }
                if (ImGui::Button("Load Config", ImVec2(-1, 0))) {
                    // Load config logic
                }
                ImGui::PopStyleColor(3);
            }
            ImGui::EndChild();

            draw->AddText(fonts::medium, 14.0f, ImVec2(pos.x + 285, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "About");

            ImGui::SetCursorPos({ 285, 85 });
            ImGui::BeginChild("##container1", ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::Text("Starlit v1.0");
                ImGui::Text("Build: 2024.03.14");
                ImGui::Separator();
                ImGui::Text("Created by fizz");
                ImGui::Text("Discord: fizz#1337");
                ImGui::Separator();
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.19f, 0.19f, 0.19f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
                if (ImGui::Button("Check for updates", ImVec2(-1, 0))) {
                    // Update check logic
                }
                ImGui::PopStyleColor(3);
            }
            ImGui::EndChild();
            break;
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void CleanupUI() {
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
} 