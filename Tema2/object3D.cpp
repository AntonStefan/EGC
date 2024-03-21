#include "object3D.h"
#include "transform3D.h"

#include <vector>
#include<iostream>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object3D::CreateTurret(
    const std::string& name,
    glm::vec3 position, // Turret position
    float baseSide, // Side length of the cubical base
    float barrelLength, // Length of the barrel
    float barrelRadius, // Radius of the barrel (hexagonal approximation)
    glm::vec3 color, // Turret color
    bool fill) // Whether to fill the shapes
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    int sides = 20; // Sides for the approximation of a cylinder

    // Barrel along the X-axis
    // Barrel bottom circle
    for (int i = 0; i <= sides; ++i) {
        float angle = 2.0f * M_PI * i / sides;
        glm::vec3 offset(0, cos(angle) * barrelRadius, sin(angle) * barrelRadius);
        vertices.push_back(VertexFormat(position + offset, color));
    }

    // Barrel top circle
    for (int i = 0; i <= sides; ++i) {
        float angle = 2.0f * M_PI * i / sides;
        glm::vec3 offset(barrelLength, cos(angle) * barrelRadius, sin(angle) * barrelRadius);
        vertices.push_back(VertexFormat(position + offset, color));
    }

    // Number of vertices per circle
    const int numVerticesPerCircle = sides + 1;

    // Add indices for the sides of the cylinder
    for (int i = 0; i < sides; ++i) {
        int current = i;
        int next = (i + 1) % sides;

        // First triangle of the quad
        indices.push_back(current);
        indices.push_back(current + numVerticesPerCircle);
        indices.push_back(next);

        // Second triangle of the quad
        indices.push_back(next);
        indices.push_back(current + numVerticesPerCircle);
        indices.push_back(next + numVerticesPerCircle);
    }

    // Create the turret base or other components as needed

// Sphere creation at the starting end of the cylinder
    int latitudeBands = 20;  // Number of bands around the sphere
    int longitudeBands = 20; // Number of bands around the circumference of the sphere

    // The starting index for the sphere vertices
    int sphereStartIndex = vertices.size();

    // The center of the sphere should be at the starting end of the cylinder
    glm::vec3 sphereCenter = position;

    // Create vertices for the sphere
    for (int latNumber = 0; latNumber <= latitudeBands; latNumber++) {
        float theta = latNumber * M_PI / latitudeBands;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int longNumber = 0; longNumber <= longitudeBands; longNumber++) {
            float phi = longNumber * 2 * M_PI / longitudeBands;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            glm::vec3 norm = glm::vec3(cosPhi * sinTheta, sinPhi * sinTheta, cosTheta);
            glm::vec3 vertexPos = sphereCenter + norm * barrelRadius * 1.5f; // Use the sphere's center
            vertices.push_back(VertexFormat(vertexPos, color));
        }
    }

    // Create indices for the triangles of the sphere
    for (int latNumber = 0; latNumber < latitudeBands; latNumber++) {
        for (int longNumber = 0; longNumber < longitudeBands; longNumber++) {
            int first = (latNumber * (longitudeBands + 1)) + longNumber + sphereStartIndex;
            int second = first + longitudeBands + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            // Only add the second triangle if it's not the last latitude band (to avoid wrapping incorrectly)
            if (latNumber != latitudeBands - 1) {
                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }
    }



    // Initialize the mesh with vertices and indices
    Mesh* turret = new Mesh(name);
    turret->InitFromData(vertices, indices);
    if (!fill) {
        turret->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        turret->SetDrawMode(GL_TRIANGLES);
    }

    return turret;
}




Mesh* object3D::CreateProjectile(
    const std::string& name,
    glm::vec3 position, // Sphere center position
    float radius, // Sphere radius
    glm::vec3 color, // Sphere color
    bool fill) // Whether to fill the shapes
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    int latitudeBands = 20;
    int longitudeBands = 20;

    // Create a sphere at the given position
    for (int latNumber = 0; latNumber <= latitudeBands; latNumber++) {
        float theta = latNumber * M_PI / latitudeBands;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int longNumber = 0; longNumber <= longitudeBands; longNumber++) {
            float phi = longNumber * 2 * M_PI / longitudeBands;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            glm::vec3 norm = glm::vec3(cosPhi * sinTheta, cosTheta, sinPhi * sinTheta);
            //asa era glm::vec3 vertexPos = position + norm * radius;
            glm::vec3 vertexPos = glm::vec3(radius * cosPhi * sinTheta, radius * cosTheta, radius * sinPhi * sinTheta);
            //vertexPos += position; // Now translating the vertex to the correct position
            vertices.push_back(VertexFormat(vertexPos, color));
            // ... Add indices for each quad (or two triangles) of the sphere
        }
    }

    // Create indices for the triangles of the sphere
    for (int latNumber = 0; latNumber < latitudeBands; latNumber++) {
        for (int longNumber = 0; longNumber < longitudeBands; longNumber++) {
            int first = (latNumber * (longitudeBands + 1)) + longNumber;
            int second = first + longitudeBands + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    Mesh* sphere = new Mesh(name);
    sphere->InitFromData(vertices, indices);

    sphere->SetDrawMode(fill ? GL_TRIANGLES : GL_LINE_LOOP);
    return sphere;
}




Mesh* object3D::CreateTurretRing(const std::string& name,
    glm::vec3 position, // Center position of the hull
    float width,       // Width of the hull base
    float length,      // Length of the hull base
    float height,      // Height of the hull
    float frontSlope,  // Inward slope for the front side
    float backSlope,   // Inward slope for the back side
    float leftSlope,   // Inward slope for the left side
    float rightSlope,  // Inward slope for the right side
    glm::vec3 color,   // Color of the hull
    bool fill)         // Whether to fill the shapes
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Calculate half dimensions for convenience
    float halfWidth = width / 2.0f;
    float halfLength = length / 2.0f;

    // Bottom rectangle vertices (base of the hull)
    glm::vec3 v1 = position + glm::vec3(-halfWidth, 0, -halfLength);
    glm::vec3 v2 = position + glm::vec3(halfWidth, 0, -halfLength);
    glm::vec3 v3 = position + glm::vec3(halfWidth, 0, halfLength);
    glm::vec3 v4 = position + glm::vec3(-halfWidth, 0, halfLength);

    // Top rectangle vertices (adjusted individually for each side slope)
    glm::vec3 v5 = position + glm::vec3(-halfWidth + leftSlope, height, -halfLength + frontSlope);
    glm::vec3 v6 = position + glm::vec3(halfWidth - rightSlope, height, -halfLength + frontSlope);
    glm::vec3 v7 = position + glm::vec3(halfWidth - rightSlope, height, halfLength - backSlope);
    glm::vec3 v8 = position + glm::vec3(-halfWidth + leftSlope, height, halfLength - backSlope);

    // Add vertices to the vector
    vertices.push_back(VertexFormat(v1, color));
    vertices.push_back(VertexFormat(v2, color));
    vertices.push_back(VertexFormat(v3, color));
    vertices.push_back(VertexFormat(v4, color));
    vertices.push_back(VertexFormat(v5, color));
    vertices.push_back(VertexFormat(v6, color));
    vertices.push_back(VertexFormat(v7, color));
    vertices.push_back(VertexFormat(v8, color));

    // Define indices for the bottom rectangle (base)
    indices.push_back(0); indices.push_back(1); indices.push_back(2);
    indices.push_back(0); indices.push_back(2); indices.push_back(3);

    // Define indices for the top rectangle (sloped top)
    indices.push_back(4); indices.push_back(5); indices.push_back(6);
    indices.push_back(4); indices.push_back(6); indices.push_back(7);

    // Define indices for the sides (trapezoids)
    indices.push_back(0); indices.push_back(1); indices.push_back(5);
    indices.push_back(0); indices.push_back(5); indices.push_back(4);

    indices.push_back(1); indices.push_back(2); indices.push_back(6);
    indices.push_back(1); indices.push_back(6); indices.push_back(5);

    indices.push_back(2); indices.push_back(3); indices.push_back(7);
    indices.push_back(2); indices.push_back(7); indices.push_back(6);

    indices.push_back(3); indices.push_back(0); indices.push_back(4);
    indices.push_back(3); indices.push_back(4); indices.push_back(7);

    // Create the mesh
    Mesh* hull2 = new Mesh(name);
    hull2->InitFromData(vertices, indices);

    // Set the draw mode based on the 'fill' flag
    hull2->SetDrawMode(fill ? GL_TRIANGLES : GL_LINE_LOOP);
    return hull2;
}



Mesh* object3D::CreateHull(const std::string& name,
    glm::vec3 position, // Center position of the hull
    float width,       // Width of the hull base
    float length,      // Length of the hull base
    float height,      // Height of the hull
    float slope,       // Inward slope for the top vertices
    glm::vec3 color,   // Color of the hull
    bool fill)         
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Calculate half dimensions for convenience
    float halfWidth = width / 2.0f;
    float halfLength = length / 2.0f;

    // Bottom rectangle vertices (base of the hull)
    glm::vec3 v1 = position + glm::vec3(-halfWidth, 0, -halfLength);
    glm::vec3 v2 = position + glm::vec3(halfWidth, 0, -halfLength);
    glm::vec3 v3 = position + glm::vec3(halfWidth, 0, halfLength);
    glm::vec3 v4 = position + glm::vec3(-halfWidth, 0, halfLength);

    // Top rectangle vertices (moved inward by 'slope' to create the sloped sides)
    glm::vec3 v5 = position + glm::vec3(-halfWidth + slope, height, -halfLength + slope);
    glm::vec3 v6 = position + glm::vec3(halfWidth - slope, height, -halfLength + slope);
    glm::vec3 v7 = position + glm::vec3(halfWidth - slope, height, halfLength - slope);
    glm::vec3 v8 = position + glm::vec3(-halfWidth + slope, height, halfLength - slope);

    // Add vertices to the vector
    vertices.push_back(VertexFormat(v1, color));
    vertices.push_back(VertexFormat(v2, color));
    vertices.push_back(VertexFormat(v3, color));
    vertices.push_back(VertexFormat(v4, color));
    vertices.push_back(VertexFormat(v5, color));
    vertices.push_back(VertexFormat(v6, color));
    vertices.push_back(VertexFormat(v7, color));
    vertices.push_back(VertexFormat(v8, color));

    // Define indices for the bottom rectangle (base)
    indices.push_back(0); indices.push_back(1); indices.push_back(2);
    indices.push_back(0); indices.push_back(2); indices.push_back(3);

    // Define indices for the top rectangle (sloped top)
    indices.push_back(4); indices.push_back(5); indices.push_back(6);
    indices.push_back(4); indices.push_back(6); indices.push_back(7);

    // Define indices for the sides (trapezoids)
    indices.push_back(0); indices.push_back(1); indices.push_back(5);
    indices.push_back(0); indices.push_back(5); indices.push_back(4);

    indices.push_back(1); indices.push_back(2); indices.push_back(6);
    indices.push_back(1); indices.push_back(6); indices.push_back(5);

    indices.push_back(2); indices.push_back(3); indices.push_back(7);
    indices.push_back(2); indices.push_back(7); indices.push_back(6);

    indices.push_back(3); indices.push_back(0); indices.push_back(4);
    indices.push_back(3); indices.push_back(4); indices.push_back(7);

    // Create the mesh
    Mesh* hull2 = new Mesh(name);
    hull2->InitFromData(vertices, indices);

    // Set the draw mode based on the 'fill' flag
    hull2->SetDrawMode(fill ? GL_TRIANGLES : GL_LINE_LOOP);
    return hull2;
}


Mesh* object3D::CreateTankTrack(
    const std::string& name,
    glm::vec3 position, // Track position
    float trackLength, // Total length of the track
    float trackWidth, // Width of the track
    float trackHeight, // Height of the track
    float cutDepth, // Depth of the cut from the front and back
    float trackThickness, // Thickness of the track (along Z-axis)
    glm::vec3 color, // Track color
    bool fill) // Whether to fill the shapes
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Calculate the vertices for the front and back faces of the track
    // Front face vertices
    glm::vec3 frontBottomLeft = position + glm::vec3(-trackWidth / 2 + cutDepth, -trackHeight / 2, -trackThickness / 2);
    glm::vec3 frontBottomRight = position + glm::vec3(trackWidth / 2 - cutDepth, -trackHeight / 2, -trackThickness / 2);
    glm::vec3 frontTopLeft = position + glm::vec3(-trackWidth / 2, trackHeight / 2, -trackThickness / 2);
    glm::vec3 frontTopRight = position + glm::vec3(trackWidth / 2, trackHeight / 2, -trackThickness / 2);

    // Back face vertices
    glm::vec3 backBottomLeft = position + glm::vec3(-trackWidth / 2 + cutDepth, -trackHeight / 2, trackThickness / 2);
    glm::vec3 backBottomRight = position + glm::vec3(trackWidth / 2 - cutDepth, -trackHeight / 2, trackThickness / 2);
    glm::vec3 backTopLeft = position + glm::vec3(-trackWidth / 2, trackHeight / 2, trackThickness / 2);
    glm::vec3 backTopRight = position + glm::vec3(trackWidth / 2, trackHeight / 2, trackThickness / 2);

    // Add vertices for the front and back faces
    vertices.push_back(VertexFormat(frontBottomLeft, color)); // 0
    vertices.push_back(VertexFormat(frontBottomRight, color)); // 1
    vertices.push_back(VertexFormat(frontTopLeft, color)); // 2
    vertices.push_back(VertexFormat(frontTopRight, color)); // 3

    vertices.push_back(VertexFormat(backBottomLeft, color)); // 4
    vertices.push_back(VertexFormat(backBottomRight, color)); // 5
    vertices.push_back(VertexFormat(backTopLeft, color)); // 6
    vertices.push_back(VertexFormat(backTopRight, color)); // 7

    // Define indices to connect the front and back faces
    // Front face
    indices.push_back(0); indices.push_back(1); indices.push_back(2);
    indices.push_back(1); indices.push_back(3); indices.push_back(2);

    // Back face
    indices.push_back(4); indices.push_back(5); indices.push_back(6);
    indices.push_back(5); indices.push_back(7); indices.push_back(6);

    // Connect the sides
    // Left side
    indices.push_back(0); indices.push_back(4); indices.push_back(2);
    indices.push_back(2); indices.push_back(4); indices.push_back(6);

    // Right side
    indices.push_back(1); indices.push_back(3); indices.push_back(5);
    indices.push_back(3); indices.push_back(7); indices.push_back(5);

    // Bottom side
    indices.push_back(0); indices.push_back(1); indices.push_back(4);
    indices.push_back(1); indices.push_back(5); indices.push_back(4);

    // Top side
    indices.push_back(2); indices.push_back(3); indices.push_back(6);
    indices.push_back(3); indices.push_back(7); indices.push_back(6);

    // ... (Add vertices and indices for the other sides of the parallelepiped to complete the mesh)

    // Create the mesh with the vertices and indices
    Mesh* tracks = new Mesh(name);

    if (!fill) {
        tracks->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        tracks->SetDrawMode(GL_TRIANGLES);
    }

    tracks->InitFromData(vertices, indices);
    return tracks;
}


std::vector<Mesh*> object3D::CreateTank(const std::string& name, glm::vec3 position, glm::vec3 color, bool fill) {
    std::vector<Mesh*> tankComponents;

    // Adjusting positions of each component relative to the input position
   
    glm::vec3 centertomiddle = glm::vec3(-1.f, -0.3f, +0.25f); //glm::vec3(0.f, 0.f, 0.f); 
    glm::vec3 turretPosition = position + glm::vec3(1.6, 1.65, -0.25) + centertomiddle;
    std::cout << "TURRET POSITON IS" << turretPosition;
    glm::vec3 hullPosition = position + glm::vec3(0.95, 0.45, -0.25) + centertomiddle;
    glm::vec3 trackPositionRight = position + glm::vec3(1.0f, 0.3f, 1.0f) + centertomiddle;
    glm::vec3 trackPositionLeft = position + glm::vec3(1.0f, 0.3f, -1.5f) + centertomiddle;
    glm::vec3 ringPosition = position + glm::vec3(1.0f, 1.25f, -0.25f) + centertomiddle;

    glm::vec3 hullColor = (name.find("enemy") != std::string::npos) ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::vec3(0.f, 0.4f, 0.f); // Red if enemy, else green

    glm::vec3 turretColor = glm::vec3(0.5f, 0.5f, 0.5f); // Grey color
    glm::vec3 trackColor = glm::vec3(0.5f, 0.5f, 0.5f); // Grey color for the track
    glm::vec3 ringColor = glm::vec3(0.5f, 0.8f, 0.5f);


    Mesh* turret = object3D::CreateTurret(
        "turret_" + name, // Unique name for the turret
        turretPosition,   // Adjusted position
        1.0f,             // Base side length
        2.0f,             // Barrel length
        0.3f,             // Barrel radius
        turretColor,            // Color (assuming same color for all components)
        fill              // Fill mode
    );
    if (turret) {
        std::cout << "Turret created: turret_" << name << std::endl;
        tankComponents.push_back(turret);
    }
    else {
        std::cerr << "Failed to create turret: turret_" << name << std::endl;
    }

    Mesh* hull = object3D::CreateHull(
        "hull_" + name,   // Unique name for the hull
        hullPosition,     // Adjusted position
        4.0f,             // Hull width
        3.0f,             // Hull length
        0.8f,             // Hull height
        0.5f,             // Hull slope
        hullColor,            // Color
        fill              // Fill mode
    );
    tankComponents.push_back(hull);

    Mesh* trackRight = object3D::CreateTankTrack(
        "TankTrackRight_" + name, // Unique name for the right track
        trackPositionRight,       // Adjusted position
        2.0f,                     // Track length
        4.5f,                     // Track width
        0.5f,                     // Track height
        0.5f,                     // Cut depth
        0.5f,                     // Track thickness
        trackColor,                    // Color
        fill                      // Fill mode
    );
    tankComponents.push_back(trackRight);

    Mesh* trackLeft = object3D::CreateTankTrack(
        "TankTrackLeft_" + name, // Unique name for the left track
        trackPositionLeft,       // Adjusted position
        2.0f,                    // Track length
        4.5f,                    // Track width
        0.5f,                    // Track height
        0.5f,                    // Cut depth
        0.5f,                    // Track thickness
        trackColor,                   // Color
        fill                     // Fill mode
    );
    tankComponents.push_back(trackLeft);

    Mesh* ringMesh = object3D::CreateTurretRing(
        "turretring_" + name, // Unique name for the turret ring
        ringPosition,         // Adjusted position
        2.0f,                 // Ring width
        2.0f,                 // Ring length
        0.7f,                 // Ring height
        0.3f,                 // Front slope
        0.3f,                 // Back slope
        0.2f,                 // Left slope
        0.8f,                 // Right slope
        ringColor,                // Color
        fill                  // Fill mode
    );
    tankComponents.push_back(ringMesh);

    // Combine all components into a single mesh
   // Mesh* tankMesh = new Mesh(name);
   // for (Mesh* component : tankComponents) {
   //     tankMesh->AddMesh(component);
    //}

   // tankMesh->SetDrawMode(fill ? GL_TRIANGLES : GL_LINE_LOOP);

    // Clean up if necessary
    // ...

    return tankComponents;
}

