#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	virtual HRESULT Render();


public:
	_uint Get_VertexCount() { return m_iNumVertices; }

	_uint Get_NumPrimitives() { return m_iNumPrimitives; }
	const vector<_float3>& Get_VertexLocalPositions() { return m_VertexLocalPositions; }
	const vector<FACEINDICES32>& Get_FaceIndices() { return m_FaceIndices; }

public:
	ID3D11Buffer* Get_Vertex_Buffer() { return m_pVB; }
	ID3D11Buffer* Get_Index_Buffer() { return m_pIB; }
	_uint		Get_IndicesOfPrimitive() { return m_iNumIndicesofPrimitive; }
	_uint		Get_Stride() { return m_iStride; }
	DXGI_FORMAT	Get_IndexFormat() { return m_eIndexFormat; }
	D3D11_PRIMITIVE_TOPOLOGY Get_Topology() { return m_eTopology; }

public:
	void Set_Topology(D3D11_PRIMITIVE_TOPOLOGY topology) { m_eTopology = topology; }
protected:
	ID3D11Buffer* m_pVB = nullptr;
	ID3D11Buffer* m_pIB = nullptr;

	D3D11_BUFFER_DESC				m_BufferDesc;
	D3D11_SUBRESOURCE_DATA			m_SubResourceData;

	D3D11_PRIMITIVE_TOPOLOGY		m_eTopology;

	vector<_float3>					m_VertexLocalPositions;
	vector<FACEINDICES32>			m_FaceIndices;

protected:
	_uint						m_iNumVertexBuffers = 0;
	_uint						m_iNumVertices = 0;
	_uint						m_iStride = 0;
	_uint						m_iNumPrimitives = 0;

	_uint						m_iIndexSizeofPrimitive = 0;
	_uint						m_iNumIndicesofPrimitive = 0;

	DXGI_FORMAT					m_eIndexFormat;

protected:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();
	HRESULT Create_Buffer(_Inout_ ID3D11Buffer * *ppOut);

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free();
};

END