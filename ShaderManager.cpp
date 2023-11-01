
#include <vector>

#include "ShaderManager.h"
#include "ShaderLoader.h"


using namespace std;
void ShaderManager::InitShaders(ComPtr<ID3D11Device>& device)
{
	ComPtr<ID3DBlob> vertexBlob;
	ShaderLoader::LoadVertexShader(device, L"BasicVS.hlsl", basicVS, vertexBlob);
	ShaderLoader::LoadPixelShader(device, L"BasicPS.hlsl", basicPS);

	// Create the input layout object
	vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3 + 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 + 4 * 3 + 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	device->CreateInputLayout(inputElements.data(), UINT(inputElements.size()), vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), &basicInputLayout);
	
	// NormalVS uses same input layout
	ShaderLoader::LoadVertexShader(device, L"NormalVS.hlsl", normalVS, vertexBlob);
	ShaderLoader::LoadGeometryShader(device, L"NormalGS.hlsl", normalGS);
	ShaderLoader::LoadPixelShader(device, L"NormalPS.hlsl", normalPS);

	// EnvMap uses same input layout
	// vertexBlob.Reset();
	ShaderLoader::LoadVertexShader(device, L"EnvMapVS.hlsl", envMapVS, vertexBlob);
	ShaderLoader::LoadPixelShader(device, L"EnvMapPS.hlsl", envMapPS);

	// CopyVS uses same input layout
	ShaderLoader::LoadVertexShader(device, L"CopyVS.hlsl", copyVS, vertexBlob);
	ShaderLoader::LoadPixelShader(device, L"ToneMappingPS.hlsl", toneMappingPS);

}
