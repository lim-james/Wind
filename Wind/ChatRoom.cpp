#include "ChatRoom.h"

// game objects
#include "CameraObject.h"
#include "FPSLabel.h"
#include "UILabel.h"
#include "UIButton.h"
#include "UITextField.h"
// components
#include "Transform.h"
#include "Render.h"
#include "Camera.h"
#include "Script.h"
// systems
#include "RenderSystem.h"
#include "ScriptSystem.h"
// utils
#include "LoadFNT.h"
// events
#include "InputEvents.h"

#include <GLFW/glfw3.h>

ChatRoom::ChatRoom() {
	components->Subscribe<Transform>(10, 1);
	components->Subscribe<Render>(10, 1);
	components->Subscribe<Text>(10, 1);
	components->Subscribe<Camera>(1, 1);
	components->Subscribe<Script>(1, 1);
		
	entities->Subscribe<Sprite>(1, 1);
	entities->Subscribe<UILabel>(10, 1);
	entities->Subscribe<UIButton>(10, 1);
	entities->Subscribe<UITextField>(1, 1);
	entities->Subscribe<FPSLabel>(1, 1);
	entities->Subscribe<CameraObject>(1, 1);

	systems->Subscribe<RenderSystem>();
	systems->Subscribe<ScriptSystem>();
}

ChatRoom::~ChatRoom() {
	delete client;
}

void ChatRoom::Awake() {
	Scene::Awake();

	auto cam = entities->Create<CameraObject>();
	cam->GetComponent<Camera>()->clearColor.Set(0.f);
	cam->GetComponent<Camera>()->SetSize(10);

	// most performant : 500 - 700 FPS
	//auto fps = entities->Create<FPSLabel>();
	////fps->GetComponent<Transform>()->translation.Set(gridSize, 0.f, 0.f);
	//fps->GetComponent<Transform>()->scale.Set(4.f);
	//fps->GetComponent<Render>()->tint.Set(0.01f);
	//fps->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	//fps->GetComponent<Text>()->text = "60";
	//fps->GetComponent<Text>()->paragraphAlignment = PARAGRAPH_RIGHT;
	//fps->GetComponent<Text>()->verticalAlignment = ALIGN_BOTTOM;

	cursor = entities->Create<Sprite>();
	cursor->GetComponent<Transform>()->scale.Set(0.1f, 1.f, 1.f);
	cursor->GetComponent<Render>()->tint.Set(0.f, 1.f, 1.f, 1.f);

	auto back = entities->Create<UILabel>();
	back->GetComponent<Transform>()->scale.Set(20.f, 2.f, 0.f);
	back->GetComponent<Render>()->tint.Set(0.25f);

	auto textField = entities->Create<UITextField>();
	textField->GetComponent<Transform>()->scale.Set(19.f, 2.f, 0.f);
	textField->GetComponent<Render>()->tint.Set(0.f);
	textField->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	textField->GetComponent<Text>()->color.Set(1.f);
	textField->GetComponent<Text>()->paragraphAlignment = PARAGRAPH_LEFT;
	textField->SetCursor(cursor);
	textField->BindDidReturn(&ChatRoom::ReturnHandler, this);

	auto prompt = entities->Create<UILabel>();
	prompt->GetComponent<Transform>()->translation.y = 2.f;
	prompt->GetComponent<Transform>()->scale.Set(20.f, 1.f, 0.f);
	prompt->GetComponent<Render>()->tint.Set(0.f);
	prompt->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	prompt->GetComponent<Text>()->color.Set(0.f, 1.f, 1.f, 1.f);
	prompt->GetComponent<Text>()->scale = 0.5f;
	prompt->GetComponent<Text>()->text = "James:";
	prompt->GetComponent<Text>()->paragraphAlignment = PARAGRAPH_LEFT;

	auto confirm = entities->Create<UILabel>();
	confirm->GetComponent<Transform>()->translation.y = -5.f;
	confirm->GetComponent<Transform>()->scale.Set(5.f, 1.5f, 0.f);
	confirm->GetComponent<Render>()->tint.Set(0.f, 1.f, 1.f, 0.5f);
	confirm->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	confirm->GetComponent<Text>()->color.Set(0.f, 0.f, 0.f, 1.f);
	confirm->GetComponent<Text>()->scale = 0.5f;
	confirm->GetComponent<Text>()->text = "JOIN";

	client = new TCP(8230);
	client->Connect("127.0.0.1");
}

void ChatRoom::ReturnHandler(UITextField* const target) {
	client->Send(target->GetComponent<Text>()->text);
}
