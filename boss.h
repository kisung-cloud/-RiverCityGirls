#pragma once
#include "gameNode.h"
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")


enum STATEBOSS
{
	BOSS_LEFT_IDLE,         BOSS_RIGHT_IDLE,         // 기본 상태
	BOSS_LEFT_WALK,         BOSS_RIGHT_WALK,         // 걷는 상태
	BOSS_LEFT_DASH,		    BOSS_RIGHT_DASH,         // 대쉬 공격
	BOSS_LEFT_HIT,          BOSS_RIGHT_HIT,			 // 맞았을 때
	BOSS_LEFT_ATTACK,       BOSS_RIGHT_ATTACK,		 // 약 공격
	BOSS_LEFT_ATTACK_ELBOW, BOSS_RIGHT_ATTACK_ELBOW, // 엘보우 공격
	BOSS_LEFT_JUMP_ATTACK,  BOSS_RIGHT_JUMP_ATTACK,  // 점프 공격
	BOSS_LEFT_HEAVY_ATTACK, BOSS_RIGHT_HEAVY_ATTACK, // 강 공격
	BOSS_LEFT_DEATH,        BOSS_RIGHT_DEATH,		 // 죽었을 때
	BOSS_LEFT_ANGRY,        BOSS_RIGHT_ANGRY,        // 화남
	BOSS_END,	
};


class boss : public gameNode
{
private:		
	float      _x, _y, _z;                          // 보스 중심점
	float      _jumpPower, _gravity;                // 보스 점프 관련 변수		
	float      _angle;                              // 보스의 이동 각도
	float      _applySpeed;                         // 적용 스피드
	STATEBOSS  _state;                              // 보스 상태 저장용
	RECT       _rc;                                 // 보스 충돌 렉트
	image*     _characterImg;                       // 보스 이미지
	image*     _shadowImg;			                // 보스 그림자 이미지
	animation* _anim[BOSS_END];                     // 애니메이션 저장용
	animation* _animPlayer;                         // 애니메이션 플레이용
	int        _delayTime;                          // 공격 후 딜레이시간
	bool       _isDelayTime;                        // 딜레이 시간인지?      
	int        _patternNumber;                      // 지금 패턴 번호

	// ----------------- 스텟 관련 변수 -----------------//
	int        _hp, maxHp;
	int        _damage;
	// ----------------- 스텟 관련 변수 -----------------//

	// ================= 임시 변수 삭제하세요 ================= //
	RECT       _player;
	float      _playerX, _playerY;
	// ================= 임시 변수 삭제하세요 ================= //
	
public:
	boss() {}
	~boss() {}
public:
	HRESULT init();
	void    release();
	void    render();
	void    update();
public:
	void    loadImage();                // 보스 이미지 리소스 로드
	void    loadAnimation();            // 보스 애니메이션 로드
	void    changeState();              // 보스 상태 변경, 한번만 들어감
	void    stateUpdate();              // 보스 스테이트 업데이트.
public:
	void    attack(float playerX, float playerZ);                   // 플레이어를 찾아서 약 공격
	void    heavyAttack(float playerX, float playerZ);              // 플레이어를 찾아서 약 공격
	void    dashAttack(float playerX, float playerZ);               // 대쉬 공격
	void    jumpAttack(float playerX, float playerZ);               // 점프 공격
	void    changePattern();
};

