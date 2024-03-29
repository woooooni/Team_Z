#include "stdafx.h"
#include "GameInstance.h"
#include "Vehicle_Flying.h"

CVehicle_Flying::CVehicle_Flying(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, OBJ_TYPE eObjType)
	: CVehicle(pDevice, pContext, strObjectTag, eObjType)
{
}

CVehicle_Flying::CVehicle_Flying(const CVehicle_Flying& rhs)
	: CVehicle(rhs)
{
}

void CVehicle_Flying::Set_Routes()
{
	if (FAILED(Ready_Routes()))
		return;
}

HRESULT CVehicle_Flying::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVehicle_Flying::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

//	if (FAILED(Ready_Components()))
//		return E_FAIL;

//	if (FAILED(Ready_Colliders()))
//		return E_FAIL;

//	if (FAILED(Ready_States()))
//		return E_FAIL;

	return S_OK;
}

void CVehicle_Flying::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	if (true == m_bInfinite)
	{
		m_fAccInfinite += fTimeDelta;
		if (m_fAccInfinite >= m_fInfiniteTime)
		{
			m_fAccInfinite = 0.f;
			m_bInfinite = false;
		}
	}
}

void CVehicle_Flying::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CVehicle_Flying::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CVehicle_Flying::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	return S_OK;
}

void CVehicle_Flying::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CVehicle_Flying::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CVehicle_Flying::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

void CVehicle_Flying::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);
}

void CVehicle_Flying::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Continue(tInfo);
}

void CVehicle_Flying::Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Exit(tInfo);
}

HRESULT CVehicle_Flying::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVehicle_Flying::Ready_States()
{

	return S_OK;
}

HRESULT CVehicle_Flying::Ready_Routes()
{
	m_RoutePoints.reserve(22);

	m_RoutePoints.push_back({ 10.f, 93.f, 153.f, 1.f });
	m_RoutePoints.push_back({ 64.f, 80.f, 136.f, 1.f });
	m_RoutePoints.push_back({ 58.f, 99.f, 27.f, 1.f });
	m_RoutePoints.push_back({ 145.5f, 117.f, 95.5f, 1.f });
	m_RoutePoints.push_back({ 148.f, 119.f, 157.f, 1.f });
	m_RoutePoints.push_back({ 22.5f, 117.5f, 160.5f, 1.f });
	m_RoutePoints.push_back({ -57.5f, 102.f, 178.f, 1.f });
	m_RoutePoints.push_back({ -145.7f, 77.f, 167.f, 1.f });
	m_RoutePoints.push_back({ -231.f, 92.5f, 157.5f, 1.f });
	m_RoutePoints.push_back({ -145.7f, 77.f, 157.f, 1.f });
	m_RoutePoints.push_back({ -157.5f, 94.5f, 24.5f, 1.f });
	m_RoutePoints.push_back({ -133.f, 120.f, -155.f, 1.f });
	m_RoutePoints.push_back({ -145.7f, 77.f, 147.f, 1.f });
	m_RoutePoints.push_back({ -100.5f, 108.5f, -168.f, 1.f });
	m_RoutePoints.push_back({ 15.f, 86.5f, -151.5f, 1.f });
	m_RoutePoints.push_back({ 95.5f, 88.f, -97.f, 1.f });
	m_RoutePoints.push_back({ 101.f, 72.f, 11.f, 1.f });
	m_RoutePoints.push_back({ 30.5f, 83.f, 18.f, 1.f });
	m_RoutePoints.push_back({ -36.f, 93.f, 102.f, 1.f });
	m_RoutePoints.push_back({ 17.5f, 90.f, 37.f, 1.f });
	m_RoutePoints.push_back({ -80.f, 85.5f, 62.f, 1.f });
	m_RoutePoints.push_back({ 35.f, 102.5f, 107.8f, 1.f });

	if(m_tStat.bIsEnemy == true
		&& Get_ObjectTag() == TEXT("Vehicle_Flying_EnemyBiplane"))
	{
		// �����Ͼ� Start Route
		m_TakeOffPoints.reserve(13);
		m_TakeOffPoints.push_back({ 0.1f, 4.f, 30.2f, 1.f });
		m_TakeOffPoints.push_back({ 0.1f, 10.f, 49.f, 1.f });
		m_TakeOffPoints.push_back({ 0.1f, 14.f, 70.f, 1.f });
		m_TakeOffPoints.push_back({ 0.1f, 19.f, 105.f, 1.f });
		m_TakeOffPoints.push_back({ 0.1f, 27.f, 120.f, 1.f });
		m_TakeOffPoints.push_back({ 0.1f, 35.f, 126.f, 1.f }); 
		m_TakeOffPoints.push_back({ 0.1f, 44.f, 132.f, 1.f });
		m_TakeOffPoints.push_back({ 0.1f, 54.f, 138.f, 1.f });
		m_TakeOffPoints.push_back({ 0.1f, 65.f, 145.f, 1.f });
		m_TakeOffPoints.push_back({ 0.1f, 77.f, 153.f, 1.f });
		m_TakeOffPoints.push_back({ 0.1f, 90.f, 160.f, 1.f });
		m_TakeOffPoints.push_back({ 0.1f, 105.f, 170.f, 1.f });
		m_TakeOffPoints.push_back({ 0.1f, 120.f, 180.f, 1.f });

		m_iTakeOffIndex = 0;
		fill(begin(m_bPass), end(m_bPass), false);

		m_iRouteIndex = GI->RandomInt(0, m_RoutePoints.size() - 1);
	}
	else
	{
		m_iRouteIndex = GI->RandomInt(0, m_RoutePoints.size() - 1);
		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_RoutePoints[m_iRouteIndex]);
		// m_pControllerCom->Set_EnterLevel_Position(m_pTransformCom->Get_Position());
	}

	return S_OK;
}

HRESULT CVehicle_Flying::Ready_Colliders()
{

	return S_OK;
}

void CVehicle_Flying::Free()
{
	__super::Free();
}
