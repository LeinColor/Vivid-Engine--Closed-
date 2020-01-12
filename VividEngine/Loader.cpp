#include "stdafx.h"
#include "Loader.h"
#include "DirectX11Wrapper.h"
#include "Renderer.h"
#include "Manager.h"
#include "Shader.h"
#include <vector>
using namespace std;

DirectX11Wrapper* Loader::graphics;
void Loader::Initialize()
{
	graphics = Renderer::GetGraphicsInterface();

	Loader::Meshes();
	Loader::VertexShader();
	Loader::InputLayout();
	Loader::PixelShader();
	Loader::ConstantBuffers();

	Manager::shaders.resize(50);

	// num, VS, VSBuffer, inputLayout, PS, CB, GS, HS, DS, CS
	vector<int> CBSet;
	CBSet.push_back(CONSTANT_BUFFER_WVP);
	CBSet.push_back(CONSTANT_BUFFER_COLOR);
	BindShader(SHADER_DEBUG, VS_DEBUG, VS_DEBUG, IL_POS, PS_DEBUG, CBSet);
	CBSet.clear();

	CBSet.push_back(CONSTANT_BUFFER_WVP);
	CBSet.push_back(CONSTANT_BUFFER_CAMERA);
	CBSet.push_back(CONSTANT_BUFFER_COLOR);
	BindShader(SHADER_BLINN_PHONG, VS_BLINN_PHONG, VS_BLINN_PHONG, IL_POS_COLOR_NORMAL, PS_BLINN_PHONG, CBSet);
	CBSet.clear();
}

void Loader::BindShader(int num, int VS, int VSBuffer, int inputLayout, int PS, vector<int> CB, int GS, int HS, int DS, int CS)
{
	Shader* shader = new Shader();
	Manager::shaders[num] = shader;
	shader->vertexShader = &Manager::vertexShaders[VS];
	shader->vertexShaderBuffers = &Manager::vertexShaderBuffers[VSBuffer];
	shader->inputLayouts = &Manager::inputLayouts[inputLayout];
	shader->pixelShader = &Manager::pixelShaders[PS];
	for (int i = 0; i < CB.size(); i++) {
		shader->constantBuffers.insert(make_pair(CB[i], &Manager::constantBuffers[i]));
	}
}