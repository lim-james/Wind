#include "LoginScene.h"

// game objects
#include "CameraObject.h"
#include "FPSLabel.h"
#include "UILabel.h"
#include "UIButton.h"
#include "UITextField.h"
// components
#include "Transform.h"
#include "Animation.h"
#include "Text.h"
#include "Render.h"
#include "Camera.h"
#include "Button.h"
#include "Script.h"
// utils
#include "LoadTGA.h"
#include "LoadFNT.h"
// events
#include "InputEvents.h"

#include <Helpers/String/StringHelpers.h>
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

#include <regex>

LoginScene::LoginScene() {
	profile = new Profile;
}

LoginScene::~LoginScene() {
	delete profile;
}

void LoginScene::Reset() {
	Scene::Reset();

	Events::EventsManager::GetInstance()->Subscribe("DROP_INPUT", &LoginScene::DropHandler, this);
}

void LoginScene::Start() {
	Scene::Start();

	auto cam = entities->Create<CameraObject>();
	cam->GetComponent<Camera>()->SetSize(20);

	imageView = entities->Create<Sprite>();
	imageView->GetComponent<Transform>()->translation.y = 10.f;
	imageView->GetComponent<Transform>()->scale.Set(4.f);
	imageView->GetComponent<Render>()->tint.Set(1.f);

	auto cursor = entities->Create<Sprite>();
	cursor->GetComponent<Transform>()->scale.Set(0.1f, 1.f, 1.f);
	cursor->GetComponent<Render>()->tint.Set(0.f, 1.f, 1.f, 1.f);

	nameField = CreateTextField("Name");
	nameField->SetCursor(cursor);
	nameField->Focus();
	nameField->BindDidReturn(&LoginScene::ReturnHandler, this);	

	statusField = CreateTextField("Status");
	statusField->SetCursor(cursor);
	statusField->GetComponent<Transform>()->translation.y = -2.f;
	statusField->BindDidReturn(&LoginScene::ReturnHandler, this);	

	confirm = entities->Create<UIButton>();
	confirm->GetComponent<Transform>()->translation.y = -10.f;
	confirm->GetComponent<Transform>()->scale.Set(5.f, 1.5f, 0.f);
	confirm->GetComponent<Render>()->tint.Set(0.f, 1.f, 1.f, 0.5f);
	confirm->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	confirm->GetComponent<Text>()->color.Set(0.f, 0.f, 0.f, 1.f);
	confirm->GetComponent<Text>()->scale = 0.65f;
	confirm->GetComponent<Text>()->text = "CONFIRM";
	confirm->GetComponent<Button>()->BindHandler(&LoginScene::MouseOverHandler, this, MOUSE_OVER);
	confirm->GetComponent<Button>()->BindHandler(&LoginScene::MouseOutHandler, this, MOUSE_OUT);
	confirm->GetComponent<Button>()->BindHandler(&LoginScene::MouseOnClick, this, MOUSE_CLICK);

}

void LoginScene::PrepareForSegue(Scene * destination) {
	auto dest = static_cast<LobbyScene*>(destination);
	dest->SetProfile(profile);
}

void LoginScene::DropHandler(Events::Event * event) {
	auto drop = static_cast<Events::DropInput*>(event);
	Console::Log << "Dragged in " << drop->paths[0] << '\n';
	imageView->GetComponent<Render>()->SetTexture(Load::TGA(drop->paths[0]));

	for (int i = 0; i < drop->count; ++i) {
			
	}
}

void LoginScene::ReturnHandler(UITextField* const target) {
	Login();
}

void LoginScene::MouseOverHandler(Entity* target) {
	target->GetComponent<Animation>()->Animate(
		AnimationBase(false, 0.2f),
		target->GetComponent<Render>()->tint.a,
		1.f
	);
}

void LoginScene::MouseOutHandler(Entity* target) {
	target->GetComponent<Animation>()->Animate(
		AnimationBase(false, 0.2f),
		target->GetComponent<Render>()->tint.a,
		0.5f
	);
}

void LoginScene::MouseOnClick(Entity * target) {
	Login();
}

UITextField * LoginScene::CreateTextField(const std::string & _prompt) {
	auto result = entities->Create<UITextField>();
	result->GetComponent<Transform>()->translation.y = 2.f;
	result->GetComponent<Transform>()->scale.Set(19.f, 2.f, 0.f);
	result->GetComponent<Render>()->tint.Set(0.f);
	result->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	result->GetComponent<Text>()->color.Set(1.f);
	result->GetComponent<Text>()->paragraphAlignment = PARAGRAPH_LEFT;

	auto back = entities->Create<UILabel>();
	back->SetParent(result);
	back->GetComponent<Transform>()->scale.Set(20.f, 2.f, 0.f);
	back->GetComponent<Render>()->tint.Set(0.25f);

	auto prompt = entities->Create<UILabel>();
	prompt->SetParent(result);
	prompt->GetComponent<Transform>()->translation.y = 2.f;
	prompt->GetComponent<Transform>()->scale.Set(20.f, 1.f, 0.f);
	prompt->GetComponent<Render>()->tint.Set(0.f);
	prompt->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	prompt->GetComponent<Text>()->color.Set(0.f, 1.f, 1.f, 1.f);
	prompt->GetComponent<Text>()->scale = 0.5f;
	prompt->GetComponent<Text>()->text = _prompt;
	prompt->GetComponent<Text>()->paragraphAlignment = PARAGRAPH_LEFT;
	
	return result;
}

void LoginScene::Login() {
	auto name = nameField->GetComponent<Text>()->text;
	name = Helpers::Trim(name);
	auto status = statusField->GetComponent<Text>()->text;
	status = Helpers::Trim(status);

	if (!name.empty()) {
		profile->name = name;
		profile->status = status;
		Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::AnyType<std::string>("LOBBY"));
	}
}


