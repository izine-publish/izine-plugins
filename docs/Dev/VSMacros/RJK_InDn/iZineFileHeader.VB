'//========================================================================================

'//	

'//	$HeadURL: $

'//	$Revision: $

'//	$Date: $

'//	$Author: $

'//	

'//	Creator: Raj Kumar Sehrawat

'//	Created: 22-3-2010

'//	

'//	Description:

'//========================================================================================

Option Explicit On



Imports System

Imports EnvDTE

Imports EnvDTE80

Imports System.Diagnostics



Public Module iZineFileHeader



	Sub AddFileHeader()

		Dim newFieldPrefix As String

		newFieldPrefix = vbNewLine + "//" + vbNewLine + "//  "



		DTE.UndoContext.Open("Add File Header")

		With DTE.ActiveDocument.Selection

			.GoToLine(1)

			.StartOfLine(vsStartOfLineOptions.vsStartOfLineOptionsFirstColumn)

			.Text = GetFileHeaderString("Raj Kumar Sehrawat")

		End With

		DTE.UndoContext.Close()



	End Sub



	Function FunctionHeaderCommentLine() As String

		FunctionHeaderCommentLine = "//----------------------------------------------------------------------------------------"

	End Function





	Function GetFileHeaderString(ByVal inCreator As String, Optional ByVal inFileDesc As String = "") As String

		Dim headerString As String

		headerString = headerString + "//========================================================================================" + vbNewLine

		headerString = headerString + "//	" + vbNewLine

		headerString = headerString + "//	$" + "HeadURL: $" + vbNewLine

		headerString = headerString + "//	$" + "Revision: $" + vbNewLine

		headerString = headerString + "//	$" + "Date: $" + vbNewLine

		headerString = headerString + "//	$" + "Author: $" + vbNewLine

		headerString = headerString + "//	" + vbNewLine

		headerString = headerString + "//	Creator: " & inCreator + vbNewLine

		headerString = headerString + "//	Created: " & VS_Utils.GetDateAsString() + vbNewLine

		headerString = headerString + "//	Copyright: 2008-2011 iZine Publish. All rights reserved." + vbNewLine

		headerString = headerString + "//	" + vbNewLine

		headerString = headerString + "//	Description:" + inFileDesc + vbNewLine

		headerString = headerString + "//========================================================================================" + vbNewLine

		GetFileHeaderString = headerString

	End Function



	Function GetImplIncludeComments() As String

		Dim toReturn As String

		toReturn = toReturn + "#include ""VCPlugInHeaders.h""" + vbNewLine + vbNewLine

		toReturn = toReturn + "//SDK Interfaces" + vbNewLine + vbNewLine

		toReturn = toReturn + "//SDK General includes" + vbNewLine + vbNewLine

		toReturn = toReturn + "//OS / ThirdParty includes" + vbNewLine + vbNewLine

		toReturn = toReturn + "#include ""IZPID.h""" + vbNewLine + vbNewLine

		toReturn = toReturn + "//IZP Interfaces" + vbNewLine + vbNewLine

		toReturn = toReturn + "//IZP General includes" + vbNewLine + vbNewLine

		toReturn = toReturn + "#pragma mark -" + vbNewLine + vbNewLine

		GetImplIncludeComments = toReturn

	End Function





	'searches for :: and copies the function name to function header

	Sub CreateFuncHeader()

		Dim functionName As String

		Dim bUndoOpened As Boolean

		bUndoOpened = DTE.UndoContext.IsOpen

		If (Not bUndoOpened) Then DTE.UndoContext.Open("Create Function Header")

		DTE.Find.FindWhat = "::"

		DTE.ExecuteCommand("Edit.FindNext")

		With DTE.ActiveDocument.Selection

			.CharRight()

			.WordRight(True)

			functionName = .text

			.LineUp()

			.StartOfLine(vsStartOfLineOptions.vsStartOfLineOptionsFirstText)

			.NewLine()

			.LineUp()

			.Text = FunctionHeaderCommentLine()

			.NewLine()

			.Text = "// "

			.Text = functionName

			.NewLine()

			.Text = FunctionHeaderCommentLine()

			.StartOfLine(vsStartOfLineOptions.vsStartOfLineOptionsFirstColumn)

		End With

		If (Not bUndoOpened) Then DTE.UndoContext.Close()

	End Sub

End Module



