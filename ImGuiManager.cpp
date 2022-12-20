#include "ImGuiManager.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
#include "Window.h"

void ImGuiManager::Initialize()
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(Window::GetInstance()->GetHwnd());

	MyDirectX* dx = MyDirectX::GetInstance();
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	HRESULT result = dx->GetDev()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));
	ImGui_ImplDX12_Init(dx->GetDev(), 2,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvHeap.Get(),
		srvHeap->GetCPUDescriptorHandleForHeapStart(),
		srvHeap->GetGPUDescriptorHandleForHeapStart());

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
}

void ImGuiManager::Finalize()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	srvHeap.Reset();
}

void ImGuiManager::Begin()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End()
{
	ImGui::Render();
}

void ImGuiManager::Draw()
{
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();

	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);
}
