#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace timothy{
    struct Transform
    {
        glm::vec3 Position;
        glm::vec3 Rotation;
        glm::vec3 Scale;

        inline glm::mat4 const getModelMatrix() const {
            glm::mat4 result = glm::mat4(1.0f);

            result = glm::rotate(   result, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            result = glm::rotate(   result, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            result = glm::rotate(   result, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            result = glm::scale(    result, Scale);
            result = glm::translate(result, Position);

            return result;
        }

        inline glm::vec3 Forward() const
        {
            return glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        }
        inline glm::vec3 Right() const
        {
            return glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        }
        inline glm::vec3 Up() const
        {
            return glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        }
    };
}
