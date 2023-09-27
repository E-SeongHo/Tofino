#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h> 

using Microsoft::WRL::ComPtr;

// Singleton
class ShaderManager
{
private:
	ShaderManager() {}
	ShaderManager(const ShaderManager&) = delete;
	ShaderManager& operator=(const ShaderManager&) = delete;

public:
	static ShaderManager& GetInstance()
	{
		static ShaderManager s;
		return s;
	}

public:
	ComPtr<ID3D11VertexShader> basicVS;
	ComPtr<ID3D11PixelShader> basicPS;
	ComPtr<ID3D11InputLayout> basicInputLayout;

	ComPtr<ID3D11VertexShader> envMapVS;
	ComPtr<ID3D11PixelShader> envMapPS;

	ComPtr<ID3D11VertexShader> copyVS;
	ComPtr<ID3D11PixelShader> toneMappingPS;

	void InitShaders(ComPtr<ID3D11Device>& device);
};