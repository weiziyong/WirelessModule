#ifndef __CONNECTION_H
#define __CONNECTION_H

#include "CfgProtocol.h"

class CConnCmd : public CCfgProtocol
{
    public:
        CConnCmd(CCommIntf* pCommIntf);
        virtual ~CConnCmd();

    protected:
        virtual BOOL CreateSendCmd();
        virtual BOOL CreateAckCmd();
        virtual BOOL IsValidAckCmd();
};

#endif// __CONNECTION_H