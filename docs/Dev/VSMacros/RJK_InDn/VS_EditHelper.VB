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

Option Explicit On



Imports System

Imports EnvDTE

Imports EnvDTE80

Imports System.Diagnostics



Friend Module VS_EditHelper



	' Cursor can be in a new line, whitespaces will be removed

	' or it can be on the line after which you want to paste

	Public Sub StartNewLineIfNotEmpty(ByVal inDoc As EnvDTE.Document _

	, Optional ByVal afterCurrentLine As Boolean = True)

		Dim currLine As String



		With inDoc.Selection

			.StartOfLine(vsStartOfLineOptions.vsStartOfLineOptionsFirstText)

			.EndOfLine(True)

			currLine = .Text

			currLine.Trim()

			If (currLine.Length() > 0) Then

				If (afterCurrentLine) Then

					.EndOfLine()

				Else

					.StartOfLine()

				End If

				.NewLine()

			End If

			.StartOfLine(vsStartOfLineOptions.vsStartOfLineOptionsFirstText)

			.EndOfLine(True)

			.Text = ""

		End With

	End Sub





	Sub DeleteFirstLine(ByVal inDoc As EnvDTE.Document)

		inDoc.Selection.StartOfDocument()

		inDoc.Selection.LineDown(True)

		inDoc.Selection.Delete()

	End Sub

	Sub DeleteLastLine(ByVal inDoc As EnvDTE.Document)

		inDoc.Selection.EndOfDocument()

		inDoc.Selection.LineUp(True)

		inDoc.Selection.Delete()

	End Sub



	Public Sub DeleteCurrentLine(ByVal inDoc As EnvDTE.Document _

	, Optional ByVal inOnlyIfEmpty As Boolean = False)

		Dim lineText As String

		With inDoc.Selection

			'select complete line

			VS_Selection.SelectCurrentCompleteLine(inDoc)

			'check if it has any non whitespace char

			If (inOnlyIfEmpty) Then

				lineText = .Text

				If (Len(Trim(lineText)) > 0) Then Exit Sub

			End If

			'Delete the line

			.delete()

			.delete()

		End With

	End Sub



	Public Sub DeleteLinesContainingWord( _

	   ByVal inDoc As EnvDTE.Document _

	   , ByVal inFindStr As String _

	   , ByVal inDeleteAll As Boolean _

	   , ByVal inExcludeLine As Integer)



		Dim currFindString As String



		currFindString = DTE.Find.FindWhat	'restore it later on

		Dim foundLine As Integer



		'Store the current line and column

		Do

			If (VS_FindHelper.PerformFindActionExact(inFindStr)) Then



				foundLine = VS_Selection.GetSelStartLine(inDoc)

				If (foundLine = inExcludeLine) Then

					Exit Do

				Else

					If foundLine < inExcludeLine Then

						inExcludeLine = inExcludeLine - 1

					End If

					'Delete the current line

					DeleteCurrentLine(inDoc)

				End If

			Else

				Exit Do

			End If

		Loop



		DTE.Find.FindWhat = currFindString

	End Sub



	Sub ReplaceCurrentLineText(ByVal inDoc As EnvDTE.Document, ByVal lineText As String)

		With inDoc.Selection

			Dim lineOldText As String

			.StartOfLine(vsStartOfLineOptions.vsStartOfLineOptionsFirstColumn)

			.Text = lineText

			.EndOfLine(True)

			lineOldText = .Text

			If (lineOldText.Length() > 0) Then .Delete()

			'.StartOfLine(vsStartOfLineOptions.vsStartOfLineOptionsFirstColumn)

		End With

	End Sub



	' swap current line text with next line, must not have tabs

	Sub SwapLineTextWithNextLine(ByVal inDoc As EnvDTE.Document)

		Dim firstLine As String

		With inDoc.Selection

			.StartOfLine(vsStartOfLineOptions.vsStartOfLineOptionsFirstText)

			.EndOfLine(True)

			firstLine = .Text

			.DeleteLeft()

			.Delete()

			.EndOfLine(False)

			.NewLine()

			.Text = firstLine

		End With

	End Sub

	' swap current line text with previous line, must not have tabs

	Sub SwapLineTextWithPreviousLine(ByVal inDoc As EnvDTE.Document)

		Dim firstLine As String

		With inDoc.Selection

			.StartOfLine(vsStartOfLineOptions.vsStartOfLineOptionsFirstText)

			.EndOfLine(True)

			firstLine = .Text

			.DeleteLeft()

			.delete()

			.Lineup()

			.StartOfLine(vsStartOfLineOptions.vsStartOfLineOptionsFirstText)

			.newline()

			.Lineup()

			.Text = firstLine



		End With

	End Sub



	'Trims trailing spaces from the selected text

	Sub TrimEndingSpaces(ByVal inDoc As EnvDTE.Document _

	, Optional ByVal startLine As Integer = 0, Optional ByVal endLine As Integer = 0)

		Dim currLine As Integer

		With inDoc.Selection

			If (startLine = 0 And endLine = 0) Then VS_Selection.GetSelectionStartEnd_InDoc(inDoc, startLine, endLine)

			currLine = startLine

			Do While currLine <= endLine

				.GoToLine(currLine, False)

				.StartOfLine(vsStartOfLineOptions.vsStartOfLineOptionsFirstColumn)

				.EndOfLine(True)

				.Text = .Text.TrimEnd()

				currLine += 1

			Loop

			.GoToLine(startLine, False)

		End With

	End Sub



	'Add #include statement at the current or after current line

	Sub AddHashIncludeStatement(ByVal inDoc As EnvDTE.Document _

	 , ByVal inFileName As String)

		With inDoc.Selection

			VS_EditHelper.StartNewLineIfNotEmpty(inDoc)

			.Text = "#include """

			inFileName = inFileName.Trim()

			.Text = inFileName

			If (inFileName.EndsWith(".h") = False) Then .Text = ".h"

			.Text = """"

			.EndOfLine()

		End With

	End Sub

	'Comments the selection in C++ style

	Sub CommentLines(ByVal inDoc As EnvDTE.Document _

	, Optional ByVal startLine As Integer = 0, Optional ByVal endLine As Integer = 0)

		Dim currLine As Integer

		With inDoc.Selection

			If (startLine = 0 And endLine = 0) Then GetSelectionStartEnd(startLine, endLine)

			currLine = startLine

			Do While currLine <= endLine

				.GoToLine(currLine, False)

				.StartOfLine(vsStartOfLineOptions.vsStartOfLineOptionsFirstColumn)

				.text = "//"

				currLine += 1

			Loop

		End With

	End Sub

	'UnComments the selection in C++ style

	Sub UnCommentLines(ByVal inDoc As EnvDTE.Document _

	, Optional ByVal startLine As Integer = 0, Optional ByVal endLine As Integer = 0)

		Dim currLine As Integer

		Dim currLineText As String

		Dim firstTwoChars As String



		With inDoc.Selection

			If (startLine = 0 And endLine = 0) Then GetSelectionStartEnd(startLine, endLine)

			currLine = startLine

			Do While currLine <= endLine

				.GoToLine(currLine, False)

				.StartOfLine(vsStartOfLineOptions.vsStartOfLineOptionsFirstText)

				.EndOfLine(True)

				currLineText = .text

				currLineText.Trim()

				If (currLineText.Length >= 2) Then

					firstTwoChars = currLineText.Substring(0, 2)

					If (firstTwoChars = "//") Then

						.StartOfLine(vsStartOfLineOptions.vsStartOfLineOptionsFirstText)

						.Delete(2)

					End If

				End If

				currLine += 1

			Loop

		End With

	End Sub



End Module



