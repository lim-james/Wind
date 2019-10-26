#ifndef SYSTEM_H
#define SYSTEM_H

class System {

public:

	virtual ~System();

	virtual void Initialize();
	virtual void Update(const float& t) = 0;

};

#endif
