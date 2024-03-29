#pragma once

#include "Client_Defines.h"
#include "VehicleFlying_Projectile.h"

BEGIN(Client)

class CEnemy_GuidedMissile final : public CVehicleFlying_Projectile
{
private:
	CEnemy_GuidedMissile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEnemy_GuidedMissile(const CEnemy_GuidedMissile& rhs);
	virtual ~CEnemy_GuidedMissile() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices) override;

public:
	virtual HRESULT Ready_Components() override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;

private:
	class CVehicle_Flying_Biplane* m_pTarget = nullptr;
	class CTransform* m_pTargetTransform = nullptr;

private:
	_float m_fAccRotation = 0.f;

private:
	_float m_fAccEffect = 0.f;
	_float m_fGenEffectTime = 0.5f;

public:
	static CEnemy_GuidedMissile* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

