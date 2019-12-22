#include "VividEngine.h"
#include "GameObject.h"
#include "Texture.h"
#include "TextureShader.h"
#include "Sprite.h"

using namespace vivid;

GameObject camera;
void VividEngine::Initialize()
{
	if (renderer.GetDevice() == nullptr)
		renderer.SetDevice(new DirectX11Wrapper(appHandle.GetWindowHandle(), false));

	GameObject* camera = new GameObject();
	camera->AddComponent<Transform>();
	camera->AddComponent<Camera>();
	gameObjectManager.Add(camera);

	Sprite* sprite = new Sprite();
	sprite->AddComponent<Transform>();
	sprite->AddComponent<SpriteRenderer>();
	spriteManager.Add(sprite);

	renderer.Initialize(gameObjectManager);
	initialized = true;
}

void VividEngine::Run()
{
	if (!initialized) {
		Initialize();
	}

	Update();
}

void VividEngine::Update()
{
	static float timer = 0.0f;

	timer += 0.01f;

	if (timer >= 1.0f) {
		timer = 0;

		// Render all of 2D objects
		renderer.Render2D(spriteManager);
	}
}

void VividEngine::SetWindow(HWND hWnd, HINSTANCE hInstance)
{
	appHandle.SetWindowHandle(hWnd);
	appHandle.SetInstanceHandle(hInstance);
}