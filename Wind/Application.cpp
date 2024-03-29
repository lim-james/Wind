#include "Application.h"

#include "InputEvents.h"
#include "LoadFNT.h"
// external
#include <Events/EventsManager.h>
#include <Logger/Logger.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// standard
#include <iostream>
#include <thread>

Application::Application()
	: context(nullptr) {}

void Application::Initialize(const int& width, const int& height, const char* title, const bool& fullscreen) {
	// initialize GLFW
	if (!glfwInit()) {
		Console::Error << "GLFW INITIALISATION FAILED.\n";
		return;
	}

	context = new Window(width, height, title, fullscreen);
	inputController.Initialize(context->Get());

	// initialize GLEW
	glewExperimental = true;
	const GLenum err = glewInit();
	if (err != GLEW_OK) {
		Console::Error << glewGetErrorString(err) << '\n';
	}

	auto em = Events::EventsManager::GetInstance();

	em->Subscribe("CLOSE", &Window::Close, context);
	em->Subscribe("KEY_INPUT", &Application::OnEvent, this);
	em->Subscribe("TEXT_INPUT", &Application::OnEvent, this);
	em->Subscribe("CURSOR_POSITION_INPUT", &Application::OnEvent, this);
	em->Subscribe("MOUSE_BUTTON_INPUT", &Application::OnEvent, this);
	em->Subscribe("SCROLL_INPUT", &Application::OnEvent, this);

#if _DEBUG
	em->Subscribe("TIMER_START", &Application::OnTimerEvent, this);
	em->Subscribe("TIMER_STOP", &Application::OnTimerEvent, this);
#endif

	sceneManager = new SceneManager;
	sceneManager->Add("LOGIN", new LoginScene);
	sceneManager->Add("ROOMS", new RoomsScene);
	sceneManager->Add("CREATE", new CreateScene);
	sceneManager->Add("LOBBY", new LobbyScene);
	sceneManager->Add("CHAT_ROOM", new ChatRoom);
	sceneManager->SetEntryPoint("LOGIN");
	// turn off vsync
	//glfwSwapInterval(0);

	context->BroadcastSize();
	em->TriggerQueued();
}

void Application::Run() {
	timer.Start();

	auto em = Events::EventsManager::GetInstance();

	em->Trigger("CURSOR_SENSITIVITY", new Events::AnyType<float>(0.1f));

	float t = 0.f;

	while (!context->ShouldClose()) {
		glfwPollEvents();

		const float et = static_cast<float>(timer.GetElapsedTime());
		const float dt = static_cast<float>(timer.GetDeltaTime());

		auto current = sceneManager->GetSource();


		t += dt;
		if (t >= FRAMERATE) {
			current->FixedUpdate(t);
			t = 0.f;
		}

		current->Update(dt);

		em->TriggerQueued();

		sceneManager->Segue();
		context->SwapBuffers();
		timer.Update();
	}
}

void Application::Exit() {
#if _DEBUG
	timers.clear();
#endif

	delete context;
	delete sceneManager;

	Load::ClearFontCache();

	Events::EventsManager::Destroy();
}

void Application::OnEvent(Events::Event* event) {
	if (event->name == "KEY_INPUT") {
		Events::KeyInput* input = static_cast<Events::KeyInput*>(event);
		// quit program if escaped
		if (input->key == GLFW_KEY_ESCAPE && input->action == GLFW_RELEASE) {
			Events::EventsManager::GetInstance()->Trigger("CLOSE");
			return;
		}
	} else if (event->name == "CURSOR_POSITION_INPUT") {
		Events::CursorPositionInput* input = static_cast<Events::CursorPositionInput*>(event);
	} else if (event->name == "MOUSE_BUTTON_INPUT") {
		Events::MouseButtonInput* input = static_cast<Events::MouseButtonInput*>(event);
	} else if (event->name == "SCROLL_INPUT") {
		Events::ScrollInput* input = static_cast<Events::ScrollInput*>(event);
	}
}

#if _DEBUG
void Application::OnTimerEvent(Events::Event* event) {
	const auto timer = static_cast<Events::AnyType<std::string>*>(event);

	if (event->name == "TIMER_START") {
		timers[timer->data].Start();
	} else if (event->name == "TIMER_STOP") {
		timers[timer->data].Update();
		Console::Log << "[TIMER] " << timer->data << " : " << timers[timer->data].GetElapsedTime() << '\n';
	}
}
#endif
