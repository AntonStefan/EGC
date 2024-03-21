#include "lab_m1/Tema2/Tema2.h"
#include "lab_m1/Tema2/transform3D.h"
#include "lab_m1/Tema2/object3D.h"

#include<random>
#include <vector>
#include <string>
#include <iostream>
#include<cstring>

// Tank struct definition
struct Tank {
    std::string name;
    glm::vec3 position;
    float rotation;
    glm::vec3 direction;
    std::vector<Mesh*> componentsList;
    float turretringroation;
    glm::vec3 balldirection;
    // Add any other tank-specific properties as needed
};

// Create a vector to hold multiple tanks
std::vector<Tank> tanksList;


struct EnemyTank {
    std::string name;
    glm::vec3 position;
    float rotation;
    glm::vec3 direction;
    std::vector<Mesh*> componentsList;
    float turretringroation;
    glm::vec3 balldirection;
    // Add any other tank-specific properties as needed
};

// Create a vector to hold multiple tanks
std::vector<EnemyTank> enemytanksList;


float lastSpawnTime;  // hold how much the projectile resist
float timetolive = 3.f;

int ok = 0;
float timeformove = 5.f;
float counter = 0.f;
int direction = 4;
float distance = 0.f;

float worldtimer = 0.f;  // end game after 90 seconds

struct TankBall {
    std::string name;
    glm::vec3 position;
    glm::vec3 direction;
    float speed;
    bool isActive;
    Mesh* mesh;
    // Other properties like mesh, color, etc.
};

std::vector<TankBall> tankballs;

float turretRotationSensitivity = 0.0005f;

float directionTime = 0.f;
float directionInterval = 5.f;


using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    renderCameraTarget = false;

    projectionType = true;

    right = 10.f;
    left = .01f;
    bottom = .01f;
    top = 10.f;
    fov = 40.f;
    Z_FAR = 200.f;
    Z_NEAR = 0.01f;



    camera = new implemented::Camera();
    camera->Set(glm::vec3(-6, 6, 0.f), glm::vec3(0, 2, 0), glm::vec3(0, 1, 0));  // daca pun la ultima 5 pe z ma pune cu susu in jos
    /*
    // TURRET

   glm::vec3 turretBottomLeftCorner = glm::vec3(1.6, 1.6, -0.25); // is not working pos bc i updated it in create
    float baseSide = 1.0f; // 1 unit side length for base
    float barrelLength = 2.0f; // Barrel is 2 units long
    float barrelRadius = 0.3f; // Barrel radius is 0.3 units
    glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f); // Grey color
    bool fill = true; // Solid turret

    // Create the rectangle
    std::string turretName = "turret_1"; // A name for the rectangle mesh
    Mesh* turret = object3D::CreateTurret(turretName, turretBottomLeftCorner, baseSide, barrelLength, barrelRadius, color, true);
    AddMeshToList(turret);


    // HULL

    glm::vec3 HullPosition = glm::vec3(0.95, 0.45, -0.25); // Position at the center of the tank
    float HullWidth = 4.0f; // The side length of the turret ring's base
    float HullLength= 3.0f; // The side length of the turret ring's base
    float HullHeight = 0.8f; // The height of the turret ring
    float HullSlope = 0.5f; // The trim amount for the sloped sides
    glm::vec3 HullColor = glm::vec3(0.f, 0.4f, 0.f); // Color of the turret ring

    std::string HullName = "hull_1"; // A name for the rectangle mesh
    

    Mesh* hull = object3D::CreateHull(HullName, HullPosition, HullWidth, HullLength, HullHeight, HullSlope, HullColor, true);
    AddMeshToList(hull);


    // TRACKS
 
    // Define the properties for the tank track
    std::string trackNameRight = "TankTrackRight";
    glm::vec3 trackPositionRight = glm::vec3(1.0f, 0.3f, 1.0f); // Center of the track
    float trackLengthRight = 2.0f; // Total length of the track
    float trackWidthRight = 4.5f; // Width of the track
    float trackHeightRight = 0.5f; // Height of the track
    float cutDepthRight = 0.5f; // Depth of the cut from the front and back
    float trackThicknessRight = 0.5f; // Thickness of the track (along Z-axis)
    glm::vec3 trackColorRight = glm::vec3(0.5f, 0.5f, 0.5f); // Grey color for the track

    // Create the tank track using the function you've defined
    Mesh* tankTrack = object3D::CreateTankTrack(
        trackNameRight,
        trackPositionRight,
        trackLengthRight,
        trackWidthRight,
        trackHeightRight,
        cutDepthRight,
        trackThicknessRight, // Thickness of the track (along Z-axis)
        trackColorRight,
        true
    );

    AddMeshToList(tankTrack);

    // Define the properties for the tank track
    std::string trackNameLeft = "TankTrackLeft";
    glm::vec3 trackPositionLeft = glm::vec3(1.0f, 0.3f, -1.5f); // Center of the track
    float trackLengthLeft = 2.0f; // Total length of the track
    float trackWidthLeft = 4.5f; // Width of the track
    float trackHeightLeft = 0.5f; // Height of the track
    float cutDepthLeft = 0.5f; // Depth of the cut from the front and back
    float trackThicknessLeft = 0.5f; // Thickness of the track (along Z-axis)
    glm::vec3 trackColorLeft = glm::vec3(0.5f, 0.5f, 0.5f); // Grey color for the track

    // Create the tank track using the function you've defined
    Mesh* tankTrackLeft = object3D::CreateTankTrack(
        trackNameLeft,
        trackPositionLeft,
        trackLengthLeft,
        trackWidthLeft,
        trackHeightLeft,
        cutDepthLeft,
        trackThicknessLeft, // Thickness of the track (along Z-axis)
        trackColorLeft,
        true
    );

    AddMeshToList(tankTrackLeft);




    // PROJECTILE
    std::string sphereName = "Projectile_1";
    glm::vec3 spherePosition = glm::vec3(0.0f, 4.0f, 0.0f); // Center of the sphere
    float sphereRadius = 0.2f; // Radius of the sphere
    glm::vec3 sphereColor = glm::vec3(1.0f, 0.5f, 0.0f); // Example color for the sphere


    // Create the sphere mesh
    Mesh* sphereMesh = object3D::CreateProjectile (sphereName, spherePosition, sphereRadius, sphereColor, true);
    AddMeshToList(sphereMesh);


    // Custom Turret RING

    std::string ringName = "turretring_1";
    glm::vec3 ringPosition = glm::vec3(1.0f, 1.2f, -0.25f); // Center of the hull
    float ringWidth = 2.0f;   // Width of the hull base
    float ringLength = 2.f;  // Length of the hull base
    float ringHeight = 0.8f;  // Height of the hull
    float rightSlope = 0.8f;  // Inward slope for the front side
    float backSlope = 0.3f;   // Inward slope for the back side
    float leftSlope = 0.2f;   // Inward slope for the left side
    float frontSlope = 0.3f;  // Inward slope for the right side
    glm::vec3 ringColor = glm::vec3(0.5f, 0.8f, 0.5f);

    // Create the hull mesh with custom slopes
    Mesh* ringMesh = object3D::CreateTurretRing(ringName, ringPosition, ringWidth, ringLength, ringHeight, frontSlope, backSlope, leftSlope, rightSlope, ringColor, true);
    AddMeshToList(ringMesh);

    */

    // TANK
 
    // Create tanks and add them to the list
    Tank tank1;
    tank1.name = "tank1";
    tank1.position = glm::vec3(0, 0, 0);
    tank1.rotation = 0.0f;
    tank1.direction = glm::vec3(1.0f, 0.0f, 0.0f);
    tank1.turretringroation = 0.f;
    tank1.balldirection = glm::vec3(.0f, 0.0f, 0.0f);


    // Create the tank and get its components
    std::vector<Mesh*> tankParts = object3D::CreateTank("tank1", tank1.position, glm::vec3(1, 0, 0), true);
    tank1.componentsList = tankParts;

    cout << "TANK INIT POS" << " " << tank1.position << endl;

    // Add the tank to the list of tanks
    tanksList.push_back(tank1);


    // PROJECTILES

    
    TankBall proj1;
    proj1.name = "proj1";
    proj1.position = glm::vec3(3, 5, 0);
    proj1.direction = glm::vec3(0, 0, 0);
    proj1.isActive = false;
    float sphereRadius = 0.2f; // Radius of the sphere
    glm::vec3 sphereColor = glm::vec3(0.0f, 0.0f, 0.0f); // Example color for the sphere



    // Create the sphere mesh
    Mesh* proj1mesh = object3D::CreateProjectile(proj1.name, proj1.position, sphereRadius, sphereColor, true);
    proj1.mesh = proj1mesh;

    tankballs.push_back(proj1);
    AddMeshToList(proj1mesh);
    

    // BUILDINGS

        // Building 1
    glm::vec3 building1Position = glm::vec3(8.0f, 0.0f, 8.0f); // Example position
    float building1Width = 4.0f;  // Width of the building
    float building1Length = 4.0f; // Length of the building
    float building1Height = 8.0f; // Height of the building
    float building1Slope = 0.0f;  // Slope for the top of the building
    glm::vec3 building1Color = glm::vec3(0.6f, 0.7f, 0.8f); // Color of the building

    Mesh* building1 = object3D::CreateHull("building1", building1Position, building1Width, building1Length, building1Height, building1Slope, building1Color, true);
    AddMeshToList(building1);

    // Building 2
    glm::vec3 building2Position = glm::vec3(-10.0f, 0.0f, -10.0f); // Different position
    float building2Width = 6.0f;  // Different width
    float building2Length = 6.0f; // Different length
    float building2Height = 12.0f; // Different height
    float building2Slope = 0.5f;  // Different slope
    glm::vec3 building2Color = glm::vec3(0.5f, 0.3f, 0.4f); // Different color

    Mesh* building2 = object3D::CreateHull("building2", building2Position, building2Width, building2Length, building2Height, building2Slope, building2Color, true);
    AddMeshToList(building2);


    // GROUND

        // Ground
    glm::vec3 groundPosition = glm::vec3(0.0f, 0.0f, 0.0f); // Centered at the origin
    float groundWidth = 50.0f;  // Large width to cover the area
    float groundLength = 50.f; // Large length to cover the area
    float groundHeight = 0.01f; // Very small height to make it flat
    float groundSlope = 0.0f;   // No slope needed for the ground
    glm::vec3 groundColor = glm::vec3(0.0f, 0.6f, 0.0f); // Green color

    Mesh* ground = object3D::CreateHull("ground", groundPosition, groundWidth, groundLength, groundHeight, groundSlope, groundColor, true);
    AddMeshToList(ground);



    EnemyTank enemytank1;
    enemytank1.name = "enemy1";
    enemytank1.rotation = 0.0f;
    enemytank1.direction = glm::vec3(1.0f, 0.0f, 0.0f);
    enemytank1.turretringroation = 0.f;
    enemytank1.balldirection = glm::vec3(.0f, 0.0f, 0.0f);
    // CREATE THE TWO ENEMIES
      // Enemy Tank 1
    glm::vec3 enemy1Position = glm::vec3(0.0f, 0.0f, -15.0f); // Example position for enemy tank 1
    enemytank1.position = enemy1Position;

    std::vector<Mesh*> enemyTank1parts = object3D::CreateTank("enemy1", enemy1Position, glm::vec3(1, 0, 0), true);
    enemytank1.componentsList = enemyTank1parts;
    enemytanksList.push_back(enemytank1);
    // Add enemyTank1 components to the scene...

;
    // Add enemyTank2 components to the scene..


    // Add the tank to the list of tanks
    

    EnemyTank enemytank2;
    enemytank2.name = "enemy2";
    enemytank2.rotation = 0.0f;
    enemytank2.turretringroation = 0.f;
    enemytank2.balldirection = glm::vec3(.0f, 0.0f, 0.0f);
    // Enemy Tank 2
    glm::vec3 enemy2Position = glm::vec3(.0f, 0.0f, 15.0f); // Example position for enemy tank 2
    enemytank2.position = enemy2Position;

    std::vector<Mesh*> enemyTank2parts = object3D::CreateTank("enemy2", enemy2Position, glm::vec3(1, 0, 0), true);
    enemytank2.componentsList = enemyTank2parts;
    enemytanksList.push_back(enemytank2);



    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    // doar schimba in ortografic pentru o si p
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{

    glm::mat4 modelMatrix = glm::mat4(1);

    RenderMesh(meshes["turret_1"], shaders["VertexColor"], modelMatrix);    // nu apare nimic

    // vertex normal, vertex color , simple

    RenderMesh(meshes["hull_1"], shaders["VertexColor"], modelMatrix);

    RenderMesh(meshes["TankTrackRight"], shaders["VertexColor"], modelMatrix);

    RenderMesh(meshes["TankTrackLeft"], shaders["VertexColor"], modelMatrix);


    glm::mat4 modelProjMatrix = glm::mat4(1);


    /*
    for (TankBall& projectile : tankballs) {
        if (projectile.isActive) {
            modelProjMatrix = glm::translate(modelProjMatrix, projectile.position);
            RenderMesh(meshes["proj2"], shaders["VertexColor"], modelProjMatrix);
        }
    }*/

    /*
    for (TankBall& projectile : tankballs) {
        //for (Mesh* mesh : tanknalls.componentsList) {
        RenderMesh(meshes["proj1"], shaders["VertexColor"], modelMatrix);
       //}
    }
    */

    RenderMesh(meshes["turretring_1"], shaders["VertexColor"], modelMatrix);


    // REDNER BUILDINGS AND GROUND

    RenderMesh(meshes["building1"], shaders["VertexColor"], modelMatrix);
    RenderMesh(meshes["building2"], shaders["VertexColor"], modelMatrix);
    RenderMesh(meshes["ground"], shaders["VertexColor"], modelMatrix);

    float tankRotationSpeed = 2.f;
    // Render the tanks
    for (const Tank& tank : tanksList) {

        glm::mat4 modelMatrix = glm::mat4(1);

        //  modelMatrix *= transform3D::Translate(tank.position.x, tank.position.y, tank.position.z);  //translate to origin, translate back after
        //  modelMatrix *= transform3D::Translate(tank.position.x - 1, tank.position.y - 0.3, tank.position.z + 0.25);  // this is center position of tank
        modelMatrix *= transform3D::Translate(tank.position.x, tank.position.y, tank.position.z);
        //  cout << " TANK ROTATION IS" << " " << (RADIANS(tank.rotation)) << endl;
        modelMatrix *= transform3D::RotateOY(tank.rotation);
        //  cout << "MODEL MATRIX IS" << " " << tank.position.z << endl;
        //  modelMatrix *= transform3D::Translate(-tank.position.x, -tank.position.y, -tank.position.z);


          // Iterate through the components of each tank and render them
        for (Mesh* mesh : tank.componentsList) {          //  nu merge sa diferentieze turretring_tank1
            std::string meshID(mesh->GetMeshID());
            if (meshID != "turretring_tank1" && meshID != "turret_tank1") {
               // cout << "MESHID" << " " << mesh->GetMeshID() << endl;
                RenderMesh(mesh, shaders["VertexColor"], modelMatrix); // Render the mesh using your rendering function
                // cout << "POSITION OF TANK" << " " << tank.position << endl;
            }
            else {
                // tank.turretringroation = 1.5f;
                // Redarea turelei cu rotația sa specifică
                glm::mat4 turretRingModelMatrix = modelMatrix;
                turretRingModelMatrix *= transform3D::RotateOY(tank.turretringroation); // Rotirea specifică turelei
               // tank.balldirection = glm::vec3(cos(tank.turretringroation), 0, -sin(tank.turretringroation));
                RenderMesh(mesh, shaders["VertexColor"], turretRingModelMatrix);
            }
        }
  
    }

    // RANDOM FOR ENEMY MOVE
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uniDirection(0, 3); // 0-W, 1-A, 2-S, 3-D

    counter += deltaTimeSeconds; // Counter for timing

    if (counter >= timeformove) {
        direction = uniDirection(rng);
        counter = 0;
    }
    
    float tankSpeed = 0.5f; // units per second
    // ...
    // In your update loop:
    float moveDistance = tankSpeed * deltaTimeSeconds;
    
   // timeformove = 5.f;


    for (EnemyTank& enemy : enemytanksList) {

        glm::mat4 modelMatrix = glm::mat4(1);
       
  
        if (direction != 4) {
            switch ((int)direction) {
            case 0: // W
                enemy.position += enemy.direction * moveDistance;
                break;
            case 1: // A
                enemy.rotation += moveDistance;
                //  enemy.position.x -= moveDistance;
                enemy.direction = glm::vec3(cos(enemy.rotation), 0, -sin(enemy.rotation));
                break;
            case 2: // S
                enemy.position -= enemy.direction * moveDistance;
                break;
            case 3: // D
                enemy.rotation -= moveDistance;
              //  enemy.position.x += moveDistance;
                enemy.direction = glm::vec3(cos(enemy.rotation), 0, -sin(enemy.rotation));
                break;
            }
        }

            modelMatrix *= transform3D::Translate(enemy.position.x, enemy.position.y, enemy.position.z);  //translate to origin, translate back after
            // modelMatrix *= transform3D::Translate(enemy.position.x - 1, enemy.position.y - 0.3, enemy.position.z + 0.25);  // this is center position of tank
            // modelMatrix *= transform3D::Translate(enemy.position.x, enemy.position.y, enemy.position.z);
            // cout << " TANK ROTATION IS" << " " << (RADIANS(tank.rotation)) << endl;
             modelMatrix *= transform3D::RotateOY(enemy.rotation);

            modelMatrix *= transform3D::Translate(-enemy.position.x, -enemy.position.y, -enemy.position.z);
            //  cout << "MODEL MATRIX IS" << " " << tank.position.z << endl;
            //modelMatrix *= transform3D::Translate(-tank.position.x, -tank.position.y, -tank.position.z);


            // Iterate through the components of each tank and render them
        for (Mesh* mesh : enemy.componentsList) {          //  nu merge sa diferentieze turretring_tank1
   
                RenderMesh(mesh, shaders["VertexColor"], modelMatrix);
            
        }

    }
   // ringMesh
    
    worldtimer += deltaTimeSeconds;

    if (worldtimer > 90.f) {
        cout << "GAME OVER, SAD FACE" << endl;
        exit(1);
    }
    lastSpawnTime += deltaTimeSeconds;
   
    
    // Update projectiles
    for (TankBall& projectile : tankballs) {
        if (lastSpawnTime >= timetolive)
            projectile.isActive = false;

        if (projectile.isActive) {
            projectile.position += projectile.direction * projectile.speed * deltaTimeSeconds;
            // Render the projectile
           // cout << "POSITION OF PROJECTILE" << " " << projectile.position << endl;
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, projectile.position);
            // Scale and orient the projectile as needed
            RenderMesh(meshes["proj2"], shaders["VertexColor"], modelMatrix); // Use the appropriate mesh and shader
        }
    }



    
    /*
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 7, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));

        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);     // asta e obiectul din mijloc
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(7, 0.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 7));
        RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);
    }

    // TODO(student): Draw more objects with different model matrices.
    // Attention! The `RenderMesh()` function overrides the usual
    // `RenderMesh()` that we've been using up until now. This new
    // function uses the view matrix from the camera that you just
    // implemented, and the local projection matrix.

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(4.f, 5.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(90.f), glm::vec3(1.f, 1.f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f, .5f, .3f));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(1.f, 3.f, 3.f));



        modelMatrix = glm::rotate(modelMatrix, RADIANS(180.f), glm::vec3(0.8f, 1.f, 0.5f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(.8f, 1.f, .9f));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }*/

    // Render the camera target. This is useful for understanding where
    // the rotation point is, when moving in third-person camera mode.
    if (renderCameraTarget)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
    }
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    //cout << "DELTATIME" << " " << deltaTime << endl;
    // pot sa pun for tank in tanks la toate si dupa sa verific daca am cheia nu stiu care -> un tank in tanks, 4 verificari taste
    
    // Check if the right mouse button is not pressed
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {

        for (Tank& tank : tanksList) {
            float tankSpeed = 2.0f;
            float cameraSpeed = 2.f;
            float tankRotationSpeed = 1.f;
            // Calculați vectorul de direcție bazat pe rotație
            float rotationRadians = tank.rotation;       // glm::radians(...)

            tank.direction = glm::vec3(cos(rotationRadians), 0, -sin(rotationRadians));

           // cout << "TANK DIR" << " " << tank.direction <<endl;

            if (window->KeyHold(GLFW_KEY_A)) {
                // Update the position of the tank(s) when "W" is pressed
                    
                    float rotationSpeed = tankRotationSpeed * deltaTime; // Rotation speed (in degrees per second)

                    // Actualizeaza unghiul total de rotatie
                   
                    tank.rotation += rotationSpeed; // Update the rotation angle     //glm::degrees pentru grade
                   //  cout << "ROTATION IS" << " " << tank.rotation << endl;
                   // tank.turretringroation = rotationSpeed;
                    tank.direction = glm::vec3(cos(tank.rotation), 0, -sin(tank.rotation));        // cos, sin era
                    //cout << "Direction IS" << " " << tank.direction << endl;
                    //tank.direction = glm::normalize(tank.direction); // Normalize the direction vector
                    camera->RotateThirdPerson_OY(cameraSpeed * deltaTime / 2.f);
                    camera->TranslateRight(deltaTime * cameraSpeed * 2);
            }

            if (window->KeyHold(GLFW_KEY_W)) {
                //glm::vec3 dir = glm::normalize(glm::vec3(tank.direction.x, 0, tank.direction.z));
                tank.position += tank.direction * deltaTime * tankSpeed;  
                camera->MoveForward(deltaTime * cameraSpeed);
                //cout << "W IS" << " " << tank.position << endl; 
            }

            if (window->KeyHold(GLFW_KEY_D)) {
                // Update the position of the tank(s) when "S" is pressed
                
                float rotationSpeed = tankRotationSpeed * deltaTime; // Rotation speed (in degrees per second)
                tank.rotation -= rotationSpeed; // Update the rotation angle
                tank.direction = glm::vec3(cos(tank.rotation), 0, -sin(tank.rotation));        // cos, sin era

                // vad functie in tema_camera.h
                camera->RotateThirdPerson_OY(-cameraSpeed * deltaTime / 2.f);
                camera->TranslateRight(-deltaTime * cameraSpeed * 2);
                // camera->MoveForward(-deltaTime * cameraSpeed);
                
            }

            // good moving back
            if (window->KeyHold(GLFW_KEY_S)) {
                // Update the position of the tank(s) when "D" is pressed
                    camera->MoveForward(-deltaTime * cameraSpeed);

                    tank.position -= deltaTime * tankSpeed * tank.direction; // Assuming the tank's left is along the negative X-axis
                
            }
        }

        // Handle other tank movement controls as needed
    }




    // move the camera only if MOUSE_RIGHT button is pressed
   /* if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the camera forward
            camera->TranslateForward(deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            camera->TranslateRight(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera backward
            camera->TranslateForward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
            camera->TranslateRight(deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO(student): Translate the camera downward
            camera->TranslateUpward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO(student): Translate the camera upward
            camera->TranslateUpward(deltaTime * cameraSpeed);
        }
    }*/
}



void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }
    // TODO(student): Switch projections

    for (Tank& tank : tanksList) {
        for (const TankBall& ball : tankballs) {
            if (key == GLFW_KEY_I) {

                TankBall newBall;
                glm::vec3 turrpos = glm::vec3(0, 1.3, 0);
              //  cout << "POSITION OF TANK" << " " << tank.position << endl;
                newBall.position = tank.position + turrpos;
                cout << "DIRECITON OF TANK" << " " << tank.direction << endl;
                newBall.direction = tank.balldirection + tank.direction - glm::vec3(1, 0 , 0);
                cout << "DIRECITON OF TANKBALL" << " " << tank.balldirection << endl;
                cout << "Ball direction" << " " << newBall.direction << endl;
                newBall.isActive = true;
                newBall.speed = 5.f;
                newBall.name = "proj2";
                lastSpawnTime = 0;
                float sphereRadius = 0.2f; // Radius of the sphere
                glm::vec3 sphereColor = glm::vec3(.0f, 0.f, 0.0f); // Example color for the sphere



                // Create the sphere mesh
                Mesh* proj2mesh = object3D::CreateProjectile(newBall.name, newBall.position, sphereRadius, sphereColor, true);
                newBall.mesh = proj2mesh;

                tankballs.push_back(newBall);
                AddMeshToList(proj2mesh);
            }
        }
    }

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }
    if (key == GLFW_KEY_O)
    {
        projectionType = false;
        projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
    }
    if (key == GLFW_KEY_P)
    {
        projectionType = true;
        projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, Z_NEAR, Z_FAR);
    }

}






void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // de facut viewMatrix si projectionMatrix ?
    // 
    // 
    // 
    //glm::vec3 mouseWorldPos = ConvertMouseTo3DCoords(mouseX, mouseY, viewMatrix, projectionMatrix,2.f);

    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
    float rotationAmountX = deltaX * turretRotationSensitivity;
    float rotationAmountY = deltaY * turretRotationSensitivity;
    // Add mouse move event
    for (Tank& tank : tanksList) {
        for (Mesh* mesh : tank.componentsList) {
            // cout << "Deltax" << " " << deltaX << endl;
            // cout << "Deltay" << " " << deltaY << endl;
            // if (deltaX >= 1 && deltaX < 5) {
            tank.turretringroation -= rotationAmountX;
            tank.balldirection = glm::vec3(cos(tank.turretringroation), 0, -sin(tank.turretringroation));
        }

            //}
           // if (deltaX <= -1 && deltaX > -5) {
              //  tank.turretringroation -= rotationAmountX;
              //  tank.balldirection = glm::vec3(cos(tank.turretringroation), 0, -sin(tank.turretringroation));
           // }
        }
    }


    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

       /* if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.

            camera->RotateFirstPerson_OX(sensivityOX * -deltaY);
            camera->RotateFirstPerson_OY(sensivityOY * -deltaX);

        }*/

        
            renderCameraTarget = true;
            // TODO(student): Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.

            camera->RotateThirdPerson_OX(sensivityOX * -deltaY);
            camera->RotateThirdPerson_OY(sensivityOY * -deltaX);

        
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)  // screen space -> world space, viewmatrix, projectionmatrix pentru 3d
{
    // Add mouse button press event

    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
        cout << mouseX << " " << mouseY << endl;
    }

}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
