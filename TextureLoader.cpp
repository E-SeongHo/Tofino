
#define _CRT_SECURE_NO_WARNINGS 

#include "TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

using namespace DirectX;
using namespace std;

void TextureLoader::CreateDDSCubemapTexture(ComPtr<ID3D11Device>& device, const std::wstring filename, ComPtr<ID3D11ShaderResourceView>& shaderResourceView)
{
    ComPtr<ID3D11Texture2D> texture;
    
    // https://github.com/microsoft/DirectXTK/wiki/DDSTextureLoader
    ThrowIfFailed(CreateDDSTextureFromFileEx(
        device.Get(), filename.c_str(), 0, D3D11_USAGE_DEFAULT,
        D3D11_BIND_SHADER_RESOURCE, 0,
        D3D11_RESOURCE_MISC_TEXTURECUBE,
        DDS_LOADER_FLAGS(false), (ID3D11Resource**)texture.GetAddressOf(),
        shaderResourceView.GetAddressOf(), nullptr));

}

void TextureLoader::CreateDDSTexture(ComPtr<ID3D11Device>& device, const std::wstring filename, ComPtr<ID3D11ShaderResourceView>& shaderResourceView)
{
    ComPtr<ID3D11Texture2D> texture;

    // https://github.com/microsoft/DirectXTK/wiki/DDSTextureLoader
    ThrowIfFailed(CreateDDSTextureFromFileEx(
        device.Get(), filename.c_str(), 0, D3D11_USAGE_DEFAULT,
        D3D11_BIND_SHADER_RESOURCE, 0,
        0,
        DDS_LOADER_FLAGS(false), (ID3D11Resource**)texture.GetAddressOf(),
        shaderResourceView.GetAddressOf(), nullptr));
}

bool TextureLoader::CreateTextureFromImage(ComPtr<ID3D11Device>& device, const std::string filename, ComPtr<ID3D11ShaderResourceView>& shaderResourceView, const bool gammaDecode)
{
    int width = 0, height = 0;
    std::vector<uint8_t> image;

    /* 
    if texture's format sRGB, then need gamma correction for exact calculation of lighting
    DX11 automatically gamma decode for SRGB format.

    and like normal, height map texture, if it's format is not SRGB, should not be gamma decoded.
    means which texture doesn't represent color, but just number.
    */
    DXGI_FORMAT pixelFormat =
        gammaDecode ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM;

    int channels;
    unsigned char* img =
        stbi_load(filename.c_str(), &width, &height, &channels, 0);

    // assert(channels == 4);

    cout << filename << " " << width << " " << height << " " << channels << endl;
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

    ComPtr<ID3D11Texture2D> texture;
    device->CreateTexture2D(&txtDesc, &InitData, texture.GetAddressOf());
    device->CreateShaderResourceView(texture.Get(), nullptr,
        shaderResourceView.GetAddressOf());
    
    return true;
}

