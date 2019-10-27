#include "RenderSystem.h"

#include "Entity.h"
#include "Transform.h"

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
	Events::EventsManager::GetInstance()->Subscribe("TEXT_ACTIVE", &RenderSystem::TextActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("TEXT_FONT", &RenderSystem::TextFontHandler, this);

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
		
		for (auto& textPair : textBatches) {
			auto& font = textPair.first;
			if (font == nullptr) continue;

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, font->texture);
			glBindVertexArray(font->VAO);

			for (auto& text : textPair.second) {
				textShader->SetVector4("color", text->color);
				auto transform = text->GetParent()->GetComponent<Transform>();

				const float scale = text->scale;

				std::vector<float> lineOffset;
				vec2f size(0.f);

				const auto& content = text->text;

				for (unsigned i = 0; i <= content.size(); ++i) {
					auto& c = content[i];

					switch (c) {
					case '\n':
					case '\0':
						++size.y;
						switch (text->paragraphAlignment)
						{
						case PARAGRAPH_CENTER:
							lineOffset.push_back(size.x * scale * 0.5f);
							break;
						case PARAGRAPH_RIGHT:
							lineOffset.push_back(transform->scale.x * -0.5f - size.x * -scale);
							break;
						default:
							lineOffset.push_back(transform->scale.x * 0.5f);
							break;
						}					
						size.x = 0.f;
						break;
					default:
						size.x += font->characters[c].xAdvance;
						break;
					}
				}
				size.y *= font->lineHeight * text->lineSpacing * text->scale;

				vec3f position(0.f);
				position.x = transform->translation.x - lineOffset[0];

				switch (text->verticalAlignment)
				{
				case ALIGN_MIDDLE:
					position.y = transform->translation.y + size.y * 0.5f;
					break;
				case ALIGN_BOTTOM:
					position.y = transform->translation.y - transform->scale.y * 0.5f + size.y;
					break;
				default:
					position.y = transform->translation.y + transform->scale.y * 0.5f;
					break;
				}

				int lineNumer = 0;
				for (auto& c : content) {
					if (c == '\0') continue;

					switch (c) {
					case '\n':
						position.y -= font->lineHeight * text->lineSpacing * text->scale;
						position.x = transform->translation.x - lineOffset[++lineNumer];
						break;
					default:
						const auto& character = font->characters[c];
						const vec3f offset = character.rect.origin * scale;

						mat4f model;
						Math::SetToTranslation(model, position + offset);
						if (text->scale != 1.f)
							Math::Scale(model, vec3f(scale));
						textShader->SetMatrix4("model", model);

						const int index = character.index * 6;
						glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(index * sizeof(unsigned)));

						position.x += character.xAdvance * text->characterSpacing * scale;
						break;
					}
				}
			}
		}

		glEnable(GL_DEPTH_TEST);
	}

}

void RenderSystem::FixedUpdate(const float& dt) {}

void RenderSystem::CameraActiveHandler(Events::Event* event) {
	auto& c = static_cast<Events::AnyType<Camera*>*>(event)->data;

	if (c->IsActive()) {
		for (unsigned i = 0; i < cameras.size(); ++i) {
			if (cameras[i]->depth >= c->depth) {
				cameras.insert(cameras.begin() + i, c);
				return;
			}
		}
		cameras.push_back(c);
	} else {
		cameras.erase(vfind(cameras, c));
	}
}

void RenderSystem::CameraDepthHandler(Events::Event* event) {
	auto& c = static_cast<Events::AnyType<Camera*>*>(event)->data;

	if (c->IsActive()) {
		cameras.erase(vfind(cameras, c));
		for (unsigned i = 0; i < cameras.size(); ++i) {
			if (cameras[i]->depth >= c->depth) {
				cameras.insert(cameras.begin() + i, c);
				return;
			}
		}
		cameras.push_back(c);
	}
}

void RenderSystem::RenderActiveHandler(Events::Event* event) {
	auto& c = static_cast<Events::AnyType<Render*>*>(event)->data;

	if (c->IsActive()) {
		components.push_back(c);
	} else {
		components.erase(vfind(components, c));
	}
}

void RenderSystem::TextActiveHandler(Events::Event* event) {
	auto& c = static_cast<Events::AnyType<Text*>*>(event)->data;
	const auto font = c->GetFont();
	if (font == nullptr) return;

	auto& textList = textBatches[font];

	if (c->IsActive()) {
		textList.push_back(c);
	} else {
		textList.erase(vfind(textList, c));
	}
}

void RenderSystem::TextFontHandler(Events::Event* event) {
	const auto change = static_cast<Events::FontChange*>(event);
	auto& c = change->component;
	auto previous = change->previous;
	auto current = c->GetFont();
	
	if (previous == nullptr) {
		textBatches[current].push_back(c);
	} else if (current == nullptr) {
		auto& textList = textBatches[previous];
		textList.erase(vfind(textList, c));
	} else {
		auto& prevList = textBatches[previous];
		prevList.erase(vfind(prevList, c));
		textBatches[current].push_back(c);
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
