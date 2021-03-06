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



Public Module VS_Editor



	' Copy the header file name and put the cursor where you want to include the file

	' Cursor can be in a new line, whitespaces will be removed

	' or it can be on the line after which you want to paste

	Sub HashIncludeFromClipboard()

		Dim fileName As String

		Dim currLine As String



		fileName = VS_Clipboard.ClipboardText

		If (fileName Is Nothing) Then Exit Sub

		If (fileName.Length <= 0) Then Exit Sub



		DTE.UndoContext.Open("#Include " + fileName)

		AddHashIncludeStatement(DTE.ActiveDocument, fileName)

		DTE.UndoContext.Close()

	End Sub



	Sub AddHeaderHashIf()

		Dim bUndoOpened As Boolean

		bUndoOpened = DTE.UndoContext.IsOpen

		If (Not bUndoOpened) Then DTE.UndoContext.Open("Add Header #If")

		With DTE.ActiveDocument.Selection

			Dim defineStr As String

			defineStr = "_h_" + VS_Document.GetFileNameAsClass(DTE.ActiveDocument) + "_"

			VS_EditHelper.StartNewLineIfNotEmpty(DTE.ActiveDocument)

			.Text = "#ifndef " + defineStr + vbNewLine

			.Text = "#define " + defineStr + vbNewLine

			.Text = "#pragma once" + vbNewLine + vbNewLine

			.EndOfDocument()

			.Text = "#endif //" + defineStr + vbNewLine



		End With

		If (Not bUndoOpened) Then DTE.UndoContext.Close()

	End Sub



	Sub AddEmptyDoWhileLoop()

		Dim bUndoOpened As Boolean

		bUndoOpened = DTE.UndoContext.IsOpen

		If (Not bUndoOpened) Then DTE.UndoContext.Open("Empty DoWhile Loop")

		Dim currDoc As Document = DTE.ActiveDocument

		With currDoc.Selection

			VS_EditHelper.StartNewLineIfNotEmpty(currDoc)

			VS_CodeCleanup.IndentToColumn(currDoc, 5)

			.Text = "do"

			.NewLine()

			.Text = "{"

			.NewLine()

			.Text = "}while(kFalse);"

			.NewLine()

			.LineUp()

			.LineUp()

			.EndOfLine()

			.NewLine()

		End With

		If (Not bUndoOpened) Then DTE.UndoContext.Close()

	End Sub



	Sub PasteFileNameAsClassName()

		DTE.ActiveDocument.Selection.Text = VS_Document.GetFileNameAsClass(DTE.ActiveDocument)

	End Sub



	Sub AddEmptyClassDefinition()

		Dim className As String

		Dim bUndoOpened As Boolean

		bUndoOpened = DTE.UndoContext.IsOpen

		Dim currDoc As Document = DTE.ActiveDocument

		className = VS_Document.GetFileNameAsClass(currDoc)

		If (Not bUndoOpened) Then DTE.UndoContext.Open("Empty Class Definition")

		With DTE.ActiveDocument.Selection

			VS_EditHelper.StartNewLineIfNotEmpty(currDoc)

			VS_ClassHelper.AddEmptyClassDefinition(currDoc, className)

		End With

		If (Not bUndoOpened) Then DTE.UndoContext.Close()

	End Sub



	Sub AddConstructorDestructorDefinition()

		Dim className As String

		Dim bUndoOpened As Boolean

		bUndoOpened = DTE.UndoContext.IsOpen

		Dim currDoc As Document = DTE.ActiveDocument

		className = VS_Document.GetFileNameAsClass(currDoc)

		If (Not bUndoOpened) Then DTE.UndoContext.Open("Class Constructor Destructor")

		With DTE.ActiveDocument.Selection

			VS_EditHelper.StartNewLineIfNotEmpty(currDoc)

			VS_ClassHelper.AddDefaultConstructorDefinition(currDoc, className)

			VS_ClassHelper.AddDestructorDefinition(currDoc, className)

			.Text = "#pragma mark -"

			.NewLine()

		End With

		If (Not bUndoOpened) Then DTE.UndoContext.Close()

	End Sub



	' Copy the function signatue and place the cursor where you want to

	' create the function definition

	Sub CreateFuncDefinitionFromClipboard()

		Dim functionDeclarationStr As String



		functionDeclarationStr = VS_Clipboard.ClipboardText

		If (functionDeclarationStr Is Nothing) Then Exit Sub

		functionDeclarationStr = functionDeclarationStr.Trim

		If (functionDeclarationStr.Length <= 0) Then Exit Sub



		DTE.UndoContext.Open("Paste Function Definition")

		CreateFunctionDefinition(DTE.ActiveDocument, functionDeclarationStr)

		DTE.UndoContext.Close()

	End Sub





	Sub CreateFuncDefinition()

		'

		' Place the cursor in the first line of function declaration

		' this will create the function definition

		'

		Dim endLine As Integer



		DTE.UndoContext.Open("Create Function Definition")

		CreateFunctionDefinition(DTE.ActiveDocument, VS_Selection.GetSelStartLine(DTE.ActiveDocument), endLine)

		DTE.UndoContext.Close()

	End Sub



	Sub CreateFunctionGetSetDeclarationFromClipboard()

		Dim variableDeclarationStr As String



		variableDeclarationStr = VS_Clipboard.ClipboardText

		If (variableDeclarationStr Is Nothing) Then Exit Sub

		variableDeclarationStr = variableDeclarationStr.Trim

		If (variableDeclarationStr.Length <= 0) Then Exit Sub



		DTE.UndoContext.Open("Paste Function Declarations")

		VS_ClassHelper.CreateFunctionGetSetDeclaration(DTE.ActiveDocument, variableDeclarationStr)

		DTE.UndoContext.Close()

	End Sub



	Sub AddDate(ByVal inDoc As EnvDTE.Document)

		inDoc.Selection.Text = VS_Utils.GetDateAsString

	End Sub



	'Trims trailing spaces from the selected text

	Sub TrimEndingSpaces(Optional ByVal startLine As Integer = 0, Optional ByVal endLine As Integer = 0)

		Dim currLine As Integer

		Dim bUndoOpened As Boolean

		bUndoOpened = DTE.UndoContext.IsOpen

		If (Not bUndoOpened) Then DTE.UndoContext.Open("Trim Ending Spaces")

		VS_EditHelper.TrimEndingSpaces(DTE.ActiveDocument, startLine, endLine)

		If (Not bUndoOpened) Then DTE.UndoContext.Close()

	End Sub



	'Swap the current line text with next line text

	Sub SwapLineTextWithNextLine()

		Dim bUndoOpened As Boolean

		bUndoOpened = DTE.UndoContext.IsOpen

		If (Not bUndoOpened) Then DTE.UndoContext.Open("Swap Lines with Next")

		VS_EditHelper.SwapLineTextWithNextLine(DTE.ActiveDocument)

		If (Not bUndoOpened) Then DTE.UndoContext.Close()

	End Sub

	'Swap the current line text with previous line text

	Sub SwapLineTextWithPreviousLine()

		Dim bUndoOpened As Boolean

		bUndoOpened = DTE.UndoContext.IsOpen

		If (Not bUndoOpened) Then DTE.UndoContext.Open("SwapLines with Previous")

		VS_EditHelper.SwapLineTextWithPreviousLine(DTE.ActiveDocument)

		If (Not bUndoOpened) Then DTE.UndoContext.Close()

	End Sub



	'Comments the selection in C++ style

	Sub Comment(ByVal inDoc As EnvDTE.Document _

	, Optional ByVal startLine As Integer = 0, Optional ByVal endLine As Integer = 0)

		DTE.UndoContext.Open("Comment Lines")

		VS_EditHelper.CommentLines(DTE.ActiveDocument, startLine, endLine)

		DTE.UndoContext.Close()

	End Sub

	'UnComments the selection in C++ style

	Sub UnComment(ByVal inDoc As EnvDTE.Document _

	, Optional ByVal startLine As Integer = 0, Optional ByVal endLine As Integer = 0)

		DTE.UndoContext.Open("UnComment Lines")

		VS_EditHelper.UnCommentLines(DTE.ActiveDocument, startLine, endLine)

		DTE.UndoContext.Close()

	End Sub



End Module

