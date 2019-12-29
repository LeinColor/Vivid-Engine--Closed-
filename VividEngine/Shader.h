#pragma once
#include "stdafx.h"

class Shader {
public:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11GeometryShader* geometryShader;
	ID3D11HullShader* hullShader;
	ID3D11DomainShader* domainSHader;
	ID3D11ComputeShader* computeShader;
private:
};