Imports System
Imports EnvDTE
Imports EnvDTE80
Imports System.Diagnostics
Imports System.Text.RegularExpressions

Friend Module VS_ClassHelper

	Sub AddEmptyClassDefinition(ByVal inDoc As EnvDTE.Document _
	, ByVal inClassName As String)
		With inDoc.Selection
			.Text = "class "
			.text = inClassName
			.NewLine()
			.Text = "{"
			.NewLine()
			.Text = "public:"
			.NewLine()
			VS_CodeCleanup.IndentToColumn(inDoc, VS_CodeCleanup.kFnStartCol)
			.text = inClassName
			.Text = "();"
			.NewLine()
			VS_Selection.SelectCurrentCompleteLine(inDoc)
			.Text = vbTab
			.Text = "virtual"
			VS_CodeCleanup.IndentToColumn(inDoc, VS_CodeCleanup.kFnStartCol)
			.Text = "~"
			.text = inClassName
			.Text = "();"
			.NewLine()
			.Text = "protected:"
			.NewLine()
			.Text = "private:"
			.NewLine()
			.Text = "};"
			.NewLine()
		End With
	End Sub

	Sub AddDefaultConstructorDefinition(ByVal inDoc As EnvDTE.Document _
	 , ByVal inClassName As String)
		With inDoc.Selection
			VS_EditHelper.StartNewLineIfNotEmpty(inDoc)
			.Text = FunctionHeaderCommentLine()
			.NewLine()
			.Text = "// Constructor"
			.NewLine()
			.Text = FunctionHeaderCommentLine()
			.NewLine()
			.Text = inClassName
			.Text = "::"
			.Text = inClassName
			.Text = "()"
			AddEmptyFunctionBody(inDoc)
		End With
	End Sub

	Sub AddDestructorDefinition(ByVal inDoc As EnvDTE.Document _
	 , ByVal inClassName As String)
		With inDoc.Selection
			VS_EditHelper.StartNewLineIfNotEmpty(inDoc)
			.Text = FunctionHeaderCommentLine()
			.NewLine()
			.Text = "// Destructor"
			.NewLine()
			.Text = FunctionHeaderCommentLine()
			.NewLine()
			.Text = inClassName
			.Text = "::~"
			.Text = inClassName
			.Text = "()"
			AddEmptyFunctionBody(inDoc)
		End With
	End Sub

	Sub AddEmptyFunctionBody(ByVal inDoc As EnvDTE.Document)
		With inDoc.Selection
			VS_EditHelper.StartNewLineIfNotEmpty(inDoc)
			.Text = "{"
			.NewLine(2)
			.Text = "}"
			.NewLine(2)
		End With
	End Sub

	Sub CreateFunctionDefinition_NotBeingUsed(ByVal inDoc As EnvDTE.Document _
	 , ByVal inFunctionDeclarationStr As String)
		'
		' Create a empty body for a function declaration. and add function header.
		'
		Dim startingLine As Integer
		Dim endingLine As Integer
		Dim lineFound As Integer

		With inDoc.Selection
			VS_EditHelper.StartNewLineIfNotEmpty(inDoc)
			startingLine = VS_Selection.GetSelStartLine(inDoc)
			.text = inFunctionDeclarationStr
			endingLine = VS_Selection.GetSelStartLine(inDoc)
			.gotoline(startingLine)
			.StartOfLine(vsStartOfLineOptions.vsStartOfLineOptionsFirstText)
			PerformFindActionExact("(")
			lineFound = .TextRanges.Item(1).StartPoint.Line
			If (lineFound < startingLine Or lineFound > endingLine) Then Return
			.charright()
			.NewLine()
			VS_EditHelper.DeleteCurrentLine(inDoc, True)
			VS_CodeCleanup.SetIndentLevelOfLine(inDoc, 1)
			.lineup()
			.endofline()
			.WordLeft(False, 2)
			.NewLine()
			.Text = VS_Document.GetFileNameAsClass(inDoc)
			.Text = "::"
			.LineUp()
			endingLine = endingLine + 2		'2 new lines
			PerformFindActionExact(";")
			lineFound = .TextRanges.Item(1).StartPoint.Line
			If (lineFound > endingLine) Then
				.gotoline(endingLine)
				.EndOfLine(False)
			End If
			.NewLine()
			AddEmptyFunctionBody(inDoc)
			.gotoline(startingLine)
			CreateFuncHeader()
			VS_EditHelper.TrimEndingSpaces(inDoc, startingLine, endingLine)
		End With
	End Sub

	Sub CreateFunctionDefinition(ByVal inDoc As EnvDTE.Document _
	  , ByVal inFunctionDeclarationStr As String, Optional ByVal inAddHeader As Boolean = True)
		'
		' Create a empty body for a function declaration. and add function header.
		'
		Dim startingLine As Integer
		Dim endingLine As Integer

		Dim funcName As String
		Dim returnType As String
		Dim functionParams As String
		Dim funcSuffix As String

		With inDoc.Selection
			VS_EditHelper.StartNewLineIfNotEmpty(inDoc)
			startingLine = VS_Selection.GetSelStartLine(inDoc)
			.text = inFunctionDeclarationStr
			CreateFunctionDefinition(inDoc, startingLine, endingLine, inAddHeader)
		End With
	End Sub

	Sub CreateFunctionDefinition(ByVal inDoc As EnvDTE.Document _
	, ByVal inStartLine As Integer, ByRef oEndLine As Integer, Optional ByVal inAddHeader As Boolean = True)
		'
		' Create a empty body for a function declaration. and add function header.
		'
		Dim endingLine As Integer

		Dim funcName As String
		Dim returnType As String
		Dim functionParams As String
		Dim funcSuffix As String

		With inDoc.Selection
			endingLine = VS_Selection.GetSelStartLine(inDoc)
			.gotoline(inStartLine)

			If (GetFnSignatures(DTE.ActiveDocument, funcName, returnType, functionParams, funcSuffix) = False) Then Exit Sub
			'now whole function signature is selected, so first delete it.
			.delete()
			.NewLine()
			.LineUp()

			'Return type
			If (returnType.StartsWith("virtual")) Then returnType = returnType.Substring(8)
			If (returnType.StartsWith("static")) Then returnType = returnType.Substring(7)
			returnType = returnType.Trim()
			.Text = returnType
			.NewLine()

			'Function Name
			.Text = VS_Document.GetFileNameAsClass(inDoc)
			.Text = "::"
			.Text = funcName
			.Text = "("

			'Params
			functionParams = functionParams.Trim
			If (functionParams.Length > 0) Then
				.NewLine()
				VS_Selection.SelectCurrentCompleteLine(inDoc)
				.Indent()
				'.Text = functionParams
				'VS further indents each line. so trim each line first.
				Dim paramLines As String()
				paramLines = functionParams.Split(vbNewLine)
				Dim paramLine As String

				For Each paramLine In paramLines
					If (paramLine.StartsWith(vbCr) Or paramLine.StartsWith(vbLf)) Then .NewLine()

					'Remove default values for function definition
					paramLine = ConvertDeclParamToDefParam(paramLine)

					.Text = paramLine.Trim
				Next
			End If
			.Text = ")"

			'Suffix
			If (funcSuffix.Length > 0) Then
#If 0 Then
				If (funcSuffix.EndsWith(";")) Then funcSuffix = Left(funcSuffix, funcSuffix.Length - 1)

				If (funcSuffix.EndsWith("=0")) Then funcSuffix = Left(funcSuffix, funcSuffix.Length - 2)
				If (funcSuffix.EndsWith("= 0")) Then funcSuffix = Left(funcSuffix, funcSuffix.Length - 3)
#Else
				Dim replaceDefaultValue As Regex

				'Remove pure virtual function marker
				Const patternSuffixPureVirtual As String = "([ \t]*\=[ \t]*0[ \t]*)"
				funcSuffix = replaceDefaultValue.Replace(funcSuffix, patternSuffixPureVirtual, "")

				'Remove semicolon if any
				Const patternSuffixSemicolon As String = "([ \t]*\;[ \t]*)"
				funcSuffix = replaceDefaultValue.Replace(funcSuffix, patternSuffixSemicolon, "")
#End If
				.Text = funcSuffix
			End If

			If (functionParams.Length <= 0) Then .NewLine() 'Otherwise next line content is included here.

			'Empty function body
			AddEmptyFunctionBody(inDoc)
			endingLine = VS_Selection.GetSelEndLine(inDoc)
			'Create header
			If (inAddHeader) Then
				.gotoline(inStartLine)
				CreateFuncHeader()
				endingLine = endingLine + 3
			End If
			VS_EditHelper.TrimEndingSpaces(inDoc, inStartLine, endingLine)
			oEndLine = endingLine
		End With
	End Sub

	Sub CreateFunctionGetSetDeclaration(ByVal inDoc As EnvDTE.Document _
	, ByVal inVariableDeclaration As String)
		'
		' Create function declaration from variable declaration statement
		' variable declaration statement must be in clipboard
		'	
		inVariableDeclaration = inVariableDeclaration.Trim
		Dim varType As String
		Dim varName As String
		If (VS_String.GetVariableTypeAndName(inVariableDeclaration, varType, varName) = False) Then
			MsgBox("clipboard text does not seems to be variable declaration statement")
			Exit Sub
		End If

		Dim varName2 As String
		varName2 = varName
		If (varName2.StartsWith("m")) Then varName2 = varName2.Substring(1)
		If (Char.IsUpper(varName2.Chars(0)) = False) Then varName2 = Char.ToUpper(varName2.Chars(0)) + varName2.Substring(1)
		With inDoc.Selection
			VS_EditHelper.StartNewLineIfNotEmpty(inDoc)
			Dim constVarType As String
			constVarType = varType
			If (constVarType.StartsWith("const") = False) Then constVarType = "const " + varType
			VS_EditHelper.ReplaceCurrentLineText(inDoc, vbTab + constVarType)
			.Text = vbTab
			Dim fnStartCol As Integer
			fnStartCol = VS_Selection.GetSelStartCol(inDoc)
			If (fnStartCol < VS_CodeCleanup.kFnStartCol) Then fnStartCol = VS_CodeCleanup.kFnStartCol
			fnStartCol = VS_CodeCleanup.GetNextIndentCol(fnStartCol)
			VS_CodeCleanup.IndentToColumn(inDoc, fnStartCol)
			.Text = "Get" + varName2 + "() const;" + vbNewLine

			.Text = "void"
			VS_CodeCleanup.IndentToColumn(inDoc, fnStartCol)
			.Text = "Set" + varName2 + "(" + vbNewLine

			VS_CodeCleanup.IndentToColumn(inDoc, fnStartCol + 4)
			If (constVarType.StartsWith("const") = False) Then .Text = "const "
			.Text = constVarType + vbTab

			Dim paramNameStartCol As Integer
			paramNameStartCol = VS_Selection.GetSelStartCol(inDoc)
			If (paramNameStartCol < VS_CodeCleanup.kFnParamNameStartCol) Then paramNameStartCol = VS_CodeCleanup.kFnParamNameStartCol
			paramNameStartCol = VS_CodeCleanup.GetNextIndentCol(paramNameStartCol)
			VS_CodeCleanup.IndentToColumn(inDoc, paramNameStartCol)
			.Text = "in" + varName2 + " );"
			.newline()
		End With
	End Sub

	Function GetFnSignatures(ByVal inDoc As EnvDTE.Document _
	, ByRef oFnName As String, ByRef oFnReturnType As String, ByRef oFnParms As String, ByRef oFnSuffix As String) As Boolean
		'
		'Gathers function signature items from the current location.
		'
		Dim functionParamStartCol As Integer
		Dim functionParamStartLine As Integer
		Dim functionParamsEndCol As Integer
		Dim functionParamsEndLine As Integer

		Dim startLine As Integer
		Dim funcStartLine As Integer

		With inDoc.Selection
			startLine = VS_Selection.GetSelStartLine(inDoc)

			'copy the function name
			.StartOfLine()
			PerformFindActionExact("(")
			funcStartLine = VS_Selection.GetSelStartLine(inDoc)
			If (startLine > funcStartLine) Then Exit Function
			.CharLeft()
			.WordLeft(True)
			oFnName = .Text

			'copy return type
			.CharLeft()
			.StartOfLine(vsStartOfLineOptions.vsStartOfLineOptionsFirstColumn, True)
			oFnReturnType = .text
			oFnReturnType = oFnReturnType.Trim

			'store location to Copy function parameters
			PerformFindActionExact("(")
			functionParamStartCol = VS_Selection.GetSelStartCol(inDoc) + 1
			functionParamStartLine = VS_Selection.GetSelStartLine(inDoc)

			'delete the semicolon in declaration
			PerformFindActionExact(")")
			functionParamsEndCol = VS_Selection.GetSelEndCol(inDoc) - 1
			functionParamsEndLine = VS_Selection.GetSelEndLine(inDoc)

			'Copy function parameters
			VS_Selection.SetSelection(inDoc, functionParamStartLine, functionParamStartCol, functionParamsEndLine, functionParamsEndCol)
			oFnParms = .text

			'copy suffix
			.CharRight(False)
			.CharRight(False)
			.EndOfLine(True)
			oFnSuffix = .Text
			.CharRight()


			'Select all the function signature.
			inDoc.Selection.MoveToLineAndOffset(funcStartLine, 1, True)
			GetFnSignatures = True
		End With
	End Function

End Module
