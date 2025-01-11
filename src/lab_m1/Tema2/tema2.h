#pragma once

#include <vector>
#include "components/simple_scene.h"
#include "components/camera.h"

namespace m1
{
    struct Tree {
        glm::vec3 position;
        float scale;
    };
    struct House {
        glm::vec3 position;
        float scale;
    };

   
    struct Projectile {
        glm::vec3 position;
        glm::vec3 velocity;
    };

   
    struct EnemyDrone {
        glm::vec3 position;
        float scale;
    };

    class tema2 : public gfxc::SimpleScene
    {
    public:
        tema2();
        ~tema2();

        void Init() override;

        
        Mesh* CreateMesh(const char* name,
            const std::vector<VertexFormat>& vertices,
            const std::vector<unsigned int>& indices);

        Mesh* CreateSphere(const std::string& name, float radius, int rings, int sectors,
            const glm::vec3& color);

        Mesh* CreateCylinder(const std::string& name, float height, float radius,
            int segments, const glm::vec3& color);

        Mesh* CreateCone(const std::string& name, float height, float radius,
            int segments, const glm::vec3& color);

        Mesh* CreateTriangularPrism(const std::string& name,
            const glm::vec3& color);

        float GetTerrainHeight(float x, float z);

        GLuint viewMatrixLocation;

        
        void CreateEnemyDroneMeshes();
        
        void RenderEnemyDrone(const glm::vec3& pos, float scale, float propAngle);

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader,
            const glm::mat4& modelMatrix);

        void RenderSimpleMeshMinimap(Mesh* mesh, Shader* shader,
            const glm::mat4& modelMatrix, gfxc::Camera* miniCam);

       
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        bool CheckCollisionWithGround(glm::vec3& newPos, float radius);
        bool CheckCollisionWithTrees(const glm::vec3& newPos, float radius);
        bool CheckCollisionWithHouses(const glm::vec3& newPos, float radius);

       
        void UpdateProjectiles(float deltaTimeSeconds);
        void RenderProjectiles();

    private:
        glm::vec3 dronePos;
        float droneYaw;
        float propellerAngle;
        float movementSpeed;
        float rotationSpeed;

        float cameraDistance;
        float cameraHeight;

        std::vector<Tree> trees;
        std::vector<House> houses;

    public:
        gfxc::Camera* miniMapCamera;
        float miniMapSize;

        
        std::vector<Projectile> projectiles;

       
        std::vector<EnemyDrone> enemies;

        float gravity;
    };
}
