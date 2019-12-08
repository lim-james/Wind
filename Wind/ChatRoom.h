#ifndef CHAT_ROOM_H
#define CHAT_ROOM_H

#include "Profile.h"
#include "ChatManager.h"

#include "UIViewController.h"

// messaging helpers
#include "Codable.h"
#include "Message.h"
// elements
#include "UITextField.h"
#include "UITableView.h"

class ChatRoom : public UIViewController {

	Profile* profile;
	TCP* client;
	Room* room;

	UITextField* textField;
	UITableView* tableView;

	bool updated;

public:

	ChatRoom();
	~ChatRoom() override;

	void Reset() override;
	void Start() override;

	void Update(const float& dt) override;

	void SetProfile(Profile* const _profile);
	void SetClient(TCP* const _client);
	void SetRoom(Room* const _room);

private:

	void DropHandler(Events::Event* event);
	void ReadHandlers(Events::Event* event);

	void ChangeHandler(UITextField* const target);
	void ReturnHandler(UITextField* const target);

	void MouseOverHandler(Entity* target);
	void MouseOutHandler(Entity* target);
	void MouseOnClick(Entity* target);
	void Close(Entity* target);

	unsigned NumberOfRows(UITableView* tableView);
	void CellForRow(UITableView* tableView, UITableViewCell* cell, unsigned row);

	UITextField* CreateTextField(const std::string& _prompt = "");

	void CommandHandler(const std::string& command);

};

#endif
