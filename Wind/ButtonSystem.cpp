#include "ButtonSystem.h"

#include "Entity.h"
#include "Transform.h"

#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

ButtonSystem::~ButtonSystem() {
	buttons.clear();
	cameras.clear();
}

void ButtonSystem::Start() {
	Events::EventsManager::GetInstance()->Subscribe("CAMERA_ACTIVE", &ButtonSystem::CameraActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("BUTTON_ACTIVE", &ButtonSystem::ButtonActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("WINDOW_RESIZE", &ButtonSystem::ResizeHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("CURSOR_POSITION_INPUT", &ButtonSystem::CursorPositionHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("MOUSE_BUTTON_INPUT", &ButtonSystem::MouseButtonHandler, this);
}

void ButtonSystem::Update(const float & dt) {
}

void ButtonSystem::FixedUpdate(const float & dt) {
	for (auto& cam : cameras) {
		// world space position
		const vec2f wsp = GetWorldSpace(cam);

		for (auto& button : buttons) {
			if (!button->isEnabled) continue;

			auto& prev = states[button];
			auto parent = button->GetParent();

			auto transform = parent->GetComponent<Transform>();
			const vec2f position = transform->GetWorldTranslation();
			const vec2f size = transform->scale * 0.5f;

			const vec2f offset = Math::Abs(position - wsp);

			if (offset.x <= size.x && offset.y <= size.y) {
				if (!prev) {
					if (button->handlers[MOUSE_OVER])
						button->handlers[MOUSE_OVER](parent);
					prev = true;
				}

				if (mouseActions[GLFW_MOUSE_BUTTON_LEFT] != GLFW_RELEASE) {
					if (prevMouseActions[GLFW_MOUSE_BUTTON_LEFT] == GLFW_RELEASE) {
						prevMouseActions[GLFW_MOUSE_BUTTON_LEFT] = mouseActions[GLFW_MOUSE_BUTTON_LEFT];
						if (button->handlers[MOUSE_DOWN])
							button->handlers[MOUSE_DOWN](parent);
					}
				} else {
					if (prevMouseActions[GLFW_MOUSE_BUTTON_LEFT] != GLFW_RELEASE) {
						prevMouseActions[GLFW_MOUSE_BUTTON_LEFT] = mouseActions[GLFW_MOUSE_BUTTON_LEFT];
						if (button->handlers[MOUSE_CLICK])
							button->handlers[MOUSE_CLICK](parent);
					}

					if (button->handlers[MOUSE_UP])
						button->handlers[MOUSE_UP](parent);
				}
			} else {
				if (prev) {
					if (button->handlers[MOUSE_OUT])
						button->handlers[MOUSE_OUT](parent);
					prev = false;
				}
			}
		}
	}

	for (auto& action : mouseActions) {
		prevMouseActions[action.first] = action.second;
	}
}

void ButtonSystem::CameraActiveHandler(Events::Event * event) {
	auto c = static_cast<Events::AnyType<Camera*>*>(event)->data;

	if (c->IsActive()) {
		cameras.push_back(c);
	} else {
		cameras.erase(vfind(cameras, c));
	}
}

void ButtonSystem::ButtonActiveHandler(Events::Event * event) {
	auto& c = static_cast<Events::AnyType<Button*>*>(event)->data;

	if (c->IsActive()) {
		buttons.push_back(c);
	} else {
		buttons.erase(vfind(buttons, c));
	}
}

void ButtonSystem::ResizeHandler(Events::Event* event) {
	windowSize = static_cast<Events::AnyType<vec2i>*>(event)->data;
}

void ButtonSystem::CursorPositionHandler(Events::Event* event) {
	Events::CursorPositionInput* input = static_cast<Events::CursorPositionInput*>(event);
	mousePosition = input->position;
	mousePosition.y = windowSize.h - mousePosition.y;
}

void ButtonSystem::MouseButtonHandler(Events::Event* event) {
	Events::MouseButtonInput* input = static_cast<Events::MouseButtonInput*>(event);
	mouseActions[input->button] = input->action;
}

vec2f ButtonSystem::GetWorldSpace(Camera * const camera) {
	const vec4f& viewport = camera->viewport;
	const vec2f viewportPosition = mousePosition - viewport.origin;

	vec2f unitPosition = viewportPosition / viewport.size;
	unitPosition = unitPosition * 2.f - vec2f(1.f);

	vec2f result = unitPosition * camera->size;
	result.x *= camera->aspectRatio;

	return result;
}
