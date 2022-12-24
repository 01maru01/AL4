#include "Model.h"
#include "ObjFile.h"
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

Light* Model::light = nullptr;
GPipeline* Model::pipeline = nullptr;
MyDirectX* Model::dx = MyDirectX::GetInstance();

void Model::Initialize(const char* filename, bool smoothing)
{
	HRESULT result;

#pragma region  ConstBuffer
	D3D12_HEAP_PROPERTIES heapProp{};
	D3D12_RESOURCE_DESC resourceDesc{};
	//	ヒープ設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU転送用

	//	リソース設定
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xFF) & ~0xFF;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//	生成
	result = dx->GetDev()->CreateCommittedResource(
		&cbHeapProp,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&transform));
	assert(SUCCEEDED(result));

	//	定数バッファのマッピング
	result = transform->Map(0, nullptr, (void**)&constMapTransform);	//	マッピング
	assert(SUCCEEDED(result));
#pragma endregion

	vertexSize = vertices.size();
	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertexSize);

	BuffInitialize(sizeVB, vertexSize);
#pragma region  WorldMatrix初期値
	mat.Initialize();
#pragma endregion
}

void Model::SetLight(Light* light)
{
	Model::light = light;
}

void Model::SetPipeline(GPipeline* pipeline_)
{
	Model::pipeline = pipeline_;
}

Model::Model(const char* filename, bool smoothing)
{
	Initialize(filename, smoothing);
}

void Model::MatUpdate(Matrix matView, Matrix matProjection, const Vector3D& cameraPos)
{
#pragma region WorldMatrix
	mat.Update();
#pragma endregion

	constMapTransform->matworld = mat.matWorld;
	constMapTransform->matview = matView;
	constMapTransform->matview *= matProjection;
	constMapTransform->cameraPos = cameraPos;
}

void Model::Draw()
{
	pipeline->Setting(dx->GetCmdList());
	pipeline->Update(dx->GetCmdList(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	BuffUpdate();
	//	テクスチャ
	//dx->GetCmdList()->SetGraphicsRootConstantBufferView(0, material->GetGPUVirtualAddress());
	dx->GetCmdList()->SetGraphicsRootDescriptorTable(1, dx->GetTextureHandle(textureHandle));
	dx->GetCmdList()->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());
	light->Draw();

	dx->GetCmdList()->DrawInstanced(vertexSize, 1, 0, 0);
}

void Model::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	std::ifstream file;
	file.open(directoryPath + filename);
	if (file.fail()) { assert(0); }

	Material* mtl_ = nullptr;

	string line;
	while (getline(file, line))
	{
		std::istringstream line_stream(line);

		string key;
		getline(line_stream, key, ' ');

		if (key[0] == '\t') {
			key.erase(key.begin());
		}

		if (key == "newmtl") {
			if (mtl_) {
				materials.push_back(*mtl_);
			}
			mtl_ = Material::Create();

			line_stream >> mtl_->name;
		}

		if (key == "Ka") {
			line_stream >> mtl->ambient.x;
			line_stream >> mtl->ambient.y;
			line_stream >> mtl->ambient.z;
		}

		if (key == "Kd") {
			line_stream >> mtl->diffuse.x;
			line_stream >> mtl->diffuse.y;
			line_stream >> mtl->diffuse.z;
		}

		if (key == "Ks") {
			line_stream >> mtl->specular.x;
			line_stream >> mtl->specular.y;
			line_stream >> mtl->specular.z;
		}

		if (key == "map_Kd") {
			line_stream >> mtl->textureFilename;

			string filepath = directoryPath + mtl->textureFilename;
			int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, mtl->wfilepath, _countof(mtl->wfilepath));
		}
	}
	file.close();

	materials.push_back(*mtl_);
}

void Model::LoadModel(const std::string& filename, bool smoothing)
{
	std::vector<unsigned short> vertexIndices, uvIndices, normalIndices;
	std::vector<Vector3D> temp_poss;
	std::vector<Vector2D> temp_uvs;
	std::vector<Vector3D> temp_normals;

	std::ifstream file_;
	const string filename = filename + ".obj";
	const string directoryPath = "Resources/Model/" + filename + "/";
	file_.open(directoryPath + filename);
	assert(!file_.fail());

	meshes.emplace_back(new Mesh);
	Mesh* mesh = meshes.back();

	string line;
	while (getline(file_, line)) {
		istringstream line_stream(line);

		string key;
		getline(line_stream, key, ' ');

		if (key == "mtllib") {
			string filename;
			line_stream >> filename;
			LoadMaterial(directoryPath, filename);
		}

		if (key == "v") {
			Vector3D pos;
			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> pos.z;

			temp_poss.emplace_back(pos);
		}

		if (key == "vt") {
			Vector2D uv;
			line_stream >> uv.x;
			line_stream >> uv.y;
			uv.y = 1.0f - uv.y;

			temp_uvs.emplace_back(uv);
		}

		if (key == "vn") {
			Vector3D normal;
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;

			temp_normals.emplace_back(normal);
		}

		if (key == "f") {
			string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				std::istringstream index_stream(index_string);
				unsigned short indexPos, indexNormal, indexUV;
				index_stream >> indexPos;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexUV;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;

				Vertex vertex{};
				vertex.pos = temp_poss[indexPos - 1];
				vertex.normal = temp_normals[indexNormal - 1];
				vertex.uv = temp_uvs[indexUV - 1];

				out_vertices.emplace_back(vertex);

				if (smoothing) {
					AddSmoothData(indexPos, (unsigned short)out_vertices.size() - 1);
				}
			}
		}
	}

	file_.close();

	if (smoothing) {
		CalcSmoothedVertexNormals(out_vertices);
	}
}

void Model::SetVertices()
{
	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < vertexSize; i++) {
		vertMap[i] = vertices[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);
}
