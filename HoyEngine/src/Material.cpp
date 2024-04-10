
#include "Material.h"
#include <iostream>

namespace Tofino
{
	Material::Material(UINT bindFlag, UINT slot)
		: m_status(bindFlag, slot)
	{
	}

	void Material::Init(ComPtr<ID3D11Device>& device)
	{
		m_status.Init(device);
	}

	void Material::Update(ComPtr<ID3D11DeviceContext>& context)
	{
		MaterialStatus& factors = m_status.GetData();

		m_status.Update(context);
	}

	void Material::Bind(ComPtr<ID3D11DeviceContext>& context) const
	{
		m_status.Bind(context);

		context->VSSetShaderResources(0, 1, m_heightMap.GetSRV().GetAddressOf());

		ID3D11ShaderResourceView* textures[] = { m_albedoMap.GetSRV().Get(), m_normalMap.GetSRV().Get() };
		context->PSSetShaderResources(0, 2, textures);
	}

	// Should be set ahead of Object::Init() called
	void Material::SetFactors(Vector4 baseColor, float roughness, float metallic)
	{
		MaterialStatus& factors = m_status.GetData();
		factors.baseColor = baseColor;
		factors.roughness = roughness;
		factors.metallic = metallic;
	}

	void Material::SetAlbedoMap(const Texture& texture)
	{
		if (!texture.IsLoaded())
		{
			std::cout << texture.GetName() << " is not loaded" << std::endl;
			return;
		}

		m_albedoMap = texture;
		m_status.GetData().hasAlbedoMap = 1;
	}

	void Material::SetNormalMap(const Texture& texture)
	{
		if (!texture.IsLoaded())
		{
			std::cout << texture.GetName() << " is not loaded" << std::endl;
			return;
		}

		m_normalMap = texture;
		m_status.GetData().hasNormalMap = 1;
	}

	void Material::SetHeightMap(const Texture& texture)
	{
		if (!texture.IsLoaded())
		{
			std::cout << texture.GetName() << " is not loaded" << std::endl;
			return;
		}

		m_heightMap = texture;
		m_status.GetData().hasHeightMap = 1;
	}

	MaterialStatus& Material::GetMaterialStatus()
	{
		return m_status.GetData();
	}
}