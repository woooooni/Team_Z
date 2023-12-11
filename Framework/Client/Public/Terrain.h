#pragma once

#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CNavigation;
class CVIBuffer_Terrain;
END

BEGIN(Client)

class CTerrain final : public CGameObject
{
protected:
	/* ������ ������ �� */
	CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* �纻�� ������ �� */
	CTerrain(const CGameObject& rhs); /* ���� ������. */

	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


#ifdef _DEBUG
public:
	void Set_DrawGrid(_bool bDraw) { m_bDraw = bDraw; }
	_bool Is_DrawGrid() { return m_bDraw; }

protected:
	class PrimitiveBatch<VertexPositionColor>* m_pBatch = nullptr;
	class BasicEffect* m_pEffect = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;
	_float4	m_vColor = _float4(0.f, 1.f, 0.f, 1.f);
	_bool m_bDraw = true;
#endif



private: /* �ش� ��ü�� ����ؾ��� ������Ʈ���� �����ϳ�. */
	CRenderer*				m_pRendererCom = { nullptr };
	CTransform*				m_pTransformCom = { nullptr };
	CShader*				m_pShaderCom = { nullptr };
	CTexture*				m_pTextureCom = { nullptr };		
	CNavigation*			m_pNavigationCom = { nullptr };
	CVIBuffer_Terrain*		m_pVIBufferCom = { nullptr };



private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override; /* �纻��ü�� �����Ҷ� ���������ͷκ��� �����ؿ� �����Ϳܿ� �� �߰��� ����� �����Ͱ� �ִٶ�� �޾ƿ��ڴ�. */
	virtual void Free() override;
};

END