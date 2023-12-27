#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)
class CUI_Manager : public CBase
{
	DECLARE_SINGLETON(CUI_Manager)

private:
	CUI_Manager();
	virtual ~CUI_Manager() = default;
	
public: // Get/Set
	class CUI_Fade* Get_Fade();
	_bool Get_MainMenuActive();

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Tick(_float fTimeDelta);
	void	LateTick(_float fTImeDelta);

public: // For Prototypes
	HRESULT Ready_UIPrototypes(LEVELID eID);

public: // For Clone Objects
	HRESULT Ready_Cursor();
	HRESULT Ready_Veils();
	HRESULT Ready_Loadings();
	HRESULT Ready_LobbyUIs();
	HRESULT Ready_CommonUIs(LEVELID eID); // 항상 쓰이는 UI들 (Cursor랑 Veil 옮길것)

public:
	HRESULT Tick_UIs(LEVELID eID, _float fTimeDelta);

	HRESULT Tick_LobbyLevel(_float fTimeDelta);
	HRESULT Tick_EvermoreLevel(_float fTimeDelta);

public:
	void Tick_Fade(_float fTimeDelta);
	void LateTick_Fade(_float fTimeDelta);
	void Render_Fade();
	_bool Is_FadeFinished();

public:
	void Update_LobbyBtnState(_uint iIndex);
	void Update_CostumeBtnState(_uint iIndex);
	void Update_InvenBtnState(_uint iIndex);
	void Update_SkillBtnState(class CTransform* pTransform, _uint iIndex);
	void Update_SkillSlotState(_uint iSectionType, _uint iSlotIndex);
	void Update_ClothSlotState(_uint iSectionType, _uint iSlotIndex);

public:
	HRESULT Using_CloseButton();
	HRESULT Using_BackButton();

	HRESULT OnOff_Veil(_bool bOnOff);
	HRESULT OnOff_SettingWindow(_bool bOnOff);

	HRESULT OnOff_GamePlaySetting(_bool bOnOff); // 기본적으로 세팅되어있는 게임화면 UI들
	HRESULT OnOff_MainMenu(_bool bOnOff);
	HRESULT OnOff_QuestWindow(_bool bOnOff);
	HRESULT OnOff_QuestPopup(_bool bOnOff);

	HRESULT OnOff_MonsterHP(_bool bOnOff, ELEMENTAL_TYPE eType = ELEMENTAL_TYPE::ELEMENTAL_END);

	HRESULT OnOff_CloseButton(_bool bOnOff);

	HRESULT OnOff_SubMenu(_bool bOnOff, _uint iMagicNum);
	HRESULT Off_OtherSubBtn(_uint iMagicNum);
	HRESULT OnOff_DialogWindow(_bool bOnOff, _uint iMagicNum);

	HRESULT OnOff_WorldMap(_bool bOnOff);
	HRESULT OnOff_CostumeWindow(_bool bOnOff); // 코스튬 탭 Window 비/활성화
	HRESULT OnOff_CostumeSlot(_uint iSection, _bool bOnOff);
	HRESULT OnOff_SkillWindowSlot(_uint iMenuType, _bool bOnOff);
	HRESULT OnOff_Announce(_int iMagicNum, _bool bOnOff);
	HRESULT OnOff_Inventory(_bool bOnOff); // 가방 탭 WIndow 비/활성화
	HRESULT OnOff_SkillWindow(_bool bOnOff);

	HRESULT OnOff_EmoticonWindow(_bool bOnOff);
	HRESULT OnOff_EmoticonBalloon(_bool bOnOff);
	void Set_EmoticonType(_uint iIndex);

public: // For UI Tool
	HRESULT Save_UIData();
	void Load_UIData();

private:
	class CUI_Default_Background* m_pDefaultBG = { nullptr };
	class CUI_Cursor* m_pUICursor = { nullptr };
	class CUI_Veil* m_pUIVeil = { nullptr };
	class CUI_Fade* m_pUIFade = { nullptr };
	class CUI_MapName* m_pUIMapName = { nullptr };
	class CUI_Basic* m_pMapText = { nullptr };

	// For Setting Window
	class CUI_Setting_Window* m_pSettingBG = { nullptr };

	class CUI_PlayerInfo* m_pPlayerStatus = { nullptr };

	class CUI_BtnShowMenu* m_pBtnShowMenu = { nullptr };
	class CUI_BtnChangeCamera* m_pBtnCamera = { nullptr };
	class CUI_BtnInventory* m_pBtnInven = { nullptr };
	class CUI_BtnQuickQuest* m_pBtnQuest = { nullptr };
	class CUI_BtnClose* m_pBtnClose = { nullptr };
	class CUI_BtnShowSetting* m_pBtnShowSetting = { nullptr };

	class CUI_MainMenu* m_pMainBG = { nullptr }; // MainMenu Background (버튼과 따로 관리된다)
	class CUI_WindowQuest* m_pWindowQuest = { nullptr };

	vector<class CUI_PopupQuest*> m_QuestPopUp;

	class CUI_Text_TabMenu* m_pTabMenuTitle = { nullptr };

	// PlayerStatus
	vector<class CUI_PlayerEXPBar*> m_PlayerEXP;
	vector<class CUI_SkillSection_ClassicSkill*> m_ClassicSkill;
	vector<class CUI_SkillSection_Frame*> m_ClassicFrame;
	vector<class CUI_SkillSection_SpecialSkill*> m_SpecialSkill;
	vector<class CUI_SkillSection_Frame*> m_SpecialFrame;
	vector<class CUI_QuickSlot_Item*> m_ItemQuickslot;

	// MonsterHP
	class CUI_MonsterHP_Background* m_pMonsterHPBack = { nullptr };
	class CUI_MonsterHP_Bar* m_pMonsterHPBar = { nullptr };
	class CUI_MonsterHP_ElementalFrame* m_pMonsterFrame = { nullptr };
	class CUI_MonsterHP_Elemental* m_pMonsterElemental = { nullptr };

	class CUI_Announced* m_pCameraAnnounce = { nullptr };

	class CUI_Emoticon_Window* m_pEmoticonWindow = { nullptr };
	class CUI_Emoticon_SpeechBalloon* m_pBalloon = { nullptr };

private:
	vector<class CUI_Basic*> m_Basic;
	vector<class CUI_BasicButton*> m_Buttons;

	vector<class CUI_LevelUp*> m_LevelUp;

	vector<class CUI_BtnCharacterSelect*> m_UnclickedPlayer;
	vector<class CUI_BtnCharacterSelect*> m_ClickedPlayer;

	// Main + Sub Menu Btns
	vector<class CUI_MainMenu*> m_MainMenuBtn;
	vector<class CUI_MenuSeparator*> m_MenuSeparator;
	vector<class CUI_SubMenu_Character*> m_SubMenuChar;
	vector<class CUI_SubMenu_Equipment*> m_SubMenuEquip;
	vector<class CUI_SubMenu_Imajinn*> m_SubMenuPet;
	vector<class CUI_SubMenu_Shop*> m_SubMenuShop;

	// For World Map
	class CUI_WindowWorldMap* m_pWorldMapBG = { nullptr };
	// For Skill Section
	class CUI_SkillSection_Background* m_pSkillBG = { nullptr };
	// For Imajinn Section
	class CUI_ImajinnSection_Background* m_pImajinnBG = { nullptr };
	// For Dialog
	class CUI_Dialog_Window* m_pDialogWindow = { nullptr };
	class CUI_Dialog_MiniWindow* m_pDialogMini = { nullptr };
	vector<class CUI_Dialog_Portrait*> m_Portrait;
	// For Costume
	vector<class CUI_Costume_Btn*> m_CostumeBtn;
	vector<class CUI_Costume_Btn*> m_CostumeClickedBtn;
	class CUI_Costume_LineBox* m_pCostumeBox = { nullptr };
	vector<class CUI_Costume_ItemSlot*> m_CostumeCloth;
	vector<class CUI_Costume_ItemSlot*> m_CostumeHairAcc;
	// For Inven
	class CUI_Inventory_LineBox* m_pInvenBox = { nullptr };
	vector<class CUI_Inventory_TabBtn*> m_InvenBtn;
	vector<class CUI_Inventory_TabBtn*> m_InvenClickedBtn;
	vector<class CUI_Inventory_Slot*> m_InvenSlots;
	// For Skill
	vector<class CUI_SkillWindow_LineBox*> m_SkillWindow;
	vector<class CUI_SkillWindow_Btn*> m_SkillBtn;
	vector<class CUI_SkillWindow_Btn*> m_SkillClickedBtn;
	vector<class CUI_SkillWindow_SkillSlot*> m_ClassSkillSlot;
	vector<class CUI_SkillWindow_SkillSlot*> m_SpecialSkillSlot;
	class CUI_SkillWindow_LineBox* m_pSkillDesc = { nullptr };

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	HRESULT Ready_UIStaticPrototypes();
	HRESULT Ready_UILobbyPrototypes();

public:
	virtual void Free() override;
};

END
