#pragma once

#include <d3d11.h>
#include <wrl.h>

namespace Tofino
{
	using Microsoft::WRL::ComPtr;

	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			// Set a breakpoint on this line to catch Win32 API errors.
			throw std::exception();
		}
	}
}