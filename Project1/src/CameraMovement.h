#pragma once
#include "Camera.h"

class CameraMovement
{
public:
	CameraMovement();
	~CameraMovement();

	void LookAt(glm::vec3& target);
	glm::mat4 GetViewMatrix(glm::vec3& target);
	void AssignCam(Camera* cam);

	void moveTo(glm::vec3 target, float speed);

	void update(float deltaTime, glm::vec3 target, float speed);


private :

	Camera* camera;

};

