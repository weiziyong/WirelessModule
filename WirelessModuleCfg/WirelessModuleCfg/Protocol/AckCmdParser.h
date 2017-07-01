//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : CAckCmdParser.h
//  @ Date : 2014/10/29 ������
//  @ Author :
//
//


#if !defined(_CACKCMDPARSER_H)
#define _CACKCMDPARSER_H

#include "Cmd.h"

class CAckCmdParser
{
    public:
        CAckCmdParser(CCmd Cmd);
        virtual ~CAckCmdParser();

        virtual BYTE GetFrameHead() const;
        virtual BYTE GetPreserveword() const;
        virtual BYTE GetCmdWord() const;
        virtual BOOL GetCmdData(PBYTE& pCmdData, DWORD& dwCmdDataLen) const;
        virtual DWORD GetCmdLen() const;
        virtual PBYTE GetCmdBuffer() const;
        virtual BYTE GetChecksum() const;

        virtual BOOL IsCfgCmd() const;
        virtual BOOL IsAckCmd() const;
        virtual BOOL IsValidCfgAckCmd() const;
        virtual BOOL IsValidSearchAckCmd() const;
        virtual BOOL IsValidAck() const;
        virtual BOOL IsCmd() const;

    private:
        BOOL IsValidCfgCmdWord() const;
        BOOL IsValidAckCmdWord() const;
        BOOL IsChecksumRight() const;

    protected:
        CCmd m_Cmd;
};

#endif  //_CACKCMDPARSER_H








