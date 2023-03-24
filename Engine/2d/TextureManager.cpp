#include "TextureManager.h"
#include "DirectX.h"
#include <DirectXTex.h>
using namespace DirectX;

int TextureManager::whiteTexHandle = 0;

void TextureManager::SetWhiteTexHandle()
{
	whiteTexHandle = LoadTextureGraph(L"Resources/Sprite/white1x1.png");
}

TextureManager* TextureManager::GetInstance()
{
	static TextureManager* instance = new TextureManager;
	return instance;
}

void TextureManager::DeleteInstance()
{
	delete TextureManager::GetInstance();
}

TextureManager::TextureManager()
{
}

void TextureManager::Initialize()
{
	textureNum = 0;
	//	サイズ確保
	const size_t kMaxSRVCount = 2056;
	texBuff.resize(kMaxSRVCount);
	uploadBuff.resize(kMaxSRVCount);
	texExist.resize(kMaxSRVCount);
	texExist[0] = true;			//	スクリーン用
}

int TextureManager::LoadTextureGraph(const wchar_t* textureName)
{
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	MyDirectX* dx = MyDirectX::GetInstance();

	HRESULT result;
	result = LoadFromWICFile(
		textureName,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	if (!SUCCEEDED(result)) {
		//	読み込みに失敗したら白色画像
		return whiteTexHandle;
	}
	int index = 0;		//	画像のindex
	for (int i = 0; i < texExist.size(); i++)
	{
		//	画像がその配列になかったら
		if (texExist[i] == false)
		{
			texExist[i] = true;
			index = i;
			break;
		}
	}
	textureNum++;

	//	ミニマップ生成
	ScratchImage mipChain{};
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	const Image* img = scratchImg.GetImage(0, 0, 0);			//	生データ

	metadata.format = MakeSRGB(metadata.format);

	D3D12_RESOURCE_DESC tectureResourceDesc{};
	tectureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	tectureResourceDesc.Format = metadata.format;
	tectureResourceDesc.Width = metadata.width;
	tectureResourceDesc.Height = (UINT)metadata.height;
	tectureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	tectureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	tectureResourceDesc.SampleDesc.Count = 1;

	//	ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	int buffIndex = index - 1;
	//	テクスチャバッファ生成
	result = dx->GetDev()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&tectureResourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&texBuff[buffIndex]));

	//	FootPrint取得
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
	UINT64 total_bytes = 0;
	dx->GetDev()->GetCopyableFootprints(&tectureResourceDesc, 0, 1, 0, &footprint, nullptr, nullptr, &total_bytes);

#pragma region Upload
	D3D12_RESOURCE_DESC uploadDesc{};
	uploadDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//uploadDesc.Width = MyMath::AlignmentSize(img->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT) * img->height;
	uploadDesc.Width = total_bytes;
	uploadDesc.Height = 1;
	uploadDesc.DepthOrArraySize = 1;
	uploadDesc.MipLevels = 1;
	uploadDesc.SampleDesc.Count = 1;
	uploadDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_HEAP_PROPERTIES uploadHeap{};
	uploadHeap.Type = D3D12_HEAP_TYPE_UPLOAD;

	dx->GetDev()->CreateCommittedResource(
		&uploadHeap,
		D3D12_HEAP_FLAG_NONE,
		&uploadDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,		//	CPUから書き込み可能、GPUは読み取り専用
		nullptr,
		IID_PPV_ARGS(&uploadBuff[buffIndex]));
#pragma endregion

	//	転送
	uint8_t* mapforImg = nullptr;
	result = uploadBuff[buffIndex]->Map(0, nullptr, (void**)&mapforImg);	//	map

	uint8_t* uploadStart = mapforImg + footprint.Offset;
	uint8_t* sourceStart = img->pixels;
	uint32_t sourcePitch = ((uint32_t)img->width * sizeof(uint32_t));

	//	画像の高さ(ピクセル)分コピーする
	for (uint32_t i = 0; i < footprint.Footprint.Height; i++)
	{
		memcpy(
			uploadStart + i * footprint.Footprint.RowPitch,
			sourceStart + i * sourcePitch,
			sourcePitch
		);
	}
	uploadBuff[buffIndex]->Unmap(0, nullptr);	//	unmap

#pragma region CopyCommand
	//	グラフィックボード上のコピー先アドレス
	D3D12_TEXTURE_COPY_LOCATION texCopyDest{};
	texCopyDest.pResource = texBuff[buffIndex].Get();
	texCopyDest.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	texCopyDest.SubresourceIndex = 0;
	//	グラフィックボード上のコピー元アドレス
	D3D12_TEXTURE_COPY_LOCATION src{};
	src.pResource = uploadBuff[buffIndex].Get();
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src.PlacedFootprint = footprint;

	//	作成
	dx->GetCmdList()->CopyTextureRegion(&texCopyDest, 0, 0, 0, &src, nullptr);

	//	resourceBarrier挿入
	D3D12_RESOURCE_BARRIER copyBarrier{};
	copyBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	copyBarrier.Transition.pResource = texBuff[buffIndex].Get();
	copyBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	copyBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	copyBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	dx->GetCmdList()->ResourceBarrier(1, &copyBarrier);

#pragma region SetSRV
	UINT incrementSize = dx->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = dx->GetCPUSRVHeapForHeapStart();
	srvHandle.ptr += incrementSize * index;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	dx->GetDev()->CreateShaderResourceView(texBuff[buffIndex].Get(), &srvDesc, srvHandle);
#pragma endregion

	return index;
}

void TextureManager::DeleteTexture(int handle)
{
	if (texExist[handle])
	{
		texExist[handle] = false;
	}
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetTextureHandle(int handle)
{
	MyDirectX* dx = MyDirectX::GetInstance();
	UINT incrementSize = dx->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = dx->GetGPUSRVHeapForHeapStart();
	srvGpuHandle.ptr += incrementSize * handle;
	return srvGpuHandle;
}
