
#include "stdafx.h"
#include "ProtocolConst.h"

// 传输速率定义，单位bps
static CONST DWORD TS_1200 = 1200;
static CONST DWORD TS_4800 = 4800;
static CONST DWORD TS_9600 = 9600;
static CONST DWORD TS_20K = 20000;
static CONST DWORD TS_40K = 40000;
static CONST DWORD TS_50K = 50000;
static CONST DWORD TS_100K = 100000;
static CONST DWORD TS_125K = 125000;
static CONST DWORD TS_200K = 200000;
static CONST DWORD TS_250K = 250000;
static CONST DWORD TS_400K = 400000;
static CONST DWORD TS_500K = 500000;
static CONST DWORD TS_1000K = 1000000;

CONST TCHAR CProtocolConst::szFreq315M[] = TEXT("315");
CONST TCHAR CProtocolConst::szFreq433M[] = TEXT("433");
CONST TCHAR CProtocolConst::szFreq779M[] = TEXT("779");
CONST TCHAR CProtocolConst::szFreq868M[] = TEXT("868");
CONST TCHAR CProtocolConst::szFreq915M[] = TEXT("915");

// 传输速率的单位为bps
const DWORD CProtocolConst::m_aTransSpeed[CProtocolConst::MAX_TRANS_SPEED_IDX + 1] = {TS_20K, TS_40K, TS_50K, TS_200K, TS_250K, TS_400K, TS_500K, TS_1000K, TS_1200, TS_4800, TS_9600};
// 315M、433M模块传输速率映射表，单位为bps
const DWORD CProtocolConst::m_aTransSpeed_433M[CProtocolConst::MAX_TRANS_SPEED_IDX_433M + 1] = {TS_1200, TS_4800, TS_9600, TS_20K, TS_40K, TS_100K, TS_125K };
// 779M、868M、915M模块传输速率映射表，单位为bps
const DWORD CProtocolConst::m_aTransSpeed_868M[CProtocolConst::MAX_TRANS_SPEED_IDX_868M + 1] = {TS_20K, TS_40K, TS_50K, TS_100K, TS_250K, TS_400K, TS_500K, TS_1000K };

// 波特率带宽为bps
const DWORD CProtocolConst::m_aBaudRateMapTable[CProtocolConst::MAX_BAUDRATE_IDX] = { CBR_1200, CBR_2400, CBR_4800, CBR_9600, CBR_19200, CBR_38400, CBR_57600, CBR_115200 };

CONST CHAR CProtocolConst::STR_HUNTER_KING[] = "HUNTER-KING";


