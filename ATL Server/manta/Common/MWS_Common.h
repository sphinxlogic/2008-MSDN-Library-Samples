// MWS_Common.h: this is the common header to the MantaWeb sample
// If defines the session database commands, and the MantaWebBase templated base class
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#pragma once

#include <atlstencil.h>
#include <atldbcli.h>
#include <atlcomtime.h>

// Disable the warning messages from the platform include's
// This disables the warning message for "deprecated" functions like strcpy(), sprintf(), etc
#define STRSAFE_NO_DEPRECATE

#include <strsafe.h>


#define DB_MAX_STRLEN		50		// Max length of string in characters in database 
#define DB_MAX_DETAILSLEN	150		// Max details length (task and schedule)
#define MAX_MSG_LENGTH		4096	// Max mail message length
#define SESSION_TIME_OUT	15		// Session time out (in minutes)

// Turn these defines on if you want the sample to attempt to send e-mail
// Note: search for this define and make sure the code is correct for your particular configuration.
// #define SEND_CONFIRMATION_MAIL
// #define SEND_FORGOTPASS_MAIL
// #define SEND_OUTGOING_MAIL

namespace MantaWeb
{
	const char MANTAWEB_PERSISTANT_COOKIE_NAME[]	=	"MantaWebCookie";			// Persisted cookie
	const char MANTAWEB_SESSION_COOKIE_NAME[]		=	"MantaWebSessionCookie";	// Session cookie
	const char MANTAWEB_DATA_SOURCE_CACHE_NAME[]	=	"MantaWebDataSourceCache";	// Data source cache name

	// Our database connection string
	// Note: the directory permissions for IUSR_<MACHINE NAME> on the database directory should be set to read/write.
	const wchar_t MANTAWEB_CONNECTION_STRING[] = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\\MantaWebSample\\Database\\MantaWeb.mdb;Persist Security Info=False";

	[
		db_command("SELECT [SessionID], [LastAccess] FROM ActiveUserTable WHERE [UserID]=?")
	]
	class CSessionData
	{
	public:
		[ db_column(ordinal="1") ] GUID m_guidSessionID;
		[ db_column(ordinal="2") ] DATE m_lastAccess;
		[ db_param(ordinal="1") ] LONG m_lUserID;
	};

	// Note: the following SQL statement is MS Access specific (use of access function Date() and Time())
	[
		db_command("UPDATE ActiveUserTable SET [LastAccess]=Date()+Time() WHERE [SessionID]=?")
	]
	class CUpdateSessionTimeStamp
	{
	public:
		[ db_param(ordinal="1") ] GUID m_guidSessionID;
	};

	// Note: the following SQL statement is MS Access specific (SQL Server statement: "DELETE ActiveUserTable WHERE [SessionID]=?")
	[
		db_command("DELETE * FROM ActiveUserTable WHERE [SessionID]=?")
	]
	class CRemoveSessionData
	{
	public:
		[ db_param(ordinal="1") ] GUID m_guidSessionID;
	};

	// Note: the following SQL statement is MS Access specific (use of access function Date() and Time())
	[
		db_command("INSERT INTO ActiveUserTable ([UserID], [LastAccess]) VALUES(?, Date()+Time())")
	]
	class CInsertSession
	{
	public:
		[ db_param(ordinal="1") ] LONG m_lUserID;
	};
}

// class CMantaWebBase
// This is the base class all request handlers derive from
// Class provides session support, data source cache access, and other helper methods
template <class T> class CMantaWebBase
{
protected:
	CDataConnection m_dataConnection;	// Cached data connection

	// This version uses the session cookie
	bool ValidateSession()
	{
		// Get the cached data connection
		if (FAILED(GetDataConnection(&m_dataConnection)))
			return false;

		// Get the session data from the session cookie
		LPCSTR lpszLogin = GetLogin();
		LPCSTR lpszFirstName = GetFirstName();
		LPCSTR lpszLastName = GetLastName();
		LONG lUserID;
		GUID guidSessionID;

		// If all the session cookie lookups succeeded
		if (lpszLogin != NULL && lpszFirstName != NULL && lpszLastName != NULL &&
			GetUserID(&lUserID) && GetSessionID(&guidSessionID))
		{
			MantaWeb::CSessionData data;
			
			// Get the session id for this user
			data.m_lUserID = lUserID;
			if (data.OpenRowset(m_dataConnection, NULL) != S_OK)
				return false;
			if (data.MoveFirst() != S_OK)
				return false;

			// If the session id does not match the one in the session cookie, return false
			if (guidSessionID != data.m_guidSessionID)
				return false;

			// If the session timed out
			if (SessionTimedOut(data.m_lastAccess))
			{
				// Remove the session from the table
				MantaWeb::CRemoveSessionData removeData;
				memcpy_s(&removeData.m_guidSessionID, sizeof(GUID), &data.m_guidSessionID, sizeof(GUID));
				removeData.OpenRowset(m_dataConnection, NULL);
				return false;
			}
			// Update the session with a new time stamp
			MantaWeb::CUpdateSessionTimeStamp updateTimestamp;
			memcpy_s(&updateTimestamp.m_guidSessionID, sizeof(GUID), &data.m_guidSessionID, sizeof(GUID));
			if (updateTimestamp.OpenRowset(m_dataConnection, NULL) != S_OK)
				return false;

			return true;	// Session is valid	
		}
		return false;	// Session is not valid
	}

	// This version just uses the user id and the session id
	bool ValidateSession(LONG lUserID, GUID& sessionID)
	{
		// Get the cached data connection
		if (FAILED(GetDataConnection(&m_dataConnection)))
			return false;

		// Lookup the session id based on the user id
		MantaWeb::CSessionData data;
		data.m_lUserID = lUserID;
		if (data.OpenRowset(m_dataConnection, NULL) != S_OK)
			return false;
		if (data.MoveFirst() != S_OK)
			return false;

		// If the session id's do not match, return false
		if (sessionID != data.m_guidSessionID)
			return false;

		// If the session timed out
		if (SessionTimedOut(data.m_lastAccess))
		{
			// Remove the session from the table
			MantaWeb::CRemoveSessionData removeData;
			memcpy_s(&removeData.m_guidSessionID, sizeof(GUID), &data.m_guidSessionID, sizeof(GUID));
			removeData.OpenRowset(m_dataConnection, NULL);
			return false;
		}
		// Update the session with a new time stamp
		MantaWeb::CUpdateSessionTimeStamp updateTimestamp;
		memcpy_s(&updateTimestamp.m_guidSessionID, sizeof(GUID), &data.m_guidSessionID, sizeof(GUID));
		if (updateTimestamp.OpenRowset(m_dataConnection, NULL) != S_OK)
			return false;
	
		return true;	// Session is valid
	}

	bool SessionTimedOut(const DATE& dLastAccess)
	{
		// Check to see if the session has spanned more than 
		// SESSION_TIME_OUT minutes past the current time
		COleDateTime lastAccess(dLastAccess);
		COleDateTimeSpan expireSpan = COleDateTime::GetCurrentTime() - lastAccess;
		if (expireSpan.GetDays() == 0 && expireSpan.GetHours() == 0 && expireSpan.GetMinutes() < SESSION_TIME_OUT)
			return false;	// Session has not timed out
		return true;	// Session has timed out
	}

	HRESULT GetDataConnection(CDataConnection* pConnection)
	{
		// Get the cached data source connection.
		// If the connection is not cached, it will create a new one and cache it,
		// using the supplied connection string.
		T* pT = static_cast<T*>(this);
		return GetDataSource(pT->m_spServiceProvider, MantaWeb::MANTAWEB_DATA_SOURCE_CACHE_NAME,
							 MantaWeb::MANTAWEB_CONNECTION_STRING, pConnection);
	}

	HTTP_CODE DatabaseError(LPCSTR lpszName, HRESULT hr)
	{
		// Return a database error message to the client
		T* pT = static_cast<T*>(this);
		
		// Clear the response
		pT->m_HttpResponse.ClearResponse();

		// Respond with the error message
		pT->m_HttpResponse << "<HTML><HEAD><TITLE>Database Error</TITLE></HEAD><BODY>";

		// Only show the error codes for debug builds
#ifdef _DEBUG
		CString str;
		str.Format("0x%x", hr);
		
		pT->m_HttpResponse << "OLE DB ERROR:<BR>" << ((lpszName) ? lpszName : "An OleDB call") << " returned: "
					       << str;
#else
		pT->m_HttpResponse << "<p>A database error has occured.</p><p>Please try again later.</p>";
#endif
		pT->m_HttpResponse << "</BODY></HTML>";

		// Flush the response to client
		pT->m_HttpResponse.Flush();
		return HTTP_SUCCESS_NO_PROCESS;
	}

	HTTP_CODE ValidationError()
	{
		// Return a validation error message to the client
		T* pT = static_cast<T*>(this);

		// Clear the response
		pT->m_HttpResponse.ClearResponse();
		// Respond with the error message
		pT->m_HttpResponse << "<HTML><HEAD><TITLE>Validation Timeout</TITLE></HEAD><BODY>"
							  "You have attempted to access a restricted resource.<br><br>"
							  "Possible causes for seeing this error message:<br>"
							  "   1) Your authenticated session timed out (timeout is 15 minutes).<br>"
							  "   2) You need to <a href=\"login.srf\">login</a>.<br><br>"
							  "</BODY></HTML>";

		// Flush the response to client
		pT->m_HttpResponse.Flush();
		return HTTP_SUCCESS_NO_PROCESS;
	}

	HTTP_CODE SMTPError()
	{
		// Return a SMTP error message to the client
		T* pT = static_cast<T*>(this);

		// Clear the response
		pT->m_HttpResponse.ClearResponse();
		// Respond with the error message
		pT->m_HttpResponse <<   "<HTML><HEAD><TITLE>MantaWeb: SMTP Error</TITLE></HEAD><BODY>"
							    "There was an error attempting to send a message through the SMTP server.<BR><BR>"
							    "Please try again later.</BODY></HTML>";
		// Flush the response to client
		pT->m_HttpResponse.Flush();
		return HTTP_SUCCESS_NO_PROCESS;
	}

	// This overload takes the integral types and converts them
	BOOL WriteSessionCookie(LPCSTR lpszUser, LPCSTR lpszFirstName, LPCSTR lpszLastName, LONG lUserID, GUID& guidSessionID)
	{
		// Convert the long to a string
		CStringA strUserID;
		strUserID.Format("%d", lUserID);
		
		// Convert the guid to a string
		LPOLESTR lpStrGuid;
		if (FAILED(StringFromCLSID(guidSessionID, &lpStrGuid)))
			return FALSE;
		
		
		// Call the other overloaded call with all strings

		BOOL bRet = WriteSessionCookie(lpszUser, lpszFirstName, lpszLastName, strUserID, CW2A(lpStrGuid));

		// Free the guid string
		CoTaskMemFree(&lpStrGuid);

		return bRet;
	}

	// This overload takes all strings and writes to the session cookie (the previous method relies on this method)
	BOOL WriteSessionCookie(LPCSTR lpszUser, LPCSTR lpszFirstName, LPCSTR lpszLastName, LPCSTR lpszUserID, LPCSTR lpszSessionID)
	{
		T* pT = static_cast<T*>(this);

		// Create the cookie as discardable, with / as the path
		CCookie cookie;
		cookie.SetDiscard(TRUE);
		cookie.SetPath("/");
		// Set the name to the session cookie name
		cookie.SetName(MantaWeb::MANTAWEB_SESSION_COOKIE_NAME);

		// Add all the values to the cookie
		cookie.AddValue("Login", lpszUser);
		cookie.AddValue("FirstName", lpszFirstName);
		cookie.AddValue("LastName", lpszLastName);
		cookie.AddValue("UserID", lpszUserID);
		cookie.AddValue("SessionID", lpszSessionID);

		// Append the cookie to the response
		return pT->m_HttpResponse.AppendCookie(cookie);
	}

	// This method writes to the persistent cookie instead of the session cookie
	BOOL WritePersistCookie(LPCSTR lpszLogin, LPCSTR lpszFirstName, GUID guidRememberID)
	{
		// Convert the guid to a string
		LPOLESTR lpStrGuid;

		if (FAILED(StringFromCLSID(guidRememberID, &lpStrGuid)))
			return FALSE;
		
		// Call the other overloaded call with all strings
		BOOL bRet = WritePersistCookie(lpszLogin, lpszFirstName, (LPCSTR) CString(lpStrGuid));

		// Free the guid string
		CoTaskMemFree(&lpStrGuid);

		return bRet;
	}

	// This method writes to the persistent cookie instead of the session cookie
	BOOL WritePersistCookie(LPCSTR lpszLogin, LPCSTR lpszFirstName, LPCSTR lpszRememberID)
	{
		T* pT = static_cast<T*>(this);
		SYSTEMTIME st;

		// Get the current time, add a year to that (cookie will expire 1 year from today)
		GetSystemTime(&st);
		st.wYear++;

		// Create the cookie that expires one year from today, set the path to '/'
		// Note: since this cookie is only used by the login handler, ideally you'd make the path
		//       exclusive to login.srf (where it resides in the URI)
		CCookie cookie;
		cookie.SetPath("/");
		cookie.SetExpires(st);

		// Set the name to the persistant cookie name
		cookie.SetName(MantaWeb::MANTAWEB_PERSISTANT_COOKIE_NAME);

		// Add all the values to the cookie
		cookie.AddValue("Login", lpszLogin);
		cookie.AddValue("FirstName", lpszFirstName);
		cookie.AddValue("RememberID", lpszRememberID);

		// Append the cookie to the response
		return pT->m_HttpResponse.AppendCookie(cookie);
	}

	// This method gets the login from either the persistent cookie or the session cookie
	LPCSTR GetLogin(bool bFromPersist = false)
	{
		T* pT = static_cast<T*>(this);
	
		LPCTSTR lpcszLogin;

		if (bFromPersist)	// If from persistent cookie, return the login
			lpcszLogin = pT->m_HttpRequest.Cookies(MantaWeb::MANTAWEB_PERSISTANT_COOKIE_NAME).Lookup("Login");
		else	// Otherwise, get the login from the session cookie
			lpcszLogin = pT->m_HttpRequest.Cookies(MantaWeb::MANTAWEB_SESSION_COOKIE_NAME).Lookup("Login");

		if (!IsStringValid(lpcszLogin))
			return NULL;

		return lpcszLogin;
	}

	// This method gets the first name from either the persistent cookie or the session cookie
	LPCSTR GetFirstName(bool bFromPersist = false)
	{
		T* pT = static_cast<T*>(this);

		LPCTSTR lpcszFirstName;

		if (bFromPersist)	// If from persistent cookie, return the first name
			lpcszFirstName = pT->m_HttpRequest.Cookies(MantaWeb::MANTAWEB_PERSISTANT_COOKIE_NAME).Lookup("FirstName");
		else	// Otherwise, get the first name from the session cookie
			lpcszFirstName = pT->m_HttpRequest.Cookies(MantaWeb::MANTAWEB_SESSION_COOKIE_NAME).Lookup("FirstName");

		if (!IsStringValid(lpcszFirstName, true, false, " "))
			return NULL;

		return lpcszFirstName;
	}

	LPCSTR GetLastName()
	{
		// Return the last name from the session cookie
		T* pT = static_cast<T*>(this);
		LPCTSTR lpcszLastName;
		
		lpcszLastName = pT->m_HttpRequest.Cookies(MantaWeb::MANTAWEB_SESSION_COOKIE_NAME).Lookup("LastName");
		
		if (!IsStringValid(lpcszLastName, true, false, " "))
			return NULL;

		return lpcszLastName;
	}

	bool GetRememberID(GUID* pguidRememberID)
	{
		ATLASSERT(pguidRememberID != NULL);

		T* pT = static_cast<T*>(this);

		// Get the remember id from the persistant cookie
		if (pT->m_HttpRequest.Cookies(MantaWeb::MANTAWEB_PERSISTANT_COOKIE_NAME).Exchange("RememberID", pguidRememberID) != VALIDATION_S_OK)
			return false;

		return true;
	}

	bool GetUserID(long* plUserID)
	{
		ATLASSERT(plUserID != NULL);

		T* pT = static_cast<T*>(this);

		// Get the long value for the user id from the session cookie
		if (pT->m_HttpRequest.Cookies(MantaWeb::MANTAWEB_SESSION_COOKIE_NAME).Exchange("UserID", plUserID) != VALIDATION_S_OK)
			return false;

		return true;
	}

	LPCSTR GetUserID()
	{
		T* pT = static_cast<T*>(this);
		LPCTSTR lpcszUserID;

		// Get the string value representing the user id
		lpcszUserID = pT->m_HttpRequest.Cookies(MantaWeb::MANTAWEB_SESSION_COOKIE_NAME).Lookup("UserID");

		if (!IsStringValid(lpcszUserID, false, true))
			return NULL;

		return lpcszUserID;
	}

	bool GetSessionID(GUID* pguidSessionID)
	{
		ATLASSERT(pguidSessionID != NULL);

		T* pT = static_cast<T*>(this);

		// Get the guid session id from the session cookie
		if (pT->m_HttpRequest.Cookies(MantaWeb::MANTAWEB_SESSION_COOKIE_NAME).Exchange("SessionID", pguidSessionID) != VALIDATION_S_OK)
			return false;

		return true;
	}

	LPCSTR GetSessionID()
	{
		T* pT = static_cast<T*>(this);
		LPCSTR lpcszSessionID;

		// Get the string session id from the sessino cookie
		lpcszSessionID = pT->m_HttpRequest.Cookies(MantaWeb::MANTAWEB_SESSION_COOKIE_NAME).Lookup("SessionID");

		if (!IsStringValid(lpcszSessionID, true, true, "{}-"))
			return NULL;

		return lpcszSessionID;
	}

	// This method puts in HTML sequences to prevent script attacks
	void HTMLPrepareString(CString& str)
	{
		str.Replace(_T("<"), "&lt;");
		str.Replace(_T(">"), "&gt;");
		str.Replace(_T("\""), "&quot;");

		// Add more here as needed
	}

	// This method is used to scrub the data to ensure its valid for insertion into the database
	bool IsStringValid(LPCTSTR lpcszString, bool bAllowAlpha = true, bool bAllowNumeric = true, LPCTSTR lpcszAdditional = NULL)
	{
		if (lpcszString == NULL)
			return false;

		int len = lstrlen(lpcszString);
		int len_additional;
		int i, j;

		if (lpcszAdditional == NULL)
			len_additional = 0;
		else
			len_additional = lstrlen(lpcszAdditional);

		for (i = 0; i < len; i++)
		{
			if ((!bAllowAlpha || (lpcszString[i] < 'A' || lpcszString[i] > 'Z') && (lpcszString[i] < 'a' || lpcszString[i] > 'z')) &&
				(!bAllowNumeric || (lpcszString[i] < '0' || lpcszString[i] > '9')))
			{
				for (j = 0; j < len_additional; j++)
				{
					if (lpcszString[i] == lpcszAdditional[j])
						break;
				}
				if (j >= len_additional)
					break;
			}
		}

		return (i < len) ? false : true;
	}

	// This method loads HTML content from resource into a CString
	// This is useful if you want to return HTML files that aren't exposed through the web server
	bool LoadHtmlFromResource(LPCTSTR lpName, CStringA& strHTML)
	{
		// Find the specified resource
		HRSRC hResInfo = FindResource(_AtlBaseModule.GetResourceInstance(), lpName, RT_HTML);
		if (hResInfo != NULL)
		{
			// Load the resource
			HGLOBAL hRes = LoadResource(_AtlBaseModule.GetResourceInstance(), hResInfo);
			if (hRes != NULL)
			{
				// Lock the resource and copy it
				// The resource does not need to be unlocked and freed
	            strHTML = (LPSTR) LockResource(hRes);
				return true;
			}
		}
		// An error occured, print the message
		char szName[MAX_PATH];
		// Get the module file name
		GetModuleFileName(_AtlBaseModule.GetResourceInstance(), szName, MAX_PATH);
		if (hResInfo == NULL)
		{
			// FindResource failed, set the HTML as the error message
			strHTML.Format("<HTML><TITLE>MantaWeb: Resource Failure</TITLE><BODY>"
				           "FindResource() failed.<BR>Module file name: %s<BR>LastError: %d</BODY></HTML>", 
						   szName, ::GetLastError());
		}
		else
		{
			// LoadResource failed, set the HTML as the error message
			strHTML.Format("<HTML><TITLE>MantaWeb: Resource Failure</TITLE><BODY>"
						   "LoadResource() failed.<BR>Module file name: %s<BR>LastError: %d</BODY></HTML>", 
						   szName, ::GetLastError());
		}
		return false;
	}
};