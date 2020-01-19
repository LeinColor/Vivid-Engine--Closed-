#pragma once
#include <vector>
class Shader;
class Mesh;
class Texture;
class Material;

struct VertexShader;
struct PixelShader;
struct GeometryShader;
struct HullShader;
struct DomainShader;
struct ComputeShader;
class Manager {
public:
	// Shader containers
	static std::vector<VertexShader*> vertexShaders;
	static std::vector<PixelShader*> pixelShaders;
	static std::vector<GeometryShader*> geometryShaders;
	static std::vector<HullShader*> hullShaders;
	static std::vector<DomainShader*> domainShaders;
	static std::vector<ComputeShader*> computeShaders;

	// Resource containers
	static std::vector<Shader*> shaders;
	static std::vector<Mesh*> meshes;
	static std::vector<Texture*> textures;
	static std::vector<Material*> materials;

	// Buffer containers
	static std::vector<ID3D10Blob*> vertexShaderBuffers;
	static std::vector<ID3D11InputLayout*> inputLayouts;
	static std::vector<ID3D11Buffer*> constantBuffers;

	static std::vector<ID3D11Buffer*> resourceBuffers;
	static std::vector<ID3D11SamplerState*> samplerStates;
};