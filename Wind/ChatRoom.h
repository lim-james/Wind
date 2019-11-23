#ifndef CHAT_ROOM_H
#define CHAT_ROOM_H

#include "TCP.h"

#include "Scene.h"

#include "UITextField.h"
#include "UITableView.h"

class ChatRoom : public Scene {

	TCP* client;

	UITextField* textField;
	UITableView* tableView;

	bool updated;
	std::vector<std::string> messages;

public:

	ChatRoom();
	~ChatRoom() override;

	void Awake() override;
	void Start() override;

	void Update(const float& dt) override;

	void SetClient(TCP* const _client);

private:

	void ReturnHandler(UITextField* const target);

	void MouseOverHandler(Entity* target);
	void MouseOutHandler(Entity* target);
	void MouseOnClick(Entity* target);

	unsigned NumberOfRows(UITableView* tableView);
	void CellForRow(UITableView* tableView, UITableViewCell* cell, unsigned row);

	UITextField* CreateTextField(const std::string& _prompt = "");

};

#endif
