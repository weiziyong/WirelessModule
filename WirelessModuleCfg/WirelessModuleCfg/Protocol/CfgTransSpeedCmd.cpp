//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : CCfgTransSpeedCmd.cpp
//  @ Date : 2014/11/5 ������
//  @ Author :
//
//

#include "stdafx.h"
#include "CfgTransSpeedCmd.h"
#include "../LogSystem.h"

CCfgTransSpeedCmd::CCfgTransSpeedCmd(CCommIntf* const pCommIntf, const DWORD dwTransSpeed)
    : CCfgProtocol(pCommIntf)
{
    m_dwTransSpeed = dwTransSpeed;
}

CCfgTransSpeedCmd::~CCfgTransSpeedCmd()
{

}

BOOL CCfgTransSpeedCmd::CreateSendCmd()
{
    LOG_TRACE_METHORD(TEXT("SpeedCmd::CreateSendCmd func."));

    UCHAR cIdxTransSpeed = GetTransSpeedIdx();
    if (CProtocolConst::INVALID_TRANS_SPEED_IDX == cIdxTransSpeed)
    {
        LOG_ERROR("Get the trans's speed index , failed !");
        return FALSE;
    }

    CCmdFactory CmdFactory;
    m_pSendCmd = CmdFactory.createCmd(CProtocolConst::CMD_SET_BAND,
                                      &cIdxTransSpeed, 1);
    if (NULL == m_pSendCmd)
    {
        LOG_ERROR("Create the configural cmd of trans's speed, failed !");
        return FALSE;
    }

    return TRUE;
}

BOOL CCfgTransSpeedCmd::CreateAckCmd()
{
    CCmdFactory CmdFactory;
    m_pAckCmd = CmdFactory.createEmptyAckCmd(m_pSendCmd->GetCmdDataBufferLen());
    if (NULL == m_pAckCmd)
    {
        LOG_ERROR("Create the ack's cmd of cfg trans's speed, failed ! " <<
                  "the pointer of returing is NULL !");
        return FALSE;
    }

    return TRUE;
}

BYTE CCfgTransSpeedCmd::GetTransSpeedIdx()
{
    for (DWORD dwIdx = 0;
            dwIdx < sizeof(CProtocolConst::m_aTransSpeed) / sizeof(DWORD);
            ++dwIdx)
    {
        if (CProtocolConst::m_aTransSpeed[dwIdx] == m_dwTransSpeed)
        {
            return (BYTE)(dwIdx + 1);
        }
    }

    LOG_ERROR("Not find the matching's index value, the same as trans's speed : " <<
              m_dwTransSpeed << " kbps");

    return CProtocolConst::INVALID_TRANS_SPEED_IDX;
}
