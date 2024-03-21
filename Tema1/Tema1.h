#pragma once

#include "components/simple_scene.h"
#define MOVE_RATIO  (0.05f)
#define LIMIT   (0.15f)
//#define SQUARE_SIDE   (100.f)
//#define SQUARES_DISTANCE  (100.f)
#define SCALE_FACTOR_X	(0.5f)
#define SCALE_FACTOR_Y  (0.5f)
#define MAXSPWANENTETIES 10.f

namespace m1
{
    // Define the Rhombus class here
    class Tema1 : public gfxc::SimpleScene
    {
    public:

        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void Tema1::RenderGameScene(const glm::mat3& modelMatrix);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float translateX, translateY, translateX2, translateY2;
        float scaleX, scaleY;
        float angularStep;
        float angularStep1;
        float angularStep2;
        float angularStep3;
        float angularStep4;
        float angularStep5;
        float angularStep6;
        float angularStep7;
        float angularStep8;
        float angularStep9;

       // int spawnedEnemies;
        float lastEnemySpawnTime;
        float spawnDelay; // Spawn an enemy every 3 seconds, for example.

        // for star
        float starSpawnTime;
        float starSpawnInterval;

        float starfromrhombusSpawnTime20;
        float starfromrhombusSpawnInterval20;

        float starfromrhombusSpawnTime21;
        float starfromrhombusSpawnInterval21;

        float starfromrhombusSpawnTime22;
        float starfromrhombusSpawnInterval22;

        float starfromrhombusSpawnTime10;
        float starfromrhombusSpawnInterval10;

        float starfromrhombusSpawnTime11;
        float starfromrhombusSpawnInterval11;

        float starfromrhombusSpawnTime12;
        float starfromrhombusSpawnInterval12;

        float starfromrhombusSpawnTime00;
        float starfromrhombusSpawnInterval00;

        float starfromrhombusSpawnTime01;
        float starfromrhombusSpawnInterval01;

        float starfromrhombusSpawnTime02;
        float starfromrhombusSpawnInterval02;

        float outerHexagonRadius; // Outer hexagon radius (adjust as needed)
        float innerHexagonRadius; // Inner hexagon radius (adjust as needed)

        std::vector<int> lineYCoordinates;
        glm::vec3 randomOuterColor;
        int lineY;

        // TODO(student): If you need any other class variables, define them here.


        glm::vec2 dragPos;
        bool isSelected = false;
        std::string selectedMeshName = "drag_1";

        std::string placedRhombs[3][3] = {
            {"", "", ""},
            {"", "", ""},
            {"", "", ""}
        };

        glm::vec3 holdcolours[3][3] = {
          {glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)},
          {glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)},
          {glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)}
        };


        std::vector<std::string> enemies = {
        "enemy1", // orange
        "enemy2", // blue
        "enemy3", // yellow
        "enemy4"  // purple
        };

        int en1l1;
        int en1l2;
        int en1l3;
        int en2l1;
        int en2l2;
        int en2l3;
        int en3l1;
        int en3l2;
        int en3l3;
        int en4l1;
        int en4l2;
        int en4l3;

    };
}   // namespace m1
