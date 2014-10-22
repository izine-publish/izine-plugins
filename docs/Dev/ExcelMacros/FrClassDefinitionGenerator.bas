Attribute VB_Name = "FrClassDefinitionGenerator"
Option Explicit


Function InDn_ClassDefinition(inDefinitionType As String, inBoss As String, inInheritFrom As String, inInterfaceCellAddress As Range) As String
'
' InDn_ClassDefinition Macro
' Macro to create fr class definition
' if inDefinitionType is empty, it is Class
' if inInheritFrom is empty, it is kInvalidClass
'

'
    Dim classDefinitionStr As String
    Dim cellValue As String
    Dim currInterfaceCell As Range
    Const prefixSpaces As String = vbTab
    Const interfaceLinePrefixSpaces As String = prefixSpaces + vbTab + vbTab
    
    inBoss = Trim(inBoss)
    If (Len(inBoss) <= 0) Then Exit Function

    MakeValidClassDefinitionType (inDefinitionType)
    MakeValidClassName inBoss
    MakeValidBaseClassName inInheritFrom
    
    classDefinitionStr = prefixSpaces + inDefinitionType + vbNewLine
    classDefinitionStr = classDefinitionStr + prefixSpaces + "{" + vbNewLine
    classDefinitionStr = classDefinitionStr + prefixSpaces + vbTab + inBoss + "," + vbNewLine
    classDefinitionStr = classDefinitionStr + prefixSpaces + vbTab + inInheritFrom + "," + vbNewLine
    classDefinitionStr = classDefinitionStr + prefixSpaces + vbTab + "{" + vbNewLine
    
    Set currInterfaceCell = inInterfaceCellAddress
    Do
        cellValue = currInterfaceCell.Value
        If (cellValue = "" And IsPreprocessorValue(currInterfaceCell.Previous.Value) = False) Then Exit Do
        
        classDefinitionStr = classDefinitionStr + MakeClassDefintionEntry(currInterfaceCell, currInterfaceCell.Next, currInterfaceCell.Next.Next, interfaceLinePrefixSpaces)
        
        Set currInterfaceCell = CellIterHelper.NextRowCell(currInterfaceCell)
    Loop
    classDefinitionStr = classDefinitionStr + prefixSpaces + vbTab + "}" + vbNewLine
    classDefinitionStr = classDefinitionStr + prefixSpaces + "}," + vbNewLine
    
    InDn_ClassDefinition = classDefinitionStr
End Function

Sub MakeValidClassDefinitionType(ByRef ioDefinitionType As String)
    
    'If (ioDefinitionType Is Nothing) Then ioDefinitionType = "Class"
    ioDefinitionType = Trim(ioDefinitionType)
    If (Len(ioDefinitionType) <= 0) Then ioDefinitionType = "Class"
    If (ioDefinitionType <> "Class" And ioDefinitionType <> "AddIn") Then ioDefinitionType = "Class"
    
End Sub

Sub MakeValidBaseClassName(ByRef ioBaseClass As String)
    
    MakeValidClassName ioBaseClass
    If (Len(ioBaseClass) <= 0) Then ioBaseClass = "kInvalidClass"

End Sub


Sub MakeValidClassName(ByRef ioBossName As String)
    
    'If (ioBaseClass Is Nothing) Then ioBaseClass = "kInvalidClass"
    ioBossName = Trim(ioBossName)
    If (Len(ioBossName) > 0 And Right(ioBossName, 4) <> "Boss") Then ioBossName = ioBossName + "Boss"
    If (Len(ioBossName) > 0 And Left(ioBossName, 1) <> "k") Then ioBossName = "k" + ioBossName
End Sub

Function IsPreprocessorValue(inValue As String) As Boolean
    inValue = Trim(inValue)
    If (Len(inValue) > 1 And Left(inValue, 1) = "#") Then
        IsPreprocessorValue = True
    Else
        IsPreprocessorValue = False
    End If
End Function

Function MakeClassDefintionEntry(inInterfaceRange As Range, inImplRange As Range _
, inComment As Range, inPrefixSpace As String) As String
    Dim strInterface As String
    Dim strImpl As String
    Dim strComment As String
    Dim toReturn As String
    
    strInterface = inInterfaceRange.Value
    strImpl = inImplRange.Value
    strComment = inComment.Value
    
    MakeProperInterfaceName strInterface
    MakeProperImpName strImpl
    
    If (Len(strInterface) <= 0 Or Len(strImpl) <= 0) Then
        If (IsPreprocessorValue(inInterfaceRange.Previous.Value)) Then
            toReturn = inInterfaceRange.Previous.Value
        Else
            Exit Function
        End If
    Else
        toReturn = inPrefixSpace + strInterface + "," + vbTab + strImpl + ","
    End If
    strComment = Trim(strComment)
        
    
    If (Len(strComment) > 0) Then toReturn = toReturn + vbTab + "// " + strComment
    toReturn = toReturn + vbNewLine
    'Comment the line if not in use
    If (inInterfaceRange.Previous.Value = "//") Then toReturn = "//" + toReturn
    
    MakeClassDefintionEntry = toReturn
End Function

Private Sub MakeProperInterfaceName(ByRef ioInterfaceName As String)
    ioInterfaceName = Trim(ioInterfaceName)
    If (Len(ioInterfaceName) <= 0) Then Exit Sub
    If (Left(ioInterfaceName, 4) <> "IID_") Then ioInterfaceName = "IID_" + ioInterfaceName
    ioInterfaceName = StrConv(ioInterfaceName, vbUpperCase)
End Sub

Private Sub MakeProperImpName(ByRef ioImplName As String)
    ioImplName = Trim(ioImplName)
    If (Len(ioImplName) <= 0) Then Exit Sub
    If (Left(ioImplName, 1) <> "k") Then ioImplName = "k" + ioImplName
    If (Right(ioImplName, 4) <> "Impl") Then ioImplName = ioImplName + "Impl"
End Sub
