#pragma once

#include "SimpleMath.h"
#include "Texture.h"
#include "ConstantBuffer.h"

namespace Tofino
{
	// Abstraction of Material ( to upload GPU )
	struct MaterialStatus
	{
		Vector4 baseColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		float roughness = 0.1f;
		float metallic = 0.1f;
		float padding[2];

		int hasAlbedoMap = 0;
		int hasNormalMap = 0;
		int hasHeightMap = 0; 
		int padding2;
	};

	// Every Mesh own Material
	class Material
	{
	public:
		Material() = default;
		Material(UINT constBufferFlag, UINT slot);
		~Material() = default;

		void Init(ComPtr<ID3D11Device>& device);
		void Update(ComPtr<ID3D11DeviceContext>& context);
		void Bind(ComPtr<ID3D11DeviceContext>& context) const;

		void SetFactors(Vector4 baseColor, float roughness, float metallic);

		// Shallow copy
		void SetAlbedoMap(const Texture& texture);
		void SetNormalMap(const Texture& texture);
		void SetHeightMap(const Texture& texture);

		MaterialStatus& GetMaterialStatus();

	private:
		ConstantBuffer<MaterialStatus> m_status;

		Texture m_albedoMap;
		Texture m_normalMap;
		Texture m_heightMap;
	};
}