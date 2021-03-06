//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : CCfgInfo.h
//  @ Date : 2014/10/29 ������
//  @ Author :
//
//


#if !defined(_CCFGINFO_H)
#define _CCFGINFO_H

#include "../stdafx.h"

class PRODUCT_DATE
{

    public:
        BYTE btYear;
        BYTE btMonth;
        BYTE btDay;
};

class PRODUCT_VER
{
    public:
        BYTE btHighVer;
        BYTE btLowVer;
};

class CCfgInfo
{
    public:
        CCfgInfo();
        virtual ~CCfgInfo();

        CString GetComPortName() const;
        CString GetCurBaudRate() const;
        CString GetMinBaudRate() const;
        CString GetMaxBaudRate() const;
        CString GetCurCommChannelNum() const;
        CString GetMaxCommChannelNum() const;
        CString GetCurTransSpeed() const;
        CString GetCurFreq() const;
        CString GetDeviceID() const;
        CString GetProductDate() const;
        CString GetHardwareVer() const;
        CString GetSoftwareVer() const;
        CString GetProductType() const;
        CString GetMdlNetID(void) const;

    public:
        CString m_strComPortName;
        DWORD m_dwCurBaudRate;
        DWORD m_dwMinBaudRate;
        DWORD m_dwMaxBaudRate;
        BYTE m_btCurCommChannelNum;
        BYTE m_btMaxCommChannelNum;
        DWORD m_dwCurTransSpeed;
        DWORD m_dwCurFreq;
        DWORD m_dwDevID;
        PRODUCT_DATE m_stProductDate;
        PRODUCT_VER m_stHardwareVer;
        PRODUCT_VER m_stSoftwareVer;
        USHORT m_uProductType;
        USHORT m_sMdlNetID;
};

#endif  //_CCFGINFO_H





