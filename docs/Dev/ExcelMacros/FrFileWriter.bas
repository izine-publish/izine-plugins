Attribute VB_Name = "FrFileWriter"
Option Explicit

Sub UpdateFrFile()

    Dim currWorksheet As Worksheet
    Dim currWorkbookPath As String
    Dim frFilePath As String
    
    Set currWorksheet = ActiveWorkbook.ActiveSheet
    If (currWorksheet.Name = "Resources" Or currWorksheet.Name = "ResourcesUI") Then
    
        currWorkbookPath = FileUtils.GetWorkbookDirPath(ActiveWorkbook)
        frFilePath = currWorkbookPath + currWorksheet.Range("D1").Value
        If (Right(frFilePath, 1) <> "\") Then frFilePath = frFilePath + "\"
        frFilePath = frFilePath + currWorksheet.Range("C1").Value
        Application.ScreenUpdating = False
        currWorksheet.Outline.ShowLevels 2, 2   'Show all the groupped rows, otherwise those rows are skipped.
        On Error GoTo exit_UpdateFrFile
        WriteFrClassDefsToFile currWorksheet, frFilePath
        currWorksheet.Outline.ShowLevels 1, 0   'Collapse all groupped rows.
        
exit_UpdateFrFile:
        Application.ScreenUpdating = True
        Application.ActiveCell.Activate
    End If
End Sub


Sub WriteFrClassDefsToFile(inWorkSheet As Worksheet, inFilePath As String)
    
    Dim fileHeader As String
    Dim frFileNum As Integer
    Dim endRow As Integer
    Dim currRow As Long
    Dim currClassDefinitionStr As String
    Dim defType As String
    Dim bossName As String
    Dim baseBossName As String
    Dim interfaceCell As Range
    Dim currCell As Range
    Dim selectionToRestore As Range
    
    fileHeader = GetFileHeaderString
    
    endRow = inWorkSheet.Range("C2").Value
    If (endRow < 3) Then
        MsgBox "End row not specified.", vbCritical, "Error"
        Exit Sub
    End If
        
    frFileNum = FreeFile
    
    Open inFilePath For Output As #frFileNum
    Print #frFileNum, fileHeader
    
    currRow = 3
    Set selectionToRestore = ActiveCell
    inWorkSheet.Range("A" & currRow).Select
    'move to first row where class definition starts
    If (ActiveCell.Value = "") Then Selection.End(xlDown).Select
    
    Dim classComments As String
            
    Do While currRow <= endRow
        Set currCell = ActiveCell
        defType = currCell.Value
        bossName = currCell.Offset(0, 1).Value
        baseBossName = currCell.Offset(0, 2).Value
        Set interfaceCell = currCell.Offset(1, 2)
        
        If (IsPreprocessorValue(defType)) Then
            currClassDefinitionStr = defType
        Else
            currClassDefinitionStr = InDn_ClassDefinition(defType, bossName, baseBossName, interfaceCell)
        End If
        If (currClassDefinitionStr <> "") Then
            classComments = currCell.Offset(0, 4).Value
            If (classComments <> "") Then
                classComments = vbTab + "/* " + classComments + " */"
                Print #frFileNum, classComments
            End If
            Print #frFileNum, currClassDefinitionStr
        End If
        currCell.Select
        Selection.End(xlDown).Select
        currRow = ActiveCell.Row
    Loop
    
    If (ActiveCell.Value <> "") Then
        MsgBox "Seems like there are more rows to be processed. Set sufficient value in cell C2", vbInformation
    End If
    
    
    Close #frFileNum
    
    selectionToRestore.Select
End Sub


Function GetFileHeaderString() As String
    Dim toReturn As String
    Dim frFileHeaderFile As Integer
    Dim frFileHeaderPath As String
    
    frFileHeaderPath = GetFrFileHeaderPath
    
    frFileHeaderFile = FreeFile
        
    Open frFileHeaderPath For Input As #frFileHeaderFile
    toReturn = ReadFileText(frFileHeaderFile)
    Close #frFileHeaderFile
    
    GetFileHeaderString = toReturn
End Function


Function ReadFileText(ByRef inFileNumber As Integer) As String
    Dim fileLen As Long
    fileLen = LOF(inFileNumber)
    ReadFileText = Input(fileLen, #inFileNumber)
End Function

Function GetFrFileHeaderPath() As String

    Dim filePath As String
    
    filePath = FileUtils.GetWorkbookDirPath(ActiveWorkbook)
    filePath = filePath + "\ExcelMacros\FrFileHeader.txt"

    GetFrFileHeaderPath = filePath
End Function
