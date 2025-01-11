#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;

Tema1::Tema1()
{
}

Mesh* lineMesh = nullptr;

Tema1::~Tema1()
{
    if (terrainMesh != nullptr) {
        delete terrainMesh;
        terrainMesh = nullptr;
    }
    delete lineMesh;
}

void Tema1::Init()
{
    lineMesh = new Mesh("trajectoryLine");
    lineMesh->SetDrawMode(GL_LINES);
    AddMeshToList(lineMesh);

    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    terrainWidth = (float)resolution.x;
    terrainHeight = (float)resolution.y;
    timeAccumulator = 0;
    tankSpeed = 100.0f;

    tank1.positionX = 110.0f;
    tank1.positionY = 0.0f;
    tank1.angle = 0.0f;
    tank1.barrelRotationAngle = glm::radians(45.0f);
    tank1.baseColor = glm::vec3(0.55f, 0.27f, 0.07f);
    tank1.bodyColor = glm::vec3(0.72f, 0.53f, 0.04f);
    tank1.barrelColor = glm::vec3(0.0f, 0.0f, 0.0f);
    tank1.tankBaseMeshName = "tankBase";
    tank1.tankBodyMeshName = "tankBody";
    tank1.tankTurretMeshName = "tankTurret";
    tank1.tankBarrelMeshName = "tankBarrel";
    tank1.health = 20.0f;
    tank1.maxHealth = 20.0f;
    tank1.collisionRadius = 50.0f;
    tank1.healthBarWidth = 60.0f;
    tank1.healthBarHeight = 5.0f;
    tank1.healthBarColor = glm::vec3(1.0f, 1.0f, 1.0f);
    tank1.healthBarBorderColor = glm::vec3(1.0f, 1.0f, 1.0f);
    tank1.ownerTankId = 1;

    tank2.positionX = terrainWidth - 150.0f;
    tank2.positionY = 0.0f;
    tank2.angle = 0.0f;
    tank2.barrelRotationAngle = glm::radians(135.0f);
    tank2.baseColor = glm::vec3(1.0f, 0.0f, 0.0f);
    tank2.bodyColor = glm::vec3(0.0f, 0.0f, 1.0f);
    tank2.barrelColor = glm::vec3(0.0f, 0.0f, 1.0f);
    tank2.tankBaseMeshName = "tankBase2";
    tank2.tankBodyMeshName = "tankBody2";
    tank2.tankTurretMeshName = "tankTurret2";
    tank2.tankBarrelMeshName = "tankBarrel2";
    tank2.health = 20.0f;
    tank2.maxHealth = 20.0f;
    tank2.collisionRadius = 50.0f;
    tank2.healthBarWidth = 60.0f;
    tank2.healthBarHeight = 5.0f;
    tank2.healthBarColor = glm::vec3(1.0f, 1.0f, 1.0f);
    tank2.healthBarBorderColor = glm::vec3(1.0f, 1.0f, 1.0f);
    tank2.ownerTankId = 2;

    isNight = false;

    sunColor = glm::vec3(1.0f, 1.0f, 0.0f);
    Mesh* sun = object2D::CreateCircle("sun", glm::vec3(1000.0f, 600.0f, 0.0f), 50.0f, sunColor, true);
    AddMeshToList(sun);
    terrainColor = glm::vec3(0.0f, 0.5f, 0.0f);

    moonColor = glm::vec3(0.9f, 0.9f, 0.9f);
    Mesh* moon = object2D::CreateCircle("moon", glm::vec3(1000.0f, 600.0f, 0.0f), 50.0f, moonColor, true);
    AddMeshToList(moon);

    glm::vec3 cloudColor = glm::vec3(1.0f, 1.0f, 1.0f);
    Mesh* cloud1_part1 = object2D::CreateCircle("cloud1_part1", glm::vec3(150.0f, 550.0f, 0.0f), 20.0f, cloudColor, true);
    Mesh* cloud1_part2 = object2D::CreateCircle("cloud1_part2", glm::vec3(170.0f, 550.0f, 0.0f), 25.0f, cloudColor, true);
    Mesh* cloud1_part3 = object2D::CreateCircle("cloud1_part3", glm::vec3(190.0f, 550.0f, 0.0f), 20.0f, cloudColor, true);
    AddMeshToList(cloud1_part1);
    AddMeshToList(cloud1_part2);
    AddMeshToList(cloud1_part3);

    Mesh* cloud2_part1 = object2D::CreateCircle("cloud2_part1", glm::vec3(500.0f, 500.0f, 0.0f), 15.0f, cloudColor, true);
    Mesh* cloud2_part2 = object2D::CreateCircle("cloud2_part2", glm::vec3(520.0f, 500.0f, 0.0f), 20.0f, cloudColor, true);
    Mesh* cloud2_part3 = object2D::CreateCircle("cloud2_part3", glm::vec3(540.0f, 500.0f, 0.0f), 15.0f, cloudColor, true);
    AddMeshToList(cloud2_part1);
    AddMeshToList(cloud2_part2);
    AddMeshToList(cloud2_part3);

    Mesh* cloud3 = object2D::CreateCircle("cloud3", glm::vec3(700.0f, 350.0f, 0.0f), 30.0f, cloudColor, true);
    AddMeshToList(cloud3);

    Mesh* cloud4_part1 = object2D::CreateCircle("cloud4_part1", glm::vec3(250.0f, 600.0f, 0.0f), 18.0f, cloudColor, true);
    Mesh* cloud4_part2 = object2D::CreateCircle("cloud4_part2", glm::vec3(270.0f, 600.0f, 0.0f), 22.0f, cloudColor, true);
    Mesh* cloud4_part3 = object2D::CreateCircle("cloud4_part3", glm::vec3(290.0f, 600.0f, 0.0f), 18.0f, cloudColor, true);
    AddMeshToList(cloud4_part1);
    AddMeshToList(cloud4_part2);
    AddMeshToList(cloud4_part3);

    Mesh* cloud5_part1 = object2D::CreateCircle("cloud5_part1", glm::vec3(800.0f, 550.0f, 0.0f), 25.0f, cloudColor, true);
    Mesh* cloud5_part2 = object2D::CreateCircle("cloud5_part2", glm::vec3(820.0f, 550.0f, 0.0f), 30.0f, cloudColor, true);
    Mesh* cloud5_part3 = object2D::CreateCircle("cloud5_part3", glm::vec3(840.0f, 550.0f, 0.0f), 25.0f, cloudColor, true);
    AddMeshToList(cloud5_part1);
    AddMeshToList(cloud5_part2);
    AddMeshToList(cloud5_part3);

    Mesh* tankBase = object2D::CreateInvertedTrapezoid("tankBase", glm::vec3(25.0f, 0.0f, 0.0f), 80.0f, 50.0f, 12.5f, tank1.baseColor, true);
    AddMeshToList(tankBase);

    Mesh* tankBody = object2D::CreateTrapezoidWithWideBase("tankBody", glm::vec3(-10.0f, 7.5f, 10.0f), 120.0f, 70.0f, 22.5f, tank1.bodyColor, true);
    AddMeshToList(tankBody);

    Mesh* tankTurret = object2D::CreateSemicircle("tankTurret", glm::vec3(-30.0f, 20.0f, 0.0f), 20.0f, 20, tank1.bodyColor, true);
    AddMeshToList(tankTurret);

    Mesh* tankBarrel = object2D::CreateRectangle("tankBarrel", glm::vec3(-5.0f, 20.0f, 0.0f), 30.0f, 5.0f, tank1.barrelColor, true);
    AddMeshToList(tankBarrel);

    Mesh* tankBase2 = object2D::CreateInvertedTrapezoid("tankBase2", glm::vec3(25.0f, 0.0f, 0.0f), 80.0f, 50.0f, 12.5f, tank2.baseColor, true);
    AddMeshToList(tankBase2);

    Mesh* tankBody2 = object2D::CreateTrapezoidWithWideBase("tankBody2", glm::vec3(-10.0f, 7.5f, 10.0f), 120.0f, 70.0f, 22.5f, tank2.bodyColor, true);
    AddMeshToList(tankBody2);

    Mesh* tankTurret2 = object2D::CreateSemicircle("tankTurret2", glm::vec3(-30.0f, 20.0f, 0.0f), 20.0f, 20, tank2.bodyColor, true);
    AddMeshToList(tankTurret2);

    Mesh* tankBarrel2 = object2D::CreateRectangle("tankBarrel2", glm::vec3(-5.0f, 20.0f, 0.0f), 30.0f, 5.0f, tank2.barrelColor, true);
    AddMeshToList(tankBarrel2);

    Mesh* projectileMesh = object2D::CreateCircle("projectile", glm::vec3(0, 0, 0), 5.0f, glm::vec3(1, 1, 0), true);
    AddMeshToList(projectileMesh);

    Mesh* healthBarBorder = object2D::CreateRectangle("healthBarBorder", glm::vec3(0, 0, 0), 60.0f, 5.0f, glm::vec3(1, 1, 1), false);
    AddMeshToList(healthBarBorder);

    Mesh* healthBarFill = object2D::CreateRectangle("healthBarFill", glm::vec3(0, 0, 0), 60.0f, 5.0f, glm::vec3(1, 1, 1), true);
    AddMeshToList(healthBarFill);

    projectileInitialSpeed = 700.0f;

    GenerateHeightMap();
    CreateTerrainMesh();
}

float Tema1::TerrainFunction(float x)
{
    float baseHeight = terrainHeight * 0.4f;
    float A1 = 50.0f, A2 = 30.0f, A3 = 20.0f;
    float sigma1 = 0.01f, sigma2 = 0.03f, sigma3 = 0.02f;

    return baseHeight +
        A1 * sin(sigma1 * x) +
        A2 * sin(sigma2 * x) +
        A3 * sin(sigma3 * x);
}

float Tema1::CalculateTankAngle(float x)
{
    TerrainPoint A, B;
    FindTerrainSegment(x, A, B);

    glm::vec2 V = glm::vec2(B.x - A.x, B.y - A.y);
    float angle = atan2(V.y, V.x);
    return angle;
}

void Tema1::RenderTank(Tank& tank)
{
    if (!tank.active) return;

    glm::mat3 modelMatrix = glm::mat3(1.0f);
    modelMatrix *= transform2D::Translate(tank.positionX, tank.positionY);
    modelMatrix *= transform2D::Rotate(tank.angle);

    glm::mat3 baseMatrix = modelMatrix;
    baseMatrix *= transform2D::Translate(-40.0f, 0);
    RenderMesh2D(meshes[tank.tankBaseMeshName], shaders["VertexColor"], baseMatrix);

    glm::mat3 bodyMatrix = modelMatrix;
    bodyMatrix *= transform2D::Translate(-25.0f, 5.0f);
    RenderMesh2D(meshes[tank.tankBodyMeshName], shaders["VertexColor"], bodyMatrix);

    glm::mat3 turretMatrix = modelMatrix;
    turretMatrix *= transform2D::Translate(-5.0f, 50.0f);
    turretMatrix *= transform2D::Rotate(-M_PI);
    RenderMesh2D(meshes[tank.tankTurretMeshName], shaders["VertexColor"], turretMatrix);

    glm::mat3 barrelMatrix = turretMatrix;
    barrelMatrix *= transform2D::Translate(-30.0f, 9.0f);
    barrelMatrix *= transform2D::Rotate(tank.barrelRotationAngle);
    barrelMatrix *= transform2D::Translate(-20.0f, -25.0f);
    RenderMesh2D(meshes[tank.tankBarrelMeshName], shaders["VertexColor"], barrelMatrix);

    RenderHealthBar(tank);

    RenderTrajectory(tank);
}

void Tema1::RenderHealthBar(Tank& tank)
{
    float healthPercent = tank.health / tank.maxHealth;

    glm::mat3 modelMatrix = glm::mat3(1.0f);
    modelMatrix *= transform2D::Translate(tank.positionX, tank.positionY + 80.0f);

    glm::mat3 borderMatrix = modelMatrix;
    borderMatrix *= transform2D::Translate(-tank.healthBarWidth / 2, 0);
    RenderMesh2D(meshes["healthBarBorder"], shaders["VertexColor"], borderMatrix);

    glm::mat3 fillMatrix = modelMatrix;
    fillMatrix *= transform2D::Translate(-tank.healthBarWidth / 2, 0);
    fillMatrix *= transform2D::Scale(healthPercent, 1.0f);
    RenderMesh2D(meshes["healthBarFill"], shaders["VertexColor"], fillMatrix);
}

void Tema1::GenerateHeightMap() {
    heightMap.clear();
    float spacing = 1.0f;
    int numPoints = (int)(terrainWidth / spacing) + 1;

    for (int i = 0; i < numPoints; i++) {
        TerrainPoint point;
        point.x = i * spacing;
        point.y = TerrainFunction(point.x);
        heightMap.push_back(point);
    }
}

void Tema1::CreateTerrainMesh() {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    for (size_t i = 0; i < heightMap.size(); i++) {
        vertices.emplace_back(glm::vec3(heightMap[i].x, heightMap[i].y, 0), terrainColor);
        vertices.emplace_back(glm::vec3(heightMap[i].x, 0.0f, 0), terrainColor);
    }

    for (unsigned int i = 0; i < (heightMap.size() - 1) * 2; i += 2) {
        indices.push_back(i);
        indices.push_back(i + 2);
        indices.push_back(i + 1);

        indices.push_back(i + 1);
        indices.push_back(i + 2);
        indices.push_back(i + 3);
    }

    terrainMesh = new Mesh("terrain");
    terrainMesh->InitFromData(vertices, indices);
    terrainMesh->SetDrawMode(GL_TRIANGLES);

    AddMeshToList(terrainMesh);
}

void Tema1::RenderTerrain() {
    modelMatrix = glm::mat3(1.0f);
    RenderMesh2D(terrainMesh, shaders["VertexColor"], modelMatrix);
}

void Tema1::FrameStart()
{
    if (isNight)
    {
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        terrainColor = glm::vec3(0.2f, 0.2f, 0.2f);
    }
    else
    {
        glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
        terrainColor = glm::vec3(0.0f, 0.5f, 0.0f);
    }


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::FindTerrainSegment(float x, TerrainPoint& A, TerrainPoint& B)
{
    if (x <= heightMap.front().x) {
        A = heightMap.front();
        B = heightMap[1];
        return;
    }
    if (x >= heightMap.back().x) {
        A = heightMap[heightMap.size() - 2];
        B = heightMap.back();
        return;
    }
    for (size_t i = 0; i < heightMap.size() - 1; ++i) {
        if (x >= heightMap[i].x && x <= heightMap[i + 1].x) {
            A = heightMap[i];
            B = heightMap[i + 1];
            return;
        }
    }
}

glm::vec3 Tema1::GetBarrelEndPosition(Tank& tank)
{
    glm::mat3 modelMatrix = glm::mat3(1.0f);
    modelMatrix *= transform2D::Translate(tank.positionX, tank.positionY);
    modelMatrix *= transform2D::Rotate(tank.angle);

    glm::mat3 turretMatrix = modelMatrix;
    turretMatrix *= transform2D::Translate(-5.0f, 50.0f);
    turretMatrix *= transform2D::Rotate(-M_PI);

    glm::mat3 barrelMatrix = turretMatrix;
    barrelMatrix *= transform2D::Translate(-30.0f, 9.0f);
    barrelMatrix *= transform2D::Rotate(tank.barrelRotationAngle);
    barrelMatrix *= transform2D::Translate(25.0f, 0.0f);

    glm::vec3 barrelTipLocal = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 barrelTipWorld = barrelMatrix * barrelTipLocal;

    return barrelTipWorld;
}

float Tema1::CalculateTankYPosition(float x)
{
    TerrainPoint A, B;
    FindTerrainSegment(x, A, B);

    float t = (x - A.x) / (B.x - A.x);
    float y = A.y * (1 - t) + B.y * t;
    return y;
}

void Tema1::DeformTerrain(float x, float y)
{
    float radius = 30.0f;
    float sigma = radius / 3.0f;
    float explosionDepth = 50.0f;

    for (size_t i = 0; i < heightMap.size(); ++i)
    {
        float dx = heightMap[i].x - x;
        float distance = abs(dx);

        if (distance <= radius)
        {
            float deltaH = explosionDepth * exp(-(dx * dx) / (2 * sigma * sigma));
            heightMap[i].y -= deltaH;

            if (heightMap[i].y < 0)
                heightMap[i].y = 0;
        }
    }
}

bool Tema1::CheckCollisionAlongPath(Projectile& projectile, glm::vec3 startPos, glm::vec3 endPos)
{
    float dx = endPos.x - startPos.x;
    float dy = endPos.y - startPos.y;
    int steps = (int)(glm::length(glm::vec2(dx, dy)) / 1.0f);

    for (int i = 0; i <= steps; ++i)
    {
        float t = (float)i / (float)steps;
        float x = startPos.x + dx * t;
        float y = startPos.y + dy * t;

        TerrainPoint A, B;
        FindTerrainSegment(x, A, B);

        float terrainT = (x - A.x) / (B.x - A.x);
        float terrainY = A.y * (1 - terrainT) + B.y * terrainT;

        float distance = y - terrainY;

        if (distance <= projectile.radius && distance >= 0)
        {
            collisionPoint = glm::vec2(x, y);
            collisionType = CollisionType::Terrain;
            return true;
        }

        if (projectile.ownerTankId == 1 && tank2.active)
        {
            if (CheckCircleCollision(glm::vec2(x, y), projectile.radius, glm::vec2(tank2.positionX, tank2.positionY + 40.0f), tank2.collisionRadius))
            {
                collisionPoint = glm::vec2(x, y);
                collisionType = CollisionType::Tank2;
                return true;
            }
        }
        else if (projectile.ownerTankId == 2 && tank1.active)
        {
            if (CheckCircleCollision(glm::vec2(x, y), projectile.radius, glm::vec2(tank1.positionX, tank1.positionY + 40.0f), tank1.collisionRadius))
            {
                collisionPoint = glm::vec2(x, y);
                collisionType = CollisionType::Tank1;
                return true;
            }
        }
    }

    return false;
}

bool Tema1::CheckCircleCollision(glm::vec2 center1, float radius1, glm::vec2 center2, float radius2)
{
    float distance = glm::length(center1 - center2);
    return distance <= (radius1 + radius2);
}

void Tema1::TerrainSliding(float deltaTime)
{
    float threshold = 5.0f;
    float k = 20.0f;

    bool terrainChanged = false;

    for (size_t i = 0; i < heightMap.size() - 1; ++i)
    {
        float h1 = heightMap[i].y;
        float h2 = heightMap[i + 1].y;
        float d = fabs(h2 - h1);

        if (d > threshold)
        {
            float epsilon = k * deltaTime;

            if (h1 > h2)
            {
                heightMap[i].y -= epsilon;
                heightMap[i + 1].y += epsilon;
            }
            else
            {
                heightMap[i].y += epsilon;
                heightMap[i + 1].y -= epsilon;
            }
            terrainChanged = true;
        }
    }

    if (terrainChanged)
    {
        CreateTerrainMesh();
    }
}

void Tema1::Update(float deltaTimeSeconds)
{
    float gravity = 981.0f;

    float movementSpeed = 100.0f;

    if (tank1.active)
    {
        if (window->KeyHold(GLFW_KEY_A)) {
            tank1.positionX -= movementSpeed * deltaTimeSeconds;
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            tank1.positionX += movementSpeed * deltaTimeSeconds;
        }
        tank1.positionX = glm::clamp(tank1.positionX, 0.0f, terrainWidth);
    }

    if (tank2.active)
    {
        if (window->KeyHold(GLFW_KEY_LEFT)) {
            tank2.positionX -= movementSpeed * deltaTimeSeconds;
        }
        if (window->KeyHold(GLFW_KEY_RIGHT)) {
            tank2.positionX += movementSpeed * deltaTimeSeconds;
        }
        tank2.positionX = glm::clamp(tank2.positionX, 0.0f, terrainWidth);
    }

    std::vector<Projectile>::iterator it = projectiles.begin();
    while (it != projectiles.end())
    {
        Projectile& projectile = *it;
        if (projectile.active)
        {
            glm::vec3 prevPosition = projectile.position;

            projectile.position += projectile.velocity * deltaTimeSeconds;

            projectile.velocity.y -= gravity * deltaTimeSeconds;

            if (CheckCollisionAlongPath(projectile, prevPosition, projectile.position))
            {
                projectile.active = false;

                if (collisionType == CollisionType::Terrain)
                {
                    DeformTerrain(collisionPoint.x, collisionPoint.y);

                    CreateTerrainMesh();
                }
                else if (collisionType == CollisionType::Tank1)
                {
                    tank1.health -= 1.0f;
                    if (tank1.health <= 0.0f)
                    {
                        tank1.active = false;
                    }
                }
                else if (collisionType == CollisionType::Tank2)
                {
                    tank2.health -= 1.0f;
                    if (tank2.health <= 0.0f)
                    {
                        tank2.active = false;
                    }
                }

                it = projectiles.erase(it);
                continue;
            }

            glm::mat3 modelMatrix = glm::mat3(1.0f);
            modelMatrix *= transform2D::Translate(projectile.position.x, projectile.position.y);
            RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);

            ++it;
        }
        else
        {
            it = projectiles.erase(it);
        }
    }

    TerrainSliding(deltaTimeSeconds);

    if (tank1.active)
    {
        tank1.positionY = CalculateTankYPosition(tank1.positionX);
        tank1.angle = CalculateTankAngle(tank1.positionX);
    }

    if (tank2.active)
    {
        tank2.positionY = CalculateTankYPosition(tank2.positionX);
        tank2.angle = CalculateTankAngle(tank2.positionX);
    }

    RenderMesh2D(terrainMesh, shaders["VertexColor"], glm::mat3(1.0f));

    if (isNight)
    {
        RenderMesh2D(meshes["moon"], shaders["VertexColor"], glm::mat3(1));
    }
    else
    {
        RenderMesh2D(meshes["sun"], shaders["VertexColor"], glm::mat3(1));
    }

    if (!isNight)
    {
        RenderMesh2D(meshes["cloud1_part1"], shaders["VertexColor"], glm::mat3(1));
        RenderMesh2D(meshes["cloud1_part2"], shaders["VertexColor"], glm::mat3(1));
        RenderMesh2D(meshes["cloud1_part3"], shaders["VertexColor"], glm::mat3(1));

        RenderMesh2D(meshes["cloud2_part1"], shaders["VertexColor"], glm::mat3(1));
        RenderMesh2D(meshes["cloud2_part2"], shaders["VertexColor"], glm::mat3(1));
        RenderMesh2D(meshes["cloud2_part3"], shaders["VertexColor"], glm::mat3(1));

        RenderMesh2D(meshes["cloud4_part1"], shaders["VertexColor"], glm::mat3(1));
        RenderMesh2D(meshes["cloud4_part2"], shaders["VertexColor"], glm::mat3(1));
        RenderMesh2D(meshes["cloud4_part3"], shaders["VertexColor"], glm::mat3(1));

        RenderMesh2D(meshes["cloud5_part1"], shaders["VertexColor"], glm::mat3(1));
        RenderMesh2D(meshes["cloud5_part2"], shaders["VertexColor"], glm::mat3(1));
        RenderMesh2D(meshes["cloud5_part3"], shaders["VertexColor"], glm::mat3(1));
    }

    RenderTank(tank1);
    RenderTank(tank2);
}

void Tema1::RenderTrajectory(Tank& tank)
{
    float gravity = 981.0f;
    float timeStep = 0.05f;
    int maxSteps = 1000;

    glm::vec3 position = GetBarrelEndPosition(tank);
    float totalAngle = tank.angle + tank.barrelRotationAngle;
    glm::vec3 velocity = glm::vec3(cos(totalAngle), sin(totalAngle), 0.0f) * projectileInitialSpeed;

    std::vector<VertexFormat> lineVertices;

    glm::vec3 prevPosition = position;

    for (int i = 0; i < maxSteps; ++i)
    {
        position += velocity * timeStep;

        velocity.y -= gravity * timeStep;

        if (position.x < 0 || position.x > terrainWidth || position.y < 0 || position.y > terrainHeight)
            break;

        TerrainPoint A, B;
        FindTerrainSegment(position.x, A, B);

        float terrainT = (position.x - A.x) / (B.x - A.x);
        float terrainY = A.y * (1 - terrainT) + B.y * terrainT;

        if (position.y <= terrainY)
        {
            float dy = position.y - prevPosition.y;
            if (dy != 0.0f)
            {
                float t = (terrainY - prevPosition.y) / dy;
                if (t >= 0.0f && t <= 1.0f)
                {
                    glm::vec3 intersectionPoint = prevPosition + (position - prevPosition) * t;

                    lineVertices.push_back(VertexFormat(prevPosition, glm::vec3(1, 1, 1)));
                    lineVertices.push_back(VertexFormat(intersectionPoint, glm::vec3(1, 1, 1)));
                }
            }

            break;
        }

        Tank* enemyTank = nullptr;
        if (tank.ownerTankId == 1 && tank2.active)
            enemyTank = &tank2;
        else if (tank.ownerTankId == 2 && tank1.active)
            enemyTank = &tank1;

        if (enemyTank)
        {
            if (CheckCircleCollision(glm::vec2(position.x, position.y), 5.0f, glm::vec2(enemyTank->positionX, enemyTank->positionY + 40.0f), enemyTank->collisionRadius))
            {
                float t = -1.0f;
                glm::vec2 deltaPos = glm::vec2(prevPosition.x, prevPosition.y) - glm::vec2(enemyTank->positionX, enemyTank->positionY + 40.0f);
                glm::vec2 deltaVel = glm::vec2(position.x - prevPosition.x, position.y - prevPosition.y);

                float a = glm::dot(deltaVel, deltaVel);
                float b = 2.0f * glm::dot(deltaPos, deltaVel);
                float c = glm::dot(deltaPos, deltaPos) - (enemyTank->collisionRadius + 5.0f) * (enemyTank->collisionRadius + 5.0f);

                float discriminant = b * b - 4 * a * c;

                if (discriminant >= 0)
                {
                    float sqrtDisc = sqrt(discriminant);
                    float t1 = (-b - sqrtDisc) / (2 * a);
                    float t2 = (-b + sqrtDisc) / (2 * a);
                    if (t1 >= 0.0f && t1 <= 1.0f)
                        t = t1;
                    else if (t2 >= 0.0f && t2 <= 1.0f)
                        t = t2;
                }

                if (t >= 0.0f && t <= 1.0f)
                {
                    glm::vec3 intersectionPoint = prevPosition + (position - prevPosition) * t;

                    lineVertices.push_back(VertexFormat(prevPosition, glm::vec3(1, 1, 1)));
                    lineVertices.push_back(VertexFormat(intersectionPoint, glm::vec3(1, 1, 1)));
                }

                break;
            }
        }

        lineVertices.push_back(VertexFormat(prevPosition, glm::vec3(1, 1, 1)));
        lineVertices.push_back(VertexFormat(position, glm::vec3(1, 1, 1)));

        prevPosition = position;
    }

    if (lineVertices.size() > 0)
    {
        Mesh* trajectoryMesh = new Mesh("trajectory");
        trajectoryMesh->SetDrawMode(GL_LINES);

        std::vector<unsigned int> indices(lineVertices.size());
        for (unsigned int i = 0; i < indices.size(); ++i)
        {
            indices[i] = i;
        }

        trajectoryMesh->InitFromData(lineVertices, indices);
        RenderMesh2D(trajectoryMesh, shaders["VertexColor"], glm::mat3(1.0f));

        //delete trajectoryMesh;
    }
}

void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    float barrelRotationSpeed = glm::radians(90.0f);

    if (window->KeyHold(GLFW_KEY_W))
    {
        tank1.barrelRotationAngle += barrelRotationSpeed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_S))
    {
        tank1.barrelRotationAngle -= barrelRotationSpeed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_UP))
    {
        tank2.barrelRotationAngle += barrelRotationSpeed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_DOWN))
    {
        tank2.barrelRotationAngle -= barrelRotationSpeed * deltaTime;
    }

    float minRotation = glm::radians(0.0f);
    float maxRotation = glm::radians(180.0f);
    tank1.barrelRotationAngle = glm::clamp(tank1.barrelRotationAngle, minRotation, maxRotation);
    tank2.barrelRotationAngle = glm::clamp(tank2.barrelRotationAngle, minRotation, maxRotation);
}

void Tema1::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE && tank1.active)
    {
        Projectile projectile;
        projectile.position = GetBarrelEndPosition(tank1);
        projectile.previousPosition = projectile.position;
        float totalAngle = tank1.angle + tank1.barrelRotationAngle;
        projectile.velocity = glm::vec3(cos(totalAngle), sin(totalAngle), 0.0f) * projectileInitialSpeed;
        projectile.radius = 5.0f;
        projectile.color = glm::vec3(1, 1, 0);
        projectile.active = true;
        projectile.ownerTankId = 1;
        projectiles.push_back(projectile);
    }
    else if (key == GLFW_KEY_ENTER && tank2.active)
    {
        Projectile projectile;
        projectile.position = GetBarrelEndPosition(tank2);
        projectile.previousPosition = projectile.position;
        float totalAngle = tank2.angle + tank2.barrelRotationAngle;
        projectile.velocity = glm::vec3(cos(totalAngle), sin(totalAngle), 0.0f) * projectileInitialSpeed;
        projectile.radius = 5.0f;
        projectile.color = glm::vec3(1, 1, 0);
        projectile.active = true;
        projectile.ownerTankId = 2;
        projectiles.push_back(projectile);
    }
    else if (key == GLFW_KEY_N)
    {
        isNight = !isNight;

        CreateTerrainMesh();
    }
}

void Tema1::OnKeyRelease(int key, int mods)
{
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
