Attribute VB_Name = "FileUtils"
Option Explicit

Function DoesFileExist(inFilePath As String) As Byte
' returns 1 if inFilePath exists, otherwise 0
    If Dir(inFilePath, vbNormal) <> "" Then
        DoesFileExist = 1
    Else
        DoesFileExist = 0
    End If
End Function


Function GetWorkbookDirPath(inWorkbook As Workbook) As String
    GetWorkbookDirPath = inWorkbook.Path
End Function
