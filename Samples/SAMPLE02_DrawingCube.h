#pragma once
#include "../VividEngine/Renderer.h"
#include "../VividEngine/Scene.h"

class SAMPLE02_DrawingCubeScene : public Scene
{
public:
	void Start() override;
	void Update() override;
	void FixedUpdate() override;

	void SetTitle(const LPCSTR pText);
};