#ifndef __WAVE_H__
#define __WAVE_H__

#include "cocos2d.h"

using namespace cocos2d;

class Wave : public Ref{
public:
	static Wave* create(std::vector<int> e, const float r);
	
	virtual bool init(std::vector<int> e, const float r);
	
	int getEnemy(const unsigned int i);
	int getSize();
	float getRate();
	
private:
	std::vector<int> _enemies;
	float _rate;

};

#endif // __WAVE_H__
