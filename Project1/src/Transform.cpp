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

void Transform::SetRotation( const glm::vec3& axis)
{

    rotation = axis;
}

glm::mat4 Transform::GetModelMatrix() const {
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::translate(modelMatrix, position);

    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    modelMatrix = glm::scale(modelMatrix, scale);
    return modelMatrix;
}

glm::mat4 Transform::GetModelInverseMatrix() const
{
    return glm::inverse(glm::transpose(GetModelInverseMatrix()));
}

glm::vec3 Transform::GetForward() const
{
    return  glm::normalize(-glm::vec3(GetModelMatrix()[2]));
}

glm::vec3 Transform::GetUp() const
{
    return glm::normalize(-glm::vec3(GetModelMatrix()[1]));
}

glm::vec3 Transform::GetRight() const
{
    return glm::normalize(glm::vec3(GetModelMatrix()[0]));
}
