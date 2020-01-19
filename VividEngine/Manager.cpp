#include "stdafx.h"
#include "Manager.h"
#include "Shader.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include <vector>
using namespace std;
// Shader containers
vector<VertexShader*> Manager::vertexShaders;
vector<PixelShader*> Manager::pixelShaders;
vector<GeometryShader*> Manager::geometryShaders;
vector<HullShader*> Manager::hullShaders;
vector<DomainShader*> Manager::domainShaders;
vector<ComputeShader*> Manager::computeShaders;

// Resource containers
vector<Shader*> Manager::shaders;
vector<Mesh*> Manager::meshes;
vector<Texture*> Manager::textures;
vector<Material*> Manager::materials;

// Buffer containers
vector<ID3D10Blob*> Manager::vertexShaderBuffers;
vector<ID3D11InputLayout*> Manager::inputLayouts;
vector<ID3D11Buffer*> Manager::constantBuffers;

vector<ID3D11Buffer*> Manager::resourceBuffers;
vector<ID3D11SamplerState*> Manager::samplerStates;