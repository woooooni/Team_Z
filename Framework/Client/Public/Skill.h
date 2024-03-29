#pragma once

#include "Client_Defines.h"
#include "Character.h"
#include "Base.h"

BEGIN(Client)

class CSkill abstract : public CBase
{	
protected:
	CSkill();
	virtual ~CSkill() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CCharacter* pOwner);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

public:
	virtual _bool Use_Skill();

public:
	_bool  Is_Useable() { return m_bUseable; }
	_float Get_CurrCoolTime() { return m_fAccCoolTime; }
	_float Get_CoolTime() { return m_fCoolTime; }

protected:
	class CCharacter* m_pCharacter = nullptr;
	class CStateMachine* m_pCharacterStateMachine = nullptr;
	ID3D11Device* m_pDevice = nullptr; 
	ID3D11DeviceContext* m_pContext = nullptr;

protected:
	_bool m_bUseable = true;
	_float m_fAccCoolTime = 10.f;
	_float m_fCoolTime = 10.f;

public:
	virtual void Free() override;
};

END

