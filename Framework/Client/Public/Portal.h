#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CRenderer;
class CTransform;
END

BEGIN(Client)

class CPortal final : public CGameObject
{
public:
	typedef struct tagPortalDesc : OBJECT_INIT_DESC
	{
		LEVELID eCurrentLevel = LEVELID::LEVEL_END;
		LEVELID eNextLevel = LEVELID::LEVEL_END;

		Vec4 vNextPosition = { 0.f, 0.f, 0.f, 1.f };
		Vec3 vNextRotation = { 0.f, 0.f, 0.f };

		Vec3 vEffectScale = { 5.f, 0.3f, 1.f };

	} PORTAL_DESC;

private:
	CPortal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPortal(const CPortal& rhs);
	virtual ~CPortal() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo);
	virtual void Collision_Continue(const COLLISION_INFO& tInfo);
	virtual void Collision_Exit(const COLLISION_INFO& tInfo);

private:
	virtual HRESULT Ready_Components() override;
	HRESULT Ready_Collider();

private:
	_bool m_bActivate = false;

	LEVELID m_eCurrentLevel = LEVELID::LEVEL_END;
	LEVELID m_eNextLevel = LEVELID::LEVEL_END;

	Vec4 m_vNextPos  = { 0.f, 0.f, 0.f, 1.f };
	Vec3 m_vRotation = { 0.f, 0.f, 0.f };

	Vec3 m_vEffectScale = { 5.f, 0.3f, 1.f };

private:
	class CVfx* pEffectObject = nullptr;

	class CTransform* m_pTransformCom = nullptr;
	class CRenderer* m_pRendererCom = nullptr;

	class CUI_Minimap_Icon* m_pMinimapIcon = { nullptr };

public:
	static CPortal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

