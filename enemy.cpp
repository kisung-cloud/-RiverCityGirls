#include "stdafx.h"
#include "enemy.h"

enemy::enemy()
{
}

enemy::~enemy()
{
}

HRESULT enemy::init(float x, float y, ENEMYTYPE et, string mapName)
{
	{
		_idle = new enemyIdleState();
		_move = new enemyMoveState();
		_run = new enemyRunState();
		_jump = new enemyJumpState();
		_attack = new enemyAttackState();
		_hit = new enemyHitState();
		_down = new enemyDownState();
		_dizzy = new enemyDizzyState();
		_dead = new enemyDeadState();
	}

	addImage();

	_et = et;
	_x = x;
	_y = y;
	_z = y + 100;
	_width = 130;
	_height = 200;

	_rc = RectMakeCenter(_x, _y, _width, _height);
	_shadow = RectMakeCenter(_x, _z, 50, 30);

	switch (_et)
	{
	case ENEMYTYPE::BOY:
		_image = IMAGEMANAGER->findImage("boy_idle");
		break;
	case ENEMYTYPE::GIRL:
		_image = IMAGEMANAGER->findImage("girl_idle");
		break;
	case ENEMYTYPE::CHEER:
		_image = IMAGEMANAGER->findImage("cheer_idle");
		break;
	}

	_shadowImg = IMAGEMANAGER->findImage("enemy_shadow");

	_mapName = mapName;
	_pixel = PIXEL::TEMP;
	_green = GREEN::TEMP;
	_table = TABLE::TEMP;

	_right = _isHit = _isJumping = false;
	_jumpPower = _gravity = 0;
	_state = _idle;
	_maxHP = 100;

	return S_OK;
}

void enemy::release()
{
}

void enemy::update()
{
	_rc = RectMakeCenter(_x, _y, _width, _height);
	_shadow = RectMakeCenter(_x, _y + 100, 80, 30);

	_state->update(*this, _playerRC, _playerX, _playerY, _et);

	if (_isLay) _layCount++;

	pixelCollision();

	//CAMERAMANAGER->setX(_x);
	//CAMERAMANAGER->setY(_y);

	draw();
}

void enemy::render()
{
	if (KEYMANAGER->isToggleKey('R'))
	{
		CAMERAMANAGER->renderRectangle(getMemDC(), _attackRC);									//공격렉트
		CAMERAMANAGER->renderRectangle(getMemDC(), _rc);										//에너미 렉트
		CAMERAMANAGER->render(getMemDC(), _shadowImg, _x, _z);									//그림자 렉트
	}
	
	//CAMERAMANAGER->renderRectangle(getMemDC(), _shadow);										//그림자 렌더
	//CAMERAMANAGER->frameRender(getMemDC(), _image, _x , _y , _currentX, _currentY);			//에너미 프레임렌더

	ZORDERMANAGER->addAlphaRender(getMemDC(), renderType::ALPHA_RENDER, _shadowImg, _x, _z, _z - 1, 200);
	ZORDERMANAGER->addFrameRender(getMemDC(), renderType::FRAME_RENDER, _image, _x, _y, _z, _currentX, _currentY);
}

void enemy::directionCheck(RECT rc, float x, float y)
{
	_playerRC = rc;
	_playerX = x;
	_playerY = y;

	//방향 체크
	if (x > _x && _currentX >= _image->getMaxFrameX())
	{
		_right = true;
	}

	if (x < _x && _currentX <= 0)
	{
		_right = false;
	}

	//거리 체크
	if (getDistance(x, y, _x, _z) < 200 && _maxHP > 0) _condition = CONDITION::CLOSE;
	if (getDistance(x, y, _x, _z) > 200 && _maxHP > 0) _condition = CONDITION::SEARCH;
}

void enemy::draw()
{
	_frameCount++;

	if ((_image == IMAGEMANAGER->findImage("boy_knockdown")) || (_image == IMAGEMANAGER->findImage("girl_knockdown")) || (_image == IMAGEMANAGER->findImage("cheer_knockdown")) ||
		(_image == IMAGEMANAGER->findImage("girl_attack3")) || (_image == IMAGEMANAGER->findImage("cheer_flip")) || (_image == IMAGEMANAGER->findImage("cheer_attack3")))
	{
		_count = 5;
	}
	else _count = 7;

	if (!_isStop)
	{
		if (_frameCount % _count == 0)
		{
			if (_right)
			{
				_currentY = 0;
				_currentX++;
				if (_currentX >= _image->getMaxFrameX()) _currentX = 0;
				_image->setFrameX(_currentX);
			}

			if (!_right)
			{
				_currentY = 1;
				_currentX--;
				if (_currentX <= 0) _currentX = _image->getMaxFrameX();
				_image->setFrameX(_currentX);
			}
			_frameCount = 0;
		}
	}
}

void enemy::pixelCollision()
{
	int _probeU = _shadow.top;
	int _probeB = _shadow.bottom;
	int _probeL = _shadow.left;
	int _probeR = _shadow.right;

	//위
	for (int i = _probeU - 5; i < _probeU + 5; ++i)
	{
		COLORREF color = GetPixel(IMAGEMANAGER->findImage(_mapName)->getMemDC(), _x, i);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);


		if (r == 255 && g == 0 && b == 0)
		{
			_pixel = PIXEL::TOP;
		}

		if (r == 160 && g == 255 && b == 0)
		{
			_green = GREEN::TOP;
			_pixel = PIXEL::TOP;
		}

		if (r == 255 && g == 0 && b == 255)
		{

		}

	}

	//아래
	for (int i = _probeB - 5; i < _probeB + 5; ++i)
	{
		COLORREF color = GetPixel(IMAGEMANAGER->findImage(_mapName)->getMemDC(), _x, i);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (r == 255 && g == 0 && b == 0)
		{
			_pixel = PIXEL::BOTTOM;
		}

		if (r == 160 && g == 255 && b == 0)
		{
			_green = GREEN::BOTTOM;
			_pixel = PIXEL::BOTTOM;
		}

		if (r == 255 && g == 0 && b == 255)
		{
			_green = GREEN::TEMP;
			_pixel = PIXEL::TEMP;
		}
		break;
	}

	//왼쪽
	for (int i = _probeL - 5; i < _probeL + 5; ++i)
	{
		COLORREF color = GetPixel(IMAGEMANAGER->findImage(_mapName)->getMemDC(), i, _z);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (r == 255 && g == 0 && b == 0)
		{
			_pixel = PIXEL::LEFT;
		}

		if (r == 160 && g == 255 && b == 0)
		{
			_green = GREEN::LEFT;
			_pixel = PIXEL::LEFT;
		}

		if (r == 255 && g == 0 && b == 255)
		{

		}
	}

	//오른쪽
	for (int i = _probeR - 5; i < _probeR + 5; ++i)
	{
		COLORREF color = GetPixel(IMAGEMANAGER->findImage(_mapName)->getMemDC(), i, _z);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (r == 255 && g == 0 && b == 0)
		{
			_pixel = PIXEL::RIGHT;
		}

		if (r == 160 && g == 255 && b == 0)
		{
			_green = GREEN::RIGHT;
			_pixel = PIXEL::RIGHT;
		}

		if (r == 255 && g == 0 && b == 255)
		{

		}
	}

	//if (_isJumping)
	//{
	//	for (int i = _probeB - 5; i < _probeB + 5; ++i)
	//	{
	//		COLORREF color = GetPixel(IMAGEMANAGER->findImage(_mapName)->getMemDC(), _x, i);

	//		int r = GetRValue(color);
	//		int g = GetGValue(color);
	//		int b = GetBValue(color);

	//		if (_jumpPower < 0)
	//		{
	//			if ((r == 160 && g == 255 && b == 0))
	//			{
	//				_table = TABLE::BOTTOM;
	//				_isOnDesk = true;
	//				_z -= 100;
	//			}
	//		}
	//		
	//		break;
	//	}
	//}

	//if (_table == TABLE::BOTTOM)
	//{
	//	_jumpCount++;
	//	if (_jumpCount > 100)
	//	{
	//		for (int i = _probeB - 5; i < _probeB + 5; ++i)
	//		{
	//			COLORREF color = GetPixel(IMAGEMANAGER->findImage(_mapName)->getMemDC(), _x, i);

	//			int r = GetRValue(color);
	//			int g = GetGValue(color);
	//			int b = GetBValue(color);

	//			if (!(r == 255 && g == 255 && b == 0))
	//			{
	//				_table = TABLE::TEMP;
	//				_isJumping = false;
	//				_isOnDesk = false;
	//				_z += 100;
	//			}
	//			break;
	//		}
	//	}
	//	_jumpCount = 0;
	//}	
}

void enemy::addImage()
{


	/*
	_idle
	_walk
	_run
	_attack1
	_attack2
	_attack3
	_sidekick
	_hit1
	_hit2
	_hit3
	_groundhit
	_knockdown
	_dizzy
	*/
}
