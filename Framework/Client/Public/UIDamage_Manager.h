#pragma once

#include "Client_Defines.h"
#include "Base.h"


BEGIN(Engine)
class CTransform;
END

BEGIN(Client)
class CUIDamage_Manager : public CBase
{
	DECLARE_SINGLETON(CUIDamage_Manager)

public:
	enum UI_DAMAGETYPE { WEAKNESS, NONE, STRENGTH, DAMAGETYPE_END };

private:
	CUIDamage_Manager();
	virtual ~CUIDamage_Manager() = default;

public:
	_uint Get_CriticalStandard() { return m_iMaxStandard; }
	void Set_CriticalStandard(_uint iStandard) { m_iMaxStandard = iStandard; }

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Tick(_float fTimeDelta);

public:
	HRESULT Ready_DamageNumberPrototypes();

public:
	HRESULT Create_PlayerDamageNumber(class CTransform* pTransformCom, _int iDamage); // ColorRed
	HRESULT Create_MonsterDamageNumber(class CTransform* pTransformCom, _bool bIsBoss, UI_DAMAGETYPE eType, _int iDamage);

	_float2 Get_Offset(_float2 vTargetPosition, _bool bIsBoss, _float fCamDistance); // 오프셋만 return

private:
	_float2 Designate_RandomPosition(_float2 vTargetPosition, _bool bIsBoss, _float fCamDistance); // 좌표계산을 마친 결과
	HRESULT Create_Miss(class CTransform* pTransformCom, _bool bIsBoss);
	HRESULT Create_Critical(UI_DAMAGETYPE eType, class CTransform* pTransformCom, _float2 vOffset);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

private:
	_int m_iStandard = { 1100 };
	_int m_iMaxStandard = { 1200 };

public:
	virtual void Free() override;
};
END

/*
public:
	_int Calculate_DamageColor(ELEMENTAL_TYPE eMonsterType, ELEMENTAL_TYPE eWeaponType, _int iDamage);
*/