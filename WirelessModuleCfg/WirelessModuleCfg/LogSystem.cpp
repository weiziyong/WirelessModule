
#include "stdafx.h"
#include "LogSystem.h"
#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/ndc.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/loggingmacros.h>

// 日志输出配置常量
static const DWORD LOG_SIZE_1M_BYTES = 1024 * 1024;
static const DWORD LOG_MAX_FILE_SIZE = 1 * LOG_SIZE_1M_BYTES;
static const DWORD LOG_MAX_BACKUP_INDEX = 1; // 日志文件最大备份文件索引号
static const bool FLAG_IMMEDIATE_FLUSH_LOG_CACHE = true; // 立即清空日志缓存
static const bool FLAG_AUTO_CREATE_LOG_DIR =
                true; // 日志目录不存在时，自动创建日志目录

// 设置日志格式
// %D：显示本地时间
// %t:线程ID
// %Q：毫秒，如：768.877
// %p：日志级别;-5:字符对齐格式，从左往右对齐宽度5个字符
// %%:输出%号
// %x:NDC，堆栈信息
// %m:输出日志原始信息
// %l:输出当前日志记录器所在的文件名和行号
// %n:打印换行
#define LOG_RECORD_FORMATTOR LOG4CPLUS_TEXT("[%D{%Y-%m-%d %H:%M:%S,%Q} [%-6t] [%-5p] [%x] [%m] [%l]%n")

#define LOG_FILE_NAME LOG4CPLUS_TEXT ("log.log")

#define LOG_RECORDER_NAME (LOG4CPLUS_TEXT("filelogger"))

Logger g_Logger;

BOOL IsLogCfgFileExist(CString strLogCfgFile)
{
    if ((strLogCfgFile.IsEmpty()) || (FALSE == PathFileExists(strLogCfgFile.GetBuffer())))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL LOG_INIT(PTCHAR pszCfgFileName)
{
    // 当前目录下，有日志配置文件存在，则使用当前的日志配置文件；
    // 否则，程序自己创建日志配置信息
    if (IsLogCfgFileExist(pszCfgFileName))
    {
        log4cplus::initialize();
        PropertyConfigurator::doConfigure(pszCfgFileName);
        g_Logger = Logger::getInstance(LOG_RECORDER_NAME);
        return TRUE;
    }

    // 程序自动配置日志系统
    log4cplus::initialize();

    SharedAppenderPtr FileAppender(new RollingFileAppender(LOG_FILE_NAME,
                                   LOG_MAX_FILE_SIZE,
                                   LOG_MAX_BACKUP_INDEX,
                                   FLAG_IMMEDIATE_FLUSH_LOG_CACHE,
                                   FLAG_AUTO_CREATE_LOG_DIR));
    FileAppender->setName(LOG4CPLUS_TEXT("First"));
    log4cplus::tstring strPattern = LOG_RECORD_FORMATTOR;
    FileAppender->setLayout(
                    std::auto_ptr<Layout>(new PatternLayout(strPattern)));

    g_Logger = Logger::getInstance(LOG_RECORDER_NAME);
    g_Logger.addAppender(FileAppender);
    g_Logger.setLogLevel(INFO_LOG_LEVEL);

    LOG_INFO("program cfg the log4cplus sys by self," <<
             " do not use the log's cfg file !");

    return TRUE;
}






