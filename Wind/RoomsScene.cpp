#include "RoomsScene.h"

// game objects
#include "CameraObject.h"
#include "UIButton.h"
// components
#include "Transform.h"
#include "Animation.h"
#include "Render.h"
#include "Camera.h"
#include "Button.h"
#include "Text.h"
// utils
#include "LoadFNT.h"
#include "LoadTGA.h"

#include "ChatRoom.h"
#include "LobbyScene.h"

#include <Events/EventsManager.h>
#include <Helpers/String/StringHelpers.h>

RoomsScene::RoomsScene() {
	manager = new ChatManager;
}

RoomsScene::~RoomsScene() {
	delete manager;
}

void RoomsScene::Reset() {
	Scene::Reset();
}

void RoomsScene::Start() {
	Scene::Start();

	auto cam = entities->Create<CameraObject>();
	cam->GetComponent<Camera>()->SetSize(20);

	auto title = entities->Create<UILabel>();
	title->GetComponent<Transform>()->translation.y = 17.f;
	title->GetComponent<Render>()->tint.Set(0.f);
	title->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	title->GetComponent<Text>()->text = "Rooms";

	auto add = entities->Create<UIButton>();
	add->GetComponent<Transform>()->translation.x = 9.f;
	add->GetComponent<Transform>()->translation.y = 17.f;
	add->GetComponent<Render>()->SetTexture(Load::TGA("Files/Textures/add.tga"));
	add->GetComponent<Button>()->BindHandler(&RoomsScene::AddHandler, this, MOUSE_CLICK);

	auto join = entities->Create<UIButton>();
	join->GetComponent<Transform>()->translation.y = -18.5f;
	join->GetComponent<Transform>()->scale.Set(23.f, 3.f, 0.f);
	join->GetComponent<Render>()->tint.Set(0.f, 1.f, 1.f, 0.5f);
	join->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	join->GetComponent<Text>()->color.Set(0.f, 0.f, 0.f, 1.f);
	join->GetComponent<Text>()->scale = 0.65f;
	join->GetComponent<Text>()->text = "JOIN";
	join->GetComponent<Button>()->BindHandler(&RoomsScene::MouseOverHandler, this, MOUSE_OVER);
	join->GetComponent<Button>()->BindHandler(&RoomsScene::MouseOutHandler, this, MOUSE_OUT);
	join->GetComponent<Button>()->BindHandler(&RoomsScene::JoinHandler, this, MOUSE_CLICK);

	sockets.clear();
	for (auto& pair : manager->GetRooms()) {
		sockets.push_back(pair.first);
	}

	auto tableView = entities->Create<UITableView>();
	tableView->GetComponent<Transform>()->scale.Set(19.f, 19.f, 1.f);
	tableView->GetComponent<Render>()->tint.Set(0.f);
	tableView->BindNumberOfRowsHandler(&RoomsScene::NumberOfRows, this);
	tableView->BindCellForRowHandler(&RoomsScene::CellForRow, this);
	tableView->BindSelectHandler(&RoomsScene::DidSelectRow, this);
	tableView->ReloadData();
}

void RoomsScene::SetProfile(Profile * const _profile) {
	profile = _profile;
	manager->SetProfile(profile);
}

void RoomsScene::PrepareForSegue(Scene * destination) {
	if (dynamic_cast<ChatRoom*>(destination)) {
		auto dest = static_cast<ChatRoom*>(destination);
		auto client = sockets[selectedRow];
		dest->SetClient(client);
		dest->SetProfile(profile);
		dest->SetRoom(manager->GetRooms()[client]);
	} else if (dynamic_cast<LobbyScene*>(destination)) {
		auto dest = static_cast<LobbyScene*>(destination);
		dest->SetChatManager(manager);
	}
}

void RoomsScene::MouseOverHandler(Entity* target) {
	target->GetComponent<Animation>()->Animate(
		AnimationBase(false, 0.2f),
		target->GetComponent<Render>()->tint.a,
		1.f
	);
}

void RoomsScene::MouseOutHandler(Entity* target) {
	target->GetComponent<Animation>()->Animate(
		AnimationBase(false, 0.2f),
		target->GetComponent<Render>()->tint.a,
		0.5f
	);
}

void RoomsScene::AddHandler(Entity * target) {
}

void RoomsScene::JoinHandler(Entity * target) {
	Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::AnyType<std::string>("LOBBY"));
}

unsigned RoomsScene::NumberOfRows(UITableView * tableView) {
	return sockets.size();
}

void RoomsScene::CellForRow(UITableView * tableView, UITableViewCell * cell, unsigned row) {
	cell->GetComponent<Render>()->tint.Set(0.f, 0.f, 0.f, 1.f);
	auto sock = sockets[row];
	auto room = manager->GetRooms()[sock];
	cell->subtitle->GetComponent<Text>()->text = sock->GetIP() + "::" + Helpers::ToString(sock->GetPort());
	cell->title->GetComponent<Text>()->text = room->title;
}

void RoomsScene::DidSelectRow(UITableView * tableView, UITableViewCell * cell, unsigned row) {
	selectedRow = row;
	Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::AnyType<std::string>("CHAT_ROOM"));
}


