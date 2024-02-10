#pragma once

#include "MonsterProjectile.h"

BEGIN(Client)

class CWitch_VulcanBullet final : public CMonsterProjectile
{
private:
	CWitch_VulcanBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CWitch_VulcanBullet(const CWitch_VulcanBullet& rhs);
	virtual ~CWitch_VulcanBullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_ShadowDepth() override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_Colliders();

private:
	class CClown_Wizard* m_pClown_Wizard = nullptr;

	_float m_fSpeed = 0.f;

	_float m_fAccReturnTime = 0.f;
	_float m_fReturnTime = 0.f;
	_bool m_bIsReturn = false;

	_float m_fAccDelteTime = 0.f;
	_float m_fDelteTime = 0.f;

	Vec3 m_vInitLook = {};
	Matrix m_RotationMatrix = Matrix::Identity;

	_bool m_bCreate = false;

public:
	static CWitch_VulcanBullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
