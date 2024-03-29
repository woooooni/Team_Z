#include "..\Public\Mesh.h"
#include "Model.h"
#include "HierarchyNode.h"
#include "GameInstance.h"
#include "Utils.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CMesh::CMesh(const CMesh& rhs)
	: CVIBuffer(rhs)
	, m_iMaterialIndex(rhs.m_iMaterialIndex)
	, m_strName(rhs.m_strName)
	, m_AnimVertices(rhs.m_AnimVertices)
	, m_NonAnimVertices(rhs.m_NonAnimVertices)
	, m_bFromBinary(rhs.m_bFromBinary)
	, m_BoneNames(rhs.m_BoneNames)
	, m_iNumBones(rhs.m_iNumBones)
{
	
}

HRESULT CMesh::Initialize_Prototype(CModel::TYPE eModelType, const aiMesh* pAIMesh, CModel* pModel, _fmatrix PivotMatrix)
{
	/* 이 메시와 이름이 같은 뼈대가 존재한다면.
	이 뼈대의 행렬을 메시를 구성하는 정점에 곱해질 수 있도록 유도하낟. */

	m_strName = CUtils::ToWString(pAIMesh->mName.C_Str());

	/* 메시마다 사용하는 머테리얼(텍스쳐정보로 표현)이 다른다. */
	/* 메시를 그릴때 마다 어떤 머테리얼을 솅디ㅓ로 던져야할 지르르 결정해주기위해서. */
	m_iMaterialIndex = pAIMesh->mMaterialIndex;

#pragma region VERTEXBUFFER

	HRESULT		hr = 0;

	if (CModel::TYPE_NONANIM == eModelType)
		hr = Ready_Vertices(pAIMesh, PivotMatrix);
	else
		hr = Ready_AnimVertices(pAIMesh, pModel);

	if (FAILED(hr))
		return E_FAIL;

#pragma endregion

#pragma region INDEXBUFFER
	m_iNumPrimitives = pAIMesh->mNumFaces;
	m_iIndexSizeofPrimitive = sizeof(FACEINDICES32);
	m_iNumIndicesofPrimitive = 3;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumPrimitives * m_iIndexSizeofPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;


	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitives];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitives);

	m_FaceIndices.reserve(m_iNumPrimitives);
	for (_uint i = 0; i < m_iNumPrimitives; ++i)
	{
		pIndices[i]._0 = pAIMesh->mFaces[i].mIndices[0];
		pIndices[i]._1 = pAIMesh->mFaces[i].mIndices[1];
		pIndices[i]._2 = pAIMesh->mFaces[i].mIndices[2];
		m_FaceIndices.push_back(pIndices[i]);
	}


	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;



	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion
	return S_OK;
}

HRESULT CMesh::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CMesh::Initialize_Bin(CModel* pModel, const vector<wstring>& BoneNames)
{
	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		CHierarchyNode* pHierarchyNode = pModel->Get_HierarchyNode(BoneNames[i]);
		m_Bones.push_back(pHierarchyNode);
		m_BoneNames.push_back(pHierarchyNode->Get_Name());
		Safe_AddRef(pHierarchyNode);
	}

	if (0 == m_iNumBones)
	{
		CHierarchyNode* pNode = pModel->Get_HierarchyNode(m_strName);
		if (nullptr == pNode)
			return S_OK;

		m_iNumBones = 1;
		m_BoneNames.push_back(pNode->Get_Name());
		m_Bones.push_back(pNode);
		Safe_AddRef(pNode);
	}

	return S_OK;
}


HRESULT CMesh::SetUp_HierarchyNodes(CModel* pModel, aiMesh* pAIMesh)
{
	m_iNumBones = pAIMesh->mNumBones;

	/* 현재 메시에 영향ㅇ르 ㅈ2ㅜ는 뼈들을 순회한다ㅏ. */
	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		aiBone* pAIBone = pAIMesh->mBones[i];

		CHierarchyNode* pHierarchyNode = pModel->Get_HierarchyNode(CUtils::ToWString(pAIBone->mName.C_Str()));

		_float4x4			OffsetMatrix;

		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));

		pHierarchyNode->Set_OffsetMatrix(XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));


		m_Bones.push_back(pHierarchyNode);
		m_BoneNames.push_back(pHierarchyNode->Get_Name());

		Safe_AddRef(pHierarchyNode);
	}

	if (0 == m_iNumBones)
	{

		CHierarchyNode* pNode = pModel->Get_HierarchyNode(m_strName);

		if (nullptr == pNode)
			return S_OK;

		m_iNumBones = 1;
		m_BoneNames.push_back(pNode->Get_Name());
		m_Bones.push_back(pNode);

		Safe_AddRef(pNode);

	}

	return S_OK;
}

HRESULT CMesh::SetUp_HierarchyNodes(CModel* pModel)
{
	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		CHierarchyNode* pHierarchyNode = pModel->Get_HierarchyNode(m_BoneNames[i]);
		m_Bones.push_back(pHierarchyNode);
		Safe_AddRef(pHierarchyNode);
	}

	if (0 == m_iNumBones)
	{

		CHierarchyNode* pNode = pModel->Get_HierarchyNode(m_strName);

		if (nullptr == pNode)
			return S_OK;

		m_iNumBones = 1;
		m_Bones.push_back(pNode);
		Safe_AddRef(pNode);
	}

	return S_OK;
}


/* 메시의 정점을 그리기위해 셰이더에 넘기기위한 뼈행렬의 배열을 구성한다. */
void CMesh::SetUp_BoneMatrices(ID3D11Texture2D* pTexture, vector<_float4x4>& Matrices, _fmatrix PivotMatrix)
{
	D3D11_MAPPED_SUBRESOURCE SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	if (0 == m_iNumBones)
	{
		_float4x4 pMatirx;
		XMStoreFloat4x4(&pMatirx, XMMatrixIdentity());

		m_pContext->Map(pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);
		memcpy(SubResource.pData, &pMatirx, sizeof(_float4x4));
		m_pContext->Unmap(pTexture, 0);

		return;
	}

	for (_uint i = 0; i < m_iNumBones; ++i)
		XMStoreFloat4x4(&Matrices[i], XMMatrixTranspose(m_Bones[i]->Get_OffSetMatrix() * m_Bones[i]->Get_CombinedTransformation() * PivotMatrix));

	m_pContext->Map(pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);
	memcpy(SubResource.pData, Matrices.data(), sizeof(_float4x4) * Matrices.size());
	m_pContext->Unmap(pTexture, 0);
}

HRESULT CMesh::Ready_Vertices(const aiMesh* pAIMesh, _fmatrix PivotMatrix)
{
	m_iNumVertexBuffers = 1;
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iStride = sizeof(VTXMODEL);

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXMODEL* pVertices = new VTXMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		/* 정점의 위치를 내가 원하느 ㄴ초기상태로 변화낳ㄴ나./ */
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));

		/* 정점의 위치가 바뀌었기때ㅑ문에 노멀도 바뀐다. */
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PivotMatrix));

		memcpy(&pVertices[i].vTexture, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));

		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vTangent, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vTangent), PivotMatrix));
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;


	m_VertexLocalPositions.reserve(m_iNumVertices);
	m_NonAnimVertices.reserve(m_iNumVertices);
	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		m_NonAnimVertices.push_back(pVertices[i]);
		m_VertexLocalPositions.push_back(m_NonAnimVertices[i].vPosition);
	}
		

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}



HRESULT CMesh::Ready_AnimVertices(const aiMesh* pAIMesh, CModel* pModel)
{
	m_iNumVertexBuffers = 1;
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iStride = sizeof(VTXANIMMODEL);

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXANIMMODEL* pVertices = new VTXANIMMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{	
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexture, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}

	/* Static과 달리 해당 메시에 영향을 주는 뼈의 정보를 저장한다. */

	for (_uint i = 0; i < pAIMesh->mNumBones; ++i)
	{
		aiBone* pAIBone = pAIMesh->mBones[i];

		for (_uint j = 0; j < pAIBone->mNumWeights; ++j)
		{
			_uint		iVertexIndex = pAIBone->mWeights[j].mVertexId;

			if (0.0f == pVertices[iVertexIndex].vBlendWeight.x)
			{
				_int iIndex = pModel->Get_HierarchyNodeIndex(pAIBone->mName.C_Str());

				if (iIndex < 0)
					MSG_BOX("Failed : CMesh::Ready_AnimVertices()");

				pVertices[iVertexIndex].vBlendIndex.x = iIndex;

				pVertices[iVertexIndex].vBlendWeight.x = pAIBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[iVertexIndex].vBlendWeight.y)
			{
				_int iIndex = pModel->Get_HierarchyNodeIndex(pAIBone->mName.C_Str());

				if (iIndex < 0)
					MSG_BOX("Failed : CMesh::Ready_AnimVertices()");

				pVertices[iVertexIndex].vBlendIndex.y = iIndex;

				pVertices[iVertexIndex].vBlendWeight.y = pAIBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[iVertexIndex].vBlendWeight.z)
			{
				_int iIndex = pModel->Get_HierarchyNodeIndex(pAIBone->mName.C_Str());

				if (iIndex < 0)
					MSG_BOX("Failed : CMesh::Ready_AnimVertices()");

				pVertices[iVertexIndex].vBlendIndex.z = iIndex;

				pVertices[iVertexIndex].vBlendWeight.z = pAIBone->mWeights[j].mWeight;
			}

			else if (0.0f == pVertices[iVertexIndex].vBlendWeight.w)
			{
				_int iIndex = pModel->Get_HierarchyNodeIndex(pAIBone->mName.C_Str());

				if (iIndex < 0)
					MSG_BOX("Failed : CMesh::Ready_AnimVertices()");

				pVertices[iVertexIndex].vBlendIndex.w = iIndex;

				pVertices[iVertexIndex].vBlendWeight.w = pAIBone->mWeights[j].mWeight;
			}
		}
	}

	m_VertexLocalPositions.reserve(m_iNumVertices);
	m_AnimVertices.reserve(m_iNumVertices);
	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		m_AnimVertices.push_back(pVertices[i]);
		m_VertexLocalPositions.push_back(pVertices[i].vPosition);
	}
		
	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_Bin_Vertices()
{
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;


	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = m_NonAnimVertices.data();

	m_VertexLocalPositions.reserve(m_iNumVertices);
	for (_uint i = 0; i < m_iNumVertices; ++i)
		m_VertexLocalPositions.push_back(m_NonAnimVertices[i].vPosition);

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;


	m_iIndexSizeofPrimitive = sizeof(FACEINDICES32);
	m_iNumIndicesofPrimitive = 3;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumPrimitives * m_iIndexSizeofPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;



	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = m_FaceIndices.data();

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;


	return S_OK;
}

HRESULT CMesh::Ready_Bin_AnimVertices()
{
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = m_AnimVertices.data();

	m_VertexLocalPositions.reserve(m_iNumVertices);
	for (_uint i = 0; i < m_iNumVertices; ++i)
		m_VertexLocalPositions.push_back(m_AnimVertices[i].vPosition);

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	m_iIndexSizeofPrimitive = sizeof(FACEINDICES32);
	m_iNumIndicesofPrimitive = 3;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumPrimitives * m_iIndexSizeofPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = m_FaceIndices.data();

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	return S_OK;
}




CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, const aiMesh* pAIMesh, CModel* pModel, _fmatrix PivotMatrix)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pAIMesh, pModel, PivotMatrix)))
	{
		MSG_BOX("Failed To Created : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMesh* CMesh::Create_Bin(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, _fmatrix PivotMatrix)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);
	pInstance->m_bFromBinary = true;
	return pInstance;
}

CComponent* CMesh::Clone(void* pArg)
{
	CMesh* pInstance = new CMesh(*this);

	if (!m_bFromBinary)
	{
		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed To Cloned : CMesh");
			Safe_Release(pInstance);
		}
	}

	return pInstance;
}

void CMesh::Free()
{
	__super::Free();

	for (auto& pHierarchyNode : m_Bones)
		Safe_Release(pHierarchyNode);

	m_Bones.clear();

}