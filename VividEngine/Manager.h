#pragma once
#include <vector>
class Shader;
class Manager {
public:
	// Shader containers
	static std::vector<ID3D11VertexShader*> vertexShaders;
	static std::vector<ID3D11PixelShader*> pixelShaders;
	static std::vector<ID3D11GeometryShader*> geometryShaders;
	static std::vector<ID3D11HullShader*> hullShaders;
	static std::vector<ID3D11DomainShader*> domainShaders;
	static std::vector<ID3D11ComputeShader*> computeShaders;

	static std::vector<Shader*> shaders;

	// Layout container
	static std::vector<ID3D10Blob*> vertexShaderBuffers;
	static std::vector<ID3D11InputLayout*> inputLayouts;
	static std::vector<ID3D11Buffer*> constantBuffers;

	static std::vector<ID3D11Buffer*> resourceBuffers;
	static std::vector<ID3D11SamplerState*> samplerStates;
};