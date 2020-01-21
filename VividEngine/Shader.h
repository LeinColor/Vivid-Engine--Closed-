#pragma once
#include "stdafx.h"

#include <string>
#include <vector>

class Shader {
private:
	struct Blobs {
		ID3D10Blob* vsBlob;
		ID3D10Blob* psBlob;
		ID3D10Blob* gsBlob;
		ID3D10Blob* hsBlob;
		ID3D10Blob* dsBlob;
		ID3D10Blob* csBlob;
	};

	struct SubShaders {
		ID3D11VertexShader*	  vs;
		ID3D11PixelShader*	  ps;
		ID3D11GeometryShader* gs;
		ID3D11HullShader*	  hs;
		ID3D11DomainShader*	  ds;
		ID3D11ComputeShader*  cs;
	};

	enum SHADER_FLAG {
		VERTEX_SHADER	= 1 << 0,
		PIXEL_SHADER	= 1 << 1,
		GEOMETRY_SHADER = 1 << 2,
		HULL_SHADER		= 1 << 3,
		DOMAIN_SHADER	= 1 << 4,
		COMPUTE_SHADER	= 1 << 5,
		SHADER_STAGE_COUNT = 6,
	};

	ID3D11InputLayout* inputLayout;
	std::vector<ID3D11Buffer*> constantBuffers;
	BYTE GetSubShaderFlag(const char* fileName);
public:
	Shader(const char* fileName);

};