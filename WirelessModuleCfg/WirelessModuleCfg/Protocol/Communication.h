//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : CCommunication.h
//  @ Date : 2014/10/29 ������
//  @ Author :
//
//


#if !defined(_CCOMMUNICATION_H)
#define _CCOMMUNICATION_H

#include "CommIntf.h"
#include "../../Com/CnComm.h"
#include "ProtocolConst.h"


class CCommunication : public CCommIntf
{
    public:
        CCommunication(const DWORD dwPort,
                       const DWORD dwBaudRate,
                       const DWORD dwTimeOut = CProtocolConst::COMM_TIMEOUT);
        virtual ~CCommunication();

        BOOL SendData(PBYTE const pSendDataBuf, const DWORD dwLen);
        BOOL ReadData(PBYTE const pReadDataBuf, const DWORD dwReadLen, DWORD& dwReadBytes);
        BOOL Close();

        void SetCurBaudRate(const DWORD dwPortNum, const DWORD dwBaudRate);
        void GetCurBaudRate(DWORD& dwPortNum, DWORD& dwBaudRate) const;

    protected:
        virtual BOOL IsOpen();
        virtual BOOL Open();

    private:
        CnComm m_ComPort;
        DWORD m_dwPort;
        DWORD m_dwBaudRate;
        DWORD m_dwTimeOut;
};

#endif  //_CCOMMUNICATION_H








