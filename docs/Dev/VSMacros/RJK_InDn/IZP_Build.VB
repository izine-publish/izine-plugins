Imports System
Imports EnvDTE
Imports EnvDTE80
Imports System.Diagnostics

'Define a type for macro function pointer
Delegate Sub macroPointer()

Public Module IZP_Build
	Dim nextMacroToRunAfterBuild As macroPointer

	Sub CompileFrFiles()
		nextMacroToRunAfterBuild = AddressOf CompileUIFrFile
		CompileFrFile()
	End Sub

	Friend Sub CompileFrFile()
		DTE.Windows.Item(Constants.vsWindowKindSolutionExplorer).Activate()
		DTE.ActiveWindow.Object.GetItem("iZinePublishPlns\iZinePublish").UIHierarchyItems.Expanded = True
		DTE.ActiveWindow.Object.GetItem("iZinePublishPlns\iZinePublish\Resource Files").UIHierarchyItems.Expanded = True
		DTE.ActiveWindow.Object.GetItem("iZinePublishPlns\iZinePublish\Resource Files\IZP.fr").Select(vsUISelectionType.vsUISelectionTypeSelect)
		DTE.Windows.Item(Constants.vsWindowKindOutput).Activate()
		DTE.ExecuteCommand("Build.Compile")
	End Sub

	Friend Sub CompileUIFrFile()
		nextMacroToRunAfterBuild = Nothing
		DTE.Windows.Item(Constants.vsWindowKindSolutionExplorer).Activate()
		DTE.ActiveWindow.Object.GetItem("iZinePublishPlns\iZinePublishUI").UIHierarchyItems.Expanded = True
		DTE.ActiveWindow.Object.GetItem("iZinePublishPlns\iZinePublishUI\Resource Files").UIHierarchyItems.Expanded = True
		DTE.ActiveWindow.Object.GetItem("iZinePublishPlns\iZinePublishUI\Resource Files\IZPUI.fr").Select(vsUISelectionType.vsUISelectionTypeSelect)
		DTE.Windows.Item(Constants.vsWindowKindOutput).Activate()
		DTE.ExecuteCommand("Build.Compile")
	End Sub

End Module
