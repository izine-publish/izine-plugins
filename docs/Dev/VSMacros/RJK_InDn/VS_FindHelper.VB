'//========================================================================================

'//	

'//	$HeadURL: $

'//	$Revision: $

'//	$Date: $

'//	$Author: $

'//	

'//	Creator: Raj Kumar Sehrawat

'//	Created: 19-3-2010

'//	

'//	Description:

'//========================================================================================

Option Explicit On



Imports System

Imports EnvDTE

Imports EnvDTE80

Imports System.Diagnostics



Public Module VS_FindHelper

	Dim gFindResult As vsFindResult



	Public Function PerformFindActionExact(ByVal findStr As String _

	, Optional ByVal inMatchCase As Boolean = True _

	, Optional ByVal inWholeWord As Boolean = True _

	, Optional ByVal isRegex As Boolean = False) As Boolean



		Dim findResult As vsFindResult

		'DTE.ExecuteCommand("Edit.Find")

		With DTE.Find

			.FindWhat = findStr

			.Target = vsFindTarget.vsFindTargetCurrentDocument

			.MatchCase = inMatchCase

			.MatchWholeWord = inWholeWord

			.Backwards = False

			.MatchInHiddenText = True

			If (isRegex = True) Then

				.PatternSyntax = vsFindPatternSyntax.vsFindPatternSyntaxRegExpr

			Else

				.PatternSyntax = vsFindPatternSyntax.vsFindPatternSyntaxLiteral

			End If

			.Action = vsFindAction.vsFindActionFind



			findResult = ExecuteFindAndWaitTillResult()

			If (findResult = vsFindResult.vsFindResultFound) Then PerformFindActionExact = True



			'Close the find window

			'CloseFindWindow()

		End With

	End Function



	Function FindInFiles(ByVal inFindStr As String _

	 , Optional ByVal isRegex As Boolean = False _

	 , Optional ByVal inMatchCase As Boolean = True, Optional ByVal inWholeWord As Boolean = True _

	 , Optional ByVal inSearchInFilesPattern As String = "*.h;*.cpp;*.fr;*.fri" _

	 , Optional ByVal inFindLocation As EnvDTE.vsFindResultsLocation = vsFindResultsLocation.vsFindResults2) As Boolean



		Dim findResult As vsFindResult

		With DTE.Find

			DTE.ExecuteCommand("Edit.FindinFiles")

			DTE.Windows.Item("{CF2DDC32-8CAD-11D2-9302-005345000000}").Activate() 'Find and Replace



			.FindWhat = inFindStr

			If (isRegex = True) Then

				.PatternSyntax = vsFindPatternSyntax.vsFindPatternSyntaxRegExpr

			Else

				.PatternSyntax = vsFindPatternSyntax.vsFindPatternSyntaxLiteral

			End If



			.MatchCase = inMatchCase

			.MatchWholeWord = inWholeWord

			.MatchInHiddenText = True



			.Target = vsFindTarget.vsFindTargetCurrentProject

			.SearchPath = "Current Project"

			.SearchSubfolders = True



			.FilesOfType = inSearchInFilesPattern

			.ResultsLocation = inFindLocation

			.Action = vsFindAction.vsFindActionFindAll





			findResult = ExecuteFindAndWaitTillResult()

			If (findResult = vsFindResult.vsFindResultFound) Then FindInFiles = True



			CloseFindWindow()

		End With

	End Function



	Function ReplaceInCurrDoc(ByRef inSearchString As String, ByRef inReplaceString As String _

	 , Optional ByVal inReplaceAll As Boolean = False) As Boolean



		Dim findAction As vsFindAction = vsFindAction.vsFindActionReplace

		If inReplaceAll Then findAction = vsFindAction.vsFindActionReplaceAll



		Dim findResult As vsFindResult

		findResult = DTE.Find.FindReplace(findAction, inSearchString, vsFindOptions.vsFindOptionsNone, inReplaceString, vsFindTarget.vsFindTargetCurrentDocument)



		If (findResult = vsFindResult.vsFindResultReplaced) Then ReplaceInCurrDoc = True



	End Function



	Function ExecuteFindAndWaitTillResult() As vsFindResult

		Dim findResult As vsFindResult

		'

		' Perform find and wait till result is available.

		'

		gFindResult = vsFindResult.vsFindResultPending

		findResult = DTE.Find.Execute()



		If (findResult = vsFindResult.vsFindResultFound) Then

			ExecuteFindAndWaitTillResult = findResult

			Exit Function

		End If

		System.Threading.Thread.Sleep(1000)

		If (findResult = vsFindResult.vsFindResultPending) Then

			gFindResult = findResult

			Do

				If (gFindResult = vsFindResult.vsFindResultPending) Then

					System.Threading.Thread.Sleep(100)

				Else

					findResult = gFindResult

					Exit Do

				End If

			Loop

		End If

		ExecuteFindAndWaitTillResult = findResult



	End Function

	Public Sub CloseFindWindow()

		DTE.Windows.Item("{CF2DDC32-8CAD-11D2-9302-005345000000}").Close()

	End Sub



End Module

