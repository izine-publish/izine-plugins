'//========================================================================================

'//	

'//	$HeadURL:$

'//	$Revision:$

'//	$Date:$

'//	$Author:$

'//	

'//	Creator: Raj Kumar Sehrawat

'//	Created: 19-3-2010

'//	

'//	Description:

'//========================================================================================

Imports System

Imports EnvDTE

Imports EnvDTE80

Imports System.Diagnostics



Friend Module VS_Utils



	Public Function GetDateAsString() As String

		Dim currTime As Date

		Dim dateString As String



		currTime = System.DateTime.Now()

		dateString = currTime.Day() & "-" & currTime.Month() & "-" & currTime.Year()

		GetDateAsString = dateString

	End Function



End Module

