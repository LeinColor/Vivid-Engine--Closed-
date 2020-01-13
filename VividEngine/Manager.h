#pragma once
#include <vector>
class Shader;
class Mesh;
class Texture;
class Material;
class Manager {
public:
	// Shader containers
	static std::vector<ID3D11VertexShader*> vertexShaders;
	static std::vector<ID3D11PixelShader*> pixelShaders;
	static std::vector<ID3D11GeometryShader*> geometryShaders;
	static std::vector<ID3D11HullShader*> hullShaders;
	static std::vector<ID3D11DomainShader*> domainShaders;
	static std::vector<ID3D11ComputeShader*> computeShaders;

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