#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "System.h"
#include "Render.h"
#include "Camera.h"
#include "Shader.h"

#include <Events/Event.h>

#include <vector>
#include <map>

struct Instance {
	vec4f uvRect;
	vec4f tint;
	mat4f model;
};

typedef std::map<unsigned, std::vector<Instance>> Batches;

class RenderSystem : public System {
	
	std::vector<Camera*> cameras;
	std::vector<Render*> components;

	static const unsigned INSTANCE_LAYOUT_LOCATION = 2;
	static unsigned instanceBuffer;
		
	static unsigned quadVAO;
	Shader* mainShader;
	Shader* textShader;

	Batches batches;

public:

	RenderSystem();
	~RenderSystem() override;

	void Update(const float& dt) override;

private:

	void CameraActiveHandler(Events::Event* event);
	void CameraDepthHandler(Events::Event* event);

	void RenderActiveHandler(Events::Event* event);

	void GenerateQuad();

};

#endif
