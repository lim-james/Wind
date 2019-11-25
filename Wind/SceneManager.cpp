#include "SceneManager.h"

#include <Events/EventsManager.h>

SceneManager::SceneManager() 
	: source("") 
	, destination("") {
	Events::EventsManager().GetInstance()->Subscribe("PRESENT_SCENE", &SceneManager::PresentHandler, this);
}

SceneManager::~SceneManager() {
	for (auto& scenePair : scenes) {
		delete scenePair.second;
	}

	scenes.clear();
}

void SceneManager::Add(const std::string & indentifier, Scene * const scene) {
	scene->Awake();
	scene->Stop();
	scenes[indentifier] = scene;
}

Scene* SceneManager::GetSource() {
	return scenes[source];
}

void SceneManager::SetEntryPoint(const std::string& indentifier) {
	destination = indentifier;
	Segue();
}

void SceneManager::Segue() {
	if (destination != "") {
		auto s = scenes[source];
		auto d = scenes[destination];

		if (s) {
			s->PrepareForSegue(d);
			s->Stop();
			s->Destroy();
		}

		d->Reset();
		d->Start();
		
		source = destination;
		destination = "";
	}
 }

void SceneManager::PresentHandler(Events::Event * event) {
	destination = static_cast<Events::AnyType<std::string>*>(event)->data;
}

