#include "stdafx.h"
#include "Shader.h"
#include "Debug.h"

#include <fstream>
#include <string>
#include <vector>

using namespace vivid;
using namespace std;

Shader::Shader(const char* fileName)
{
	BYTE flag = GetSubShaderFlag(fileName);


}

BYTE Shader::GetSubShaderFlag(const char* fileName)
{
	BYTE flag;

	const string defaultPath = "../VividEngine/HLSL/";
	const string formats[] = { "_vs.hlsl", "_ps.hlsl", "_gs.hlsl", "_hs.hlsl", "_ds.hlsl", "_cs.hlsl" };
	const BYTE stage[] = { VERTEX_SHADER, PIXEL_SHADER, GEOMETRY_SHADER, HULL_SHADER, DOMAIN_SHADER, COMPUTE_SHADER };

	for (int i = 0; i < 6; i++) {
		string existingPath = defaultPath + formats[i];

		ifstream f(existingPath);
		if (f.is_open())
		{
			flag |= stage[i];
		}
		f.close();
	}

	return flag;
}