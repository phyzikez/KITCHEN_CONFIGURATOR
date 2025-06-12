#pragma once
#define GLEW_STATIC

#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include <list>
#include <string>
#include <format>
#include "GL/glew.h"
#include "gl/GL.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Sound
#include "irr/irrKlang.h"
#pragma comment(lib, "irrKlang.lib")

// Base
#include "KitchenCamera.h"
#include "KitchenMesh.h"
#include "KitchenShader.h"
#include "KitchenTexture.h"
#include "KitchenMaterial.h"
// Modules
#include "ModuleRoom.h"
#include "ModuleStove.h"
#include "ModuleBigCargo.h"
#include "ModuleBigRefrigerator.h"
#include "ModuleBottomCargo.h"
#include "ModuleBottomBox.h"
#include "ModuleBottomSmart.h"
#include "ModuleBottomWasher.h"
#include "ModuleTable.h"
#include "ModuleUpperCargo.h"
#include "ModuleUpperCornerLeft.h"
#include "ModuleUpperCornerRight.h"
#include "ModuleUpperShellsNarrow.h"
#include "ModuleUpperShellsSide.h"
#include "ModuleUpperShell.h"
#include "ModulePlinths.h"

// UI
#include "KitchenUI.h"

// CubeBox
#include "Skybox.h"

enum class SOUND 
{
	INTRO,
	START,
	HOVER_1,
	HOVER_2,
	BUTTON_PRESS_1,
	BUTTON_PRESS_2,
	BUTTON_PRESS_3,
	BUTTON_PRESS_4,
	BUTTON_PRESS_5,
	BUTTON_PRESS_6,
	BUTTON_PRESS_7,
	BUTTON_PRESS_8,
	BUTTON_PRESS_9,
	BUTTON_PRESS_10,
	SWITCH_1,
	SWITCH_2,
	SWITCH_3,
	INFO,
	WARNING,
	TURN,
	OPEN_CLOSE,
};

// UI Open Functionality
struct UIFunctionality {
	virtual void changeRoomLightness(double newVal) = 0; 
	virtual void switchRenderMode() = 0;
	virtual bool isModuleEnable(size_t id) = 0;
	virtual void selectModule(size_t id) = 0;
	virtual size_t getTotalModuleCount() = 0;
	virtual void activateModule(size_t id) = 0;
	virtual void deactivateModule(size_t id) = 0;
	virtual KitchenModule* getCurModule() = 0;
	virtual glm::vec3 getCameraPosition() = 0;
	virtual void setNewCameraPosition(const POINT_3D& newPos) = 0;
};

// Main Management Class
class KitchenConfigurator : UIFunctionality
{
public:
	KitchenConfigurator();
	~KitchenConfigurator();
	static void initGL();
	static void onFrameBufSize(GLFWwindow* win, int w, int h);
	static void onMouseScroll(GLFWwindow* window, double deltaX, double deltaY);
	static void onCursorMove(GLFWwindow* window, double xpos, double ypos);
	static void onMouseButtonPress(GLFWwindow* window, int button, int action, int mods);
	static void onSelection(int xPos, int yPos);
	void initKitchen();
	void initModules();
	void renderScene();
	static void update(double elapsedTime);
	static void updateUI();
	static void updateCamera();
	static void showFPS(GLFWwindow* win);
	static void updateTargetModule();

	// Movable
	static void increaseWallHeight(float dL);
	static void decreaseWallHeight(float dL);
	static void increaseWallWidth(float dL);
	static void decreaseWallWidth(float dL);
	static void increaseTableHeight(float dL);
	static void decreaseTableHeight(float dL);
	static void increaseUpperShelfsHeight(float dL);
	static void decreaseUpperShelfsHeight(float dL);
	static void moveRefrigeratorForward(float dL);
	static void moveRefrigeratorBackward(float dL);
	static void increaseTableBoardHeight(float dL) {};
	static void decreaseTableBoardHeight(float dL) {};
	static void increaseTablePanelHeight(float dL) {};
	static void decreaseTablePanelHeight(float dL) {};

	 static inline double W = 1600, H = 900;
private:
	 static inline GLFWwindow* pWin = nullptr;
	 static inline MyFPSCamera* fpsCamera = nullptr;
	 static inline const char* TITLE = "Kitchen Configurator";
	 static inline  const double ZOOM_SENSITIVITY = -3.0;
	 static inline  const float MOUSE_SENSITIVITY = 0.2f;
	 static inline  float gRadius = 10.0f, gYaw = 0.0f, gPitch = 0.0f;
	 static inline bool isCursorChoiseModuleMode = false;

	 static inline KitchenUI* kitchenUI = nullptr;
	 
	 static inline bool isWireMode = false;
	 static inline float sunIntensity = 0.9f; // daylight
	 static inline float move_speed = 5.0f;
	 static inline KitchenMaterialPack* materialPack = nullptr;
	 static inline KitchenMeshBase* selectedMesh = nullptr;
	 static inline ModuleRoom* moduleRoom = nullptr;
	 static inline ModuleStove* moduleStove = nullptr;
	 static inline ModuleBigCargo* moduleBigCargo = nullptr;
	 static inline ModuleRefrigerator* moduleRefrigerator = nullptr;
	 static inline ModuleBottomCargo* moduleBottomCargo = nullptr;
	 static inline std::array<ModuleBottomBox*, 3> modulesBottom;
	 static inline ModuleBottomSmart* moduleBottomSmart = nullptr;
	 static inline ModuleBottomWasher* moduleBottomWasher = nullptr;
	 static inline ModuleTable* moduleTable = nullptr;
	 static inline ModuleUpperCargo* moduleUpperCargo = nullptr;
	 static inline ModuleUpperCornerLeft* moduleUpperCornerLeft = nullptr;
	 static inline ModuleUpperCornerRight* moduleUpperCornerRight = nullptr;
	 static inline ModuleUpperShelfsNarrow* moduleUpperShelfsNarrow = nullptr;
	 static inline ModuleUpperShelfsSide* moduleUpperShelfsSide = nullptr;
	 static inline ModulePlinthTop* modulePlinthTop = nullptr;
	 static inline std::array<ModuleUpperShelf*, 3> modulesTop;
	 static inline ModulePlinthBottom* modulePlinthBottom = nullptr;
	 static inline std::vector<KitchenModule*> modules;
	 static inline int selectedModuleIdx = 0;
	 static inline KitchenModule* selectedModule = nullptr;
	 static inline std::map<int, bool> isKeyAlreadyPressed;

	 mutable int loadPercentage = 0;
	 bool isLoadingScreenCalled = false;
	 bool isAllModulesReadFromFile = false;
	 std::jthread* jthr = nullptr;  // UI Loading Thread

	 KitchenShader* skyBoxShader;
	 KitchenShader* kitchenShader;
	 KitchenMeshBase* mesh_ground;
	 KitchenMeshBase* ambienceBox;
	 KitchenMeshBase* mesh_skybox;

	 static inline irrklang::ISoundEngine* engine = nullptr; // Sound

public:
	 bool isSceneLoaded = false;
	void changeRoomLightness(double newVal) override;
	void switchRenderMode() override;
	bool isModuleEnable(size_t id) override;
	void selectModule(size_t id) override;
	size_t getTotalModuleCount() override;
	KitchenModule* getCurModule() override;
	void activateModule(size_t id) override;
	void deactivateModule(size_t id) override;
	int getLoadingPercentage() { return loadPercentage; }
	glm::vec3 getCameraPosition() override;
	void setNewCameraPosition(const POINT_3D& newPos) override;
	static void playSound(SOUND type, bool loop = false);
	static void mute();
	static void setVolume(float vol);
};