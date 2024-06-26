// CalendarDB.h : Defines the database classes for the schedule
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#pragma once

[
	db_command("")
]
class CCalendarAppointments
{
public:
	[ db_column(ordinal="1") ] LONG m_lAppointmentID;
	[ db_column(ordinal="2") ] TCHAR m_szTitle[DB_MAX_STRLEN + 1];
	[ db_column(ordinal="3") ] DATE m_Date;
	[ db_param("1") ]  LONG m_lUserID;
};
