#include "stdafx.h"
#include "QuestTree.h"

#include "BTNode_Select.h"

CQuestTree::CQuestTree()
{
}

HRESULT CQuestTree::Initialize()
{
	return S_OK;
}

_bool CQuestTree::Tick(const _float& fTimeDelta)
{
	return false;
}

void CQuestTree::Free()
{
	__super::Free();
}