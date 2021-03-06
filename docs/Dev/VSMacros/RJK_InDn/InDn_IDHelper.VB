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



Public Module InDn_IDHelper

	Private Const IDIndentColumn As Integer = 109

	Sub NewInDnID()

		'

		'Create a new ID declaration from the current line.

		' Only the new ID variable name should be present in current line

		'

		Dim newIDToCreate As String

		Dim bUndoOpened As Boolean



		bUndoOpened = DTE.UndoContext.IsOpen

		If (Not bUndoOpened) Then DTE.UndoContext.Open("New InDesign ID")

		Dim currDoc As Document = DTE.ActiveDocument

		With currDoc.Selection

			newIDToCreate = VS_Document.GetCurrentLineText(currDoc)

			CreateNewInDnIDDeclaration(currDoc, newIDToCreate)

		End With

		If (Not bUndoOpened) Then DTE.UndoContext.Close()

	End Sub



	'Create a new ID declaration from the variable copied to clipboard.

	Sub PasteAsInDnID()

		Dim newIDToCreate As String

		Dim bUndoOpened As Boolean



		newIDToCreate = VS_Clipboard.ClipboardText

		If (newIDToCreate Is Nothing) Then Exit Sub

		newIDToCreate = newIDToCreate.Trim

		If (VS_String.IsValidVariableName(newIDToCreate) = False) Then Exit Sub



		bUndoOpened = DTE.UndoContext.IsOpen

		If (Not bUndoOpened) Then DTE.UndoContext.Open("Paste InDesign ID")

		VS_EditHelper.StartNewLineIfNotEmpty(DTE.ActiveDocument)

		CreateNewInDnIDDeclaration(DTE.ActiveDocument, newIDToCreate)

		If (Not bUndoOpened) Then DTE.UndoContext.Close()

	End Sub



	'Create a new InDesign ID declaration similar to the previous line

	Sub CreateNewInDnIDDeclaration(ByVal inDoc As EnvDTE.Document _

	 , ByVal inNewIDVariable As String)



		Dim newIDLineNumber As Integer

		Dim newDeclarationStr As String

		With inDoc.Selection

			newIDLineNumber = .TextRanges.Item(1).StartPoint.Line

			newDeclarationStr = GetNextInDnIDFromPreviousLine(inDoc)

			.GoToLine(newIDLineNumber)

			VS_EditHelper.ReplaceCurrentLineText(inDoc, newDeclarationStr)

			SelectInDnIDInCurrentLine(inDoc)

			.Text = inNewIDVariable

			.CharRight()		'comma after id variable



			'indent prefix to 109 column

			VS_CodeCleanup.IndentToColumn(inDoc, IDIndentColumn)

		End With

	End Sub



	'Update InDn ID from previous ID value

	Sub UpdateInDnID()

		Dim oldIDToUpdate As String

		Dim oldIDLineNumber As Integer

		Dim newDeclarationStr As String

		Dim bUndoOpened As Boolean



		bUndoOpened = DTE.UndoContext.IsOpen

		If (Not bUndoOpened) Then DTE.UndoContext.Open("Update InDesign ID")

		Dim currDoc As Document = DTE.ActiveDocument

		With currDoc.Selection

			oldIDLineNumber = .TextRanges.Item(1).StartPoint.Line

			SelectInDnIDInCurrentLine(currDoc)

			oldIDToUpdate = .Text

			VS_EditHelper.ReplaceCurrentLineText(currDoc, "")	  'Remove the current line

			newDeclarationStr = GetNextInDnIDFromPreviousLine(currDoc)

			.GoToLine(oldIDLineNumber)

			VS_EditHelper.ReplaceCurrentLineText(currDoc, newDeclarationStr)

			SelectInDnIDInCurrentLine(currDoc)

			.Text = oldIDToUpdate

			.CharRight()		'comma after id variable



			'indent prefix to 109 column

			VS_CodeCleanup.IndentToColumn(currDoc, IDIndentColumn)



			InDn_FindHelper.FindInDnIDDeclaration(True)

		End With

		If (Not bUndoOpened) Then DTE.UndoContext.Close()

	End Sub



	'Get ID vaue from previous declaration and increment it by one

	' return the full declaration string

	Private Function GetNextInDnIDFromPreviousLine(ByVal inDoc As EnvDTE.Document) As String

		Dim prevLine As String

		Dim lastPlusIndex As Integer

		Dim lastBracketIndex As Integer

		Dim prevIDStr As String

		Dim newIDValStr As String

		Dim newID As Integer

		Dim incrementedID As String



		InDn_FindHelper.FindInDnIDDeclaration()



		prevLine = VS_Document.GetCurrentLineText(inDoc)

		'Get ID from previous line

		lastPlusIndex = prevLine.LastIndexOf("+")

		lastBracketIndex = prevLine.LastIndexOf(")")

		lastPlusIndex = lastPlusIndex + 1

		prevIDStr = prevLine.Substring(lastPlusIndex, lastBracketIndex - lastPlusIndex)

		newID = Int(prevIDStr)

		newID += 1

		newIDValStr = newID

		incrementedID = prevLine.Substring(0, lastPlusIndex)

		incrementedID = incrementedID + " " + newIDValStr + " )"

		GetNextInDnIDFromPreviousLine = incrementedID

	End Function



	Private Sub SelectInDnIDInCurrentLine(ByVal inDoc As EnvDTE.Document)

		With inDoc.Selection

			.endofline(False)

			InDn_FindHelper.FindInDnIDDeclaration()

			.WordRight(False, 3)

			.WordRight(True)

		End With

	End Sub





	' Deletes all lines from the current document that contains the word

	' which is currently selected or where the text cursor is.

	' The current line is not deleted.

	Public Sub DeleteAllLinesOfCurrentWord(ByVal inDoc As EnvDTE.Document)



		Dim currSelection As TextSelection

		Dim startLine As Integer

		Dim startCol As Integer

		Dim selectedWord As String



		'Store the current line and column

		currSelection = inDoc.Selection

		startLine = VS_Selection.GetSelStartLine(inDoc)

		startCol = VS_Selection.GetSelStartCol(inDoc)



		VS_Selection.SelectWordInLine(inDoc, startLine, startCol)

		selectedWord = inDoc.Selection.Text

		selectedWord = selectedWord.Trim()



		If (selectedWord.Length > 0) Then

			DeleteLinesContainingWord(inDoc, selectedWord, True, startLine)

		End If



	End Sub



End Module



