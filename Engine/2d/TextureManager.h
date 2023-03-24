#pragma once
#include <d3d12.h>
#include <vector>
#include <wrl.h>
#pragma comment(lib, "d3d12.lib")

class TextureManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	static int whiteTexHandle;

	int textureNum = 0;
	std::vector<ComPtr<ID3D12Resource>> texBuff;
	std::vector<ComPtr<ID3D12Resource>> uploadBuff;
	std::vector<bool> texExist;
public:
	void SetWhiteTexHandle();		//	読み込み失敗用白色画像のセット
	static TextureManager* GetInstance();
	static void DeleteInstance();
	TextureManager();
	~TextureManager() {};
	void Initialize();
	void DeleteTexture(int handle);
	int LoadTextureGraph(const wchar_t* textureName);
	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureHandle(int handle);
	ID3D12Resource* GetTextureBuffer(uint32_t index) const { return texBuff[index - 1].Get(); }
};

