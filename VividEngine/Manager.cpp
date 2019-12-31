#include "Manager.h"
#include <vector>
using namespace std;

vector<GameObject*> Manager::gameObjects;
vector<Mesh*> Manager::meshes;
vector<Texture*> Manager::textures;
vector<Shader*> Manager::shaders;