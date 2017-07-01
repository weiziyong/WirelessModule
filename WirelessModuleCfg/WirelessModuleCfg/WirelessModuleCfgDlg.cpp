
// WirelessModuleCfgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WirelessModuleCfg.h"
#include "WirelessModuleCfgDlg.h"
#include "afxdialogex.h"
#include "LogSystem.h"
#include "./Protocol/ComPortExplorer.h"
#include "./Protocol/WirelessModuleProtocol.h"
#include "./Protocol/Communication.h"
#include "./Protocol/CfgInfo.h"


#pragma comment(lib,"gdiplus.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_PROGRESS_MAX_VALUE (WM_USER + 1)
#define WM_PROGRESS_INCREMENT (WM_PROGRESS_MAX_VALUE + 1)

#define COMPANY_DESC  TEXT("רҵ���߽������������\r\n�з���" \
                           "���Ƹ����������ݴ���ģ�顢���������ն�")

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
    public:
        CAboutDlg();

        // Dialog Data
        enum { IDD = IDD_ABOUTBOX };

    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

        // Implementation
    protected:
        DECLARE_MESSAGE_MAP()
    public:
        virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    CString strAboutMenu;
    BOOL bNameValid = strAboutMenu.LoadString(IDS_STRING_ABOUT_DLG_TITLE);
    ASSERT(bNameValid);
    if (!strAboutMenu.IsEmpty())
    {
        SetWindowText(strAboutMenu);
    }

    return TRUE;
}

// CWirelessModuleCfgDlg dialog



CWirelessModuleCfgDlg::CWirelessModuleCfgDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CWirelessModuleCfgDlg::IDD, pParent)
{
    /*IDI_ICON_TITLE*/
    m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_TITLE_BIG);

    m_mapAlreadyLinkedDevCom.RemoveAll();

    m_dwProgOffsetToBottom = 6;

    m_iMinProgressVal = 1;
    m_iMaxProgressVal = 1;

    m_rcDlg.SetRectEmpty();
    m_rcProg.SetRectEmpty();

    m_bInitDlg = FALSE;

    m_pUpdateHexCode.release();
    m_uUpgradeFileSize = 0;

    m_bStopUpgradeFirmware = FALSE;
}

void CWirelessModuleCfgDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PROG_BAR, m_ctrlProgBar);
}

BEGIN_MESSAGE_MAP(CWirelessModuleCfgDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_SCAN_DEVICE,
                  &CWirelessModuleCfgDlg::OnBnClickedBtnScanDevice)
    ON_BN_CLICKED(IDC_SAVE_CUR_DEVICE,
                  &CWirelessModuleCfgDlg::OnBnClickedSaveCurDevice)
    ON_LBN_SELCHANGE(IDC_LIST_ALREADY_LINKED_COM_PORT,
                     &CWirelessModuleCfgDlg::OnSelchangeListAlreadyLinkedComPort)
    ON_WM_SIZE()
    ON_MESSAGE(WM_PROGRESS_MAX_VALUE, CWirelessModuleCfgDlg::OnSetProgressRange)
    ON_MESSAGE(WM_PROGRESS_INCREMENT,
               CWirelessModuleCfgDlg::OnIncProgressBarProgress)
    ON_MESSAGE(WM_FIRMWARE_UPGRADE_TIP,
               CWirelessModuleCfgDlg::OnFirmwareUpgradeTips)
    ON_MESSAGE(WM_FW_UPGRADE_RESET_PROG_BAR,
               CWirelessModuleCfgDlg::OnFwUpgradeResetProgBar)
    ON_BN_CLICKED(IDC_BTN_ABOUT, &CWirelessModuleCfgDlg::OnBnClickedBtnAbout)
    ON_WM_CREATE()
    ON_BN_CLICKED(IDC_BTN_UPDATE_FIRMWARE,
                  &CWirelessModuleCfgDlg::OnBnClickedBtnUpdateFirmware)
    ON_BN_CLICKED(IDC_BTN_SEL_UPGRADE_FILE, &CWirelessModuleCfgDlg::OnBnClickedBtnSelUpgradeFile)
    ON_BN_CLICKED(IDC_BTN_STOP_UPGRADE, &CWirelessModuleCfgDlg::OnBnClickedBtnStopUpgrade)
    ON_EN_CHANGE(IDC_EDIT_MDL_NET_ID, &CWirelessModuleCfgDlg::OnEnChangeEditMdlNetId)
END_MESSAGE_MAP()

// CWirelessModuleCfgDlg message handlers

BOOL CWirelessModuleCfgDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    // TODO: Add extra initialization here
    InitializeSys();

    CComboBox* pComPort = (CComboBox*)GetDlgItem(IDC_CBX_COM_SEL);
    pComPort->SetCurSel(0);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWirelessModuleCfgDlg::InitializeSys()
{
    InitLogSys();
    InitCfgCtrlStatus();
    m_bInitDlg = TRUE;

    HideProgressBar();
    AuoExplorerComPort();

    InitUpgradeCtrlStatus();
}

void CWirelessModuleCfgDlg::AuoExplorerComPort()
{
    CStringArray allComPort;
    CComPortExplorer comPortSniffer;
    if (!comPortSniffer.GetSysAllComPort(allComPort))
    {
        LOG_ERROR(L"failed to get all the com port of the system.");
        return;
    }

    if (allComPort.GetSize() == 0)
    {
        LOG_ERROR(L"system has no com port.");
        return;
    }

    CComboBox* pComPort = (CComboBox*)GetDlgItem(IDC_CBX_COM_SEL);
    for (INT iIdx = 0; iIdx < allComPort.GetSize(); ++iIdx)
    {
        pComPort->AddString(allComPort[iIdx]);
    }

    pComPort->SetCurSel(0);
    CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COM_PORT_NAME);
    CString strCom;
    pComPort->GetWindowText(strCom);
    pEdit->SetWindowText(strCom);
}

void CWirelessModuleCfgDlg::InitLogSys(void)
{
    CString strCurProgDir = GetCurProgDir();

    TCHAR szCfgFile[MAX_PATH] = { 0 };
    _tcsncpy_s(szCfgFile, MAX_PATH,
               CString(strCurProgDir + LOG_CFG_FILE_NAME).GetBuffer(), MAX_PATH);
    if (FALSE == LOG_INIT(szCfgFile))
    {
        AfxMessageBox(TEXT("��־ϵͳ��ʼ������ʧ�ܣ�"));
    }
    else
    {
        LOG_INFO(TEXT("program start, init log system successful."));
    }
}

void CWirelessModuleCfgDlg::InitCfgCtrlStatus()
{
    EnableCtrl(IDC_LIST_ALREADY_LINKED_COM_PORT, FALSE);
    EnableCtrl(IDC_CBX_BAUDRATE, FALSE);
    EnableCtrl(IDC_CBX_COMM_CHANNEL_NUM, FALSE);
    EnableCtrl(IDC_CBX_CUR_TRANS_SPEED, FALSE);
    EnableCtrl(IDC_EDIT_MDL_NET_ID, FALSE);
    EnableCtrl(IDC_SAVE_CUR_DEVICE, FALSE);
    EnableCtrl(IDC_BTN_SCAN_DEVICE, TRUE);
}

void CWirelessModuleCfgDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
}

int CWirelessModuleCfgDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialogEx::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    GetWindowRect(&m_rcDlg);

    return 0;
}

VOID CWirelessModuleCfgDlg::HideProgressBar(const BOOL bHide)
{
    if (m_bInitDlg)
    {
        // ��������ʼ��
        InitProgBar();
        m_bInitDlg = FALSE;
    }

    m_ctrlProgBar.ShowWindow(SW_SHOW);
    INT iDlgHeight = m_rcDlg.Height();
    if (bHide)
    {
        iDlgHeight = m_rcDlg.Height() - m_rcProg.Height() - m_dwProgOffsetToBottom;
        m_ctrlProgBar.ShowWindow(SW_HIDE);
    }

    SetWindowPos(NULL, m_rcDlg.left, m_rcDlg.top, m_rcDlg.Width(),
                 iDlgHeight, SWP_NOMOVE);
}

void CWirelessModuleCfgDlg::InitProgBar()
{
    m_ctrlProgBar.GetWindowRect(&m_rcProg);
    m_ctrlProgBar.ShowWindow(SW_HIDE);

    m_ctrlProgBar.SetRange32(0, 1);
    m_ctrlProgBar.SetStep(1);
    m_ctrlProgBar.SetPos(0);
}

void CWirelessModuleCfgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        //   CAboutDlg dlgAbout;
        //  dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWirelessModuleCfgDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {

        CDialogEx::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWirelessModuleCfgDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CWirelessModuleCfgDlg::UpdateToGui(CCfgInfo CfgInfo)
{
    CEdit* pComPortName = (CEdit*)GetDlgItem(IDC_EDIT_COM_PORT_NAME);
    pComPortName->SetWindowText(CfgInfo.m_strComPortName);

    // ��ʾ������
    DisplayBaudRate(CfgInfo);

    // ��ʾͨ���ŵ�
    DisplayCommChannelNum(CfgInfo);

    // ��ʾ��ǰ��������
    DisplayTransSpeed(CfgInfo);

    CEdit* pCurWorkFreq = (CEdit*)GetDlgItem(IDC_EDIT_CUR_WORK_FREQ);
    pCurWorkFreq->SetWindowText(CfgInfo.GetCurFreq());

    CEdit* pDeviceID = (CEdit*)GetDlgItem(IDC_EDIT_DEV_ID);
    pDeviceID->SetWindowText(CfgInfo.GetDeviceID());

    CEdit* pHardwareVer = (CEdit*)GetDlgItem(IDC_EDIT_HARDWARE_VER);
    pHardwareVer->SetWindowText(CfgInfo.GetHardwareVer());

    CEdit* pProductDate = (CEdit*)GetDlgItem(IDC_EDIT_PRODUCT_DATE);
    pProductDate->SetWindowText(CfgInfo.GetProductDate());

    CEdit* pSoftwareVer = (CEdit*)GetDlgItem(IDC_EDIT_SOFTWARE_VER);
    pSoftwareVer->SetWindowText(CfgInfo.GetSoftwareVer());

    CEdit* pProductType = (CEdit*)GetDlgItem(IDC_EDIT_PRODUCT_TYPE);
    pProductType->SetWindowText(CfgInfo.GetProductType());

    // ��ʾģ������ID��
    CEdit* pMdlNetID = (CEdit*)GetDlgItem(IDC_EDIT_MDL_NET_ID);
    pMdlNetID->SetWindowText(CfgInfo.GetMdlNetID());

    SetStatusInfo(TEXT("�ɹ���ȡ���ӵ��˿ڣ� ") + CfgInfo.m_strComPortName +
                  TEXT(" (�����ʣ�") + CfgInfo.GetCurBaudRate() +
                  TEXT(" bps) �ϵ��豸��Ϣ����ɣ�"));
}

void CWirelessModuleCfgDlg::DisplayBaudRate(CCfgInfo CfgInfo)
{
    DWORD dwCurBaudRateIdx = CProtocolConst::INVALID_BAUDRATE_IDX;
    DWORD dwMinBaudRateIdx = CProtocolConst::INVALID_BAUDRATE_IDX;
    DWORD dwMaxBaudRateIdx = CProtocolConst::INVALID_BAUDRATE_IDX;
    if (!GetCurBaudRateRange(CfgInfo,
                             dwCurBaudRateIdx,
                             dwMinBaudRateIdx,
                             dwMaxBaudRateIdx))
    {
        LOG_ERROR("Get the min baudrate index, the max baudrate index,"
                  " the cur baudrate index, failed !");
        return;
    }

    // ��ʾ������ֵ���б����
    AddAllBaudRateToCombox(dwMinBaudRateIdx, dwMaxBaudRateIdx);

    SetCurBaudRate(CfgInfo);
}

BOOL CWirelessModuleCfgDlg::GetCurBaudRateRange(CCfgInfo CfgInfo,
        DWORD& dwCurBaudRateIdx,
        DWORD& dwMinBaudRateIdx,
        DWORD& dwMaxBaudRateIdx)
{
    dwCurBaudRateIdx = CProtocolConst::INVALID_BAUDRATE_IDX;
    dwMinBaudRateIdx = CProtocolConst::INVALID_BAUDRATE_IDX;
    dwMaxBaudRateIdx = CProtocolConst::INVALID_BAUDRATE_IDX;
    for (size_t iIdx = 0; iIdx < CProtocolConst::MAX_BAUDRATE_IDX; iIdx++)
    {
        if (CProtocolConst::m_aBaudRateMapTable[iIdx] == CfgInfo.m_dwMinBaudRate)
        {
            dwMinBaudRateIdx = iIdx + 1;
        }

        if (CProtocolConst::m_aBaudRateMapTable[iIdx] == CfgInfo.m_dwMaxBaudRate)
        {
            dwMaxBaudRateIdx = iIdx + 1;
        }

        if (CProtocolConst::m_aBaudRateMapTable[iIdx] == CfgInfo.m_dwCurBaudRate)
        {
            dwCurBaudRateIdx = iIdx + 1;
        }
    }

    CString strLog = TEXT("");
    if ((CProtocolConst::INVALID_BAUDRATE_IDX == dwCurBaudRateIdx) ||
            (CProtocolConst::INVALID_BAUDRATE_IDX == dwMinBaudRateIdx) ||
            (CProtocolConst::INVALID_BAUDRATE_IDX == dwMaxBaudRateIdx))
    {
        LOG_ERROR("dwMinBaudRateIdx = " << dwMinBaudRateIdx <<
                  " , dwMaxBaudRateIdx = " << dwMaxBaudRateIdx <<
                  " , dwCurBaudRateIdx = " << dwCurBaudRateIdx <<
                  " , have at least, one value of index for baudrate that is "
                  "invalid, the invalide baudrate value is : " <<
                  CProtocolConst::INVALID_BAUDRATE_IDX << " exception !");
        SetReadedBaudRateFromDev(CfgInfo.GetCurBaudRate());
        AddAllBaudRateToCombox(CProtocolConst::MIN_BAUDRATE_IDX,
                               CProtocolConst::MAX_BAUDRATE_IDX);
        return FALSE;
    }

    if (dwMinBaudRateIdx > dwMaxBaudRateIdx)
    {
        LOG_ERROR("dwMinBaudRateIdx = " << dwMinBaudRateIdx << "great than" <<
                  "dwMaxBaudRateIdx = " <<
                  dwMaxBaudRateIdx << ", display baudrate failed !");
        SetReadedBaudRateFromDev(CfgInfo.GetCurBaudRate());
        AddAllBaudRateToCombox(CProtocolConst::MIN_BAUDRATE_IDX,
                               CProtocolConst::MAX_BAUDRATE_IDX);
        return FALSE;
    }

    if ((dwCurBaudRateIdx < dwMinBaudRateIdx) || (dwCurBaudRateIdx > dwMaxBaudRateIdx))
    {
        LOG_ERROR("dwCurBaudRateIdx = " << dwCurBaudRateIdx <<
                  ", not between the range: [" << dwMinBaudRateIdx << "~" <<
                  dwMaxBaudRateIdx << "]");
        SetReadedBaudRateFromDev(CfgInfo.GetCurBaudRate());
        AddAllBaudRateToCombox(CProtocolConst::MIN_BAUDRATE_IDX,
                               CProtocolConst::MAX_BAUDRATE_IDX);
        return FALSE;
    }

    return TRUE;
}

void CWirelessModuleCfgDlg::AddAllBaudRateToCombox(const DWORD dwMinBaudRateIdx,
        const DWORD dwMaxBaudRateIdx)
{
    CComboBox* pcbxBaudRate = (CComboBox*)GetDlgItem(IDC_CBX_BAUDRATE);
    pcbxBaudRate->ResetContent();

    for (size_t iIdxBaudRate = dwMinBaudRateIdx - 1;
            iIdxBaudRate < dwMaxBaudRateIdx; ++iIdxBaudRate)
    {
        CString strBaudRate = TEXT("");
        strBaudRate.Format(TEXT("%d"),
                           CProtocolConst::m_aBaudRateMapTable[iIdxBaudRate]);

        if (CB_ERR == pcbxBaudRate->AddString(strBaudRate.GetBuffer()))
        {
            LOG_ERROR("Add baudrate: " << strBaudRate <<
                      "to the combox of baudrate, failed !");
        }
    }
}

void CWirelessModuleCfgDlg::SetCurBaudRate(CCfgInfo CfgInfo)
{
    CComboBox* pcbxBaudRate = (CComboBox*)GetDlgItem(IDC_CBX_BAUDRATE);
    DWORD iIdxCbxBaudRateItem = pcbxBaudRate->FindString(-1, CfgInfo.GetCurBaudRate());
    if (CB_ERR == iIdxCbxBaudRateItem)
    {
        LOG_ERROR("Not find the baudrate's value : " << CfgInfo.GetCurBaudRate() <<
                  "in the combox of baudrate, failed !");
        return;
    }

    if (CB_ERR == pcbxBaudRate->SetCurSel(iIdxCbxBaudRateItem))
    {
        CString strLog = TEXT("�ڲ�������Ͽ���б����У��趨��ǰ�����ʣ�") +
                         CfgInfo.GetCurBaudRate() + TEXT("ʧ�ܣ�");
        LOG_ERROR("Set the current baudrate:(" << CfgInfo.GetCurBaudRate().GetBuffer() <<
                  "bps), in the combox of baudrate, failed !");
        return;
    }
}

void CWirelessModuleCfgDlg::SetReadedBaudRateFromDev(CString strCurReadBaudRate)
{
    CComboBox* pcbxBaudRate = (CComboBox*)GetDlgItem(IDC_CBX_BAUDRATE);
    pcbxBaudRate->SetWindowText(strCurReadBaudRate);
}

void CWirelessModuleCfgDlg::DisplayCommChannelNum(CCfgInfo CfgInfo)
{
    CString strLog = TEXT("");
    CComboBox* pcbxComChannelNum = (CComboBox*)GetDlgItem(IDC_CBX_COMM_CHANNEL_NUM);
    pcbxComChannelNum->ResetContent();
    for (size_t iIdxChannelNum = 1; iIdxChannelNum <= CfgInfo.m_btMaxCommChannelNum; ++iIdxChannelNum)
    {
        CString strCommChannelNum = TEXT("");
        strCommChannelNum.Format(TEXT("%d"), iIdxChannelNum);
        if (CB_ERR == pcbxComChannelNum->AddString(strCommChannelNum.GetBuffer()))
        {
            LOG_ERROR("Add the communication's channel num : " << strCommChannelNum.GetBuffer() <<
                      "to the combox of commnication's channel num, failed !");
            return;
        }
    }

    // ���õ�ǰ�豸����ʹ�õ��ŵ����
    INT iIdxCommCurChannelNum = pcbxComChannelNum->FindString(-1, CfgInfo.GetCurCommChannelNum());
    if (CB_ERR == iIdxCommCurChannelNum)
    {
        LOG_ERROR("Not find the current channel number of communication : " <<
                  CfgInfo.GetCurCommChannelNum().GetBuffer() <<
                  ", in the channel's num combox of communication, failed !");
        pcbxComChannelNum->SetWindowText(CfgInfo.GetCurCommChannelNum());
        return;
    }

    if (CB_ERR == pcbxComChannelNum->SetCurSel(iIdxCommCurChannelNum))
    {
        LOG_ERROR("Set the current communication channel number :" <<
                  CfgInfo.GetCurCommChannelNum().GetBuffer() <<
                  "in the combox of communication's channel num, failed !");
        pcbxComChannelNum->SetWindowText(CfgInfo.GetCurCommChannelNum());
        return;
    }
}

void  CWirelessModuleCfgDlg::DisplayTransSpeed(CCfgInfo CfgInfo)
{
    CString strLog = TEXT("");
    CComboBox* pcbxCurTransSpeed = (CComboBox*)GetDlgItem(IDC_CBX_CUR_TRANS_SPEED);
    pcbxCurTransSpeed->ResetContent();

    // ���Ӱ�������ģ�鹤��Ƶ��ѡ��ͬ�Ĳ�����ӳ���
    const DWORD* pTransSpeedMapTable = NULL;
    DWORD dwTotalTransSpeedNums = 0;
    if (!GetTransSpeedMapTable(CfgInfo.GetCurFreq(),
                               pTransSpeedMapTable,
                               dwTotalTransSpeedNums))
    {
        LOG_ERROR("Not find the suitable trans's speed map table, failed !");
        return;
    }

    // ��ʾ�����ʵ�λ��bpsת��Ϊkbps����������ֵҪ����1000
    for (size_t iIdx = 0; iIdx < dwTotalTransSpeedNums; iIdx++)
    {
        CString strTransSpeed = TEXT("");
        strTransSpeed.Format(TEXT("%0.1f"), pTransSpeedMapTable[iIdx] / 1000.0);
        if (CB_ERR == pcbxCurTransSpeed->AddString(strTransSpeed.GetBuffer()))
        {
            LOG_ERROR("Add the current trans's speed : " << strTransSpeed.GetBuffer() <<
                      "to the combox of translation's speed, failed !");
            return;
        }
    }

    INT iCurTransSpeed = pcbxCurTransSpeed->FindString(-1, CfgInfo.GetCurTransSpeed());
    if (CB_ERR == iCurTransSpeed)
    {
        LOG_ERROR("Not find the current tran's speed : " <<
                  CfgInfo.GetCurTransSpeed().GetBuffer() <<
                  ", in the combox of tran's speed, failed !");
        return;
    }

    if (CB_ERR == pcbxCurTransSpeed->SetCurSel(iCurTransSpeed))
    {
        LOG_ERROR("Set the current tran's speed : " << CfgInfo.GetCurTransSpeed().GetBuffer() <<
                  ", in the combox of trans's speed, failed !");
        return;
    }
}

BOOL CWirelessModuleCfgDlg::GetTransSpeedMapTable(CString strCurWorkFreq,
        const DWORD*& pTransSpeedMapTable, DWORD& dwTotalTransSpeedNums)
{
    if ((0 == strCurWorkFreq.CompareNoCase(CProtocolConst::szFreq315M) ||
            (0 == strCurWorkFreq.CompareNoCase(CProtocolConst::szFreq433M))))
    {
        pTransSpeedMapTable = &CProtocolConst::m_aTransSpeed_433M[0];
        dwTotalTransSpeedNums = sizeof(CProtocolConst::m_aTransSpeed_433M) /
                                sizeof(CProtocolConst::m_aTransSpeed_433M[0]);
        return TRUE;
    }

    if ((0 == strCurWorkFreq.CompareNoCase(CProtocolConst::szFreq779M)) ||
            (0 == strCurWorkFreq.CompareNoCase(CProtocolConst::szFreq868M)) ||
            (0 == strCurWorkFreq.CompareNoCase(CProtocolConst::szFreq915M)))
    {
        pTransSpeedMapTable = &CProtocolConst::m_aTransSpeed_868M[0];
        dwTotalTransSpeedNums = sizeof(CProtocolConst::m_aTransSpeed_868M) /
                                sizeof(CProtocolConst::m_aTransSpeed_868M[0]);
        return TRUE;
    }

    return FALSE;
}

void CWirelessModuleCfgDlg::OnBnClickedBtnScanDevice()
{
    LOG_TRACE_METHORD(TEXT("CWirelessModuleCfgDlg::OnBnClickedBtnScanDevice func."));

    AfxBeginThread(CWirelessModuleCfgDlg::ScanDeviceFunc, (LPVOID)this);
}

UINT __cdecl CWirelessModuleCfgDlg::ScanDeviceFunc(LPVOID pParam)
{
    CWirelessModuleCfgDlg* pWirelessModuleCfgDlg = (CWirelessModuleCfgDlg*)pParam;
    pWirelessModuleCfgDlg->ScanDevice();

    return 0;
}

void CWirelessModuleCfgDlg::ScanDevice()
{
    LOG_TRACE_METHORD(TEXT("CWirelessModuleCfgDlg::ScanDevice func."));

    InitProgBar();
    if (!m_ctrlProgBar.IsWindowVisible())
    {
        HideProgressBar(FALSE);
    }

    CButton* pScanDev = (CButton*)GetDlgItem(IDC_BTN_SCAN_DEVICE);
    pScanDev->EnableWindow(FALSE);
    pScanDev->SetWindowText(TEXT("����ɨ���豸..."));

    // ����Ѿ����ֵ��豸�˿���Ϣ
    m_mapAlreadyLinkedDevCom.RemoveAll();

    // ����ϴ���ʾ����Ϣ���ͽ�����ʾ���б���Ϣ
    ClearPreDisplayInfo();

    SetStatusInfo(
                    TEXT("����ɨ�������ӵ���λ�����豸��������Ҫ���Ѽ����ӣ������ĵȴ�..."));

    CComPortExplorer ComPortExplore(m_hWnd);
    if (!ComPortExplore.GetCurLinkedDevCommPort(m_mapAlreadyLinkedDevCom))
    {
        CString strError =
                        TEXT("û���豸���ӵ���λ������ȷ���豸�Ƿ��Ѿ���ȷ���ӣ�");
        SetStatusInfo(strError);

        strError = TEXT("No device to be connected to the upper machine.");
        LOG_ERROR(strError);

        pScanDev->SetWindowText(TEXT("ɨ���豸"));
        InitCfgCtrlStatus();
        HideProgressBar();

        return;
    }

    if (m_mapAlreadyLinkedDevCom.IsEmpty())
    {
        SetStatusInfo(
                        TEXT("û�з��������ӵ���λ�����豸���뽫�����ӱ������ں���ִ��ɨ�����!"));
        pScanDev->SetWindowText(TEXT("ɨ���豸"));
        InitCfgCtrlStatus();
        HideProgressBar();

        return;
    }

    POSITION Pos = m_mapAlreadyLinkedDevCom.GetStartPosition();
    CString strSelComPort = TEXT("");
    DWORD dwCurBaudRate = 0;
    m_mapAlreadyLinkedDevCom.GetNextAssoc(Pos, strSelComPort, dwCurBaudRate);
    UpdateAlreadyPortList(strSelComPort);

    OnSelchangeListAlreadyLinkedComPort();

    SetStatusInfo(TEXT("�ѷ������ӵ���λ�����豸��ɨ���豸�����ɹ���ɣ�"));

    pScanDev->EnableWindow();
    pScanDev->SetWindowText(TEXT("ɨ���豸"));

    EnableCtrl(IDC_LIST_ALREADY_LINKED_COM_PORT, TRUE);
    EnableCtrl(IDC_CBX_BAUDRATE, TRUE);
    EnableCtrl(IDC_CBX_COMM_CHANNEL_NUM, TRUE);
    EnableCtrl(IDC_CBX_CUR_TRANS_SPEED, TRUE);
    EnableCtrl(IDC_EDIT_MDL_NET_ID, TRUE);
    EnableCtrl(IDC_SAVE_CUR_DEVICE, TRUE);

    HideProgressBar();
}

void CWirelessModuleCfgDlg::ClearPreDisplayInfo()
{
    CListBox* pAlreadyFindPortList =
                    (CListBox*)GetDlgItem(IDC_LIST_ALREADY_LINKED_COM_PORT);
    pAlreadyFindPortList->ResetContent();

    CEdit* pComPortName = (CEdit*)GetDlgItem(IDC_EDIT_COM_PORT_NAME);
    pComPortName->SetWindowText(TEXT(""));

    CComboBox* pcbxBaudRate = (CComboBox*)GetDlgItem(IDC_CBX_BAUDRATE);
    pcbxBaudRate->ResetContent();

    CComboBox* pcbxCommChannelNum =
                    (CComboBox*)GetDlgItem(IDC_CBX_COMM_CHANNEL_NUM);
    pcbxCommChannelNum->ResetContent();

    CComboBox* pcbxCurTransSpeed = (CComboBox*)GetDlgItem(IDC_CBX_CUR_TRANS_SPEED);
    pcbxCurTransSpeed->ResetContent();

    CEdit* peditCurMdlID = (CEdit*)GetDlgItem(IDC_EDIT_MDL_NET_ID);
    peditCurMdlID->SetWindowText(TEXT(""));

    CEdit* peditCurWorkFreq = (CEdit*)GetDlgItem(IDC_EDIT_CUR_WORK_FREQ);
    peditCurWorkFreq->SetWindowText(TEXT(""));

    CEdit* peditDevID = (CEdit*)GetDlgItem(IDC_EDIT_DEV_ID);
    peditDevID->SetWindowText(TEXT(""));

    CEdit* peditHardwareVer = (CEdit*)GetDlgItem(IDC_EDIT_HARDWARE_VER);
    peditHardwareVer->SetWindowText(TEXT(""));

    CEdit* peditProductDate = (CEdit*)GetDlgItem(IDC_EDIT_PRODUCT_DATE);
    peditProductDate->SetWindowText(TEXT(""));

    CEdit* peditSoftwareVer = (CEdit*)GetDlgItem(IDC_EDIT_SOFTWARE_VER);
    peditSoftwareVer->SetWindowText(TEXT(""));

    CEdit* peditProductType = (CEdit*)GetDlgItem(IDC_EDIT_PRODUCT_TYPE);
    peditProductType->SetWindowText(TEXT(""));
}

void CWirelessModuleCfgDlg::OnSelchangeListAlreadyLinkedComPort()
{
    // ��ȡ�û�ѡ���Ķ˿������ӵ��豸��Ϣ
    CListBox* pAlreadyLinkedComPortList =
                    (CListBox*)GetDlgItem(IDC_LIST_ALREADY_LINKED_COM_PORT);
    INT iCurSel = pAlreadyLinkedComPortList->GetCurSel();
    if (LB_ERR == iCurSel)
    {
        iCurSel = 0;
        pAlreadyLinkedComPortList->SetCurSel(iCurSel);
    }

    CString strCurSel = TEXT("");
    pAlreadyLinkedComPortList->GetText(iCurSel, strCurSel);

    // ��ȡ�û�ѡ��Ķ˿�����
    strCurSel = strCurSel.Trim();
    INT iIdxMiddleLine = strCurSel.Find(TEXT("-"));

    CString strPortName = strCurSel.Left(iIdxMiddleLine);
    strPortName = strPortName.Trim();

    INT iIdxBaudRateUnit = strCurSel.Find(TEXT("kbps"), iIdxMiddleLine + 1);
    CString strBaudRate = strCurSel.Mid(iIdxMiddleLine + 1,
                                        strCurSel.GetLength() - iIdxMiddleLine -
                                        lstrlen(TEXT("kbps") - 1));
    strBaudRate = strBaudRate.Trim();

    CComPortExplorer cpe;
    DWORD dwPort = cpe.GetPortNum(strPortName);

    DWORD dwBaudRate = 0;
    _stscanf_s(strBaudRate.GetBuffer(), TEXT("%d"), &dwBaudRate);

    // ��ȡ���ӵ��˿��ϵ��豸������Ϣ
    CCommunication* pCommunicaton = new CCommunication(dwPort, dwBaudRate);
    CWirelessModuleProtocol wmp;
    CCfgInfo CfgInfo;
    if (!wmp.SendSearchCmd(pCommunicaton, CfgInfo))
    {
        CString strError = TEXT("��ȡ���ӵ��˿ڣ�") + strPortName +
                           TEXT(" �ϵ��豸������Ϣʧ�ܣ�");
        SetStatusInfo(strError);
        LOG_ERROR("Read the information of device that connected to the com port, failed !");
        return;
    }

    CfgInfo.m_strComPortName = strPortName;
    UpdateToGui(CfgInfo);
}

void CWirelessModuleCfgDlg::OnBnClickedSaveCurDevice()
{
    if (!CheckCfgParam())
    {
        return;
    }

    CListBox* pAlreadyLinkedComPortList =
                    (CListBox*)GetDlgItem(IDC_LIST_ALREADY_LINKED_COM_PORT);
    if (pAlreadyLinkedComPortList->GetCount() == 0)
    {
        SetStatusInfo(TEXT("����ִ��ɨ���豸�����������豸��Ϣ��"));
        EnableCtrl(IDC_SAVE_CUR_DEVICE, FALSE);
        return;
    }

    SetStatusInfo(TEXT("���ڱ����������ݵ��豸 ... "));

    EnableCtrl(IDC_LIST_ALREADY_LINKED_COM_PORT, FALSE);
    EnableCtrl(IDC_BTN_SCAN_DEVICE, FALSE);
    EnableCtrl(IDC_SAVE_CUR_DEVICE, FALSE);
    CButton* pCurBtn = (CButton*)GetDlgItem(IDC_SAVE_CUR_DEVICE);
    pCurBtn->SetWindowText(TEXT("���ڱ��浱ǰ������Ϣ���豸..."));

    // ��������
    CString strComPortName = GetCurCommPortName();
    // ���ں�
    DWORD dwComPortNum = GetCurComPortNum();

    CString strLog = TEXT("");

    // ��ǰͨ���ŵ�
    CComboBox* pcbxComChannelNum = (CComboBox*)GetDlgItem(IDC_CBX_COMM_CHANNEL_NUM);
    CString strCommChannelNum = TEXT("");
    pcbxComChannelNum->GetWindowText(strCommChannelNum);
    DWORD dwCommChannelNum = 0;
    _stscanf_s(strCommChannelNum.GetBuffer(), TEXT("%d"), &dwCommChannelNum);

    // ���þ��еĴ��ڲ�����ͨ��
    DWORD dwPreBaudRate = CProtocolConst::INVALID_BAUDRATE_VALUE;
    if (!GetPreBaudRate(strComPortName, dwPreBaudRate))
    {
        strLog.Format(
                        TEXT("���浱ǰ���õ��豸ʧ�ܣ�������ɨ���豸����ִ�б������ò���!"));
        LOG_ERROR("Read the com port: " << strComPortName.GetBuffer() <<
                  " , the previous baudrate, failed !");
        SetStatusInfo(strLog);

        EnableCtrl(IDC_LIST_ALREADY_LINKED_COM_PORT);
        EnableCtrl(IDC_BTN_SCAN_DEVICE);
        EnableCtrl(IDC_SAVE_CUR_DEVICE);
        pCurBtn->SetWindowText(TEXT("���浱ǰ���õ��豸"));
        return;
    }

    CCommunication* pCommCommChannelNum = new CCommunication(dwComPortNum, dwPreBaudRate);
    CWirelessModuleProtocol wmp;
    if (!wmp.SendCfgChannelNumCmd(pCommCommChannelNum, (BYTE)dwCommChannelNum))
    {
        strLog.Format(
                        TEXT("ͨ�����ڣ�COM%d, �����ʣ�%d������ͨ���ŵ�Ϊ��%d ʱ������ʧ�ܣ�������ɨ���豸����ִ�б������ò���!"),
                        dwComPortNum, dwPreBaudRate, dwCommChannelNum);
        LOG_ERROR("By the com port: " << strComPortName.GetBuffer() << ", baudrate :" << dwPreBaudRate
                  << ", set the comm channel number to " << dwCommChannelNum << ", failed !");
        SetStatusInfo(strLog);

        EnableCtrl(IDC_LIST_ALREADY_LINKED_COM_PORT);
        EnableCtrl(IDC_BTN_SCAN_DEVICE);
        EnableCtrl(IDC_SAVE_CUR_DEVICE);
        pCurBtn->SetWindowText(TEXT("���浱ǰ���õ��豸"));

        return;
    }

    // ��ǰ��������
    CComboBox* pcbxCurTransSpeed = (CComboBox*)GetDlgItem(IDC_CBX_CUR_TRANS_SPEED);
    CString strCurTransSpeed = TEXT("");
    pcbxCurTransSpeed->GetWindowText(strCurTransSpeed);

    FLOAT fCurTransSpeed = 0.0;
    _stscanf_s(strCurTransSpeed.GetBuffer(), TEXT("%f"), &fCurTransSpeed);

    // �����ϵĴ������ʵ�λ��kbps������1000��ת�����ڲ�ʹ�õ�bps��λ
    DWORD dwCurTransSpeed = (DWORD)(fCurTransSpeed * 1000.0);
    CCommunication* pCommTransSpeed = new CCommunication(dwComPortNum, dwPreBaudRate);
    if (!wmp.SendCfgTransSpeedCmd(pCommTransSpeed, dwCurTransSpeed))
    {
        strLog.Format(
                        TEXT("ͨ�����ڣ�COM%d, �����ʣ�%d�����ô�������Ϊ��%0.1f kbpsʱ������ʧ�ܣ�������ɨ���豸����ִ�б������ò���!"),
                        dwComPortNum, dwPreBaudRate, fCurTransSpeed);
        LOG_ERROR("By the com port : " << strComPortName << " , baudrate : " << dwPreBaudRate <<
                  ", set the trans's speed to " << dwCurTransSpeed << ", failed !");
        SetStatusInfo(strLog);

        EnableCtrl(IDC_LIST_ALREADY_LINKED_COM_PORT);
        EnableCtrl(IDC_BTN_SCAN_DEVICE);
        EnableCtrl(IDC_SAVE_CUR_DEVICE);
        pCurBtn->SetWindowText(TEXT("���浱ǰ���õ��豸"));

        return;
    }

    // ����ģ������ID��
    CCommunication* pCommCfgMdlNetID = new CCommunication(dwComPortNum, dwPreBaudRate);
    if (!wmp.SendCfgMdlNetID(pCommCfgMdlNetID, (USHORT)GetMdlNetID()))
    {
        strLog.Format(L"����ģ������ID��%04X��ʧ��!", (USHORT)GetMdlNetID());
        SetStatusInfo(strLog);
        EnableCtrl(IDC_LIST_ALREADY_LINKED_COM_PORT);
        EnableCtrl(IDC_BTN_SCAN_DEVICE);
        EnableCtrl(IDC_SAVE_CUR_DEVICE);
        pCurBtn->SetWindowText(TEXT("���浱ǰ���õ��豸"));
        return;
    }

    // ���õ�ǰ������
    DWORD dwCurBaudRate = GetCurBaudRate();
    if (!CfgCurBaudRate(dwCurBaudRate))
    {
        LOG_ERROR("Cfg the current baudrate : " << dwCurBaudRate << ", failed !");
        strLog.Format(
                        TEXT("���ô��ڣ�%S�� ������Ϊ�� %d bps ʱ��ʧ�ܣ�������ɨ���豸����ִ�б������ò���!"),
                        strComPortName, dwCurBaudRate);
        SetStatusInfo(strLog);

        EnableCtrl(IDC_LIST_ALREADY_LINKED_COM_PORT);
        EnableCtrl(IDC_BTN_SCAN_DEVICE);
        EnableCtrl(IDC_SAVE_CUR_DEVICE);
        pCurBtn->SetWindowText(TEXT("���浱ǰ���õ��豸"));

        return;
    }

    EnableCtrl(IDC_LIST_ALREADY_LINKED_COM_PORT);
    EnableCtrl(IDC_BTN_SCAN_DEVICE);
    EnableCtrl(IDC_SAVE_CUR_DEVICE);
    pCurBtn->SetWindowText(TEXT("��������ǰ�豸"));

    SetStatusInfo(TEXT("��������������ɣ�"));
}

DWORD CWirelessModuleCfgDlg::GetMdlNetID()
{
    CEdit* pEditMdlNetID = (CEdit*)GetDlgItem(IDC_EDIT_MDL_NET_ID);
    CString strMdlNetID = L"";
    pEditMdlNetID->GetWindowText(strMdlNetID);

    UINT uMdlNetID = 0;
    _stscanf_s(strMdlNetID, TEXT("%d"), &uMdlNetID);

    return uMdlNetID;
}

BOOL CWirelessModuleCfgDlg::CheckCfgParam(void)
{

    if (GetMdlNetID() > CProtocolConst::MAX_MDL_NET_ID)
    {
        CString strErr = L"";
        strErr.Format(L"���õ�ģ������ID�ţ�%d����������������ֵ%d",
                      GetMdlNetID(), CProtocolConst::MAX_MDL_NET_ID);
        AfxMessageBox(strErr);
        return FALSE;
    }

    return TRUE;
}

void CWirelessModuleCfgDlg::SetStatusInfo(CString strStatusInfo)
{
    CEdit* pStatusBar = (CEdit*)GetDlgItem(IDC_EDIT_STATUS);
    pStatusBar->SetWindowText(strStatusInfo.GetBuffer());
}

CString CWirelessModuleCfgDlg::GetCurCommPortName()
{
    CEdit* pComPortName = (CEdit*)GetDlgItem(IDC_EDIT_COM_PORT_NAME);
    CString strComPortName = TEXT("");
    pComPortName->GetWindowText(strComPortName);

    LOG_INFO(TEXT("current com port name : ") << (PCTSTR)strComPortName);
    return strComPortName;
}

DWORD CWirelessModuleCfgDlg::GetCurComPortNum()
{
    CString strComPortName = GetCurCommPortName();
    CString strComPortNum = strComPortName.Mid(3);
    DWORD dwComPortNum = 0;
    _stscanf_s(strComPortNum.GetBuffer(), TEXT("%d"), &dwComPortNum);

    LOG_INFO(TEXT("current com port num : ") << dwComPortNum);
    return dwComPortNum;
}

UCHAR* CWirelessModuleCfgDlg::getUpgradeHexCode()
{
    return m_pUpdateHexCode.get();
}

ULONG CWirelessModuleCfgDlg::getUpgradeFileSize()
{
    return m_uUpgradeFileSize;
}

DWORD CWirelessModuleCfgDlg::GetCurBaudRate()
{
    CComboBox* pcbxBaudRate = (CComboBox*)GetDlgItem(IDC_CBX_BAUDRATE);
    CString strBaudRate = TEXT("");
    pcbxBaudRate->GetWindowText(strBaudRate);
    DWORD dwCurBaudRate = 0;
    _stscanf_s(strBaudRate.GetBuffer(), TEXT("%d"), &dwCurBaudRate);
    return dwCurBaudRate;
}

BOOL CWirelessModuleCfgDlg::CfgCurBaudRate(const DWORD dwCurBaudRate)
{
    CString strLog = TEXT("");

    CString strComPortName = GetCurCommPortName();

    // ������һ��ͨ��ʹ�õĲ�����
    DWORD dwPreBaudRate = 0;
    if (!GetPreBaudRate(strComPortName, dwPreBaudRate))
    {
        LOG_ERROR("Call the function : GetPreBaudRate(), find the prevous baudrate of com port: " <<
                  strComPortName.GetBuffer() << ", failed !");
        return FALSE;
    }

    // ������ڲ����ʷ����仯���������ô��ڲ�����
    if (dwCurBaudRate != dwPreBaudRate)
    {
        CCommunication* pCommCfgBaudRate =
                        new CCommunication(GetCurComPortNum(), dwPreBaudRate);
        CWirelessModuleProtocol wmp;
        if (!wmp.SendCfgBaudRateCmd(pCommCfgBaudRate, dwCurBaudRate))
        {
            strLog.Format(TEXT("���ô��ڣ�%s �Ĳ�����Ϊ��%d bps ʱ��ʧ�ܣ�"), strComPortName,
                          dwCurBaudRate);
            LOG_ERROR("Set the baudrate of the com port : " << strComPortName.GetBuffer() << " to " <<
                      dwCurBaudRate << " bps, failed !");
            SetStatusInfo(strLog);
            return FALSE;
        }
    }

    // ���óɹ��󣬸��ı���ĵ�ǰ�˿ڵĲ�����Ϊ��������ֵ
    m_mapAlreadyLinkedDevCom[strComPortName] = dwCurBaudRate;

    // �����������豸�˿ں��б������
    UpdateAlreadyPortList(strComPortName);

    return TRUE;
}

// ������һ��ͨ��ʹ�õĲ�����
BOOL CWirelessModuleCfgDlg::GetPreBaudRate(CString strComPortName, DWORD& dwPreBaudRate)
{
    CString strLog = TEXT("");
    if (!m_mapAlreadyLinkedDevCom.Lookup(strComPortName, dwPreBaudRate))
    {
        LOG_ERROR("Find the prevous baudrate of com port: " <<
                  strComPortName.GetBuffer() << ", failed !");
        return FALSE;
    }

    return TRUE;
}

void CWirelessModuleCfgDlg::UpdateAlreadyPortList(CString& strCurSelComPortName)
{
    CListBox* pAlreadyFindPortList =
                    (CListBox*)GetDlgItem(IDC_LIST_ALREADY_LINKED_COM_PORT);
    pAlreadyFindPortList->ResetContent();

    // ����ÿ���豸�˿ڣ���ӵ�����List�б����
    POSITION Pos = m_mapAlreadyLinkedDevCom.GetStartPosition();
    while (NULL != Pos)
    {
        CString strComPort = TEXT("");
        DWORD dwCurBaudRate = 0;
        m_mapAlreadyLinkedDevCom.GetNextAssoc(Pos, strComPort, dwCurBaudRate);

        CString strCurBaudRate = TEXT("");
        strCurBaudRate.Format(TEXT("%d bps"), dwCurBaudRate);
        CString strListItem = strComPort + TEXT(" - ") + strCurBaudRate;

        CListBox* pAlreadyLinkedComPortList =
                        (CListBox*)GetDlgItem(IDC_LIST_ALREADY_LINKED_COM_PORT);
        pAlreadyLinkedComPortList->AddString(strListItem.GetBuffer());
    }

    // ѡ����ǰѡ���Ķ˿�
    for (INT iIdxItem = 0; iIdxItem < pAlreadyFindPortList->GetCount(); ++iIdxItem)
    {
        CString strItemText = TEXT("");
        pAlreadyFindPortList->GetText(iIdxItem, strItemText);
        if (strItemText.Find(strCurSelComPortName) != -1)
        {
            pAlreadyFindPortList->SetCurSel(iIdxItem);
        }
    }
}

LRESULT CWirelessModuleCfgDlg::OnSetProgressRange(WPARAM wParam, LPARAM lParam)
{
    m_iMaxProgressVal = (INT)wParam;
    m_iMinProgressVal = 1;
    m_ctrlProgBar.SetRange32(m_iMinProgressVal, m_iMaxProgressVal + 1);
    m_ctrlProgBar.SetStep(1);
    m_ctrlProgBar.SetPos(m_iMinProgressVal);

    return 0;
}

LRESULT CWirelessModuleCfgDlg::OnIncProgressBarProgress(WPARAM wParam, LPARAM lParam)
{
    m_ctrlProgBar.StepIt();
    return 0;
}

LRESULT CWirelessModuleCfgDlg::OnFirmwareUpgradeTips(WPARAM wParam, LPARAM lParam)
{
    CString strTips((PWCHAR)wParam);
    SetStatusInfo(strTips);

    return 0;
}

LRESULT CWirelessModuleCfgDlg::OnFwUpgradeResetProgBar(WPARAM wParam, LPARAM lParam)
{
    //    InitProgBar();
    HideProgressBar(FALSE);

    return 0;
}

void CWirelessModuleCfgDlg::EnableCtrl(const DWORD dwCrlID, const BOOL bEnable)
{
    CWnd* pWndCtrl = (CWnd*)GetDlgItem(dwCrlID);
    pWndCtrl->EnableWindow(bEnable);
}

BOOL CWirelessModuleCfgDlg::ImageFromIDResource(UINT nID, LPCTSTR sTR, Image*& pImg)
{
    HINSTANCE hInst = AfxGetResourceHandle();
    HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(nID), sTR); // type
    if (!hRsrc)
    {
        return FALSE;
    }
    // load resource into memory
    DWORD len = SizeofResource(hInst, hRsrc);
    BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
    if (!lpRsrc)
    {
        return FALSE;
    }
    // Allocate global memory on which to create stream
    HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
    BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
    memcpy(pmem, lpRsrc, len);
    GlobalUnlock(m_hMem);
    IStream* pstm;
    CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);
    // load from stream
    pImg = Gdiplus::Image::FromStream(pstm);
    // free/release stuff
    pstm->Release();
    FreeResource(lpRsrc);
    GlobalFree(m_hMem);
    return TRUE;
}

CString CWirelessModuleCfgDlg::GetCurProgDir()
{
    CString strPath(TEXT(""));
    if (0 == GetModuleFileName(NULL,
                               strPath.GetBufferSetLength(MAX_PATH + 1),
                               MAX_PATH))
    {
        LOG_ERROR("Read the exe program dir, failed !");
        return CString(TEXT(""));
    }

    strPath.ReleaseBuffer();
    int nPos = strPath.ReverseFind('\\');
    strPath = strPath.Left(nPos);
    return strPath;
}

void CWirelessModuleCfgDlg::OnBnClickedBtnAbout()
{
    //     CAboutDlg dlgAbout;
    //
    //     dlgAbout.DoModal();
}

UINT UpgradeFirmwareFunc(LPVOID pParam)
{
    CWirelessModuleCfgDlg* pWmcDlg = (CWirelessModuleCfgDlg*)pParam;

    while (TRUE)
    {
        CWirelessModuleProtocol wmp;
        STATUS_CODE statusCode = wmp.UpdateFirmware(pWmcDlg->GetCurComPortNum(),
                                 pWmcDlg->getUpgradeHexCode(),
                                 pWmcDlg->getUpgradeFileSize(),
                                 pWmcDlg->m_bStopUpgradeFirmware,
                                 pWmcDlg->m_hWnd);

        if (pWmcDlg->m_bStopUpgradeFirmware)
        {
            pWmcDlg->SetUpradeEndCtrlStatus();
            pWmcDlg->HideProgressBar();
            ::PostMessage(pWmcDlg->m_hWnd, WM_FIRMWARE_UPGRADE_TIP,
                          (WPARAM)L"�Ѿ�ֹͣ������", 0);
            return 0;
        }

        pWmcDlg->SetStatusInfo(pWmcDlg->getErrorCodeDesc(statusCode));

        if (STATUS_SUCCESS != statusCode)
        {
            pWmcDlg->SetUpradeEndCtrlStatus();

            // �������ɹ����˳��������̣��ȴ��û���������������һ�ε���������
            return 0;
        }

        pWmcDlg->HideProgressBar();
    }

    return 0;
}

void CWirelessModuleCfgDlg::OnBnClickedBtnUpdateFirmware()
{
    LOG_TRACE_METHORD(TEXT("CWirelessModuleCfgDlg::OnBnClickedBtnUpdateFirmware func."));

    CComboBox* pComPort = (CComboBox*)GetDlgItem(IDC_CBX_COM_SEL);
    pComPort->SetCurSel(pComPort->GetCurSel());
    CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COM_PORT_NAME);
    CString strCom;
    pComPort->GetWindowText(strCom);
    pEdit->SetWindowText(strCom);

    SetUpgradingCtrlStatus();
    m_bStopUpgradeFirmware = FALSE;
    m_pUpgradeFWThread = AfxBeginThread(UpgradeFirmwareFunc, this);
    InitProgBar();
    HideProgressBar(FALSE);
}

CString CWirelessModuleCfgDlg::getErrorCodeDesc(const USHORT& uErrorCode) const
{
    CString strStatusCodeDesc = TEXT("");
    switch (uErrorCode)
    {
        case STATUS_SUCCESS:
            strStatusCodeDesc = TEXT("�豸�����ɹ���");
            break;

        case STATUS_FAILED_SEND_FIRMWARE_CODE:
            strStatusCodeDesc = TEXT("���ͳ�����뵽�豸ʱʱ����");
            break;

        case STATUS_FAILED_TO_CONNECT_DEVICE:
            strStatusCodeDesc = TEXT("�����豸����");
            break;
        case STATUS_UPGRAD_FILE_TOO_BIG:
            strStatusCodeDesc =
                            TEXT("�����ļ�̫���޷���������ȷ���ļ������ļ��Ƿ���ȷ��");
            break;

        case STATUS_FAILED_SEND_UPGRAD_END_CMD:
            strStatusCodeDesc = TEXT("����������������ʧ�ܣ�");
            break;

        case STATUS_USER_FORCE_STOP_UPGRADE:
            strStatusCodeDesc = L"�Ѿ�ֹͣ�����豸��";
            break;

        default:
            strStatusCodeDesc = TEXT("�����豸����ʧ�ܣ�");
    }

    return strStatusCodeDesc;
}

CString CWirelessModuleCfgDlg::getFirewareFile(void)
{
    LOG_TRACE_METHORD(TEXT("CWirelessModuleCfgDlg::getFirewareFile func."));

    TCHAR szFilter[] = _T("Bin Files (*.bin)|*.bin|")
                       _T("Hex Files (*.hex)|*.hex|")
                       _T("All Files (*.*)|*.*||");

    CFileDialog fileDialog(TRUE,
                           NULL,
                           NULL,
                           OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                           szFilter);
    if (IDCANCEL == fileDialog.DoModal())
    {
        return CString();
    }

    return fileDialog.GetPathName();
}

BOOL CWirelessModuleCfgDlg::readFirmwareCode(CString& strFileName,
        auto_ptr<UCHAR>& pszUpdateHexCode,
        ULONG& uFileSize)
{
    CFile hexFile;
    if (FALSE == hexFile.Open(strFileName, CFile::modeRead | CFile::typeBinary))
    {
        LOG_ERROR(TEXT("failed to open the updating firmware file : ") <<
                  (LPCWSTR)strFileName);
        return FALSE;
    }

    ULONGLONG uHexFileSize = hexFile.GetLength();
    if (0 == uHexFileSize)
    {
        LOG_ERROR("firmware file length is 0.");
        CString strError = L"ѡ��������ļ�(\"" + strFileName +
                           L"\")����Ϊ�㣬��ѡ����ȷ���ļ�������";
        AfxMessageBox(strError);
        return FALSE;
    }

    hexFile.SeekToBegin();
    pszUpdateHexCode = auto_ptr<UCHAR>(new UCHAR[(UINT)uHexFileSize]);
    memset(pszUpdateHexCode.get(), 0, (UINT)uHexFileSize);
    uFileSize = hexFile.Read(pszUpdateHexCode.get(), (UINT)uHexFileSize);
    hexFile.Close();

    return TRUE;
}

void CWirelessModuleCfgDlg::InitUpgradeCtrlStatus()
{
    // ���ļ�ѡ��ť����������ť����ֹͣ��ť��
    EnableCtrl(IDC_BTN_SEL_UPGRADE_FILE, TRUE);
    EnableCtrl(IDC_BTN_UPDATE_FIRMWARE, FALSE);
    EnableCtrl(IDC_BTN_STOP_UPGRADE, FALSE);
}

void CWirelessModuleCfgDlg::OpenUpgradeCtrlStatus()
{
    EnableCtrl(IDC_BTN_SEL_UPGRADE_FILE, TRUE);
    EnableCtrl(IDC_BTN_UPDATE_FIRMWARE, TRUE);
    EnableCtrl(IDC_BTN_STOP_UPGRADE, FALSE);
}

void CWirelessModuleCfgDlg::SetUpgradingCtrlStatus()
{
    InitCfgCtrlStatus();
    EnableCtrl(IDC_BTN_SCAN_DEVICE, FALSE);

    // ���ļ�ѡ��ť����������ť����ֹͣ��ť
    EnableCtrl(IDC_BTN_SEL_UPGRADE_FILE, FALSE);
    EnableCtrl(IDC_BTN_UPDATE_FIRMWARE, FALSE);
    EnableCtrl(IDC_BTN_STOP_UPGRADE, TRUE);
}

void CWirelessModuleCfgDlg::SetUpradeEndCtrlStatus()
{
    EnableCtrl(IDC_BTN_SEL_UPGRADE_FILE, TRUE);
    EnableCtrl(IDC_BTN_UPDATE_FIRMWARE, TRUE);
    EnableCtrl(IDC_BTN_STOP_UPGRADE, FALSE);

    InitCfgCtrlStatus();
}

void CWirelessModuleCfgDlg::OnBnClickedBtnSelUpgradeFile()
{
    LOG_TRACE_METHORD(L"CWirelessModuleCfgDlg::OnBnClickedBtnSelUpgradeFile");

    CString strFileName = getFirewareFile();
    if (strFileName.IsEmpty())
    {
        return;
    }

    if (!readFirmwareCode(strFileName, m_pUpdateHexCode, m_uUpgradeFileSize))
    {
        LOG_ERROR(L"failed to read upgrade firmware file.");
        InitUpgradeCtrlStatus();
        return;
    }

    OpenUpgradeCtrlStatus();
}


void CWirelessModuleCfgDlg::OnBnClickedBtnStopUpgrade()
{
    m_bStopUpgradeFirmware = TRUE;
    HideProgressBar(TRUE);
    Sleep(1000);
}


void CWirelessModuleCfgDlg::OnEnChangeEditMdlNetId()
{
    CEdit* pMdlNetID = (CEdit*)GetDlgItem(IDC_EDIT_MDL_NET_ID);
    CString strMdlNetID = L"";
    pMdlNetID->GetWindowText(strMdlNetID);
    DWORD dwMdlNetID = 0;
    _stscanf_s(strMdlNetID, TEXT("%d"), &dwMdlNetID);

    if (dwMdlNetID > 65535)
    {
        CString strError;
        strError.Format(L"������豸����ID�ţ�%d�����������ֵ65535", dwMdlNetID);
        AfxMessageBox(strError);
    }
}
