#ifndef __SEND_FIRMWARE_CODE_H
#define __SEND_FIRMWARE_CODE_H

#include "CfgProtocol.h"

class CSendFirmwareCode : public CCfgProtocol
{
    public:
        CSendFirmwareCode(CCommIntf* pCommIntf,
                          const USHORT& sPageNum,
                          const UCHAR* pHexCode,
                          USHORT sHexCodeSize);

        virtual ~CSendFirmwareCode();

    private:
        virtual BOOL CreateSendCmd();
        virtual BOOL CreateAckCmd();
        virtual BOOL IsValidAckCmd();

        BOOL IsCmdIDEqual();
        BOOL IsCheckSumEqaul();
    private:
        const USHORT m_sPageNum;
        const UCHAR* m_pHexCode;
        const USHORT m_sHexCodeSize;
};

#endif // !__SEND_FIRMWARE_CODE_H
