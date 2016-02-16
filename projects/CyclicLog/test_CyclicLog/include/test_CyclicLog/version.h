
#ifndef __TEST_CYCLICLOG_VERSION_H__
#define __TEST_CYCLICLOG_VERSION_H__

#define TEST_CYCLICLOG_MAJOR 1
#define TEST_CYCLICLOG_MINOR 0
#define TEST_CYCLICLOG_PATCH 2


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


#endif // __TEST_CYCLICLOG_VERSION_H__
