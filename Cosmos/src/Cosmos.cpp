#include "Cosmos.h"
#include <vector>
#include <string>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

using namespace std;
using namespace DirectX::SimpleMath;

Cosmos::Cosmos()
{
    m_scene = new Scene();
}

Cosmos::~Cosmos()
{
    Application::~Application();
    delete m_scene;
}   

bool Cosmos::Init()
{
    if (!Application::Init()) return false;

    DesignScenes();

    SetCurrentScene(m_scene);
    return true;
}
void Cosmos::DesignScenes()
{
    Camera* cam = new Camera();
    cam->SetAspect(static_cast<float>(m_width) / m_height);

    m_scene->SetCamera(cam);

    Geometry* sphere = new Sphere("Test Sphere", true);
    sphere->LoadGeometry();
    const string texDirectory = "./Assets/Texture/wet-mossy-rocks-ue/";
    //sphere->AttachAlbedoTexture(texDirectory + "wet-mossy-rocks_albedo.png");
    //sphere->AttachNormalTexture(texDirectory + "wet-mossy-rocks_normal-dx.png");
    //sphere->AttachHeightTexture(texDirectory + "wet-mossy-rocks_height.png");
    sphere->SetMaterials(0.5f, 0.1f);
    m_scene->AddObject(sphere);

    Model* spaceship = new Model("Space Station", true);
    spaceship->LoadModel("D:/Workspace/3DModels/sci-fi-space-station/SpaceStation.fbx", 10.0f);
    spaceship->AttachAlbedoTexture(spaceship->m_directory + "SpaceStationParts2_BaseColor.png", 0);
    spaceship->SetMaterials(0.1f, 0.7f);
    m_scene->AddObject(spaceship);

    Model* aircraft = new Model("E-45 Aircraft", true);
    aircraft->LoadModel("D:/Workspace/3DModels/E-45-Aircraft/E 45 Aircraft_obj.obj", 3.0f);
    aircraft->AttachAlbedoTexture(aircraft->m_directory + "E-45 _col.jpg", 0);
    aircraft->AttachNormalTexture(aircraft->m_directory + "E-45-nor_1.jpg", 0);
    aircraft->AttachHeightTexture(aircraft->m_directory + "E-45-nor_1.jpg", 0);
    aircraft->AttachAlbedoTexture("", 1);
    aircraft->AttachNormalTexture(aircraft->m_directory + "E-45_glass_nor_.jpg", 1);
    aircraft->AttachHeightTexture(aircraft->m_directory + "E-45_glass_nor_.jpg", 1);
    aircraft->SetMaterials(0.1f, 0.5f);
    m_scene->AddObject(aircraft);

    EnvMap* skybox = new EnvMap("Universe", L"./Assets/Cubemap/HDRI/PlanetaryEarth/");
    m_scene->AddSkybox(skybox);

    Light* light = new Light();
    //light->type = LightType::DIRECTIONAL_LIGHT;
    light->pos = DirectX::SimpleMath::Vector3(-0.3f, 0.2f, -2.0f);
    light->strength = 0.5f;
    light->direction = DirectX::SimpleMath::Vector3(0.1f, -0.2f, 1.0f);
    light->coefficient = 2.2f;
    m_scene->AddLight(light);

    GlobalBuffer* sceneGlobal = new GlobalBuffer();
    sceneGlobal->view = cam->GetViewMatrix().Transpose();
    sceneGlobal->projection = cam->GetProjectionMatrix().Transpose();
    sceneGlobal->eye = cam->GetOrigin();
    sceneGlobal->light = *light;
    m_scene->AddSceneBuffer(sceneGlobal);

}

void Cosmos::Update(float deltaTime)
{
    Application::Update(deltaTime);

}

void Cosmos::RenderGUI()
{
    ImGui::NewFrame();
    ImGui::Begin("Scene Control");

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("Redering Camera"))
    {
        Vector3 origin = m_scene->GetCamera()->GetOrigin();
        Vector3 direction = m_scene->GetCamera()->GetDirection();

        ImGui::SeparatorText("Origin");
        ImGui::Text("X: (%d) Y: (%d), Z: (%d)", (int)origin.x, (int)origin.y, (int)origin.z);
        ImGui::SeparatorText("Direction");
        ImGui::Text("X: (%.2f) Y: (%.2f), Z: (%.2f)", direction.x, direction.y, direction.z);
        ImGui::Text(" ");
    }

    /*if (ImGui::CollapsingHeader("Rendering Options"))
    {
        ImGui::Checkbox("Show WireFrame", &m_wireRendering);
    }*/

    /*if (ImGui::CollapsingHeader("Light"))
    {
        ImGui::SliderFloat3("Origin", &m_globalConstBufferCPU.light.pos.x, -200.0f, 200.0f);
        ImGui::SliderFloat("Strength", &m_globalConstBufferCPU.light.strength, 0.0f, 10.0f);
        ImGui::SliderFloat3("Direction", &m_globalConstBufferCPU.light.direction.x, -10.0f, 10.0f);
        ImGui::SliderFloat("Coefficient", &m_globalConstBufferCPU.light.coefficient, 0.0f, 10.0f);
    }*/

    // Setting Objects
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    ImGuiColorEditFlags flags = ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel
        | ImGuiColorEditFlags_AlphaBar;

    if (ImGui::CollapsingHeader("Scene Objects"))
    {
        for (auto& object : m_scene->GetAllSceneObjects())
        {
            int updateFlag = 0;
            if (ImGui::TreeNode(object->m_name.c_str()))
            {
                if (ImGui::SliderFloat3("Position", &object->m_transform.Location.x, -100.0f, 100.0f))
                {
                    object->SetLocation(object->m_transform.Location);
                    updateFlag++;
                }
                if (ImGui::SliderFloat3("Rotation", &object->m_transform.Rotation.x, -360.0f, 360.0f))
                {
                    object->SetRotation(object->m_transform.Rotation);
                    updateFlag++;
                }
                if (ImGui::SliderFloat3("Scale", &object->m_transform.Scale.x, 0.0f, 10.0f))
                {
                    object->SetScale(object->m_transform.Scale);
                    updateFlag++;
                }
                if (!object->m_modelBufferCPU.activeAlbedoMap)
                {
                    updateFlag += ImGui::ColorPicker4("Albedo", &object->m_modelBufferCPU.material.albedo.x, flags, NULL);
                }
                updateFlag += ImGui::SliderFloat("Roughness", &object->m_modelBufferCPU.material.roughness, 0.0f, 1.0f);
                updateFlag += ImGui::SliderFloat("Metaillic", &object->m_modelBufferCPU.material.metallic, 0.0f, 1.0f);

                updateFlag += ImGui::CheckboxFlags("Albedo Map", &object->m_modelBufferCPU.activeAlbedoMap, 1);
                updateFlag += ImGui::CheckboxFlags("Height Map", &object->m_modelBufferCPU.activeHeightMap, 1);
                updateFlag += ImGui::CheckboxFlags("Normal Map", &object->m_modelBufferCPU.activeNormalMap, 1);

                if (updateFlag) object->m_updateFlag = true;

                ImGui::TreePop();
            }
        }
    }

    ImGui::End();
}

Application* CreateApplication()
{
    return new Cosmos();
}
