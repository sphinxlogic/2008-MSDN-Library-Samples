// dmdoc.cpp
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
//
// Purpose: implementation of the CDynaMenuDoc class
//
// Functions:
//      Most of this file was generated by AppWizard.  The functions
//      which contain code specific to this sample are:
//
//      CDynaMenuDoc::CDynaMenuDoc()         -- document constructor
//      CDynaMenuDoc::DoSelectColor()        -- handle text color commands
//      CDynaMenuDoc::DoUpdateSelectColor()  -- update text color UI objects
//      CDynaMenuDoc::GetMessageString()     -- get status msg for color cmds
//      CDynaMenuDoc::OnCmdMsg()             -- perform command routing
//      CDynaMenuDoc::OnColorOptions()       -- display color options dialog

#include "stdafx.h"
#include "dynamenu.h"
#include "dmdoc.h"
#include "coloropt.h"  // for CColorOptDlg
#include "mdichild.h"  // for CDynaMDIChildWnd

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// This table defines the possible text colors which will be displayed in
// the color options dialog.  The colors selected in the options dialog
// for a document will appear on the Color submenu when that document is
// active.
//
DynaMenuDef CDynaMenuDoc::m_aColorDef[NUM_TEXTCOLOR] =
{
	{ IDS_BLACK,    ID_COLOR1, RGB(0,   0, 0) },
	{ IDS_RED,      ID_COLOR2, RGB(128, 0, 0) },
	{ IDS_PURPLE,   ID_COLOR3, RGB(128, 0, 128) },
	{ IDS_BLUE,     ID_COLOR4, RGB(0, 0, 128) },
};

/////////////////////////////////////////////////////////////////////////////
// CDynaMenuDoc

IMPLEMENT_DYNCREATE(CDynaMenuDoc, CDocument)

BEGIN_MESSAGE_MAP(CDynaMenuDoc, CDocument)
	//{{AFX_MSG_MAP(CDynaMenuDoc)
	ON_COMMAND(ID_COLOR_OPTIONS, OnColorOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// CDynaMenuDoc construction/destruction

CDynaMenuDoc::CDynaMenuDoc()
{
	// Initialize document data.  Start out by allowing all possible
	// text colors and default to using the first one.
	m_iColor = 0;
	for (int i = 0; i < NUM_TEXTCOLOR; i++)
	   m_abAllowColor[i] = TRUE;
}

CDynaMenuDoc::~CDynaMenuDoc()
{
}

BOOL CDynaMenuDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDynaMenuDoc serialization

void CDynaMenuDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDynaMenuDoc diagnostics

#ifdef _DEBUG
void CDynaMenuDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDynaMenuDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDynaMenuDoc implementation

//***********************************************************************
// Function: CDynaMenuDoc::OnCmdMsg()
//
// Purpose:
//    OnCmdMsg() is called by the framework to route and dispatch
//    command messages and to handle the update of command
//    user-interface objects.
//
//    Here we extend the standard command routing to intercept command
//    messages with variable command IDs.  In this sample, the
//    currently defined text color options are stored in
//    CDynaMenuDoc::m_aColorDef.  Each element of this array contains
//    the command ID associated with that color.  When a command ID
//    is passed to this function, we search m_aColorDef for a matching
//    command ID.  If one is found, we process the message.  Otherwise,
//    we route the command to the standard OnCmdMsg processing.
//
// Parameters:
//    nID           -- contains the command ID
//    nCode         -- identifies the command notification code
//    pExtra        -- used according to the value of nCode
//    pHandlerInfo  -- if not NULL, filled in with pTarget and pmf
//                     members of CMDHANDLERINFO structure, instead
//                     of dispatching the command.  Typically NULL.
//
// Returns:
//    nonzero if message handled, otherwise 0.
//
// Comments:
//    See the documentation for CCmdTarget::OnCmdMsg() for further
//    information.  Command routing is also discussed in tech note #21.
//
//***********************************************************************
BOOL CDynaMenuDoc::OnCmdMsg(UINT nID, int nCode, void* pExtra,
							AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// If pHandlerInfo is NULL, then handle the message
	if (pHandlerInfo == NULL)
	{
		// Filter the commands sent to a text color menu option
		for (int i = NUM_TEXTCOLOR-1; i >= 0; i--)
		{
			if (nID == m_aColorDef[i].m_nID)
			{
				if (nCode == CN_COMMAND)
				{
					// Handle WM_COMMAND message
					DoSelectColor(i);
				}
				else if (nCode == CN_UPDATE_COMMAND_UI)
				{
					// Update UI element state
					DoUpdateSelectColor(i, (CCmdUI*)pExtra);
				}
				return TRUE;
			}
		}
	}

	// If we didn't process the command, call the base class
	// version of OnCmdMsg so the message-map can handle the message
	return CDocument::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

//***********************************************************************
// Function: CDynaMenuDoc::DoSelectColor()
//
// Purpose:
//    DoSelectColor processes text color command messages for
//    CDynaMenuDoc::OnCmdMsg(). It updates the document member variable
//    m_iColor and calls UpdateAllViews() to redraw the document's
//    views.
//
// Parameters:
//    iColor -- index into CDynaMenuDoc::m_aColorDef for new
//              color selection
//
// Returns:
//    none.
//
// Comments:
//    In the debug version, an assert is generated if the index is
//    out of bounds or if the specified color is not currently
//    enabled for the document.
//
//***********************************************************************
void CDynaMenuDoc::DoSelectColor(int iColor)
{
	ENSURE(iColor >= 0 && iColor < NUM_TEXTCOLOR);
	ASSERT(m_abAllowColor[iColor] == TRUE);

	m_iColor = iColor;
	UpdateAllViews(NULL);
}

//***********************************************************************
// Function: CDynaMenuDoc::DoUpdateSelectColor()
//
// Purpose:
//    DoUpdateSelectColor handled the updates to user-interface objects
//    associated with text color command messages for
//    CDynaMenuDoc::OnCmdMsg(). If the item is associated with the
//    currently selected text color for the document, the item is checked.
//
// Parameters:
//    iColor -- index into CDynaMenuDoc::m_aColorDef for new
//              color selection
//
// Returns:
//    none.
//
// Comments:
//    In the debug version, an assert is generated if the index is
//    out of bounds or if the item is disabled for the document.
//
//***********************************************************************
void CDynaMenuDoc::DoUpdateSelectColor(int iColor, CCmdUI* pCmdUI)
{
	ENSURE(iColor >= 0 && iColor < NUM_TEXTCOLOR);
	ASSERT(m_abAllowColor[iColor] == TRUE);

	pCmdUI->SetCheck(m_iColor == iColor);
	pCmdUI->Enable(TRUE);
}

//***********************************************************************
// Function: CDynaMenuDoc::GetMessageString()
//
// Purpose:
//    GetMessageString formats and returns a string containing the
//    message text to display in a status bar for any of the color
//    selection commands.
//
// Parameters:
//    nID        -- command ID to get message for
//    strMessage -- buffer to fill with message
//
// Returns:
//    TRUE if nID is a color selection command ID, otherwise FALSE.
//
// Comments:
//    If the function returns FALSE, strMessage is not changed
//
//***********************************************************************
BOOL CDynaMenuDoc::GetMessageString(UINT nID, CString& strMessage)
{
	for (int i = 0; i < NUM_TEXTCOLOR; i++)
	{
		if (m_aColorDef[i].m_nID == nID)
		{
			CString strColor;
			strColor.LoadString(m_aColorDef[i].m_nString);
			AfxFormatString1(strMessage, IDS_COLORHELPFMT, strColor);
			return TRUE;
		}
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CDynaMenuDoc commands

//***********************************************************************
// Function: CDynaMenuDoc::OnColorOptions()
//
// Purpose:
//    OnColorOptions displays a dialog which allows the user to pick
//    the text colors which should be displayed on the Color submenu
//    when the document is active.
//
// Parameters:
//    none.
//
// Returns:
//    none.
//
// Comments:
//    none.
//
//***********************************************************************
void CDynaMenuDoc::OnColorOptions()
{
	CColorOptDlg dlg(this);
	if (dlg.DoModal())
	{
		// find the MDI frame window associated with this document
		POSITION pos = GetFirstViewPosition();
		CView* pview = GetNextView(pos);
		ENSURE(pview != NULL);

		CDynaMDIChildWnd* pwnd = (CDynaMDIChildWnd*)pview->GetParentFrame();
		ASSERT_KINDOF(CDynaMDIChildWnd, pwnd);

		// force an update of the Color submenu
		pwnd->RefreshColorMenu();
	}
}
