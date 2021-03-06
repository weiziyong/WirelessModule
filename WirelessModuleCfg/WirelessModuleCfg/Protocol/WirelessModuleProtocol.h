//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : CWirelessModuleProtocol.h
//  @ Date : 2014/10/29 星期三
//  @ Author :
//
//


#if !defined(_CWIRELESSMODULEPROTOCOL_H)
#define _CWIRELESSMODULEPROTOCOL_H

#include "CfgInfo.h"
#include "CfgProtocol.h"

class CWirelessModuleProtocol
{
    public:
        CWirelessModuleProtocol();
        virtual ~CWirelessModuleProtocol();

        BOOL SendCfgBaudRateCmd(CCommIntf* const pCommIntf,
                                const DWORD dwBaudRate);
        BOOL SendCfgChannelNumCmd(CCommIntf* const pCommIntf,
                                  const BYTE btCommChannelNum);
        BOOL SendCfgTransSpeedCmd(CCommIntf* const pCommIntf,
                                  const DWORD dwTransSpeed);
        BOOL SendSearchCmd(CCommIntf* const pCommIntf,
                           CCfgInfo& AllCfgInfo);

        BOOL SendCfgMdlNetID(CCommIntf* const pCommIntf,
                             const USHORT& uMdlNetID);

        STATUS_CODE UpdateFirmware(const DWORD& dwCurComPortNum,
                                   const UCHAR* pHexFileCode,
                                   const UINT& uFileSize,
                                   const BOOL& bFlagStopUpgrade,
                                   const HWND& hWnd);

    protected:
        // -----------------------固件升级相关命令--------------------------------
        // 发送连接设备命令
        BOOL SendConnectDeviceCmd(const DWORD& dwCurComPortNum,
                                  USHORT& sDevID,
                                  USHORT& sPageTotal,
                                  USHORT& sPageSize
                                 );

        // 发送一页固件代码
        BOOL SendFWPageCode(const DWORD& dwCurComPortNum,
                            const USHORT& sPageNum,
                            const UCHAR* pFirmwarePageCode,
                            const USHORT& sPageSize);

        // 读指定地址固件文件
        BOOL ReadFirmwarePageCode(const DWORD&  dwCurComPortNum,
                                  const USHORT& sPageNum,
                                  UCHAR* pFirmwarePageCode,
                                  const USHORT& sPageSize);

        // 发送更新固件完成命令
        BOOL SendUpdateFirmwareEnd(const DWORD&  dwCurComPortNum);

        BOOL SendCfgCmd(CCfgProtocol* const pCfgProtocol);

        BOOL ConnectDevice(const DWORD& dwCurComPortNum,
                           USHORT& sDevID,
                           USHORT& sPageTotal,
                           USHORT& sPageSize,
                           const USHORT& iMaxTryToConnectTimes =
                                           CProtocolConst::MAX_TRY_CONNECT_DEVICE_TIMES);

        STATUS_CODE SendFirmwareCode(const DWORD& dwCurComPortNum,
                                     const USHORT& sPageSize,
                                     const BYTE*& pHexFileCode,
                                     const UINT& uFileSize,
                                     const BOOL& bFlagStopUpgrade,
                                     const HWND& hWnd);

        BOOL CheckSentFWPageCode(const DWORD&  dwCurComPortNum,
                                 const UINT& iPageIdx,
                                 const UCHAR* pCurSendedCode,
                                 const USHORT& sPageSize);

        BOOL SendAndCheckPageCode(const DWORD& dwCurComPortNum,
                                  const USHORT& iPageIdx,
                                  const UCHAR* pCurPageCode,
                                  const USHORT& sPageSize);

        BOOL isHexFileGtFlash(const UINT& iHexFileSize,
                              const USHORT& sPageTotal,
                              const USHORT& sPageSize) const;
};

#endif  //_CWIRELESSMODULEPROTOCOL_H











