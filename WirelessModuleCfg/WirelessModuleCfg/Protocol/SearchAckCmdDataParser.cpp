//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : CSearchAckCmdDataParser.cpp
//  @ Date : 2014/10/29 ������
//  @ Author :
//
//

#include "stdafx.h"
#include "SearchAckCmdDataParser.h"
#include "ProtocolConst.h"
#include "../LogSystem.h"

CSearchAckCmdDataParser::CSearchAckCmdDataParser(PBYTE const pAckCmdData, const DWORD dwCmdDataLen)
{
    ASSERT(NULL != pAckCmdData);

    m_pAckCmdData = pAckCmdData;
    m_dwCmdDataLen = dwCmdDataLen;
}

CSearchAckCmdDataParser::~CSearchAckCmdDataParser()
{
}

BOOL CSearchAckCmdDataParser::IsValidBaudrateIdx(const BYTE btBaudRateIdx) const
{
    if ((btBaudRateIdx < CProtocolConst::MIN_BAUDRATE_IDX) ||
            (btBaudRateIdx > CProtocolConst::MAX_BAUDRATE_IDX))
    {
        LOG_ERROR("The index value of baudrate : " << btBaudRateIdx << " is not between [" <<
                  CProtocolConst::MIN_BAUDRATE_IDX << "-" <<
                  CProtocolConst::MAX_BAUDRATE_IDX << "]");
        return FALSE;
    }

    return TRUE;
}

DWORD CSearchAckCmdDataParser::GetCurBaudrate() const
{
    BYTE btCurBaudRateIdx = m_pAckCmdData[CProtocolConst::OFFSET_CUR_BAUDRATE_IDX];
    if (!IsValidBaudrateIdx(btCurBaudRateIdx))
    {
        LOG_ERROR("The index value of baudrate : " << btCurBaudRateIdx << " is invalid !");

        // ��ǰ����������ֵ��Чʱ��ֱ�ӷ����յ���ԭʼ����������ֵ
        return btCurBaudRateIdx;
    }

    LOG_DEBUG("The current baudrate's value:" << CProtocolConst::m_aBaudRateMapTable[btCurBaudRateIdx -
                               1]);

    return CProtocolConst::m_aBaudRateMapTable[btCurBaudRateIdx - 1];
}

DWORD CSearchAckCmdDataParser::GetMinBaudrate() const
{
    BYTE btBaudRateMinIdx = m_pAckCmdData[CProtocolConst::OFFSET_BAUDRATE_MIN_IDX];
    if (!IsValidBaudrateIdx(btBaudRateMinIdx))
    {
        LOG_DEBUG("The min index's value of baudrate : " << btBaudRateMinIdx << "is invalid !");

        // ��Чʱ��ֱ�ӷ����յ�����Чֵ
        return btBaudRateMinIdx;
    }

    LOG_DEBUG("The min value of baudrate : " << CProtocolConst::m_aBaudRateMapTable[btBaudRateMinIdx -
                               1]);

    return CProtocolConst::m_aBaudRateMapTable[btBaudRateMinIdx - 1];
}

DWORD CSearchAckCmdDataParser::GetMaxBaudrate() const
{
    BYTE btBaudRateMAXIdx = m_pAckCmdData[CProtocolConst::OFFSET_BAUDRATE_MAX_IDX];
    if (!IsValidBaudrateIdx(btBaudRateMAXIdx))
    {
        LOG_DEBUG("The max value of baudrate : " << btBaudRateMAXIdx << " is invalid !");

        // ֱ�ӷ����յ�����Ч�����Ĳ���������
        return btBaudRateMAXIdx;
    }

    LOG_DEBUG("The max baudrate : " << CProtocolConst::m_aBaudRateMapTable[btBaudRateMAXIdx - 1] <<
              " bps");

    return CProtocolConst::m_aBaudRateMapTable[btBaudRateMAXIdx - 1];
}

BYTE CSearchAckCmdDataParser::GetCurCommChannelNum() const
{
    LOG_DEBUG("The Current communication's channel number: " <<
              m_pAckCmdData[CProtocolConst::OFFSET_CUR_COMM_CHANNEL_NUM]);
    if ((m_pAckCmdData[CProtocolConst::OFFSET_CUR_COMM_CHANNEL_NUM] <
            CProtocolConst::MIN_COMM_CHANNEL_NUM) ||
            (m_pAckCmdData[CProtocolConst::OFFSET_CUR_COMM_CHANNEL_NUM] > CProtocolConst::MAX_COMM_CHANNEL_NUM))
    {
        LOG_ERROR("The current commnicational channel number : " <<
                  m_pAckCmdData[CProtocolConst::OFFSET_CUR_COMM_CHANNEL_NUM] <<
                  " is not between [" << CProtocolConst::MIN_COMM_CHANNEL_NUM << " - " <<
                  CProtocolConst::MAX_COMM_CHANNEL_NUM << "]");
    }

    return m_pAckCmdData[CProtocolConst::OFFSET_CUR_COMM_CHANNEL_NUM];
}

BYTE CSearchAckCmdDataParser::GetMaxCommChannelNum() const
{
    LOG_DEBUG("The max communication's channel number : " <<
              m_pAckCmdData[CProtocolConst::OFFSET_MAX_COMM_CHANNEL_NUM]);

    if (m_pAckCmdData[CProtocolConst::OFFSET_MAX_COMM_CHANNEL_NUM] >
            CProtocolConst::MAX_COMM_CHANNEL_NUM)
    {
        LOG_ERROR("The current max communicational channel number : " <<
                  m_pAckCmdData[CProtocolConst::OFFSET_MAX_COMM_CHANNEL_NUM] << " is greater than " <<
                  " the max value : " << CProtocolConst::MAX_COMM_CHANNEL_NUM);
    }

    return m_pAckCmdData[CProtocolConst::OFFSET_MAX_COMM_CHANNEL_NUM];
}

DWORD CSearchAckCmdDataParser::GetCurTransSpeed() const
{
    BYTE btCurTransSpeedLowByte = m_pAckCmdData[CProtocolConst::OFFSET_CUR_TRANS_SPEED_LOW_BYTE];
    BYTE btCurTransSpeedHighByte = m_pAckCmdData[CProtocolConst::OFFSET_CUR_TRANS_SPEED_HIGH_BYTE];
    DWORD btCurTransSpeed = btCurTransSpeedHighByte;
    btCurTransSpeed = (btCurTransSpeed << 8) + btCurTransSpeedLowByte;

    LOG_DEBUG("The current trans's speed : " << btCurTransSpeed << " kbps");

    return btCurTransSpeed;
}

DWORD CSearchAckCmdDataParser::GetCurFrequency() const
{
    BYTE btCurFreqLowByte = m_pAckCmdData[CProtocolConst::OFFSET_CUR_FREQ_LOW_BYTE];
    BYTE btCurFreqHighByte = m_pAckCmdData[CProtocolConst::OFFSET_CUR_FREQ_HIGH_BYTE];
    DWORD dwCurFreq = btCurFreqHighByte;
    dwCurFreq = (dwCurFreq << 8) + btCurFreqLowByte;

    LOG_DEBUG("The current work's frequency : " << dwCurFreq << " MHZ");

    return dwCurFreq;
}

DWORD CSearchAckCmdDataParser::GetID() const
{
    DWORD dwIDByte_0 = m_pAckCmdData[CProtocolConst::OFFSET_CUR_ID_BYTE_0];
    DWORD dwIDByte_1 = m_pAckCmdData[CProtocolConst::OFFSET_CUR_ID_BYTE_1];
    DWORD dwIDByte_2 = m_pAckCmdData[CProtocolConst::OFFSET_CUR_ID_BYTE_2];
    DWORD dwIDByte_3 = m_pAckCmdData[CProtocolConst::OFFSET_CUR_ID_BYTE_3];

    DWORD dwID = (dwIDByte_3 << 24) + (dwIDByte_2 << 16) + (dwIDByte_1 << 8) + dwIDByte_0;

    LOG_DEBUG("The Device ID : " << dwID);

    return dwID;
}

PRODUCT_DATE CSearchAckCmdDataParser::GetProductDate() const
{
    PRODUCT_DATE ProductDate;
    ProductDate.btYear = m_pAckCmdData[CProtocolConst::OFFSET_DATE_YEAR];
    ProductDate.btMonth = m_pAckCmdData[CProtocolConst::OFFSET_DATE_MONTH];
    ProductDate.btDay = m_pAckCmdData[CProtocolConst::OFFSET_DATE_DAY];

    LOG_DEBUG("The Product date : " << ProductDate.btYear << "-" << ProductDate.btMonth << "-" <<
              ProductDate.btDay);

    return ProductDate;
}

PRODUCT_VER CSearchAckCmdDataParser::GetHardwareVer() const
{
    PRODUCT_VER HardwareVer;
    HardwareVer.btLowVer = m_pAckCmdData[CProtocolConst::OFFSET_HARDWARE_LOW_VER];
    HardwareVer.btHighVer = m_pAckCmdData[CProtocolConst::OFFSET_HARDWARE_HIGH_VER];

    LOG_DEBUG("The hardware version of Product : " << HardwareVer.btHighVer << "." <<
              HardwareVer.btLowVer);

    return HardwareVer;
}

PRODUCT_VER CSearchAckCmdDataParser::GetSoftwareVer() const
{
    PRODUCT_VER SoftwareVer;
    SoftwareVer.btLowVer = m_pAckCmdData[CProtocolConst::OFFSET_SOFTWARE_VER_LOW_VER];
    SoftwareVer.btHighVer = m_pAckCmdData[CProtocolConst::OFFSET_SOFTWARE_VER_HIGH_VER];

    LOG_DEBUG("The software version of product : " << SoftwareVer.btHighVer << "." <<
              SoftwareVer.btLowVer);

    return SoftwareVer;
}

USHORT CSearchAckCmdDataParser::GetProductType() const
{
    BYTE uProductTypeLowByte = m_pAckCmdData[CProtocolConst::OFFSET_PRODUCT_TYPE_LOW_BYTE];
    BYTE uProductTypeHighByte = m_pAckCmdData[CProtocolConst::OFFSET_PRODUCT_TYPE_HIGH_BYTE];

    BYTE uProductType = uProductTypeHighByte << 8;
    uProductType = uProductType + uProductTypeLowByte;

    LOG_DEBUG("The type of product : 0X" << hex << uProductType << ", decimal:" << uProductType);

    return uProductType;
}

USHORT CSearchAckCmdDataParser::GetMdlNetID() const
{
    BYTE bHighByte = m_pAckCmdData[CProtocolConst::OFFSET_MDL_NET_ID_HIGH_BYTE];
    BYTE bLowByte = m_pAckCmdData[CProtocolConst::OFFSET_MDL_NET_ID_LOW_BYTE];

    USHORT sMdlNetID = MERGE_BYTES_SHORT(bHighByte, bLowByte);

    LOG_INFO(L"module net id: bHighByte = 0X" << std::uppercase <<
             std::hex << bHighByte << L", bLowByte = 0X" << std::uppercase <<
             std::hex << bLowByte << L", sMdlNetID = 0X" << std::uppercase <<
             std::hex << sMdlNetID);

    return sMdlNetID;
}
