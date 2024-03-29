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
#include "Text.h"
// utils
#include "LoadFNT.h"
#include "LoadTGA.h"
// events
#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <Helpers/String/StringHelpers.h>
#include <Helpers/File/FileHelpers.h>
#include <GLFW/glfw3.h>

#include <thread>

ChatRoom::ChatRoom() {  }

ChatRoom::~ChatRoom() { }

void ChatRoom::Reset() {
	UIViewController::Reset();

	Events::EventsManager::GetInstance()->Subscribe("DROP_INPUT", &ChatRoom::DropHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("NEW_MESSAGE", &ChatRoom::ReadHandlers, this);
}

void ChatRoom::Start() {
	UIViewController::Start();

	auto cam = entities->Create<CameraObject>();
	cam->GetComponent<Camera>()->SetSize(20);

	auto title = entities->Create<UILabel>();
	title->GetComponent<Transform>()->translation.y = 17.f;
	title->GetComponent<Render>()->tint.Set(0.f);
	title->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	title->GetComponent<Text>()->text = "Room";

	auto close = entities->Create<UIButton>();
	close->GetComponent<Transform>()->translation.x = -9.f;
	close->GetComponent<Transform>()->translation.y = 17.f;
	close->GetComponent<Render>()->SetTexture(Load::TGA("Files/Textures/close.tga"));
	close->GetComponent<Button>()->BindHandler(&ChatRoom::Close, this, MOUSE_CLICK);

	auto cursor = entities->Create<Sprite>();
	cursor->SetTag("CURSOR");
	cursor->GetComponent<Transform>()->scale.Set(0.1f, 1.f, 1.f);
	cursor->GetComponent<Render>()->tint.Set(0.f, 1.f, 1.f, 1.f);

	textField = CreateTextField(profile->name);
	textField->GetComponent<Transform>()->translation.y = -17.f;
	textField->SetCursor(cursor);
	textField->Focus();
	textField->BindDidChange(&ChatRoom::ChangeHandler, this);
	textField->BindDidReturn(&ChatRoom::ReturnHandler, this);

	tableView = entities->Create<UITableView>();
	tableView->GetComponent<Transform>()->scale.Set(19.f, 19.f, 1.f);
	tableView->GetComponent<Render>()->tint.Set(0.f);
	tableView->BindNumberOfRowsHandler(&ChatRoom::NumberOfRows, this);
	tableView->BindCellForRowHandler(&ChatRoom::CellForRow, this);
	tableView->ReloadData();
}

void ChatRoom::Update(const float & dt) {
	UIViewController::Update(dt);

	if (updated) {
		tableView->ReloadData();
		updated = false;
	}
}

void ChatRoom::SetProfile(Profile * const _profile) {
	profile = _profile;
}

void ChatRoom::SetClient(TCP * const _client) {
	client = _client;
}

void ChatRoom::SetRoom(Room * const _room) {
	room = _room;
}

void ChatRoom::DropHandler(Events::Event * event) {
	auto drop = static_cast<Events::DropInput*>(event);

	for (int i = 0; i < drop->count; ++i) {
		const auto path = drop->paths[i];

		const auto contents = Helpers::ReadFile(path);
		const auto groups = Helpers::Group(contents, 4096);

		Content message;
		message.type = FILE_CONTENT;
		message.title = Helpers::GetFileName(path);
		message.body = contents;
		client->Send(Codable::Join(2, profile, &message));
	}
}

void ChatRoom::ReadHandlers(Events::Event* event) {
	if (static_cast<Events::AnyType<TCP*>*>(event)->data == client)
		updated = true;
}

void ChatRoom::ChangeHandler(UITextField * const target) {
	auto component = target->GetComponent<Text>();

	if (component->text.empty()) return;

	if (component->text.front() == '/') {
		component->color.Set(0.8f);
	} else {
		component->color.Set(1.f);
	}
}

void ChatRoom::ReturnHandler(UITextField * const target) {
	auto text = target->GetComponent<Text>()->text;
	text = Helpers::Trim(text);
	if (text.empty()) return;

	if (text.front() == '/') {
		CommandHandler(text);
	} else {
		Content message;
		message.body = text;
		client->Send(Codable::Join(2, profile, &message));
	}

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

void ChatRoom::Close(Entity * target) {
	Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::AnyType<std::string>("ROOMS"));
}

unsigned ChatRoom::NumberOfRows(UITableView * tableView) {
	return room->messages.size();
}

void ChatRoom::CellForRow(UITableView * tableView, UITableViewCell * cell, unsigned row) {
	cell->GetComponent<Render>()->tint.Set(0.f, 0.f, 0.f, 1.f);
	const int i = room->messages.size() - 1 - row;
	auto current = room->messages[i];
	cell->subtitle->GetComponent<Text>()->text = current.profile.name + " is " + current.profile.status;

	switch (current.content.type) {
	case TEXT_CONTENT:
		cell->title->GetComponent<Text>()->text = current.content.body;
		break;
	case FILE_CONTENT:
		cell->title->GetComponent<Text>()->text = current.content.title;
		break;
	case JOIN_CONTENT:
		cell->title->GetComponent<Text>()->text = "Welcome " + current.profile.name;
		break;
	default:
		break;
	}

	if (!current.profile.picture.empty())
		cell->image->GetComponent<Render>()->SetTexture(Load::TGA(current.profile.picture));

	if (row == 0) {
		cell->title->GetComponent<Text>()->color.a = 0.f;
		cell->GetComponent<Animation>()->Animate(
			AnimationBase(false, 0.25f),
			cell->title->GetComponent<Text>()->color.a,
			1.f
		);
	}
}

void ChatRoom::CommandHandler(const std::string & command) {
	auto list = Helpers::Split(command, ' ');
	auto name = list[0];

	if (name == "/iam") {
		list.erase(list.begin());
		profile->status = Helpers::Join(list, ' ');
	}
}
