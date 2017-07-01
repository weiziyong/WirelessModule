#include "stdafx.h"
#include "ReadFirmwareCode.h"
#include "CfgProtocol.h"
#include "..\LogSystem.h"
#include "ProtocolConst.h"
#include "CmdFactory.h"
#include <memory>


CReadFirmwareCode::CReadFirmwareCode(CCommIntf* pCommIntf,
                                     const USHORT& sPageNum,
                                     const USHORT& sPageSize)
    : CCfgProtocol(pCommIntf),
      m_sPageNum(sPageNum),
      m_sPageSize(sPageSize)
{
}

CReadFirmwareCode::~CReadFirmwareCode()
{
}

BOOL CReadFirmwareCode::CreateSendCmd()
{
    LOG_TRACE_METHORD(TEXT("CReadFirmwareCode::CreateSendCmd func."));

    // 页面号发送时是高字节在前，低字节在后，交换高低字节顺序
    UCHAR bCmdData[sizeof(m_sPageNum)] = {0};
    bCmdData[0] = SHORT_HIGH_BYTE(m_sPageNum);
    bCmdData[1] = SHORT_LOW_BYTE(m_sPageNum);

    CCmdFactory CmdFactory;
    m_pSendCmd = CmdFactory.createCmd(
                                 CProtocolConst::CMD_READ_FIRMWARE_CODE,
                                 bCmdData,
                                 CProtocolConst::FIELD_LEN_PAGE_NUM);
    if (NULL == m_pSendCmd)
    {
        return FALSE;
    }

    return TRUE;
}

BOOL CReadFirmwareCode::CreateAckCmd()
{
    LOG_TRACE_METHORD(TEXT("CReadFirmwareCode::CreateAckCmd func."));

    DWORD dwCmdDataLen =  CProtocolConst::FIELD_LEN_PAGE_NUM +
                          m_sPageSize;
    CCmdFactory CmdFactory;
    m_pAckCmd = CmdFactory.createEmptyAckCmd(dwCmdDataLen);
    if (NULL == m_pAckCmd)
    {
        LOG_ERROR(TEXT("failed to create the ack cmd of reading firmware page code，" <<
                       " cmd id = 0X") << std::uppercase << std::hex <<
                  m_pSendCmd->GetCmdWord());

        return FALSE;
    }

    return TRUE;
}

BOOL CReadFirmwareCode::IsValidAckCmd()
{
    LOG_TRACE_METHORD(TEXT("CConnCmd::IsValidAckCmd func."));

    ASSERT(NULL != m_pAckCmd);

    if (CProtocolConst::ACK_CMD_READ_FIRMWARE_CODE != m_pAckCmd->GetCmdWord())
    {
        LOG_ERROR(TEXT("ack cmd id is error, ack cmd id = 0X") <<
                  std::uppercase << std::hex << m_pAckCmd->GetCmdWord());
        return FALSE;
    }

    if (m_pAckCmd->CalcChecksum() != m_pAckCmd->GetChecksum())
    {
        LOG_ERROR(TEXT("ack cmd checksum error."));
        return FALSE;
    }

    return TRUE;
}
