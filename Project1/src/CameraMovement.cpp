#include "CameraMovement.h"

CameraMovement::CameraMovement()
{
}

CameraMovement::~CameraMovement()
{
}

void CameraMovement::LookAt(glm::vec3& target)
{
   

    camera->transform.position = target - glm::vec3(2,2,2);
    

   
}

glm::mat4 CameraMovement::GetViewMatrix(glm::vec3& target)
{
    return glm::mat4();
}

void CameraMovement::AssignCam(Camera* cam)
{

    this->camera = cam;
}

void CameraMovement::moveTo(glm::vec3 target, float speed)
{
    
       glm::vec3 direction = glm::normalize(target - camera->transform.position);
       camera->transform.position += direction * speed;
    
}

void CameraMovement::update(float deltaTime, glm::vec3 target, float speed)
{
    // Smoothly move the camera towards the target
   // moveTo(target, speed * deltaTime);
    // Make the camera look at the target
   //  camera->updateCameraVectors();
    LookAt(target);
  
}
