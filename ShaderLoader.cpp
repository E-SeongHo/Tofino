#include "ShaderLoader.h"


// Note : CreateInputLayout requires the vertex shader in addition to the vertex input layout.
// because it cross checks the two to make sure the given vertex layout(parameter로 전달된) contains all the information 
// vertex shader asks for.
// https://www.walkerb.net/blog/dx-3/
void ShaderLoader::LoadVertexShader(ComPtr<ID3D11Device>& device, const std::wstring& filename, 
	ComPtr<ID3D11VertexShader>& vertexShader, ComPtr<ID3DBlob>& vertexBlob )
{
	ComPtr<ID3DBlob> errorBlob;
	
	if (FAILED(D3DCompileFromFile(filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", 0, 0, &vertexBlob, &errorBlob)))
	{
		if (errorBlob) {
			std::cout << "Vertex shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
		}
	}

	ThrowIfFailed(device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), NULL, &vertexShader));
}

void ShaderLoader::LoadPixelShader(ComPtr<ID3D11Device>& device, const std::wstring& filename, 
	ComPtr<ID3D11PixelShader>& pixelShader)
{
	ComPtr<ID3DBlob> pixelBlob;
	ComPtr<ID3DBlob> errorBlob;

	if (FAILED(D3DCompileFromFile(filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", 0, 0, &pixelBlob, &errorBlob)))
	{
		if (errorBlob) {
			std::cout << "Pixel shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
		}
	}
	ThrowIfFailed(device->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), NULL, &pixelShader));
}


void ShaderLoader::LoadGeometryShader(ComPtr<ID3D11Device>& device, const std::wstring& filename, ComPtr<ID3D11GeometryShader>& geometryShader)
{
	ComPtr<ID3DBlob> geometryBlob;
	ComPtr<ID3DBlob> errorBlob;

	if (FAILED(D3DCompileFromFile(filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "gs_5_0", 0, 0, &geometryBlob, &errorBlob)))
	{
		if (errorBlob) {
			std::cout << "Pixel shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
		}
	}
	ThrowIfFailed(device->CreateGeometryShader(geometryBlob->GetBufferPointer(), geometryBlob->GetBufferSize(), NULL, &geometryShader));
}