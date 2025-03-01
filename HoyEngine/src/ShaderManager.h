#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h> 

namespace Tofino
{
	using Microsoft::WRL::ComPtr;

	class ShaderManager
	{

	public:
		static ShaderManager& GetInstance()
		{
			static ShaderManager s;
			return s;
		}

		void InitShaders(ComPtr<ID3D11Device>& device);

	public:
		ComPtr<ID3D11VertexShader> basicVS;
		ComPtr<ID3D11PixelShader> basicPS;
		ComPtr<ID3D11InputLayout> basicInputLayout;

		ComPtr<ID3D11VertexShader> normalVS;
		ComPtr<ID3D11GeometryShader> normalGS;
		ComPtr<ID3D11PixelShader> normalPS;

		ComPtr<ID3D11VertexShader> colliderVS;
		ComPtr<ID3D11PixelShader> colliderPS;

		ComPtr<ID3D11VertexShader> envMapVS;
		ComPtr<ID3D11PixelShader> envMapPS;

		ComPtr<ID3D11VertexShader> copyVS;
		ComPtr<ID3D11PixelShader> toneMappingPS;

	private:
		ShaderManager() {}
		ShaderManager(const ShaderManager&) = delete;
		ShaderManager& operator=(const ShaderManager&) = delete;

	};
}