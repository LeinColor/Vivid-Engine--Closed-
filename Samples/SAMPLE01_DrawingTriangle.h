#pragma once
#include "../VividEngine/Renderer.h"
#include "../VividEngine/Scene.h"

class SAMPLE01_DrawingTriangleScene : public Scene
{
public:
	void Start() override;
	void Update() override;
	void FixedUpdate() override;

	void SetTitle(LPCSTR pText);
};