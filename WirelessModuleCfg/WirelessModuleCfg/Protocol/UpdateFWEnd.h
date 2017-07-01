#ifndef __UPDATE_FW_END_H
#define __UPDATE_FW_END_H

#include "CfgProtocol.h"

class CUpdateFwEnd : public CCfgProtocol
{
    public:
        CUpdateFwEnd(CCommIntf* pCommIntf);
        virtual ~CUpdateFwEnd();

    protected:
        virtual BOOL CreateSendCmd();
        virtual BOOL CreateAckCmd();
        virtual BOOL IsValidAckCmd();
};

#endif// __CONNECTION_H