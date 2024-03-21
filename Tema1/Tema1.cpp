#include "lab_m1/Tema1/Tema1.h"

#include<random>
#include <vector>
#include <iostream>
#include<math.h>
#include<algorithm>
#include <map>

#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/transform2D.h"


using namespace std;
using namespace m1;

float currentTime = glfwGetTime();
int lifes = 3;
int starsaquired = 0;
bool isRhombusDisappearing = false;
float rhombusDisappearingDelay = 2.0f; // Adjust this value as needed
float rhombusDisappearingTimer = 0.0f;

// enemies struct
struct Enemy {
    std::string name;
    glm::vec3 position;
    float spawnTime;
    glm::vec3 color;
    bool countedForLifeLoss = false;
    Mesh* mesh;
    float hitstaken;
};

// random stars struct
std::vector<Enemy> enemiesList;

struct Star {
    glm::vec3 position;
    std::string name;
    Mesh* mesh;
};

// porjectiles struct
std::vector<Star> starsList;

struct Projectile {
    glm::vec3 position;
    std::string name;
    Mesh* mesh;   
};

std::vector<Projectile> projectilesList20;
std::vector<Projectile> projectilesList21;
std::vector<Projectile> projectilesList22;
std::vector<Projectile> projectilesList10;
std::vector<Projectile> projectilesList11;
std::vector<Projectile> projectilesList12;
std::vector<Projectile> projectilesList00;
std::vector<Projectile> projectilesList01;
std::vector<Projectile> projectilesList02;



/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}

void Tema1::RenderGameScene(const glm::mat3& modelMatrix) {
    // Render 3x3 grid of squares
    for (int i = 0; i < 3; i++) { // For each row
        for (int j = 0; j < 3; j++) { // For each column
            std::string squareName = "square_" + std::to_string(i * 3 + j);
            RenderMesh2D(meshes[squareName], shaders["VertexColor"], modelMatrix);
        }
    }

    RenderMesh2D(meshes["rectangle_1"], shaders["VertexColor"], modelMatrix);

    // Render red squares
    for (int i = 0; i < lifes; i++) {
        std::string redsquareName = "redsquare_" + std::to_string(i);
        RenderMesh2D(meshes[redsquareName], shaders["VertexColor"], modelMatrix);
    }

    // Render hollow squares
    for (int i = 0; i < 4; i++) {
        std::string hallowsquareName = "hallowsquare_" + std::to_string(i);
        RenderMesh2D(meshes[hallowsquareName], shaders["VertexColor"], modelMatrix);
    }
    
    // Render 4 rhombus for each type
    for (int i = 0; i < 4; i++) {
        std::string rhombusName = "rhombus_" + std::to_string(i);
        RenderMesh2D(meshes[rhombusName], shaders["VertexColor"], modelMatrix);
    }

    // Render stars to show cost for each rhombus
    std::vector<float> neededstars = { 1, 2, 2, 3 };
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < neededstars[i]; j++) {
            std::string starsName = "starsName_" + std::to_string(i) + "_" + std::to_string(j);
            RenderMesh2D(meshes[starsName], shaders["VertexColor"], modelMatrix);
        }
    }
    for (int i = 0; i < starsaquired; i++) {
        std::string starsaquiredname = "starsaquired_" + std::to_string(i);
        RenderMesh2D(meshes[starsaquiredname], shaders["VertexColor"], modelMatrix);
    }
}

bool CheckCollision(const glm::vec2& center1, float radius1, const glm::vec2& center2, float radius2) {
    // Calculating the squared distance between the two centers (avoiding sqrt for performance)
    float distanceSquared = glm::length2(center1 - center2);

    // Calculate the sum of squared radii
    float sumOfRadiiSquared = (radius1 + radius2) * (radius1 + radius2);

    // If the squared distance is less than or equal to the sum of squared radii, there is a collision
    return distanceSquared <= sumOfRadiiSquared;
}


void Tema1::Init()
{
    /*Punctul (0, 0) va fi în colțul din stânga jos.
    Punctul (resolution.x, 0) va fi în colțul din dreapta jos.
    Punctul (0, resolution.y) va fi în colțul din stânga sus.
    Punctul (resolution.x, resolution.y) va fi în colțul din dreapta sus.
    IN OPENGL coordonatele cresc de jos in sus*/
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    //  squares
    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;
    float padding = 25;

    // colors
    glm::vec3 greenColor = glm::vec3(0, 1, 0);
    glm::vec3 redColor = glm::vec3(1, 0, 0);
    glm::vec3 greyColor = glm::vec3(0.5, 0.5, 0.5);
    glm::vec3 orangeColor = glm::vec3(1, 0.5, 0);
    glm::vec3 blueColor = glm::vec3(0, 0, 1);
    glm::vec3 purpleColor = glm::vec3(0.5, 0, 0.5);
    glm::vec3 yellowColor = glm::vec3(1.0, 1.0, 0.0);
    glm::vec3 lightPurple = glm::vec3(0.7f, 0.5f, 0.8f); // RGB: light purple

    // rhombs/ enemy colours
    std::vector<glm::vec3> colors = {
    glm::vec3(1, 0.5, 0), // orange
    glm::vec3(0, 0, 1),  // blue
    glm::vec3(1, 1, 0),   // yellow
    glm::vec3(0.5, 0, 0.5),  // purple
    };

    // Initialize angularStep
    angularStep = 0;
    angularStep1 = 0;
    angularStep2 = 0;
    angularStep3 = 0;
    angularStep4 = 0;
    angularStep5 = 0;
    angularStep6 = 0;
    angularStep7 = 0;
    angularStep8 = 0;
    angularStep9 = 0;


    // big red rectangle 
    glm::vec3 rectangleBottomLeftCorner = glm::vec3(30, 25, 0); // The given position for the rectangle
    glm::vec3 rectangleColor = glm::vec3(1, 0, 0); // Let's assume you want the color red for the rectangle
    float rectangleWidth = 65; // Given that the width should be the squareSide
    float rectangleHeight = 350; // The specified height

    // Create the rectangle
    std::string rectangleName = "rectangle_1"; // A name for the rectangle mesh
    Mesh* rectangle = object2D::CreateRectangle(rectangleName, rectangleBottomLeftCorner, rectangleWidth, rectangleHeight, rectangleColor, true);
    AddMeshToList(rectangle);

    // First, define your colors using glm::vec3 for RGB values ranging from 0 to 1
    glm::vec3 rhombusColor = glm::vec3(1, 0.5, 0); // Orange color
    glm::vec3 outerHexagonColor = glm::vec3(0, 0, 1); // Blue color
    glm::vec3 starColor = glm::vec3(0.5, 0, 0.5); // Purple color

    // dimensions for shapes
    // rhombus
    float rhombusDiagonal1 = 40.0f; // Length of the first diagonal (adjust as needed)
    float rhombusDiagonal2 = 80.0f; // Length of the second diagonal (adjust as needed)

    // enemy
    float outerHexagonRadius = 30.0f; // Outer hexagon radius (adjust as needed)
    float innerHexagonRadius = 20.0f; // Inner hexagon radius (adjust as needed)

    // star
    float starOuterRadius = 20.0f; // Outer radius of the star (adjust as needed)
    float starInnerRadius = 10.0f; // Inner radius of the star (adjust as needed)

    // example of generating a rhombus
    glm::vec3 rhombusCenterPosition = glm::vec3(540, 460, 0);
    // Create a rhombus
    Mesh* rhombus = object2D::CreateRhombus("rhombus", rhombusCenterPosition, rhombusDiagonal1, rhombusDiagonal2, rhombusColor, true);
    AddMeshToList(rhombus);


    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?


    // Coordonatele dorite pentru colțul din dreapta-jos al gridului 3 x 3
    float desiredBottomleftX = 125;
    float desiredBottomleftY = 25;

    float startX = desiredBottomleftX;
    float startY = desiredBottomleftY;

    // Create 3x3 grid of squares green ones
    for (int i = 0; i < 3; i++) { // For each row
        for (int j = 0; j < 3; j++) { // For each column
            // Calculate the top-left corner position of the current square
            float posX = startX + j * (squareSide + padding);
            float posY = startY + i * (squareSide + padding);
            glm::vec3 squarePosition = glm::vec3(posX, posY, 0);

            // Create a green square
            std::string squareName = "square_" + std::to_string(i * 3 + j);
            Mesh* square = object2D::CreateSquare(squareName, squarePosition, squareSide, greenColor, true);
            AddMeshToList(square);
        }
    }

    int redpadding = 40;
    float startXredTriangles = 850;
    float startYredTriangles = 625;
    float redsquareSide = 70;

    // create the 3 red squares from top right
    for (int i = 0; i < 3; i++) {
        float posX = startXredTriangles + i * (redsquareSide + redpadding);
        glm::vec3 redsquarePosition = glm::vec3(posX, startYredTriangles, 0);
        std::string redsquareName = "redsquare_" + std::to_string(i);
        Mesh* square = object2D::CreateSquare(redsquareName, redsquarePosition, redsquareSide, redColor, true);
        AddMeshToList(square);
    }

    int hallowpadding = 70;
    float startXhallowTriangles = 100;
    float startYhallowTriangles = 600;
    float hallowsquareSide = 100;

    // create the 4 hallow squares 
    for (int i = 0; i < 4; i++) {
        float posX = startXhallowTriangles + i * (hallowsquareSide + hallowpadding);
        glm::vec3 HallowsquarePosition = glm::vec3(posX, startYhallowTriangles, 0);
        std::string hallowsquareName = "hallowsquare_" + std::to_string(i);
        Mesh* square = object2D::CreateHallowSquare(hallowsquareName, HallowsquarePosition, hallowsquareSide, greyColor);
        AddMeshToList(square);
    }

    // create the 4 rhombs to drag
    float startXrhombs = 140;
    float startYrhombs = 650;
    for (int i = 0; i < 4; i++) {
        float posX = startXrhombs + i * (hallowsquareSide + hallowpadding);
        glm::vec3 rhombusPosition = glm::vec3(posX, startYrhombs, 0.1);
        std::string rhombusName = "rhombus_" + std::to_string(i);
        glm::vec3 currentColor = colors[i];
        Mesh* rhombus = object2D::CreateRhombus(rhombusName, rhombusPosition, rhombusDiagonal1, rhombusDiagonal2, currentColor, true);
        AddMeshToList(rhombus);
    }

    // needed stars for each rhomb
    std::vector<float> neededstars = { 1, 2, 2, 3 };

    // create the stars according to each rhombs hallow square 
    float startXstars = 110;
    float startYstars = 575;
    float starSpacing = 40; // Spacing between stars

    for (int i = 0; i < 4; i++) {
        float posX = startXstars + i * (hallowsquareSide + hallowpadding); // Starting X position for the first star in each group

        for (int j = 0; j < neededstars[i]; j++) {
            std::string starsName = "starsName_" + std::to_string(i) + "_" + std::to_string(j); // Unique name for each star
            glm::vec3 starPosition = glm::vec3(posX, startYstars, 0); // Current star position
            Mesh* star = object2D::CreateStar(starsName, starPosition, starOuterRadius, starInnerRadius, greyColor, true);
            AddMeshToList(star);
            posX += starSpacing; // Move X position for the next star
        }
    }


    // stars displayed aquired, max 10 stars available
    int aquiredpadding = 40;
    int posxaquiredstars = 800;
    int posyaquiredstars = 600;
    for (int i = 0; i < 13; i++) {
        glm::vec3 staraquiredPosition = glm::vec3(posxaquiredstars + i * aquiredpadding, posyaquiredstars, 0);
        std::string starsName = "starsaquired_" + std::to_string(i);
        Mesh* star = object2D::CreateStar(starsName, staraquiredPosition, starOuterRadius, starInnerRadius, greyColor, true);
        AddMeshToList(star);
    }

    lastEnemySpawnTime = 0.0f;
    spawnDelay = 5.0f;


    starSpawnTime = 0.0f;
    starSpawnInterval = 2.5f; // 2.5 secunde

    starfromrhombusSpawnTime20 = 0.0f;
    starfromrhombusSpawnInterval20 = 3.f;

    starfromrhombusSpawnTime21 = 0.0f;
    starfromrhombusSpawnInterval21 = 3.f;
    starfromrhombusSpawnTime22 = 0.0f;

    starfromrhombusSpawnInterval22 = 3.f;

    starfromrhombusSpawnTime10 = 0.0f;
    starfromrhombusSpawnInterval10 = 3.f;

    starfromrhombusSpawnTime11 = 0.0f;
    starfromrhombusSpawnInterval11 = 3.f;

    starfromrhombusSpawnTime12 = 0.0f;
    starfromrhombusSpawnInterval12 = 3.f;

    starfromrhombusSpawnTime00 = 0.0f;
    starfromrhombusSpawnInterval00 = 3.f;

    starfromrhombusSpawnTime01 = 0.0f;
    starfromrhombusSpawnInterval01 = 3.f;

    starfromrhombusSpawnTime02 = 0.0f;
    starfromrhombusSpawnInterval02 = 3.f;

    // Move enemy on random line
    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    translateX2 = 0;
    translateY2 = 0;

    // numbers of enemies on each line
    en1l1 = 0;
    en1l2 = 0;
    en1l3 = 0;
    en2l1 = 0;
    en2l2 = 0;
    en2l3 = 0;
    en3l1 = 0;
    en3l2 = 0;
    en3l3 = 0;
    en4l1 = 0;
    en4l2 = 0;
    en4l3 = 0;
    

    // star
    float ProjectilestarOuterRadius = 35.0f; // Outer radius of the star (adjust as needed)
    float ProjectilestarInnerRadius = 17.0f; // Inner radius of the star (adjust as needed)
    // creating rotating moving star
    glm::vec3 starCenterPosition = glm::vec3(175, 75, 0.1);
    // Create a star
    Mesh* star = object2D::CreateStar("star", starCenterPosition, ProjectilestarOuterRadius, ProjectilestarInnerRadius, starColor, true);
    AddMeshToList(star);


    glm::vec3 enemyposition = glm::vec3(1240, 0, 0.1);
    // 4 enemies types
    Mesh* enemy1 = object2D::CreateEnemy("enemy1", enemyposition, outerHexagonRadius, innerHexagonRadius, orangeColor, greenColor);
    AddMeshToList(enemy1);
    Mesh* enemy2 = object2D::CreateEnemy("enemy2", enemyposition, outerHexagonRadius, innerHexagonRadius, blueColor, greenColor);
    AddMeshToList(enemy2);
    Mesh* enemy3 = object2D::CreateEnemy("enemy3", enemyposition, outerHexagonRadius, innerHexagonRadius, yellowColor, greenColor);
    AddMeshToList(enemy3);
    Mesh* enemy4 = object2D::CreateEnemy("enemy4", enemyposition, outerHexagonRadius, innerHexagonRadius, purpleColor, greenColor);
    AddMeshToList(enemy4);
    
    // Drag and drop rombus mesh
    Mesh* meshX = object2D::CreateRhombus("drag_1",
        glm::vec3(-1 * rhombusDiagonal2 / 2, - 1 * rhombusDiagonal1 / 2, 0),
        rhombusDiagonal1,
        rhombusDiagonal2,
        orangeColor,
        true);
    AddMeshToList(meshX);

    Mesh* meshX2 = object2D::CreateRhombus("drag_2",
        glm::vec3(-1 * rhombusDiagonal2 / 2, -1 * rhombusDiagonal1 / 2, 0),
        rhombusDiagonal1,
        rhombusDiagonal2,
        blueColor,
        true);
    AddMeshToList(meshX2);

    Mesh* meshX3 = object2D::CreateRhombus("drag_3",
        glm::vec3(-1 * rhombusDiagonal2 / 2, -1 * rhombusDiagonal1 / 2, 0),
        rhombusDiagonal1,
        rhombusDiagonal2,
        yellowColor,
        true);
    AddMeshToList(meshX3);

    Mesh* meshX4 = object2D::CreateRhombus("drag_4",
        glm::vec3(-1 * rhombusDiagonal2 / 2, -1 * rhombusDiagonal1 / 2, 0),
        rhombusDiagonal1,
        rhombusDiagonal2,
        purpleColor,
        true);
    AddMeshToList(meshX4);

    // stars that appear random on screen
    Mesh* star1 = object2D::CreateStar("star1",glm::vec3(0.f, 0.f, 0.1f), starOuterRadius*1.5, starInnerRadius*1.5, lightPurple, true);
    AddMeshToList(star1);

    // create the 4 projectiles type
      // star
   // float ProjectilestarOuterRadius = 35.0f; // Outer radius of the star (adjust as needed)
   // float ProjectilestarInnerRadius = 17.0f; // Inner radius of the star (adjust as needed)
    // creating rotating moving star
   // glm::vec3 starCenterPosition = glm::vec3(175, 75, 0.1);
    // Create the projectiles
    Mesh* starprojectile1 = object2D::CreateStar("proj_1", glm::vec3(0.f, 0.f, 0.1f), ProjectilestarOuterRadius, ProjectilestarInnerRadius, orangeColor, true);
    AddMeshToList(starprojectile1);
    Mesh* starprojectile2 = object2D::CreateStar("proj_2", glm::vec3(0.f, 0.f, 0.1f), ProjectilestarOuterRadius, ProjectilestarInnerRadius, blueColor, true);
    AddMeshToList(starprojectile2);
    Mesh* starprojectile3 = object2D::CreateStar("proj_3", glm::vec3(0.f, 0.f, 0.1f), ProjectilestarOuterRadius, ProjectilestarInnerRadius, yellowColor, true);
    AddMeshToList(starprojectile3);
    Mesh* starprojectile4 = object2D::CreateStar("proj_4", glm::vec3(0.f, 0.f, 0.1f), ProjectilestarOuterRadius, ProjectilestarInnerRadius, purpleColor, true);
    AddMeshToList(starprojectile4);

}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{

    // TODO(student): Update steps for translation, rotation and scale,
    // in order to create animations. Use the class variables in the
    // class header, and if you need more of them to complete the task,
    // add them over there!
    glm::ivec2 resolution = window->GetResolution();
    modelMatrix = glm::mat3(1);
    currentTime = glfwGetTime(); // Actualizăm timpul curent la fiecare frame


    // redner game scene
    RenderGameScene(modelMatrix);


    // Set up the model matrix for the rhombus
   // glm::mat3 modelMatrixRhombus = glm::mat3(1);
    // If you want to translate the rhombus to a certain position, you can do so like this:
    // modelMatrixRhombus *= transform2D::Translate(rhombusX, rhombusY);
    // Render the rhombus
   // RenderMesh2D(meshes["rhombus"], shaders["VertexColor"], modelMatrixRhombus);

    angularStep -= deltaTimeSeconds;
    // Set up the model matrix for the star
    glm::mat3 modelMatrixStar = glm::mat3(1);

    // Increment the translation to move the star to the right
    translateX += MOVE_RATIO * deltaTimeSeconds * resolution.x;

    // Apply the translation to the star's model matrix
    modelMatrixStar *= transform2D::Translate(translateX, 0);  // This will move the star right

    // Now apply the rotation transformations
    modelMatrixStar *= transform2D::Translate(175.f, 75.f);    // Translate to the center of rotation
    modelMatrixStar *= transform2D::Rotate(angularStep);       // Rotate by angularStep
    modelMatrixStar *= transform2D::Translate(-175.f, -75.f);  // Translate back


    // Verifică dacă steaua a atins punctul dorit pe axa x
    if (translateX < 1000) {
        // Calculează un factor de scalare bazat pe cât de departe este steaua de punctul dorit
       // float scaleFactor = fmax(1.0f - (translateX - 1000) / SCALE_FACTOR_X, 0.f);

        // Aplică scalarea pe stea
      //  modelMatrixStar *= transform2D::Scale(scaleFactor, scaleFactor);


        // Render the star
        RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrixStar);
    }



    std::vector<int> lineYCoordinates = { 0, 125, 250 };
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(0, enemies.size() - 1);
    std::uniform_int_distribution<int> uniIndex(0, lineYCoordinates.size() - 1);

 
    lastEnemySpawnTime += deltaTimeSeconds;
    if (lastEnemySpawnTime >= spawnDelay) {
        lastEnemySpawnTime = 0;

        Enemy newEnemy;

        //translateX2 -= MOVE_RATIO * deltaTimeSeconds * resolution.x;
        //modelMatrix *= transform2D::Translate(translateX2, translateY2);
        newEnemy.name = enemies[uni(rng)];
        if (newEnemy.name == "enemy1") {
            newEnemy.color = glm::vec3(1, 0.5, 0);
        }
        if (newEnemy.name == "enemy2") {
            newEnemy.color = glm::vec3(0, 0, 1);
        }
        if (newEnemy.name == "enemy3") {
            newEnemy.color = glm::vec3(1.0, 1.0, 0.0);
        }
        if (newEnemy.name == "enemy4") {
            newEnemy.color = glm::vec3(0.5, 0, 0.5);
        }
        newEnemy.position = glm::vec3(1240, lineYCoordinates[uniIndex(rng)], 0.1);
        newEnemy.mesh = meshes[newEnemy.name];
        enemiesList.push_back(newEnemy);
        //RenderMesh2D(meshes["enemy1"], shaders["VertexColor"], modelMatrix);
    }

    for (auto& enemy : enemiesList) {
        // Move each enemy to the right
        //std::cout << "DELTA" << deltaTimeSeconds << std::endl;
        //std::cout << "Before Update - Enemy X Position: " << enemy.position.x << std::endl;
        //std::cout << "MOVE_RATIO: " << MOVE_RATIO << ", deltaTimeSeconds: " << deltaTimeSeconds << ", resolution.x: " << resolution.x << std::endl;
        
        enemy.position.x -= 60.f * deltaTimeSeconds;

        //std::cout << "After Update - Enemy X Position: " << enemy.position.x << std::endl;
        
        //modelMatrix *= transform2D::Scale(0.f, 0.f);
        
        modelMatrix = glm::mat3(1.0f);
        modelMatrix *= transform2D::Translate(100, 75);
        modelMatrix *= transform2D::Translate(enemy.position.x, enemy.position.y);

        // Render each enemy
        //cout << enemy.position.x << " this" << endl;

        if (enemy.position.x > -1260 && enemy.position.x < 0) {
            //cout << enemy.position.y << " " << endl;
            if (enemy.position.y == 0) {
                if (enemy.color == glm::vec3(1, 0.5, 0)) {
                    en1l3 = 1;
                }
                else if (enemy.color == glm::vec3(0, 0, 1)) {
                    en2l3 = 1;
                }
                else if (enemy.color == glm::vec3(1, 1, 0)) {
                    en3l3 = 1;
                }
                else if (enemy.color == glm::vec3(0.5, 0, 0.5)) {
                    en4l3 = 1;
                }
            }
            if (enemy.position.y == 125) {
                if (enemy.color == glm::vec3(1, 0.5, 0)) {
                    en1l2 = 1;
                }
                else if (enemy.color == glm::vec3(0, 0, 1)) {
                    en2l2 = 1;
                }
                else if (enemy.color == glm::vec3(1, 1, 0)) {
                    en3l2 = 1;
                }
                else if (enemy.color == glm::vec3(0.5, 0, 0.5)) {
                    en4l2 = 1;
                }
            }
            if (enemy.position.y == 250) {
                if (enemy.color == glm::vec3(1, 0.5, 0)) {
                    en1l1 = 1;
                }
                else if (enemy.color == glm::vec3(0, 0, 1)) {
                    en2l1 = 1;
                }
                else if (enemy.color == glm::vec3(1, 1, 0)) {
                    en3l1 = 1;
                }
                else if (enemy.color == glm::vec3(0.5, 0, 0.5)) {
                    en4l1 = 1;
                }
            }
        }

        if (enemy.position.x <= -1260 && enemy.position.x >= -1300) {
            //cout << " SUNT INTRE GRANITE";
            if (enemy.position.y == 0) {
                if (enemy.color == glm::vec3(1, 0.5, 0)) {
                    en1l3 = 0;
                }
                else if (enemy.color == glm::vec3(0, 0, 1)) {
                    en2l3 = 0;

                }
                else if (enemy.color == glm::vec3(1, 1, 0)) {
                    en3l3 = 0;

                }
                else if (enemy.color == glm::vec3(0.5, 0, 0.5)) {
                    en4l3 = 0;

                }
            }
            if (enemy.position.y == 125) {
                if (enemy.color == glm::vec3(1, 0.5, 0)) {
                    en1l2 = 0;

                }
                else if (enemy.color == glm::vec3(0, 0, 1)) {
                    en2l2 = 0;

                }
                else if (enemy.color == glm::vec3(1, 1, 0)) {
                    en3l2 = 0;
                }
                else if (enemy.color == glm::vec3(0.5, 0, 0.5)) {
                    en4l2 = 0;
                }
            }
            if (enemy.position.y == 250) {
                if (enemy.color == glm::vec3(1, 0.5, 0)) {
                    en1l1 = 0;
                }
                else if (enemy.color == glm::vec3(0, 0, 1)) {
                    en2l1 = 0;
                }
                else if (enemy.color == glm::vec3(1, 1, 0)) {
                    en3l1 = 0;
                }
                else if (enemy.color == glm::vec3(0.5, 0, 0.5)) {
                    en4l1 = 0;
                }
            }

        }
     
        
        if (enemy.position.x > -1260) {

            RenderMesh2D(enemy.mesh, shaders["VertexColor"], modelMatrix);
        }
        else if (!enemy.countedForLifeLoss) {
            lifes--;
            enemy.countedForLifeLoss = true;  // Mark this enemy as counted
            std::cout << "LIFES" << lifes<< " " << endl;
        } 
    }

    // stele random pe ecran aparitie
    std::uniform_int_distribution<int> distX(100, 1180);  // Limita pe axa X
    std::uniform_int_distribution<int> distY(100, 620);  // Limita pe axa Y
    // În bucla de joc:
   
    
    //cout << "delta is" << deltaTimeSeconds<<" " << endl;
    

    // eroare nu stiu de ce starSpawnTime e asa mare
    starSpawnTime += deltaTimeSeconds ;
   // cout << "INTERVAL" << starSpawnInterval << endl;
    //cout << "INTERVAL" << starSpawnTime << endl;
    starSpawnInterval = 2.5f;
    if (starSpawnTime >= starSpawnInterval) {
     //   cout << "startSpawnTime" << " " << starSpawnTime << " " << endl;

        starSpawnTime = 0;
       // cout << "startSpawnTime" << " "<< starSpawnTime << " " << endl;
        //cout << "ASA DA" << endl;
        Star newStar;
        newStar.name = "star1";
        // Generați o poziție pentru steluța nouă
        newStar.position = glm::vec3(distX(rng), distY(rng), 0.2f);
        //cout << "POSITION IS" << " " << newStar.position << endl;
        newStar.mesh = meshes[newStar.name];
        starsList.push_back(newStar);
    }

    for (auto& star : starsList) {
        glm::mat3 modelStarList = glm::mat3(1.0f);
        modelStarList *= transform2D::Translate(star.position.x, star.position.y);
        RenderMesh2D(star.mesh, shaders["VertexColor"], modelStarList);
    }



    // animation for drag and drop and star appear from rhomb
    if (isSelected) {
        RenderMesh2D(meshes[selectedMeshName], shaders["VertexColor"], glm::mat3(1) *transform2D::Translate(dragPos.x, dragPos.y));
    }

    // if in grid display them
    if (placedRhombs[2][0].compare("") != 0) {
        RenderMesh2D(meshes[placedRhombs[2][0]], shaders["VertexColor"], glm::mat3(1) * transform2D::Translate(200, 100));

        starfromrhombusSpawnTime20 += deltaTimeSeconds;

        if (starfromrhombusSpawnTime20 > starfromrhombusSpawnInterval20) {
            starfromrhombusSpawnTime20 = 0;

            Projectile newProjectile;
            newProjectile.position = glm::vec3(175.f, 75.f, 0.1f);
            
            //cout << holdcolours[2][0] << " " << endl;
            //newProjectile.color = holdcolours[2][0]; // Culoarea pentru drag_1
            //newProjectile.name = "proj_1";
            //cout << en1l3 << " " << endl;
            if (en1l3 == 1) {
                if (holdcolours[2][0] == glm::vec3(1.f, 0.5f, 0.f)) {
                    newProjectile.name = "proj_1";
                }
            }
            if (en2l3 == 1) {
                if (holdcolours[2][0] == glm::vec3(0.f, 0.f, 1.f)) {
                    newProjectile.name = "proj_2";
                }
            }
            if (en3l3 == 1) {
                if (holdcolours[2][0] == glm::vec3(1.f, 1.f, 0.f)) {
                    newProjectile.name = "proj_3";
                }
            }
            if (en4l3 == 1) {
                if (holdcolours[2][0] == glm::vec3(0.5f, 0.f, 0.5f)) {
                    newProjectile.name = "proj_4";
                }
            }


            // culoarea sa o dau dupa nume?
           // newProjectile.color = holdcolours[2][0]; // Culoarea pentru drag_1
            newProjectile.mesh = meshes[newProjectile.name];
            projectilesList20.push_back(newProjectile);
        }



        for (auto& proj : projectilesList20) {
            glm::mat3 modelProjectileList = glm::mat3(1.0f);
            modelProjectileList *= transform2D::Translate(proj.position.x, proj.position.y);


            angularStep1 -= deltaTimeSeconds;
            // Set up the model matrix for the star
            glm::mat3 modelMatrixProjectile = glm::mat3(1);

            // Increment the translation to move the star to the right
            proj.position.x += MOVE_RATIO * 3 * deltaTimeSeconds * resolution.x;

            // Apply the translation to the star's model matrix
            modelMatrixProjectile *= transform2D::Translate(proj.position.x, proj.position.y);  // This will move the star right

            // Now apply the rotation transformations
             modelMatrixProjectile *= transform2D::Rotate(angularStep);       // Rotate by angularStep

            RenderMesh2D(proj.mesh, shaders["VertexColor"], modelMatrixProjectile);
        }
    }
    if (placedRhombs[2][1].compare("") != 0) {
        RenderMesh2D(meshes[placedRhombs[2][1]], shaders["VertexColor"], glm::mat3(1) * transform2D::Translate(325, 100));

        starfromrhombusSpawnTime21 += deltaTimeSeconds;

        if (starfromrhombusSpawnTime21 > starfromrhombusSpawnInterval21) {
            starfromrhombusSpawnTime21 = 0;

            Projectile newProjectile;
            newProjectile.position = glm::vec3(300.f, 75.f, 0.1f);
            
            if (en1l3 == 1) {
                if (holdcolours[2][1] == glm::vec3(1.f, 0.5f, 0.f)) {
                    newProjectile.name = "proj_1";
                }
            }
            if (en2l3 == 1) {
                if (holdcolours[2][1] == glm::vec3(0.f, 0.f, 1.f)) {
                    newProjectile.name = "proj_2";
                }
            }
            if (en3l3 == 1) {
                if (holdcolours[2][1] == glm::vec3(1.f, 1.f, 0.f)) {
                    newProjectile.name = "proj_3";
                }
            }
            if (en4l3 == 1) {
                if (holdcolours[2][1] == glm::vec3(0.5f, 0.f, 0.5f)) {
                    newProjectile.name = "proj_4";
                }
            }

            newProjectile.mesh = meshes[newProjectile.name];
            projectilesList21.push_back(newProjectile);
        }


        for (auto& proj : projectilesList21) {
            glm::mat3 modelProjectileList = glm::mat3(1.0f);
            modelProjectileList *= transform2D::Translate(proj.position.x, proj.position.y);


            angularStep2 -= deltaTimeSeconds;
            // Set up the model matrix for the star
            glm::mat3 modelMatrixProjectile = glm::mat3(1);

            // Increment the translation to move the star to the right
            proj.position.x += MOVE_RATIO * 3 * deltaTimeSeconds * resolution.x;

            // Apply the translation to the star's model matrix
            modelMatrixProjectile *= transform2D::Translate(proj.position.x, proj.position.y);  // This will move the star right

            // Now apply the rotation transformations
            modelMatrixProjectile *= transform2D::Rotate(angularStep);       // Rotate by angularStep

            RenderMesh2D(proj.mesh, shaders["VertexColor"], modelMatrixProjectile);
        }
    }
    if (placedRhombs[2][2].compare("") != 0) {
        RenderMesh2D(meshes[placedRhombs[2][2]], shaders["VertexColor"], glm::mat3(1) * transform2D::Translate(450, 100));

        starfromrhombusSpawnTime22 += deltaTimeSeconds;

        if (starfromrhombusSpawnTime22 > starfromrhombusSpawnInterval22) {
            starfromrhombusSpawnTime22 = 0;

            Projectile newProjectile;
            newProjectile.position = glm::vec3(425.f, 75.f, 0.1f);

            if (en1l3 == 1) {
                if (holdcolours[2][2] == glm::vec3(1.f, 0.5f, 0.f)) {
                    newProjectile.name = "proj_1";
                }
            }
            if (en2l3 == 1) {
                if (holdcolours[2][2] == glm::vec3(0.f, 0.f, 1.f)) {
                    newProjectile.name = "proj_2";
                }
            }
            if (en3l3 == 1) {
                if (holdcolours[2][2] == glm::vec3(1.f, 1.f, 0.f)) {
                    newProjectile.name = "proj_3";
                }
            }
            if (en4l3 == 1) {
                if (holdcolours[2][2] == glm::vec3(0.5f, 0.f, 0.5f)) {
                    newProjectile.name = "proj_4";
                }
            }

            newProjectile.mesh = meshes[newProjectile.name];
            projectilesList22.push_back(newProjectile);
        }


        for (auto& proj : projectilesList22) {
            glm::mat3 modelProjectileList = glm::mat3(1.0f);
            modelProjectileList *= transform2D::Translate(proj.position.x, proj.position.y);


            angularStep3 -= deltaTimeSeconds;
            // Set up the model matrix for the star
            glm::mat3 modelMatrixProjectile = glm::mat3(1);

            // Increment the translation to move the star to the right
            proj.position.x += MOVE_RATIO * 3 * deltaTimeSeconds * resolution.x;

            // Apply the translation to the star's model matrix
            modelMatrixProjectile *= transform2D::Translate(proj.position.x, proj.position.y);  // This will move the star right

            // Now apply the rotation transformations
            modelMatrixProjectile *= transform2D::Rotate(angularStep);       // Rotate by angularStep

            RenderMesh2D(proj.mesh, shaders["VertexColor"], modelMatrixProjectile);
        }
    }
    if (placedRhombs[1][0].compare("") != 0) {
        RenderMesh2D(meshes[placedRhombs[1][0]], shaders["VertexColor"], glm::mat3(1) * transform2D::Translate(200, 225));


        starfromrhombusSpawnTime10 += deltaTimeSeconds;

        if (starfromrhombusSpawnTime10 > starfromrhombusSpawnInterval10) {
            starfromrhombusSpawnTime10 = 0;

            Projectile newProjectile;
            newProjectile.position = glm::vec3(175.f, 200.f, 0.1f);
            
            if (en1l2 == 1) {
                if (holdcolours[1][0] == glm::vec3(1.f, 0.5f, 0.f)) {
                    newProjectile.name = "proj_1";
                }
            }
            if (en2l2 == 1) {
                if (holdcolours[1][0] == glm::vec3(0.f, 0.f, 1.f)) {
                    newProjectile.name = "proj_2";
                }
            }
            if (en3l2 == 1) {
                if (holdcolours[1][0] == glm::vec3(1.f, 1.f, 0.f)) {
                    newProjectile.name = "proj_3";
                }
            }
            if (en4l2 == 1) {
                if (holdcolours[1][0] == glm::vec3(0.5f, 0.f, 0.5f)) {
                    newProjectile.name = "proj_4";
                }
            }

            newProjectile.mesh = meshes[newProjectile.name];
            projectilesList10.push_back(newProjectile);
        }


        for (auto& proj : projectilesList10) {
            glm::mat3 modelProjectileList = glm::mat3(1.0f);
            modelProjectileList *= transform2D::Translate(proj.position.x, proj.position.y);


            angularStep4 -= deltaTimeSeconds;
            // Set up the model matrix for the star
            glm::mat3 modelMatrixProjectile = glm::mat3(1);

            // Increment the translation to move the star to the right
            proj.position.x += MOVE_RATIO * 3 * deltaTimeSeconds * resolution.x;

            // Apply the translation to the star's model matrix
            modelMatrixProjectile *= transform2D::Translate(proj.position.x, proj.position.y);  // This will move the star right

            // Now apply the rotation transformations
            modelMatrixProjectile *= transform2D::Rotate(angularStep);       // Rotate by angularStep

            RenderMesh2D(proj.mesh, shaders["VertexColor"], modelMatrixProjectile);
        }
    }
    if (placedRhombs[1][1].compare("") != 0) {
        RenderMesh2D(meshes[placedRhombs[1][1]], shaders["VertexColor"], glm::mat3(1) * transform2D::Translate(325, 225));

        starfromrhombusSpawnTime11 += deltaTimeSeconds;

        if (starfromrhombusSpawnTime11 > starfromrhombusSpawnInterval11) {
            starfromrhombusSpawnTime11 = 0;

            Projectile newProjectile;
            newProjectile.position = glm::vec3(300.f, 200.f, 0.1f);
            
            if (en1l2 == 1) {
                if (holdcolours[1][1] == glm::vec3(1.f, 0.5f, 0.f)) {
                    newProjectile.name = "proj_1";
                }
            }
            if (en2l2 == 1) {
                if (holdcolours[1][1] == glm::vec3(0.f, 0.f, 1.f)) {
                    newProjectile.name = "proj_2";
                }
            }
            if (en3l2 == 1) {
                if (holdcolours[1][1] == glm::vec3(1.f, 1.f, 0.f)) {
                    newProjectile.name = "proj_3";
                }
            }
            if (en4l2 == 1) {
                if (holdcolours[1][1] == glm::vec3(0.5f, 0.f, 0.5f)) {
                    newProjectile.name = "proj_4";
                }
            }

            newProjectile.mesh = meshes[newProjectile.name];
            projectilesList11.push_back(newProjectile);
        }


        for (auto& proj : projectilesList11) {
            glm::mat3 modelProjectileList = glm::mat3(1.0f);
            modelProjectileList *= transform2D::Translate(proj.position.x, proj.position.y);


            angularStep5 -= deltaTimeSeconds;
            // Set up the model matrix for the star
            glm::mat3 modelMatrixProjectile = glm::mat3(1);

            // Increment the translation to move the star to the right
            proj.position.x += MOVE_RATIO * 3 * deltaTimeSeconds * resolution.x;

            // Apply the translation to the star's model matrix
            modelMatrixProjectile *= transform2D::Translate(proj.position.x, proj.position.y);  // This will move the star right

            // Now apply the rotation transformations
            modelMatrixProjectile *= transform2D::Rotate(angularStep);       // Rotate by angularStep

            RenderMesh2D(proj.mesh, shaders["VertexColor"], modelMatrixProjectile);
        }
    }
    if (placedRhombs[1][2].compare("") != 0) {
        RenderMesh2D(meshes[placedRhombs[1][2]], shaders["VertexColor"], glm::mat3(1) * transform2D::Translate(450, 225));

        starfromrhombusSpawnTime12 += deltaTimeSeconds;

        if (starfromrhombusSpawnTime12 > starfromrhombusSpawnInterval12) {
            starfromrhombusSpawnTime12 = 0;

            Projectile newProjectile;
            newProjectile.position = glm::vec3(425.f, 200.f, 0.1f);
            
            if (en1l2 == 1) {
                if (holdcolours[1][2] == glm::vec3(1.f, 0.5f, 0.f)) {
                    newProjectile.name = "proj_1";
                }
            }
            if (en2l2 == 1) {
                if (holdcolours[1][2] == glm::vec3(0.f, 0.f, 1.f)) {
                    newProjectile.name = "proj_2";
                }
            }
            if (en3l2 == 1) {
                if (holdcolours[1][2] == glm::vec3(1.f, 1.f, 0.f)) {
                    newProjectile.name = "proj_3";
                }
            }
            if (en4l2 == 1) {
                if (holdcolours[1][2] == glm::vec3(0.5f, 0.f, 0.5f)) {
                    newProjectile.name = "proj_4";
                }
            }

            newProjectile.mesh = meshes[newProjectile.name];
            projectilesList12.push_back(newProjectile);
        }


        for (auto& proj : projectilesList12) {
            glm::mat3 modelProjectileList = glm::mat3(1.0f);
            modelProjectileList *= transform2D::Translate(proj.position.x, proj.position.y);


            angularStep6 -= deltaTimeSeconds;
            // Set up the model matrix for the star
            glm::mat3 modelMatrixProjectile = glm::mat3(1);

            // Increment the translation to move the star to the right
            proj.position.x += MOVE_RATIO * 3 * deltaTimeSeconds * resolution.x;

            // Apply the translation to the star's model matrix
            modelMatrixProjectile *= transform2D::Translate(proj.position.x, proj.position.y);  // This will move the star right

            // Now apply the rotation transformations
            modelMatrixProjectile *= transform2D::Rotate(angularStep);       // Rotate by angularStep

            RenderMesh2D(proj.mesh, shaders["VertexColor"], modelMatrixProjectile);
        }
    }
    if (placedRhombs[0][0].compare("") != 0) {
        RenderMesh2D(meshes[placedRhombs[0][0]], shaders["VertexColor"], glm::mat3(1) * transform2D::Translate(200, 350));

        starfromrhombusSpawnTime00 += deltaTimeSeconds;

        if (starfromrhombusSpawnTime00 > starfromrhombusSpawnInterval00) {
            starfromrhombusSpawnTime00 = 0;

            Projectile newProjectile;
            newProjectile.position = glm::vec3(175.f, 325.f, 0.1f);
            
            if (en1l1 == 1) {
                if (holdcolours[0][0] == glm::vec3(1.f, 0.5f, 0.f)) {
                    newProjectile.name = "proj_1";
                }
            }
            if (en2l1 == 1) {
                if (holdcolours[0][0] == glm::vec3(0.f, 0.f, 1.f)) {
                    newProjectile.name = "proj_2";
                }
            }
            if (en3l1 == 1) {
                if (holdcolours[0][0] == glm::vec3(1.f, 1.f, 0.f)) {
                    newProjectile.name = "proj_3";
                }
            }
            if (en4l1 == 1) {
                if (holdcolours[0][0] == glm::vec3(0.5f, 0.f, 0.5f)) {
                    newProjectile.name = "proj_4";
                }
            }

            newProjectile.mesh = meshes[newProjectile.name];
            projectilesList00.push_back(newProjectile);
        }


        for (auto& proj : projectilesList00) {
            glm::mat3 modelProjectileList = glm::mat3(1.0f);
            modelProjectileList *= transform2D::Translate(proj.position.x, proj.position.y);


            angularStep7 -= deltaTimeSeconds;
            // Set up the model matrix for the star
            glm::mat3 modelMatrixProjectile = glm::mat3(1);

            // Increment the translation to move the star to the right
            proj.position.x += MOVE_RATIO * 3 * deltaTimeSeconds * resolution.x;

            // Apply the translation to the star's model matrix
            modelMatrixProjectile *= transform2D::Translate(proj.position.x, proj.position.y);  // This will move the star right

            // Now apply the rotation transformations
            modelMatrixProjectile *= transform2D::Rotate(angularStep);       // Rotate by angularStep

            RenderMesh2D(proj.mesh, shaders["VertexColor"], modelMatrixProjectile);
        }
    }
    if (placedRhombs[0][1].compare("") != 0) {
        RenderMesh2D(meshes[placedRhombs[0][1]], shaders["VertexColor"], glm::mat3(1) * transform2D::Translate(325, 350));

        starfromrhombusSpawnTime01 += deltaTimeSeconds;

        if (starfromrhombusSpawnTime01 > starfromrhombusSpawnInterval01) {
            starfromrhombusSpawnTime01 = 0;

            Projectile newProjectile;
            newProjectile.position = glm::vec3(300.f, 325.f, 0.1f);
            
            if (en1l1 == 1) {
                if (holdcolours[0][1] == glm::vec3(1.f, 0.5f, 0.f)) {
                    newProjectile.name = "proj_1";
                }
            }
            if (en2l1 == 1) {
                if (holdcolours[0][1] == glm::vec3(0.f, 0.f, 1.f)) {
                    newProjectile.name = "proj_2";
                }
            }
            if (en3l1 == 1) {
                if (holdcolours[0][1] == glm::vec3(1.f, 1.f, 0.f)) {
                    newProjectile.name = "proj_3";
                }
            }
            if (en4l1 == 1) {
                if (holdcolours[0][1] == glm::vec3(0.5f, 0.f, 0.5f)) {
                    newProjectile.name = "proj_4";
                }
            }

            newProjectile.mesh = meshes[newProjectile.name];
            projectilesList01.push_back(newProjectile);
        }


        for (auto& proj : projectilesList01) {
            glm::mat3 modelProjectileList = glm::mat3(1.0f);
            modelProjectileList *= transform2D::Translate(proj.position.x, proj.position.y);


            angularStep8 -= deltaTimeSeconds;
            // Set up the model matrix for the star
            glm::mat3 modelMatrixProjectile = glm::mat3(1);

            // Increment the translation to move the star to the right
            proj.position.x += MOVE_RATIO * 3 * deltaTimeSeconds * resolution.x;

            // Apply the translation to the star's model matrix
            modelMatrixProjectile *= transform2D::Translate(proj.position.x, proj.position.y);  // This will move the star right

            // Now apply the rotation transformations
            modelMatrixProjectile *= transform2D::Rotate(angularStep);       // Rotate by angularStep

            RenderMesh2D(proj.mesh, shaders["VertexColor"], modelMatrixProjectile);
        }
    }
    if (placedRhombs[0][2].compare("") != 0) {
        RenderMesh2D(meshes[placedRhombs[0][2]], shaders["VertexColor"], glm::mat3(1) * transform2D::Translate(450, 350));


        starfromrhombusSpawnTime02 += deltaTimeSeconds;

        if (starfromrhombusSpawnTime02 > starfromrhombusSpawnInterval02) {
            starfromrhombusSpawnTime02 = 0;

            Projectile newProjectile;
            newProjectile.position = glm::vec3(425.f, 325.f, 0.1f);
            
            if (en1l1 == 1) {
                if (holdcolours[0][2] == glm::vec3(1.f, 0.5f, 0.f)) {
                    newProjectile.name = "proj_1";
                }
            }
            if (en2l1 == 1) {
                if (holdcolours[0][2] == glm::vec3(0.f, 0.f, 1.f)) {
                    newProjectile.name = "proj_2";
                }
            }
            if (en3l1 == 1) {
                if (holdcolours[0][2] == glm::vec3(1.f, 1.f, 0.f)) {
                    newProjectile.name = "proj_3";
                }
            }
            if (en4l1 == 1) {
                if (holdcolours[0][2] == glm::vec3(0.5f, 0.f, 0.5f)) {
                    newProjectile.name = "proj_4";
                }
            }

            newProjectile.mesh = meshes[newProjectile.name];
            projectilesList02.push_back(newProjectile);
        }


        for (auto& proj : projectilesList02) {
            glm::mat3 modelProjectileList = glm::mat3(1.0f);
            modelProjectileList *= transform2D::Translate(proj.position.x, proj.position.y);


            angularStep9 -= deltaTimeSeconds;
            // Set up the model matrix for the star
            glm::mat3 modelMatrixProjectile = glm::mat3(1);

            // Increment the translation to move the star to the right
            proj.position.x += MOVE_RATIO * 3 * deltaTimeSeconds * resolution.x;

            // Apply the translation to the star's model matrix
            modelMatrixProjectile *= transform2D::Translate(proj.position.x, proj.position.y);  // This will move the star right

            // Now apply the rotation transformations
            modelMatrixProjectile *= transform2D::Rotate(angularStep);       // Rotate by angularStep

            RenderMesh2D(proj.mesh, shaders["VertexColor"], modelMatrixProjectile);
        }
    }
}



void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}



void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (isSelected) {
        dragPos = glm::vec2(mouseX, window->GetResolution().y - mouseY);
    }
}

// Funcția pentru a verifica dacă mouse-ul este peste o stea
bool isMouseOverStar(const Star& star, const glm::vec2& mousePosition) {
    float starRadius = 30; // Raza stelei

    float dx = mousePosition.x - star.position.x;
    float dy = mousePosition.y - star.position.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    return distance < starRadius;
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    int hallowpadding = 70;
    float startXhallowTriangles = 100;
    float startYhallowTriangles = 600;
    float hallowsquareSide = 100;

    float x = mouseX;
    float y = window->GetResolution().y - mouseY; // Converteste coordonatele mouse - ului
    // Add mouse button press event
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
        cout << x << " " << y << endl;
        // Check if a rhombus is under the mouse cursor
        if (x > 100 && x < 200 && y < 700 && y > 600 && starsaquired >=1) {
            isSelected = true;
            selectedMeshName = "drag_1";
            starsaquired -= 1;
        }
        if (x > 270 && x < 370 && y < 700 && y > 600 && starsaquired >= 2) {
            isSelected = true;
            selectedMeshName = "drag_2";
            starsaquired -= 2;
        }
        if (x > 440 && x < 540 && y < 700 && y > 600 && starsaquired >= 2) {
            isSelected = true;
            selectedMeshName = "drag_3";
            starsaquired -= 2;
        }
        if (x > 610 && x < 710 && y < 700 && y > 600 && starsaquired >= 3) {
            isSelected = true;
            selectedMeshName = "drag_4";
            starsaquired -= 3;
        }
        for (auto it = starsList.begin(); it != starsList.end(); ) {
            if (isMouseOverStar(*it, glm::vec2(x, y))) {
                // Logica pentru când mouse-ul este peste o stea
                // De exemplu, eliminați steaua și actualizați scorul sau colectați steaua
                starsaquired++;
                cout << starsaquired<<endl;
                it = starsList.erase(it);
            }
            else {
                ++it;
            }
        }

    }
}




void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    float x = mouseX;
    float y = window->GetResolution().y - mouseY;
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
        // rand 3
        if (isSelected == true) {
            if (x > 125 && x < 225 && y > 25 && y < 125) {
               // cout << selectedMeshName << " " << endl;
                placedRhombs[2][0] = selectedMeshName + "";
                if (selectedMeshName == "drag_1") {
                    holdcolours[2][0] = glm::vec3(1.f, 0.5f, 0.f);
                  //  cout << holdcolours[2][0] << " " << endl;
                }
                if (selectedMeshName == "drag_2") {
                    holdcolours[2][0] = glm::vec3(0.f, 0.f, 1.f);
                }

                if (selectedMeshName == "drag_3") {
                    holdcolours[2][0] = glm::vec3(1.f, 1.f, 0.f);
                }
                if (selectedMeshName == "drag_4") {
                    holdcolours[2][0] = glm::vec3(0.5f, 0.f, 0.5f);
                }
            }

        }
        if (isSelected == true) {
            if (x > 250 && x < 350 && y > 25 && y < 125) {
                placedRhombs[2][1] = selectedMeshName + "";

                if (selectedMeshName == "drag_1") {
                    holdcolours[2][1] = glm::vec3(1.f, 0.5f, 0.f);
                }
                if (selectedMeshName == "drag_2") {
                    holdcolours[2][1] = glm::vec3(0.f, 0.f, 1.f);
                }

                if (selectedMeshName == "drag_3") {
                    holdcolours[2][1] = glm::vec3(1.f, 1.f, 0.f);
                }
                if (selectedMeshName == "drag_4") {
                    holdcolours[2][1] = glm::vec3(0.5f, 0.f, 0.5f);
                }
            }
        }
        if (isSelected == true) {
            if (x > 375 && x < 475 && y > 25 && y < 125) {
                placedRhombs[2][2] = selectedMeshName + "";

                if (selectedMeshName == "drag_1") {
                    holdcolours[2][2] = glm::vec3(1.f, 0.5f, 0.f);
                }
                if (selectedMeshName == "drag_2") {
                    holdcolours[2][2] = glm::vec3(0.f, 0.f, 1.f);
                }

                if (selectedMeshName == "drag_3") {
                    holdcolours[2][2] = glm::vec3(1.f, 1.f, 0.f);
                }
                if (selectedMeshName == "drag_4") {
                    holdcolours[2][2] = glm::vec3(0.5f, 0.f, 0.5f);
                }
            }
        }

        // rand 2
        if (isSelected == true) {
            if (x > 125 && x < 225 && y > 150 && y < 250) {
                placedRhombs[1][0] = selectedMeshName + "";

                if (selectedMeshName == "drag_1") {
                    holdcolours[1][0] = glm::vec3(1.f, 0.5f, 0.f);
                }
                if (selectedMeshName == "drag_2") {
                    holdcolours[1][0] = glm::vec3(0.f, 0.f, 1.f);
                }

                if (selectedMeshName == "drag_3") {
                    holdcolours[1][0] = glm::vec3(1.f, 1.f, 0.f);
                }
                if (selectedMeshName == "drag_4") {
                    holdcolours[1][0] = glm::vec3(0.5f, 0.f, 0.5f);
                }
            }

        }
        if (isSelected == true) {
            if (x > 250 && x < 350 && y > 150 && y < 250) {
                placedRhombs[1][1] = selectedMeshName + "";
                if (selectedMeshName == "drag_1") {
                    holdcolours[1][1] = glm::vec3(1.f, 0.5f, 0.f);
                }
                if (selectedMeshName == "drag_2") {
                    holdcolours[1][1] = glm::vec3(0.f, 0.f, 1.f);
                }

                if (selectedMeshName == "drag_3") {
                    holdcolours[1][1] = glm::vec3(1.f, 1.f, 0.f);
                }
                if (selectedMeshName == "drag_4") {
                    holdcolours[1][1] = glm::vec3(0.5f, 0.f, 0.5f);
                }
            }
        }
        if (isSelected == true) {
            if (x > 375 && x < 475 && y > 150 && y < 250) {
                placedRhombs[1][2] = selectedMeshName + "";

                if (selectedMeshName == "drag_1") {
                    holdcolours[1][2] = glm::vec3(1.f, 0.5f, 0.f);
                }
                if (selectedMeshName == "drag_2") {
                    holdcolours[1][2] = glm::vec3(0.f, 0.f, 1.f);
                }

                if (selectedMeshName == "drag_3") {
                    holdcolours[1][2] = glm::vec3(1.f, 1.f, 0.f);
                }
                if (selectedMeshName == "drag_4") {
                    holdcolours[1][2] = glm::vec3(0.5f, 0.f, 0.5f);
                }
            }
        }

        // rand1 de sus in jos le numerotai
        if (isSelected == true) {
            if (x > 125 && x < 225 && y > 275 && y < 375) {
                placedRhombs[0][0] = selectedMeshName + "";

                if (selectedMeshName == "drag_1") {
                    holdcolours[0][0] = glm::vec3(1.f, 0.5f, 0.f);
                }
                if (selectedMeshName == "drag_2") {
                    holdcolours[0][0] = glm::vec3(0.f, 0.f, 1.f);
                }

                if (selectedMeshName == "drag_3") {
                    holdcolours[0][0] = glm::vec3(1.f, 1.f, 0.f);
                }
                if (selectedMeshName == "drag_4") {
                    holdcolours[0][0] = glm::vec3(0.5f, 0.f, 0.5f);
                }
            }

        }
        if (isSelected == true) {
            if (x > 250 && x < 350 && y > 275 && y < 375) {
                placedRhombs[0][1] = selectedMeshName + "";

                if (selectedMeshName == "drag_1") {
                    holdcolours[0][1] = glm::vec3(1.f, 0.5f, 0.f);
                }
                if (selectedMeshName == "drag_2") {
                    holdcolours[0][1] = glm::vec3(0.f, 0.f, 1.f);
                }

                if (selectedMeshName == "drag_3") {
                    holdcolours[0][1] = glm::vec3(1.f, 1.f, 0.f);
                }
                if (selectedMeshName == "drag_4") {
                    holdcolours[0][1] = glm::vec3(0.5f, 0.f, 0.5f);
                }
            }
        }
        if (isSelected == true) {
            if (x > 375 && x < 475 && y > 275 && y < 375) {
                placedRhombs[0][2] = selectedMeshName + "";
                if (selectedMeshName == "drag_1") {
                    holdcolours[0][2] = glm::vec3(1.f, 0.5f, 0.f);
                }
                if (selectedMeshName == "drag_2") {
                    holdcolours[0][2] = glm::vec3(0.f, 0.f, 1.f);
                }

                if (selectedMeshName == "drag_3") {
                    holdcolours[0][2] = glm::vec3(1.f, 1.f, 0.f);
                }
                if (selectedMeshName == "drag_4") {
                    holdcolours[0][2] = glm::vec3(0.5f, 0.f, 0.5f);
                }
            }
        }
        isSelected = false;
    }

    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT)) {
        if (x > 125 && x < 225 && y > 25 && y < 125) {

            if (selectedMeshName == "drag_1") {
                en1l3 = 0;
            }
            if (selectedMeshName == "drag_2") {
                en2l3 = 0;
            }
            if (selectedMeshName == "drag_3") {
                en3l3 = 0;
            }
            if (selectedMeshName == "drag_4") {
                en4l3 = 0;
            }
            placedRhombs[2][0] = "";
            projectilesList20.clear();
            holdcolours[2][0] = glm::vec3(0.0f);

        }
        if (x > 250 && x < 350 && y > 25 && y < 125) {
            if (selectedMeshName == "drag_1") {
                en1l3 = 0;
            }
            if (selectedMeshName == "drag_2") {
                en2l3 = 0;
            }
            if (selectedMeshName == "drag_3") {
                en3l3 = 0;
            }
            if (selectedMeshName == "drag_4") {
                en4l3 = 0;
            }
            placedRhombs[2][1] = "";
            projectilesList21.clear();
            holdcolours[2][1] = glm::vec3(0.0f);
        }
        if (x > 375 && x < 475 && y > 25 && y < 125) {
            if (selectedMeshName == "drag_1") {
                en1l3 = 0;
            }
            if (selectedMeshName == "drag_2") {
                en2l3 = 0;
            }
            if (selectedMeshName == "drag_3") {
                en3l3 = 0;
            }
            if (selectedMeshName == "drag_4") {
                en4l3 = 0;
            }
            placedRhombs[2][2] = "";
            projectilesList22.clear();
            holdcolours[2][2] = glm::vec3(0.0f);
        }

        if (x > 125 && x < 225 && y > 150 && y < 250) {
            if (selectedMeshName == "drag_1") {
                en1l2 = 0;
            }
            if (selectedMeshName == "drag_2") {
                en2l2 = 0;
            }
            if (selectedMeshName == "drag_3") {
                en3l2 = 0;
            }
            if (selectedMeshName == "drag_4") {
                en4l2 = 0;
            }
            placedRhombs[1][0] = "";
            projectilesList10.clear();
            holdcolours[1][0] = glm::vec3(0.0f);
        }
        if (x > 250 && x < 350 && y > 150 && y < 250) {
            if (selectedMeshName == "drag_1") {
                en1l2 = 0;
            }
            if (selectedMeshName == "drag_2") {
                en2l2 = 0;
            }
            if (selectedMeshName == "drag_3") {
                en3l2 = 0;
            }
            if (selectedMeshName == "drag_4") {
                en4l2 = 0;
            }
            placedRhombs[1][1] = "";
            projectilesList11.clear();
            holdcolours[1][1] = glm::vec3(0.0f);
        }
        if (x > 375 && x < 475 && y > 150 && y < 250) {
            if (selectedMeshName == "drag_1") {
                en1l2 = 0;
            }
            if (selectedMeshName == "drag_2") {
                en2l2 = 0;
            }
            if (selectedMeshName == "drag_3") {
                en3l2 = 0;
            }
            if (selectedMeshName == "drag_4") {
                en4l2 = 0;
            }
            placedRhombs[1][2] = "";
            projectilesList12.clear();
            holdcolours[1][2] = glm::vec3(0.0f);
        }
        if (x > 125 && x < 225 && y > 275 && y < 375) {
            if (selectedMeshName == "drag_1") {
                en1l1 = 0;
            }
            if (selectedMeshName == "drag_2") {
                en2l1 = 0;
            }
            if (selectedMeshName == "drag_3") {
                en3l1 = 0;
            }
            if (selectedMeshName == "drag_4") {
                en4l1 = 0;
            }
            placedRhombs[0][0] = "";
            projectilesList00.clear();
            holdcolours[0][0] = glm::vec3(0.0f);
        }
        if (x > 250 && x < 350 && y > 275 && y < 375) {
            if (selectedMeshName == "drag_1") {
                en1l1 = 0;
            }
            if (selectedMeshName == "drag_2") {
                en2l1 = 0;
            }
            if (selectedMeshName == "drag_3") {
                en3l1 = 0;
            }
            if (selectedMeshName == "drag_4") {
                en4l1 = 0;
            }
            placedRhombs[0][1] = "";
            projectilesList01.clear();
            holdcolours[0][1] = glm::vec3(0.0f);
        }
        if (x > 375 && x < 475 && y > 275 && y < 375) {
            if (selectedMeshName == "drag_1") {
                en1l1 = 0;
            }
            if (selectedMeshName == "drag_2") {
                en2l1 = 0;
            }
            if (selectedMeshName == "drag_3") {
                en3l1 = 0;
            }
            if (selectedMeshName == "drag_4") {
                en4l1 = 0;
            }
            placedRhombs[0][2] = "";
            projectilesList02.clear();
            holdcolours[0][2] = glm::vec3(0.0f);
        }
    }
}



void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}