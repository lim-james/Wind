#include "Scene.h"

Scene::Scene() {
	components = new ComponentsManager;
	entities = new EntityManager(components);
	systems = new SystemsManager;
}

Scene::~Scene() {
	delete entities;
	delete components;
	delete systems;

	Destroy();
}

void Scene::Awake() {

}

void Scene::Reset() {

}

void Scene::Start() {

}

void Scene::FixedUpdate() {

}

void Scene::Update(const float& dt) {
	systems->Update(dt);
}

void Scene::Destroy() {}