#pragma once
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <functional>
#include <format>
#include <thread>
#include <chrono>
#include "GL/glew.h"
#include "stb_image/stb_image.h"

class KitchenConfigurator;

#pragma warning(disable : 4996)
using namespace std::chrono_literals;
const float PI = 3.1415927;


// Timer For UI Events
class KitchenUITimer 
{
public:
    KitchenUITimer();
    ~KitchenUITimer();
    void start(double duration, std::function<void(std::stop_token)>& func);
    void stop();
private:
    std::jthread* thr;
};


const std::array<const std::string, 19> PART_NAMES {
    "CONTENT",		"DECO BOX",		"DECO PANEL",		"DIVIDER",
    "DRAWER",		"FACADE",		"FRAME",			"GAS PANEL",
    "MAIN BOX",	    "PLINTH",		"REFRIGERATOR",		"SMART DRAWER",
    "TABLE",		"WALLS",		"WASHER",			"FLOOR",
    "SINK",         "HEAT PANEL",   "COFFEE"
};


// UI Element Move Direction
enum class UI_MOVE_DIR {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

// Pixel Data
struct Pixel {
    friend class KitchenConfigurator;
public:
    stbi_uc R;
    stbi_uc G;
    stbi_uc B;
    stbi_uc A;
    // Getters
    stbi_uc getRed() { return R; }
    stbi_uc getGreen() { return G; }
    stbi_uc getBlue() { return B; }
    stbi_uc getAlpha() { return A; }
    bool isTransparent() { return A == 0; }
    // Setters
    void setRed(stbi_uc newRed) { R = newRed < 255 ? newRed : 255; }
    void setGreen(stbi_uc newGreen) { G = newGreen < 255 ? newGreen : 255; }
    void setBlue(stbi_uc newBlue) { B = newBlue < 255 ? newBlue : 255; }
    void setAlpha(stbi_uc newAlpha) { A = newAlpha < 255 ? newAlpha : 255; }
    bool operator==(const Pixel& pix) { return this->R == pix.R && this->G == pix.G && this->B == pix.B && this->A == pix.A; }
    bool operator!=(const Pixel& pix) { return !(this->R == pix.R && this->G == pix.G && this->B == pix.B && this->A == pix.A); }
};

// Image UI Data
struct KitchenUIElementImage {
    //friend class KitchenConfigurator;
public:
    KitchenUIElementImage(const std::string& src, GLenum frmt);
    ~KitchenUIElementImage();
    void drawImageUpperPart(GLint bottomRow);
    void drawImageRightPart(GLint leftColumn);
    void drawImageLeftPart(GLint rightColumn);
    void setColor(const Pixel& newColor);
    void setOpacity(int alpha);
private:
    std::string srcImage;
    GLenum format;
    GLint width, height, components;
    std::vector<Pixel> pixels;
    Pixel* operator[](size_t idx) { return pixels.data() + idx * width; }
    Pixel* data() { return pixels.data(); }
};


// UI Element State Enum
enum class UI_STATE 
{
    OVER,
    OVER_AND_PRESSED,
    OVER_AND_RELEASED,
    OUT
};


// Renderable Element With Active User Action
struct KitchenUIElement {
public:
    // CTOR Image Version
    KitchenUIElement(const std::string& name, double initPosX, double initPosY, size_t zindex = 0, const std::string& srcSimple = "", const std::string& srcHovered = "",
                                const std::string& srcPressed = "", const std::string& srcActivated = "", GLenum frmt = STBI_rgb_alpha);
    ~KitchenUIElement();
    void drawStatic(KitchenUIElement* over = nullptr, GLint leftCol = 0, GLint rightCol = 0);
    void hideElement();
    void unhideElement();
    void activateElement();
    void deactivateElement(double setOpac = 255);
    // Transforms
    void moveElement(double speed, UI_MOVE_DIR dir);
    void rotateElement(float angle, KitchenUIElement* over = nullptr, bool isFilterEnable = true);
    void scaleElement(double scaleKoef, bool isCentered = true);
    bool isCursorOverElement(double curX, double curY, bool isRectArea = false);
    int getPixelIdByCoord(double x, double y);
    std::pair<double, double> getPixelCoord(int idx);
    std::pair<double, double> getElementCoord(); // Returns Left Bottom Rect Coords Of Whole UI-Element
    std::pair<double, double> getElementScreenCoord(double pixX, double pixY);
    void setElementOpacity(double alpha);

public:
    std::string nameElement = "empty";
    std::string stateName = "NONE";
    struct Pos { double x; double y; } pos;
    struct Scale { double x = 1.f; double y = 1.f; } scale;
    float angle = 0.f;
    bool isVisible = true;
    bool isActive = true;
    size_t zIndex;
    KitchenUIElementImage* img_cur = nullptr;
    KitchenUIElementImage* img_simple = nullptr;
    KitchenUIElementImage* img_hovered = nullptr;
    KitchenUIElementImage* img_pressed = nullptr;
    KitchenUIElementImage* img_activated = nullptr;
    std::vector<KitchenUIElementImage*> img_alts;
    UI_STATE state = UI_STATE::OUT;
};


// Block Of Connected Elements
struct KitchenUIElementBlock {
public:
    KitchenUIElementBlock(bool isvisible, KitchenConfigurator* cofigurator);
    ~KitchenUIElementBlock();
    void unhideBlock();
    void hideBlock();
    virtual void render(double curx, double cury);
    void onCursorPositionUpdate(double curx, double cury);
    void onMouseButtonUpdate(int curx, int curY, int mouseButton, int mouseButtonState);

    virtual void onSimpled(KitchenUIElement* el, double curX, double curY);
    virtual void onHovered(KitchenUIElement* el, double curX, double curY);
    virtual void onPressed(KitchenUIElement* el, double curX, double curY);
    virtual void onReleased(KitchenUIElement* el, double curX, double curY);

    // Each Tick Updating Actions In Block
    virtual void updateBlock(double curx, double cury) {};

    void updateActiveUIElements();
public:
    std::vector<KitchenUIElement*> elements;
    bool isBlockVisible;
    KitchenConfigurator* configurator;

};

class UIMeshPanel;
struct UILoadingScreen;

enum class MSG_STATE {
    NONE,
    SHOWING
};

// Main Kitchen UI Class
class KitchenUI
{
    friend class KitchenConfigurator;
public:
	KitchenUI(size_t wwin, size_t hwin, KitchenConfigurator* cofigurator);
	~KitchenUI();
    void render();
    void moveElementForwardOnScreen(const std::string& elemName);

public:
    static inline double pixelKoefWidth = 0, pixelKoefHeight = 0;
    void showMessage(const std::string& msg);

private:
    void onFrameBufferUpdate(size_t w, size_t h);
    void onCursorPositionUpdate(double curx, double cury);
    void onMouseButtonUpdate(int mouseButton, int mouseButtonState);
private:
    double curX, curY;
    size_t wWin, hWin; 
    std::vector<KitchenUIElementBlock*> blocks;
    KitchenConfigurator* cofigurator;
    UILoadingScreen* loadingScreenBlock;
    MSG_STATE showState = MSG_STATE::NONE;
};

struct UICornerButton;
struct UIBottomPanel;
struct UIGenerateModulePanel;
struct UITextBig;
struct UITextInfo;
struct UITextWarning;
struct UIButtonCargo;
struct UIButtonSides;
struct UIScalePanel;
struct UIHandlePanel;
struct UIMeshPanel;
struct UIRotationPanel;
struct UIMaterialPanel;
struct UIDividerPanel;
struct UIMainSelectPanel;
struct UICargoContentPanel;
struct UIDrawerSidePanel;
class KitchenUITimer;

enum class AnimationState 
{
    HIDDEN,
    SHOWING,
    SHOWED,
    HIDING
};

// ---------------------------------------- CONCRETE  BLOCKS  -------------------------------------- //
// 0. Loading Screen
struct UILoadingScreen : KitchenUIElementBlock {
public:
    UILoadingScreen(KitchenConfigurator* cofigurator);
    ~UILoadingScreen();
    void render(double curx, double cury) override;
    void updateProgressBar();

public:
    KitchenUIElement* bg;
    KitchenUIElement* text;
    KitchenUIElement* progressBarGrey;
    KitchenUIElement* progressBarBlue;
    KitchenUIElement* progressBarCur;
};


// 1. Corner Button
struct UICornerButton : KitchenUIElementBlock {
public:
    UICornerButton(KitchenConfigurator* cofigurator);
    ~UICornerButton();
    void render(double curx, double cury) override;

    void onSimpled(KitchenUIElement* el, double curX, double curY) override;
    void onHovered(KitchenUIElement* el, double curX, double curY) override;
    void onPressed(KitchenUIElement* el, double curX, double curY) override;
    void onReleased(KitchenUIElement* el, double curX, double curY) override;

    void updateLightness(double curx, double cury);
    void updateBlock(double curx, double cury) override;

    void animate();

public:
    KitchenUIElement* buttonRoom;
    KitchenUIElement* lightProgressbar;
    KitchenUIElement* staticCircle;
    KitchenUIElement* buttonView;
    KitchenUIElement* buttonRender;
    KitchenUIElement* buttonSound;
    KitchenUIElement* buttonExport;
    KitchenUIElement* buttonExit;
    KitchenUIElement* bgDots;
    std::array<KitchenUIElement*, 5> buttons;

    double dotsOpac = 0.0;
    AnimationState animState = AnimationState::HIDDEN;
    struct AnimParams {
        std::pair<double, double> posStart;
        std::pair<double, double> posEnd;
        double opac = 0.0;
        AnimationState animState = AnimationState::HIDDEN;
    };
    std::array<AnimParams, 5> animParams;
};

// 2. Bottom Panel
struct UIBottomPanel : KitchenUIElementBlock {
public:
    UIBottomPanel(KitchenConfigurator* cofigurator);
    ~UIBottomPanel();
    void render(double curx, double cury) override;
    
    void onSimpled(KitchenUIElement* el, double curX, double curY) override;
    void onHovered(KitchenUIElement* el, double curX, double curY) override;
    void onPressed(KitchenUIElement* el, double curX, double curY) override;
    void onReleased(KitchenUIElement* el, double curX, double curY) override;

public:
    KitchenUIElement* fixedPanel;
    KitchenUIElement* buttonGenerate;
    KitchenUIElement* checkboxRotation;
    KitchenUIElement* checkboxHandle;
    KitchenUIElement* checkboxMesh;
    KitchenUIElement* checkboxMaterial;
    KitchenUIElement* checkboxScale;
    KitchenUIElement* checkboxExtra;
    KitchenUIElement* checkboxTexts;
    KitchenUIElement* checkboxWarnings;

    UIGenerateModulePanel* panelGenModule;
    UITextInfo* panelTextInfo;
    UITextWarning* panelTextWarning;
    UIButtonCargo* panelButtonCargo;
    UIButtonSides* panelButtonSides;
    UICargoContentPanel* panelCargo;
    UIDrawerSidePanel* panelSides;
    UIScalePanel* panelScale;
    UIHandlePanel* panelHandle;
    UIMeshPanel* panelMesh;
    UIRotationPanel* panelRotation;
    UIMaterialPanel* panelMaterial;
    UIDividerPanel* panelDivider;
    UITextBig* bigText;
};

// 3. Deco Big Text
struct UITextBig : KitchenUIElementBlock {
public:
    enum class BIG_TEXT_STATE {
        CREATE, SELECT, CONFIG
    };
    UITextBig(KitchenConfigurator* cofigurator);
    ~UITextBig();
    void render(double curx, double cury) override;
    void changeState(BIG_TEXT_STATE newState);

public:
    std::array<KitchenUIElement*, 3> texts;
    KitchenUIElement* text;
    BIG_TEXT_STATE state = BIG_TEXT_STATE::CREATE;
};

// 4. Warning Text
struct UITextWarning : KitchenUIElementBlock {
public:
    UITextWarning(KitchenConfigurator* cofigurator);
    ~UITextWarning();
    void render(double curx, double cury) override;
    void animate(double animSpeedOpacity, double animSpeed);
    void reset();

public:
    KitchenUIElement* curText;
    std::vector<KitchenUIElement*> infoTexts;
    AnimationState animState = AnimationState::HIDDEN;
    double opac = 0.0;
    double translate = 0.4;
    const double translateMAX = 0.4;
    std::pair<int, int> initPos;
    double fixedDeltaTime = 2.5;
    std::chrono::system_clock::time_point time_start;
};

// 5. Info Text
struct UITextInfo : KitchenUIElementBlock {
public:
    UITextInfo(KitchenConfigurator* cofigurator);
    ~UITextInfo();
    void render(double curx, double cury) override;
    void animate(double animSpeedOpacity, double animSpeed);
    void reset();

public:
    KitchenUIElement* curText;
    std::vector<KitchenUIElement*> infoTexts;
    AnimationState animState = AnimationState::HIDDEN;
    double opac = 0.0;
    double translate = 0.4;
    const double translateMAX = 0.4;
    std::pair<int, int> initPos;
    double fixedDeltaTime = 4.9;
    std::chrono::system_clock::time_point time_start;
};

// 6. Button Cargo Content
struct UIButtonCargo : KitchenUIElementBlock {
public:
    UIButtonCargo(KitchenConfigurator* cofigurator);
    ~UIButtonCargo();
    void render(double curx, double cury) override;

    void onSimpled(KitchenUIElement* el, double curX, double curY) override;
    void onHovered(KitchenUIElement* el, double curX, double curY) override;
    void onPressed(KitchenUIElement* el, double curX, double curY) override;
    void onReleased(KitchenUIElement* el, double curX, double curY) override;

public:
    KitchenUIElement* button;
    UICargoContentPanel* panelGargoContent;
};

// 7. Button SlideBox Sides
struct UIButtonSides : KitchenUIElementBlock {
public:
    UIButtonSides(KitchenConfigurator* cofigurator);
    ~UIButtonSides();
    void render(double curx, double cury) override;

    void onSimpled(KitchenUIElement* el, double curX, double curY) override;
    void onHovered(KitchenUIElement* el, double curX, double curY) override;
    void onPressed(KitchenUIElement* el, double curX, double curY) override;
    void onReleased(KitchenUIElement* el, double curX, double curY) override;

public:
    KitchenUIElement* button;
    UIDrawerSidePanel* panelDrawerSide;
};

// 8. Scale Panel
struct UIScalePanel : KitchenUIElementBlock {
public:
    UIScalePanel(KitchenConfigurator* cofigurator);
    ~UIScalePanel();
    void render(double curx, double cury) override;

    void onSimpled(KitchenUIElement* el, double curX, double curY) override;
    void onHovered(KitchenUIElement* el, double curX, double curY) override;
    void onPressed(KitchenUIElement* el, double curX, double curY) override;
    void onReleased(KitchenUIElement* el, double curX, double curY) override;

public:
    KitchenUIElement* buttonPlusDepth;
    KitchenUIElement* buttonPlusWidth;
    KitchenUIElement* buttonPlusHeight;
    KitchenUIElement* buttonMinusDepth;
    KitchenUIElement* buttonMinusWidth;
    KitchenUIElement* buttonMinusHeight;
    KitchenUIElement* textDepth;
    KitchenUIElement* textWidth;
    KitchenUIElement* textHeight;
    KitchenUIElement* textHeader;
};

struct ISelectedRotateable {
    void animate(double rotSpeed, KitchenUIElement* selector, KitchenUIElement* back);
    double ang = 0.0;
};

// 9. Handle Panel
struct UIHandlePanel : KitchenUIElementBlock, ISelectedRotateable {
public:
    UIHandlePanel(KitchenConfigurator* cofigurator);
    ~UIHandlePanel();
    void render(double curx, double cury) override;

    void onSimpled(KitchenUIElement* el, double curX, double curY) override;
    void onHovered(KitchenUIElement* el, double curX, double curY) override;
    void onPressed(KitchenUIElement* el, double curX, double curY) override;
    void onReleased(KitchenUIElement* el, double curX, double curY) override;

public:
    KitchenUIElement* textHeader;
    KitchenUIElement* buttonOn;
    KitchenUIElement* buttonOff;
    KitchenUIElement* selector;
    KitchenUIElement* activeButton;
};

// 10. Mesh Panel
struct UIMeshPanel : KitchenUIElementBlock, ISelectedRotateable {
public:
    UIMeshPanel(KitchenConfigurator* cofigurator);
    ~UIMeshPanel();
    void render(double curx, double cury) override;

    void onSimpled(KitchenUIElement* el, double curX, double curY) override;
    void onHovered(KitchenUIElement* el, double curX, double curY) override;
    void onPressed(KitchenUIElement* el, double curX, double curY) override;
    void onReleased(KitchenUIElement* el, double curX, double curY) override;

public:
    KitchenUIElement* textHeader;
    KitchenUIElement* buttonMeshSimple;
    KitchenUIElement* buttonMeshChamfer;
    KitchenUIElement* buttonMeshBevel;
    KitchenUIElement* selector;
    KitchenUIElement* activeButton;
};

// 11. Rotation Panel
struct UIRotationPanel : KitchenUIElementBlock, ISelectedRotateable {
public:
    UIRotationPanel(KitchenConfigurator* cofigurator);
    ~UIRotationPanel();
    void render(double curx, double cury) override;

    void onSimpled(KitchenUIElement* el, double curX, double curY) override;
    void onHovered(KitchenUIElement* el, double curX, double curY) override;
    void onPressed(KitchenUIElement* el, double curX, double curY) override;
    void onReleased(KitchenUIElement* el, double curX, double curY) override;

public:
    KitchenUIElement* textHeader;
    KitchenUIElement* buttonRotationLeft;
    KitchenUIElement* buttonRotationRight;
    KitchenUIElement* buttonRotationUp;
    KitchenUIElement* buttonRotationUpPaired;
    KitchenUIElement* selector;
    KitchenUIElement* activeButton;
};

// 12. Material Panel
struct UIMaterialPanel : KitchenUIElementBlock, ISelectedRotateable {
public:
    UIMaterialPanel(KitchenConfigurator* cofigurator);
    ~UIMaterialPanel();
    void render(double curx, double cury) override;

    void onSimpled(KitchenUIElement* el, double curX, double curY) override;
    void onHovered(KitchenUIElement* el, double curX, double curY) override;
    void onPressed(KitchenUIElement* el, double curX, double curY) override;
    void onReleased(KitchenUIElement* el, double curX, double curY) override;

public:
    KitchenUIElement* textHeader;
    KitchenUIElement* selector;
    std::vector<KitchenUIElement*> buttonMaterials;
    KitchenUIElement* activeButton;
};

// 13. Divider Panel
struct UIDividerPanel : KitchenUIElementBlock, ISelectedRotateable {
public:
    UIDividerPanel(KitchenConfigurator* cofigurator);
    ~UIDividerPanel();
    void render(double curx, double cury) override;

    void onSimpled(KitchenUIElement* el, double curX, double curY) override;
    void onHovered(KitchenUIElement* el, double curX, double curY) override;
    void onPressed(KitchenUIElement* el, double curX, double curY) override;
    void onReleased(KitchenUIElement* el, double curX, double curY) override;

public:
    KitchenUIElement* textHeader;
    KitchenUIElement* selector;
    std::vector<KitchenUIElement*> buttonDividers;
    KitchenUIElement* activeButton;
};

// 14. Main Select Panel
struct UIMainSelectPanel : KitchenUIElementBlock {
public:
    UIMainSelectPanel(KitchenConfigurator* cofigurator);
    ~UIMainSelectPanel();
    void render(double curx, double cury) override;

public:
    KitchenUIElement* textDescriptor;
    KitchenUIElement* buttonEnterModule;
    KitchenUIElement* buttonLeaveModule;
    KitchenUIElement* bgDots;
    KitchenUIElement* buttonNext;
    KitchenUIElement* buttonPrev;
};

// 15. Cargo Content Panel
struct UICargoContentPanel : KitchenUIElementBlock {
public:
    UICargoContentPanel(KitchenConfigurator* cofigurator);
    ~UICargoContentPanel();
    void render(double curx, double cury) override;

    void onSimpled(KitchenUIElement* el, double curX, double curY) override;
    void onHovered(KitchenUIElement* el, double curX, double curY) override;
    void onPressed(KitchenUIElement* el, double curX, double curY) override;
    void onReleased(KitchenUIElement* el, double curX, double curY) override;

public:
    std::array<KitchenUIElement*, 4> buttonContents;
    KitchenUIElement* bgDots;
    KitchenUIElement* activeButton;

    UIMaterialPanel* matPanel;
    UIMeshPanel* meshPanel;
};

// 16. DrawerSide Panel
struct UIDrawerSidePanel : KitchenUIElementBlock, ISelectedRotateable {
public:
    UIDrawerSidePanel(KitchenConfigurator* cofigurator);
    ~UIDrawerSidePanel();
    void render(double curx, double cury) override;

    void onSimpled(KitchenUIElement* el, double curX, double curY) override;
    void onHovered(KitchenUIElement* el, double curX, double curY) override;
    void onPressed(KitchenUIElement* el, double curX, double curY) override;
    void onReleased(KitchenUIElement* el, double curX, double curY) override;

public:
    std::array<KitchenUIElement*, 3> buttonSides;
    KitchenUIElement* activeButton;
    KitchenUIElement* selector;
};

// 17. Generate Module Panel
struct UIGenerateModulePanel : KitchenUIElementBlock {
public:
    UIGenerateModulePanel(KitchenConfigurator* cofigurator);
    ~UIGenerateModulePanel();
    void render(double curx, double cury) override;

    void onSimpled(KitchenUIElement* el, double curX, double curY) override;
    void onHovered(KitchenUIElement* el, double curX, double curY) override;
    void onPressed(KitchenUIElement* el, double curX, double curY) override;
    void onReleased(KitchenUIElement* el, double curX, double curY) override;

public:
    struct SixtoBlock {
        KitchenUIElement* modInfoRect;
        KitchenUIElement* button;
    };
    std::array<SixtoBlock*, 24> allBlocks;
    std::array<SixtoBlock*, 6> curSixtoBlocks;
    KitchenUIElement* buttonNext;
    KitchenUIElement* buttonPrev;
    KitchenUIElement* bg;

    size_t curSixtoId = 1;
};

// 18. Select Module/Part Panel
struct UISelectModulePanel : KitchenUIElementBlock {
public:
    UISelectModulePanel(KitchenConfigurator* cofigurator);
    ~UISelectModulePanel();
    void render(double curx, double cury) override;

    void onSimpled(KitchenUIElement* el, double curX, double curY) override;
    void onHovered(KitchenUIElement* el, double curX, double curY) override;
    void onPressed(KitchenUIElement* el, double curX, double curY) override;
    void onReleased(KitchenUIElement* el, double curX, double curY) override;

public:
    KitchenUIElement* bgDots;
    KitchenUIElement* buttonNext;
    KitchenUIElement* buttonPrev;
    KitchenUIElement* buttonEnter;
    KitchenUIElement* buttonLeave;
    KitchenUIElement* curButton;
    std::array<KitchenUIElement*, 21> moduleNameTexts;
    std::array<KitchenUIElement*, 19> partNameTexts;
    KitchenUIElement* curNameText;

    bool isActiveEnter = true;
    int curId = 0;
    
    // Links To Another UI Elements
    UIButtonCargo* panelButtonCargo;
    UIButtonSides* panelButtonSides;
    UIScalePanel* panelScale;
    UIHandlePanel* panelHandle;
    UIDrawerSidePanel* panelSides;
    UIMeshPanel* panelMesh;
    UIRotationPanel* panelRotation;
    UIMaterialPanel* panelMaterial;
    UIDividerPanel* panelDivider;
    UIBottomPanel* panelBottomMain;
    UITextBig* bigText;
};