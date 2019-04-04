#ifndef _GOLD_H__
#define _GOLD_H__

#include "LootEntity.h"

class Gold : public LootEntity
{
public:
	Gold(int posX, int posY, int value);
	~Gold();
	
	bool Update(float dt);
    virtual bool CleanUp();

public:


};
#endif // !_GOLD_H__
