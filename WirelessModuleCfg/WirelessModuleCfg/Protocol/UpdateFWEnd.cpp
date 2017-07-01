#include"stdafx.h"
#include "UpdateFWEnd.h"
#include "..\LogSystem.h"
#include "ProtocolConst.h"


CUpdateFwEnd::CUpdateFwEnd(CCommIntf* pCommIntf)
    : CCfgProtocol(pCommIntf)
{
}

CUpdateFwEnd::~CUpdateFwEnd()
{
}

BOOL CUpdateFwEnd::CreateSendCmd()
{
    LOG_TRACE_METHORD(TEXT("CUpdateFwEnd::CreateSendCmd func."));

    UCHAR szCmdData[CProtocolConst::FIELD_LEN_UPDATE_FW_END_DATA] = {0};
    CCmdFactory CmdFactory;
    m_pSendCmd = CmdFactory.createCmd(
                                 CProtocolConst::CMD_UPDATE_FIRMWARE_FINISH,
                                 szCmdData,
                                 sizeof(szCmdData));
    if (NULL == m_pSendCmd)
    {
        LOG_ERROR(TEXT("failed to send the cmd of updating firmware end, cmd id = 0X") <<
                  std::uppercase << std::hex << CProtocolConst::CMD_UPDATE_FIRMWARE_FINISH);
        return FALSE;
    }

    return TRUE;
}

BOOL CUpdateFwEnd::CreateAckCmd()
{
    DWORD dwCmdDataLen = CProtocolConst::FIELD_LEN_UPDATE_FW_END_ACK_CMD_DATA;
    CCmdFactory CmdFactory;
    m_pAckCmd = CmdFactory.createEmptyAckCmd(dwCmdDataLen);
    if (NULL == m_pAckCmd)
    {
        LOG_ERROR(TEXT("failed to create the ack cmd of updating firmware end," <<
                       " cmd id = 0X") << std::uppercase << std::hex <<
                  m_pSendCmd->GetCmdWord());
        return FALSE;
    }

    return TRUE;
}

BOOL CUpdateFwEnd::IsValidAckCmd()
{
    LOG_TRACE_METHORD(TEXT("CUpdateFwEnd::IsValidAckCmd func."));

    ASSERT(NULL != m_pAckCmd);

    if (CProtocolConst::ACK_CMD_UPDATE_FIRMWARE_FINISH != m_pAckCmd->GetCmdWord())
    {
        LOG_ERROR(TEXT("ack cmd id is error, ack cmd id = 0X") << std::uppercase <<
                  std::hex << m_pAckCmd->GetCmdWord());
        return FALSE;
    }

    if (m_pAckCmd->CalcChecksum() != m_pAckCmd->GetChecksum())
    {
        LOG_ERROR(TEXT("the checksum of ack cmd error, the ack of upgrade firmware end cmd"));
        return FALSE;
    }

    return TRUE;
}
