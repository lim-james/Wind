#include "CreateScene.h"

// game objects
#include "CameraObject.h"
#include "FPSLabel.h"
#include "UILabel.h"
#include "UIButton.h"
#include "UITextField.h"
// components
#include "Transform.h"
#include "Animation.h"
#include "Render.h"
#include "Camera.h"
#include "Button.h"
#include "Script.h"
// systems
#include "AnimationSystem.h"
#include "RenderSystem.h"
#include "ButtonSystem.h"
#include "ScriptSystem.h"
// utils
#include "LoadTGA.h"
#include "LoadFNT.h"
// events
#include "InputEvents.h"
// destination scenes
#include "ChatRoom.h"

#include <Events/EventsManager.h>
#include <Helpers/File/FileHelpers.h>
#include <Helpers/String/StringHelpers.h>
#include <GLFW/glfw3.h>

CreateScene::CreateScene() { }

CreateScene::~CreateScene() { }

void CreateScene::Start() {
	UIViewController::Start();

	auto cam = entities->Create<CameraObject>();
	cam->GetComponent<Camera>()->SetSize(20);

	auto title = entities->Create<UILabel>();
	title->GetComponent<Transform>()->translation.y = 17.f;
	title->GetComponent<Render>()->tint.Set(0.f);
	title->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	title->GetComponent<Text>()->text = "Create";

	auto close = entities->Create<UIButton>();
	close->GetComponent<Transform>()->translation.x = -9.f;
	close->GetComponent<Transform>()->translation.y = 17.f;
	close->GetComponent<Render>()->SetTexture(Load::TGA("Files/Textures/close.tga"));
	close->GetComponent<Button>()->BindHandler(&CreateScene::Close, this, MOUSE_CLICK);

	auto cursor = entities->Create<Sprite>();
	cursor->GetComponent<Transform>()->scale.Set(0.1f, 1.f, 1.f);
	cursor->GetComponent<Render>()->tint.Set(0.f, 1.f, 1.f, 1.f);

	//titleField = CreateTextField("Title:");
	//titleField->SetCursor(cursor);
	//titleField->Focus();
	//titleField->BindDidChange(&CreateScene::DidChangeHandler, this);
	//titleField->BindDidReturn(&CreateScene::ReturnHandler, this);

	portField = CreateTextField("Port:");
	portField->SetCursor(cursor);
	portField->Focus();
	//portField->GetComponent<Transform>()->translation.y = -2.f;
	portField->BindDidReturn(&CreateScene::ReturnHandler, this);

	confirm = entities->Create<UIButton>();
	confirm->GetComponent<Transform>()->translation.y = -10.f;
	confirm->GetComponent<Transform>()->scale.Set(5.f, 1.5f, 0.f);
	confirm->GetComponent<Render>()->tint.Set(0.f, 1.f, 1.f, 0.5f);
	confirm->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	confirm->GetComponent<Text>()->color.Set(0.f, 0.f, 0.f, 1.f);
	confirm->GetComponent<Text>()->scale = 0.65f;
	confirm->GetComponent<Text>()->text = "CONFIRM";
	//confirm->GetComponent<Button>()->isEnabled = false;
	confirm->GetComponent<Button>()->BindHandler(&CreateScene::MouseOverHandler, this, MOUSE_OVER);
	confirm->GetComponent<Button>()->BindHandler(&CreateScene::MouseOutHandler, this, MOUSE_OUT);
	confirm->GetComponent<Button>()->BindHandler(&CreateScene::MouseOnClick, this, MOUSE_CLICK);
}

void CreateScene::PrepareForSegue(Scene * destination) {
	if (dynamic_cast<ChatRoom*>(destination)) {
		auto dest = static_cast<ChatRoom*>(destination);
		dest->SetProfile(manager->GetProfile());
		dest->SetClient(client);
		dest->SetRoom(manager->GetRoom(client));
	}
}

void CreateScene::SetChatManager(ChatManager * const _manager) {
	manager = _manager;
}

//void CreateScene::DidChangeHandler(UITextField * const target) {
//	auto text = target->GetComponent<Text>();
//	auto& textColor = text->color;
//
//	if (titleField->GetComponent<Text>()->text.empty() || titleField->GetComponent<Text>()->text.empty()) {
//		textColor.Set(1.f, 0.f, 0.f, 1.f);
//
//		confirm->GetComponent<Button>()->isEnabled = false;
//		confirm->GetComponent<Animation>()->Animate(
//			AnimationBase(false, 0.2f),
//			confirm->GetComponent<Render>()->tint,
//			vec4f(1.f, 0.f, 0.f, 0.5f)
//		);
//	} else {
//		textColor.Set(1.f);
//
//		confirm->GetComponent<Button>()->isEnabled = true;
//		confirm->GetComponent<Animation>()->Animate(
//			AnimationBase(false, 0.2f),
//			confirm->GetComponent<Render>()->tint,
//			vec4f(0.f, 1.f, 1.f, 0.5f)
//		);
//	}
//}

void CreateScene::ReturnHandler(UITextField* const target) {
	Join();
}

void CreateScene::MouseOverHandler(Entity* target) {
	target->GetComponent<Animation>()->Animate(
		AnimationBase(false, 0.2f),
		target->GetComponent<Render>()->tint.a,
		1.f
	);
}

void CreateScene::MouseOutHandler(Entity* target) {
	target->GetComponent<Animation>()->Animate(
		AnimationBase(false, 0.2f),
		target->GetComponent<Render>()->tint.a,
		0.5f
	);
}

void CreateScene::MouseOnClick(Entity * target) {
	Join();
}

void CreateScene::Close(Entity * target) {
	Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::AnyType<std::string>("ROOMS"));
}

void CreateScene::Join() {
	if (!confirm->GetComponent<Button>()->isEnabled) return;

	//auto& title = titleField->GetComponent<Text>()->text;
	auto port = Helpers::Parse<USHORT>(portField->GetComponent<Text>()->text);

	manager->Host(port, "");

	client = manager->Join(TCP::GetLocalIP(), port, [this](TCP* _client) {
		Content message;
		message.type = JOIN_CONTENT;
		auto profile = manager->GetProfile();
		if (!profile->picture.empty()) {
			message.title = Helpers::GetFileName(profile->picture);
			message.body = Helpers::ReadFile(profile->picture);
		}
		_client->Send(Codable::Join(2, profile, &message));

		Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::AnyType<std::string>("CHAT_ROOM"));
	});
}
