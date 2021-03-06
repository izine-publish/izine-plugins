Imports System
Imports EnvDTE
Imports EnvDTE80
Imports System.Diagnostics

Friend Module InDn_InterfaceHelper


	Sub AddInterfaceClassEmptyDefinition(ByVal inDoc As EnvDTE.Document _
	, ByVal inClassName As String)
		With inDoc.Selection
			.Text = "class "
			DTE.ExecuteCommand("Macros.RJK_InDn.VS_Editor.PasteFileNameAsClassName")
			.Text = " : public IPMUnknown"
			.NewLine()
			.Text = "{"
			.NewLine()
			.Text = "public:"
			.NewLine()
			.Text = "enum { kDefaultIID - "
			.DeleteLeft(2)
			.Text = "= IID_"
			DTE.ExecuteCommand("Macros.RJK_InDn.VS_Editor.PasteFileNameAsClassName")
			.WordLeft()
			.WordRight(True)
			.ChangeCase(vsCaseOptions.vsCaseOptionsUppercase)
			.CharRight(False, 1)
			.Text = " };"
			.NewLine(2)
			.Text = "};"
			.NewLine(2)
		End With
	End Sub
End Module
