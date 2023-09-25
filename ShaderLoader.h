#pragma once

#include <wrl.h>

#include "Debug.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <iostream>

using Microsoft::WRL::ComPtr;
class ShaderLoader
{
public:
	static void LoadVertexShader(ComPtr<ID3D11Device>& device, const std::wstring& filename,
		ComPtr<ID3D11VertexShader>& vertexShader, ComPtr<ID3DBlob>& vertexBlob);
	static void LoadPixelShader(ComPtr<ID3D11Device>& device, const std::wstring& filename,
		ComPtr<ID3D11PixelShader>& pixelShader);

};