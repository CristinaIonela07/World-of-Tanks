#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;

Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    camera1 = new implemented::Camera1();
    camera2 = new implemented::Camera1();

    cameraPos = glm::vec3(0, 4.5, 5);
    cameraFront = glm::vec3(0, 0, -5);
    cameraUp = glm::vec3(0, 1, 0);

    camera1->Set(cameraPos, cameraFront, cameraUp, 10);
    camera2->Set(cameraPos, cameraFront, cameraUp, 10);

    rotate_turela = 0;
    tank_rotate = 0;

    my_time = 0;

    random = rand() % 10;
    while (random < 5) {
        random = rand() % 10;
    }


    for (int i = 0; i < random; i++) {
        int x, y;
        if (i % 2 == 0) {
            x = rand() % 80;
            y = rand() % 80;
        }

        if (i % 2 == 1) {
            x = rand() % 30;
            y = rand() % 30;
        }

        if (x % 2 == 0)
            x = -x;
        if (y % 3 == 0)
            y = -y;

        enemy_positions.push_back( glm::vec3(x, 0.4f, y));
        enemy_colors.push_back(glm::vec3(rand() % 10 / float(10), rand() % 10 / float(10), rand() % 10 / float(10)));
        enemy_nr_hits.push_back(0);
        enemy_tank_rotate.push_back(rand() % 20 / float(10));
        enemy_turela_rotate.push_back(0);
        time_to_choose.push_back(0);
        choice.push_back(0);
        enemy_is_shooting.push_back(0);
        enemy_update_bullet.push_back(0);
        enemy_bullet_pos.push_back(glm::vec3(0, 0, 0));
        enemy_old_rotate_tank.push_back(0);
        enemy_old_rotate_turela.push_back(0);
        enemy_time.push_back(0);
    }

    random2 = rand() % 20;
    while (random2 < 10) {
        random2 = rand() % 20;
    }

    for (int i = 0; i < random2; i++) {
        int x, y;
        if (i % 2 == 0) {
            x = rand() % 80;
            y = rand() % 80;
        }

        if (i % 2 == 1) {
            x = rand() % 30;
            y = rand() % 30;
        }

        if (x % 2 == 0)
            x = -x;
        if (y % 3 == 0)
            y = -y;

        buildings_position.push_back(glm::vec3(x, 0, y));

        int l, w, h;
        w = rand() % 5 + 2;
        l = rand() % 5 + 2;
        h = rand() % 40 + 1;
        buildings_size.push_back(glm::vec3(l, h, w));
    }

    {
        Mesh* mesh = new Mesh("corp");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank"), "corp2.fbx");
        tank.push_back(mesh);
    }

    {
        Mesh* mesh = new Mesh("turela");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank"), "turela.fbx");
        tank.push_back(mesh);
    }

    {
        Mesh* mesh = new Mesh("tun");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank"), "tun.fbx");
        tank.push_back(mesh);
    }

    {
        Mesh* mesh = new Mesh("senile");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank"), "senile_final.fbx");
        tank.push_back(mesh);
    }

    {
        Mesh* mesh = new Mesh("proiectil");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
        tank.push_back(mesh);
    }
    
    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cladire");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Light & material properties
    {
        lightPosition = camera1->GetTargetPosition();
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;
        nr_hit = 0;
    }

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.2, 0.2, 0.4, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    global_time += deltaTimeSeconds;
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.4f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(5, 5, 5));
        RenderSimpleMesh(meshes["plane"], shaders["LabShader"], modelMatrix, glm::vec3(0.2, 0.2, 0.2), -1);
    }
    
    for (int i = 0; i < random2; i++)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, buildings_position[i]);
        modelMatrix = glm::scale(modelMatrix, buildings_size[i]);
        RenderSimpleMesh(meshes["cladire"], shaders["LabShader"], modelMatrix, glm::vec3(1, 1, 1), -1);
    }

    //INAMICI

    for (int i = 0; i < random; i++) {
        if (finish == 0) {
            for (int j = 0; j < i; j++) {
                float distance = glm::distance(enemy_positions[i], enemy_positions[j]);
                if (distance < 7) {
                    glm::vec3 dif = enemy_positions[j] - enemy_positions[i];
                    dif.y = 0;
                    glm::vec3 p = (7 - distance) * glm::normalize(dif);
                    enemy_positions[i] += p * -1.f;
                    enemy_positions[j] += p * 1.f;
                }
            }

            for (int j = 0; j < random2; j++) {
                glm::vec3 ep = enemy_positions[i];

                glm::vec3 c1 = buildings_position[j] + glm::vec3(buildings_size[j].x / float(2), 0, buildings_size[j].z / float(2));
                glm::vec3 c2 = buildings_position[j] + glm::vec3(buildings_size[j].x / float(2), 0, -buildings_size[j].z / float(2));
                glm::vec3 c3 = buildings_position[j] + glm::vec3(-buildings_size[j].x / float(2), 0, -buildings_size[j].z / float(2));
                glm::vec3 c4 = buildings_position[j] + glm::vec3(-buildings_size[j].x / float(2), 0, buildings_size[j].z / float(2));

                if (distance(ep, c1) <= 5.f) {
                    glm::vec3 dif = buildings_position[j] - ep;
                    dif.y = 0;
                    glm::vec3 p = (5.f - distance(ep, c1)) * glm::normalize(dif);
                    enemy_positions[i] += p * -1.f;
                }
                if (distance(ep, c2) <= 5.f) {
                    glm::vec3 dif = buildings_position[j] - ep;
                    dif.y = 0;
                    glm::vec3 p = (5.f - distance(ep, c2)) * glm::normalize(dif);
                    enemy_positions[i] += p * -1.f;
                }

                if (distance(ep, c3) <= 5.f) {
                    glm::vec3 dif = buildings_position[j] - ep;
                    dif.y = 0;
                    glm::vec3 p = (5.f - distance(ep, c3)) * glm::normalize(dif);
                    enemy_positions[i] += p * -1.f;
                }

                if (distance(ep, c4) <= 5.f) {
                    glm::vec3 dif = buildings_position[j] - ep;
                    dif.y = 0;
                    glm::vec3 p = (5.f - distance(ep, c4)) * glm::normalize(dif);
                    enemy_positions[i] += p * -1.f;
                }
            }

            float d = glm::distance(enemy_positions[i], camera1->GetTargetPosition());
            if (d < 7) {
                glm::vec3 dif = camera1->GetTargetPosition() - enemy_positions[i];
                dif.y = 0;
                glm::vec3 p = (7 - d) * glm::normalize(dif);
                enemy_positions[i] += p * -1.f;
                camera1->position += p * 1.f;
                camera2->position += p * 1.f;
            }

            if (enemy_nr_hits[i] == 3 && glm::distance(my_bullet_pos, enemy_positions[i]) < 2) {
                shoot = 0;
                update_bullet = 0;
            }

            if (enemy_nr_hits[i] < 3) {
                time_to_choose[i] += deltaTimeSeconds * enemy_colors[i].x;

                int r = rand() % 10;
                while (r < 6)
                    r = rand() % 10;

                if (time_to_choose[i] > r) {
                    choice[i] = rand() % 4;
                    time_to_choose[i] = 0;
                }

                float depart = distance(enemy_positions[i], glm::vec3(0, 0, 0));
                if (depart > 100) {
                    glm::vec3 d1 = enemy_positions[i] - glm::vec3(0.02 * sin(enemy_tank_rotate[i]), 0, 0.02 * cos(enemy_tank_rotate[i]));
                    glm::vec3 d2 = enemy_positions[i] + glm::vec3(0.02 * sin(enemy_tank_rotate[i]), 0, 0.02 * cos(enemy_tank_rotate[i]));
                    if (distance(d1, glm::vec3(0, 0, 0)) > depart)
                        choice[i] = 1;
                    else if (distance(d2, glm::vec3(0, 0, 0)) > depart)
                        choice[i] = 0;

                }

                if (choice[i] == 0)
                    enemy_positions[i] -= glm::vec3(0.02 * sin(enemy_tank_rotate[i]), 0, 0.02 * cos(enemy_tank_rotate[i]));

                if (choice[i] == 1)
                    enemy_positions[i] += glm::vec3(0.02 * sin(enemy_tank_rotate[i]), 0, 0.02 * cos(enemy_tank_rotate[i]));

                if (choice[i] == 2)
                    enemy_tank_rotate[i] += 0.15f * deltaTimeSeconds;

                if (choice[i] == 3)
                    enemy_tank_rotate[i] -= 0.15f * deltaTimeSeconds;
                float deltaAngle = 1000;
                
                if (glm::distance(camera1->GetTargetPosition(), enemy_positions[i]) < 20) {
                    glm::vec3 target = camera1->GetTargetPosition() - enemy_positions[i];
                    target.y = 0;
                    target = glm::normalize(target);

                    float angle = enemy_turela_rotate[i] + enemy_tank_rotate[i];
                    glm::vec3 turret_vector = glm::vec3(glm::sin(angle), 0, glm::cos(angle));

                    deltaAngle = glm::asinf(glm::cross(target, turret_vector).y);
                    
                    if (deltaAngle > 0.05)
                        enemy_turela_rotate[i] += 0.05;
                    else if (deltaAngle < -0.05)
                        enemy_turela_rotate[i] -= 0.05;
                    else enemy_turela_rotate[i] += deltaAngle;
                }
                enemy_time[i] += deltaTimeSeconds;

                if (enemy_is_shooting[i] == 0 && enemy_time[i] > 4) {
                    if ((deltaAngle < 0.05 && deltaAngle > 0) || (deltaAngle > -0.05 && deltaAngle < 0))
                        enemy_is_shooting[i] = 1;
                    enemy_time[i] = 0;
                }

                if (enemy_is_shooting[i] == 1) {
                    enemy_old_rotate_tank[i] = enemy_tank_rotate[i];
                    enemy_old_rotate_turela[i] = enemy_turela_rotate[i];
                    enemy_is_shooting[i] = 2;
                }

                if (enemy_is_shooting[i] == 2) {
                    enemy_update_bullet[i] = enemy_update_bullet[i] + 15 * deltaTimeSeconds;
                }

                if (enemy_update_bullet[i] > 30) {
                    enemy_update_bullet[i] = 0;
                    enemy_is_shooting[i] = 0;
                }
            }
        }
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, enemy_positions[i]);
            modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(1, 0, 0));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::rotate(modelMatrix, -enemy_tank_rotate[i], glm::vec3(0, 0.1f, -5));
            RenderSimpleMesh(tank[0], shaders["LabShader"], modelMatrix, enemy_colors[i], enemy_nr_hits[i], i);
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, enemy_positions[i]);
            modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(1, 0, 0));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::rotate(modelMatrix, enemy_tank_rotate[i], glm::vec3(0, 0, 1));
            modelMatrix = glm::rotate(modelMatrix, enemy_turela_rotate[i], glm::vec3(0, 0, 1));
            RenderSimpleMesh(tank[1], shaders["LabShader"], modelMatrix, enemy_colors[i], enemy_nr_hits[i], i);
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, enemy_positions[i]);
            modelMatrix = glm::rotate(modelMatrix, enemy_tank_rotate[i], glm::vec3(0, 1, 0));
            modelMatrix = glm::rotate(modelMatrix, enemy_turela_rotate[i], glm::vec3(0, 1, 0));
            modelMatrix = glm::translate(modelMatrix, -(glm::vec3(1.72f, -1.6f, 0)));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f, 0.04f, 2));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(1.72f, -1.6f, 0));

            RenderSimpleMesh(tank[2], shaders["LabShader"], modelMatrix, enemy_colors[i], enemy_nr_hits[i], i);
        }

        if (enemy_is_shooting[i] == 2)
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, enemy_positions[i]);
            modelMatrix = glm::rotate(modelMatrix, enemy_old_rotate_tank[i], glm::vec3(0, 1, 0));
            modelMatrix = glm::rotate(modelMatrix, enemy_old_rotate_turela[i], glm::vec3(0, 1, 0));
            modelMatrix = glm::translate(modelMatrix, (glm::vec3(-0.045f, 1.98f, -4.9f - enemy_update_bullet[i])));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
            modelMatrix = glm::translate(modelMatrix, -glm::vec3(-0.045f, 1.98f, -4.9f - enemy_update_bullet[i]));
            RenderSimpleMesh(tank[4], shaders["LabShader"], modelMatrix, glm::vec3(0, 0, 0), -1);

            if (finish == 0) {
                enemy_bullet_pos[i] = glm::vec3(modelMatrix[3]);

                for (int j = 0; j < random2; j++) {
                    glm::vec3 ep = enemy_bullet_pos[i];

                    glm::vec3 c1 = buildings_position[j] + glm::vec3(buildings_size[j].x / float(2), 0, buildings_size[j].z / float(2));
                    glm::vec3 c2 = buildings_position[j] + glm::vec3(buildings_size[j].x / float(2), 0, -buildings_size[j].z / float(2));
                    glm::vec3 c3 = buildings_position[j] + glm::vec3(-buildings_size[j].x / float(2), 0, -buildings_size[j].z / float(2));
                    glm::vec3 c4 = buildings_position[j] + glm::vec3(-buildings_size[j].x / float(2), 0, buildings_size[j].z / float(2));

                    if (distance(ep, c1) <= 3.f) {
                        enemy_is_shooting[i] = 0;
                        enemy_update_bullet[i] = 0;
                    }
                    
                    if (distance(ep, c2) <= 3.f) {
                        enemy_is_shooting[i] = 0;
                        enemy_update_bullet[i] = 0;
                    }

                    if (distance(ep, c3) <= 3.f) {
                        enemy_is_shooting[i] = 0;
                        enemy_update_bullet[i] = 0;
                    }

                    if (distance(ep, c4) <= 3.f) {
                        enemy_is_shooting[i] = 0;
                        enemy_update_bullet[i] = 0;
                    }
                }

                if (glm::distance(camera1->GetTargetPosition(), enemy_bullet_pos[i]) < 3) {
                    enemy_is_shooting[i] = 0;
                    enemy_update_bullet[i] = 0;
                    nr_hit++;
                    if (nr_hit == 3) {
                        cout << "YOU LOST!" << endl;
                        finish = 1;
                    }
                }
            }
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, enemy_positions[i]);
            modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::rotate(modelMatrix, enemy_tank_rotate[i], glm::vec3(0, 1, 0));
            modelMatrix = glm::translate(modelMatrix, -(glm::vec3(-1.4f, 2.f, 0.f)));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.76f));
            modelMatrix = glm::translate(modelMatrix, (glm::vec3(-1.4f, 2.f, 0.f)));
            RenderSimpleMesh(tank[3], shaders["LabShader"], modelMatrix, enemy_colors[i], enemy_nr_hits[i], i);
        }
    }

    lightPosition = camera2->position + glm::vec3(0, 20, 0);
    
    if (finish == 0) {
        if (shoot == 1) {
            old_rotate_tank = tank_rotate;
            old_rotate_turela = rotate_turela;
            shoot = 2;
        }
        my_time += deltaTimeSeconds;
        if (shoot == 2) {
            update_bullet = update_bullet + 15 * deltaTimeSeconds;
        }

        if (update_bullet > 30) {
            update_bullet = 0;
            shoot = 0;
        }

        for (int j = 0; j < random2; j++) {
            glm::vec3 ep = camera1->GetTargetPosition();
            glm::vec3 bp = buildings_position[j];
            glm::vec3 bs = buildings_size[j];
            glm::vec3 c1 = buildings_position[j] + glm::vec3(buildings_size[j].x / float(2), 0, buildings_size[j].z / float(2));
            glm::vec3 c2 = buildings_position[j] + glm::vec3(buildings_size[j].x / float(2), 0, -buildings_size[j].z / float(2));
            glm::vec3 c3 = buildings_position[j] + glm::vec3(-buildings_size[j].x / float(2), 0, -buildings_size[j].z / float(2));
            glm::vec3 c4 = buildings_position[j] + glm::vec3(-buildings_size[j].x / float(2), 0, buildings_size[j].z / float(2));

            if (distance(ep, c1) <= 5.f) {
                glm::vec3 dif = buildings_position[j] - ep;
                dif.y = 0;
                glm::vec3 p = (5.f - distance(ep, c1)) * glm::normalize(dif);
                camera1->position += p * -1.f;
                camera2->position += p * -1.f;
            }
            if (distance(ep, c2) <= 5.f) {
                glm::vec3 dif = buildings_position[j] - ep;
                dif.y = 0;
                glm::vec3 p = (5.f - distance(ep, c2)) * glm::normalize(dif);
                camera1->position += p * -1.f;
                camera2->position += p * -1.f;
            }

            if (distance(ep, c3) <= 5.f) {
                glm::vec3 dif = buildings_position[j] - ep;
                dif.y = 0;
                glm::vec3 p = (5.f - distance(ep, c3)) * glm::normalize(dif);
                camera1->position += p * -1.f;
                camera2->position += p * -1.f;
            }

            if (distance(ep, c4) <= 5.f) {
                glm::vec3 dif = buildings_position[j] - ep;
                dif.y = 0;
                glm::vec3 p = (5.f - distance(ep, c4)) * glm::normalize(dif);
                camera1->position += p * -1.f;
                camera2->position += p * -1.f;
            }
        }
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera1->GetTargetPosition());
        modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(0, 0, 1));
        modelMatrix = glm::rotate(modelMatrix, -tank_rotate, glm::vec3(0, 0.1f, -5));
        RenderSimpleMesh(tank[0], shaders["LabShader"], modelMatrix, glm::vec3(0, 0.2, 0), nr_hit, -10);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera1->GetTargetPosition());
        modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(0, 0, 1));
        modelMatrix = glm::rotate(modelMatrix, tank_rotate, glm::vec3(0, 0, 1));
        modelMatrix = glm::rotate(modelMatrix, rotate_turela, glm::vec3(0, 0, 1));
        RenderSimpleMesh(tank[1], shaders["LabShader"], modelMatrix, glm::vec3(0, 1, 0), nr_hit, -10);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera1->GetTargetPosition());
        modelMatrix = glm::rotate(modelMatrix, tank_rotate, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, rotate_turela, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, -(glm::vec3(1.72f, -1.6f, 0)));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f, 0.04f, 2));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(1.72f, -1.6f, 0));
        
        RenderSimpleMesh(tank[2], shaders["LabShader"], modelMatrix, glm::vec3(1, 1, 1), nr_hit, -10);
    }

    if (shoot == 2)
    {
        glm::mat4 modelMatrix = glm::mat4(1); 
        modelMatrix = glm::translate(modelMatrix, camera1->GetTargetPosition());
        modelMatrix = glm::rotate(modelMatrix, old_rotate_tank, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, old_rotate_turela, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, (glm::vec3(-0.045f,1.98f, -4.9f - update_bullet)));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
        modelMatrix = glm::translate(modelMatrix, -glm::vec3(-0.045f, 1.98f, -4.9f - update_bullet));
        RenderSimpleMesh(tank[4], shaders["LabShader"], modelMatrix, glm::vec3(0, 0, 0), -1);
        
        if (finish == 0) {
            my_bullet_pos = glm::vec3(modelMatrix[3]);

            for (int j = 0; j < random2; j++) {
                glm::vec3 ep = my_bullet_pos;

                glm::vec3 c1 = buildings_position[j] + glm::vec3(buildings_size[j].x / float(2), 0, buildings_size[j].z / float(2));
                glm::vec3 c2 = buildings_position[j] + glm::vec3(buildings_size[j].x / float(2), 0, -buildings_size[j].z / float(2));
                glm::vec3 c3 = buildings_position[j] + glm::vec3(-buildings_size[j].x / float(2), 0, -buildings_size[j].z / float(2));
                glm::vec3 c4 = buildings_position[j] + glm::vec3(-buildings_size[j].x / float(2), 0, buildings_size[j].z / float(2));

                if (distance(ep, c1) <= 3.f) {
                    shoot = 0;
                    update_bullet = 0;
                }
                if (distance(ep, c2) <= 3.f) {
                    shoot = 0;
                    update_bullet = 0;
                }

                if (distance(ep, c3) <= 3.f) {
                    shoot = 0;
                    update_bullet = 0;
                }

                if (distance(ep, c4) <= 3.f) {
                    shoot = 0;
                    update_bullet = 0;
                }
            }

            for (int i = 0; i < random; i++) {
                if (glm::distance(enemy_positions[i], my_bullet_pos) < 3) {
                    shoot = 0;
                    update_bullet = 0;

                    if (enemy_nr_hits[i] < 3) {
                        enemy_nr_hits[i]++;

                        if (enemy_nr_hits[i] == 3) {
                            points++;
                        }
                    }
                }
            }
        }
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix,camera1->GetTargetPosition());
        modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, tank_rotate, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, -(glm::vec3(-1.4f,2.f, 0.f)));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.76f));
        modelMatrix = glm::translate(modelMatrix, (glm::vec3(-1.4f, 2.f, 0.f)));
        RenderSimpleMesh(tank[3], shaders["LabShader"], modelMatrix, glm::vec3(1, 1, 1), nr_hit, -10);
    }

    if (global_time > 90 && finish == 0) {
        if (points == 0)
            cout << "You are a good runner! You've got the unmatched score of 0 points!" << endl;
        else cout << "You win! You got " << points << " points! " << endl;
        finish = 1;
    }
}


void Tema2::FrameEnd()
{
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera1->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, const int hp, const int type)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light & material properties
    int light_pos = glGetUniformLocation(shader->program, "light_position");
    glUniform3fv(light_pos, 1, glm::value_ptr(lightPosition));

    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();

    int eye_pos = glGetUniformLocation(shader->program, "eye_position");
    glUniform3fv(eye_pos, 1, glm::value_ptr(eyePosition));


    int place = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(place, materialShininess);

    place = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(place, materialKd);

    place = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(place, materialKs);

    place = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(place, 1, glm::value_ptr(color));

    place = glGetUniformLocation(shader->program, "hp");
    glUniform1i(place, hp);

    place = glGetUniformLocation(shader->program, "type");
    glUniform1i(place, type);


    // Bind model matrix
    int loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = camera2->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::OnInputUpdate(float deltaTime, int mods)
{

    float cameraSpeed = 4.0f;
    if (finish == 0) {
        if (window->KeyHold(GLFW_KEY_W)) {
            glm::vec3 pos = camera1->position;
            camera1->MoveForward(deltaTime * cameraSpeed);
            camera2->position = camera2->position + (camera1->position - pos);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            glm::vec3 pos = camera1->position;
            camera1->MoveForward(-deltaTime * cameraSpeed);
            camera2->position = camera2->position + (camera1->position - pos);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            tank_rotate += deltaTime;
            camera1->RotateThirdPerson_OY(deltaTime);
            camera2->RotateThirdPerson_OY(deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            tank_rotate -= deltaTime;
            camera1->RotateThirdPerson_OY(-deltaTime);
            camera2->RotateThirdPerson_OY(-deltaTime);
        }
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT && finish == 0)) {
        if (nr_hit < 3) {
            ok = 1;
            float sensivityOX = 0.0015f;
            float sensivityOY = 0.0015f;
            camera2->RotateThirdPerson_OX(sensivityOX * -deltaY);
            camera2->RotateThirdPerson_OY(sensivityOY * -deltaX);
            //  rotate_turela -= sensivityOX * deltaX;
            camera2->forward = (camera1->GetTargetPosition() - camera2->position) / glm::vec3(10, 10, 10);
        }
    }
    else {
        if (ok == 1) {
            ok = 0;
        }
        else rotate_turela += 0.005f * deltaX;
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (shoot == 0 && my_time > 2)
        if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
            shoot = 1;
            my_time = 0;
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
