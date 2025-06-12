#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <string>
#include <format>
#include <algorithm>
#include <numeric>
#include <functional>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "KitchenMaterial.h"

const float NOLIMIT = 9999.f;
// Convert MM to Unit (1 unit = 140 mm)
const long double UNIT = 140.;
//static unsigned long long operator""mm(unsigned long long sizeInMM) { return sizeInMM / UNIT; }
static long double operator""mm(long double sizeInMM) { return sizeInMM / UNIT; }
static float sqr(float s) {return (float)pow(s, 2); }

enum BOX_POS : char 
{
	L = 'L',
	M = 'M',
	R = 'R'
};

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoords;
	glm::vec3 normal;
};

enum class MODIFY_TYPE 
{
	MOV_FORWARD,
	MOV_BACKWARD,
	MOV_LEFT,
	MOV_RIGHT,
	MOV_UP,
	MOV_BOTTOM,
	INC_FORWARD,
	INC_BACKWARD,
	INC_LEFT,
	INC_RIGHT,
	INC_UP,
	INC_BOTTOM,
	DEC_FORWARD,
	DEC_BACKWARD,
	DEC_LEFT,
	DEC_RIGHT,
	DEC_UP,
	DEC_BOTTOM
};

enum class ROT_AXIS 
{
	ROLL, 	// X
	YAW, 	// Y
	PITCH 	// Z
};

// Base Mesh Class
class KitchenMeshBase 
{
public:
	KitchenMeshBase(const std::string& filename, const std::string& meshName = "");
	~KitchenMeshBase();
	bool readFromFile(const std::string& filename);
	bool loadObj();
	void updateBuffers();
	void render(glm::mat4& model, const glm::vec3& scale = glm::vec3{ 1.f, 1.f, 1.f }, const glm::vec3& pos = glm::vec3{ 0.f, 0.f, 0.f });
	void addMaterial(KitchenMaterial* new_mat);
	void setMaterial(int id_mat);
	bool isVisibleMesh();
	void setVisible(bool isVisible = true);
	void move(float dL, MODIFY_TYPE moveBy, float limFrom = NOLIMIT, float limTo = NOLIMIT);
	void scale(float dL, MODIFY_TYPE moveBy);
	void rotate(float angle, ROT_AXIS rotType, glm::vec3 pivot);
	void setPosition(glm::vec3 newPos);

	float getMostLeftVertex();
	float getMostRightVertex();
	float getMostForwardVertex();
	float getMostBackwardVertex();
	float getMostTopVertex();
	float getMostBottomVertex();
	float getWidth();
	float getDepth();
	float getHeight();
	float getMidDepthVertex();
	float getMidWidthVertex();
	float getMidHeightVertex();
	int getVertexId(glm::vec3 pos);
	glm::vec3 getPosById(size_t id);
	std::vector<Vertex>& getVertices();
	std::string getName();
	size_t getMatCount() { return materials.size(); }
	GLuint getVAO() { return mVAO; }
	GLuint getVBO() { return mVBO; }
	std::vector<KitchenMaterial*> getMaterials() { return materials; }

protected:
	std::string name;
	std::vector<Vertex> vertices;
	GLuint mVBO, mVAO;
	bool isLoad;
	bool isVisible;
	std::vector<KitchenMaterial*> materials;
	KitchenMaterial* curMaterial = nullptr;

	// Temp Helpers (Caching)
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;
};

class FacadeBase;

enum class OPT_HANDLE { NO, YES };
enum class OPT_ROTATION { NO, LEFT, RIGHT, LEFT_RIGHT, LEFT_UPPER, UPPER, PAIRED, UPPER_PAIRED, ALL };
enum class OPT_MESH { NO, SIMPLE, CHAMFER, BEVEL, SIMPLE_CHAMFER, SIMPLE_BEVEL, CHAMFER_BEVEL, ALL };
enum class OPT_MOVE { NO, W, D, H, WD, WH, DH, ALL };
enum class OPT_SCALE { NO, W, D, H, WD, WH, DH, ALL };
enum class OPT_DIVIDER { NO, D0, D1, D2, D3, D4, D5, D6, D7 };
enum class OPT_CONTENT { NO, YES };
enum class OPT_SIDEBOX { NO, ONE, TWO, THREE };
enum class OPT_SCALE_PARAM { INC, DEC };
enum class OPT_MOVE_PARAM { IN, OUT };

class KitchenModule;

struct POINT_3D {
	glm::vec3 pos;
	float pitch;
	float yaw;
};

struct ModulePart {
public:
	ModulePart(KitchenModule* parentModule, std::vector<KitchenMeshBase*> meshes, const std::string& name, OPT_HANDLE opt_handle, OPT_ROTATION opt_rotation, OPT_MESH opt_mesh,
		OPT_MOVE opt_move, OPT_SCALE opt_scale, OPT_DIVIDER opt_divider, OPT_CONTENT opt_content,
		OPT_SIDEBOX opt_sidebox, bool isOpeneable, POINT_3D pointPos = { glm::vec3(0, 0, 0), 0.f, 0.f }) :
		parentModule{ parentModule }, meshes{ meshes }, name { name }, opt_handle{ opt_handle }, opt_rotation{ opt_rotation }, opt_mesh{ opt_mesh }, opt_move{ opt_move },
		opt_scale{ opt_scale }, opt_divider{ opt_divider }, opt_content{ opt_content }, opt_sidebox{ opt_sidebox }, isOpeneable{ isOpeneable }, pointPos { pointPos }
	{}
	void switchPartHandle(bool isHandleOn);
	void switchPartRotation(OPT_ROTATION curHandle);
	void switchPartMesh(OPT_MESH curMesh);
	void switchPartDivider(OPT_DIVIDER curDivide);
	void switchPartContent(OPT_CONTENT curContent);
	void switchPartSidebox(OPT_SIDEBOX curSideBox);
	void switchPartMaterial(std::string newMat);
	void movePart(float dL, OPT_MOVE_PARAM param);
	void scalePart(float dL, OPT_SCALE curScale, OPT_SCALE_PARAM param);
	void openPartBox(float dL);
	void openPartFacade(float dL);
	void closePartBox(float dL);
	void closePartFacade(float dL);

public:
	KitchenModule* parentModule;
	std::vector<KitchenMeshBase*> meshes;
	std::string name;
	OPT_HANDLE opt_handle;
	OPT_ROTATION opt_rotation;
	OPT_MESH opt_mesh;
	OPT_MOVE opt_move;
	OPT_SCALE opt_scale;
	OPT_DIVIDER opt_divider;
	OPT_CONTENT opt_content;
	OPT_SIDEBOX opt_sidebox;
	std::vector<std::string> opt_material;
	bool isOpeneable;
	POINT_3D pointPos;

	// Currents Values
	OPT_HANDLE cur_opt_handle = OPT_HANDLE::NO;
	OPT_ROTATION cur_opt_rotation = OPT_ROTATION::NO;
	OPT_MESH cur_opt_mesh = OPT_MESH::NO;
	OPT_SIDEBOX cur_opt_sidebox = OPT_SIDEBOX::NO;
	OPT_DIVIDER cur_opt_divider = OPT_DIVIDER::NO;
	std::string cur_opt_material = "";
};

class KitchenModule {
	friend class KitchenConfigurator;
public:
	KitchenModule(std::string moduleName, bool isFacades = false);
	void render(glm::mat4& model, const glm::vec3& scale = glm::vec3{ 1.f, 1.f, 1.f }, const glm::vec3& pos = glm::vec3{ 0.f, 0.f, 0.f });
	void moveModule(float dL, MODIFY_TYPE mod);
	FacadeBase* getFacade() { return curFacade; }
	std::string getModuleName() { return moduleName; }
	void switchMaterial() { matFuncs[curMatID](); }
	void switchMaterialFunc() { curMatID = curMatID < matFuncs.size() - 1 ? curMatID + 1 : 0; }
	void setVisibility();
	void setVisibility(bool isVisible);

	virtual void resizeModule(float dL, MODIFY_TYPE mod);
	virtual void switchBaseMaterial() {};
	virtual void switchPartHandle(bool isHandleOn);
	virtual void switchPartRotation(OPT_ROTATION curHandle);
	virtual void switchPartMesh(OPT_MESH curMesh);
	virtual void switchPartDivider(OPT_DIVIDER curDivide);
	virtual void switchPartContent(OPT_CONTENT curContent);
	virtual void switchPartSidebox(OPT_SIDEBOX curSideBox);
	virtual void switchPartMaterial(std::string newMat);
	virtual void movePart(float dL, OPT_MOVE_PARAM param);
	virtual void scalePart(float dL, OPT_SCALE curScale, OPT_SCALE_PARAM param);
	virtual void openPartBox(float dL);
	virtual void openPartFacade(float dL);
	virtual void closePartBox(float dL);
	virtual void closePartFacade(float dL);


protected:
	std::string moduleName;
	std::vector<KitchenMeshBase*> childs;
	FacadeBase* curFacade= nullptr;
	std::vector<std::function<void()>> matFuncs;
	size_t curMatID;
	bool isModuleVisible = true;

public:
	void nextPart();
	void prevPart();
	std::vector<ModulePart*> parts;
	ModulePart* curPart;
	int curPartId;
	POINT_3D pointPos;
};

enum class FACADE_ROT_TYPE 
{
	CLOCKWISE,
	COUNTERCLOCKWISE,
	UPWARD,
	DOWNWARD,
	PAIRED,
};

enum FACADE_MOV_STATE 
{
	OPENING,
	OPENED,
	CLOSING,
	CLOSED,
	BLOCKED
};

struct RotationType 
{
	FACADE_ROT_TYPE curOpenType;
	glm::vec3 pivotPoint;
	float minAngle = 0.f;
	float maxAngle = 90.f;
};

class FacadeBase 
{
	friend class KitchenConfigurator;
public:
	virtual void switchMesh() = 0;
	virtual void switchMesh(size_t id) = 0;
	virtual void setHandler(bool isVisible) = 0;
	virtual void openFacade(float delta) = 0;
	virtual void closeFacade(float delta, bool isAutoCloser) = 0;
	virtual void setFacadeVisible(bool isVisible) = 0;
	FACADE_MOV_STATE getFacadeState() { return movState; }
	void setFacadeState(FACADE_MOV_STATE newState) { movState = newState; }
	void setFacadeEnable(bool isEnable) { isFacadeEnable = isEnable; }
	virtual float getDeltaMove() = 0;
public:
	FACADE_MOV_STATE movState = FACADE_MOV_STATE::CLOSED;
	bool isFacadeEnable = true;
	bool isHandleVisible = false;
};

class FacadeSlidingBase : public FacadeBase {};

class FacadeRotateBase : public FacadeBase {
public:
	virtual void switchOpenType(size_t swtypeId) = 0;
	virtual bool isRotationSwitchable() = 0;
};

enum FACADE_VIEW {
	SLIDE,
	ROT,
};

template<size_t CountFacadeMeshes, FACADE_VIEW, size_t CountOpenTypes = 0> class Facade;

struct FacadeOpeningData {
	glm::vec3 pivotPoint;
	float maxAngle;
	ROT_AXIS rotAxis;
	FACADE_ROT_TYPE rotType;
};

enum HANDLE_SWITCH_TYPE {
	NONE,
	X,
	X_INV,
	Y,
	Y_INV,
	Z,
	Z_INV
};

enum FACADE_NORMAL_DIR {
	XY,
	YZ,
	ZX,
};


// FACADE  ROTATING
template<size_t CountFacadeMeshes, size_t CountRotTypes>
class Facade<CountFacadeMeshes, FACADE_VIEW::ROT, CountRotTypes> : public FacadeRotateBase {
	friend class KitchenConfigurator;

protected:
	std::array<KitchenMeshBase*, CountFacadeMeshes> meshes;
	std::array<FacadeOpeningData, CountRotTypes> rotTypesData;
	FACADE_NORMAL_DIR normalDir;
	KitchenMeshBase* handle = nullptr;
	KitchenMeshBase* curMesh = nullptr;
	size_t curMeshId;

	HANDLE_SWITCH_TYPE handleSwitchType = HANDLE_SWITCH_TYPE::NONE;
	bool isRotationSwitchable() override { return CountRotTypes > 1; }

public:
	size_t curRotTypeDataId;
	float angle = 0.f;

	Facade(const std::array<KitchenMeshBase*, CountFacadeMeshes>& facadeMeshes,
				const std::array<FacadeOpeningData, CountRotTypes>& rotTypesData, 
				FACADE_NORMAL_DIR normalDir, KitchenMeshBase* handle = nullptr,
				HANDLE_SWITCH_TYPE handleSwitchType = HANDLE_SWITCH_TYPE::NONE) :
		meshes{ facadeMeshes }, rotTypesData{ rotTypesData }, normalDir{ normalDir }, handleSwitchType { handleSwitchType }
	{
		curMesh = meshes[0]; 
		curMeshId = 0;
		curRotTypeDataId = 0;
		if (handle) { this->handle = handle; }

		for (size_t i = 0; i < meshes.size(); ++i) { meshes[i]->setVisible(false); }
		curMesh->setVisible(true);
		if (handle) { (this->handle = handle)->setVisible(false); }
	}

	KitchenMeshBase* getCurMesh() { return curMesh; }

	virtual void openFacade(float delta) override {
		if (delta < 1.f) delta = 1.f;
		if (isFacadeEnable == false) return;
		if (movState == FACADE_MOV_STATE::BLOCKED) return;
		if (std::abs(angle - rotTypesData[curRotTypeDataId].maxAngle) < 0.001f) {
			movState = FACADE_MOV_STATE::OPENED;
			//std::cout << "Facade Rotate Opened.\n";
			return;
		}
		if (movState != FACADE_MOV_STATE::OPENED) {
			// INVERT
			if (rotTypesData[curRotTypeDataId].rotType == FACADE_ROT_TYPE::COUNTERCLOCKWISE) {
				delta = -delta;
			}
			for (auto* msh : meshes) { msh->rotate(-delta, rotTypesData[curRotTypeDataId].rotAxis, rotTypesData[curRotTypeDataId].pivotPoint); }
			if (handle) handle->rotate(-delta, rotTypesData[curRotTypeDataId].rotAxis, rotTypesData[curRotTypeDataId].pivotPoint);
			angle += std::abs(delta);
			movState = FACADE_MOV_STATE::OPENING;
			//std::cout << "Facade Rotate Opening " << angle << '\n';
		}
	}

	virtual void closeFacade(float delta, bool isAutoCloser) override {
		// TODO: !!! Closing Uniform
		if (delta < 1.f) delta = 1.f;
		if (isFacadeEnable == false) return;
		if (movState == FACADE_MOV_STATE::BLOCKED) return;
		if (std::abs(angle) < 0.001f) {
			movState = FACADE_MOV_STATE::CLOSED;
			//std::cout << "Facade Rotate Closed.\n";
			return;
		}
		if (movState != FACADE_MOV_STATE::CLOSED) {
			if (rotTypesData[curRotTypeDataId].rotType == FACADE_ROT_TYPE::COUNTERCLOCKWISE) {
				delta = -delta;
			}
			for (auto* msh : meshes) { msh->rotate(delta, rotTypesData[curRotTypeDataId].rotAxis, rotTypesData[curRotTypeDataId].pivotPoint); }
			if (handle) handle->rotate(delta, rotTypesData[curRotTypeDataId].rotAxis, rotTypesData[curRotTypeDataId].pivotPoint);
			angle -= std::abs(delta);
			movState = FACADE_MOV_STATE::CLOSING;
			//std::cout << "Facade Rotate Closing " << angle << '\n';
		}
	}

	void switchMesh() override {
		if (isFacadeEnable == false) return;
		curMeshId = curMeshId < meshes.size() - 1 ? curMeshId + 1 : 0;
		curMesh = meshes[curMeshId];
		for (size_t i = 0; i < meshes.size(); ++i) { meshes[i]->setVisible(false); }
		curMesh->setVisible(true);
	}

	void switchMesh(size_t id) override {
		if (isFacadeEnable == false) return;
		curMesh = meshes[id];
		for (size_t i = 0; i < meshes.size(); ++i) { meshes[i]->setVisible(false); }
		curMesh->setVisible(true);
	}

	void setHandler(bool isVisible) override {
		if (isFacadeEnable == false) return;
		if (handle) { 
			handle->setVisible(isVisible); 
			isHandleVisible = isVisible;
		}
	}

	void switchOpenType(size_t swtypeId) override {
		if (isFacadeEnable == false || isRotationSwitchable() == false || movState == FACADE_MOV_STATE::BLOCKED) return;

		// Close Facade If Opened Before Switching Type
		this->closeFacade(angle, false);
		this->setFacadeState(FACADE_MOV_STATE::CLOSED);
		angle = 0.f;

		// Switch
		curRotTypeDataId = swtypeId;
		if (handle) {
			float center_facade = 0.f;
			float center_handle = 0.f;
			float dL = 0.f;
			switch (handleSwitchType)
			{
			case NONE:
			case Y:
			case Y_INV:
			case Z_INV:
				// No Need To Implement It
				break;
			case X:
				center_facade = curMesh->getMidDepthVertex();
				center_handle = handle->getMidDepthVertex();
				dL = 2 * (std::abs(center_facade - center_handle));
				if (rotTypesData[curRotTypeDataId].rotType == FACADE_ROT_TYPE::COUNTERCLOCKWISE ||
					rotTypesData[curRotTypeDataId].rotType == FACADE_ROT_TYPE::UPWARD) {
					handle->move(dL, MODIFY_TYPE::MOV_BACKWARD);
				}
				else {
					handle->move(dL, MODIFY_TYPE::MOV_FORWARD);
				}
				break;
			case X_INV:
				center_facade = curMesh->getMidDepthVertex();
				center_handle = handle->getMidDepthVertex();
				dL = 2 * (std::abs(center_facade - center_handle));
				if (rotTypesData[curRotTypeDataId].rotType == FACADE_ROT_TYPE::COUNTERCLOCKWISE ||
					rotTypesData[curRotTypeDataId].rotType == FACADE_ROT_TYPE::UPWARD) {
					handle->move(dL, MODIFY_TYPE::MOV_FORWARD);
				}
				else {
					handle->move(dL, MODIFY_TYPE::MOV_BACKWARD);
				}
				break;
			}
		}

		// Mirror Facade Mesh
		mirror();
		//std::cout << "Switched Facade Rotation Type.\n";
	}

	void setFacadeVisible(bool isVisible) override {
		if (isFacadeEnable == false) return;
		curMesh->setVisible(isVisible);
		if (handle) handle->setVisible(isVisible);
	}
	
	float getDeltaMove() override { return angle; }

	void changeMaxAngle(float addAngle) {
		rotTypesData[0].maxAngle += addAngle;
	}

private:
	// Mirror Mesh Among mirrorAxis
	void mirror() {
		float ax = curMesh->getMidDepthVertex();
		float ay = curMesh->getMidHeightVertex();
		float az = curMesh->getMidWidthVertex();

		ROT_AXIS mirrorAxis = rotTypesData[curRotTypeDataId].rotAxis;

		auto Involve = [this, &mirrorAxis](float& pivotX, float& pivotY, float& pivotZ, bool isNormalize = 0) {
			float ax = pivotX;
			float ay = pivotY;
			float az = pivotZ;
			glm::vec3 pivot = { ax, ay, az };

			if (isNormalize) {

				if ((normalDir == FACADE_NORMAL_DIR::XY && mirrorAxis == ROT_AXIS::YAW) || 
					(normalDir == FACADE_NORMAL_DIR::ZX && mirrorAxis == ROT_AXIS::PITCH)) {
					ay = 0.f;
					az = 0.f;
				}
				else if (	(normalDir == FACADE_NORMAL_DIR::XY && mirrorAxis == ROT_AXIS::ROLL) || 
							(normalDir == FACADE_NORMAL_DIR::XY && mirrorAxis == ROT_AXIS::PITCH) || 
							(normalDir == FACADE_NORMAL_DIR::YZ && mirrorAxis == ROT_AXIS::PITCH)) {
					ax = 0.f;
					az = 0.f;
				}
				else if (	(normalDir == FACADE_NORMAL_DIR::ZX && mirrorAxis == ROT_AXIS::ROLL) ||
							(normalDir == FACADE_NORMAL_DIR::ZX && mirrorAxis == ROT_AXIS::YAW) ||
							(normalDir == FACADE_NORMAL_DIR::YZ && mirrorAxis == ROT_AXIS::ROLL) ||
							(normalDir == FACADE_NORMAL_DIR::YZ && mirrorAxis == ROT_AXIS::YAW)) {
					ax = 0.f;
					ay = 0.f;
				}

				float L = sqrt(sqr(ax) + sqr(ay) + sqr(az));
				if (ax != 0.f) { ax /= L; }
				if (ay != 0.f) { ay /= L; }
				if (az != 0.f) { az /= L; }
			}
			else {
				ax = 0.f;
				ay = 0.f;
				az = 0.f;
			}

			glm::mat3 invol{
				2 * sqr(ax) - 1,			2 * ax * ay,				2 * ax * az,
				2 * ax * ay,				2 * sqr(ay) - 1,			2 * ay * az,
				2 * ax * az,				2 * ay * az,				2 * sqr(az) - 1
			};

			glm::vec3 p;
			auto& verts = curMesh->getVertices();
			for (auto& v : verts) {
				float x = v.position.x;
				float y = v.position.y;
				float z = v.position.z;
				p = { x, y, z };

				p.x -= pivot.x;
				p.y -= pivot.y;
				p.z -= pivot.z;

				p = p * invol;

				p.x += pivot.x;
				p.y += pivot.y;
				p.z += pivot.z;

				v.position = p;
			}
		};

		Involve(ax, ay, az);
		Involve(ax, ay, az, true);

		curMesh->updateBuffers();
	}

};

class IFadaceSwitchable {
	friend class KitchenConfigurator;
public:
	virtual void switchFacade() = 0;
};