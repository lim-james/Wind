#include "LobbyScene.h"

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

#include <regex>

LobbyScene::LobbyScene() {
	client = new TCP();
}

LobbyScene::~LobbyScene() {
	delete client;
}

void LobbyScene::Awake() {
	Scene::Awake();

	components->Subscribe<Transform>(10, 1);
	components->Subscribe<Animation>(10, 1);
	components->Subscribe<Render>(10, 1);
	components->Subscribe<Text>(10, 1);
	components->Subscribe<Camera>(1, 1);
	components->Subscribe<Button>(10, 1);
	components->Subscribe<Script>(1, 1);
		
	entities->Subscribe<Sprite>(1, 1);
	entities->Subscribe<UILabel>(10, 1);
	entities->Subscribe<UIButton>(10, 1);
	entities->Subscribe<UITextField>(1, 1);
	entities->Subscribe<FPSLabel>(1, 1);
	entities->Subscribe<CameraObject>(1, 1);

	systems->Subscribe<AnimationSystem>();
	systems->Subscribe<RenderSystem>();
	systems->Subscribe<ButtonSystem>();
	systems->Subscribe<ScriptSystem>();
}

void LobbyScene::Start() {
	Scene::Start();

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
	close->GetComponent<Button>()->BindHandler(&LobbyScene::Close, this, MOUSE_CLICK);

	auto cursor = entities->Create<Sprite>();
	cursor->GetComponent<Transform>()->scale.Set(0.1f, 1.f, 1.f);
	cursor->GetComponent<Render>()->tint.Set(0.f, 1.f, 1.f, 1.f);

	ipField = CreateTextField("IP Address:");
	ipField->SetCursor(cursor);
	ipField->Focus();
	ipField->BindDidChange(&LobbyScene::DidChangeHandler, this);
	ipField->BindDidReturn(&LobbyScene::ReturnHandler, this);	

	portField = CreateTextField("Port:");
	portField->SetCursor(cursor);
	portField->GetComponent<Transform>()->translation.y = -2.f;

	confirm = entities->Create<UIButton>();
	confirm->GetComponent<Transform>()->translation.y = -10.f;
	confirm->GetComponent<Transform>()->scale.Set(5.f, 1.5f, 0.f);
	confirm->GetComponent<Render>()->tint.Set(0.f, 1.f, 1.f, 0.5f);
	confirm->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	confirm->GetComponent<Text>()->color.Set(0.f, 0.f, 0.f, 1.f);
	confirm->GetComponent<Text>()->scale = 0.65f;
	confirm->GetComponent<Text>()->text = "JOIN";
	confirm->GetComponent<Button>()->isEnabled = false;
	confirm->GetComponent<Button>()->BindHandler(&LobbyScene::MouseOverHandler, this, MOUSE_OVER);
	confirm->GetComponent<Button>()->BindHandler(&LobbyScene::MouseOutHandler, this, MOUSE_OUT);
	confirm->GetComponent<Button>()->BindHandler(&LobbyScene::MouseOnClick, this, MOUSE_CLICK);
}

void LobbyScene::PrepareForSegue(Scene * destination) {
	if (dynamic_cast<ChatRoom*>(destination)) {
		auto dest = static_cast<ChatRoom*>(destination);
		dest->SetProfile(manager->GetProfile());
		dest->SetClient(client);
		dest->SetRoom(manager->GetRoom(client));
	}
}

void LobbyScene::SetChatManager(ChatManager * const _manager) {
	manager = _manager;
}

void LobbyScene::DidChangeHandler(UITextField * const target) {
	auto text = target->GetComponent<Text>();
	auto& textColor = text->color;

	const std::regex ipCheck("(\\d{1,3}(\\.\\d{1,3}){3})");

	if (std::regex_match(text->text, ipCheck)) {
		textColor.Set(1.f);

		confirm->GetComponent<Button>()->isEnabled = true;
		confirm->GetComponent<Animation>()->Animate(
			AnimationBase(false, 0.2f),
			confirm->GetComponent<Render>()->tint,
			vec4f(0.f, 1.f, 1.f, 0.5f)
		);
	} else {
		textColor.Set(1.f, 0.f, 0.f, 1.f);

		confirm->GetComponent<Button>()->isEnabled = false;
		confirm->GetComponent<Animation>()->Animate(
			AnimationBase(false, 0.2f),
			confirm->GetComponent<Render>()->tint,
			vec4f(1.f, 0.f, 0.f, 0.5f)
		);
	}
}

void LobbyScene::ReturnHandler(UITextField* const target) {
	Connect();
}

void LobbyScene::MouseOverHandler(Entity* target) {
	target->GetComponent<Animation>()->Animate(
		AnimationBase(false, 0.2f),
		target->GetComponent<Render>()->tint.a,
		1.f
	);
}

void LobbyScene::MouseOutHandler(Entity* target) {
	target->GetComponent<Animation>()->Animate(
		AnimationBase(false, 0.2f),
		target->GetComponent<Render>()->tint.a,
		0.5f
	);
}

void LobbyScene::MouseOnClick(Entity * target) {
	Connect();
}

void LobbyScene::Close(Entity * target) {
	Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::AnyType<std::string>("ROOMS"));
}

UITextField * LobbyScene::CreateTextField(const std::string & _prompt) {
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

void LobbyScene::Connect() {
	if (!confirm->GetComponent<Button>()->isEnabled) return;

	auto& ip = ipField->GetComponent<Text>()->text;
	auto port = Helpers::Parse<USHORT>(portField->GetComponent<Text>()->text);
	Console::Warn << "connecting to " << ip << "::" << port << '\n';

	client = manager->Join(ip, port, [this](TCP* _client) {
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
