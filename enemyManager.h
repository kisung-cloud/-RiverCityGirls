#pragma once
#include "gameNode.h"
#include "enemyBoy.h"
#include "enemyGirl.h"

class player;

class enemyManager : public gameNode
{
	typedef vector<enemy*> getVEnemy;
	typedef vector<enemy*>::iterator getVIEnemy;
private:
	player* _player;

	getVEnemy _vBoy;
	getVIEnemy _viBoy;

	getVEnemy _vGirl;
	getVIEnemy _viGirl;


public:
	enemyManager() {}
	~enemyManager() {}

	virtual HRESULT init();
	virtual void render();
	virtual void update();
	virtual void release();

	void setStage1();	
	void setStage2();

	void plEnCollision();						//플레이어와 에너미 충돌 함수

	void eraseBoy(int arrNum);
	void eraseGirl(int arrNum);
	void eraseCheer(int arrNum);

	void setPlayerLink(player* player) { _player = player; }

public:
	vector<enemy*> getVBoy() { return _vBoy; }
	vector<enemy*>::iterator getVIBoy() { return _viBoy; }
};

