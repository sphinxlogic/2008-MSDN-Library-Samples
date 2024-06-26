// ComposeHandler.h : Defines the ATL Server request handler class
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#pragma once

#include "ComposeDB.h"

// class CComposeHandler
// This handler is responsible for adding composed messages to the user's outbox.
[ request_handler("Compose") ]
class CComposeHandler : public CMantaWebBase<CComposeHandler>
{
private:
	CStringA m_strTo;			// Message to field
	CStringA m_strFrom;			// Message from field
	CStringA m_strSubject;		// Message subject
	CStringA m_strMessage;		// Message body
	bool m_bFailed;				// Boolean to specify if composing failed

public:
	// Default constructor
	CComposeHandler() : m_bFailed(false)
	{
	}

	HTTP_CODE ValidateAndExchange()
	{
		// Set the content type to html
		m_HttpResponse.SetContentType("text/html");
		m_HttpResponse.SetExpires(0);

		// Validate the current session
		if (!ValidateSession())
			return ValidationError();

		const CHttpRequestParams& FormFields = m_HttpRequest.GetFormVars();
		const CHttpRequestParams& QueryFields = m_HttpRequest.GetQueryParams();

		// Set the from field
		m_strFrom.Format("%s (%s %s)", GetLogin(), GetFirstName(), GetLastName());
		
		// If all the form fields are present
		if (FormFields.Lookup("to") != NULL &&
            FormFields.Lookup("subject") != NULL &&
			FormFields.Lookup("message") != NULL)
		{
			if (!ValidateFormData(FormFields))
			{
				m_bFailed = true;
				return HTTP_SUCCESS;
			}
			return AddNewMessage();
		}
		// Set the to and subject lines if query specified
		if (QueryFields.Lookup("msgto") != NULL)
			m_strTo = QueryFields.Lookup("msgto");
		if (QueryFields.Lookup("msgsubject") != NULL)
            m_strSubject = QueryFields.Lookup("msgsubject");
		return HTTP_SUCCESS;
	}

	[ tag_name("To") ]
	HTTP_CODE OnTo()
	{
		// Respond with the "to" field
		m_HttpResponse << m_strTo;
		return HTTP_SUCCESS;
	}

	[ tag_name("From")]
	HTTP_CODE OnFrom()
	{
		// Respond with the "from" field
		m_HttpResponse << m_strFrom;
		return HTTP_SUCCESS;
	}

	[ tag_name("Subject") ]
	HTTP_CODE OnSubject()
	{
		// Respond with the "subject" field
		m_HttpResponse << m_strSubject;
		return HTTP_SUCCESS;
	}

	[ tag_name("Message") ]
	HTTP_CODE OnMessage()
	{
		// Respond with the message body
		m_HttpResponse << m_strMessage;
		return HTTP_SUCCESS;
	}

	[ tag_name("Failure") ] 
	HTTP_CODE OnFailure()
	{
		// Return HTTP_SUCCESS if there was a failure
		return (m_bFailed) ? HTTP_SUCCESS : HTTP_S_FALSE;
	}
protected:

	bool ValidateFormData(const CHttpRequestParams& FormFields)
	{
		// Get the form fields
		if (FormFields.Exchange("to", &m_strTo) != VALIDATION_S_OK ||
			!IsStringValid(m_strTo, true, true, "@_."))
			return false;

		if (FormFields.Exchange("subject", &m_strSubject) != VALIDATION_S_OK)
			return false;

		if (FormFields.Exchange("message", &m_strMessage) != VALIDATION_S_OK)
			return false;

		// If this is not an internet address (i.e. missing @)
		if (m_strTo.Find("@") == -1)
		{
			// Find the user in the data base
			CFindUserID findUserID;
			
			if(FAILED(StringCchCopy(findUserID.m_szLogin, DB_MAX_STRLEN, m_strTo)))
				return false;
			HRESULT hr = findUserID.OpenRowset(m_dataConnection, NULL);
			if (hr != S_OK)
				return false;

			hr = findUserID.MoveFirst();
			if (hr != S_OK)
				return false;
		}
		return true;
	}

	HTTP_CODE AddNewMessage()
	{
		// Add message into the database
		CAddNewMessage msg;
		msg.m_lBox = 1;
		if(!GetUserID(&msg.m_lUserID))
			return HTTP_FAIL;

		// Prepare all the strings
		HTMLPrepareString(m_strTo);
		HTMLPrepareString(m_strSubject);
		HTMLPrepareString(m_strFrom);
		HTMLPrepareString(m_strMessage);

		if(FAILED(StringCchCopy(msg.m_szTo, DB_MAX_STRLEN, m_strTo)))
			return HTTP_FAIL;
		if(FAILED(StringCchCopy(msg.m_szSubject, DB_MAX_STRLEN, m_strSubject)))
			return HTTP_FAIL;
		if(FAILED(StringCchCopy(msg.m_szFrom, DB_MAX_STRLEN, m_strFrom)))
			return HTTP_FAIL;
		if(FAILED(StringCchCopy(msg.m_szMessage, MAX_MSG_LENGTH, m_strMessage)))
			return HTTP_FAIL;
				
		HRESULT hr = msg.OpenRowset(m_dataConnection, NULL);
		if (hr != S_OK)
			return DatabaseError("CAddNewMessage::OpenRowset()", hr);

		// Close the message to commit the data
		msg.Close();

		// Redirect to the outbox
		if(!m_HttpResponse.Redirect("outbox.srf"))
			return HTTP_FAIL;

		return HTTP_SUCCESS_NO_PROCESS;
	}
	
}; // class CComposeHandler
