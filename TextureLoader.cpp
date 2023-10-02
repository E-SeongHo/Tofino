#include "TextureLoader.h"

using namespace DirectX;
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
