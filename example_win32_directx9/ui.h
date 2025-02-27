#pragma once
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

struct UIState {
    bool active;
    bool showFov;
    int aimPartcombo;
    int aimKeybindcombo;
    int comboModelcombobox;
    float smoothing;
    float aimStrength;
    float aimSpeed;
    ImVec4 menuColors;
    bool rcsEnabled;
    float rcsStrength;
    float rcsSmoothing;
    float rcsRandomization;
    bool rcsFirstShotRandom;
    bool autoPistol;
    bool miscFeatures[3];
    int configSelection;
    
    // Visual settings
    bool showVisuals;
    int mouseMethod;
    ImVec4 boxColor;
    ImVec4 filledBoxColor;
    bool showBox;
    bool fillBox;
    bool showHealthbar;
    bool showName;
    bool showDistance;
    float boxThickness;
    
    // Operator selection
    int teamSelection;
    int operatorSelection;
    int primaryWeapon;
    int secondaryWeapon;
    bool autoDetectLoadout;
    
    // Macros
    bool quickPeekEnabled;
    int quickPeekKey;
    bool dropShotEnabled;
    int dropShotKey;
    bool quickLeanEnabled;
    int quickLeanKey;
    float macroSpeed;
};

enum heads {
    HEAD_1,
    HEAD_2,
    HEAD_3,
    HEAD_4,
    HEAD_5
};

namespace fonts {
    extern ImFont* medium;
    extern ImFont* semibold;
    extern ImFont* logo;
}

void InitializeUI();
void RenderUI(bool& done);
void CleanupUI(); 