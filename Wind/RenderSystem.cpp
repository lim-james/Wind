#include "RenderSystem.h"

#include "Entity.h"
#include "Transform.h"
#include "LoadFNT.h"

#include <Events/EventsManager.h>
#include <Math/MatrixTransform.hpp>
#include <MACROS.h>
#include <GL/glew.h>

unsigned RenderSystem::instanceBuffer = 0;
unsigned RenderSystem::quadVAO = 0;

RenderSystem::RenderSystem() {

	Events::EventsManager::GetInstance()->Subscribe("CAMERA_ACTIVE", &RenderSystem::CameraActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("CAMERA_DEPTH", &RenderSystem::CameraDepthHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("RENDER_ACTIVE", &RenderSystem::RenderActiveHandler, this);

	if (instanceBuffer == 0)
		glGenBuffers(1, &instanceBuffer);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (quadVAO == 0)
		GenerateQuad();

	mainShader = new Shader("Files/Shaders/standard2D.vert", "Files/Shaders/standard2D.frag");
	mainShader->Use();
	mainShader->SetInt("tex", 0);

	textShader = new Shader("Files/Shaders/text.vert", "Files/Shaders/standard2D.frag");
	textShader->Use();
	textShader->SetInt("tex", 0);
}

RenderSystem::~RenderSystem() {
	components.clear();

	delete mainShader;
	delete textShader;
}

void RenderSystem::Update(const float& dt) {

	for (auto& cam : cameras) {
		glBindVertexArray(quadVAO);
		mainShader->Use();
		const auto& viewport = cam->GetViewport();
		const auto& projection = cam->GetProjectionMatrix();
		const auto& lookAt = cam->GetParent()->GetComponent<Transform>()->GetLocalLookAt();

		const Math::vec2<GLint> origin(
			static_cast<GLint>(viewport.origin.x),
			static_cast<GLint>(viewport.origin.y)
		);

		const Math::vec2<GLint> size(
			static_cast<GLsizei>(viewport.size.w),
			static_cast<GLsizei>(viewport.size.h)
		);

		glViewport(origin.x, origin.y, size.x, size.y);
		glScissor(origin.x, origin.y, size.x, size.y);
		glClearColor(cam->clearColor.r, cam->clearColor.g, cam->clearColor.b, cam->clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mainShader->SetMatrix4("projection", projection);
		mainShader->SetMatrix4("view", lookAt);

		batches.clear();

		for (auto& c : components) {
			batches[c->texture].push_back({
				c->uvRect,
				c->tint,
				c->GetParent()->GetComponent<Transform>()->GetLocalTransform()
			});
		}

		for (auto& batchPair : batches) {
			auto& batch = batchPair.second;

			const unsigned count = batch.size();
			const unsigned unit = 4 * sizeof(float);
			const unsigned stride = sizeof(Instance);

			glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
			glBufferData(GL_ARRAY_BUFFER, count * stride, &batch[0], GL_STATIC_DRAW);

			unsigned i, offset;
			i = INSTANCE_LAYOUT_LOCATION;
			offset = 0;

			// rect	
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i++, 4, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
			offset += sizeof(vec4f);
			// tint	
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i++, 4, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
			offset += sizeof(vec4f);
			// model
			for (int u = 0; u < 4; ++u) {
				glEnableVertexAttribArray(i);
				glVertexAttribPointer(i++, 4, GL_FLOAT, GL_FALSE, stride, (void*)(u * unit + offset));
			}

			for (; i >= INSTANCE_LAYOUT_LOCATION; --i)
				glVertexAttribDivisor(i, 1);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, batchPair.first);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, count);
		}

		glDisable(GL_DEPTH_TEST);

		textShader->Use();
		textShader->SetMatrix4("projection", projection);
		textShader->SetMatrix4("view", lookAt);
		textShader->SetVector4("color", vec4f(1.f));

		auto font = Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga");

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, font->texture);
		glBindVertexArray(font->VAO);

		vec3f position(0.f);

		for (auto c : "Wind\nEngine") {
			if (c == '\0') continue;
			switch (c) {
			case '\0':
				continue;
			case '\n':
				position.y -= font->lineHeight;
				position.x = 0.f;
				break;
			default:
				auto character = font->characters[c];

				const vec3f offset = character.rect.origin;

				mat4f model;
				Math::SetToTranslation(model, position + offset);
				textShader->SetMatrix4("model", model);

				const int index = character.index * 6;
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(index * sizeof(unsigned)));

				position.x += character.xAdvance;
				break;
			}
		}

		glEnable(GL_DEPTH_TEST);
	}

}

void RenderSystem::CameraActiveHandler(Events::Event* event) {
	const auto camera = static_cast<Events::AnyType<Camera*>*>(event)->data;

	if (camera->IsActive()) {
		for (unsigned i = 0; i < cameras.size(); ++i) {
			if (cameras[i]->depth >= camera->depth) {
				cameras.insert(cameras.begin() + i, camera);
				return;
			}
		}
		cameras.push_back(camera);
	} else {
		cameras.erase(vfind(cameras, camera));
	}
}

void RenderSystem::CameraDepthHandler(Events::Event* event) {
	const auto camera = static_cast<Events::AnyType<Camera*>*>(event)->data;

	if (camera->IsActive()) {
		cameras.erase(vfind(cameras, camera));
		for (unsigned i = 0; i < cameras.size(); ++i) {
			if (cameras[i]->depth >= camera->depth) {
				cameras.insert(cameras.begin() + i, camera);
				return;
			}
		}
		cameras.push_back(camera);
	}
}

void RenderSystem::RenderActiveHandler(Events::Event* event) {
	const auto c = static_cast<Events::AnyType<Render*>*>(event)->data;

	if (c->IsActive()) {
		components.push_back(c);
	} else {
		components.erase(vfind(components, c));
	}
}

void RenderSystem::GenerateQuad() {
	float quadVertices[] = {
		-0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  1.0f, 0.0f,

		-0.5f,  0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  1.0f, 1.0f
	};

	unsigned VBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}
