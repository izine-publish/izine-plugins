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



Public Module VS_CodeCleanup

	Public Const kFnStartCol As Integer = 25

	Public Const kFnParamTypeStartCol As Integer = kFnStartCol + 4

	Public Const kFnParamNameStartCol As Integer = 57



	'Select the lines that you want to sort

	Sub SortIncludeStatements()

		Dim selectedText As String

		Dim inputSelectionStartLine As Integer

		Dim inputSelectionStartCol As Integer

		Dim inputSelectionEndLine As Integer

		Dim inputSelectionEndCol As Integer



		Dim currDoc As Document = DTE.ActiveDocument

		selectedText = currDoc.Selection.Text

		If (selectedText Is Nothing) Then Exit Sub

		If (selectedText.Length <= 0) Then Exit Sub



		GetSelection(currDoc, inputSelectionStartLine, inputSelectionStartCol, inputSelectionEndLine, inputSelectionEndCol)

		If (inputSelectionStartLine = inputSelectionEndLine) Then Exit Sub



		DTE.UndoContext.Open("#Include Sorting")



		currDoc.Selection.text = VS_String.SortLines(selectedText)

		DTE.UndoContext.Close()

	End Sub



	Sub SetIndentLevelOfLine(ByVal inDoc As EnvDTE.Document _

	, Optional ByVal inIndentLevel As Integer = 1)

		'

		'Sets the code indentation to the given level, works only with single line.

		'

		With inDoc.Selection

			.EndOfLine()

			.StartOfLine(vsStartOfLineOptions.vsStartOfLineOptionsFirstText)

			.StartOfLine(vsStartOfLineOptions.vsStartOfLineOptionsFirstColumn, True)

			.Indent(inIndentLevel)

		End With

	End Sub



	Sub IndentToColumn(ByVal inDoc As EnvDTE.Document _

	, ByVal inIndentColumn As Integer)

		'

		' indent column should be multiple of 4 + 1, otherwise spaces get inserted.

		'

		Dim tempStr As String

		With inDoc.Selection

			Dim currCol As Integer

			currCol = VS_Selection.GetSelStartCol(inDoc)

			If (currCol = 1) Then .Text = vbTab

			.WordRight(True)

			If (VS_Selection.GetSelStartLine(inDoc) <> VS_Selection.GetSelEndLine(inDoc)) Then

				.CharLeft()	'selection is in multiple line, so no word after current column, so move left

			Else

				tempStr = .Text

				Dim trimmedText As String

				trimmedText = tempStr.Trim()

				' Delete if only spaces are selected and not till endofline

				If (Len(trimmedText) <= 0 And Not tempStr.EndsWith(vbNewLine)) Then .Delete()

				tempStr = .Text

				' If endofline gets selected, it means there is not text after current column

				If (tempStr.EndsWith(vbNewLine)) Then .CharLeft()

			End If

			.PadToColumn(inIndentColumn)

		End With

	End Sub



	Function GetNextIndentCol(ByVal inCol As Integer) As Integer

		If ((inCol Mod 4) <> 1) Then

			inCol = ((inCol + 3) \ 4) * 4 + 1

		End If

		GetNextIndentCol = inCol

	End Function

End Module

