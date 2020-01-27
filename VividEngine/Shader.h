#pragma once
#include "stdafx.h"

#include <string>
#include <vector>

class Shader {
private:
	struct Blobs {
		ID3D10Blob* vsBlob = nullptr;
		ID3D10Blob* psBlob = nullptr;
		ID3D10Blob* gsBlob = nullptr;
		ID3D10Blob* hsBlob = nullptr;
		ID3D10Blob* dsBlob = nullptr;
		ID3D10Blob* csBlob = nullptr;
	};

	struct SubShaders {
		ID3D11VertexShader*	  vs = nullptr;
		ID3D11PixelShader*	  ps = nullptr;
		ID3D11GeometryShader* gs = nullptr;
		ID3D11HullShader*	  hs = nullptr;
		ID3D11DomainShader*	  ds = nullptr;
		ID3D11ComputeShader*  cs = nullptr;
	};

	enum SHADER_TYPE {
		VERTEX_SHADER	= 1 << 0,
		PIXEL_SHADER	= 1 << 1,
		GEOMETRY_SHADER = 1 << 2,
		HULL_SHADER		= 1 << 3,
		DOMAIN_SHADER	= 1 << 4,
		COMPUTE_SHADER	= 1 << 5,
		SHADER_STAGE_COUNT   = 6,
	};

	Blobs	   blobs;
	SubShaders subShaders;

	ID3D11InputLayout*		   inputLayout;
	int32_t					   inputLayoutType;
	std::vector<ID3D11Buffer*> constantBuffers;

	BYTE GetSubShaderFlag(const std::string& fileName);
	void CompileFromFile(const std::string& fileName, SHADER_TYPE subShaderType);
	void CreateSubShader(SHADER_TYPE subShaderType);
	void CreateInputLayout(INPUT_LAYOUT_TYPE type);

public:
	Shader(const std::string& fileName, INPUT_LAYOUT_TYPE inputLayoutTypeValue);

	static void Initialize();

};