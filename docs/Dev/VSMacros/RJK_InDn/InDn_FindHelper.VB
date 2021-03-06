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



Friend Module InDn_FindHelper

	Function FindInDnIDDeclaration(Optional ByVal forward As Boolean = False) As Boolean

		DTE.ExecuteCommand("Edit.Find")

		With DTE.Find

			.FindWhat = "DECLARE_PMID"

			.Target = vsFindTarget.vsFindTargetCurrentDocument

			'.MatchCase = False

			'.MatchWholeWord = False

			.MatchInHiddenText = True

			.Backwards = Not forward

			.PatternSyntax = vsFindPatternSyntax.vsFindPatternSyntaxLiteral

			.Action = vsFindAction.vsFindActionFind

			If (forward) Then

				DTE.ExecuteCommand("Edit.FindNext")

			Else

				DTE.ExecuteCommand("Edit.FindPrevious")

			End If

			VS_FindHelper.CloseFindWindow()

		End With

	End Function



End Module



