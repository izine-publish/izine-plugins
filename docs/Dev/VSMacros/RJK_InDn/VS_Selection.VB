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



Public Module VS_Selection

	'select word starting or having given column number

	Sub SelectWordInLine( _

	 ByVal inDoc As EnvDTE.Document _

	 , ByVal inLine As Integer _

	 , ByVal atCol As Integer)



		inDoc.Selection.MoveToLineAndOffset(inLine, atCol)

		inDoc.Selection.WordRight(False, 1)

		inDoc.Selection.WordLeft(True, 1)



	End Sub



	Public Sub SelectCurrentCompleteLine(ByVal inDoc As EnvDTE.Document)

		'

		'Select a complete line

		'

		inDoc.Selection.startofline(EnvDTE.vsStartOfLineOptions.vsStartOfLineOptionsFirstColumn)

		inDoc.Selection.EndOfLine(True)

	End Sub



	' Return the line number of selection start

	Function GetSelStartLine( _

	 ByVal inDoc As EnvDTE.Document) As Integer



		GetSelStartLine = inDoc.Selection.TextRanges.Item(1).StartPoint.Line

	End Function



	' Return the line number of selection start

	Function GetSelEndLine( _

	 ByVal inDoc As EnvDTE.Document) As Integer



		Dim startLine As Integer

		With inDoc.Selection

			startLine = .TextRanges.Item(1).StartPoint.Line

			Dim rangesCount = 1

			rangesCount = .TextRanges.Count()

			GetSelEndLine = startLine + rangesCount - 1

		End With

	End Function



	' Return the column number of selection start

	Function GetSelStartCol( _

	 ByVal inDoc As EnvDTE.Document) As Integer



		GetSelStartCol = inDoc.Selection.TextRanges.Item(1).StartPoint.LineCharOffset

	End Function

	' Return the column number of selection start

	Function GetSelEndCol( _

	 ByVal inDoc As EnvDTE.Document) As Integer



		'GetSelEndCol = inDoc.Selection.TextRanges.Item(1).EndPoint.LineCharOffset

		Dim endCol As Integer

		With inDoc.Selection

			endCol = .TextRanges.Item(.textranges.count()).EndPoint.Linecharoffset

			GetSelEndCol = endCol

		End With

	End Function



	'Get selection range

	Sub GetSelection(ByVal inDoc As EnvDTE.Document _

	, ByRef oStartLine As Integer, ByRef oStartCol As Integer, ByRef oEndLine As Integer, ByRef oEndCol As Integer)



		oStartLine = GetSelStartLine(inDoc)

		oStartCol = GetSelStartCol(inDoc)

		oEndLine = GetSelEndLine(inDoc)

		oEndCol = GetSelEndCol(inDoc)



	End Sub

	' Get Selected line start and end

	Sub GetSelectionStartEnd(ByRef startLine As Integer, ByRef endLine As Integer)

		GetSelectionStartEnd_InDoc(DTE.ActiveDocument, startLine, endLine)

	End Sub



	' Get Selected line start and end from given document

	Sub GetSelectionStartEnd_InDoc(ByVal inDoc As EnvDTE.Document _

	 , ByRef startLine As Integer, ByRef endLine As Integer)

		With inDoc.Selection

			startLine = .TextRanges.Item(1).StartPoint.Line

			Dim rangesCount = 1

			rangesCount = .TextRanges.Count()

			endLine = startLine + rangesCount - 1

		End With

	End Sub



	Sub SetSelection(ByVal inDoc As EnvDTE.Document _

	, ByVal inStartLine As Integer, ByVal inStartCol As Integer)



		inDoc.Selection.MoveToLineAndOffset(inStartLine, inStartCol)

	End Sub



	Sub SetSelection(ByVal inDoc As EnvDTE.Document _

	, ByVal inStartLine As Integer, ByVal inStartCol As Integer, ByVal inEndLine As Integer, ByVal inEndCol As Integer)



		inDoc.Selection.MoveToLineAndOffset(inStartLine, inStartCol)

		inDoc.Selection.MoveToLineAndOffset(inEndLine, inEndCol, True)



	End Sub



	Sub GoToLastHashInclude()



		Dim inDoc As EnvDTE.Document

		inDoc = DTE.ActiveDocument

		If (VS_FindHelper.PerformFindActionExact("#include") = False) Then Exit Sub

		inDoc.Selection.EndOfDocument()

		DTE.ExecuteCommand("Edit.FindPrevious")

		inDoc.Selection.EndOfLine()

		inDoc.Selection.NewLine()

	End Sub

End Module

