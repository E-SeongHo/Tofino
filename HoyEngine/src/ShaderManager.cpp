
#include <vector>

#include "ShaderManager.h"
#include "ShaderLoader.h"

namespace Tofino
{

	void ShaderManager::InitShaders(ComPtr<ID3D11Device>& device)
	{
		ComPtr<ID3DBlob> vertexBlob;
		ShaderLoader::LoadVertexShader(device, L"../HoyEngine/Shaders/BasicVS.hlsl", basicVS, vertexBlob);
		ShaderLoader::LoadPixelShader(device, L"../HoyEngine/Shaders/BasicPS.hlsl", basicPS);

		// Create the input layout object
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		device->CreateInputLayout(inputElements.data(), UINT(inputElements.size()), vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), &basicInputLayout);

		// NormalVS uses same input layout
		ShaderLoader::LoadVertexShader(device, L"../HoyEngine/Shaders/NormalVS.hlsl", normalVS, vertexBlob);
		ShaderLoader::LoadGeometryShader(device, L"../HoyEngine/Shaders/NormalGS.hlsl", normalGS);
		ShaderLoader::LoadPixelShader(device, L"../HoyEngine/Shaders/NormalPS.hlsl", normalPS);

		// ColliderVS uses same input layout
		ShaderLoader::LoadVertexShader(device, L"../HoyEngine/Shaders/ColliderVS.hlsl", colliderVS, vertexBlob);
		ShaderLoader::LoadPixelShader(device, L"../HoyEngine/Shaders/ColliderPS.hlsl", colliderPS);

		// EnvMap uses same input layout
		// vertexBlob.Reset();
		ShaderLoader::LoadVertexShader(device, L"../HoyEngine/Shaders/EnvMapVS.hlsl", envMapVS, vertexBlob);
		ShaderLoader::LoadPixelShader(device, L"../HoyEngine/Shaders/EnvMapPS.hlsl", envMapPS);

		// CopyVS uses same input layout
		ShaderLoader::LoadVertexShader(device, L"../HoyEngine/Shaders/CopyVS.hlsl", copyVS, vertexBlob);
		ShaderLoader::LoadPixelShader(device, L"../HoyEngine/Shaders/ToneMappingPS.hlsl", toneMappingPS);

	}
}