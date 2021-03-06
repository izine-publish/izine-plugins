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



Public Module InDn_Delegator



	Dim gInterfaceName As String = "IStringData"

	Sub DelegateAllFunctions()

		Try

			gInterfaceName = InputBox("Enter Interface Name", gInterfaceName)

			Dim currDoc As Document = DTE.ActiveDocument

			Do Until False

				DelegateFunctionDefinition(currDoc, gInterfaceName)

			Loop

		Catch ex As Exception

		End Try

	End Sub

	Sub CreateDelegateFunction()

		gInterfaceName = InputBox("Enter Interface Name", gInterfaceName)

		DelegateFunctionDefinition(DTE.ActiveDocument, gInterfaceName)

	End Sub



	' Usage: the function must start with virtual

	' the function declaration must not contain ")"

	' manually pass the parameters if any.

	' Check manually all the function definitions

	Sub DelegateFunctionDefinition(ByVal inDoc As EnvDTE.Document, ByVal inInterfaceName As String)

		Dim interfaceName As String

		Dim fnInLogStr As String

		Dim fnOutLogStr1 As String

		Dim fnOutLogStr2 As String

		interfaceName = inInterfaceName

		If (inInterfaceName.Length <= 0) Then inInterfaceName = "IDelegateInterface"

		fnInLogStr = "LogFunctionEnterExit;"

		'fnInLogStr = "IZPLog_Str_( thisFileLA, enLT_DebugInfo, " & Chr(34) & "In " & Chr(34) & " );"

		fnOutLogStr1 = "" 'IZPLog_Str_( thisFileLA, enLT_DebugInfo, " & Chr(34) & "Out " & Chr(34) & " );"

		fnOutLogStr2 = "IZPLog_Str_( thisFileLA, enLT_DebugInfo, " & Chr(34) & "Out :  " & Chr(34) & ", toReturn );"

		If (False = DelegateFunctionDef(inDoc, interfaceName, fnInLogStr, fnOutLogStr1, fnOutLogStr2)) Then

			Throw New Exception("No virtual keyword found")

		End If

	End Sub





	Private Function DelegateFunctionDef(ByVal inDoc As EnvDTE.Document _

	 , ByVal inInterfaceStr As String _

	 , ByVal inFnInLogStr As String _

	 , ByVal inFnOutLogStr1 As String _

	 , ByVal inFnOutLogStr2 As String _

	 ) As Boolean

		Dim funcName As String

		Dim returnType As String

		Dim functionParams As String

		Dim funcSuffix As String

		Dim endLine As Integer

		Dim bUndoOpened As Boolean



		bUndoOpened = DTE.UndoContext.IsOpen

		If (Not bUndoOpened) Then DTE.UndoContext.Open("Delegate function definition")

		'delete the virtual keyword

		PerformFindActionExact("virtual")

		With inDoc.Selection

			'Check if the search string is selected or not.

			If (.text <> "virtual") Then Exit Function



			If (VS_ClassHelper.GetFnSignatures(inDoc, funcName, returnType, functionParams, funcSuffix) = False) Then Exit Function



			If (returnType.StartsWith("virtual")) Then returnType = returnType.Substring(8)

			returnType = returnType.Trim()



			VS_ClassHelper.CreateFunctionDefinition(inDoc, VS_Selection.GetSelStartLine(inDoc), endLine, False)



			.gotoline(endLine)

			.LineUp(False, 3)

			.EndOfLine()

			VS_EditHelper.StartNewLineIfNotEmpty(inDoc)

			.Text = vbTab



			AddDelegateFunctionDefaultContent(inDoc, inInterfaceStr, inFnInLogStr, inFnOutLogStr1, inFnOutLogStr2 _

			 , funcName, returnType, functionParams)



		End With

		If (Not bUndoOpened) Then DTE.UndoContext.Close()

		DelegateFunctionDef = True

	End Function



	'Adds the code for function delegate

	Sub AddDelegateFunctionDefaultContent(ByVal inDoc As EnvDTE.Document _

	, ByVal inInterfaceStr As String _

	 , ByVal inFnInLogStr As String _

	 , ByVal inFnOutLogStr1 As String _

	 , ByVal inFnOutLogStr2 As String _

	 , ByVal inFunctionName As String _

	 , ByVal inReturnType As String _

	, ByVal inFunctionParams As String)



		Dim functionArguments As String



		With inDoc.Selection

			If (inFnInLogStr.Length() > 0) Then

				.Text = inFnInLogStr + vbNewLine

			End If



			.Text = "InterfacePtr<" + inInterfaceStr + "> orgImpl( this, mDelegateIID );" + vbNewLine

			.Text = "if( orgImpl )" + vbNewLine

			If (inReturnType <> "void") Then

				.Text = "{" + vbNewLine

			End If

			.Text = "orgImpl->"

			.Text = inFunctionName

			functionArguments = VS_String.ConvertParamsToArgs(inFunctionParams)

			.Text = "( " + functionArguments + " );"

			If (inReturnType <> "void") Then

				.StartOfLine(vsStartOfLineOptions.vsStartOfLineOptionsFirstText, False)

				.Text = inReturnType

				.Text = " toReturn = "

				.endofline()

				.NewLine()

				If (inFnOutLogStr2.Length() > 0) Then

					.Text = inFnOutLogStr2

					.NewLine()

				End If

				.Text = "return toReturn;" + vbNewLine

				.Text = "}"

				.NewLine()

				.Text = "return ( " + inReturnType + " )0;"

			Else

				If (inFnOutLogStr1.Length() > 0) Then

					.NewLine()

					.Text = inFnOutLogStr1

				End If

			End If

		End With

	End Sub

End Module



