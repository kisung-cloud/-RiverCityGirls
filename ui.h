#pragma once
#include "gameNode.h"
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

class ui : public gameNode
{
private:
	image* _hpUI;
	image* _selectArrow;
	image* _loading;
	RECT _hpRC;
	RECT _inventory[10];

	int _itemSelectIndex;

	float _timer;

	float _phoneAlpha;
	float _hpWidth;
	

	bool _isPhone;

public:
	ui() {}
	~ui() {}
public:
	virtual HRESULT init();
	virtual void render();
	virtual void update();
	virtual void release();

	void setHpGauge(float curHp, float maxHp);
	bool getIsPhone() { return _isPhone; }
};

