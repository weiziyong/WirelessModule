//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : CCmdFactory.cpp
//  @ Date : 2014/11/5 ������
//  @ Author :
//
//

#include "stdafx.h"
#include "CmdFactory.h"
#include "../LogSystem.h"

CCmdFactory::CCmdFactory()
{

}

CCmdFactory::~CCmdFactory()
{

}

CCmd* CCmdFactory::createCmd(const BYTE btCmdWord,
                             UCHAR* pCmdData,
                             const DWORD dwCmdDataLen) const
{
    LOG_TRACE_METHORD(TEXT("CCmdFactory::createCmd func."));

    CCmd* pCmd = new CCmd();
    if (NULL == pCmd)
    {
        LOG_FATAL(TEXT("failed to allocate the object of cmd."));
        return NULL;
    }

    if (!pCmd->SetCmdDataLen(dwCmdDataLen))
    {
        LOG_ERROR(TEXT("failed to set data length."));
        return NULL;
    }

    pCmd->SetFrameHead(CProtocolConst::DEFAULT_FRAME_HEAD);
    pCmd->SetCmdPreserveWord(CProtocolConst::DEFAULT_PRESERVE_WORD);
    pCmd->SetCmdWord(btCmdWord);
    if (!pCmd->SetCmdData(pCmdData, dwCmdDataLen))
    {
        LOG_ERROR(TEXT("failed to set the data of cmd."));
        return NULL;
    }

    pCmd->SetCmdChecksum(pCmd->CalcChecksum());

    return pCmd;
}

CCmd* CCmdFactory::createEmptyAckCmd(const DWORD dwCmdDataLen) const
{
    LOG_TRACE_METHORD(TEXT("CCmdFactory::createEmptyAckCmd func."));

    CCmd* pCmd = new CCmd();
    if (NULL == pCmd)
    {
        LOG_FATAL(TEXT("Allocate the object of cmd, pcmd == NULL, failed!"));
        return NULL;
    }

    if (!pCmd->SetCmdDataLen(dwCmdDataLen))
    {
        LOG_ERROR(TEXT("Set the ack cmd's data length, failed !"));
        return NULL;
    }

    memset(pCmd->GetCmdBuffer(), 0, pCmd->GetCmdLen());

    return pCmd;
}


