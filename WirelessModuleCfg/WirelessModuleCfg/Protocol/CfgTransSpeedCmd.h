//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : CCfgTransSpeedCmd.h
//  @ Date : 2014/11/5 ������
//  @ Author :
//
//


#if !defined(_CCFGBANDCMD_H)
#define _CCFGBANDCMD_H

#include "CfgProtocol.h"

class CCfgTransSpeedCmd : public CCfgProtocol
{
    public:
        CCfgTransSpeedCmd(CCommIntf* const pCommIntf, const DWORD dwTransSpeed);
        virtual ~CCfgTransSpeedCmd();

    protected:
        BOOL CreateSendCmd();
        BOOL CreateAckCmd();

    private:
        BYTE GetTransSpeedIdx();

    private:
        DWORD m_dwTransSpeed;
};

#endif  //_CCFGBANDCMD_H




