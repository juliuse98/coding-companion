#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <nano_signal_slot.hpp>

namespace cabbage
{
struct Transform
{

    glm::vec3 Position() { return m_position; };
    glm::vec3 Rotation() { return m_rotation; };
    glm::vec3 Scale() { return m_scale; };

    void SetPosition(glm::vec3 position, bool noUpdate = false)
    {
        m_position = position;
        if (noUpdate)
            return;
        changed.fire();
    }
    void SetRotation(glm::vec3 rotation, bool noUpdate = false)
    {
        m_rotation = rotation;
        if (noUpdate)
            return;
        changed.fire();
    }
    void SetScale(glm::vec3 scale, bool noUpdate = false)
    {
        m_scale = scale;
        if (noUpdate)
            return;
        changed.fire();
    }

    Nano::Signal<void()> changed;

    inline glm::mat4 const getModelMatrix() const
    {
        glm::mat4 result = glm::mat4(1.0f);

        result = glm::translate(result, m_position);
        result = glm::rotate(result, glm::radians(m_rotation.z),
                             glm::vec3(0.0f, 0.0f, 1.0f));
        result = glm::rotate(result, glm::radians(m_rotation.y),
                             glm::vec3(0.0f, 1.0f, 0.0f));
        result = glm::rotate(result, glm::radians(m_rotation.x),
                             glm::vec3(1.0f, 0.0f, 0.0f));
        result = glm::scale(result, m_scale);

        return result;
    }

    inline glm::vec3 Forward() const
    {
        return glm::vec3(glm::rotate(glm::mat4(1.0f),
                                     glm::radians(m_rotation.y),
                                     glm::vec3(0.0f, 1.0f, 0.0f)) *
                         glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    }
    inline glm::vec3 Right() const
    {
        return glm::vec3(glm::rotate(glm::mat4(1.0f),
                                     glm::radians(m_rotation.y),
                                     glm::vec3(0.0f, 1.0f, 0.0f)) *
                         glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    }
    inline glm::vec3 Up() const
    {
        return glm::vec3(glm::rotate(glm::mat4(1.0f),
                                     glm::radians(m_rotation.y),
                                     glm::vec3(0.0f, 1.0f, 0.0f)) *
                         glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    }

  private:
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
};
} // namespace cabbage
