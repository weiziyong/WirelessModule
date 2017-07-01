//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : CComPortExplorer.cpp
//  @ Date : 2014/11/7 ������
//  @ Author :
//
//

#include "stdafx.h"
#include "ComPortExplorer.h"
#include "../LogSystem.h"
#include "ProtocolConst.h"
#include "WirelessModuleProtocol.h"
#include "Communication.h"



CComPortExplorer::CComPortExplorer(const HWND hWnd)
{
    m_aAllSysCom.RemoveAll();
    m_hWnd = hWnd;
}

CComPortExplorer::~CComPortExplorer()
{

}

BOOL CComPortExplorer::GetCurLinkedDevCommPort(CMap<CString, LPCTSTR, DWORD, DWORD>&
        mapAllLinkedDeviceCom)
{
    LOG_TRACE_METHORD(TEXT("CComPortExplorer::GetCurLinkedDevCommPort func."));

    // ��ȡ��λ����ǰ�����д�������
    if (!GetSystemAllComPort())
    {
        LOG_ERROR("No com port in upper machine.");

        return FALSE;
    }

    if (!GetAllAvailableComPort(mapAllLinkedDeviceCom))
    {
        LOG_ERROR("No available com port in upper machine.");
        return FALSE;
    }

    return TRUE;
}

BOOL CComPortExplorer::GetSystemAllComPort()
{
    LOG_TRACE_METHORD(TEXT("CComPortExplorer::GetSystemAllComPort func."));

    HKEY hKey = NULL;

    m_aAllSysCom.RemoveAll();

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                     TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"),
                     0,
                     KEY_READ,
                     &hKey) != ERROR_SUCCESS)
    {
        return FALSE;
    }

    CString strValueName(TEXT(""));
    CString strDataBuffer(TEXT(""));
    DWORD dwValueNameBufferLength = 200;
    DWORD dwValueType = 0;
    DWORD dwDataBufferSize = 200;

    DWORD dwIdx = 0;
    while (RegEnumValue(hKey,
                        dwIdx++,
                        strValueName.GetBuffer(200),
                        &dwValueNameBufferLength,
                        NULL,
                        &dwValueType,
                        (LPBYTE)strDataBuffer.GetBuffer(200),
                        &dwDataBufferSize) != ERROR_NO_MORE_ITEMS)
    {

        m_aAllSysCom.Add(strDataBuffer.GetBuffer());
        LOG_DEBUG("Find the com port of upper machine : " << strDataBuffer);

        dwDataBufferSize = 200;
        dwValueNameBufferLength = 200;

        strDataBuffer.ReleaseBuffer();
    }

    RegCloseKey(hKey);

    CString strLog = TEXT("Find the all com port : ");
    for (INT iIdx = 0; iIdx < m_aAllSysCom.GetSize(); iIdx++)
    {
        if (iIdx > 0)
        {
            strLog += TEXT(",");
        }

        strLog = strLog + m_aAllSysCom.GetAt(iIdx);
    }

    LOG_INFO((LPCTSTR)strLog);

    return TRUE;
}

BOOL CComPortExplorer::GetAllAvailableComPort(CMap<CString, LPCTSTR, DWORD, DWORD>&
        mapAllLinkedDeviceCom)
{
    LOG_TRACE_METHORD(TEXT("CComPortExplorer::GetAllAvailableComPort func."));

    mapAllLinkedDeviceCom.RemoveAll();

    DWORD dwProgressMaxRangeValue = m_aAllSysCom.GetSize() * CProtocolConst::MAX_BAUDRATE_IDX;
    ::PostMessage(m_hWnd, WM_PROGRESS_MAX_VALUE, (WPARAM)dwProgressMaxRangeValue, 0);

    // ����ÿ���˿��ܹ�ʹ�õ����в����ʣ��������ò���������鿴�豸��ǰʹ�õĶ˿ںźͲ�����
    for (INT iPortIdx = 0; iPortIdx < m_aAllSysCom.GetSize(); ++iPortIdx)
    {
        LOG_INFO(TEXT("Traverse the com port : ") << m_aAllSysCom[iPortIdx].GetBuffer() <<
                 " , begin !!!!!!!!!!!!!!");

        for (DWORD dwIdxBaudRate = 0;
                dwIdxBaudRate < CProtocolConst::MAX_BAUDRATE_IDX; ++dwIdxBaudRate)
        {
            ::PostMessage(m_hWnd, WM_PROGRESS_INCREMENT, 0, 0);

            DWORD dwBaudRate = CProtocolConst::m_aBaudRateMapTable[dwIdxBaudRate];

            LOG_DEBUG("Traverse the com port : " <<
                      m_aAllSysCom[iPortIdx].GetBuffer() << ", by " << dwBaudRate
                      << " bps");

            DWORD dwPortNum = GetPortNum(m_aAllSysCom[iPortIdx]);
            if ((CProtocolConst::INVALID_COM_PORT_NUM == dwPortNum))
            {
                LOG_FATAL("The com port num that extracted from com port name : " <<
                          m_aAllSysCom[iPortIdx].GetBuffer() << "is invalid !");
                LOG_INFO(TEXT("Traverse the com port : ") << m_aAllSysCom[iPortIdx].GetBuffer() <<
                         " , excepntion end !!!!!!!!!!!!!!");


                return FALSE;
            }

            LOG_DEBUG("From the com port : " << m_aAllSysCom[iPortIdx].GetBuffer() <<
                      ", extracted the com port num : " << dwPortNum);

            CCommunication* pCommunication = new CCommunication(dwPortNum, dwBaudRate, 400);
            CWirelessModuleProtocol WirelessMdouleProtocol;
            if (WirelessMdouleProtocol.SendCfgBaudRateCmd(pCommunication, dwBaudRate))
            {
                LOG_DEBUG("Find the com port : " << GetPortNum(m_aAllSysCom[iPortIdx]) << ", use the baudrate : " <<
                          dwBaudRate << " to connect the device !");

                // ���������豸�Ĵ���
                mapAllLinkedDeviceCom[m_aAllSysCom[iPortIdx]] = dwBaudRate;
            }
        }

        LOG_INFO(TEXT("Traverse the com port : ") <<
                 (LPCTSTR)m_aAllSysCom[iPortIdx] << TEXT(" , end !"));
    }

    CString strLog = TEXT("Finded the linked device com port : ");
    POSITION Pos = mapAllLinkedDeviceCom.GetStartPosition();
    CString strComPortName = TEXT("");
    DWORD dwComPortBaudRate = 0;
    DWORD dwIdx = 0;
    while (NULL != Pos)
    {
        mapAllLinkedDeviceCom.GetNextAssoc(Pos, strComPortName, dwComPortBaudRate);
        CString strComInfo = TEXT("");
        if (dwIdx > 0)
        {
            strComInfo = TEXT("��");
        }

        ++dwIdx;

        strComInfo.Format(TEXT("[%s - %d kbps]"), strComPortName.GetBuffer(), dwComPortBaudRate);
        strLog += strComInfo;

        LOG_INFO((PCTSTR)strLog);
    }

    return TRUE;
}

DWORD CComPortExplorer::GetPortNum(CString strComPortName)
{
    LOG_TRACE_METHORD(TEXT("CComPortExplorer::GetPortNum func."));

    INT nDelLength = strComPortName.Delete(0, 3);

    DWORD dwLength = strComPortName.GetLength();

    INT iComNum = CProtocolConst::INVALID_COM_PORT_NUM;
    _stscanf_s(strComPortName.GetBuffer(), TEXT("%d"), &iComNum);

    return iComNum;
}

BOOL CComPortExplorer::GetSysAllComPort(CStringArray& allSysCom)
{
    LOG_TRACE_METHORD(L"CComPortExplorer::GetSysAllComPort");

    if (!GetSystemAllComPort())
    {
        return FALSE;
    }

    allSysCom.Copy(m_aAllSysCom);

    return TRUE;
}
























