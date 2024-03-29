#include "stdafx.h"
#include "Level_Tool.h"
#include "GameInstance.h"
#include "Camera.h"
#include "ImGui_Manager.h"
#include "Character.h"
#include "Dummy.h"
#include "Terrain.h"
#include "Camera_Manager.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Weapon_SwordTemp.h"
#include "LensFlare.h"
#include "CurlingGame_Manager.h"
#include "Trigger.h"

CLevel_Tool::CLevel_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Tool::Initialize()
{
	SetWindowText(g_hWnd, TEXT("Scene_Tool"));
	if (FAILED(__super::Initialize()))
		return E_FAIL;

 	m_pImGuiManager = CImGui_Manager::GetInstance();
	Safe_AddRef(m_pImGuiManager);

	if (FAILED(Ready_Layer_Camera(LAYER_TYPE::LAYER_CAMERA)))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(LAYER_TYPE::LAYER_BACKGROUND)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(LAYER_TYPE::LAYER_TERRAIN)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(LAYER_TYPE::LAYER_PLAYER)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Character(LAYER_TYPE::LAYER_CHARACTER)))
		return E_FAIL; 

	if (FAILED(Ready_Layer_Monster(LAYER_TYPE::LAYER_MONSTER)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(LAYER_TYPE::LAYER_EFFECT)))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(LAYER_TYPE::LAYER_UI)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Weapon(LAYER_TYPE::LAYER_WEAPON)))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Skydome(LAYER_TYPE::LAYER_SKYBOX)))
	//	return E_FAIL;

	//if (FAILED(GI->Add_ShadowLight(LEVEL_TOOL, XMVectorSet(0.f, 10.f, 0.f, 1.f), XMVectorSet(10.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 1.f, 0.f, 0.f))))
	//	return E_FAIL;

	if (FAILED(CCurlingGame_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
		return E_FAIL;

	//CTrigger::TRIGGER_DESC TriggerDesc;
	//TriggerDesc.eTriggerType = TRIGGER_TYPE::TRIGGER_WITCH_ESCORT1;
	//TriggerDesc.vStartPosition = { 49.282f, -5.259f, -3.901f, 1.f };
	//TriggerDesc.vExtents = { 30.f, 30.f, 30.f };

	//if (FAILED(GI->Add_GameObject(LEVEL_TOOL, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
	//	return E_FAIL;

	//TriggerDesc.eTriggerType = TRIGGER_TYPE::TRIGGER_WITCH_ESCORT2;
	//TriggerDesc.vStartPosition = { 7.889f, -6.283f, -22.237f, 1.f };
	//TriggerDesc.vExtents = { 30.f, 30.f, 0.f };

	//if (FAILED(GI->Add_GameObject(LEVEL_TOOL, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
	//	return E_FAIL;
		// Triggers.
	//CTrigger::TRIGGER_DESC TriggerDesc;
	//TriggerDesc.eTriggerType = TRIGGER_TYPE::TRIGGER_MAP_NAME;
	//TriggerDesc.strMapName = TEXT("���� ����");
	//TriggerDesc.vStartPosition = { 0.f, -20.f, 0.f, 1.f };
	//TriggerDesc.vExtents = { 30.f, 20.f, 30.f };
	//TriggerDesc.vAt = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	//TriggerDesc.vEye = Vec4(7.0f, 100.0f, 0.0f, 1.0f);

	//if (FAILED(GI->Add_GameObject(LEVEL_TOOL, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
	//	return E_FAIL;

	//TriggerDesc.strMapName = TEXT("���� ����");
	//TriggerDesc.vStartPosition = { -65.841f, -20.f, 13.031f, 1.f };
	//TriggerDesc.vExtents = { 32.5f, 15.f, 45.f };
	//TriggerDesc.vAt = Vec4(0.0f, -574.0f, 0.0f, 1.0f);
	//TriggerDesc.vEye = Vec4(-60.622f, 100.0f, -39.127f, 1.0f);
	//if (FAILED(GI->Add_GameObject(LEVEL_TOOL, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
	//	return E_FAIL;


	//TriggerDesc.vStartPosition = { -85.5f, -20.f, 60.6f, 1.f };
	//TriggerDesc.vExtents = { 50.f, 50.f, 150.f };
	//TriggerDesc.vAt = Vec4(0.0f, -574.0f, 0.0f, 1.0f);
	//TriggerDesc.vEye = Vec4(-85.0f, 100.0f, 0.0f, 1.0f);
	//if (FAILED(GI->Add_GameObject(LEVEL_TOOL, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
	//	return E_FAIL;



	//TriggerDesc.strMapName = TEXT("���� ����");
	//TriggerDesc.vStartPosition = { 88.85f, -20.f, 60.6f, 1.f };
	//TriggerDesc.vExtents = { 50.f, 50.f, 150.f };
	//if (FAILED(GI->Add_GameObject(LEVEL_TOOL, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
	//	return E_FAIL;



	//CRespawn_Box::RESPAWN_DESC RespawnDesc = {};
	//RespawnDesc.vStartPosition = Vec4(0.f, -100.f, 0.f, 1.f);
	//RespawnDesc.vRespawnPosition = Vec4(0.f, 0.f, 0.f, 1.f);
	//RespawnDesc.vExtents = Vec3(1000.f, 5.f, 1000.f);

	//if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_RespawnBox"), &RespawnDesc)))
	//	return E_FAIL;


	return S_OK;
}

HRESULT CLevel_Tool::Tick(_float fTimeDelta)
{
	m_pImGuiManager->Tick(fTimeDelta);

	if (KEY_TAP(KEY::F8))
	{
		if (FAILED(GI->Add_GameObject(LEVEL_TOOL, _uint(LAYER_MONSTER), TEXT("Prorotype_GameObject_Glanix"))))
			return E_FAIL;

		//if (FAILED(GI->Add_GameObject(LEVEL_TOOL, _uint(LAYER_MONSTER), TEXT("Prorotype_GameObject_Stellia"))))
		//	return E_FAIL;
	}

	CCurlingGame_Manager::GetInstance()->Tick(fTimeDelta);

	return S_OK;
}

HRESULT CLevel_Tool::LateTick(_float fTimeDelta)
{


	CCurlingGame_Manager::GetInstance()->LateTick(fTimeDelta);

	return S_OK;
}

HRESULT CLevel_Tool::Render_Debug()
{
	m_pImGuiManager->Render_ImGui();
	return S_OK;
}

HRESULT CLevel_Tool::Enter_Level()
{
	CImGui_Manager::GetInstance()->Reserve_Manager(g_hWnd, m_pDevice, m_pContext);

	return S_OK;
}

HRESULT CLevel_Tool::Exit_Level()
{
	return S_OK;
}

HRESULT CLevel_Tool::Ready_Lights()
{
	


	//GI->Reset_Lights();

	//LIGHTDESC			LightDesc;

	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eType = LIGHTDESC::TYPE_POINT;
	//LightDesc.vPosition = _float4(15.0f, 5.0f, 15.0f, 1.f);
	//LightDesc.fRange = 10.f;
	//LightDesc.vDiffuse = _float4(1.f, 0.0f, 0.f, 1.f);
	//LightDesc.vAmbient = _float4(0.5f, 0.5f, 0.5f, 1.f);
	//LightDesc.vSpecular = LightDesc.vDiffuse;

	//if (FAILED(GI->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;

	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eType = LIGHTDESC::TYPE_POINT;
	//LightDesc.vPosition = _float4(25.0f, 5.0f, 15.0f, 1.f);
	//LightDesc.fRange = 10.f;
	//LightDesc.vDiffuse = _float4(0.0f, 1.f, 0.f, 1.f);
	//LightDesc.vAmbient = _float4(0.5f, 0.5f, 0.5f, 1.f);
	//LightDesc.vSpecular = LightDesc.vDiffuse;

	//if (FAILED(GI->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;

	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	//LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	//LightDesc.vDiffuse = _float4(0.5, 0.5, 0.5, 1.f);
	//LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	//LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	//if (FAILED(GI->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;



	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_Camera(const LAYER_TYPE eLayerType)
{
	if (FAILED(CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FREE)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_Player(const LAYER_TYPE eLayerType)
{
	if (FAILED(GI->Add_GameObject(LEVEL_TOOL, _uint(eLayerType), TEXT("Prototype_GameObject_Dummy"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_Character(const LAYER_TYPE eLayerType)
{
	/*if (FAILED(GI->Add_GameObject(LEVEL_TOOL, _uint(eLayerType), TEXT("Prototype_GameObject_Zenitsu"))))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_BackGround(const LAYER_TYPE eLayerType)
{

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_Terrain(const LAYER_TYPE eLayerType)
{
	if (FAILED(GI->Add_GameObject(LEVEL_TOOL, LAYER_TYPE::LAYER_TERRAIN, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_Monster(const LAYER_TYPE eLayerType)
{
	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_UI(const LAYER_TYPE eLayerType)
{
	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_Effect(const LAYER_TYPE eLayerType)
{
	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_Weapon(const LAYER_TYPE eLayerType)
{
	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_Skydome(const LAYER_TYPE eLayerType)
{
	if (FAILED(GI->Add_GameObject(LEVEL_TOOL, LAYER_TYPE::LAYER_SKYBOX, TEXT("Prototype_GameObject_Skydome"))))
		return E_FAIL;
	//if (FAILED(GI->Add_GameObject(LEVEL_TOOL, LAYER_TYPE::LAYER_SKYBOX, TEXT("Prototype_GameObject_SkyPlane"))))
	//	return E_FAIL;

	return S_OK;
}

CLevel_Tool* CLevel_Tool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Tool* pInstance = new CLevel_Tool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Tool::Free()
{
	__super::Free();
	Safe_Release(m_pImGuiManager);
}
