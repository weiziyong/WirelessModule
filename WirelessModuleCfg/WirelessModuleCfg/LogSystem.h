
#if !defined(_LOG_SYSTEM_H)
#define _LOG_SYSTEM_H

#include "stdafx.h"
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/loggingmacros.h>
#include "Shlwapi.h"

#pragma comment(lib,"shlwapi.lib")

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

#define LOG_CFG_FILE_NAME TEXT("\\log4cplus.properties")

extern Logger g_Logger;

extern BOOL LOG_INIT(PTCHAR pszCfgFileName);

// 日志记录宏定义
#define _LOGGER_OBJ (g_Logger)
#define LOG_TRACE_METHORD(LOG_TEXT) LOG4CPLUS_TRACE_METHOD(_LOGGER_OBJ, LOG_TEXT)
#define LOG_TRACE(LOG_TEXT)         LOG4CPLUS_TRACE(_LOGGER_OBJ, LOG_TEXT)
#define LOG_DEBUG(LOG_TEXT)         LOG4CPLUS_DEBUG(_LOGGER_OBJ, LOG_TEXT)
#define LOG_INFO(LOG_TEXT)          LOG4CPLUS_INFO(_LOGGER_OBJ, LOG_TEXT)
#define LOG_WARN(LOG_TEXT)          LOG4CPLUS_WARN(_LOGGER_OBJ, LOG_TEXT)
#define LOG_ERROR(LOG_TEXT)         LOG4CPLUS_ERROR(_LOGGER_OBJ, LOG_TEXT)
#define LOG_FATAL(LOG_TEXT)         LOG4CPLUS_FATAL(_LOGGER_OBJ, LOG_TEXT)

#endif // _LOG_SYSTEM_H



















