#include "Transform.h"



Transform::Transform() : position{ glm::vec3(0) }, rotation{ glm::vec3(0) }, scale{ glm::vec3(1.0f) }
{

}

Transform::Transform(const Transform& transform)
{
    this->position = transform.position;
    this->rotation = transform.rotation;
    this->scale = transform.scale;
}

void Transform::SetPosition(const glm::vec3& translation)
{
    position = translation;
}

void Transform::SetScale(const glm::vec3& scaling)
{
    scale = scaling;
}

void Transform::SetQuatRotation(glm::quat quatRotation)
{
    quaternionRotation = quatRotation;
    UpdateEulerFromQuaternion();
}

void Transform::SetRotation( const glm::vec3& axis)
{

    rotation = axis;
    UpdateQuaternionFromEuler();
}

glm::mat4 Transform::GetModelMatrix() const 
{
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::translate(modelMatrix, position);

   /* modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    modelMatrix = glm::scale(modelMatrix, scale);*/

    modelMatrix = modelMatrix * glm::mat4_cast(quaternionRotation);

    modelMatrix = glm::scale(modelMatrix, scale);

    return modelMatrix;
}

glm::mat4 Transform::GetModelInverseMatrix() const
{
    return glm::inverse(glm::transpose(GetModelMatrix()));
}

glm::vec3 Transform::GetForward() const
{
    return glm::normalize(-glm::vec3(glm::mat4_cast(quaternionRotation)[2]));
    //return  glm::normalize(-glm::vec3(GetModelMatrix()[2]));
}

glm::vec3 Transform::GetUp() const
{
    return glm::normalize(glm::vec3(glm::mat4_cast(quaternionRotation)[1]));
   // return glm::normalize(-glm::vec3(GetModelMatrix()[1]));
}

glm::vec3 Transform::GetRight() const
{
    return glm::normalize(glm::vec3(glm::mat4_cast(quaternionRotation)[0]));
  //  return glm::normalize(glm::vec3(GetModelMatrix()[0]));
}

void Transform::SetUp(glm::vec3 newUp)
{
    newUp = glm::normalize(newUp);

    glm::vec3 axis = glm::cross(this->GetUp(), newUp);
    float angle = glm::acos(glm::dot(this->GetUp(), newUp));
    glm::quat rotationQuat = glm::angleAxis(angle, axis);

    this->SetQuatRotation(rotationQuat);
}

void Transform::SetRight(glm::vec3 newRight)
{
    newRight = glm::normalize(newRight);

    glm::vec3 axis = glm::cross(this->GetRight(), newRight);
    float angle = glm::acos(glm::dot(this->GetRight(), newRight));
    glm::quat rotationQuat = glm::angleAxis(angle, axis);

    this->SetQuatRotation(rotationQuat);
}

void Transform::SetForward(glm::vec3 newForward)
{
    newForward = glm::normalize(newForward);

    glm::vec3 axis = glm::cross(this->GetForward(), newForward);
    float angle = glm::acos(glm::dot(this->GetForward(), newForward));
    glm::quat rotationQuat = glm::angleAxis(angle, axis);

    this->SetQuatRotation(rotationQuat);
}

void Transform::SetOrientationFromDirections(glm::vec3 newUp, glm::vec3 newRight)
{
    newUp = glm::normalize(newUp);
    newRight = glm::normalize(newRight);
    glm::vec3 newForward = glm::cross(newRight, newUp);

    glm::quat rotationQuat = glm::quatLookAt(newForward, newUp);

    this->SetQuatRotation(rotationQuat);
}

void  Transform:: SetOrientationTowardsDirection(const glm::vec3& targetDirection) {
    // Calculate the rotation quaternion to align the forward direction with the target direction
    glm::quat rotationQuat = glm::quatLookAt(targetDirection, this->GetUp());

    // Set the quaternion rotation
    SetQuatRotation(rotationQuat);
}


void Transform::UpdateQuaternionFromEuler()
{
    quaternionRotation = glm::quat(glm::radians(rotation));
}

void Transform::UpdateEulerFromQuaternion()
{
    rotation = glm::degrees(glm::eulerAngles(quaternionRotation));
}