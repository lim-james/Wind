#include "ChatRoom.h"

// game objects
#include "CameraObject.h"
#include "FPSLabel.h"
#include "UILabel.h"
#include "UIButton.h"
#include "UITextField.h"
#include "UITableView.h"
#include "UITableViewCell.h"
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
#include "LoadFNT.h"
// events
#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

#include <thread>

ChatRoom::ChatRoom() {

}

ChatRoom::~ChatRoom() {}

void ChatRoom::Awake() {
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
	entities->Subscribe<UITableView>(1, 1);
	entities->Subscribe<UITableView>(10, 1);
	entities->Subscribe<CameraObject>(1, 1);

	systems->Subscribe<AnimationSystem>();
	systems->Subscribe<RenderSystem>();
	systems->Subscribe<ButtonSystem>();
	systems->Subscribe<ScriptSystem>();
}

void ChatRoom::Start() {
	Scene::Start();

	auto cam = entities->Create<CameraObject>();
	cam->GetComponent<Camera>()->SetSize(20);

	auto cursor = entities->Create<Sprite>();
	cursor->GetComponent<Transform>()->scale.Set(0.1f, 1.f, 1.f);
	cursor->GetComponent<Render>()->tint.Set(0.f, 1.f, 1.f, 1.f);

	textField = CreateTextField("James");
	textField->GetComponent<Transform>()->translation.y = -17.f;
	textField->SetCursor(cursor);
	textField->Focus();
	textField->BindDidReturn(&ChatRoom::ReturnHandler, this);

	tableView = entities->Create<UITableView>();
	tableView->GetComponent<Transform>()->scale.Set(19.f, 19.f, 1.f);
	tableView->GetComponent<Render>()->tint.Set(0.f);
	tableView->BindNumberOfRowsHandler(&ChatRoom::NumberOfRows, this);
	tableView->BindCellForRowHandler(&ChatRoom::CellForRow, this);
	tableView->ReloadData();

	std::thread([this]() {
		while (true) {
			auto result = client->Receive();
			if (result != "") {
				messages.push_back(result);
				updated = true;
			}
		}
	}).detach();
}

void ChatRoom::Update(const float & dt) {
	Scene::Update(dt);

	if (updated) {
		tableView->ReloadData();
		updated = false;
	}
}

void ChatRoom::SetClient(TCP * const _client) {
	client = _client;
}

void ChatRoom::ReturnHandler(UITextField * const target) {
	client->Send(target->GetComponent<Text>()->text);
}

void ChatRoom::MouseOverHandler(Entity * target) {
	target->GetComponent<Animation>()->Animate(
		AnimationBase(false, 0.2f),
		target->GetComponent<Render>()->tint.a,
		1.f
	);
}

void ChatRoom::MouseOutHandler(Entity * target) {
	target->GetComponent<Animation>()->Animate(
		AnimationBase(false, 0.2f),
		target->GetComponent<Render>()->tint.a,
		0.5f
	);
}

void ChatRoom::MouseOnClick(Entity * target) {
	Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::AnyType<std::string>("LOBBY"));
}

unsigned ChatRoom::NumberOfRows(UITableView * tableView) {
	return messages.size();
}

void ChatRoom::CellForRow(UITableView * tableView, UITableViewCell * cell, unsigned row) {
	cell->GetComponent<Render>()->tint.Set(0.f, 0.f, 0.f, 1.f);
	const int i = messages.size() - 1 - row;
	cell->title->GetComponent<Text>()->text = messages[i];

	if (row == 0) {
		cell->title->GetComponent<Text>()->color.a = 0.f;
		cell->GetComponent<Animation>()->Animate(
			AnimationBase(false, 0.25f),
			cell->title->GetComponent<Text>()->color.a,
			1.f
		);
	}
}

UITextField * ChatRoom::CreateTextField(const std::string & _prompt) {
	auto result = entities->Create<UITextField>();
	result->GetComponent<Transform>()->translation.z = -10.f;
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
	prompt->GetComponent<Transform>()->scale.Set(19.f, 1.f, 0.f);
	prompt->GetComponent<Render>()->tint.Set(0.f);
	prompt->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	prompt->GetComponent<Text>()->color.Set(0.f, 1.f, 1.f, 1.f);
	prompt->GetComponent<Text>()->scale = 0.5f;
	prompt->GetComponent<Text>()->text = _prompt;
	prompt->GetComponent<Text>()->paragraphAlignment = PARAGRAPH_LEFT;

	return result;
}
