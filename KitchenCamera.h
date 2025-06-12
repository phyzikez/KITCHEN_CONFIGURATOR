#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include <ostream>


// Base Camera Class
class KitchenCamera
{
	friend class KitchenConfigurator;
public:
	KitchenCamera();
	glm::mat4 getViewMatrix() const;
	virtual void setPosition(const glm::vec3& position) {}
	virtual void rotate(float yaw, float pitch) {}  // in degrees
	virtual void move(const glm::vec3& offsetPos) {}
	float getFOV() const { return mFOV; }
	void setFOV(float fov) { mFOV = fov; }
	const glm::vec3& getLook() const;
	const glm::vec3& getRight() const;
	const glm::vec3& getUp() const;
	const glm::vec3& getPosition() const;
	const glm::vec3& getTargetPosition() const;
	friend std::ostream& operator<<(std::ostream& os, const KitchenCamera* cam);
protected:
	virtual void updateCameraVectors();
	glm::vec3 mPosition;
	glm::vec3 mTargetPos;
	glm::vec3 mUp;
	glm::vec3 mLook;
	glm::vec3 mRight;
	const glm::vec3 WORLD_UP;

	float mYaw; //in radians
	float mPitch; //in radians
	float mFOV; //in degrees
};

enum class CAMERA_STATE 
{
	STATIC, 
	MOVING
};

struct Camera_Interpolate 
{
	glm::vec3 from;
	glm::vec3 to;
	struct Pred 
	{ 
		bool x; 
		bool y; 
		bool z; 
		bool ptch; 
		bool yw; 
	} pred;
	struct Ready 
	{ 
		bool x; 
		bool y; 
		bool z; 
		bool ptch; 
		bool yw; 
	} ready;
	glm::vec3 curPos;
	float pitchTo;
	float yawTo;
	float curPitch;
	float curYaw;
};

// Actual Camera
class MyFPSCamera : public KitchenCamera
{
	friend class KitchenConfigurator;
public:
	MyFPSCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = glm::pi<float>(), float pitch = 0.0f);
	virtual void setPosition(const glm::vec3& position);
	virtual void rotate(float yaw, float pitch); // in degrees
	virtual void move(const glm::vec3& offsetPos);
	void setInitParams(glm::vec3 position, float yaw, float pitch);
private:
	void updateCameraVectors();
	CAMERA_STATE state = CAMERA_STATE::STATIC;
	Camera_Interpolate interpolation;
};