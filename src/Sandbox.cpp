#include "Sandbox.h"
#include "Core/System/RenderSystem.h"
#include "Core/System/CameraSystem.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

ti::System::RenderSystem rendersystem;
ti::System::CameraSystem camerasystem;
Assimp::Importer importer;

void Sandbox::Init() {
	auto& engine = registry->Store<ti::EngineProperties>();

	const aiScene *scene = importer.ReadFile("D:\\C++\\2.5D Engine\\res\\cylinder.obj", aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
	
	auto entity = registry->Create();
	registry->Add<ti::Component::Model>(entity, scene);
	registry->Add<ti::Component::Transform>(entity);
	registry->Add<ti::Component::Properties>(entity, "name", entity);

	auto camera = registry->Create();
	registry->Add<ti::Component::Camera>(camera, ti::Projection::PERSPECTIVE, (float)engine.width, (float)engine.height, true);
	registry->Add<ti::Component::Transform>(camera);
	registry->Add<ti::Component::Properties>(camera, "cam0", camera);

	rendersystem.registry = registry;
	camerasystem.registry = registry;
	rendersystem.Init();
	camerasystem.Init();

	for (int i = 0; i < scene->mNumMaterials; i++)
		registry->Store<ti::MaterialRegistry>().RegisterMaterial(ti::Component::Material(scene->mMaterials[i]));

	// SDL_ShowCursor(SDL_DISABLE);
}

void Sandbox::Update(double dt) {
	auto& engine = registry->Store<ti::EngineProperties>();

	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glViewport(0, 0, engine.width, engine.height);
	glClearColor(0.2, 0.2, 0.2, 0.2);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	camerasystem.Update(dt);
	rendersystem.Update(dt);
}
