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



Friend Module InDn_ImplHelper



	Sub AddImplEmptyClassDefinition(ByVal inDoc As EnvDTE.Document _

	, ByVal inClassName As String, ByVal inInheritClass As String)



		With inDoc.Selection

			.Text = "class "

			.text = inClassName

			If (inInheritClass.Length > 0) Then

				.Text = " : public "

				.Text = inInheritClass

			End If

			.NewLine()

			.Text = "{"

			.NewLine()

			.Text = "public:"

			.NewLine()

			.Indent()

			VS_CodeCleanup.IndentToColumn(inDoc, VS_CodeCleanup.kFnStartCol)

			.text = inClassName

			.Text = "(IPMUnknown * boss);"

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



	Sub AddImplConstructorDestructor(ByVal inDoc As EnvDTE.Document _

	, ByVal inClassName As String, ByVal inInheritClass As String)

		With inDoc.Selection

			.text = inClassName

			.Text = "::"

			.text = inClassName

			.Text = "(IPMUnknown * boss)"

			If (inInheritClass.Length > 0) Then

				.NewLine()

				.Text = ": "

				.Text = inInheritClass + "(boss)"

			End If

			AddEmptyFunctionBody(inDoc)

			AddDestructorDefinition(inDoc, inClassName)

		End With

	End Sub



	Function ConvertCreatePMInterfaceLineToInterfaceFactoryLine(ByVal inDoc As EnvDTE.Document) As Boolean

		Dim currLine As Integer

		Dim foundLine As Integer

		ConvertCreatePMInterfaceLineToInterfaceFactoryLine = False



		If Not DTE.ActiveWindow Is inDoc.ActiveWindow Then inDoc.Activate()



#If 1 Then	'This method is faster.

		VS_FindHelper.PerformFindActionExact("(:b*//:b*)*CREATE(_|_PERSIST_)PMINTERFACE:b*\(:i:b*\,:b*:i:b*\).*", True, False, True)

		Dim lineText As String

		lineText = inDoc.Selection.Text

		If (lineText.Length > 0 And (lineText.IndexOf("CREATE_") <> -1)) Then

			With inDoc.Selection

				lineText = lineText.Replace("CREATE_PERSIST_", "REGISTER_")

				lineText = lineText.Replace("CREATE_", "REGISTER_")

				VS_EditHelper.ReplaceCurrentLineText(inDoc, lineText)

				ConvertCreatePMInterfaceLineToInterfaceFactoryLine = True

			End With

		End If

#Else

		VS_FindHelper.PerformFindActionExact("CREATE", True, False)

		With inDoc.Selection

			If (.Text <> "CREATE") Then Exit Function

			.Delete()

			.StartOfLine(vsStartOfLineOptions.vsStartOfLineOptionsFirstColumn, True)



			'Check if the line was commented.

			Dim textToDelete As String

			textToDelete = .Text

			textToDelete = textToDelete.Trim

			If (textToDelete.EndsWith("//")) Then .Text = "//"



			.Text = "REGISTER"

			ConvertCreatePMInterfaceLineToInterfaceFactoryLine = True

			currLine = VS_Selection.GetSelStartLine(inDoc)

			If (VS_FindHelper.PerformFindActionExact("PERSIST_", True, False) = True) Then

				foundLine = VS_Selection.GetSelStartLine(inDoc)

				If (currLine = foundLine) Then .Delete()

			End If

			.GoToLine(currLine)

		End With

#End If

	End Function



	Function PasteImplFnDeclarationFromInterfaceDecl(ByVal inDoc As EnvDTE.Document _

	 , ByVal inFunctionsDeclaration As String) As Boolean



		With inDoc.Selection

			Dim startLine As Integer

			Dim endLine As Integer

			VS_EditHelper.StartNewLineIfNotEmpty(inDoc)

			startLine = VS_Selection.GetSelStartLine(inDoc)

			.Text = inFunctionsDeclaration

			endLine = VS_Selection.GetSelStartLine(inDoc)

			Dim i As Integer

			Dim lineText As String

			Dim trimmedText As String

			Dim lineNewText As String

			Dim leadingWhitespace As String

			For i = startLine To endLine

				.MoveToLineAndOffset(i, 1)

				VS_Selection.SelectCurrentCompleteLine(inDoc)

				lineText = .Text

				trimmedText = lineText.TrimStart()

				If (lineText.Length - trimmedText.Length > 0) Then _

				leadingWhitespace = lineText.Substring(0, lineText.Length - trimmedText.Length)



				lineNewText = trimmedText

				If (trimmedText.StartsWith("virtual ") Or trimmedText.StartsWith("virtual	")) Then lineNewText = trimmedText.Substring(8)

				lineNewText = lineNewText.Trim

				lineNewText = VS_String.ReplacePattern(lineNewText, "[ \t]*\=[ \t]*0[ \t]*;", ";")



				VS_EditHelper.ReplaceCurrentLineText(inDoc, leadingWhitespace + lineNewText)

			Next i

		End With

		PasteImplFnDeclarationFromInterfaceDecl = True

	End Function



End Module



