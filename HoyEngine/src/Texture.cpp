
#define _CRT_SECURE_NO_WARNINGS 

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxtk/DDSTextureLoader.h>
#include <vector>

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

using namespace DirectX;
using namespace std;

Texture::Texture(ComPtr<ID3D11Device>& device, const std::string filename, TextureType type)
{
    m_isLoaded = Load(device, filename, type);
}

Texture::Texture(const Texture& other)
    : m_filename(other.m_filename), m_texture(other.m_texture), m_srv(other.m_srv)
{
}

bool Texture::Load(ComPtr<ID3D11Device>& device, const std::string filename, TextureType type)
{
    m_filename = filename;
    string format = filename.substr(filename.rfind('.') + 1);

	if (format == "dds")
	{
		wstring wfilename = wstring().assign(filename.begin(), filename.end());
		
		// https://github.com/microsoft/DirectXTK/wiki/DDSTextureLoader
		ThrowIfFailed(CreateDDSTextureFromFileEx(
			device.Get(), wfilename.c_str(), 0, D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE, 0,
			static_cast<UINT>(type), // TextureType :: CUBE or DEFAULT
			DDS_LOADER_FLAGS(false), (ID3D11Resource**)m_texture.GetAddressOf(),
			m_srv.GetAddressOf(), nullptr));

        m_isLoaded = true;
	}
	else
	{
        int width = 0, height = 0;
        std::vector<uint8_t> image;

        /*
        if texture's format sRGB, then need gamma correction for exact calculation of lighting
        DX11 automatically gamma decode for SRGB format.

        and like normal, height map texture, if its format is not sRGB, should not be gamma decoded.
        means which texture doesn't represent color, but just number.
        */
        bool gammaDecode = 
            type == TextureType::NORMAL || type == TextureType::HEIGHT ? true : false;
         
        DXGI_FORMAT pixelFormat =
            gammaDecode ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM;

        int channels;
        unsigned char* img =
            stbi_load(filename.c_str(), &width, &height, &channels, 0);

        // assert(channels == 4);

        //cout << filename << " " << width << " " << height << " " << channels << endl;
        if (filename.empty())
        {
            cout << "Empty file name: " << endl;
            return false;
        }

        // make 4 channels and copy
        image.resize(width * height * 4);

        if (channels == 1) {
            for (size_t i = 0; i < width * height; i++) {
                uint8_t g = img[i * channels + 0];
                for (size_t c = 0; c < 4; c++) {
                    image[4 * i + c] = g;
                }
            }
        }
        else if (channels == 2) {
            for (size_t i = 0; i < width * height; i++) {
                for (size_t c = 0; c < 2; c++) {
                    image[4 * i + c] = img[i * channels + c];
                }
                image[4 * i + 2] = 255;
                image[4 * i + 3] = 255;
            }
        }
        else if (channels == 3) {
            for (size_t i = 0; i < width * height; i++) {
                for (size_t c = 0; c < 3; c++) {
                    image[4 * i + c] = img[i * channels + c];
                }
                image[4 * i + 3] = 255;
            }
        }
        else if (channels == 4) {
            for (size_t i = 0; i < width * height; i++) {
                for (size_t c = 0; c < 4; c++) {
                    image[4 * i + c] = img[i * channels + c];
                }
            }
        }
        else {
            std::cout << "Cannot read " << channels << " channels" << endl;
            return false;
        }

        D3D11_TEXTURE2D_DESC txtDesc = {};
        txtDesc.Width = width;
        txtDesc.Height = height;
        txtDesc.MipLevels = txtDesc.ArraySize = 1;
        txtDesc.Format = pixelFormat;
        txtDesc.SampleDesc.Count = 1;
        txtDesc.Usage = D3D11_USAGE_IMMUTABLE;
        txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        // Fill in the subresource data.
        D3D11_SUBRESOURCE_DATA InitData;
        InitData.pSysMem = image.data();
        InitData.SysMemPitch = txtDesc.Width * sizeof(uint8_t) * 4;
        // InitData.SysMemSlicePitch = 0;

        device->CreateTexture2D(&txtDesc, &InitData, m_texture.GetAddressOf());
        device->CreateShaderResourceView(m_texture.Get(), nullptr,
            m_srv.GetAddressOf());

        m_isLoaded = true;
	}

    return m_isLoaded;
}

bool Texture::IsLoaded() const
{
    return m_isLoaded;
}

std::string Texture::GetName() const
{
    return m_filename;
}

ComPtr<ID3D11ShaderResourceView>& Texture::GetSRV()
{
    return m_srv;
}