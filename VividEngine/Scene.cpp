#include "Scene.h"
#include <vector>
using namespace std;

vector<Object*> Scene::objects;
vector<AABB*> aabbs;
vector<Mesh*> Scene::meshes;
vector<Texture*> Scene::textures;
vector<Shader*> Scene::shaders;