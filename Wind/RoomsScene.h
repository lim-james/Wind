#ifndef ROOMS_SCENE_H
#define ROOMS_SCENE_H

#include "ChatManager.h"
#include "ChatRoom.h"

class RoomsScene : public UIViewController {

	Profile* profile;
	ChatManager* manager;
	std::vector<TCP*> sockets;

	unsigned selectedRow;

public:

	RoomsScene();
	~RoomsScene() override;

	void Reset() override;
	void Start() override;

	void SetProfile(Profile* const _profile);

	void PrepareForSegue(Scene* destination) override;

private:

	void MouseOverHandler(Entity* target);
	void MouseOutHandler(Entity* target);
	void AddHandler(Entity* target);
	void JoinHandler(Entity* target);

	unsigned NumberOfRows(UITableView* tableView);
	void CellForRow(UITableView* tableView, UITableViewCell* cell, unsigned row);
	void DidSelectRow(UITableView* tableView, UITableViewCell* cell, unsigned row);

};

#endif
