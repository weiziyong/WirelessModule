//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : CCfgInfo.cpp
//  @ Date : 2014/10/29 ������
//  @ Author :
//
//

#include "stdafx.h"
#include "CfgInfo.h"



CCfgInfo::CCfgInfo()
{
    m_strComPortName = TEXT("");
}

CCfgInfo::~CCfgInfo()
{

}

CString CCfgInfo::GetComPortName() const
{
    return m_strComPortName;
}

CString CCfgInfo::GetCurBaudRate() const
{
    CString strCurBaudRate = TEXT("");
    strCurBaudRate.Format(TEXT("%d"), m_dwCurBaudRate);
    return strCurBaudRate;
}

CString CCfgInfo::GetMinBaudRate() const
{
    CString strMinBaudRate = TEXT("");
    strMinBaudRate.Format(TEXT("%d"), m_dwMinBaudRate);
    return strMinBaudRate;
}

CString CCfgInfo::GetMaxBaudRate() const
{
    CString strMaxBaudRate = TEXT("");
    strMaxBaudRate.Format(TEXT("%d"), m_dwMaxBaudRate);
    return strMaxBaudRate;
}

CString CCfgInfo::GetCurCommChannelNum() const
{
    CString strCurCommChannelNum = TEXT("");
    strCurCommChannelNum.Format(TEXT("%d"), m_btCurCommChannelNum);
    return strCurCommChannelNum;
}

CString CCfgInfo::GetMaxCommChannelNum() const
{
    CString strMaxCommChannelNum = TEXT("");
    strMaxCommChannelNum.Format(TEXT("%d"), m_btMaxCommChannelNum);
    return strMaxCommChannelNum;
}

CString CCfgInfo::GetCurTransSpeed() const
{
    CString strCurBand = TEXT("");
    strCurBand.Format(TEXT("%d"), m_dwCurTransSpeed);
    return strCurBand;
}

CString CCfgInfo::GetCurFreq() const
{
    CString strCurFreq = TEXT("");
    strCurFreq.Format(TEXT("%d"), m_dwCurFreq);
    return strCurFreq;
}

CString CCfgInfo::GetDeviceID() const
{
    CString strDeviceID = TEXT("");
    strDeviceID.Format(TEXT("%08X"), m_dwDevID);
    return strDeviceID;
}

CString CCfgInfo::GetProductDate() const
{
    CString strProductDate = TEXT("");
    strProductDate.Format(TEXT("20%d-%d-%d"), m_stProductDate.btYear, m_stProductDate.btMonth,
                          m_stProductDate.btDay);
    return strProductDate;
}

CString CCfgInfo::GetHardwareVer() const
{
    CString  strHardwareVer = TEXT("");
    strHardwareVer.Format(TEXT("%d.%d"), m_stHardwareVer.btHighVer, m_stHardwareVer.btLowVer);
    return strHardwareVer;
}

CString CCfgInfo::GetSoftwareVer() const
{
    CString strSoftwareVer = TEXT("");
    strSoftwareVer.Format(TEXT("%d.%d"), m_stSoftwareVer.btHighVer, m_stSoftwareVer.btLowVer);
    return strSoftwareVer;
}

CString CCfgInfo::GetProductType() const
{
    CString strProductType = TEXT("");
    strProductType.Format(TEXT("%08X"), m_uProductType);
    return strProductType;
}

CString CCfgInfo::GetMdlNetID(void) const
{
    CString strMdlNetID = L"";
    strMdlNetID.Format(L"%d", m_sMdlNetID);
    return strMdlNetID;
}




