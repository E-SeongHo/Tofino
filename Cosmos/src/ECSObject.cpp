
#include "ECSObject.h"

#include "MeshLoader.h"

using namespace Tofino;

ECSObject::ECSObject(Tofino::Scene* scene, const std::string name, const bool isHittable)
	:Object(scene, name, isHittable)
{
    AddComponent<TransformComponent>();

	//AddComponent<MeshComponent>();
    //GetComponent<MeshComponent>().Meshes = MeshLoader::LoadSphere();
	//SetAllMeshMaterialFactors(Vector4(0.0f), 0.1f, 0.5f);
}
