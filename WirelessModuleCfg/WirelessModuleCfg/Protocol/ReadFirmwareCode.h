#ifndef __READ_FIRMWARE_CODE_H
#define __READ_FIRMWARE_CODE_H

#include "CfgProtocol.h"

class CReadFirmwareCode : public CCfgProtocol
{
    public:
        CReadFirmwareCode(CCommIntf* pCommIntf,
                          const USHORT& sPageNum,
                          const USHORT& sPageSize);

        virtual ~CReadFirmwareCode();

    protected:
        virtual BOOL CreateSendCmd();
        virtual BOOL CreateAckCmd();
        virtual BOOL IsValidAckCmd();

    private:
        const USHORT m_sPageNum;
        const USHORT& m_sPageSize;
};

#endif // !__READ_FIRMWARE_CODE_H
