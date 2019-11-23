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
	virtual void FixedUpdate(const float& dt);
	// updates every frame
	virtual void Update(const float& dt);

	virtual void Stop();
	virtual void Destroy();

	virtual void PrepareForSegue(Scene* destination);

private:

	// event handlers

	void NearestEntityHanlder(Events::Event* event);
	void FirstEntityHanlder(Events::Event* event);

};

#endif
