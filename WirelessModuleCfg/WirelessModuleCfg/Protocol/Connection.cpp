#include"stdafx.h"
#include "Connection.h"
#include "..\LogSystem.h"
#include "ProtocolConst.h"


CConnCmd::CConnCmd(CCommIntf* pCommIntf)
    : CCfgProtocol(pCommIntf)
{
}

CConnCmd::~CConnCmd()
{
}

BOOL CConnCmd::CreateSendCmd()
{
    LOG_TRACE_METHORD(TEXT("CConnCmd::CreateSendCmd func."));

    CCmdFactory CmdFactory;
    m_pSendCmd = CmdFactory.createCmd(CProtocolConst::CMD_CONN_DEVICE,
                                      (UCHAR*)CProtocolConst::STR_HUNTER_KING,
                                      strlen(CProtocolConst::STR_HUNTER_KING));
    if (NULL == m_pSendCmd)
    {
        LOG_ERROR(TEXT("failed to create the cmd of connecting device, cmd id = 0X") <<
                  std::uppercase << std::hex << CProtocolConst::CMD_CONN_DEVICE);
        return FALSE;
    }

    return TRUE;
}

BOOL CConnCmd::CreateAckCmd()
{
    DWORD dwCmdDataLen = CProtocolConst::FIELD_LEN_PRESERVED_ID +
                         CProtocolConst::FIELD_LEN_PAGE_TOTAL +
                         CProtocolConst::FIELD_LEN_PAGE_SIZE;
    CCmdFactory CmdFactory;
    m_pAckCmd = CmdFactory.createEmptyAckCmd(dwCmdDataLen);
    if (NULL == m_pAckCmd)
    {
        LOG_ERROR(TEXT("failed to create the ack cmd of connecting devcie, cmd id = 0X") <<
                  std::uppercase << std::hex << m_pSendCmd->GetCmdWord());
        return FALSE;
    }

    return TRUE;
}

BOOL CConnCmd::IsValidAckCmd()
{
    LOG_TRACE_METHORD(TEXT("CConnCmd::IsValidAckCmd func."));

    ASSERT(NULL != m_pAckCmd);

    if (CProtocolConst::ACK_CMD_CONN_DEVICE != m_pAckCmd->GetCmdWord())
    {
        LOG_ERROR(TEXT("ack cmd id is error, ack cmd id = 0X") << std::uppercase <<
                  std::hex << m_pAckCmd->GetCmdWord());
        return FALSE;
    }

    if (m_pAckCmd->CalcChecksum() != m_pAckCmd->GetChecksum())
    {
        LOG_ERROR(TEXT("ack cmd checksum of connection device is error."));
        return FALSE;
    }

    return TRUE;
}
