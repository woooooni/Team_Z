#pragma once

#include "Client_Defines.h"
#include "Water.h"


BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CNavigation;
class CVIBuffer_Terrain;
class CModel;
END

BEGIN(Client)

class CEvermoreWater final : public CWater
{
private:
	enum TEX_TYPE { TEX_DIFFUSE, TEX_NOISE1, TEX_NOISE2, TEX_NORMAL, TEX_END };


private:
	explicit CEvermoreWater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CEvermoreWater(const CWater& rhs);
	virtual ~CEvermoreWater() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */ 
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom[TEX_TYPE::TEX_END] = {};
	CModel* m_pModelCom = nullptr;
	CVIBuffer_Terrain* m_pVIBufferCom = { nullptr };

	

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CEvermoreWater* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override; /* 사본객체를 생성할때 원본데이터로부터 복제해올 데이터외에 더 추가해 줘야할 데이터가 있다라면 받아오겠다. */
	virtual void Free() override;
};

END

