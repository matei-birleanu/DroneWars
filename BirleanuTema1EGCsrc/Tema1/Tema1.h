#pragma once

#include "components/simple_scene.h"

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void RenderLine(glm::vec3 start, glm::vec3 end, glm::vec3 color);
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        Mesh* terrainMesh;

    protected:
        struct TerrainPoint {
            float x, y;
        };
        struct Tank {
            float positionX;
            float positionY;
            float angle;
            float barrelRotationAngle;
            glm::vec3 baseColor;
            glm::vec3 bodyColor;
            glm::vec3 barrelColor;
            std::string tankBaseMeshName;
            std::string tankBodyMeshName;
            std::string tankTurretMeshName;
            std::string tankBarrelMeshName;

            float health;
            float maxHealth;
            float collisionRadius;
            float healthBarWidth;
            float healthBarHeight;
            glm::vec3 healthBarColor;
            glm::vec3 healthBarBorderColor;
            bool active = true;
            int ownerTankId;
        };
        struct Projectile {
            glm::vec3 position;
            glm::vec3 previousPosition;
            glm::vec3 velocity;
            float radius;
            glm::vec3 color;
            bool active;
            int ownerTankId;
        };

        enum class CollisionType {
            None,
            Terrain,
            Tank1,
            Tank2
        };

        std::vector<TerrainPoint> heightMap;
        float terrainWidth;
        float terrainHeight;
        float timeAccumulator;
        float tankSpeed;

        Tank tank1;
        Tank tank2;

        std::vector<Projectile> projectiles;
        float projectileInitialSpeed;

        void GenerateHeightMap();
        float TerrainFunction(float x);
        void RenderTerrain();
        void CreateTerrainMesh();
        void RenderTank(Tank& tank);
        float CalculateTankYPosition(float x);
        void FindTerrainSegment(float x, TerrainPoint& A, TerrainPoint& B);
        float CalculateTankAngle(float x);
        glm::vec3 GetBarrelEndPosition(Tank& tank);
        void DeformTerrain(float x, float y);
        void TerrainSliding(float deltaTime);

        bool CheckCollisionAlongPath(Projectile& projectile, glm::vec3 startPos, glm::vec3 endPos);
        bool CheckCircleCollision(glm::vec2 center1, float radius1, glm::vec2 center2, float radius2);
        glm::vec2 collisionPoint;
        CollisionType collisionType = CollisionType::None;

        void RenderHealthBar(Tank& tank);
        void RenderTrajectory(Tank& tank);

        glm::mat3 modelMatrix;

        bool isNight;
        glm::vec3 terrainColor;
        glm::vec3 sunColor;
        glm::vec3 moonColor;
    };
}
