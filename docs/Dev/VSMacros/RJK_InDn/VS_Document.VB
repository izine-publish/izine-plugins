'//========================================================================================

'//	

'//	$HeadURL: $

'//	$Revision: $

'//	$Date: $

'//	$Author: $

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



Friend Module VS_Document



	' Get the file name after removing the extension

	' spaces are replaced with underscore

	Function GetFileNameAsClass(ByVal inDoc As EnvDTE.Document _

	, Optional ByVal includeExt As Boolean = False) As String



		Dim className As String

		className = GetFileName(inDoc)

		'Replace space with underscore

		className = className.Replace(Chr(32), Chr(95))

		GetFileNameAsClass = className

	End Function



	' Get file name

	Function GetFileName(ByVal inDoc As EnvDTE.Document _

	, Optional ByVal includeExt As Boolean = False) As String



		Dim fileName As String

		Dim extPos As Integer



		With inDoc

			fileName = .Name

			If (includeExt) Then



			Else

				extPos = fileName.LastIndexOf(".")

				fileName = fileName.Substring(0, extPos)

			End If

		End With



		GetFileName = fileName

	End Function



	'Get Current line text - all

	Function GetCurrentLineText(ByVal inDoc As EnvDTE.Document) As String

		With inDoc.Selection

			.StartOfLine(vsStartOfLineOptions.vsStartOfLineOptionsFirstColumn)

			.EndOfLine(True)

			GetCurrentLineText = .Text

		End With

	End Function

End Module

