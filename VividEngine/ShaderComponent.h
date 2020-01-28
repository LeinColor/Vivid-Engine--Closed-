#pragma once
#include "stdafx.h"

#include <string>
#include <vector>

class ShaderComponent {
	friend class Importer;
	friend class Renderer;
private:
	struct Blobs {
		ID3D10Blob* vs = nullptr;
		ID3D10Blob* ps = nullptr;
		ID3D10Blob* gs = nullptr;
		ID3D10Blob* hs = nullptr;
		ID3D10Blob* ds = nullptr;
		ID3D10Blob* cs = nullptr;
	};

	struct SubShaders {
		ID3D11VertexShader*	  vs = nullptr;
		ID3D11PixelShader*	  ps = nullptr;
		ID3D11GeometryShader* gs = nullptr;
		ID3D11HullShader*	  hs = nullptr;
		ID3D11DomainShader*	  ds = nullptr;
		ID3D11ComputeShader*  cs = nullptr;
	};

	Blobs	    blobs;
	SubShaders  subShaders;
	BYTE		shaderFlag;

	ID3D11InputLayout*		   inputLayout;
	int32_t					   inputLayoutType;
	std::vector<ID3D11Buffer*> constantBuffers;

	BYTE GetSubShaderFlag(const std::string& fileName);
	void CompileFromFile(const std::string& fileName, SHADER_TYPE subShaderType);
	void CreateSubShader(SHADER_TYPE subShaderType);
	void CreateInputLayout(INPUT_LAYOUT_TYPE type);

public:
};