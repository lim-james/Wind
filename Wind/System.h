#ifndef SYSTEM_H
#define SYSTEM_H

class System {

public:

	virtual ~System();

	virtual void Start();
	virtual void Stop();

	virtual void Initialize();
	virtual void Update(const float& dt) = 0;
	virtual void FixedUpdate(const float& dt) = 0;

};

#endif
