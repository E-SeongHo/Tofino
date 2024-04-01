#pragma once

#include <iostream>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h> 

using Microsoft::WRL::ComPtr;

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr)) 
	{
		// Set a breakpoint on this line to catch Win32 API errors.
		throw std::exception();
	}
}