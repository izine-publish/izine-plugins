Attribute VB_Name = "CellIterHelper"
Option Explicit

Public Function NextRowCell(inCell As Range) As Range
    
    If (inCell Is Nothing) Then Exit Function
    Set NextRowCell = inCell.Offset(1, 0)
        
End Function
