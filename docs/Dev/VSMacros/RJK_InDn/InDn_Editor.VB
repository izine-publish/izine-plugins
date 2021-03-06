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



Public Module InDn_Editor



	Sub IPtr()



		Dim bUndoOpened As Boolean



		bUndoOpened = DTE.UndoContext.IsOpen

		If (Not bUndoOpened) Then DTE.UndoContext.Open("InterfacePtr Line")

		VS_EditHelper.StartNewLineIfNotEmpty(DTE.ActiveDocument)

		DTE.ActiveDocument.Selection.Text = vbTab + "InterfacePtr<> ( , UseDefaultIID() );"

		If (Not bUndoOpened) Then DTE.UndoContext.Close()

	End Sub



	Sub DeleteAllLinesOfCurrentWord()



		Dim curDocFileSuffix As String

		Dim bUndoOpened As Boolean



		bUndoOpened = DTE.UndoContext.IsOpen

		curDocFileSuffix = VS_Document.GetFileName(DTE.ActiveDocument, True)

		'This macro is meant to work with ID.h files.

		If (Not curDocFileSuffix.EndsWith("ID.h")) Then Exit Sub



		If (Not bUndoOpened) Then DTE.UndoContext.Open("DeletedLines with word - ")

		InDn_IDHelper.DeleteAllLinesOfCurrentWord(DTE.ActiveDocument)

		If (Not bUndoOpened) Then DTE.UndoContext.Close()

	End Sub



	Sub NewInterfaceHeaderFileContent()

		Dim className As String

		Dim bUndoOpened As Boolean

		bUndoOpened = DTE.UndoContext.IsOpen

		Dim currDoc As Document = DTE.ActiveDocument

		className = VS_Document.GetFileNameAsClass(currDoc)



		If (Not bUndoOpened) Then DTE.UndoContext.Open("New Interface Header File Content ")

		With currDoc.Selection

			Dim startLine As Integer

			startLine = VS_Selection.GetSelStartLine(currDoc)

			.EndOfLine()

			.NewLine()

			InDn_InterfaceHelper.AddInterfaceClassEmptyDefinition(currDoc, className)



			.NewLine()



			VS_Selection.SetSelection(currDoc, startLine, 1, startLine, 1)

			DTE.ExecuteCommand("Macros.RJK_InDn.VS_Editor.AddHeaderHashIf")

		End With

		If (Not bUndoOpened) Then DTE.UndoContext.Close()

	End Sub



	Sub AddHashIncludeGroupComments()

		Dim bUndoOpened As Boolean



		bUndoOpened = DTE.UndoContext.IsOpen



		If (Not bUndoOpened) Then DTE.UndoContext.Open("#include goup comments")

		With DTE.ActiveDocument.Selection

			'.EndOfDocument()

			VS_EditHelper.StartNewLineIfNotEmpty(DTE.ActiveDocument)

			.Text = iZineFileHeader.GetImplIncludeComments

		End With

		If (Not bUndoOpened) Then DTE.UndoContext.Close()

	End Sub



	Sub CreateNewImplFileContent()

		Dim className As String

		Dim bUndoOpened As Boolean



		bUndoOpened = DTE.UndoContext.IsOpen

		Dim currDoc As Document = DTE.ActiveDocument

		className = VS_Document.GetFileNameAsClass(currDoc)



		If (Not bUndoOpened) Then DTE.UndoContext.Open("Create New Impl File Content")

		With currDoc.Selection

			Dim inheritClass As String

			.EndOfDocument()

			.Text = iZineFileHeader.GetImplIncludeComments

			.Text = "#include ""IZPLog.h"""

			.NewLine(2)

			.Text = "#define thisFileLA"

			.Indent()

			.Text = "enLMA_"

			.NewLine()

			.Text = "#define LogFunctionEnterExit"

			.Indent()

			.Text = "IZPLog_func( thisFileLA )"

			.NewLine(2)



			'define base class macro "inheritClass"

			.Text = "#define inheritClass CPMUnknown<"

			.text = className

			.WordLeft()

			.Text = "I"

			.Delete()

			.WordRight()

			.Text = ">"

			'.WordLeft(True, 4)

			'inheritClass = .Text

			'.CharRight()

			.NewLine()



			inheritClass = "inheritClass"

			AddImplEmptyClassDefinition(currDoc, className, inheritClass)

			.NewLine(2)

			.Text = "CREATE_PMINTERFACE("

			.Text = className

			.Text = ", "

			.Text = className

			.Text = "Impl"

			.WordLeft()

			.Text = "k"

			.EndOfLine()

			.Text = ")"

			.NewLine(2)

			AddImplConstructorDestructor(currDoc, className, inheritClass)

			.Text = "#pragma mark -"

			.NewLine()

		End With

		If (Not bUndoOpened) Then DTE.UndoContext.Close()

	End Sub



	Sub PasteImplFnDeclarationFromInterfaceDecl()

	'

	' Used to copy function declarations from Interface file to impl file.

	'

		Dim fnDecl As String

		

		fnDecl = VS_Clipboard.ClipboardText

		If (fnDecl Is Nothing) Then Exit Sub

		If (fnDecl.Length <= 0) Then Exit Sub



		DTE.UndoContext.Open("paste function declaration " + FileName)

		InDn_ImplHelper.PasteImplFnDeclarationFromInterfaceDecl(DTE.ActiveDocument, fnDecl)

		DTE.UndoContext.Close()

	End Sub



	Function MakeInterfaceFactory() As Boolean

		'

		' Convert search result of create pminterface to Register PMinterface

		' seach string with regex is

		'CREATE(_|_PERSIST_)PMINTERFACE[ \t]*\([A-Za-z]+[ \t]*\,[ \t]*k[a-zA-Z]+Impl[ \t]*\)

		'

		'New Regex for Visual Studio

		'CREATE(_|_PERSIST_)PMINTERFACE:b*\(:i:b*\,:b*:i:b*\)

		'

		' function returning nothing to avoid its visibilty in macro explorer.



		Dim bUndoOpened As Boolean



		bUndoOpened = DTE.UndoContext.IsOpen



		If (Not bUndoOpened) Then DTE.UndoContext.Open("MakeInterfaceFactory")

		Dim currDoc As Document = DTE.ActiveDocument

		Do

			If (InDn_ImplHelper.ConvertCreatePMInterfaceLineToInterfaceFactoryLine(currDoc) = False) Then Exit Do

		Loop



		If Not DTE.ActiveWindow Is currDoc.ActiveWindow Then currDoc.Activate()



		'

		'	Replace REGISTER_PERSIST_PMINTERFACE with REGISTER_PMINTERFACE

		'

		VS_FindHelper.ReplaceInCurrDoc("REGISTER_PERSIST_PMINTERFACE", "REGISTER_PMINTERFACE", True)



		If (Not bUndoOpened) Then DTE.UndoContext.Close()

	End Function



	Sub UpdateIntefaceFactories()

		'

		' Updates the interface factories for All projects

		'



		Dim prjItems As UIHierarchyItems = VS_SlnTreeUtils.GetProjectItems()



		For Each prjItem As UIHierarchyItem In prjItems

			UpdateInterfaceFactoryForProject(prjItem)

		Next



		'Dim thePrjFileItem As UIHierarchyItem

		'DTE.Windows.Item(Constants.vsWindowKindSolutionExplorer).Activate()

		'DTE.ActiveWindow.Object.GetItem("iZinePublishPlns\iZinePublish").Select(vsUISelectionType.vsUISelectionTypeSelect)

		'DTE.ActiveWindow.Object.GetItem("iZinePublishPlns\iZinePublish\Header Files").UIHierarchyItems.Expanded = True

		'thePrjFileItem = DTE.ActiveWindow.Object.GetItem("iZinePublishPlns\iZinePublish\Header Files\" + "IZPFactoryList_Auto.h")

		'UpdateIntefaceFactoryProjectFileItem(thePrjFileItem)



		'DTE.Windows.Item(Constants.vsWindowKindSolutionExplorer).Activate()

		'DTE.ActiveWindow.Object.GetItem("iZinePublishPlns\iZinePublishUI").Select(vsUISelectionType.vsUISelectionTypeSelect)

		'thePrjFileItem = DTE.ActiveWindow.Object.GetItem("iZinePublishPlns\iZinePublishUI\Header Files\" + "IZPUIFactoryList_Auto.h")

		'UpdateIntefaceFactoryProjectFileItem(thePrjFileItem)



	End Sub



	Sub UpdateInterfaceFactoryForSelectedProject()

		'

		' Updates the interface factory the selected project.

		'

	

		Dim prjItem As UIHierarchyItem = VS_SlnTreeUtils.GetSelectProject()



		If prjItem Is Nothing Then

			MsgBox("No project selected")

		Else

			UpdateInterfaceFactoryForProject(prjItem)

		End If

	End Sub



	Sub UpdateInterfaceFactoryForProject(ByVal inPrjItem As UIHierarchyItem)



		Dim slnFilePath As String

		Dim factoryFileName As String = "FactoryList_Auto.h"

		Dim factoryFileItem As UIHierarchyItem = VS_SlnTreeUtils.FindItem(inPrjItem.UIHierarchyItems, factoryFileName, AddressOf VS_String.StringEndsWith, slnFilePath)



		If factoryFileItem Is Nothing Then Return 'Should we alert the user



		'Dim fileName As String = VS_String.GetFileNameFromPath(slnFilePath)

		'Dim slnFileParPath As String = slnFilePath.Substring(0, slnFilePath.Length - fileName.Length)



		'MsgBox(slnFileParPath + vbNewLine + fileName)



		UpdateIntefaceFactoryProjectFileItem(factoryFileItem)

	End Sub



	Sub UpdateIntefaceFactoryProjectFileItem(ByVal inPrjFileItem As UIHierarchyItem)

		'

		' Updates the interface factory of current project

		'

		On Error GoTo resumeScreenUpdates

		Dim oldSuppressUIValue As Boolean

		Dim suppressUIDte As DTE



		suppressUIDte = DTE.MainWindow.DTE

		oldSuppressUIValue = suppressUIDte.SuppressUI	'DTE.SuppressUI



		Dim clipboardText As String



		'Select the file item so that it's project becomes the current project for findInFiles

		VS_SlnTreeUtils.SelectItemInSolutionExp(inPrjFileItem)



		'Open its window if not open

		DTE.ActiveWindow.Object.DoDefaultAction()

		Dim theFileName As String = inPrjFileItem.Name

		DTE.Windows.Item(theFileName).Activate()



		Dim factoryDoc As Document = DTE.ActiveDocument



		'VS_FindHelper.FindInFiles("CREATE(_|_PERSIST_)PMINTERFACE[ \t]*\([A-Za-z]+[ \t]*\,[ \t]*k[a-zA-Z]+Impl[ \t]*\)", True, , False, "*.cpp")

		VS_FindHelper.FindInFiles("CREATE(_|_PERSIST_)PMINTERFACE:b*\(:i:b*\,:b*:i:b*\)", True, , False, "*.cpp")



		DTE.Windows.Item(Constants.vsWindowKindFindResults2).Activate()

		DTE.ExecuteCommand("Edit.SelectAll")

		DTE.ExecuteCommand("Edit.Copy")



		'Search takes some time, so wait till searh is over.

		Do

			clipboardText = VS_Clipboard.ClipboardText

			If (clipboardText.LastIndexOf("Matching lines: ") <> -1) Then

				Exit Do

			End If

			If (clipboardText.LastIndexOf("Matching files: ") <> -1) Then

				MsgBox("Error updating interface factory file : " + theFileName + vbNewLine + vbNewLine + _

				"Unselect the find option ""Display file names only.""", MsgBoxStyle.Critical)

				Exit Sub

			ElseIf (clipboardText.LastIndexOf("No files were found to look in.") <> -1) Then

				MsgBox("Error updating interface factory file : " + theFileName + vbNewLine + vbNewLine + _

				"Find in files is not working.", MsgBoxStyle.Critical)

				Exit Sub

			End If



			System.Threading.Thread.Sleep(100)

			DTE.ExecuteCommand("Edit.SelectAll")

			DTE.ExecuteCommand("Edit.Copy")

		Loop



		suppressUIDte.SuppressUI = True



		'DTE.ItemOperations.NewFile("Visual C++\C++ File (.cpp)")

		'VS_SlnTreeUtils.SelectItemInSolutionExp(inPrjFileItem)



		factoryDoc.ActiveWindow.DTE.SuppressUI = True

		If Not DTE.ActiveWindow Is factoryDoc.ActiveWindow Then factoryDoc.Activate()

		



		DTE.ExecuteCommand("Edit.SelectAll")



		factoryDoc.Selection.Paste()



		'Delete first and last line

		VS_EditHelper.DeleteFirstLine(factoryDoc)

		VS_EditHelper.DeleteLastLine(factoryDoc)



		MakeInterfaceFactory()



		'iZineFileHeader.AddFileHeader()

		factoryDoc.Selection.GoToLine(1)

		factoryDoc.Selection.StartOfLine(vsStartOfLineOptions.vsStartOfLineOptionsFirstColumn)

		factoryDoc.Selection.Text = iZineFileHeader.GetFileHeaderString("Raj Kumar Sehrawat", _

   		"Auto generated file. Don't modify it manually." + vbNewLine _

   		+ "//" + vbTab + "To update this file use macro ""UpdateIntefaceFactories"".")



		factoryDoc.Save()

		factoryDoc.Close()	'DTE.ExecuteCommand("Window.CloseDocumentWindow")

		'DTE.ActiveWindow.Close(vsSaveChanges.vsSaveChangesNo)



resumeScreenUpdates:

		suppressUIDte.SuppressUI = oldSuppressUIValue

	End Sub





	Sub CompileFrForSelectedProject()

		'

		' Compile InDesign resource file for the selected project.

		'



		Dim prjItem As UIHierarchyItem = VS_SlnTreeUtils.GetSelectProject()



		If prjItem Is Nothing Then

			MsgBox("No project selected")

		Else

			CompileFrForProject(prjItem)

		End If

	End Sub



	Sub CompileFrForProject(ByVal inPrjItem As UIHierarchyItem)

		'

		' Compile InDesign resource file for the given project.

		'

		Dim slnFilePath As String

		Dim factoryFileName As String = ".fr"

		Dim factoryFileItem As UIHierarchyItem = VS_SlnTreeUtils.FindItem(inPrjItem.UIHierarchyItems, factoryFileName, AddressOf VS_String.StringEndsWith, slnFilePath)



		If factoryFileItem Is Nothing Then Return 'Should we alert the user



		factoryFileItem.Select(vsUISelectionType.vsUISelectionTypeSelect)

		DTE.Windows.Item(Constants.vsWindowKindOutput).Activate()

		DTE.ExecuteCommand("Build.Compile")

	End Sub

End Module

