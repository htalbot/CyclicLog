
#include "test_CyclicLogAppLayer.h"
#include "CyclicLog/CyclicLog.h"

using namespace CyclicLog_ns;

#include <stdio.h>

#if defined _MSC_VER
#   ifdef _DEBUG
        const char TEST_CYCLICLOG_DebugFlag[] = "DEBUG";
#   else
        const char TEST_CYCLICLOG_DebugFlag[] = "RELEASE";
#   endif
#else
#   ifdef DEBUG
        const char TEST_CYCLICLOG_DebugFlag[] = "DEBUG";
#   else
        const char TEST_CYCLICLOG_DebugFlag[] = "RELEASE";
#   endif
#endif

Test_CyclicLogAppLayer::Test_CyclicLogAppLayer()
{
}


bool Test_CyclicLogAppLayer::run()
{
    printf("***** Test_CyclicLogAppLayer (%s) *****\n\n", TEST_CYCLICLOG_DebugFlag);

    int major, minor, patch;
    CyclicLog_get_version(major, minor, patch);
    printf("CyclicLog version: %d.%d.%d\n", major, minor, patch);

    #ifdef ACE_WIN32
    std::string file_name = "c:\\TestLog.txt";
    #else
    std::string file_name = "TestLog.txt";
    #endif
    //file_name = "\\\\10.0.2.15\\var\\test.log";
    long line_size = 80;
    long max_file_size = 5000;
    bool flush = false;

    std::string source = "ObjectA";
    long severity = LM_WARNING;
    long code = 99;
    std::string description = "Description from ObjectA";


    CyclicLog log;

    if (!log.Init(file_name, flush, line_size, max_file_size))
    {
        ACE_DEBUG((LM_DEBUG, "Failed to init.\n"));
        return false;
    }

    if (!log.Add(source, severity, code, description))
    {
        ACE_DEBUG((LM_DEBUG, "Failed to add.\n"));
        return false;
    }


    return true;
}

