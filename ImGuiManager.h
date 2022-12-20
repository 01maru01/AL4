#pragma once
#include "DirectX.h"
class ImGuiManager
{
private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;
public:
	void Initialize();
	void Finalize();

	void Begin();
	void End();
	void Draw();
};

