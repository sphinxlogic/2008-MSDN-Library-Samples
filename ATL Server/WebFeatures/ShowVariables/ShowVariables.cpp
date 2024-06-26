// ShowVariables.cpp : Defines the entry point for the DLL application.
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#include "stdafx.h"
#include "..\VCUE\VCUE_ErrorString.h"
#include "..\VCUE\VCUE_Time.h"
#include "..\VCUE\VCUE_AtlServerSample.h"
using namespace VCUE;

VCUE::CModule _Module;

// For custom assert and trace handling with WebDbg
#ifdef _DEBUG
CDebugReportHook g_ReportHook;
#endif

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
	return _Module.DllMain(dwReason, lpReserved); 
}

class CShowVariables : 
	public CRequestHandlerT<CShowVariables>
{
public:

	HTTP_CODE ValidateAndExchange()
	{
		// Set the headers
		m_HttpResponse.SetContentType("text/html");
		m_HttpResponse.AppendHeader("Last-Modified", HttpTime());

		return HTTP_SUCCESS; // continue processing request
	}

	DWORD FormFlags()
	{
		// Don't create files if form data contains them.
		return ATL_FORM_FLAG_IGNORE_FILES;
	}

#ifdef _DEBUG

// Return error string in debug builds
#define STREAM_RAW_VARIABLE( ItemName )							\
		bSuccess = m_HttpRequest.GetServerVariable(#ItemName, strValue);	\
		m_HttpResponse << "<tr><td>" << #ItemName << "</td><td>";		\
		if (bSuccess)											\
			m_HttpResponse << strValue;									\
		else													\
			m_HttpResponse << "<i>" << GetErrorString() << "</i>";	\
		m_HttpResponse << "</td></tr>\n";

#define STREAM_RAW_VARIABLE_PRE( ItemName )							\
		bSuccess = m_HttpRequest.GetServerVariable(#ItemName, strValue);	\
		m_HttpResponse << "<tr><td>" << #ItemName << "</td><td>";		\
		if (bSuccess)											\
			m_HttpResponse << "<pre>" << strValue << "</pre>";									\
		else													\
			m_HttpResponse << "<i>" << GetErrorString() << "</i>";	\
		m_HttpResponse << "</td></tr>\n";

#else

// Return a generic error in production builds
#define STREAM_RAW_VARIABLE( ItemName )							\
		bSuccess = m_HttpRequest.GetServerVariable(#ItemName, strValue);	\
		m_HttpResponse << "<tr><td>" << #ItemName << "</td><td>";		\
		if (bSuccess)											\
			m_HttpResponse << strValue;									\
		else													\
			m_HttpResponse << "<i>" << "Error retrieving variable" << "</i>";	\
		m_HttpResponse << "</td></tr>\n";

#define STREAM_RAW_VARIABLE_PRE( ItemName )							\
		bSuccess = m_HttpRequest.GetServerVariable(#ItemName, strValue);	\
		m_HttpResponse << "<tr><td>" << #ItemName << "</td><td>";		\
		if (bSuccess)											\
			m_HttpResponse << "<pre>" << strValue << "</pre>";									\
		else													\
			m_HttpResponse << "<i>" << "Error retrieving variable" << "</i>";	\
		m_HttpResponse << "</td></tr>\n";

#endif

	HTTP_CODE OnShowVariables()
	{
		CStringA strValue;
		BOOL bSuccess = FALSE;

		STREAM_RAW_VARIABLE_PRE(ALL_HTTP);
		STREAM_RAW_VARIABLE_PRE(ALL_RAW);
		STREAM_RAW_VARIABLE(APPL_MD_PATH);
		STREAM_RAW_VARIABLE(APPL_PHYSICAL_PATH);
		STREAM_RAW_VARIABLE(AUTH_PASSWORD);
		STREAM_RAW_VARIABLE(AUTH_TYPE);
		STREAM_RAW_VARIABLE(AUTH_USER);
		STREAM_RAW_VARIABLE(CERT_COOKIE);
		STREAM_RAW_VARIABLE(CERT_FLAGS);
		STREAM_RAW_VARIABLE(CERT_ISSUER);
		STREAM_RAW_VARIABLE(CERT_KEYSIZE);
		STREAM_RAW_VARIABLE(CERT_SECRETKEYSIZE);
		STREAM_RAW_VARIABLE(CERT_SERIALNUMBER);
		STREAM_RAW_VARIABLE(CERT_SERVER_ISSUER);
		STREAM_RAW_VARIABLE(CERT_SERVER_SUBJECT);
		STREAM_RAW_VARIABLE(CERT_SUBJECT);
		STREAM_RAW_VARIABLE(CONTENT_LENGTH);
		STREAM_RAW_VARIABLE(CONTENT_TYPE);
		STREAM_RAW_VARIABLE(GATEWAY_INTERFACE);
		STREAM_RAW_VARIABLE(HTTP_ACCEPT);
		STREAM_RAW_VARIABLE(HTTP_ACCEPT_CHARSET);
		STREAM_RAW_VARIABLE(HTTP_ACCEPT_ENCODING);
		STREAM_RAW_VARIABLE(HTTP_ACCEPT_LANGUAGE);
		STREAM_RAW_VARIABLE(HTTP_AUTHORIZATION);
		STREAM_RAW_VARIABLE(HTTP_CACHE_CONTROL);
		STREAM_RAW_VARIABLE(HTTP_CONNECTION);
		STREAM_RAW_VARIABLE(HTTP_CONTENT_LENGTH);
		STREAM_RAW_VARIABLE(HTTP_CONTENT_TYPE);
		STREAM_RAW_VARIABLE(HTTP_COOKIE);
		STREAM_RAW_VARIABLE(HTTP_DATE);
		STREAM_RAW_VARIABLE(HTTP_EXPECT);
		STREAM_RAW_VARIABLE(HTTP_FROM);
		STREAM_RAW_VARIABLE(HTTP_HOST);
		STREAM_RAW_VARIABLE(HTTP_IF_MATCH);
		STREAM_RAW_VARIABLE(HTTP_IF_MODIFIED_SINCE);
		STREAM_RAW_VARIABLE(HTTP_IF_NONE_MATCH);
		STREAM_RAW_VARIABLE(HTTP_IF_RANGE);
		STREAM_RAW_VARIABLE(HTTP_IF_UNMODIFIED_SINCE);
		STREAM_RAW_VARIABLE(HTTP_MAX_FORWARDS);
		STREAM_RAW_VARIABLE(HTTP_PRAGMA);
		STREAM_RAW_VARIABLE(HTTP_PROXY_AUTHORIZATION);
		STREAM_RAW_VARIABLE(HTTP_RANGE);
		STREAM_RAW_VARIABLE(HTTP_REFERER);
		STREAM_RAW_VARIABLE(HTTP_TE);
		STREAM_RAW_VARIABLE(HTTP_TRAILER);
		STREAM_RAW_VARIABLE(HTTP_TRANSFER_ENCODING);
		STREAM_RAW_VARIABLE(HTTP_UPGRADE);
		STREAM_RAW_VARIABLE(HTTP_URL);
		STREAM_RAW_VARIABLE(HTTP_USER_AGENT);
		STREAM_RAW_VARIABLE(HTTP_VERSION);
		STREAM_RAW_VARIABLE(HTTP_VIA);
		STREAM_RAW_VARIABLE(HTTP_WARNING);
		STREAM_RAW_VARIABLE(HTTPS);
		STREAM_RAW_VARIABLE(HTTPS_KEYSIZE);
		STREAM_RAW_VARIABLE(HTTPS_SECRETKEYSIZE);
		STREAM_RAW_VARIABLE(HTTPS_SERVER_ISSUER);
		STREAM_RAW_VARIABLE(HTTPS_SERVER_SUBJECT);
		STREAM_RAW_VARIABLE(INSTANCE_ID);
		STREAM_RAW_VARIABLE(INSTANCE_META_PATH);
		STREAM_RAW_VARIABLE(LOCAL_ADDR);
		STREAM_RAW_VARIABLE(LOGON_USER);
		STREAM_RAW_VARIABLE(LOGONSERVER);
		STREAM_RAW_VARIABLE(NUMBER_OF_PROCESSORS);
		STREAM_RAW_VARIABLE(OS);
		STREAM_RAW_VARIABLE(PATH_INFO);
		STREAM_RAW_VARIABLE(PATH_TRANSLATED);
		STREAM_RAW_VARIABLE(PROCESSOR_ARCHITECTURE);
		STREAM_RAW_VARIABLE(PROCESSOR_IDENTIFIER);
		STREAM_RAW_VARIABLE(PROCESSOR_LEVEL);
		STREAM_RAW_VARIABLE(PROCESSOR_REVISION);
		STREAM_RAW_VARIABLE(QUERY_STRING);
		STREAM_RAW_VARIABLE(REMOTE_ADDR);
		STREAM_RAW_VARIABLE(REMOTE_HOST);
		STREAM_RAW_VARIABLE(REMOTE_IDENT);
		STREAM_RAW_VARIABLE(REMOTE_USER);
		STREAM_RAW_VARIABLE(REQUEST_METHOD);
		STREAM_RAW_VARIABLE(SCRIPT_NAME);
		STREAM_RAW_VARIABLE(SERVER_NAME);
		STREAM_RAW_VARIABLE(SERVER_PORT);
		STREAM_RAW_VARIABLE(SERVER_PORT_SECURE);
		STREAM_RAW_VARIABLE(SERVER_PROTOCOL);
		STREAM_RAW_VARIABLE(SERVER_SOFTWARE);
		STREAM_RAW_VARIABLE(TEMP);
		STREAM_RAW_VARIABLE(TMP);
		STREAM_RAW_VARIABLE(URL);
		STREAM_RAW_VARIABLE(USERNAME);

		return HTTP_SUCCESS;
	}

	HTTP_CODE OnTitle()
	{
		m_HttpResponse << "ShowVariables Sample";
		return HTTP_SUCCESS;
	}
	
	BEGIN_REPLACEMENT_METHOD_MAP(CShowVariables)
		REPLACEMENT_METHOD_ENTRY("Title", OnTitle)
		REPLACEMENT_METHOD_ENTRY("ShowVariables", OnShowVariables)
	END_REPLACEMENT_METHOD_MAP()
};

BEGIN_HANDLER_MAP()
	HANDLER_ENTRY("Default", CShowVariables)
END_HANDLER_MAP()
