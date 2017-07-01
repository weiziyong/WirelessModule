#include "stdafx.h"
#include "SendFirmwareCode.h"
#include "CfgProtocol.h"
#include "..\LogSystem.h"
#include "ProtocolConst.h"
#include "CmdFactory.h"
#include <memory>


CSendFirmwareCode::CSendFirmwareCode(CCommIntf* pCommIntf,
                                     const USHORT& sPageNum,
                                     const UCHAR* pHexCode,
                                     USHORT sHexCodeSize)
    : CCfgProtocol(pCommIntf),
      m_sPageNum(sPageNum),
      m_pHexCode(pHexCode),
      m_sHexCodeSize(sHexCodeSize)
{
}

CSendFirmwareCode::~CSendFirmwareCode()
{
}

BOOL CSendFirmwareCode::CreateSendCmd()
{
    LOG_TRACE_METHORD(TEXT("CSendFirmwareCode::CreateSendCmd func."));

    if (0 == m_sHexCodeSize)
    {
        LOG_ERROR(TEXT("page size of hex code to be sent is 0."));
        return FALSE;
    }

    const DWORD dwCmdDataSize = m_sHexCodeSize +
                                CProtocolConst::FIELD_LEN_PAGE_NUM;
    auto_ptr<UCHAR> pCmdData(new UCHAR[dwCmdDataSize]);
    // ¿½±´Ò³ÃæºÅ
    pCmdData.get()[0] = SHORT_HIGH_BYTE(m_sPageNum);
    pCmdData.get()[1] = SHORT_LOW_BYTE(m_sPageNum);

    // ¿½±´Ò»Ò³¹Ì¼þ´úÂë
    memcpy_s(pCmdData.get() + CProtocolConst::FIELD_LEN_PAGE_NUM,
             m_sHexCodeSize, m_pHexCode, m_sHexCodeSize);

    CCmdFactory CmdFactory;
    m_pSendCmd = CmdFactory.createCmd(
                                 CProtocolConst::CMD_SEND_FIRMWARE_CODE,
                                 pCmdData.get(),
                                 dwCmdDataSize);
    if (NULL == m_pSendCmd)
    {
        LOG_ERROR(TEXT("failed to create the cmd of sending firmware code," <<
                       " cmd id = 0X") << std::uppercase << std::hex <<
                  CProtocolConst::CMD_SEND_FIRMWARE_CODE);
        return FALSE;
    }

    return TRUE;
}

BOOL CSendFirmwareCode::CreateAckCmd()
{
    LOG_TRACE_METHORD(TEXT("CSendFirmwareCode::CreateAckCmd func."));

    DWORD dwCmdDataLen =  CProtocolConst::FIELD_LEN_PAGE_NUM +
                          CProtocolConst::FIELD_LEN_PAGE_WRITE_STATUS;
    CCmdFactory CmdFactory;
    m_pAckCmd = CmdFactory.createEmptyAckCmd(dwCmdDataLen);
    if (NULL == m_pAckCmd)
    {
        LOG_ERROR(TEXT("failed to create the ack cmd of sending firmware code£¬" <<
                       " cmd id = 0X") << std::uppercase << std::hex <<
                  m_pSendCmd->GetCmdWord());

        return FALSE;
    }

    return TRUE;
}

BOOL CSendFirmwareCode::IsValidAckCmd()
{
    LOG_TRACE_METHORD(TEXT("CConnCmd::IsValidAckCmd func."));

    if (!IsCmdIDEqual())
    {
        return FALSE;
    }

    if (!IsCheckSumEqaul())
    {
        return FALSE;
    }

    return TRUE;
}

BOOL CSendFirmwareCode::IsCmdIDEqual()
{
    ASSERT(NULL != m_pAckCmd);

    if (CProtocolConst::ACK_CMD_SEND_FIRMWARE_CODE != m_pAckCmd->GetCmdWord())
    {
        LOG_ERROR(TEXT("ack cmd id is error, ack cmd id = 0X") <<
                  std::uppercase << std::hex << m_pAckCmd->GetCmdWord());
        return FALSE;
    }

    return TRUE;
}

BOOL CSendFirmwareCode::IsCheckSumEqaul()
{
    ASSERT(NULL != m_pAckCmd);

    BYTE bCalcChecksumResult = m_pAckCmd->CalcChecksum();
    if (bCalcChecksumResult != m_pAckCmd->GetChecksum())
    {
        LOG_ERROR(TEXT("ack cmd checksum error, bCalcChecksumResult = ") <<
                  bCalcChecksumResult << TEXT(", return checksum = ") <<
                  m_pAckCmd->GetChecksum());
        return FALSE;
    }

    return TRUE;
}
