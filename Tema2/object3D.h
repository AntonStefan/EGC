#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object3D
{

    // Create square with given bottom left corner, length and caolor
    Mesh* CreateTurret(const std::string& name,
        glm::vec3 position, // Turret position
        float baseSide, // Side length of the cubical base
        float barrelLength, // Length of the barrel
        float barrelRadius, // Radius of the barrel (hexagonal approximation)
        glm::vec3 color, // Turret color
        bool fill);



    Mesh* CreateTurretRing(const std::string& name,
        glm::vec3 position, // Center position of the hull
        float width,       // Width of the hull base
        float length,      // Length of the hull base
        float height,      // Height of the hull
        float frontSlope,  // Inward slope for the front side
        float backSlope,   // Inward slope for the back side
        float leftSlope,   // Inward slope for the left side
        float rightSlope,  // Inward slope for the right side
        glm::vec3 color,   // Color of the hull
        bool fill);         // Whether to fill the shapes

    Mesh* CreateHull(const std::string& name,
        glm::vec3 position, // Center position of the hull
        float width,       // Width of the hull base
        float length,      // Length of the hull base
        float height,      // Height of the hull
        float slope,       // Inward slope for the top vertices
        glm::vec3 color,   // Color of the hull
        bool fill);  

    Mesh* CreateTankTrack(
        const std::string& name,
        glm::vec3 position, // Track position
        float trackLength, // Total length of the track
        float trackWidth, // Width of the track
        float trackHeight, // Height of the track
        float cutDepth, // Depth of the cut from the front and back
        float trackThickness, // Thickness of the track (along Z-axis)
        glm::vec3 color, // Track color
        bool fill); // Whether to fill the shapes

    Mesh* CreateProjectile(
        const std::string& name,
        glm::vec3 position, // Sphere center position
        float radius, // Sphere radius
        glm::vec3 color, // Sphere color
        bool fill); // Whether to fill the shapes

    std::vector<Mesh*> CreateTank(const std::string& name, glm::vec3 position, glm::vec3 color, bool fill);

}
