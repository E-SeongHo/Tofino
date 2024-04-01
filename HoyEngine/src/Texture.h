#pragma once

#include <string>

#include "Helper.h"

enum class TextureType
{
	DEFAULT,
	ALBEDO,
	NORMAL,
	HEIGHT,
	CUBE
};

class Texture
{
public:
	Texture() = default;
	Texture(ComPtr<ID3D11Device>& device, const std::string filename, TextureType type);
	Texture(const Texture& other);
	~Texture() = default;

	bool Load(ComPtr<ID3D11Device>& device, const std::string filename, TextureType type);
	bool IsLoaded() const;
	std::string GetName() const;

	ComPtr<ID3D11ShaderResourceView>& GetSRV();

private:
	std::string m_filename;
	bool m_isLoaded = false;
	ComPtr<ID3D11Texture2D> m_texture;
	ComPtr<ID3D11ShaderResourceView> m_srv;
};

