Imports System
Imports EnvDTE
Imports EnvDTE80
Imports System.Diagnostics

Public Module iZinePln_Editor

	Public Sub NewWSRequestHeaderFile()
		'
		'Use this macro when creating a new WS request header file.
		'
		With DTE.ActiveDocument

			.Selection.SelectAll()
			iZineFileHeader.AddFileHeader()
			VS_Editor.AddHeaderHashIf()

			.Selection.LineUp(False, 2)
			.Selection.NewLine()
			.Selection.Text = "#include ""AZPSoapRequest.h"""
			.Selection.NewLine()
			.Selection.Text = "class AZPSoapResponse;"
			.Selection.NewLine(2)

			VS_Editor.AddEmptyClassDefinition()
			.Selection.LineUp(False, 8)
			.Selection.EndOfLine()
			.Selection.Text = " : public AZPSoapRequest"
			.Selection.LineDown(False, 5)
			.Selection.LineUp()
			.Selection.NewLine(2)
			.Selection.Text = "#ifdef Debug_WS"
			.Selection.NewLine()
			.Selection.Text = "const char *"
			.Selection.Indent(2)
			.Selection.Text = "GetName() const;"
			.Selection.NewLine()
			.Selection.Text = "#endif"
			.Selection.LineDown()
			.Selection.EndOfLine()
			.Selection.NewLine()
			.Selection.Text = "AZPSoapResponse *"
			.Selection.Indent()
			.Selection.Text = "CreateResponseObj();"
			.Save()
		End With
	End Sub

	Public Sub NewWSRequestCppFile()
		With DTE.ActiveDocument.Selection

			iZineFileHeader.AddFileHeader()
			InDn_Editor.AddHashIncludeGroupComments()

			'Add request/response header include
			.LineUp(False, 3)
			.NewLine()
			.Text = "#include """
			VS_Editor.PasteFileNameAsClassName()
			.Text = ".h"""
			.NewLine()
			.Text = "#include """
			VS_Editor.PasteFileNameAsClassName()
			.DeleteLeft(5)
			.Text = "sponse.h"""

			'Create constructor/destructor
			.EndOfDocument()
			VS_Editor.AddConstructorDestructorDefinition()
			.LineUp(False, 13)
			.NewLine()
			.LineUp(False, 1)
			.Text = ": AZPSoapRequest( eReq_ )"
			.WordLeft(False, 2)
			.CharRight(False, 5)
			VS_Editor.PasteFileNameAsClassName()
			.CharLeft(True, 7)
			.Delete()
			.WordLeft()
			.CharRight(False, 5)
			.Delete(3)
			.EndOfDocument()

			'Function definitions common in all requests.
			.Text = "#ifdef Debug_WS"
			.NewLine()
			.Text = "#endif"
			.NewLine()
			.NewLine()
			.Text = "#pragma mark -"
			.NewLine()
			.LineUp(False, 3)
			.NewLine()
			.LineUp()
			.Text = "const char *"
			.NewLine()
			VS_Editor.PasteFileNameAsClassName()
			.Text = "::GetName() const"
			.NewLine()
			.Text = "{"
			.NewLine()
			.Text = "return """";"
			.NewLine()
			.Text = "}"
			.LineUp(False, 4)
			iZineFileHeader.CreateFuncHeader()
			.LineDown(False, 7)
			.NewLine()
			.Text = "AZPSoapResponse *"
			.NewLine()
			VS_Editor.PasteFileNameAsClassName()
			.Text = "::CreateResponseObj()"
			.NewLine()
			.Text = "{"
			.NewLine()
			.Text = "return new "
			VS_Editor.PasteFileNameAsClassName()
			.DeleteLeft(5)
			.Text = "sponse;"
			.NewLine()
			.Text = "}"
			.NewLine()
			.LineUp(False, 5)
			iZineFileHeader.CreateFuncHeader()
			.EndOfDocument()
			DTE.ActiveDocument.Save()
		End With
	End Sub

	Public Sub NewWSResponseHeaderFile()
		'
		'Use this macro when creating a new WS response header file.
		'
		With DTE.ActiveDocument.Selection
			iZineFileHeader.AddFileHeader()
			VS_Editor.AddHeaderHashIf()

			.LineUp(False, 2)
			.NewLine()
			.Text = "#include ""AZPSoapResponse.h"""
			.NewLine(2)
			VS_Editor.AddEmptyClassDefinition()
			.LineUp(False, 8)
			.EndOfLine()
			.Text = " : public AZPSoapResponse"
			DTE.ActiveDocument.Save()
		End With
	End Sub

	Public Sub NewWSResponseCppFile()
		With DTE.ActiveDocument.Selection
			iZineFileHeader.AddFileHeader()
			InDn_Editor.AddHashIncludeGroupComments()

			.LineUp(False, 3)
			.Text = "#include """
			VS_Editor.PasteFileNameAsClassName()
			.Text = ".h"""
			.NewLine()
			.EndOfDocument()

			VS_Editor.AddConstructorDestructorDefinition()
			.LineUp(False, 13)
			.NewLine()
			.LineUp(False, 1)
			.Text = ": AZPSoapResponse( eResp_ )"
			.WordLeft(False, 2)
			.CharRight(False, 6)
			VS_Editor.PasteFileNameAsClassName()
			.CharLeft(True, 8)
			.Delete()
			.WordLeft()
			.CharRight(False, 6)
			.Delete(3)
			.EndOfDocument()

			DTE.ActiveDocument.Save()
		End With
	End Sub
End Module
