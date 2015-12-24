// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently

#if !defined(AFX_STDAFX_H__EEA78529_F892_44D6_A1B3_8012000E08D9__INCLUDED_)
#define AFX_STDAFX_H__EEA78529_F892_44D6_A1B3_8012000E08D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#endif

#include "USB1020.h"
#include "NiIncludes.h"

//分别对应三关节控制卡物理ID号
#define   USB1020_1   1
#define   USB1020_2   2
#define   USB1020_3   4

//总的连接设备数
const int TotelDeviceNum = 3;   

//单卡总的轴数
const int AxisNum = 3;

#define   PULSENUMMODE    0x1
#define   ANGLEMODE           0x0
