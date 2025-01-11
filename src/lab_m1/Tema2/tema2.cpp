#include "lab_m1/Tema2/tema2.h"

#include <vector>
#include <iostream>
#include <string>
#include <math.h>
#include <random>
#include <ctime>
#include <algorithm>
#include "glm/gtx/quaternion.hpp"
#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;
using namespace m1;

static void BuildTerrainMesh(
    int M, int N,
    float spacing,
    std::vector<VertexFormat>& vertices,
    std::vector<unsigned int>& indices)
{
    vertices.clear();
    indices.clear();
    vertices.reserve((M + 1) * (N + 1));

    for (int i = 0; i <= M; i++) {
        for (int j = 0; j <= N; j++) {
            float x = i * spacing - (M * spacing / 2.0f);
            float z = j * spacing - (N * spacing / 2.0f);

            vertices.push_back(
                VertexFormat(
                    glm::vec3(x, 0.0f, z),
                    glm::vec3(0, 1, 0),
                    glm::vec2((float)i, (float)j),
                    glm::vec3(0.0f)));
        }
    }

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            int idxA = i * (N + 1) + j;
            int idxB = (i + 1) * (N + 1) + j;
            int idxC = i * (N + 1) + (j + 1);
            int idxD = (i + 1) * (N + 1) + (j + 1);

            indices.push_back(idxA);
            indices.push_back(idxB);
            indices.push_back(idxC);

            indices.push_back(idxB);
            indices.push_back(idxD);
            indices.push_back(idxC);
        }
    }
}

static float random2DCPU(const glm::vec2& p)
{
    return glm::fract(glm::sin(glm::dot(p, glm::vec2(12.9898f, 78.233f))) * 43758.5453f);
}
static float lerpCPU(float a, float b, float t)
{
    return a + t * (b - a);
}
static float noise2DCPU(const glm::vec2& p)
{
    glm::vec2 ip = glm::floor(p);
    glm::vec2 f = glm::fract(p);

    float a = random2DCPU(ip);
    float b = random2DCPU(ip + glm::vec2(1.0f, 0.f));
    float c = random2DCPU(ip + glm::vec2(0.f, 1.0f));
    float d = random2DCPU(ip + glm::vec2(1.0f, 1.0f));

    float u = lerpCPU(a, b, f.x);
    float v = lerpCPU(c, d, f.x);
    return lerpCPU(u, v, f.y);
}

float tema2::GetTerrainHeight(float x, float z)
{
    int M = 20;
    int N = 20;
    float sp = 2.0f;

    float localX = x + (M * sp / 2.0f);
    float localZ = z + (N * sp / 2.0f);

    float h = noise2DCPU(glm::vec2(localX, localZ) * 0.2f) * 2.0f;
   
    h *= 2.0f;

    return h;
}

Mesh* tema2::CreateSphere(const std::string& name, float radius, int rings, int sectors,
    const glm::vec3& color)
{
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    float R = 1.0f / (float)(rings - 1);
    float S = 1.0f / (float)(sectors - 1);

    for (int r = 0; r < rings; r++) {
        for (int s = 0; s < sectors; s++) {
            float y = sin(-M_PI_2 + M_PI * r * R);
            float x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
            float z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

            glm::vec3 pos = glm::vec3(x * radius, y * radius, z * radius);
            glm::vec3 norm = glm::normalize(glm::vec3(x, y, z));
            vertices.push_back(VertexFormat(pos, norm, glm::vec2(0), color));
        }
    }

    for (int r = 0; r < rings - 1; r++) {
        for (int s = 0; s < sectors - 1; s++) {
            int cur = r * sectors + s;
            int nxt = r * sectors + (s + 1);
            int curR = (r + 1) * sectors + s;
            int nxtR = (r + 1) * sectors + (s + 1);

            indices.push_back(cur);
            indices.push_back(curR);
            indices.push_back(nxt);

            indices.push_back(nxt);
            indices.push_back(curR);
            indices.push_back(nxtR);
        }
    }

    return CreateMesh(name.c_str(), vertices, indices);
}

Mesh* tema2::CreateCylinder(const std::string& name, float height, float radius,
    int segments, const glm::vec3& color)
{
    
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    float angleStep = 2.0f * (float)M_PI / (float)segments;

    vertices.push_back(VertexFormat(glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(0, 1, 0), glm::vec2(0), color));
    vertices.push_back(VertexFormat(glm::vec3(0.f, height, 0.f),
        glm::vec3(0, 1, 0), glm::vec2(0), color));

    for (int i = 0; i < segments; i++) {
        float theta = i * angleStep;
        float xx = radius * cos(theta);
        float zz = radius * sin(theta);

        vertices.push_back(VertexFormat(glm::vec3(xx, 0.f, zz),
            glm::vec3(0, 1, 0), glm::vec2(0), color));
        vertices.push_back(VertexFormat(glm::vec3(xx, height, zz),
            glm::vec3(0, 1, 0), glm::vec2(0), color));
    }

    for (int i = 0; i < segments; i++) {
        int curr = 2 + 2 * i;
        int nxt = 2 + 2 * ((i + 1) % segments);

        indices.push_back(0);
        indices.push_back(nxt);
        indices.push_back(curr);
    }
    for (int i = 0; i < segments; i++) {
        int curr = 3 + 2 * i;
        int nxt = 3 + 2 * ((i + 1) % segments);

        indices.push_back(1);
        indices.push_back(curr);
        indices.push_back(nxt);
    }
    for (int i = 0; i < segments; i++) {
        int base1 = 2 + 2 * i;
        int base2 = 3 + 2 * i;
        int base1N = 2 + 2 * ((i + 1) % segments);
        int base2N = 3 + 2 * ((i + 1) % segments);

        indices.push_back(base1);
        indices.push_back(base1N);
        indices.push_back(base2);

        indices.push_back(base1N);
        indices.push_back(base2N);
        indices.push_back(base2);
    }

    return CreateMesh(name.c_str(), vertices, indices);
}

Mesh* tema2::CreateCone(const std::string& name, float height, float radius,
    int segments, const glm::vec3& color)
{
    
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back(VertexFormat(glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(0, 1, 0), glm::vec2(0), color));
    vertices.push_back(VertexFormat(glm::vec3(0.f, height, 0.f),
        glm::vec3(0, 1, 0), glm::vec2(0), color));

    float angleStep = 2.f * (float)M_PI / segments;
    for (int i = 0; i < segments; i++) {
        float theta = i * angleStep;
        float xx = radius * cos(theta);
        float zz = radius * sin(theta);
        vertices.push_back(VertexFormat(glm::vec3(xx, 0.f, zz),
            glm::vec3(0, 1, 0), glm::vec2(0), color));
    }

    for (int i = 0; i < segments; i++) {
        int curr = 2 + i;
        int nxt = 2 + (i + 1) % segments;
        indices.push_back(0);
        indices.push_back(nxt);
        indices.push_back(curr);
    }
    for (int i = 0; i < segments; i++) {
        int curr = 2 + i;
        int nxt = 2 + (i + 1) % segments;
        indices.push_back(1);
        indices.push_back(curr);
        indices.push_back(nxt);
    }

    return CreateMesh(name.c_str(), vertices, indices);
}

Mesh* tema2::CreateTriangularPrism(const std::string& name, const glm::vec3& color)
{
   
    std::vector<VertexFormat> vertices = {
       
        VertexFormat(glm::vec3(-0.5f, 0.f,  0.5f), glm::vec3(0,1,0),
                     glm::vec2(0), color),
        VertexFormat(glm::vec3(0.5f, 0.f,   0.5f), glm::vec3(0,1,0),
                     glm::vec2(0), color),
        VertexFormat(glm::vec3(0.f,   0.5f, 0.5f), glm::vec3(0,1,0),
                     glm::vec2(0), color),

                    
                     VertexFormat(glm::vec3(-0.5f, 0.f, -0.5f), glm::vec3(0,1,0),
                                  glm::vec2(0), color),
                     VertexFormat(glm::vec3(0.5f,  0.f, -0.5f), glm::vec3(0,1,0),
                                  glm::vec2(0), color),
                     VertexFormat(glm::vec3(0.f,   0.5f,-0.5f), glm::vec3(0,1,0),
                                  glm::vec2(0), color),
    };

    std::vector<unsigned int> indices = {
       
        0,1,2,
        3,4,5,
        0,1,4,
        0,4,3,
        1,2,5,
        1,5,4,
        0,3,5,
        0,5,2
    };

    return CreateMesh(name.c_str(), vertices, indices);
}
tema2::tema2()
{
    dronePos = glm::vec3(0, 15, 0);
    droneYaw = glm::radians(45.0f);
    propellerAngle = 0.0f;
    movementSpeed = 10.0f;
    rotationSpeed = 2.0f;

    cameraDistance = 5.0f;
    cameraHeight = 2.0f;

    srand((unsigned int)time(nullptr));

    
    miniMapCamera = new gfxc::Camera();
    miniMapSize = 30.0f;
}

tema2::~tema2()
{
    delete miniMapCamera;
}

Mesh* tema2::CreateMesh(const char* name,
    const std::vector<VertexFormat>& vertices,
    const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(vertices[0]) * vertices.size(),
        &vertices[0], GL_STATIC_DRAW);

    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices[0]) * indices.size(),
        &indices[0], GL_STATIC_DRAW);

    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        sizeof(VertexFormat), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
        sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
        sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,
        sizeof(VertexFormat),
        (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

    glBindVertexArray(0);
    CheckOpenGLError();

    Mesh* m = new Mesh(name);
    m->InitFromBuffer(VAO, (unsigned int)indices.size());
    m->vertices = vertices;
    m->indices = indices;
    meshes[name] = m;
    return m;
}

void tema2::CreateEnemyDroneMeshes()
{
   
    {
        float halfLength = 1.0f;
        float halfThick = 0.05f;
        vector<VertexFormat> vertices = {
            
            VertexFormat(glm::vec3(-halfLength, -halfThick,  halfThick),
                         glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),
            VertexFormat(glm::vec3(halfLength,  -halfThick,  halfThick),
                         glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),
            VertexFormat(glm::vec3(-halfLength,  halfThick,  halfThick),
                         glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),
            VertexFormat(glm::vec3(halfLength,   halfThick,  halfThick),
                         glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),

                         
                         VertexFormat(glm::vec3(-halfLength, -halfThick, -halfThick),
                                      glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),
                         VertexFormat(glm::vec3(halfLength,  -halfThick, -halfThick),
                                      glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),
                         VertexFormat(glm::vec3(-halfLength,  halfThick, -halfThick),
                                      glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),
                         VertexFormat(glm::vec3(halfLength,   halfThick, -halfThick),
                                      glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),
        };
        vector<unsigned int> indices = {
            0,1,2,  1,3,2,
            2,3,7,  2,7,6,
            1,7,3,  1,5,7,
            6,7,4,  7,5,4,
            0,4,1,  1,4,5,
            2,6,4,  0,2,4
        };
        CreateMesh("armEnemy", vertices, indices);
    }
    {
        float lx = 0.5f;
        float ly = 0.01f;
        float lz = 0.025f;
        vector<VertexFormat> vertices = {
          
            VertexFormat(glm::vec3(-lx, -ly,  lz),
                         glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),
            VertexFormat(glm::vec3(lx,  -ly,  lz),
                         glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),
            VertexFormat(glm::vec3(-lx,  ly,  lz),
                         glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),
            VertexFormat(glm::vec3(lx,   ly,  lz),
                         glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),

                         
                         VertexFormat(glm::vec3(-lx, -ly, -lz),
                                      glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),
                         VertexFormat(glm::vec3(lx,  -ly, -lz),
                                      glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),
                         VertexFormat(glm::vec3(-lx,  ly, -lz),
                                      glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),
                         VertexFormat(glm::vec3(lx,   ly, -lz),
                                      glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),
        };
        vector<unsigned int> indices = {
            0,1,2, 1,3,2,
            2,3,7, 2,7,6,
            1,7,3, 1,5,7,
            6,7,4, 7,5,4,
            0,4,1, 1,4,5,
            2,6,4, 0,2,4
        };
        CreateMesh("propellerEnemy", vertices, indices);
    }
    {
       
        vector<VertexFormat> vertices =
        {
            VertexFormat(glm::vec3(-0.5f, -0.5f,  0.5f),
                         glm::vec3(0,0,1), glm::vec2(0), glm::vec3(0.0f)),
            VertexFormat(glm::vec3(0.5f, -0.5f,   0.5f),
                         glm::vec3(0,0,1), glm::vec2(0), glm::vec3(0.0f)),
            VertexFormat(glm::vec3(-0.5f,  0.5f,  0.5f),
                         glm::vec3(0,0,1), glm::vec2(0), glm::vec3(0.0f)),
            VertexFormat(glm::vec3(0.5f,   0.5f,  0.5f),
                         glm::vec3(0,0,1), glm::vec2(0), glm::vec3(0.0f)),

            VertexFormat(glm::vec3(-0.5f, -0.5f, -0.5f),
                         glm::vec3(0,0,1), glm::vec2(0), glm::vec3(0.0f)),
            VertexFormat(glm::vec3(0.5f,  -0.5f, -0.5f),
                         glm::vec3(0,0,1), glm::vec2(0), glm::vec3(0.0f)),
            VertexFormat(glm::vec3(-0.5f,   0.5f, -0.5f),
                         glm::vec3(0,0,1), glm::vec2(0), glm::vec3(0.0f)),
            VertexFormat(glm::vec3(0.5f,   0.5f, -0.5f),
                         glm::vec3(0,0,1), glm::vec2(0), glm::vec3(0.0f)),
        };
        vector<unsigned int> indices =
        {
            0,1,2,  1,3,2,
            2,3,7,  2,7,6,
            1,7,3,  1,5,7,
            6,7,4,  7,5,4,
            0,4,1,  1,4,5,
            2,6,4,  0,2,4
        };
        CreateMesh("cubeEnemy", vertices, indices);
    }
}
void tema2::RenderEnemyDrone(const glm::vec3& pos, float scale, float propAngle)
{
   
    glm::mat4 base(1);
    base = glm::translate(base, pos);
   
    base = glm::rotate(base, glm::radians(0.0f), glm::vec3(0, 1, 0));
    base = glm::scale(base, glm::vec3(scale));

    
    {
        glm::mat4 matArm = base;
        RenderSimpleMesh(meshes["armEnemy"], shaders["LabShader"], matArm);

        matArm = glm::rotate(base, glm::radians(90.0f), glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["armEnemy"], shaders["LabShader"], matArm);
    }

    
    for (int sign = -1; sign <= 1; sign += 2)
    {
        glm::mat4 matCube = base;
        matCube = glm::translate(matCube, glm::vec3(sign * 1.f, 0.f, 0.f));
        matCube = glm::scale(matCube, glm::vec3(0.2f));
        RenderSimpleMesh(meshes["cubeEnemy"], shaders["LabShader"], matCube);

        glm::mat4 matProp = base;
        matProp = glm::translate(matProp, glm::vec3(sign * 1.f, 0.f, 0.f));
        matProp = glm::rotate(matProp, propAngle, glm::vec3(0, 1, 0));
        matProp = glm::translate(matProp, glm::vec3(0, 0.05f, 0));
        RenderSimpleMesh(meshes["propellerEnemy"], shaders["LabShader"], matProp);
    }
    for (int sign = -1; sign <= 1; sign += 2)
    {
        glm::mat4 matCube = base;
        matCube = glm::translate(matCube, glm::vec3(0.f, 0.f, sign * 1.f));
        matCube = glm::scale(matCube, glm::vec3(0.2f));
        RenderSimpleMesh(meshes["cubeEnemy"], shaders["LabShader"], matCube);

        glm::mat4 matProp = base;
        matProp = glm::translate(matProp, glm::vec3(0.f, 0.f, sign * 1.f));
        matProp = glm::rotate(matProp, propAngle, glm::vec3(0, 1, 0));
        matProp = glm::translate(matProp, glm::vec3(0, 0.05f, 0));
        RenderSimpleMesh(meshes["propellerEnemy"], shaders["LabShader"], matProp);
    }
}

void tema2::Init()
{
   
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(
            PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"),
            "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
       
        Mesh* sphere = CreateSphere("sphere", 0.2f, 16, 16, glm::vec3(1.0f, 0.0f, 0.0f));
        meshes[sphere->GetMeshID()] = sphere;
    }
   

    
    {
        vector<VertexFormat> vertices =
        {
            VertexFormat(glm::vec3(-0.5f, -0.5f,  0.5f),
                         glm::vec3(0,0,1), glm::vec2(0), glm::vec3(0.5f)),
            VertexFormat(glm::vec3(0.5f, -0.5f,   0.5f),
                         glm::vec3(0,0,1), glm::vec2(0), glm::vec3(0.5f)),
            VertexFormat(glm::vec3(-0.5f,  0.5f,  0.5f),
                         glm::vec3(0,0,1), glm::vec2(0), glm::vec3(0.5f)),
            VertexFormat(glm::vec3(0.5f,   0.5f,  0.5f),
                         glm::vec3(0,0,1), glm::vec2(0), glm::vec3(0.5f)),

            VertexFormat(glm::vec3(-0.5f, -0.5f, -0.5f),
                         glm::vec3(0,0,1), glm::vec2(0), glm::vec3(0.5f)),
            VertexFormat(glm::vec3(0.5f,  -0.5f, -0.5f),
                         glm::vec3(0,0,1), glm::vec2(0), glm::vec3(0.5f)),
            VertexFormat(glm::vec3(-0.5f,   0.5f, -0.5f),
                         glm::vec3(0,0,1), glm::vec2(0), glm::vec3(0.5f)),
            VertexFormat(glm::vec3(0.5f,   0.5f, -0.5f),
                         glm::vec3(0,0,1), glm::vec2(0), glm::vec3(0.5f)),
        };
        vector<unsigned int> indices =
        {
            0,1,2,  1,3,2,
            2,3,7,  2,7,6,
            1,7,3,  1,5,7,
            6,7,4,  7,5,4,
            0,4,1,  1,4,5,
            2,6,4,  0,2,4
        };
        CreateMesh("cube", vertices, indices);
    }
    {
        float halfLength = 1.0f;
        float halfThick = 0.05f;
        vector<VertexFormat> vertices = {
            
            VertexFormat(glm::vec3(-halfLength, -halfThick,  halfThick),
                         glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.7f)),
            VertexFormat(glm::vec3(halfLength,  -halfThick,  halfThick),
                         glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.7f)),
            VertexFormat(glm::vec3(-halfLength,  halfThick,  halfThick),
                         glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.7f)),
            VertexFormat(glm::vec3(halfLength,   halfThick,  halfThick),
                         glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.7f)),

                        
                         VertexFormat(glm::vec3(-halfLength, -halfThick, -halfThick),
                                      glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.7f)),
                         VertexFormat(glm::vec3(halfLength,  -halfThick, -halfThick),
                                      glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.7f)),
                         VertexFormat(glm::vec3(-halfLength,  halfThick, -halfThick),
                                      glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.7f)),
                         VertexFormat(glm::vec3(halfLength,   halfThick, -halfThick),
                                      glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.7f)),
        };
        vector<unsigned int> indices = {
            0,1,2,  1,3,2,
            2,3,7,  2,7,6,
            1,7,3,  1,5,7,
            6,7,4,  7,5,4,
            0,4,1,  1,4,5,
            2,6,4,  0,2,4
        };
        CreateMesh("arm", vertices, indices);
    }
    {
        float lx = 0.5f;
        float ly = 0.01f;
        float lz = 0.025f;
        vector<VertexFormat> vertices = {
            
            VertexFormat(glm::vec3(-lx, -ly,  lz),
                         glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),
            VertexFormat(glm::vec3(lx,  -ly,  lz),
                         glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),
            VertexFormat(glm::vec3(-lx,  ly,  lz),
                         glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),
            VertexFormat(glm::vec3(lx,   ly,  lz),
                         glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),

                         VertexFormat(glm::vec3(-lx, -ly, -lz),
                                      glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),
                         VertexFormat(glm::vec3(lx,  -ly, -lz),
                                      glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),
                         VertexFormat(glm::vec3(-lx,  ly, -lz),
                                      glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),
                         VertexFormat(glm::vec3(lx,   ly, -lz),
                                      glm::vec3(0,1,0), glm::vec2(0), glm::vec3(0.0f)),
        };
        vector<unsigned int> indices = {
            0,1,2, 1,3,2,
            2,3,7, 2,7,6,
            1,7,3, 1,5,7,
            6,7,4, 7,5,4,
            0,4,1,  1,4,5,
            2,6,4,  0,2,4
        };
        CreateMesh("propeller", vertices, indices);
    }
    
    {
        int M = 20;
        int N = 20;
        float sp = 2.0f;
        vector<VertexFormat> verts;
        vector<unsigned int> inds;

        BuildTerrainMesh(M, N, sp, verts, inds);
        CreateMesh("terrain", verts, inds);
    }
    {
        Mesh* cylinder = CreateCylinder("cylinder", 1.0f, 0.2f, 16,
            glm::vec3(0.36f, 0.2f, 0.09f));
        meshes[cylinder->GetMeshID()] = cylinder;

        Mesh* cone = CreateCone("cone", 1.0f, 0.6f, 16,
            glm::vec3(0.1f, 0.6f, 0.1f));
        meshes[cone->GetMeshID()] = cone;
    }
    {
        Mesh* roofPrism = CreateTriangularPrism("roof",
            glm::vec3(0.8f, 0.1f, 0.1f));
        meshes[roofPrism->GetMeshID()] = roofPrism;
    }

    
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(
            PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1,
                "Tema2", "shaders", "VertexShader.glsl"),
            GL_VERTEX_SHADER);
        shader->AddShader(
            PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1,
                "Tema2", "shaders", "FragmentShader.glsl"),
            GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    
    GetSceneCamera()->SetPosition(glm::vec3(0, 2, -5));
    GetSceneCamera()->SetRotation(glm::vec3(0, 0, 0));
    GetSceneCamera()->Update();

    viewMatrixLocation = glGetUniformLocation(shaders["LabShader"]->GetProgramID(), "View");

    
    {
        int numTrees = 10 + rand() % 6;
        float rangeXZ = 25.0f;
        float minDist = 4.0f;

        while ((int)trees.size() < numTrees)
        {
            float rx = -rangeXZ + static_cast<float>(rand())
                / (static_cast<float>(RAND_MAX) / (2 * rangeXZ));
            float rz = -rangeXZ + static_cast<float>(rand())
                / (static_cast<float>(RAND_MAX) / (2 * rangeXZ));

            float terrainY = GetTerrainHeight(rx, rz);
            float ry = terrainY + 0.3f;

            float scale = 0.5f + static_cast<float>(rand())
                / (static_cast<float>(RAND_MAX) / 1.5f);

            bool overlap = false;
            for (auto& t : trees) {
                float dx = rx - t.position.x;
                float dz = rz - t.position.z;
                float dist2 = sqrtf(dx * dx + dz * dz);
                if (dist2 < minDist) {
                    overlap = true;
                    break;
                }
            }

            if (!overlap)
            {
                Tree newTree;
                newTree.position = glm::vec3(rx, ry, rz);
                newTree.scale = scale;
                trees.push_back(newTree);
            }
        }
    }
    {
        int numHouses = 5 + rand() % 3;
        float rangeXZ = 25.0f;
        float minDist = 4.0f;
        while ((int)houses.size() < numHouses)
        {
            float rx = -rangeXZ + static_cast<float>(rand())
                / (static_cast<float>(RAND_MAX) / (2 * rangeXZ));
            float rz = -rangeXZ + static_cast<float>(rand())
                / (static_cast<float>(RAND_MAX) / (2 * rangeXZ));

            float terrainY = GetTerrainHeight(rx, rz) / 2.0f;
            float ry;
            if (terrainY > 1.5f)
                ry = terrainY + 1.0f;
            else
                ry = terrainY + 0.3f;

            float scale = 1.0f + static_cast<float>(rand())
                / (static_cast<float>(RAND_MAX) / 1.5f);

            bool overlap = false;
            
            for (auto& t : trees) {
                float dx = rx - t.position.x;
                float dz = rz - t.position.z;
                float dist2 = sqrtf(dx * dx + dz * dz);
                if (dist2 < minDist) {
                    overlap = true;
                    break;
                }
            }
            if (overlap) continue;

            for (auto& h : houses) {
                float dx = rx - h.position.x;
                float dz = rz - h.position.z;
                float dist2 = sqrtf(dx * dx + dz * dz);
                if (dist2 < minDist) {
                    overlap = true;
                    break;
                }
            }
            if (overlap) continue;

            House newHouse;
            newHouse.position = glm::vec3(rx, ry, rz);
            newHouse.scale = scale;
            houses.push_back(newHouse);
        }
    }

   
    CreateEnemyDroneMeshes();
    {
        int numEnemies = 5 + rand() % 3;
        float rangeXZ = 25.0f;
        float minDist = 4.0f;  

        while ((int)enemies.size() < numEnemies)
        {
            float rx = -rangeXZ + static_cast<float>(rand())
                / (static_cast<float>(RAND_MAX) / (2 * rangeXZ));
            float rz = -rangeXZ + static_cast<float>(rand())
                / (static_cast<float>(RAND_MAX) / (2 * rangeXZ));

            float terrainY = GetTerrainHeight(rx, rz);
           
            float ry = terrainY + 2.0f + 1.5f;

            float scale = 1.0f;

            bool overlap = false;
            for (auto& t : trees) {
                float dx = rx - t.position.x;
                float dz = rz - t.position.z;
                float dist2 = sqrtf(dx * dx + dz * dz);
                if (dist2 < minDist) {
                    overlap = true;
                    break;
                }
            }
            if (overlap) continue;

            for (auto& h : houses) {
                float dx = rx - h.position.x;
                float dz = rz - h.position.z;
                float dist2 = sqrtf(dx * dx + dz * dz);
                if (dist2 < minDist) {
                    overlap = true;
                    break;
                }
            }
            if (overlap) continue;

            for (auto& e : enemies) {
                float dx = rx - e.position.x;
                float dz = rz - e.position.z;
                float dist2 = sqrtf(dx * dx + dz * dz);
                if (dist2 < minDist) {
                    overlap = true;
                    break;
                }
            }
            if (overlap) continue;

            EnemyDrone ed;
            ed.position = glm::vec3(rx, ry, rz);
            ed.scale = scale;
            enemies.push_back(ed);
        }
    }
}

void tema2::UpdateProjectiles(float deltaTimeSeconds)
{
    for (unsigned int i = 0; i < projectiles.size(); )
    {
        Projectile& pr = projectiles[i];

        float gravity = 9.8f;
        pr.velocity.y -= gravity * deltaTimeSeconds;
        pr.position += pr.velocity * deltaTimeSeconds;

        float groundH = GetTerrainHeight(pr.position.x, pr.position.z);

        
        if (pr.position.y <= groundH)
        {
            projectiles.erase(projectiles.begin() + i);
            continue;
        }

       
        bool erased = false;
        for (unsigned int e = 0; e < enemies.size() && !erased; e++)
        {
            glm::vec3 diff = pr.position - enemies[e].position;
            float dist = glm::length(diff);
            float Rproj = 0.5f;
            float Renemy = 1.2f;
            if (dist < (Rproj + Renemy))
            {
               
                enemies.erase(enemies.begin() + e);
                projectiles.erase(projectiles.begin() + i);
                erased = true;
            }
        }

        if (!erased)
            i++;
    }
}

void tema2::RenderProjectiles()
{
    for (auto& pr : projectiles)
    {
        glm::mat4 mm(1);
        mm = glm::translate(mm, pr.position);
        mm = glm::scale(mm, glm::vec3(1.f));
        
        RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], mm);
    }
}

void tema2::FrameStart()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_CULL_FACE);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void tema2::Update(float deltaTimeSeconds)
{
   
    propellerAngle += 4.0f * deltaTimeSeconds;

   
    {
        glm::vec3 forward(sin(droneYaw), 0.f, cos(droneYaw));
        glm::vec3 camPos = dronePos - forward * cameraDistance + glm::vec3(0.f, cameraHeight, 0.f);
        glm::vec3 target = dronePos;
        glm::vec3 up(0.f, 1.f, 0.f);

        glm::vec3 f = glm::normalize(target - camPos);
        glm::vec3 r = glm::normalize(glm::cross(f, up));
        glm::vec3 uu = glm::cross(r, f);

        glm::mat4 view(1.f);
        view[0][0] = r.x;  view[1][0] = r.y;  view[2][0] = r.z;
        view[0][1] = uu.x; view[1][1] = uu.y; view[2][1] = uu.z;
        view[0][2] = -f.x; view[1][2] = -f.y; view[2][2] = -f.z;
        view = glm::translate(view, -camPos);

        glm::mat4 modelCam = glm::inverse(view);
        glm::vec3 finalPos = glm::vec3(modelCam[3]);
        modelCam[3] = glm::vec4(0, 0, 0, 1);
        glm::quat finalRot = glm::toQuat(modelCam);

        GetSceneCamera()->SetPositionAndRotation(finalPos, finalRot);
        GetSceneCamera()->Update();
    }

    
    {
       
        {
            glm::mat4 mm(1);
            mm = glm::scale(mm, glm::vec3(2.f));
            RenderSimpleMesh(meshes["terrain"], shaders["LabShader"], mm);
        }

        
        {
            glm::mat4 base(1);
            base = glm::translate(base, dronePos);
            base = glm::rotate(base, droneYaw, glm::vec3(0, 1, 0));
            base = glm::rotate(base, glm::radians(45.0f), glm::vec3(0, 1, 0));

            
            {
                glm::mat4 matArm = base;
                RenderSimpleMesh(meshes["arm"], shaders["LabShader"], matArm);

                matArm = glm::rotate(base, glm::radians(90.0f), glm::vec3(0, 1, 0));
                RenderSimpleMesh(meshes["arm"], shaders["LabShader"], matArm);
            }
           
            for (int sign = -1; sign <= 1; sign += 2)
            {
                glm::mat4 matC = base;
                matC = glm::translate(matC, glm::vec3(sign * 1.f, 0.f, 0.f));
                matC = glm::scale(matC, glm::vec3(0.2f));
                RenderSimpleMesh(meshes["cube"], shaders["LabShader"], matC);

                glm::mat4 matP = base;
                matP = glm::translate(matP, glm::vec3(sign * 1.f, 0.f, 0.f));
                matP = glm::rotate(matP, propellerAngle, glm::vec3(0, 1, 0));
                matP = glm::translate(matP, glm::vec3(0, 0.05f, 0));
                RenderSimpleMesh(meshes["propeller"], shaders["LabShader"], matP);
            }
            for (int sign = -1; sign <= 1; sign += 2)
            {
                glm::mat4 matC = base;
                matC = glm::translate(matC, glm::vec3(0.f, 0.f, sign * 1.f));
                matC = glm::scale(matC, glm::vec3(0.2f));
                RenderSimpleMesh(meshes["cube"], shaders["LabShader"], matC);

                glm::mat4 matP = base;
                matP = glm::translate(matP, glm::vec3(0.f, 0.f, sign * 1.f));
                matP = glm::rotate(matP, propellerAngle, glm::vec3(0, 1, 0));
                matP = glm::translate(matP, glm::vec3(0, 0.05f, 0));
                RenderSimpleMesh(meshes["propeller"], shaders["LabShader"], matP);
            }
        }

        
        for (auto& tree : trees)
        {
            glm::mat4 mm(1);
            mm = glm::translate(mm, tree.position);
            mm = glm::scale(mm, glm::vec3(tree.scale));

            glm::mat4 trunkMatrix = glm::scale(mm, glm::vec3(1.f, 2.f, 1.f));
            RenderSimpleMesh(meshes["cylinder"], shaders["LabShader"], trunkMatrix);

            glm::mat4 coneMatrix1 = glm::translate(mm, glm::vec3(0, 2.f, 0));
            RenderSimpleMesh(meshes["cone"], shaders["LabShader"], coneMatrix1);

            glm::mat4 coneMatrix2 = glm::translate(mm, glm::vec3(0, 3.f, 0));
            RenderSimpleMesh(meshes["cone"], shaders["LabShader"], coneMatrix2);
        }

        
        for (auto& house : houses)
        {
            glm::mat4 mm(1);
            mm = glm::translate(mm, house.position);
            mm = glm::scale(mm, glm::vec3(house.scale));

            glm::mat4 body = glm::scale(mm, glm::vec3(2.f, 5.f, 2.f));
            RenderSimpleMesh(meshes["cube"], shaders["LabShader"], body);

            glm::mat4 roof = glm::translate(mm, glm::vec3(0.f, 2.5f, 0.f));
            roof = glm::scale(roof, glm::vec3(2.f, 1.f, 2.f));
            RenderSimpleMesh(meshes["roof"], shaders["LabShader"], roof);
        }

        
        for (auto& e : enemies)
        {
            float enemyProp = propellerAngle * 2.5f;
            RenderEnemyDrone(e.position, e.scale, enemyProp);
        }
    }
   
    UpdateProjectiles(deltaTimeSeconds);
   
    RenderProjectiles();

   
    {
        glm::ivec2 resolution = window->GetResolution();
        int miniWidth = resolution.x / 4;
        int miniHeight = resolution.y / 4;
        glViewport(0, 0, miniWidth, miniHeight);

        glClear(GL_DEPTH_BUFFER_BIT);

        {
            glm::vec3 position = dronePos + glm::vec3(0, 10.f, 0);
            glm::vec3 forward = glm::vec3(0, -1, 0);
            glm::vec3 up = glm::vec3(0, 0, -1);

            glm::mat4 view = glm::lookAt(position, position + forward, up);
            miniMapCamera->SetPosition(position);
            miniMapCamera->SetRotation(glm::quatLookAt(forward, up));

            miniMapCamera->SetOrthographic(
                -miniMapSize, miniMapSize,
                -miniMapSize, miniMapSize,
                0.1f, 200.f
            );
            miniMapCamera->Update();
        }

        {
            glm::mat4 mm(1);
            mm = glm::scale(mm, glm::vec3(2.f));
            RenderSimpleMeshMinimap(meshes["terrain"], shaders["LabShader"], mm, miniMapCamera);
        }

        glm::mat4 base(1);
        base = glm::translate(base, dronePos);
        base = glm::rotate(base, droneYaw, glm::vec3(0, 1, 0));
        base = glm::rotate(base, glm::radians(45.0f), glm::vec3(0, 1, 0));
        base = glm::scale(base, glm::vec3(2, 2, 2));

        {
            glm::mat4 matArm = base;
            RenderSimpleMeshMinimap(meshes["arm"], shaders["LabShader"], matArm, miniMapCamera);

            matArm = glm::rotate(base, glm::radians(90.0f), glm::vec3(0, 1, 0));
            RenderSimpleMeshMinimap(meshes["arm"], shaders["LabShader"], matArm, miniMapCamera);
        }
        for (int sign = -1; sign <= 1; sign += 2)
        {
            glm::mat4 c = base;
            c = glm::translate(c, glm::vec3(sign * 1.f, 0.f, 0.f));
            c = glm::scale(c, glm::vec3(0.2f));
            RenderSimpleMeshMinimap(meshes["cube"], shaders["LabShader"], c, miniMapCamera);

            glm::mat4 p = base;
            p = glm::translate(p, glm::vec3(sign * 1.f, 0.f, 0.f));
            p = glm::rotate(p, propellerAngle, glm::vec3(0, 1, 0));
            p = glm::translate(p, glm::vec3(0, 0.05f, 0));
            RenderSimpleMeshMinimap(meshes["propeller"], shaders["LabShader"], p, miniMapCamera);
        }
        for (int sign = -1; sign <= 1; sign += 2)
        {
            glm::mat4 c = base;
            c = glm::translate(c, glm::vec3(0.f, 0.f, sign * 1.f));
            c = glm::scale(c, glm::vec3(0.2f));
            RenderSimpleMeshMinimap(meshes["cube"], shaders["LabShader"], c, miniMapCamera);

            glm::mat4 p = base;
            p = glm::translate(p, glm::vec3(0.f, 0.f, sign * 1.f));
            p = glm::rotate(p, propellerAngle, glm::vec3(0, 1, 0));
            p = glm::translate(p, glm::vec3(0, 0.05f, 0));
            RenderSimpleMeshMinimap(meshes["propeller"], shaders["LabShader"], p, miniMapCamera);
        }

        for (auto& tree : trees)
        {
            glm::mat4 mm(1);
            mm = glm::translate(mm, tree.position);
            mm = glm::scale(mm, glm::vec3(tree.scale));

            glm::mat4 trunkMatrix = glm::scale(mm, glm::vec3(1.f, 2.f, 1.f));
            RenderSimpleMeshMinimap(meshes["cylinder"], shaders["LabShader"], trunkMatrix, miniMapCamera);

            glm::mat4 coneMatrix1 = glm::translate(mm, glm::vec3(0, 2.f, 0));
            RenderSimpleMeshMinimap(meshes["cone"], shaders["LabShader"], coneMatrix1, miniMapCamera);

            glm::mat4 coneMatrix2 = glm::translate(mm, glm::vec3(0, 3.f, 0));
            RenderSimpleMeshMinimap(meshes["cone"], shaders["LabShader"], coneMatrix2, miniMapCamera);
        }

        for (auto& house : houses)
        {
            glm::mat4 mm(1);
            mm = glm::translate(mm, house.position);
            mm = glm::scale(mm, glm::vec3(house.scale));

            glm::mat4 bodyMatrix = glm::scale(mm, glm::vec3(2.f, 5.f, 2.f));
            RenderSimpleMeshMinimap(meshes["cube"], shaders["LabShader"], bodyMatrix, miniMapCamera);

            glm::mat4 roofMatrix = glm::translate(mm, glm::vec3(0.f, 2.5f, 0.f));
            roofMatrix = glm::scale(roofMatrix, glm::vec3(2.f, 1.f, 2.f));
            RenderSimpleMeshMinimap(meshes["roof"], shaders["LabShader"], roofMatrix, miniMapCamera);
        }

        
        for (auto& e : enemies)
        {
            float enemyProp = propellerAngle * 2.5f;
        
            glm::mat4 baseE(1);
            baseE = glm::translate(baseE, e.position);
            baseE = glm::rotate(baseE, glm::radians(0.0f), glm::vec3(0, 1, 0));
            baseE = glm::scale(baseE, glm::vec3(e.scale));

            
            {
                glm::mat4 matArm = baseE;
                RenderSimpleMeshMinimap(meshes["armEnemy"], shaders["LabShader"], matArm, miniMapCamera);

                matArm = glm::rotate(baseE, glm::radians(90.0f), glm::vec3(0, 1, 0));
                RenderSimpleMeshMinimap(meshes["armEnemy"], shaders["LabShader"], matArm, miniMapCamera);
            }
            
            for (int sign = -1; sign <= 1; sign += 2)
            {
                glm::mat4 c = baseE;
                c = glm::translate(c, glm::vec3(sign * 1.f, 0.f, 0.f));
                c = glm::scale(c, glm::vec3(0.2f));
                RenderSimpleMeshMinimap(meshes["cubeEnemy"], shaders["LabShader"], c, miniMapCamera);

                glm::mat4 p = baseE;
                p = glm::translate(p, glm::vec3(sign * 1.f, 0.f, 0.f));
                p = glm::rotate(p, enemyProp, glm::vec3(0, 1, 0));
                p = glm::translate(p, glm::vec3(0, 0.05f, 0));
                RenderSimpleMeshMinimap(meshes["propellerEnemy"], shaders["LabShader"], p, miniMapCamera);
            }
            for (int sign = -1; sign <= 1; sign += 2)
            {
                glm::mat4 c = baseE;
                c = glm::translate(c, glm::vec3(0.f, 0.f, sign * 1.f));
                c = glm::scale(c, glm::vec3(0.2f));
                RenderSimpleMeshMinimap(meshes["cubeEnemy"], shaders["LabShader"], c, miniMapCamera);

                glm::mat4 p = baseE;
                p = glm::translate(p, glm::vec3(0.f, 0.f, sign * 1.f));
                p = glm::rotate(p, enemyProp, glm::vec3(0, 1, 0));
                p = glm::translate(p, glm::vec3(0, 0.05f, 0));
                RenderSimpleMeshMinimap(meshes["propellerEnemy"], shaders["LabShader"], p, miniMapCamera);
            }
        }
    }
}

void tema2::FrameEnd()
{
   
}


void tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader,
    const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID()) return;

    glUseProgram(shader->program);

    bool isTerrain = (std::string(mesh->GetMeshID()) == "terrain");
    GLint terrainLoc = glGetUniformLocation(shader->program, "isTerrain");
    glUniform1i(terrainLoc, (isTerrain ? 1 : 0));

    GLint modelLoc = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE,
        glm::value_ptr(modelMatrix));

    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    GLint viewLoc = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE,
        glm::value_ptr(viewMatrix));

    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    GLint projLoc = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE,
        glm::value_ptr(projectionMatrix));

    float time = Engine::GetElapsedTime();
    GLint timeLoc = glGetUniformLocation(shader->program, "Time");
    glUniform1f(timeLoc, time);

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(),
        (int)mesh->indices.size(),
        GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void tema2::RenderSimpleMeshMinimap(Mesh* mesh, Shader* shader,
    const glm::mat4& modelMatrix, gfxc::Camera* miniCam)
{
    if (!mesh || !shader || !shader->GetProgramID()) return;

    glUseProgram(shader->program);

    bool isTerrain = (std::string(mesh->GetMeshID()) == "terrain");
    GLint terrainLoc = glGetUniformLocation(shader->program, "isTerrain");
    glUniform1i(terrainLoc, (isTerrain ? 1 : 0));

    GLint modelLoc = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE,
        glm::value_ptr(modelMatrix));

    glm::mat4 viewMatrix = miniCam->GetViewMatrix();
    GLint viewLoc = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE,
        glm::value_ptr(viewMatrix));

    glm::mat4 projectionMatrix = miniCam->GetProjectionMatrix();
    GLint projLoc = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE,
        glm::value_ptr(projectionMatrix));

    float time = Engine::GetElapsedTime();
    GLint timeLoc = glGetUniformLocation(shader->program, "Time");
    glUniform1f(timeLoc, time);

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(),
        (int)mesh->indices.size(),
        GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

bool tema2::CheckCollisionWithGround(glm::vec3& newPos, float radius)
{
    float terrainH = GetTerrainHeight(newPos.x, newPos.z);
    if (newPos.y - radius <= terrainH + 0.3f)
    {
        cout << "da este coliziune" << endl;
        newPos.y = terrainH + radius;
        return true;
    }
    return false;
}

bool tema2::CheckCollisionWithTrees(const glm::vec3& newPos, float radius)
{
    for (auto& t : trees) {
        float R = 0.8f * t.scale;
        float dx = newPos.x - t.position.x;
        float dz = newPos.z - t.position.z;
        float distXZ = sqrtf(dx * dx + dz * dz);
        if (distXZ < (radius + R))
        {
            float treeMinY = t.position.y;
            float treeMaxY = t.position.y + 5.f * t.scale;
            float drMinY = newPos.y - radius;
            float drMaxY = newPos.y + radius;
            if (!(drMaxY < treeMinY || drMinY > treeMaxY))
                return true;
        }
    }
    return false;
}

bool tema2::CheckCollisionWithHouses(const glm::vec3& newPos, float radius)
{
    for (auto& h : houses)
    {
        float halfW = 1.f * h.scale;
        float halfH = 2.f * h.scale;
        float halfD = 1.f * h.scale;

        float boxMinX = h.position.x - halfW;
        float boxMaxX = h.position.x + halfW;
        float boxMinY = h.position.y;
        float boxMaxY = h.position.y + 4.f * h.scale;
        float boxMinZ = h.position.z - halfD;
        float boxMaxZ = h.position.z + halfD;

        float cx = std::max(boxMinX, std::min(newPos.x, boxMaxX));
        float cy = std::max(boxMinY, std::min(newPos.y, boxMaxY));
        float cz = std::max(boxMinZ, std::min(newPos.z, boxMaxZ));

        float dx = newPos.x - cx;
        float dy = newPos.y - cy;
        float dz = newPos.z - cz;
        float dist2 = dx * dx + dy * dy + dz * dz;
        if (dist2 < radius * radius)
            return true;
    }
    return false;
}

void tema2::OnInputUpdate(float deltaTime, int mods)
{
    glm::vec3 oldPos = dronePos;
    float droneRadius = 1.2f;

    float dx = 0, dy = 0, dz = 0;

    if (window->KeyHold(GLFW_KEY_W)) {
        dx += sin(droneYaw) * movementSpeed * deltaTime;
        dz += cos(droneYaw) * movementSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        dx -= sin(droneYaw) * movementSpeed * deltaTime;
        dz -= cos(droneYaw) * movementSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        dx -= cos(droneYaw) * movementSpeed * deltaTime;
        dz += sin(droneYaw) * movementSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        dx += cos(droneYaw) * movementSpeed * deltaTime;
        dz -= sin(droneYaw) * movementSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_UP)) {
        dy += movementSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_DOWN)) {
        dy -= movementSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_LEFT)) {
        droneYaw += rotationSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        droneYaw -= rotationSpeed * deltaTime;
    }

    dronePos.x += dx;
    dronePos.y += dy;
    dronePos.z += dz;

    bool cGround = CheckCollisionWithGround(dronePos, droneRadius);
    if (cGround) {
        dronePos = oldPos;
        dronePos.y = oldPos.y + 0.1f;
    }
    bool cTree = CheckCollisionWithTrees(dronePos, droneRadius);
    bool cHouse = CheckCollisionWithHouses(dronePos, droneRadius);

    if (cTree || cHouse)
    {
        dronePos = oldPos;
        CheckCollisionWithGround(dronePos, droneRadius);
    }
    CheckCollisionWithGround(dronePos, droneRadius);
}

void tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE)
    {
        Projectile p;
        std::cout << "[SPACE] -> creez proiectil!\n";

        glm::vec3 forward(sin(droneYaw), 0.f, cos(droneYaw));
        p.position = dronePos + glm::vec3(0, 0.f, 0) + forward * 1.5f;

        p.velocity = forward * 10.0f;
        p.velocity.y = 5.0f;

        projectiles.push_back(p);
        std::cout << "Proiectile: " << projectiles.size() << std::endl;
    }
}
void tema2::OnKeyRelease(int key, int mods) {}
void tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}
void tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}
void tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
void tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}
void tema2::OnWindowResize(int width, int height) {}
