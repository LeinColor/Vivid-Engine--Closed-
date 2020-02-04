#pragma once
#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <vector>
#include <unordered_map>

constexpr uint32_t INVALID_MESH_ID = UINT32_MAX;
constexpr uint32_t INVALID_SHADER_ID = UINT32_MAX;
class Resources {
	friend class Importer;
public:
	inline static uint32_t GetMeshID(const std::string& name)   { return meshLookUp[name]; }
	inline static uint32_t GetShaderID(const std::string& name) { return shaderLookUp[name]; }
	inline static size_t   GetMeshCount() { return meshes.size(); }
	inline static size_t   GetShaderCount() { return shaders.size(); }
	inline static Mesh&    GetMesh(uint32_t id) { return meshes[id]; }
	inline static Shader&  GetShader(uint32_t id) { return shaders[id]; }
private:
	static std::vector<Mesh> meshes;
	static std::vector<Shader> shaders;

	static std::unordered_map<std::string, uint32_t> meshLookUp;
	static std::unordered_map<std::string, uint32_t> shaderLookUp;
};