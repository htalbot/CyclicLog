
#include "CyclicLog/CyclicLog.h"
#include "CyclicLog/version.h"
#include "CyclicLog.evc.h"

#include "ace/Date_Time.h"
#include <memory>
#include <sstream>
#include <iomanip>

#ifdef ACE_WIN32
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#else
#include <unistd.h>
#include <sys/types.h>
#endif

bool IsDirExist(const std::string & strPath)
{
    //~ WIN32_FIND_DATA findFileData;

    //~ HANDLE handle = FindFirstFile(strPath.c_str(), &findFileData);

    //~ // if directory does not exist, create it
    //~ if ( handle == INVALID_HANDLE_VALUE )
        //~ return false;

    //~ FindClose(handle);

    ACE_stat stat_buf;
    if (ACE_OS::lstat (strPath.c_str(), &stat_buf) == -1)
    {
        return false;
    }

    if ((stat_buf.st_mode & S_IFDIR) == 0)
    {
        return false;
    }

    return true;
}

int CreerDir(const std::string & strPath)
{
    //~ WIN32_FIND_DATA findFileData;

    //~ HANDLE handle = FindFirstFile(strPath.c_str(), &findFileData);

    //~ // if directory does not exist, create it.
    //~ if ( handle == INVALID_HANDLE_VALUE )
    //~ {
        //~ SECURITY_ATTRIBUTES secAttr;

        //~ secAttr.nLength = sizeof(secAttr);
        //~ secAttr.lpSecurityDescriptor = NULL;
        //~ secAttr.bInheritHandle = false;

        //~ if ( !CreateDirectory(strPath.c_str(), &secAttr) )
        //~ {
            //~ // intermediate path does not exist, try do create it...
            //~ size_t nPosBackslash = strPath.rfind('\\');

            //~ if ( nPosBackslash != -1 )
            //~ {
                //~ // parent == intermediate path
                //~ std::string parent = strPath.substr(0, nPosBackslash);

                //~ int retCode = CreerDir(parent);

                //~ // once the intermediate path is created, create the complete path
                //~ if ( retCode == NO_ERROR )
                    //~ if ( !CreateDirectory(strPath.c_str(), &secAttr) )
                        //~ return GetLastError();
                    //~ else
                        //~ return NO_ERROR;
                //~ else
                    //~ return retCode;
            //~ }
            //~ else
                //~ return NO_ERROR;    // no path
        //~ }
        //~ else
            //~ return NO_ERROR;
    //~ }
    //~ else
    //~ {
        //~ FindClose(handle);

        //~ return ERROR_ALREADY_EXISTS;
    //~ }

    return ACE_OS::mkdir(strPath.c_str());
}



const short sDateTimeStringLength = 32;

namespace CyclicLog_ns
{
    void CyclicLog_get_version(int & major, int & minor, int & patch)
    {
        major = CYCLICLOG_MAJOR;
        minor = CYCLICLOG_MINOR;
        patch = CYCLICLOG_PATCH;
    }

    CyclicLog::CyclicLog()
    {
        // Initialize the title that indicate the position
        m_strTitle = "Position: ";
        m_lTitleLength = m_strTitle.length();

        int inc;
        #ifdef ACE_WIN32
        inc = 2;
        #else
        inc = 1;
        #endif

        m_lHeadingSize = m_lTitleLength + sPosLength + inc;
    }


    CyclicLog::~CyclicLog()
    {
    }

    bool CyclicLog::Init(const std::string & strFileName, bool bFlush, long lLineSize, long lMaxFileSize, bool bReinit)
    {
        m_bFlush = bFlush;
        m_lLineSize = lLineSize + 2;
        bool bNewFile = false;

        if (bReinit)
        {
            m_fstream.clear();
            m_fstream.close();
        }

        m_fstream.open(strFileName.c_str(), std::ios_base::in | std::ios_base::out); // to check readonly status
        if ( m_fstream.fail() )    // the file does not exist or the file is readonly
        {
            m_fstream.clear();

            // check if the file is readonly
            m_fstream.open(strFileName.c_str(), std::ios_base::in);
            if ( ! m_fstream.fail() ) // the file could be opened ==> read-only
            {
                raise_event(_SRC, CyclicLogEvents::ev_file_is_read_only, LM_ERROR,
                    strFileName.c_str());
                return false;
            }
            else
            {
                m_fstream.clear();  // the file could not be opened...

                // the file does not exist, try to create it

                m_fstream.open(strFileName.c_str(), ios::out);
                if ( m_fstream.fail() ) // maybe the path does not exist
                {
                    m_fstream.clear();

                    size_t nPosBackSlashRight = strFileName.rfind('\\');
                    size_t nPosBackSlashLeft = strFileName.find('\\');
                    if ( nPosBackSlashLeft == nPosBackSlashRight )
                    {
                        // we land in a root directory and then the file could not be created (it is not a subdirectory)
                        if (!bReinit)
                        {
                            raise_event(_SRC, CyclicLogEvents::ev_impossible_to_create_file, LM_ERROR,
                                strFileName.c_str());
                        }
                        return false;
                    }

                    if ( nPosBackSlashRight != std::string::npos )
                    {
                        // there is an intermadiate path (subdirectories)

                        std::string strPath = strFileName.substr(0, nPosBackSlashRight);
                        if ( IsDirExist(strPath) )
                        {
                            // the path exists, so the file could not be created
                            if (!bReinit)
                            {
                                raise_event(_SRC, CyclicLogEvents::ev_impossible_to_create_file, LM_ERROR,
                                    strFileName.c_str());
                            }
                            return false;
                        }
                        else
                        {
                            if ( CreerDir(strPath) != 0 )
                            {
                                if (!bReinit)
                                {
                                    raise_event(_SRC, CyclicLogEvents::ev_impossible_to_create_dir, LM_ERROR,
                                        strPath.c_str());
                                }
                                return false;
                            }
                        }

                        // now, we can try to create the file in the new path
                        m_fstream.open(strFileName.c_str(), ios::out);
                        if ( m_fstream.fail() )
                        {
                            m_fstream.clear();
                            if (!bReinit)
                            {
                                raise_event(_SRC, CyclicLogEvents::ev_impossible_to_create_file, LM_ERROR,
                                    strFileName.c_str());
                            }
                            return false;
                        }
                        else
                            bNewFile = true;
                    }
                    else
                    {
                        if (!bReinit)
                        {
                            raise_event(_SRC, CyclicLogEvents::ev_impossible_to_create_file, LM_ERROR,
                                strFileName.c_str());
                        }
                        return false;    // no file and... no path
                    }
                }
                else
                    bNewFile = true;
            }
        }

        m_fstream.close();

        // Set the needed file size
        size_t lAvailableLines = (lMaxFileSize - m_lHeadingSize)/m_lLineSize;
        size_t lMinNumberOfLines = lAvailableLines ? 1 : 2;    // at least 2 lines (one for data and another for delemiting)
        m_lMaxFileSize = m_lHeadingSize + (lAvailableLines * m_lLineSize) + (lMinNumberOfLines * m_lLineSize);

        if ( m_lMaxFileSize < m_lHeadingSize + m_lLineSize )
        {
            // It must be greater than the space needed to put the pos and at least two lines (of size m_lLineSize)
            // One for at least one line and another for the delimiting line
            raise_event(_SRC, CyclicLogEvents::ev_invalid_file_size, LM_ERROR,
                m_lMaxFileSize);
            return false;
        }

        #ifdef ACE_WIN32
        // Change the size of the file if needed
        // Open file (low level)
        int fh(0);
#ifdef ACE_WIN32
        if( (fh = _open( strFileName.c_str(), _O_RDWR | _O_CREAT, _S_IREAD | _S_IWRITE ))  != -1 )
#else
        if( (fh = std::fopen( strFileName.c_str(), _O_RDWR | _O_CREAT, _S_IREAD | _S_IWRITE ))  != -1 )
#endif
        {
            size_t lPresentFileSize = _filelength( fh );
            if ( lPresentFileSize != m_lMaxFileSize )
            {
                int rc = _chsize( fh, (long)m_lMaxFileSize );
                if ( rc != 0 )
                {
                    raise_event(_SRC, CyclicLogEvents::ev_changing_file_size_failure, LM_ERROR,
                        m_lMaxFileSize);
                    return false;
                }
            }

            _close( fh );
        }
        #else
            ACE_stat stat_buf;
            if (ACE_OS::lstat (strFileName.c_str(), &stat_buf) == -1)
            {
                return false;
            }

            if ( stat_buf.st_size != (ACE_OFF_T)m_lMaxFileSize )
            {
                truncate(strFileName.c_str(), stat_buf.st_size);
            }
        #endif

        m_fstream.open(strFileName.c_str(), ios::in | ios::out);

        // Put on the initial pos
        if ( bNewFile )
        {
            // Put on the initial Pos
            m_fstream.seekp(0);    // go to the beginning
            char * pszPos = new char[m_lHeadingSize];
            char szFormat[10];
            sprintf(szFormat, "%%s%%%dd", sPosLength);
            sprintf(pszPos, szFormat, m_strTitle.c_str(), m_lHeadingSize);

            WriteBuff(pszPos);

            delete [] pszPos;
        }

        return true;
    }


    bool CyclicLog::Add(const std::string & source, long lSeverity, long lCode, const std::string & description)
    {
        ACE_Date_Time dt;
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(4) << dt.year();
        ss << "-";
        ss << std::setfill('0') << std::setw(2) << dt.month(),
        ss << "-";
        ss << std::setfill('0') << std::setw(2) << dt.day(),
        ss << " ";
        ss << std::setfill('0') << std::setw(2) << dt.hour(),
        ss << ":";
        ss << std::setfill('0') << std::setw(2) << dt.minute(),
        ss << ":";
        ss << std::setfill('0') << std::setw(2) << dt.second(),
        ss << ".";
        ss << std::setfill('0') << std::setw(3) << dt.microsec() / 1000;
        ss << " - ";

        char szDateTime[sDateTimeStringLength];
        strcpy(szDateTime, ss.str().c_str());

        // Put the parameters in a string buffer
        char szSeverity[20];
        char szCode[20];
        unsigned uTabLength = 1;

        //~ ltoa(lSeverity, szSeverity, 10 );
        //~ ltoa(lCode, szCode, 10 );
        std::stringstream ss_sev;
        ss_sev << lSeverity;
        strcpy(szSeverity, ss_sev.str().c_str());

        std::stringstream ss_code;
        ss_code << lCode;
        strcpy(szCode, ss_code.str().c_str());


        size_t uLength = strlen(szDateTime) + uTabLength
            + source.length() + uTabLength
            + strlen(szSeverity) + uTabLength
            + strlen(szCode) + uTabLength
            + description.length();

        try
        {
            char * pszTemp = new char[uLength + 1];
            std::auto_ptr<char> spTemp(pszTemp);

            sprintf(pszTemp, "%s\t%s\t%s\t%s\t%s", szDateTime, source.c_str(), szSeverity, szCode, description.c_str());

            while (true)
            {
                size_t lTempLen = strlen(pszTemp);
                if ( lTempLen > m_lLineSize )
                {
                    if (!PutToLog(pszTemp, lTempLen))
                    {
                        return false;
                    }
                    strncpy(pszTemp, pszTemp + m_lLineSize, lTempLen - m_lLineSize + 1);
                    lTempLen = strlen(pszTemp);
                }
                else
                {
                    if ( strlen(pszTemp) )
                    {
                        // Last one
                        if (!PutToLog(pszTemp, lTempLen))
                        {
                            return false;
                        }
                    }

                    break;
                }
            }
        }
        catch(...)
        {
            return false;
        }

        return true;
    }


    bool CyclicLog::Write(char * szBuff, bool bDelimitingLine)
    {
        // Get the last pos from the file
        size_t lLastPos;
        GetLastPos(lLastPos);

        size_t lNewStringLength = strlen(szBuff);
        if ( IsOverEndOfFile(lLastPos, lNewStringLength) )
        {
            // We get the limit of the file and we must go at the beginning of the file
            m_fstream.seekp(m_lHeadingSize);    // Go at the beginning (after Heading)
        }
        else
            m_fstream.seekp(lLastPos);

        if (!WriteBuff(szBuff))
        {
            return false;
        }

        if ( ! bDelimitingLine )
        {
            UpdateLastPos();
            WriteDelimitingLine();
        }

        return true;
    }


    void CyclicLog::GetLastPos(size_t & lLastPos)
    {
        // Get last pos from the file
        char szLastPos[MaxLineLength + 1];    // + 1:  0 terminated character

        m_fstream.seekg(0);

        m_fstream.getline(szLastPos, MaxLineLength);    // + 1:  LF (get the entire line)

        char szTemp[MaxLineLength + 1];
        strncpy(szTemp, &szLastPos[strlen(szLastPos) - sPosLength], sPosLength );
        szTemp[sPosLength] = 0;

        lLastPos = atol(szTemp);
    }


    bool CyclicLog::IsOverEndOfFile(size_t lLastPos, size_t lNewStringLength)
    {
        return lLastPos + lNewStringLength - 2 > m_lMaxFileSize;
    }


    bool CyclicLog::WriteBuff(char * szBuff)
    {
        m_fstream << szBuff << "\n";

        if ( m_bFlush )
        {
            m_fstream.flush();
        }

        if (!m_fstream.good())
        {
            return false;
        }

        return true;
    }

    void CyclicLog::UpdateLastPos()
    {
        // Get the current pos
        std::streamoff lLastPos = m_fstream.tellg();

        // let's update LastPos
        char szLastPos[sPosLength + 1];    // + 1:  0 terminated character
        char szFormat[10];
        sprintf(szFormat, "%%%dd", sPosLength);
        sprintf(szLastPos, szFormat, lLastPos);

        m_fstream.seekp(m_lTitleLength, ios::beg);
        WriteBuff(szLastPos);
    }


    void CyclicLog::FillBuffer(char * pszLog)
    {
        // Fill the buffer up to the line length (m_lLineSize)
        size_t szLogLength = strlen(pszLog);
        if ( szLogLength <= m_lLineSize )
        {
            for ( size_t i = 0; i < m_lLineSize - szLogLength; i++ )
                strcat(pszLog, " ");
        }
    }


    bool CyclicLog::PutToLog(char * pszTemp, size_t lTempLen)
    {
        try
        {
            char * pszLog = new char[m_lLineSize + 1];
            pszLog[m_lLineSize] = 0;
            std::auto_ptr<char> spLog(pszLog);

            strncpy(pszLog, pszTemp, std::min(lTempLen + 1, m_lLineSize));

            FillBuffer(pszLog);

            // Write the buffer in the file
            if(!Write(pszLog))
            {
                return false;
            }
        }
        catch(...)
        {
            return false;
        }

        return true;
    }


    void CyclicLog::WriteDelimitingLine()
    {
        try
        {
            char * pszLine = new char[m_lLineSize + 1];
            pszLine[m_lLineSize] = 0;

            for ( size_t i = 0; i < m_lLineSize; i++ )
                pszLine[i] = '-';

            Write(pszLine, true);

            delete [] pszLine;
        }
        catch(...)
        {
            throw;
        }
    }


} // CyclicLog_ns namespace
