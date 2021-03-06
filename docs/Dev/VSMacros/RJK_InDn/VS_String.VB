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

Imports System.Text.RegularExpressions



Friend Module VS_String



	Function IsValidVariableName(ByVal inStr As String) As Boolean

		If (inStr.Length <= 0) Then Exit Function

		If (inStr.Contains(" ") Or inStr.Contains(vbTab)) Then Exit Function

		IsValidVariableName = True

	End Function



	'Convert function parameter to arguments

	' removes parameter types

	Function ConvertParamsToArgs(ByVal inFunctionParams As String) As String



		Dim argStr As String

		Dim params As String()

		params = inFunctionParams.Split(",")

		Dim paramStr As String

		Dim argName As String

		argStr = ""

		For Each paramStr In params

			argName = ConvertParamToArg(paramStr)

			If ((argStr Is Nothing) Or argStr.Length() <= 0) Then

				argStr = argName

			Else

				argStr = argStr + ", " + argName

			End If

		Next paramStr

		ConvertParamsToArgs = argStr

	End Function



	' removes parameter types

	Function ConvertParamToArg(ByVal inFunctionParam As String) As String



		Dim argName As String

		Dim equalToIndex As Integer



		If (inFunctionParam Is Nothing Or inFunctionParam.Length <= 0) Then Exit Function



		equalToIndex = inFunctionParam.LastIndexOf("=")

		If (equalToIndex <> -1) Then

			inFunctionParam = inFunctionParam.Substring(1, equalToIndex - 1)

		End If

		inFunctionParam = inFunctionParam.Trim



		'Remove * or & from argument

		argName = inFunctionParam.Replace("*", " ")

		argName = argName.Replace("&", " ")

		argName = argName.Replace(vbTab, Chr(32))

		argName = argName.Substring(argName.LastIndexOf(Chr(32)) + 1)



		ConvertParamToArg = argName

	End Function



	' removes default values from parameters

	Function ConvertDeclParamToDefParam(ByVal inFunctionParam As String) As String



		Dim temp As String

		Dim equalToIndex As Integer

		Dim replaceDefaultValue As Regex



		ConvertDeclParamToDefParam = inFunctionParam

		If (inFunctionParam Is Nothing Or inFunctionParam.Length <= 0) Then Exit Function



		equalToIndex = inFunctionParam.IndexOf("=")

		If (equalToIndex = -1) Then

			Exit Function

		End If



		'Prepare a regular expression object

		Const pattern As String = "([ \t]*\=[ \t]*[_a-zA-Z0-9]*(::)*[_a-zA-Z0-9]*[ \t]*)"

		temp = replaceDefaultValue.Replace(inFunctionParam, pattern, "")

		ConvertDeclParamToDefParam = temp



	End Function



	'sort the lines in the string. empty lines will be deleted.

	Function SortLines(ByVal inLines As String, Optional ByVal inTrimBeforeSorting As Boolean = True) As String

		Dim linesText As String()

		Dim sortedLines As String

		Dim lineText As String

		linesText = inLines.Split(vbNewLine)



		Dim arrSize As Integer

		arrSize = linesText.Length()

		If (arrSize) <= 0 Then Exit Function



		Dim lastLineWhitespace As String

		Dim lastLine As String

		Dim lastLineTrimmed As String

		lastLine = linesText(arrSize - 1)

		lastLineTrimmed = lastLine.Clone.TrimEnd

		If (lastLineTrimmed.Length < lastLine.Length) Then

			lastLineWhitespace = lastLine.Substring(lastLineTrimmed.Length)

		End If

		If (inTrimBeforeSorting) Then

			Dim i As Integer

			For i = 0 To arrSize - 1

				linesText(i) = linesText(i).Trim()

			Next i

		End If

		linesText.Sort(linesText)



		For Each lineText In linesText

			If (lineText.Length > 0) Then

				sortedLines = sortedLines + lineText + vbNewLine

			End If

		Next

		SortLines = sortedLines.Trim() + lastLineWhitespace

	End Function



	Function GetVariableTypeAndName(ByVal inVarDeclaration As String, _

	 ByRef oVarType As String, ByRef oVarName As String) As Boolean

		inVarDeclaration = inVarDeclaration.Trim

		If (inVarDeclaration.Length <= 0) Then Exit Function



		Dim spaceIndexBeforeVarName As Integer

		Dim spaceSet(2) As Char

		spaceSet(0) = vbTab.Chars(0)

		spaceSet(1) = " ".Chars(0)

		spaceIndexBeforeVarName = inVarDeclaration.LastIndexOfAny(spaceSet)

		If (spaceIndexBeforeVarName = -1) Then Exit Function

		oVarType = inVarDeclaration.Substring(0, spaceIndexBeforeVarName)

		oVarName = inVarDeclaration.Substring(spaceIndexBeforeVarName)

		oVarType = oVarType.Trim

		oVarName = oVarName.Trim

		If (oVarName.EndsWith(";")) Then oVarName = oVarName.Substring(0, oVarName.Length - 1)

		GetVariableTypeAndName = True

	End Function



	Function ReplacePattern(ByVal inString As String, ByVal inSearchPattern As String, ByVal inReplaceString As String) As String

		Dim regex As New Text.RegularExpressions.Regex(inSearchPattern, Text.RegularExpressions.RegexOptions.Multiline)

		ReplacePattern = regex.Replace(inString, inReplaceString)

	End Function



	Function GetFileNameFromPath(ByVal inFilePath As String) As String

		Dim foundIndex As Integer = inFilePath.LastIndexOf("\")

		If foundIndex <> -1 Then Return inFilePath.Substring(foundIndex + 1)

	End Function



	Function StringEndsWith(ByRef inString As String, ByRef inStrToSearch As String) As Boolean

		Return inString.EndsWith(inStrToSearch)

	End Function

End Module

