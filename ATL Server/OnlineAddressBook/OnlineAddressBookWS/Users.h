// Users.h : Declaration of the CUsers
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#pragma once

// code generated on Tuesday, January 16, 2001, 6:55 PM

[
	db_command("SELECT [Password], [Salt], [FirstName], [LastName],[UserID] FROM Users WHERE [Email]=?")
]
class CLoginUser
{
public:
	[ db_column(ordinal = "1", length ="m_lPassLen") ] BYTE m_Password[DB_MAX_STRLEN+1];
	[ db_column(ordinal = "2", length = "m_lSaltLen") ] BYTE m_Salt[DB_MAX_STRLEN+1];
	[ db_column(ordinal = "3") ] TCHAR m_szFirstName[DB_MAX_STRLEN + 1];
	[ db_column(ordinal = "4") ] TCHAR m_szLastName[DB_MAX_STRLEN + 1];
	[ db_column(ordinal = "5") ] LONG m_lUserID;	
	[ db_param("1") ]  TCHAR m_param_szEmail[DB_MAX_STRLEN + 1];

	DBLENGTH m_lPassLen;
	DBLENGTH m_lSaltLen;

};

[ 
	db_command("INSERT INTO Users ([FirstName], [LastName], [Email], [Password], [Salt]) VALUES (?,?,?,?,?)")
]
class CAddUser
{
public:
	[ db_param("1") ] TCHAR m_szFirstName[DB_MAX_STRLEN + 1];
	[ db_param("2") ] TCHAR m_szLastName[DB_MAX_STRLEN + 1];
	[ db_param("3") ] TCHAR m_szEmail[DB_MAX_STRLEN + 1];
	[ db_param("4", length="m_lPassLen") ] BYTE m_Password[DB_MAX_STRLEN+1];
	[ db_param("5", length="m_lSaltLen") ] BYTE m_Salt[DB_MAX_STRLEN+1];

	DBLENGTH m_lPassLen;
	DBLENGTH m_lSaltLen;
};

