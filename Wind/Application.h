#ifndef APPLICATION_H
#define APPLICATION_H

#include "Window.h"
#include "InputController.h"
#include "Scene.h"

#include <Timer/Timer.h>
#include <Events/Event.h>

#include <map>

class Application {

	const unsigned FPS = 60;
	const unsigned FRAMERATE = 1000 / FPS;

#if _DEBUG
	std::map<std::string, Timer> timers;
#endif

	Timer timer;

	Window* context;
	InputController inputController;

	Scene* current;

public:

	Application();

	void Initialize(const int& width, const int& height, const char* title, const bool& fullscreen);
	void Run();
	void Exit();

private:

	void OnEvent(Events::Event* event);

#if _DEBUG
	void OnTimerEvent(Events::Event* event);
#endif

};

#endif
