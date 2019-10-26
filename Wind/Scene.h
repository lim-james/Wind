#ifndef SCENE_H
#define SCENE_H

#include "EntityManager.h"
#include "ComponentsManager.h"
#include "SystemsManager.h"

class Scene {

protected:

	EntityManager* entities;
	ComponentsManager* components;
	SystemsManager* systems;

public:

	Scene();
	virtual ~Scene();

	// Initialization
	virtual void Awake();
	virtual void Reset();
	virtual void Start();

	// updates every fixed timestep
	virtual void FixedUpdate();
	// updates every frame
	virtual void Update(const float& dt);

	virtual void Destroy();

};

#endif
