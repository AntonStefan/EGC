#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}


Mesh* object2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float width, float height, // Now taking width and height separately
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    // Vertices are now adjusted to create a rectangle of width and height
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles to fill the rectangle
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}


// Rhombus
Mesh* object2D::CreateRhombus(
    const std::string& name,
    glm::vec3 center, // Centrul rombului
    float diagonal1, float diagonal2, // Diagonalele rombului
    glm::vec3 color,
    bool fill)

{
    // Calculăm coordonatele vârfurilor rombului folosind diagonalele
    std::vector<VertexFormat> vertices = {
        VertexFormat(center + glm::vec3(-diagonal1 / 2, 0, 1), color), // Stânga
        VertexFormat(center + glm::vec3(0, diagonal2 / 2, 1), color),   // Sus
        VertexFormat(center + glm::vec3(diagonal1 / 2, 0, 1), color),  // Dreapta
        VertexFormat(center + glm::vec3(0, -diagonal2 / 2, 1), color),  // Jos
        VertexFormat(center + glm::vec3(0, 10, 1), color),  // Colt stanga sus drept
        VertexFormat(center + glm::vec3(0, -10, 1), color), // Colt stanga jos drept
        VertexFormat(center + glm::vec3(50, 10, 1), color),  // Colt dreapta sus drept
        VertexFormat(center + glm::vec3(50, -10, 1), color), // Colt dreapta jos drept
        VertexFormat(center + glm::vec3(0, 0, 1), color), // Colt dreapta jos drept

    };

    Mesh* rhombus = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3, 4, 5, 6, 7 , 8 };

    if (!fill) {
        rhombus->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Desenăm 2 triunghiuri pentru a umple rombul
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);

        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(0);

        indices.push_back(8);
        indices.push_back(4);
        indices.push_back(6);

        indices.push_back(8);
        indices.push_back(5);
        indices.push_back(7);

        indices.push_back(8);
        indices.push_back(6);
        indices.push_back(7);

    }

    rhombus->InitFromData(vertices, indices);
    return rhombus;
}



// Inamic
Mesh* object2D::CreateHexagon(
    const std::string& name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    bool fill,
    float z) // Coordonata z pentru a controla ordinea de desenare
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Add the center vertex for the hexagon
    vertices.push_back(VertexFormat(center, color)); // Index 0 is the center

    // Crearea vârfurilor pentru hexagon
    for (int i = 0; i < 6; ++i) {
        float angle_deg = 60 * i; // 60 de grade între fiecare vârf
        float angle_rad = glm::radians(angle_deg);
        // Adăugăm coordonatele vârfurilor hexagonului
        vertices.push_back(VertexFormat(center + glm::vec3(radius * cos(angle_rad), radius * sin(angle_rad), z), color));
    }

    Mesh* hexagon = new Mesh(name);

    // Crearea indicilor pentru hexagon
    for (int i = 0; i < 6; ++i) {
        indices.push_back(i);
        indices.push_back((i + 1) % 6);
        indices.push_back(6);
    }

    if (!fill) {
        hexagon->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Desenăm triunghiuri pentru a umple hexagonul
        for (int i = 0; i < 6; ++i) {
            indices.push_back(6);
            indices.push_back(i);
            indices.push_back((i + 1) % 6);
        }
    }

    hexagon->InitFromData(vertices, indices);

    return hexagon;
}

// Utilizarea funcției CreateHexagon pentru a crea inamicul
Mesh* object2D::CreateEnemy(
    const std::string& name,
    glm::vec3 center,
    float outer_radius,
    float inner_radius,
    glm::vec3 outer_color,
    glm::vec3 inner_color)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Add the center vertex for the inner hexagon
    vertices.push_back(VertexFormat(center, inner_color));

    // Add vertices for the outer hexagon
    for (int i = 0; i < 6; ++i) {
        float angle_deg = 60 * i - 30; // Offset by 30 degrees to have a flat top
        float angle_rad = glm::radians(angle_deg);
        vertices.push_back(VertexFormat(center + glm::vec3(outer_radius * cos(angle_rad), outer_radius * sin(angle_rad), 0), outer_color));
    }

    // Add vertices for the inner hexagon
    for (int i = 0; i < 6; ++i) {
        float angle_deg = 60 * i - 30; // Offset by 30 degrees to have a flat top
        float angle_rad = glm::radians(angle_deg);
        vertices.push_back(VertexFormat(center + glm::vec3(inner_radius * cos(angle_rad), inner_radius * sin(angle_rad), 0.1f), inner_color));
    }

    // The number of vertices before adding the center vertices for the outer triangles
    int num_vertices = vertices.size();

    Mesh* enemy = new Mesh(name);

    // Create triangles for the inner hexagon by connecting the center vertex with the inner hexagon vertices
    for (int i = 1; i <= 6; ++i) {
        indices.push_back(0); // The center vertex
        indices.push_back(i + 6);
        indices.push_back((i % 6) + 7); // Wrap around by using modulo
    }

    // Add the center vertex for the outer triangles (outer area) with the outer color
    int center_outer_index = vertices.size();
    vertices.push_back(VertexFormat(center, outer_color));

    // Create triangles for the outer area by connecting the outer hexagon vertices with the center vertex
    for (int i = 1; i <= 6; ++i) {
        indices.push_back(center_outer_index); // Center vertex with outer color
        indices.push_back(i); // Current outer vertex
        indices.push_back((i % 6) + 1); // Next outer vertex (wrap around with modulo)
    }

    enemy->InitFromData(vertices, indices);

    return enemy;
}


Mesh* object2D::CreateStar(
    const std::string& name,
    glm::vec3 center,
    float outerRadius,
    float innerRadius,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    //glm::vec3 purpleColor = glm::vec3(0.5f, 0.0f, 0.5f); // Purple color

    const int points = 5; // Number of points in the star
    const float step = 360.0f / points;
    const float halfStep = step / 2.0f;

    // Generating vertices for the star's points and inner vertices
    for (int i = 0; i < points; ++i) {
        // Outer vertex (tip of the star)
        float angle_deg = step * i + 90; // Offset by 90 degrees to start at the top
        float angle_rad = glm::radians(angle_deg);
        vertices.push_back(VertexFormat(center + glm::vec3(outerRadius * cos(angle_rad), outerRadius * sin(angle_rad), 0), color));

        // Inner vertex
        angle_deg += halfStep;
        angle_rad = glm::radians(angle_deg);
        vertices.push_back(VertexFormat(center + glm::vec3(innerRadius * cos(angle_rad), innerRadius * sin(angle_rad), 0), color));
    }

    Mesh* star = new Mesh(name);

    // Generating indices for the star mesh
    if (!fill) {
        for (int i = 0; i < points * 2; ++i) {
            indices.push_back(i);
        }
        // To close the loop
        indices.push_back(0);
        star->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // If filled, create triangles



        for (int i = 0; i < points; i++) {
            int nextOuter = (i + 1) % points;
            int currentInner = i + points;
            int nextInner = (currentInner + 1) % (points * 2);

            // Triangle from the current outer to the next outer and current inner
            indices.push_back(i);
            indices.push_back(nextOuter);
            indices.push_back(currentInner);

            // Triangle from the next outer to the current inner and the next inner
            indices.push_back(nextOuter);
            indices.push_back(currentInner);
            indices.push_back(nextInner);
        }
        star->SetDrawMode(GL_TRIANGLES);
    }

    star->InitFromData(vertices, indices);
    return star;
}

Mesh* object2D::CreateHallowSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color)
{
    glm::vec3 corner = leftBottomCorner;

    // Define the vertices of the square
    std::vector<VertexFormat> vertices = {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    // Define the indices for the line loop
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    // Create the square mesh
    Mesh* hallowsquare = new Mesh(name);

    // Set the mesh to be drawn as a line loop (hollow)
    hallowsquare->SetDrawMode(GL_LINE_LOOP);

    // Initialize the mesh with the vertices and indices
    hallowsquare->InitFromData(vertices, indices);

    return hallowsquare;
}


