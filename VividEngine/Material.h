#pragma once
#include "stdafx.h"
#include "Shader.h"
#include "Texture.h"

#include <vector>

class Material {
public:
	Shader* shader;
	Texture* texture;
private:
};