
#pragma once

#define CYCLICLOG_MAJOR 1
#define CYCLICLOG_MINOR 0
#define CYCLICLOG_PATCH 2


#if defined _MSC_VER
#   ifdef _DEBUG
        const char CYCLICLOG_DebugFlag[] = "DEBUG";
#   else
        const char CYCLICLOG_DebugFlag[] = "RELEASE";
#   endif
#else
#   ifdef DEBUG
        const char CYCLICLOG_DebugFlag[] = "DEBUG";
#   else
        const char CYCLICLOG_DebugFlag[] = "RELEASE";
#   endif
#endif


