#pragma once

#include "StaticObject.h"

BEGIN(Client)

class CGrounds final : public CStaticObject
{
private:
	explicit CGrounds(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	explicit CGrounds(const CGrounds& rhs);
	virtual ~CGrounds() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint eType, const wstring& strFilePath, const wstring& strFileName) override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_ShadowDepth() override;

public:
	virtual HRESULT Render_Instance(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices);
	virtual HRESULT Render_Instance_Shadow(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices);

	virtual HRESULT Render_Picking();

public:
	virtual HRESULT Ready_Components() override;

public:
	static CGrounds* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag,
		const wstring& strFilePath, const wstring& strFileName, _int eObjType, _uint eModelType);
	virtual CGameObject* Clone(void* pArg);
};

END

