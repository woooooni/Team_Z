#pragma once

#include "Grandprix_Enemy.h"

BEGIN(Client)
class CGrandprix_Engineer final : public CGrandprix_Enemy
{
public:
	typedef struct tagEnemyDesc
	{
		_float fMaxHP = 100000.f;
		_float fCurHP = 100000.f;

	} ENEMY_STAT;

private:
	CGrandprix_Engineer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGrandprix_Engineer(const CGrandprix_Engineer& rhs);
	virtual ~CGrandprix_Engineer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	
protected:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT Ready_States() override;
	virtual HRESULT Ready_Colliders() override;

private:
	HRESULT Ready_Sockets();
	HRESULT Ready_Parts();

private:
	_float2 Transpose_ProjectionPosition();

private:
	class CModel* m_pCharacterPartModels[PART_TYPE::PART_END] = {};
	
public:
	static CGrandprix_Engineer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
