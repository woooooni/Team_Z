#include "stdafx.h"
#include "Level_WitchForest.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Character.h"
#include "Dummy.h"
#include "Terrain.h"
#include "Camera_Manager.h"
#include "Level_Loading.h"
#include "Game_Manager.h"
#include "Player.h"

#include "UI_Manager.h"
#include "UI_Fade.h"

#include "Light_Manager.h"
#include "Light.h"
#include <Utils.h>
#include <FileUtils.h>
#include "Trigger.h"
#include "GameNpc.h"
#include "Animals.h"
#include "Water.h"

CLevel_WitchForest::CLevel_WitchForest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_WitchForest::Initialize()
{
	SetWindowText(g_hWnd, TEXT("Ni no Kuni : Cross Worlds"));
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(LAYER_TYPE::LAYER_CAMERA)))
		return E_FAIL;
//
	if (FAILED(Ready_Layer_BackGround(LAYER_TYPE::LAYER_BACKGROUND)))
		return E_FAIL;
//
//	if (FAILED(Ready_Layer_Terrain(LAYER_TYPE::LAYER_TERRAIN)))
//		return E_FAIL;
//	
	if (FAILED(Ready_Layer_Character(LAYER_TYPE::LAYER_CHARACTER)))
		return E_FAIL;

//	if (FAILED(Ready_Layer_Monster(LAYER_TYPE::LAYER_MONSTER)))
//		return E_FAIL;

	if (FAILED(Ready_Layer_UI(LAYER_TYPE::LAYER_UI)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Dynamic(LAYER_TYPE::LAYER_DYNAMIC, L"Witch")))
		return E_FAIL;

	if (FAILED(Ready_Light(TEXT("Witch Light"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Npc(LAYER_TYPE::LAYER_NPC)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Prop(LAYER_TYPE::LAYER_PROP)))
		return E_FAIL;
	

	if (nullptr != CUI_Manager::GetInstance()->Get_Fade())
		CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 3.f);

	GI->Play_BGM(TEXT("BGM_Field_Village_Winter_Po_1.mp3"), GI->Get_ChannelVolume(CHANNELID::SOUND_BGM_CURR), false, BGM_START_FADEIN_DURATION);

	return S_OK;
}

HRESULT CLevel_WitchForest::Tick(_float fTimeDelta)
{
	CUI_Manager::GetInstance()->Tick_UIs(LEVELID::LEVEL_WITCHFOREST, fTimeDelta);

	if (KEY_TAP(KEY::PAGE_UP))
	{
		CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FOLLOW);
	}

	if (KEY_TAP(KEY::PAGE_DOWN))
	{
		CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FREE);
	}

	if (KEY_TAP(KEY::F8))
	{
		GI->Add_GameObject(LEVEL_WITCHFOREST, _uint(LAYER_MONSTER), TEXT("Prorotype_GameObject_Glanix"));
	}

	return S_OK;
}

HRESULT CLevel_WitchForest::LateTick(_float fTimeDelta)
{
	CUI_Manager::GetInstance()->LateTick_GamePlayLevel(fTimeDelta);

	return S_OK;
}

HRESULT CLevel_WitchForest::Render_Debug()
{

	return S_OK;
}

HRESULT CLevel_WitchForest::Enter_Level()
{

	return S_OK;
}

HRESULT CLevel_WitchForest::Exit_Level()
{
	return S_OK;
}

HRESULT CLevel_WitchForest::Ready_Layer_Camera(const LAYER_TYPE eLayerType)
{
	if (FAILED(CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FOLLOW)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_WitchForest::Ready_Layer_BackGround(const LAYER_TYPE eLayerType)
{

	list<CGameObject*>& Grounds = GI->Find_GameObjects(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_GROUND);
	for (auto& Ground : Grounds)
	{
		if (FAILED(GI->Add_Ground(Ground,
			Ground->Get_Component<CModel>(L"Com_Model"),
			Ground->Get_Component<CTransform>(L"Com_Transform")->Get_WorldMatrix())))
		{
			MSG_BOX("피직스 그라운드 생성에 실패했습니다.");
		}
	}


	list<CGameObject*>& Buildings = GI->Find_GameObjects(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_BUILDING);
	for (auto& Building : Buildings)
	{
		if (FAILED(GI->Add_Building(Building,
			Building->Get_Component<CModel>(L"Com_Model"),
			Building->Get_Component<CTransform>(L"Com_Transform")->Get_WorldMatrix())))
		{
			MSG_BOX("피직스 빌딩 생성에 실패했습니다.");
		}
	}



	list<CGameObject*>& Props = GI->Find_GameObjects(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_PROP);
	for (auto& Prop : Props)
	{
		CModel* pModel = Prop->Get_Component<CModel>(L"Com_Model");
		if (nullptr == pModel)
			continue;

		if (pModel->Get_Name().find(L"Evermore_Prob_03") != wstring::npos)
		{
			if (FAILED(GI->Add_Building(Prop,
				Prop->Get_Component<CModel>(L"Com_Model"),
				Prop->Get_Component<CTransform>(L"Com_Transform")->Get_WorldMatrix())))
			{
				MSG_BOX("피직스 계단 생성에 실패했습니다.");
			}
		}
	}
	
	return S_OK;
}

HRESULT CLevel_WitchForest::Ready_Layer_Terrain(const LAYER_TYPE eLayerType)
{
	if (FAILED(GI->Add_GameObject(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_TERRAIN, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_WitchForest::Ready_Layer_Character(const LAYER_TYPE eLayerType)
{
	if (!CCamera_Manager::GetInstance()->Is_Empty_Camera(CAMERA_TYPE::FOLLOW))
	{
		CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Set_TargetObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
		CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Set_LookAtObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
	}

	return S_OK;
}

HRESULT CLevel_WitchForest::Ready_Layer_Monster(const LAYER_TYPE eLayerType)
{

	return S_OK;
}

HRESULT CLevel_WitchForest::Ready_Layer_UI(const LAYER_TYPE eLayerType)
{
	if (FAILED(CUI_Manager::GetInstance()->Ready_GameObjectToLayer(LEVELID::LEVEL_WITCHFOREST)))
		return E_FAIL;

	CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
	if (pPlayer == nullptr)
		return E_FAIL;
	CCharacter* pCharacter = pPlayer->Get_Character();
	if (pCharacter == nullptr)
		return E_FAIL;

	CHARACTER_TYPE eCharacterType = pCharacter->Get_CharacterType();
	ELEMENTAL_TYPE eElementalType = pCharacter->Get_ElementalType();

	CUI_Manager::GetInstance()->Ready_CharacterTypeForUI(eCharacterType);
	CUI_Manager::GetInstance()->Ready_ElementalTypeForUI(eElementalType);

	return S_OK;
}

HRESULT CLevel_WitchForest::Ready_Layer_Dynamic(const LAYER_TYPE eLayerType, const wstring& strMapFileName)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strMapFileName + L"/" + strMapFileName + L"Dynamic.map";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);


	GI->Clear_Layer(LEVELID::LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_DYNAMIC);


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
		if (FAILED(GI->Add_GameObject(LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_DYNAMIC, strPrototypeTag, &Init_Data, &pObj)))
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
		else if (pObj->Get_ObjectType() == OBJ_TYPE::OBJ_ANIMAL)
		{
			_uint iSize;
			File->Read<_uint>(iSize);

			if (iSize != 0)
			{
				CAnimals* pAnimals = static_cast<CAnimals*>(pObj);
				vector<Vec4> Points;
				Points.reserve(iSize);

				for (_uint i = 0; i < iSize; ++i)
				{
					Vec4 vPoint;
					File->Read<Vec4>(vPoint);
					Points.push_back(vPoint);
				}

				pAnimals->Set_RomingPoints(Points);

				_float* pSpeed = pAnimals->Get_Speed();
				File->Read<_float>(*pSpeed); // 0

				vPos = Points.front();
			}
		}

		pTransform->Set_State(CTransform::STATE_RIGHT, XMLoadFloat4(&vRight));
		pTransform->Set_State(CTransform::STATE_UP, XMLoadFloat4(&vUp));
		pTransform->Set_State(CTransform::STATE_LOOK, XMLoadFloat4(&vLook));
		pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));

		CPhysX_Controller* pController = pObj->Get_Component<CPhysX_Controller>(L"Com_Controller");

		if (nullptr != pController)
			pController->Set_EnterLevel_Position(pTransform->Get_Position());
	}

	return S_OK;
}

HRESULT CLevel_WitchForest::Ready_Layer_Npc(const LAYER_TYPE eLayerType)
{
	wstring strNpcFileName = L"Witch";
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strNpcFileName + L"/" + strNpcFileName + L"NPC.map";
	 
	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);

	for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
	{
		if (LAYER_TYPE::LAYER_NPC != i)
			continue;

		GI->Clear_Layer(LEVELID::LEVEL_WITCHFOREST, i);


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


			OBJECT_INIT_DESC Init_Data = {};
			Init_Data.vStartPosition = vPos;
			CGameObject* pObj = nullptr;
			if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_WITCHFOREST, i, strPrototypeTag, &Init_Data, &pObj)))
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

			_uint ObjectType;
			File->Read<_uint>(ObjectType);

			if (OBJ_TYPE::OBJ_NPC == ObjectType)
			{
				CGameNpc* pNpc = dynamic_cast<CGameNpc*>(pObj);

				if (pNpc == nullptr)
				{
					MSG_BOX("Fail Load : NPC");
					return E_FAIL;
				}

				_uint iSize;
				File->Read<_uint>(iSize);

				_uint eState;
				File->Read<_uint>(eState);


				if (iSize != 0)
				{
					vector<Vec4> Points;
					Points.reserve(iSize);

					for (_uint i = 0; i < iSize; ++i)
					{
						Vec4 vPoint;
						File->Read<Vec4>(vPoint);
						Points.push_back(vPoint);
					}

					pNpc->Set_RoamingArea(Points);

					if (Points.size() != 0)
					{
						vPos = Points.front();
						pNpc->Set_Point(true);
					}
				}

				CGameNpc::NPC_STAT eStat;
				File->Read<CGameNpc::NPC_STAT>(eStat);

				pNpc->Set_NpcState(static_cast<CGameNpc::NPC_STATE>(eState));
				CStateMachine* pStateMachine = pNpc->Get_Component<CStateMachine>(TEXT("Com_StateMachine"));
				if (pStateMachine != nullptr)
				{
					pStateMachine->Change_State(eState);
				}
				else
				{
					MSG_BOX("Fail Get : NPC_StateMachine");
					return E_FAIL;
				}
				pNpc->Set_Stat(eStat);
			}


			pTransform->Set_State(CTransform::STATE_RIGHT, XMLoadFloat4(&vRight));
			pTransform->Set_State(CTransform::STATE_UP, XMLoadFloat4(&vUp));
			pTransform->Set_State(CTransform::STATE_LOOK, XMLoadFloat4(&vLook));
			pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));
		}

	}

	return S_OK;
}

HRESULT CLevel_WitchForest::Ready_Light(const wstring& strLightFilePath)
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

HRESULT CLevel_WitchForest::Ready_Layer_Prop(const LAYER_TYPE eLayerType)
{
	CTrigger::TRIGGER_DESC TriggerDesc;
	TriggerDesc.eTriggerType = TRIGGER_TYPE::TRIGGER_WITCH_WOOD_ENTER;
	TriggerDesc.strMapName = L"";
	TriggerDesc.vStartPosition = { 101.275f, -4.906f, 28.147f, 1.f };
	TriggerDesc.vExtents = { 5.f, 5.f, 5.f };

	if (FAILED(GI->Add_GameObject(LEVEL_WITCHFOREST, eLayerType, TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
		return E_FAIL;

	return S_OK;
}

CLevel_WitchForest* CLevel_WitchForest::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_WitchForest* pInstance = new CLevel_WitchForest(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Create : CLevel_WitchForest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_WitchForest::Free()
{
	__super::Free();
}
