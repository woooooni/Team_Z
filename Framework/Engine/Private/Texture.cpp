#include "..\Public\Texture.h"
#include <filesystem>
#include "Shader.h"
#include "Utils.h"

CTexture::CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{

}

CTexture::CTexture(const CTexture& rhs)
	: CComponent(rhs)
	, m_iNumTextures(rhs.m_iNumTextures)
	, m_SRVs(rhs.m_SRVs)
	, m_FileNames(rhs.m_FileNames)
{
	for (auto& pSRV : m_SRVs)
		Safe_AddRef(pSRV);
}

HRESULT CTexture::Initialize_Prototype(const wstring& strTextureFilePath, _uint iNumTextures, _bool bWithPath, _bool bArray)
{
	m_iNumTextures = iNumTextures;
	m_FileNames.reserve(m_iNumTextures);
	m_bArray = bArray;

	if (bWithPath)
		Load_Texture_In_Path(strTextureFilePath);
	else
		Load_Texture(strTextureFilePath, iNumTextures);

	

	return S_OK;
}

HRESULT CTexture::Initialize(void* pArg)
{
	return S_OK;
} 

HRESULT CTexture::Bind_ShaderResource(const CShader* pShader, const char* pConstantName, _uint iTextureIndex)
{
	return pShader->Bind_Texture(pConstantName, m_SRVs[iTextureIndex]);
}

HRESULT CTexture::Bind_ShaderResources(const CShader* pShader, const char* pConstantName)
{
	return pShader->Bind_Textures(pConstantName, m_SRVs.data(), m_iNumTextures);
}

HRESULT CTexture::Load_Texture(const wstring& strTextureFilePath, _uint iNumTextures)
{
	m_iNumTextures = iNumTextures;

	for (_uint i = 0; i < iNumTextures; ++i)
	{
		_tchar			szFullPath[MAX_PATH] = TEXT("");

		wsprintf(szFullPath, strTextureFilePath.c_str(), i);

		_tchar			szDrive[MAX_PATH] = TEXT("");
		_tchar			szFolderPath[MAX_PATH] = TEXT("");
		_tchar			szName[MAX_PATH] = TEXT("");
		_tchar			szExt[MAX_PATH] = TEXT("");

		_wsplitpath_s(szFullPath, szDrive, MAX_PATH, szFolderPath, MAX_PATH, szName, MAX_PATH, szExt, MAX_PATH);

		HRESULT hr = 0;

		ID3D11Resource* pTexture = nullptr;
		ID3D11ShaderResourceView* pSRV = nullptr;

		if (false == lstrcmp(TEXT(".dds"), szExt))
			hr = DirectX::CreateDDSTextureFromFile(m_pDevice, szFullPath, &pTexture, &pSRV);

		else if (false == lstrcmp(TEXT(".tga"), szExt))
			hr = E_FAIL;

		else
		{
			hr = DirectX::CreateWICTextureFromFile(m_pDevice, szFullPath, &pTexture, &pSRV);
			if (FAILED(hr))
			{
				wstring strNewPath = L"";
				strNewPath += szFolderPath;
				strNewPath += szName;
				strNewPath += L".dds";
				hr = DirectX::CreateDDSTextureFromFile(m_pDevice, strNewPath.c_str(), &pTexture, &pSRV);
			}
		}
		
		if (FAILED(hr))
		{
			MessageBox(nullptr, szFullPath, L"Failed_Load_Texture", MB_OK);
			return E_FAIL;
		}

		Safe_Release(pTexture);
		
		m_FileNames.push_back(wstring(szName) + szExt);
		m_SRVs.push_back(pSRV);
	}

	return S_OK;
}

HRESULT CTexture::Load_Texture_In_Path(const wstring& strTextureFilePath)
{
	_uint iTextureCount = 0;
	for (auto& p : std::filesystem::directory_iterator(strTextureFilePath))
	{

		if (p.is_directory())
			Load_Texture_In_Path(p.path());


		wstring strFullPath = CUtils::PathToWString(p.path().wstring());

		_tchar			szDrive[MAX_PATH] = TEXT("");
		_tchar			szFolderPath[MAX_PATH] = TEXT("");
		_tchar			szName[MAX_PATH] = TEXT("");
		_tchar			szExt[MAX_PATH] = TEXT("");

		_wsplitpath_s(strFullPath.c_str(), szDrive, MAX_PATH, szFolderPath, MAX_PATH, szName, MAX_PATH, szExt, MAX_PATH);


		HRESULT hr = 0;

		ID3D11Resource* pTexture = nullptr;
		ID3D11ShaderResourceView* pSRV = nullptr;
		
		if (false == lstrcmp(TEXT(".dds"), szExt))
			hr = DirectX::CreateDDSTextureFromFile(m_pDevice, strFullPath.c_str(), &pTexture, &pSRV);

		else if (false == lstrcmp(TEXT(".tga"), szExt))
			hr = E_FAIL;

		else
		{
			hr = DirectX::CreateWICTextureFromFile(m_pDevice, strFullPath.c_str(), &pTexture, &pSRV);
			if (FAILED(hr))
			{
				wstring strNewPath = L"";
				strNewPath += szFolderPath;
				strNewPath += szName;
				strNewPath += L".dds";
			}
		}
			

		if (FAILED(hr))
			return E_FAIL;

		Safe_Release(pTexture);

		m_FileNames.push_back(wstring(szName));
		m_SRVs.push_back(pSRV);
		++iTextureCount;
	}
	m_iNumTextures = iTextureCount;

	return S_OK;
}

CTexture* CTexture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strTextureFilePath, _uint iNumTextures, _bool bWithPath, _bool bArray)
{
	CTexture* pInstance = new CTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(strTextureFilePath, iNumTextures, bWithPath, bArray)))
	{
		MSG_BOX("Failed To Created : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTexture::Clone(void* pArg)
{
	CTexture* pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CTexture::Free()
{
	__super::Free();

	for (auto& pSRV : m_SRVs)
		Safe_Release(pSRV);

	m_SRVs.clear();

	Safe_Release(m_pTexture2DArray);
}
