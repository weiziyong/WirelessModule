//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : CCfgMdlNetID.h
//  @ Date : 2014/11/5 ������
//  @ Author :
//
//


#if !defined(__CFG_MDL_NET_ID)
#define __CFG_MDL_NET_ID

#include "CfgProtocol.h"

class CCfgMdlNetID : public CCfgProtocol
{
    public:
        CCfgMdlNetID(CCommIntf* pCommIntf, const USHORT& uMdlNetID);
        virtual ~CCfgMdlNetID();

    protected:
        BOOL CreateSendCmd();
        BOOL CreateAckCmd();

    private:
        USHORT m_uMdlNetID;
};

#endif  //__CFG_MDL_NET_ID
