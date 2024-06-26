SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

/* Run this script in the pubs database that ships with SQL Server.  This script will create a sample */
/* stored procedure that you can use with ATLRT */
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[sp_ShowAuthors]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[sp_ShowAuthors]
GO

CREATE PROCEDURE sp_ShowAuthors

AS

SELECT * FROM AUTHORS
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

