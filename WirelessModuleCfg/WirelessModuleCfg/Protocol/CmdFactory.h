//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : CCmdFactory.h
//  @ Date : 2014/11/5 ������
//  @ Author :
//
//


#if !defined(_CCMDFACTORY_H)
#define _CCMDFACTORY_H

#include "Cmd.h"

class CCmdFactory
{
    public:
        CCmdFactory();
        ~CCmdFactory();

        CCmd* createCmd(const BYTE btCmdWord,
                        UCHAR* pCmdData,
                        const DWORD dwCmdDataLen) const;
        CCmd* createEmptyAckCmd(const DWORD dwCmdDataLen) const;
};

#endif  //_CCMDFACTORY_H
