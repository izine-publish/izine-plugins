Imports System
Imports EnvDTE
Imports EnvDTE80
Imports System.Diagnostics

'Define a type for macro function pointer to filter a string
Delegate Function fileNameFilter(ByRef inFileName As String, ByRef inSearchPattern As String) As Boolean

Friend Module VS_SlnTreeUtils
	Const kSlnItem_Solution As String = "SolutionClass"
	Const kSlnItem_Project As String = "Project"
	Const kSlnItem_ProjectItem As String = "ProjectItem"

	Function GetSolutionExp() As UIHierarchy
		'
		' Return the solution explorer object
		'
		If DTE.Solution Is Nothing Or DTE.Solution.Projects Is Nothing Or DTE.Solution.Projects.Count() = 0 Then Return Nothing

		'GetSolutionExp = DTE.ToolWindows.SolutionExplorer
		GetSolutionExp = DTE.Windows.Item(Constants.vsext_wk_SProjectWindow).Object
	End Function

	Function GetSolutionExpItems() As EnvDTE.UIHierarchyItems
		GetSolutionExpItems = GetSolutionExp().UIHierarchyItems
	End Function

	Function GetItemTypeName(ByRef inSlnTreeItem As EnvDTE.UIHierarchyItem) As String
		Return Microsoft.VisualBasic.Information.TypeName(inSlnTreeItem.Object)
	End Function

	Function GetProjectItems() As UIHierarchyItems
		'
		'	Returns the project items in the solution
		'
		Dim solution As Solution2 = DTE.Solution
		If Not solution.IsOpen Then Return Nothing

		Dim items As EnvDTE.UIHierarchyItems = GetSolutionExpItems()

		For Each currentItem As UIHierarchyItem In items
			Dim typeName As String = GetItemTypeName(currentItem)
			If typeName = kSlnItem_Solution Then
				Return currentItem.UIHierarchyItems
			End If
		Next

	End Function


	Sub SelectItemInSolutionExp(ByRef inItem As UIHierarchyItem)
		DTE.Windows.Item(Constants.vsWindowKindSolutionExplorer).Activate()
		'DTE.ActiveWindow.Object.GetItem(solutionExplorerPath).Select(vsUISelectionType.vsUISelectionTypeSelect)
		inItem.Select(vsUISelectionType.vsUISelectionTypeSelect)
	End Sub

	Function GetSelectProject() As UIHierarchyItem
		'
		' Return the selected project in the tree or the project containing the first selected item
		'
		Dim slnExp As UIHierarchy = GetSolutionExp()

		If slnExp Is Nothing Then Return Nothing

		Dim selItems As Array = slnExp.SelectedItems()
		If selItems.Length = 0 Then Return Nothing

		For Each currItem As UIHierarchyItem In selItems

			Dim typeName As String = GetItemTypeName(currItem)

			If typeName = kSlnItem_Project Then
				Return currItem
			ElseIf typeName = kSlnItem_ProjectItem Then
				'The selection is not a project, return the project of selected item
				Dim projItem As ProjectItem = currItem.Object
				Dim prj As Project = projItem.ContainingProject

				Dim prjUIHItem As UIHierarchyItem = GetProjectUIHierarchyItem(prj)
				Return prjUIHItem
			End If
		Next

	End Function

	Function GetProjectUIHierarchyItem(ByRef inPrj As Project) As UIHierarchyItem

		Dim prjs As UIHierarchyItems = GetProjectItems()
		For Each currPrjItem As UIHierarchyItem In prjs
			If currPrjItem.Name = inPrj.Name Then Return currPrjItem
		Next
	End Function
	
	Public Function FindItem(ByVal inChildren As UIHierarchyItems _
	 , ByVal inFileName As String, ByVal inFilterFn As fileNameFilter _
	 , ByRef ioSolutionExplorerPath As String) As UIHierarchyItem

		For Each currentItem As UIHierarchyItem In inChildren
			Dim typeName As String = GetItemTypeName(currentItem)
			If typeName = kSlnItem_ProjectItem Then
				Dim projectItem As EnvDTE.ProjectItem = CType(currentItem.Object, EnvDTE.ProjectItem)
				Dim i As Integer = 1
				While i <= projectItem.FileCount
					Dim currFileName As String = projectItem.FileNames(i)
					Dim isMatch As Boolean
					If (inFilterFn Is Nothing) Then
						isMatch = (currFileName = inFileName)
					Else
						isMatch = inFilterFn(currFileName, inFileName)
					End If
					If isMatch Then
						ioSolutionExplorerPath = currentItem.Name
						Return currentItem
					End If
					i = i + 1
				End While
			End If

			Dim childItem As UIHierarchyItem = FindItem(currentItem.UIHierarchyItems, inFileName, inFilterFn, ioSolutionExplorerPath)
			If Not childItem Is Nothing Then
				ioSolutionExplorerPath = currentItem.Name + "\" + ioSolutionExplorerPath
				Return childItem
			End If
		Next
	End Function


	Public Sub TrackProjectItem()
		'
		' Function to test the working of above code
		'
		Dim solution As Solution2 = DTE.Solution
		If Not solution.IsOpen OrElse DTE.ActiveDocument Is Nothing Then Return

		'solution.FindProjectItem(DTE.ActiveDocument.FullName).ExpandView()

		Dim fileName As String = DTE.ActiveDocument.FullName
		Dim solutionExplorerPath As String
		Dim items As EnvDTE.UIHierarchyItems = GetSolutionExpItems()

		Dim item As UIHierarchyItem = FindItem(items, fileName, Nothing, solutionExplorerPath)

		If item Is Nothing Then
			MsgBox("Couldn't find the item in Solution Explorer.")
			Return
		End If

		SelectItemInSolutionExp(item)
	End Sub

End Module
