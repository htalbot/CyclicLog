
#ifndef __CYCLICLOG_H__
#define __CYCLICLOG_H__

#ifdef ACE_WIN32
#pragma warning(disable: 4251) // wittingly: DPO deals with this issue.
#endif

#include "BasicEventHandling/BasicEventHandling.h"

#include <fstream>
#include <string>

#include "CyclicLog/CYCLICLOG_Export.h"
#include "CyclicLog/version.h"

const int MaxLineLength = 1024;

namespace CyclicLog_ns
{
    extern "C" CYCLICLOG_Export void CyclicLog_get_version(int & major, int & minor, int & patch);

    class CYCLICLOG_Export CyclicLog: public BasicEventHandling_ns::BasicEventRaiser
    {
    public:
        CyclicLog();
        ~CyclicLog();

        bool Init(const std::string & szFileName, bool bFlush, long m_lLineSize, long MaxFileSize, bool bReinit = false);
        bool Add(const std::string & szSource, long Severity, long Code, const std::string & description);

    private:

        enum { sPosLength = 20 };

        size_t m_lTitleLength;
        size_t m_lHeadingSize;
        std::string m_strTitle;
        std::fstream m_fstream;
        bool m_bFlush;
        size_t m_lLineSize;
        size_t m_lMaxFileSize;

        // methods
        bool WriteBuff(char * pszBuff);
        bool Write(char * szBuff, bool bEndOfLog = false);
        void GetLastPos(size_t & lLastPos);
        bool IsOverEndOfFile(size_t lLastPos, size_t lNewStringLength);
        void UpdateLastPos();
        void WriteDelimitingLine();
        void FillBuffer(char * psz);
        bool PutToLog(char * pszTemp, size_t lTempLen);
    };
} // CyclicLog_ns namespace


#endif // __CYCLICLOG_H__
