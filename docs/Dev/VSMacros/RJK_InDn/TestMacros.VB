Imports System
Imports EnvDTE
Imports EnvDTE80
Imports System.Diagnostics

Public Module TestMacros
	Sub test_GetFnSignatures()
		Dim funcName As String
		Dim returnType As String
		Dim functionParams As String
		Dim funcSuffix As String

		GetFnSignatures(DTE.ActiveDocument, funcName, returnType, functionParams, funcSuffix)
	End Sub

End Module
