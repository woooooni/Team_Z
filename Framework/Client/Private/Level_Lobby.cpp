#include "stdafx.h"
#include "Level_Lobby.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Character.h"
#include "Dummy.h"
#include "Terrain.h"
#include "Camera_Manager.h"
#include "Level_Loading.h"
#include "Light.h"

#include "UI_Manager.h"
#include "Water.h"
#include <FileUtils.h>
#include <Utils.h>

#include "Particle_Manager.h"

CLevel_Lobby::CLevel_Lobby(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Lobby::Initialize()
{
	SetWindowText(g_hWnd, TEXT("Ni no Kuni : Cross Worlds"));
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(LAYER_TYPE::LAYER_BACKGROUND)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Character(LAYER_TYPE::LAYER_CHARACTER)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(LAYER_TYPE::LAYER_EFFECT)))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(LAYER_TYPE::LAYER_UI)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(LAYER_TYPE::LAYER_CAMERA)))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Dynamic(LAYER_TYPE::LAYER_DYNAMIC, TEXT("Lobby"))))
	//	return E_FAIL;

	if (FAILED(Ready_Light(TEXT("Lobby Light"))))
		return E_FAIL;

	GI->Play_BGM(TEXT("BGM_Scene_CharacterSelect_1.mp3"), GI->Get_ChannelVolume(CHANNELID::SOUND_BGM_CURR), false, BGM_START_FADEIN_DURATION);

	return S_OK;
}

HRESULT CLevel_Lobby::Tick(_float fTimeDelta)
{
	CUI_Manager::GetInstance()->Tick_UIs(LEVELID::LEVEL_LOBBY, fTimeDelta);

	return S_OK;
}

HRESULT CLevel_Lobby::LateTick(_float fTimeDelta)
{

	return S_OK;
}

HRESULT CLevel_Lobby::Render_Debug()
{

	return S_OK;
}

HRESULT CLevel_Lobby::Enter_Level()
{

	return S_OK;
}

HRESULT CLevel_Lobby::Exit_Level()
{
	GI->Clear_PhysX_Ground();
	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_BackGround(const LAYER_TYPE eLayerType)
{
	
	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_Character(const LAYER_TYPE eLayerType)
{
	switch (g_eLoadCharacter)
	{
	case Client::SWORDMAN_CH:
	{
		if (FAILED(GI->Add_GameObject(LEVEL_LOBBY, eLayerType, TEXT("Prototype_GameObject_UI_Lobby_Dummy_Swordsman"))))
			return E_FAIL;
	}
		break;
	case Client::DESTROYER_CH:
	{
		if (FAILED(GI->Add_GameObject(LEVEL_LOBBY, eLayerType, TEXT("Prototype_GameObject_UI_Lobby_Dummy_Destroyer"))))
			return E_FAIL;
	}
		break;
	case Client::ENGINEER_CH:
	{
		if (FAILED(GI->Add_GameObject(LEVEL_LOBBY, eLayerType, TEXT("Prototype_GameObject_UI_Lobby_Dummy_Engineer"))))
			return E_FAIL;
	}
		break;
	case Client::ALL_CH:
	{
		if (FAILED(GI->Add_GameObject(LEVEL_LOBBY, eLayerType, TEXT("Prototype_GameObject_UI_Lobby_Dummy_Swordsman"))))
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(LEVEL_LOBBY, eLayerType, TEXT("Prototype_GameObject_UI_Lobby_Dummy_Destroyer"))))
			return E_FAIL;
	
		if (FAILED(GI->Add_GameObject(LEVEL_LOBBY, eLayerType, TEXT("Prototype_GameObject_UI_Lobby_Dummy_Engineer"))))
			return E_FAIL;
	}
		break;
	default:
		break;
	}

	
	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_UI(const LAYER_TYPE eLayerType)
{
	if (FAILED(CUI_Manager::GetInstance()->Ready_LobbyUIs()))
		return E_FAIL;

	CUI_Manager::GetInstance()->OnOff_LobbyUIs(false);

	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_Effect(const LAYER_TYPE eLayerType)
{
	_matrix WorldMatrix = XMMatrixIdentity();
	WorldMatrix.r[3] = XMVectorSet(0.f, 30.f, 0.f, 1.f);
	if (FAILED(GET_INSTANCE(CParticle_Manager)->AddLevel_Particle(LEVEL_LOBBY, TEXT("Particle_Leaf"), WorldMatrix, _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_Dynamic(const LAYER_TYPE eLayerType, const wstring& strMapFileName)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strMapFileName + L"/" + strMapFileName + L"Dynamic.map";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);


	GI->Clear_Layer(LEVEL_LOBBY, LAYER_TYPE::LAYER_DYNAMIC);


	_uint iObjectCount = File->Read<_uint>();
	for (_uint j = 0; j < iObjectCount; ++j)
	{
		// 3. Object_Prototype_Tag
		wstring strPrototypeTag = CUtils::ToWString(File->Read<string>());
		wstring strObjectTag = CUtils::ToWString(File->Read<string>());

		// 6. Obejct States
		_float4 vRight, vUp, vLook, vPos;

		File->Read<_float4>(vRight);
		File->Read<_float4>(vUp);
		File->Read<_float4>(vLook);
		File->Read<_float4>(vPos);

		_uint objectType;
		File->Read<_uint>(objectType);


		OBJECT_INIT_DESC Init_Data = {};
		Init_Data.vStartPosition = vPos;

		CGameObject* pObj = nullptr;
		if (FAILED(GI->Add_GameObject(LEVEL_TEST, eLayerType, strPrototypeTag, &Init_Data, &pObj)))
		{
			MSG_BOX("Load_Objects_Failed.");
			return E_FAIL;
		}

		if (nullptr == pObj)
		{
			MSG_BOX("Add_Object_Failed.");
			return E_FAIL;
		}
		pObj->Set_ObjectTag(strObjectTag);

		CTransform* pTransform = pObj->Get_Component<CTransform>(L"Com_Transform");
		if (nullptr == pTransform)
		{
			MSG_BOX("Get_Transform_Failed.");
			return E_FAIL;
		}



		pTransform->Set_State(CTransform::STATE_RIGHT, XMLoadFloat4(&vRight));
		pTransform->Set_State(CTransform::STATE_UP, XMLoadFloat4(&vUp));
		pTransform->Set_State(CTransform::STATE_LOOK, XMLoadFloat4(&vLook));
		pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));

		if (pObj->Get_ObjectType() == OBJ_TYPE::OBJ_WATER)
		{
			CWater::VS_GerstnerWave vsWave;
			File->Read<CWater::VS_GerstnerWave>(vsWave);
			CWater::PS_GerstnerWave psWave;
			File->Read<CWater::PS_GerstnerWave>(psWave);
			_float damp;
			File->Read<_float>(damp);

			static_cast<CWater*>(pObj)->Set_VSGerstnerWave(vsWave);
			static_cast<CWater*>(pObj)->Set_PSGerstnerWave(psWave);
			static_cast<CWater*>(pObj)->Set_Damper(damp);
		}
	}

	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Light(const wstring& strLightFilePath)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strLightFilePath + L"/" + strLightFilePath + L".light";

	shared_ptr<CFileUtils> pFile = make_shared<CFileUtils>();
	pFile->Open(strMapFilePath, FileMode::Read);

	_uint iLightSize = 0;
	pFile->Read<_uint>(iLightSize);
	// 라이트 개수
	list<CLight*>* pLightlist = GI->Get_LightList();
	for (auto& pLight : *pLightlist)
		Safe_Release<CLight*>(pLight);

	pLightlist->clear();

	for (_uint i = 0; i < iLightSize; ++i)
	{
		LIGHTDESC LightDesc;
		::ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

		// Type
		_uint iLightType = 0;
		_uint iLightID = 0;

		pFile->Read<_uint>(iLightType);

		if (LIGHTDESC::TYPE_DIRECTIONAL == iLightType)
		{
			// ID
			pFile->Read<_uint>(iLightID);

			// State
			Vec3 vDiffuse, vAmbientLowerColor, vAmbientUpperColor, vDirection;
			pFile->Read<Vec3>(vDiffuse);
			pFile->Read<Vec3>(vAmbientLowerColor);
			pFile->Read<Vec3>(vAmbientUpperColor);
			pFile->Read<Vec3>(vDirection);

			LightDesc.eType = static_cast<LIGHTDESC::TYPE>(iLightType);
			LightDesc.vTempColor = vDiffuse;
			LightDesc.vAmbientLowerColor = vAmbientLowerColor;
			LightDesc.vAmbientUpperColor = vAmbientUpperColor;
			LightDesc.vTempDirection = vDirection;
		}
		else if (LIGHTDESC::TYPE_POINT == iLightType)
		{
			pFile->Read<_uint>(iLightID);

			// State
			Vec3 vPos, vColor;
			_float fRange;
			_bool bNonCull;
			pFile->Read<Vec3>(vPos);
			pFile->Read<_float>(fRange);
			pFile->Read<Vec3>(vColor);
			pFile->Read	<_bool>(bNonCull);

			LightDesc.eType = static_cast<LIGHTDESC::TYPE>(iLightType);
			LightDesc.vTempPosition = vPos;
			LightDesc.fTempRange = fRange;
			LightDesc.vTempColor = vColor;
			LightDesc.bNonCull = bNonCull;
		}
		else if (LIGHTDESC::TYPE_SPOT == iLightType)
		{
			pFile->Read<_uint>(iLightID);

			Vec3 vPos, vColor, vDirection;
			_float fTempRange, fOuterAngle, fInnerAngle;
			_bool bNonCull;
			pFile->Read<Vec3>(vPos);
			pFile->Read<Vec3>(vDirection);
			pFile->Read<Vec3>(vColor);
			pFile->Read<_float>(fTempRange);
			pFile->Read<_float>(fOuterAngle);
			pFile->Read<_float>(fInnerAngle);
			pFile->Read<_bool>(bNonCull);

			LightDesc.eType = static_cast<LIGHTDESC::TYPE>(iLightType);
			LightDesc.vTempPosition = vPos;
			LightDesc.vTempDirection = vDirection;
			LightDesc.vTempColor = vColor;
			LightDesc.fTempRange = fTempRange;
			LightDesc.fOuterAngle = fOuterAngle;
			LightDesc.fInnerAngle = fInnerAngle;
			LightDesc.bNonCull = bNonCull;
		}

		if (FAILED(GI->Add_Light(m_pDevice, m_pContext, LightDesc)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_Camera(const LAYER_TYPE eLayerType)
{
	if (FAILED(CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::ACTION)))
		return E_FAIL;

	return S_OK;
}

CLevel_Lobby* CLevel_Lobby::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Lobby* pInstance = new CLevel_Lobby(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Create : CLevel_Lobby");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Lobby::Free()
{
	__super::Free();
}
