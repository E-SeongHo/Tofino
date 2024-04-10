#include "Cosmos.h"
#include <vector>
#include <string>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "Helper.h"

using namespace Tofino;
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
    {
        Geometry* sphere = new Sphere("Test Sphere", true);
        sphere->LoadGeometry();
        sphere->SetAllMeshMaterialFactors(Vector4(0.0f), 0.1f, 0.5f);
        m_scene->AddObject(sphere);
    }

    {
        Model* spaceship = new Model("Space Station", true);
        spaceship->LoadModel("D:/Workspace/3DModels/sci-fi-space-station/SpaceStation.fbx", 0.005f);
        //spaceship->SetScale({ 0.01f, 0.01f, 0.01f });

        // Since Assimp failed to find appropriate textures
        {
            Texture bodyAlbedo2(
                RendererDevice,
                spaceship->m_directory + "SpaceStationParts2_BaseColor.png",
                TextureType::ALBEDO);

            Texture bodyAlbedo1(
                RendererDevice,
                spaceship->m_directory + "SpaceStationParts1_BaseColor.png",
                TextureType::ALBEDO);

            spaceship->GetMeshes()[0].GetMaterial().SetAlbedoMap(bodyAlbedo2);
            spaceship->GetMeshes()[1].GetMaterial().SetAlbedoMap(bodyAlbedo1);

            /*std::vector<Mesh> meshes = spaceship->m_meshes;
            spaceship->m_meshes.clear();
            spaceship->m_meshes.push_back(meshes[2]);*/
        }

        spaceship->SetAllMeshMaterialFactors(Vector4(0.0f), 0.1f, 0.7f);
        spaceship->GetMeshes()[0].SetMaterialFactors(Vector4(0.0f), 0.1, 0.9f);
        m_scene->AddObject(spaceship);
    }

    {
        Model* aircraft = new Model("E-45 Aircraft", true);
        aircraft->LoadModel("D:/Workspace/3DModels/E-45-Aircraft/E 45 Aircraft_obj.obj", 3.0f);
        
        // Since Assimp failed to find appropriate textures
        {
            Texture bodyAlbedo(RendererDevice, aircraft->m_directory + "E-45 _col.jpg", TextureType::ALBEDO);
            Texture bodyNormal(RendererDevice, aircraft->m_directory + "E-45-nor_1.jpg", TextureType::NORMAL);

            Texture glassNormal(RendererDevice, aircraft->m_directory + "E-45_glass_nor_.jpg", TextureType::NORMAL);

            // body
            aircraft->GetMeshes()[0].GetMaterial().SetAlbedoMap(bodyAlbedo);
            aircraft->GetMeshes()[0].GetMaterial().SetNormalMap(bodyNormal);
            aircraft->GetMeshes()[0].GetMaterial().GetMaterialStatus().hasHeightMap = 0;

            // glass
            aircraft->GetMeshes()[1].GetMaterial().GetMaterialStatus().hasAlbedoMap = 0;
            aircraft->GetMeshes()[1].GetMaterial().SetNormalMap(glassNormal);
            aircraft->GetMeshes()[1].GetMaterial().GetMaterialStatus().hasHeightMap = 0;
        }

        aircraft->SetAllMeshMaterialFactors(Vector4(0.0f), 0.1f, 0.7f);
        m_scene->AddObject(aircraft);
    }

    {
        EnvMap* skybox = new EnvMap("Universe", "./Assets/Cubemap/HDRI/PlanetaryEarth/");
        m_scene->AddSkybox(skybox);
    }

    {
        Light* light = new Light();
        //light->type = LightType::DIRECTIONAL_LIGHT;
        light->pos = Vector3(-0.3f, 0.2f, -2.0f);
        light->strength = 0.5f;
        light->direction = Vector3(0.1f, -0.2f, 1.0f);
        light->coefficient = 2.2f;
        m_scene->AddLight(light);
    }


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
    {\
        for (auto& object : m_scene->GetAllSceneObjects())
        {
            int updateFlag = 0;
            if (ImGui::TreeNode(object->GetName().c_str()))
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
                
                updateFlag += ImGui::CheckboxFlags("Albedo Map", &object->GetConstBuffer().GetData().activeAlbedoMap, 1);
                updateFlag += ImGui::CheckboxFlags("Height Map", &object->GetConstBuffer().GetData().activeHeightMap, 1);
                updateFlag += ImGui::CheckboxFlags("Normal Map", &object->GetConstBuffer().GetData().activeNormalMap, 1);

                if (updateFlag) object->SetUpdateFlag(true);

                std::vector<Mesh>& meshes = object->GetMeshes();
                for (int i = 0; i < meshes.size(); i++)
                {
                    int meshUpdateFlag = 0;
                    if (ImGui::TreeNode((std::string("Part ") + std::to_string(i)).c_str()))
                    {
                        auto& materialData = meshes[i].GetMaterial().GetMaterialStatus();
                        if (!object->GetConstBuffer().GetData().activeAlbedoMap)
                        {
                            meshUpdateFlag += ImGui::ColorPicker4("BaseColor", &materialData.baseColor.x, flags, NULL);
                        }
                        meshUpdateFlag += ImGui::SliderFloat("Roughness", &materialData.roughness, 0.0f, 1.0f);
                        meshUpdateFlag += ImGui::SliderFloat("Metaillic", &materialData.metallic, 0.0f, 1.0f);
                        
                        ImGui::TreePop();
                    }

                    if (meshUpdateFlag)
                    {
                        meshes[i].SetUpdateFlag(true);
                        object->SetUpdateFlag(true);
                    }

                }
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
