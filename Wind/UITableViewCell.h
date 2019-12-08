#ifndef UI_TABLE_VIEW_CELL_H
#define UI_TABLE_VIEW_CELL_H

#include "Sprite.h"
#include "UILabel.h"

class UITableViewCell : public Sprite {

	unsigned row;

public:

	Sprite* image;
	UILabel* title;
	UILabel* subtitle;

	virtual void Build();

	friend class UITableView;

};

#endif
