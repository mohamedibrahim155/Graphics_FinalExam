#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform 
{
public:

    Transform();
    Transform(const Transform& transform);


    glm::vec3 position;
    glm::vec3 scale;
    float rotationAngle;
    glm::vec3 rotation;

    void SetPosition(const glm::vec3& translation);
    void SetScale(const glm::vec3& scaling);
    void SetRotation( const glm::vec3& axis);

    glm::mat4 GetModelMatrix() const;
    glm::mat4 GetModelInverseMatrix() const;

    glm::vec3 GetForward() const;
    glm::vec3 GetUp() const;
    glm::vec3 GetRight() const;

private:
    
};