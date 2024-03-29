#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CUIMinigame_Manager : public CBase
{
	DECLARE_SINGLETON(CUIMinigame_Manager)

public:
	enum GRANDPRIX_ENEMY { ENGINEER, ENEMY1, ENEMY2, ENEMY3, ENEMY4, ENEMY5, ENEMY6, ENEMY7, ENEMY8, ENEMY_END };

private:
	CUIMinigame_Manager();
	virtual ~CUIMinigame_Manager() = default;

public: // Getter & Setter
	void	Set_HPOwner(CGameObject* pOwner, _uint eEnemyID);
	void	Set_TargetUI(class CVehicle_Flying* pTarget);

	_bool	Is_GrandprixIntroStarted() { return m_bIntroStarted; }
	_bool	Is_BiplaneFlying() { return m_bFlying; }
	void	Set_Flyable(_bool bFlyable);

	_bool	Is_AimActive() { return m_bActiveAim; }
	void	Set_GrandprixAimActive(_bool bOnOff) { m_bActiveAim = bOnOff; }

	_bool	Is_RaderError() { return m_bError; }

	class CUI_Minigame_Curling_Base* Get_MiniGame_Curling_Ui(const _uint& iObjTag);

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Tick(_float fTimeDelta);

public: // Prototypes
	HRESULT Ready_MinigameUI_Prototypes(LEVELID eID);

public: // Clone
	HRESULT Ready_MinigameUI_GameObject(LEVELID eID);
	HRESULT Ready_MinigameUI_ToLayer(LEVELID eID);

public:
	void Tick_Minigame(LEVELID eID, _float fTimeDelta);
	void LateTick_Minigame(LEVELID eID, _float fTimeDelta);

public: // Level Evermore
	void OnOff_TowerDefence_Select(_bool bOnOff);
	void OnOff_Grandprix(_bool bOnOff); // Grandprix Default Setting

	void Intro_Grandprix();
	void Start_Grandprix(); // Countdown
	void OnOff_GrandprixGauge(_bool bOnOff);
	void End_Grandprix(); // End Text

	void Use_GrandprixSkill(SKILL_TYPE eType);

	void OnOff_RushVignette(_bool bOnOff);
	void On_DamagedVignette();
	void OnOff_RaderIcons(_bool bOnOff);
	void On_GrandprixPopup(_uint iIndex);

public: // Level IceLand
	void OnOff_CurlingUI(_bool bOnOff);

private: // Prototypes
	HRESULT Ready_MinigameUI_Evermore();
	HRESULT Ready_MinigameUI_IceLand();

private: // clone
	HRESULT Ready_TowerDence();
	HRESULT Ready_Granprix();
	HRESULT Ready_Curling();

private:
	void Tick_Grandprix(_float fTimeDelta);
	void LateTick_Grandprix(_float fTimeDelta);

private:
	void Tick_Curling(_float fTimeDelta);

private: // Evermore Tower Defence
	class CUI_Minigame_Basic*				m_pMenu = { nullptr };
	vector<class CUI_Minigame_TowerSelect*> m_TowerSelect;
	class CUI_Minigame_Timer*				m_pTimer = { nullptr };
	class CUI_Minigame_Basic*				m_pStartBtn = { nullptr };
	class CUI_Minigame_Basic*				m_pGold = { nullptr };

private: // Evermore Granprix
	class CUI_Minigame_PlayerInfo*			m_pPlayerStat = { nullptr };
	class CUI_Minigame_Basic*				m_pCloud = { nullptr };
	vector<class CUI_Minigame_EnemyInfo*>	m_EnemyHP;
	vector<class CUI_Minigame_ClassSkill*>	m_Skill;
	_bool									m_bCountStart = { false }; // 게임이 시작되면 CountDown UI On
	_bool									m_bGrandprixEnd = { false }; // 게임이 끝나면 End UI On
	_uint									m_iCountIndex = { 0 };
	vector<class CUI_Minigame_Basic*>		m_Counts;

	vector<class CUI_Minigame_Basic*>		m_GaugeBack;
	class CUI_Minigame_GaugeBar*			m_pGaugeBar = { nullptr };
	class CUI_Minigame_Basic*				m_pSpace = { nullptr };
	class CUI_Minigame_Basic*				m_pBiplaneIcon = { nullptr };

	_bool									m_bFlying = { false };
	_bool									m_bActiveAim = { false };
	_bool									m_bError = { false };

	vector<class CUI_Grandprix_Vignette*>	m_Vignette;

	// Intro Background
	_bool			m_bIntroStarted = { false };
	class CUI_Grandprix_IntroBackground*	m_pIntroBackground;
	vector<class CUI_Grandprix_IntroIcons*> m_IntroIcons;
	_bool									m_bIntroFinished = { false };
	_float									m_fIntroAcc = { 0.f };

	// Rader
	vector<class CUI_Grandprix_RaderFrame*> m_RaderFrame;
	class CUI_Grandprix_Rader*				m_pRader;

	vector<class CUI_Grandprix_Popup*>		m_Popup;
	class CUI_Grandprix_Target*				m_pTarget;

private: // Iceland Curling
	vector<class CUI_Minigame_Curling_Base*> m_CurlingGameUIs;

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

public:
	virtual void Free() override;
};

END