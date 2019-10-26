#include "CameraObject.h"

#include "Transform.h"
#include "Camera.h"

void CameraObject::Build() {
	AddComponent<Transform>();
	AddComponent<Camera>();
}

void CameraObject::Initialize() {
	Entity::Initialize();
	// Move camera back
	GetComponent<Transform>()->translation.z = 1.f;
}