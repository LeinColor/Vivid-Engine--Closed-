#include "stdafx.h"
#include "Importer.h"
#include "tiny_obj_loader.h"
#include "Debug.h"
#include "Resources.h"

#include "Renderer.h"
#include "Mesh.h"
#include "Shader.h"

#include "WICTextureLoader.h"

#include <fstream>
#include <vector>
#include <string>
using namespace std;

uint32_t Importer::LoadObjFile(const std::string name, const char* fileName)
{
	Mesh mesh;

	tinyobj::attrib_t attrib;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;

	string warn, error;
	bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &error, fileName, "../VividEngine/Obj", true);

	if (!success) {
		vivid::Debug::Log("Obj file import failed");
	}

	for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
		auto& index = shapes[0].mesh.indices[i];


		mesh.positions.push_back(XMFLOAT3(
			attrib.vertices[index.vertex_index * 3 + 0],
			attrib.vertices[index.vertex_index * 3 + 1],
			attrib.vertices[index.vertex_index * 3 + 2]));


		XMFLOAT2 tex = XMFLOAT2(0, 0);
		if (index.texcoord_index >= 0 && !attrib.texcoords.empty()) {
			tex = XMFLOAT2(
				attrib.texcoords[index.texcoord_index * 2 + 0],
				attrib.texcoords[index.texcoord_index * 2 + 1]);
		}
		mesh.texcoords.push_back(tex);

		if (index.normal_index >= 0) {
			mesh.normals.push_back(XMFLOAT3(
				attrib.normals[index.normal_index * 3 + 0],
				attrib.normals[index.normal_index * 3 + 1],
				attrib.normals[index.normal_index * 3 + 2]));
		}
	}

	Resources::meshLookUp[name] = Resources::meshes.size();
	Resources::meshes.push_back(mesh);

	return Resources::meshes.size() - 1;
}

uint32_t Importer::LoadShaderFile(const std::string name, const std::string& fileName, const INPUT_LAYOUT_TYPE inputLayoutTypeValue)
{
	Shader shader;

	shader.inputLayoutType = inputLayoutTypeValue;

	BYTE flag = 0;

	const string defaultPath = "../VividEngine/HLSL/";
	const string formats[] = { "_vs.hlsl", "_ps.hlsl", "_gs.hlsl", "_hs.hlsl", "_ds.hlsl", "_cs.hlsl" };
	const BYTE stage[] = { VERTEX_SHADER, PIXEL_SHADER, GEOMETRY_SHADER, HULL_SHADER, DOMAIN_SHADER, COMPUTE_SHADER };

	for (int i = 0; i < SHADER_STAGE_COUNT; i++) {
		string existingPath = defaultPath + fileName + formats[i];

		ifstream f(existingPath);
		if (f.is_open())
		{
			flag |= stage[i];
		}
		f.close();
	}

	shader.shaderFlag = flag;

	HRESULT result = E_FAIL;

	if (flag & VERTEX_SHADER)
	{
		string path = defaultPath + fileName + "_vs.hlsl";
		wstring wString = wstring(path.begin(), path.end());
		const wchar_t* pFileName = wString.c_str();
		result = D3DCompileFromFile(pFileName, nullptr, nullptr, "VSMain", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &shader.blobs.vs, nullptr);
	}
	if (flag & PIXEL_SHADER)
	{
		string path = defaultPath + fileName + "_ps.hlsl";
		wstring wString = wstring(path.begin(), path.end());
		const wchar_t* pFileName = wString.c_str();
		result = D3DCompileFromFile(pFileName, nullptr, nullptr, "PSMain", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &shader.blobs.ps, nullptr);
	}
	if (flag & GEOMETRY_SHADER)
	{
		string path = defaultPath + fileName + "_gs.hlsl";
		wstring wString = wstring(path.begin(), path.end());
		const wchar_t* pFileName = wString.c_str();
		result = D3DCompileFromFile(pFileName, nullptr, nullptr, "GSMain", "gs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &shader.blobs.gs, nullptr);
	}
	if (flag & HULL_SHADER)
	{
		string path = defaultPath + fileName + "_hs.hlsl";
		wstring wString = wstring(path.begin(), path.end());
		const wchar_t* pFileName = wString.c_str();
		result = D3DCompileFromFile(pFileName, nullptr, nullptr, "HSMain", "hs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &shader.blobs.hs, nullptr);
	}
	if (flag & DOMAIN_SHADER)
	{
		string path = defaultPath + fileName + "_ds.hlsl";
		wstring wString = wstring(path.begin(), path.end());
		const wchar_t* pFileName = wString.c_str();
		result = D3DCompileFromFile(pFileName, nullptr, nullptr, "DSMain", "ds_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &shader.blobs.ds, nullptr);
	}
	if (flag & COMPUTE_SHADER)
	{
		string path = defaultPath + fileName + "_cs.hlsl";
		wstring wString = wstring(path.begin(), path.end());
		const wchar_t* pFileName = wString.c_str();
		result = D3DCompileFromFile(pFileName, nullptr, nullptr, "CSMain", "cs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &shader.blobs.cs, nullptr);
	}

	if (FAILED(result))
	{
		vivid::Debug::Log("Cannot compile shader!");
	}

	Resources::shaderLookUp[name] = Resources::shaders.size();
	Resources::shaders.push_back(shader);

	return Resources::shaders.size() - 1;
}

uint32_t Importer::LoadTextureFile(const std::string name, const WCHAR* fileName)
{
	Texture texture;

	if (Renderer::graphics == nullptr) {
		vivid::Debug::Log("Cannot find the graphics API!");
		return INVALID_TEXTURE_ID;
	}

	CreateWICTextureFromFile(Renderer::graphics->GetDevice(), fileName, nullptr, &texture.resourceView);

	Resources::textureLookUp[name] = Resources::textures.size();
	Resources::textures.push_back(texture);

	return Resources::textures.size() - 1;
}