#ifndef CHAT_ROOM_H
#define CHAT_ROOM_H

#include "TCP.h"

#include "Scene.h"

// messaging helpers
#include "Codable.h"
#include "Message.h"
#include "Profile.h"
// elements
#include "UITextField.h"
#include "UITableView.h"

class ChatRoom : public Scene {

	TCP* client;

	Profile* profile;

	UITextField* textField;
	UITableView* tableView;

	bool updated;
	std::vector<Message> messages;

public:

	ChatRoom();
	~ChatRoom() override;

	void Awake() override;
	void Reset() override;
	void Start() override;

	void Update(const float& dt) override;

	void SetClient(TCP* const _client);

private:

	void DropHandler(Events::Event* event);

	void ReadThread();

	void ChangeHandler(UITextField* const target);
	void ReturnHandler(UITextField* const target);

	void MouseOverHandler(Entity* target);
	void MouseOutHandler(Entity* target);
	void MouseOnClick(Entity* target);

	unsigned NumberOfRows(UITableView* tableView);
	void CellForRow(UITableView* tableView, UITableViewCell* cell, unsigned row);

	UITextField* CreateTextField(const std::string& _prompt = "");

	void CommandHandler(const std::string& command);

};

#endif
