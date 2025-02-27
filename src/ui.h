#pragma once
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

enum FovShape {
    FOV_CIRCLE,
    RECTANGLE,
    SQUARE
};

enum CrosshairShape {
    CROSSHAIR_CROSS,
    CROSSHAIR_DOT,
    CROSSHAIR_T
};

enum TriggerMode {
    TRIGGER_PIXEL,
    TRIGGER_AI,
    TRIGGER_GLAZ
};

enum heads {
    HEAD_1,
    HEAD_2,
    HEAD_3,
    HEAD_4,
    HEAD_5
};

struct UIState {
    bool active = false;
    bool showFov = false;
    int aimPartcombo = 0;
    int aimKeybindcombo = 0;
    float smoothing = 2.0f;
    int comboModelcombobox = 0;
    float aimSpeed = 50.0f;
    float aimStrength = 50.0f;
    float customXOffset = 0.0f;
    float customYOffset = 0.0f;
    FovShape fovShape = FOV_CIRCLE;
    float fovSize = 100.0f;
    ImVec4 fovColor = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
    
    bool triggerActive = false;
    TriggerMode triggerMode = TRIGGER_PIXEL;
    int triggerKey = 0;
    bool triggerUseRCS = false;
    float triggerThreshold = 5.0f;
    float triggerDelay = 0.0f;
    
    ImVec4 menuColors = ImVec4(0.4f, 0.4f, 1.0f, 1.0f);
    ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 backgroundColor = ImVec4(0.06f, 0.06f, 0.06f, 1.0f);
    ImVec4 tabBackgroundColor = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    
    bool rcsEnabled = false;
    float rcsStrength = 1.0f;
    float rcsSmoothing = 1.0f;
    float rcsRandomization = 0.0f;
    float rcsDelay = 0.0f;
    bool rcsFirstShotRandom = false;
    bool autoPistol = false;
    bool miscFeatures[3] = {false, false, false};
    int configSelection = 0;

    // Auto Detection settings
    bool autoDetectionAlwaysOn = false;
    int autoDetectionKey = 0;
    float autoDetectionHudSize = 50.0f;

    bool showVisuals = true;
    bool showHeadESP = true;
    bool showPlayerESP = true;
    int mouseMethod = 0;

    int teamSelection = 0;
    int operatorSelection = 0;
    int primaryWeapon = 0;
    int secondaryWeapon = 0;
    bool autoDetectLoadout = true;
    
    bool quickPeekEnabled = false;
    int quickPeekKey = 0;
    int quickPeekType = 0;  // 0 = Normal, 1 = Shaiko, 2 = Fast
    bool quickPeekRepeat = false;
    bool dropShotEnabled = false;
    int dropShotKey = 0;
    bool quickLeanEnabled = false;
    int quickLeanKey = 0;
    float macroSpeed = 1.0f;

    // User related state
    bool showUserPanel = false;
    int daysLeft = 30;
    char username[32] = "YoungFizzler";
    bool isPremium = true;
    bool canShareConfigs = true;

    CrosshairShape crosshairShape = CROSSHAIR_CROSS;
    float crosshairSize = 6.0f;
    float crosshairOpacity = 1.0f;

    // Text to speech settings
    bool textToSpeechEnabled = false;
    float textToSpeechVolume = 1.0f;
    
    // Macro delay
    float macroDelay = 0.0f;
};

extern UIState state;

namespace fonts {
    extern ImFont* medium;
    extern ImFont* semibold;
    extern ImFont* logo;
}

void InitializeUI();
void RenderUI(bool& done);
void CleanupUI(); 
