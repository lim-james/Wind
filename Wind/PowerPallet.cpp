#include "PowerPallet.h"

#include "Transform.h"
#include "Render.h"
#include "Collider.h"

void PowerPallet::Build() {
	AddComponent<Transform>();
	AddComponent<Render>();
	AddComponent<Collider>();
}

