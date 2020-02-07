#include "Resources.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

#include <vector>
#include <string>
#include <unordered_map>

std::vector<Mesh>    Resources::meshes;
std::vector<Shader>  Resources::shaders;
std::vector<Texture> Resources::textures;

std::unordered_map<std::string, uint32_t> Resources::meshLookUp;
std::unordered_map<std::string, uint32_t> Resources::shaderLookUp;
std::unordered_map<std::string, uint32_t> Resources::textureLookUp;