// Cinco.cpp : Defines the entry point for the DLL application.
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#include "stdafx.h"
// For custom assert and trace handling with WebDbg.exe
#ifdef _DEBUG
CDebugReportHook g_ReportHook;
#endif

#include "CincoDict.h"
#include "Cinco.h"
CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()



// DLL Entry Point
//
extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	return _Module.DllMain(hInstance, dwReason, lpReserved, ObjectMap, NULL); 
}
