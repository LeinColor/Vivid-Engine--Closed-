#pragma once
#include "stdafx.h"
#include <unordered_map>
using namespace std;

class Shader {
public:
	ID3D11VertexShader** vertexShader;
	ID3D11PixelShader** pixelShader;
	ID3D11GeometryShader** geometryShader;
	ID3D11HullShader** hullShader;
	ID3D11DomainShader** domainSHader;
	ID3D11ComputeShader** computeShader;

	ID3D10Blob** vertexShaderBuffers;
	ID3D11InputLayout** inputLayouts;
	//ID3D11Buffer** constantBuffers;
	unordered_map<int, ID3D11Buffer**> constantBuffers;

	ID3D11Buffer** resourceBuffers;
	ID3D11SamplerState** samplerStates;
private:
};