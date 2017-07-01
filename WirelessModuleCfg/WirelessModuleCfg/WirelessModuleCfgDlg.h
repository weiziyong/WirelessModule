
// WirelessModuleCfgDlg.h : header file
//

#pragma once

#include "./Protocol/CfgInfo.h"
#include "afxcmn.h"
#include <comdef.h>//初始化一下com口
#include "GdiPlus.h"
#include "afxlinkctrl.h"
#include <memory>


using namespace Gdiplus;
using namespace std;


// CWirelessModuleCfgDlg dialog
class CWirelessModuleCfgDlg : public CDialogEx
{
        // Construction
    public:
        CWirelessModuleCfgDlg(CWnd* pParent = NULL);    // standard constructor

        CString getFirewareFile(void);
        BOOL readFirmwareCode(CString& strFileName,
                              auto_ptr<UCHAR>& pszUpdateHexCode,
                              ULONG& uFileSize);
        CString getErrorCodeDesc(const USHORT& uErrorCode) const;
        DWORD GetCurComPortNum();

        UCHAR* getUpgradeHexCode();
        ULONG getUpgradeFileSize();
        void SetUpradeEndCtrlStatus();
        void SetStatusInfo(CString strStatusInfo);
        void InitProgBar();
        void HideProgressBar(const BOOL bHide = TRUE);

        afx_msg void OnBnClickedBtnScanDevice();
        afx_msg void OnBnClickedSaveCurDevice();
        afx_msg void OnSelchangeListAlreadyLinkedComPort();
        afx_msg LRESULT OnSetProgressRange(WPARAM wParam, LPARAM lParam);
        afx_msg LRESULT OnIncProgressBarProgress(WPARAM wParam, LPARAM lParam);
        afx_msg LRESULT OnFirmwareUpgradeTips(WPARAM wParam, LPARAM lParam);
        afx_msg LRESULT OnFwUpgradeResetProgBar(WPARAM wParam, LPARAM lParam);
        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg void OnBnClickedBtnAbout();
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnBnClickedBtnUpdateFirmware();
        afx_msg void OnBnClickedBtnSelUpgradeFile();
        afx_msg void OnBnClickedBtnStopUpgrade();

        // Dialog Data
        enum { IDD = IDD_WIRELESSMODULECFG_DIALOG };

        CWinThread* m_pUpgradeFWThread;
        BOOL m_bStopUpgradeFirmware;
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        static UINT __cdecl ScanDeviceFunc(LPVOID pParam);

        // Implementation
        HICON m_hIcon;

        DWORD GetMdlNetID();
        BOOL CheckCfgParam(void);
        void DisplayBaudRate(CCfgInfo CfgInfo);
        void DisplayCommChannelNum(CCfgInfo CfgInfo);
        void DisplayTransSpeed(CCfgInfo CfgInfo);

        void UpdateToGui(CCfgInfo CfgInfo);

        // Generated message map functions
        virtual BOOL OnInitDialog();
        afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
        afx_msg void OnPaint();
        afx_msg HCURSOR OnQueryDragIcon();
        DECLARE_MESSAGE_MAP()

    private:
        BOOL GetCurBaudRateRange(CCfgInfo CfgInfo, DWORD& dwCurBaudRateIdx, DWORD& dwMinBaudRateIdx,
                                 DWORD& dwMaxBaudRateIdx);

        void AddAllBaudRateToCombox(const DWORD dwMinBaudRateIdx, const DWORD dwMaxBaudRateIdx);
        void SetCurBaudRate(CCfgInfo CfgInfo);
        void SetReadedBaudRateFromDev(CString strCurReadBaudRate);
        CString GetCurCommPortName();
        DWORD GetCurBaudRate();
        BOOL GetPreBaudRate(CString strComPortName, DWORD& dwPreBaudRate);
        BOOL CfgCurBaudRate(const DWORD dwCurBaudRate);
        void UpdateAlreadyPortList(CString& strCurSelComPortName);
        void ScanDevice();

        void EnableCtrl(const DWORD dwCrlID, const BOOL bEnable = TRUE);
        void ClearPreDisplayInfo();
        BOOL ImageFromIDResource(UINT nID, LPCTSTR sTR, Image*& pImg);

        CString GetCurProgDir();
        BOOL GetTransSpeedMapTable(CString strCurWorkFreq,
                                   const DWORD*& pTransSpeedMapTable,
                                   DWORD& dwTotalTransSpeedNums);

        void InitializeSys();
        void InitLogSys(void);
        void InitCfgCtrlStatus();
        void AuoExplorerComPort();

        void InitUpgradeCtrlStatus();
        void OpenUpgradeCtrlStatus();
        void SetUpgradingCtrlStatus();


        CMap<CString, LPCTSTR, DWORD, DWORD> m_mapAlreadyLinkedDevCom;
        CProgressCtrl m_ctrlProgBar;
        DWORD m_dwProgOffsetToBottom;
        INT m_iMaxProgressVal;
        INT m_iMinProgressVal;
        CFont m_Font;
        CRect m_rcDlg;
        CRect m_rcProg;
        BOOL m_bInitDlg;
        auto_ptr<UCHAR> m_pUpdateHexCode;
        ULONG m_uUpgradeFileSize;
    public:
        afx_msg void OnEnChangeEditMdlNetId();
};







