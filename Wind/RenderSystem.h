#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "System.h"
#include "Render.h"
#include "Camera.h"
#include "Text.h"
#include "Shader.h"

#include <Events/Event.h>

#include <vector>
#include <map>

struct Instance {
	Render* component;
	vec4f uvRect;
	vec4f tint;
	mat4f model;
};

bool operator==(const Instance& lhs, const Instance& rhs);

typedef std::map<unsigned, std::vector<Instance>> Batches;

class RenderSystem : public System {
	
	std::vector<Camera*> cameras;

	std::map<Font*, std::vector<Text*>> textBatches;

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
	void FixedUpdate(const float& dt) override;

private:

	void CameraActiveHandler(Events::Event* event);
	void CameraDepthHandler(Events::Event* event);

	void RenderActiveHandler(Events::Event* event);
	void TextureChangeHandler(Events::Event* event);

	void TextActiveHandler(Events::Event* event);
	void TextFontHandler(Events::Event* event);

	void GenerateQuad();

};

#endif
