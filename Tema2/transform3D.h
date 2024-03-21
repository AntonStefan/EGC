#pragma once

#include "utils/glm_utils.h"


#include <glm/gtc/matrix_transform.hpp>  // Include this for glm::rotate

namespace transform3D
{
    // ... other functions ...

    /*
    // Rotate matrix for 3D
    inline glm::mat4 Rotate(float radians, const glm::vec3& axis)
    {
        // Use glm's built-in rotate function to create a 4x4 rotation matrix
        return glm::rotate(glm::mat4(1.0f), radians, axis);
    }*/

    inline glm::mat4 Translate(float translateX, float translateY, float translateZ)
    {
        return glm::mat4(
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            translateX, translateY, translateZ, 1.f
        );
    }

    // Scale matrix for 3D
    inline glm::mat4 Scale(float scaleX, float scaleY, float scaleZ)
    {
        // Use glm's built-in scale function to create a 4x4 scaling matrix
        return glm::mat4(
            scaleX, 0.f, 0.f, 0.f,
            0.f, scaleY, 0.f, 0.f,
            0.f, 0.f, scaleZ, 0.f,
            0.f, 0.f, 0.f, 1.f
        );
    }

    // Rotate matrix relative to the OZ axis
    inline glm::mat4 RotateOZ(float radians)
    {
        // TODO(student): Implement the rotation matrix
        GLfloat c = cos(radians);
        GLfloat s = sin(radians);

        return glm::mat4(
            c, s, 0.f, 0.f,
            -s, c, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f
        );

    }

    // Rotate matrix relative to the OY axis
    inline glm::mat4 RotateOY(float radians)
    {
        // TODO(student): Implement the rotation matrix
        GLfloat c = cos(radians);
        GLfloat s = sin(radians);

        return glm::mat4(
            c, 0.f, -s, 0.f,
            0.f, 1.f, 0.f, 0.f,
            s, 0.f, c, 0.f,
            0.f, 0.f, 0.f, 1.f
        );

    }

    // Rotate matrix relative to the OX axis
    inline glm::mat4 RotateOX(float radians)
    {
        // TODO(student): Implement the rotation matrix
        GLfloat c = cos(radians);
        GLfloat s = sin(radians);

        return glm::mat4(
            1.f, 0.f, 0.f, 0.f,
            0.f, c, s, 0.f,
            0.f, -s, c, 0.f,
            0.f, 0.f, 0.f, 1.f
        );

    }
}
