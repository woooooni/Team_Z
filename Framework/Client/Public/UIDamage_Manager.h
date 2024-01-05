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
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Tick(_float fTimeDelta);

public:
	HRESULT Ready_DamageNumberPrototypes();

public:
	HRESULT Create_PlayerDamageNumber(class CTransform* pTransformCom, _int iDamage); // ColorRed
	HRESULT Create_MonsterDamageNumber(class CTransform* pTransformCom, UI_DAMAGETYPE eType, _int iDamage);

private:
	_float2 Designate_RandomPosition(_float2 vTargetPosition);
	HRESULT Create_Miss(class CTransform* pTransformCom);
	HRESULT Create_Critical(UI_DAMAGETYPE eType, _float2 vPosition);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

private:
	_uint m_iStandard = { 8 };
	_uint m_iMaxStandard = { 9 };

public:
	virtual void Free() override;
};
END

/*
public:
	_int Calculate_DamageColor(ELEMENTAL_TYPE eMonsterType, ELEMENTAL_TYPE eWeaponType, _int iDamage);
*/