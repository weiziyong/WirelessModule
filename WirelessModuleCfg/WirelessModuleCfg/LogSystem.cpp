
#include "stdafx.h"
#include "LogSystem.h"
#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/ndc.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/loggingmacros.h>

// ��־������ó���
static const DWORD LOG_SIZE_1M_BYTES = 1024 * 1024;
static const DWORD LOG_MAX_FILE_SIZE = 1 * LOG_SIZE_1M_BYTES;
static const DWORD LOG_MAX_BACKUP_INDEX = 1; // ��־�ļ���󱸷��ļ�������
static const bool FLAG_IMMEDIATE_FLUSH_LOG_CACHE = true; // ���������־����
static const bool FLAG_AUTO_CREATE_LOG_DIR =
                true; // ��־Ŀ¼������ʱ���Զ�������־Ŀ¼

// ������־��ʽ
// %D����ʾ����ʱ��
// %t:�߳�ID
// %Q�����룬�磺768.877
// %p����־����;-5:�ַ������ʽ���������Ҷ�����5���ַ�
// %%:���%��
// %x:NDC����ջ��Ϣ
// %m:�����־ԭʼ��Ϣ
// %l:�����ǰ��־��¼�����ڵ��ļ������к�
// %n:��ӡ����
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
    // ��ǰĿ¼�£�����־�����ļ����ڣ���ʹ�õ�ǰ����־�����ļ���
    // ���򣬳����Լ�������־������Ϣ
    if (IsLogCfgFileExist(pszCfgFileName))
    {
        log4cplus::initialize();
        PropertyConfigurator::doConfigure(pszCfgFileName);
        g_Logger = Logger::getInstance(LOG_RECORDER_NAME);
        return TRUE;
    }

    // �����Զ�������־ϵͳ
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






