#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill = false);
    
    Mesh* CreateRhombus(
        const std::string& name,
        glm::vec3 center, // Centrul rombului
        float diagonal1, float diagonal2, // Diagonalele rombului
        glm::vec3 color,
        bool fill);

    Mesh* CreateHexagon(
        const std::string& name,
        glm::vec3 center,
        float radius,
        glm::vec3 color,
        bool fill,
        float z);

    /*Mesh* CreateEnemy(
        const std::string& name,
        glm::vec3 center,
        float outer_radius,
        float inner_radius);*/
    Mesh* CreateEnemy(
        const std::string& name,
        glm::vec3 center,
        float outer_radius,
        float inner_radius,
        glm::vec3 outer_color,
        glm::vec3 inner_color);

    Mesh* CreateStar(
        const std::string& name,
        glm::vec3 center,
        float outerRadius,
        float innerRadius,
        glm::vec3 color,
        bool fill);

    Mesh* CreateHallowSquare(
        const std::string& name,
        glm::vec3 leftBottomCorner,
        float length,
        glm::vec3 color);
}
