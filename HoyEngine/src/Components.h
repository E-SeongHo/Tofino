#pragma once

#include "SimpleMath.h"
#include "Mesh.h"

namespace Tofino
{
	using ObjectID = uint32_t;

	struct TransformComponent
	{
		Vector3 Translation = { 0.0f, 0.0f, 0.0f };
		Vector3 Rotation = { 0.0f, 0.0f, 0.0f };
		Vector3 Scale = { 1.0f, 1.0f, 1.0f };
	};

	struct MeshComponent
	{
		std::vector<Mesh> Meshes;
	};


	// Template meta programming
	// Defining Component Group and expands with parameter pack
	template<typename... Component>
	struct ComponentGroup
	{
	};

	using AllComponents = ComponentGroup<TransformComponent, MeshComponent>;
}