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

Imports System

Imports EnvDTE

Imports EnvDTE80

Imports System.Diagnostics



Friend Module VS_Clipboard

	Private clipText As String



	Public Property ClipboardText() As String

		Get

			RunThread(AddressOf GetClipboardText)

			Return clipText

		End Get

		Set(ByVal value As String)

			clipText = value

			RunThread(AddressOf CopyToClipboard)

		End Set

	End Property



	Private Function RunThread(ByVal fct As Threading.ThreadStart)

		Dim thread As New Threading.Thread(fct)

		thread.ApartmentState = Threading.ApartmentState.STA



		thread.Start()

		thread.Join()

	End Function



	Private Sub GetClipboardText()

		clipText = My.Computer.Clipboard.GetText()

	End Sub



	Private Sub CopyToClipboard()

		'The second parameter is required

		My.Computer.Clipboard.SetText(clipText, True)

	End Sub



End Module

