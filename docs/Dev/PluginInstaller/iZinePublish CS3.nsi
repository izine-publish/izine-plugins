#//========================================================================================
#//	
#//	$HeadURL: svn://localhost/izine/iZinePlns/Docs/Dev/PluginInstaller/iZinePublish%20CS3.nsi $
#//	$Revision: 2480 $
#//	$Date: 2010-11-09 13:11:21 +0100 (Tue, 09 Nov 2010) $
#//	$Author: rajkumar.sehrawat $
#//	
#//	Creator: Raj Kumar Sehrawat
#//	Created: 8-11-2010
#//	Copyright: 2008-2010 iZine Publish. All rights reserved.
#//	
#//	Description:
#//========================================================================================


!include LogicLib.nsh

#product name
Name "iZine Publish"

# name the installer
outFile "iZinePublish.exe"
 
 
VIAddVersionKey  "ProductName" "iZine Publish"
VIAddVersionKey  "Comments" "Installs iZinePublish plug-ins for InDesign/InCopy CS3."
VIAddVersionKey  "CompanyName" "iZine Publish"
VIAddVersionKey  "LegalCopyright" "© 2008-2010 iZine Publish. All rights reserved."
VIAddVersionKey  "FileDescription" "Installer for iZinePublish"


!include iZPInstallerVersion.nsh

# Text shown at the bottom of window.
BrandingText "iZine Publish Installer"

!define TEMP1 $R0 ;Temp variable


#----------------------------------------------------------------------------------------------#
# Installed path can be found using the class ID.
	# HKEY_CLASSES_ROOT\
	# InDesign Reg Paths
	#CS3	CLSID\{1041D8C8-B5E8-49c1-A0DD-9E593B1EB5D5}\LocalServer32
	#CS4	CLSID\{4D8E7D87-0103-42f6-BE82-598C7E90A42C}\LocalServer32
	#CS5	CLSID\{ED36EA7C-9E21-43dc-BF9D-7E1879734358}\LocalServer32

	# InCopy Reg Paths
	#CS3	CLSID\{333C0F34-917D-4a47-A6C5-20E5A0C30A73}\LocalServer32
	
Var RegInstPath_InDn
Var RegInstPath_InCp

Function ReadRegInDesign
	
	ReadRegStr $RegInstPath_InDn HKCR "CLSID\{1041D8C8-B5E8-49c1-A0DD-9E593B1EB5D5}\LocalServer32" ""
	DetailPrint "InDesign CS3 path is : $RegInstPath_InDn"
	
	StrLen $9 $RegInstPath_InDn
	DetailPrint "InDesign CS3 path len = $9"
	
	${If} $9 > 16
		StrCpy $RegInstPath_InDn $RegInstPath_InDn -13 # path without "InDesign.exe"
		DetailPrint "InDesign CS3 dir is : $RegInstPath_InDn"
	${Else}
		StrCpy $RegInstPath_InDn ""
	${Endif}

FunctionEnd


Function ReadRegInCopy

	ReadRegStr $RegInstPath_InCp HKCR "CLSID\{333C0F34-917D-4a47-A6C5-20E5A0C30A73}\LocalServer32" ""
	DetailPrint "InCopy CS3 path is : $RegInstPath_InCp"
	
	StrLen $9 $RegInstPath_InCp
	DetailPrint "InCopy CS3 path len = $9"
	
	${If} $9 > 14
		StrCpy $RegInstPath_InCp $RegInstPath_InCp -11 # path without "InCopy.exe"
		DetailPrint "InCopy CS3 dir is : $RegInstPath_InCp"
	${Else}
		StrCpy $RegInstPath_InCp ""
	${Endif}

FunctionEnd

Function CopyInstallationFile

	# define output path to application folder
		setOutPath $INSTDIR
	DetailPrint "Installing files to : $INSTDIR"

	# Copy the dependency files to application folder
		file libeay32.dll
		file ssleay32.dll
		file zlibwapi.dll

	# define output path to plug-ins folder
		setOutPath $INSTDIR\Plug-Ins


	# Copy the plugin files
		file iZinePublish.pln
		file iZinePublishUI.pln


	# define uninstaller name
		writeUninstaller "$INSTDIR\unInstaller iZinePublish.exe"
 
FunctionEnd

#----------------------------------------------------------------------------------------------#
; Show install details
ShowInstDetails show

;Things that need to be extracted on startup (keep these lines before any File command!)
;Only useful for BZIP2 compression
;Use ReserveFile for your own InstallOptions INI files too!

ReserveFile "${NSISDIR}\Plugins\InstallOptions.dll"
ReserveFile "Components.ini"

Function .onInit
	
	;Extract InstallOptions files
	;$PLUGINSDIR will automatically be removed when the installer closes
	  
  	InitPluginsDir
  	File /oname=$PLUGINSDIR\Components.ini "Components.ini"
	
	# Read the registry for install paths
	Call ReadRegInDesign
	Call ReadRegInCopy

	Call UpdateComponentFile
	
	#MessageBox MB_OK "No installation found for Adobe InDesign or InCopy. Installation will quit."
	#Abort ; causes installer to quit.
 FunctionEnd


#----------------------------------------------------------------------------------------------#
;Order of pages
Page custom CreatePageComponents ValidatePageComponent ": Install Options" ;
Page instfiles


Function CreatePageComponents

  ;Display the InstallOptions dialog

  Push ${TEMP1}

    InstallOptions::dialog "$PLUGINSDIR\Components.ini"
    Pop ${TEMP1}
  
  Pop ${TEMP1}

FunctionEnd

Function ValidatePageComponent

	StrCpy $8 0
	ReadINIStr ${TEMP1} "$PLUGINSDIR\Components.ini" "Field 2" "State"
	${If} ${TEMP1} == 1
		ReadINIStr ${TEMP1} "$PLUGINSDIR\Components.ini" "Field 3" "State"
		StrLen $9 ${TEMP1}
		${If} $9 <= 3
			MessageBox MB_ICONEXCLAMATION|MB_OK "You must provide InDesign application path."
			Abort
		${Endif}
		StrCpy $8 1
	${Endif}

	ReadINIStr ${TEMP1} "$PLUGINSDIR\Components.ini" "Field 5" "State"
	${If} ${TEMP1} == 1
		ReadINIStr ${TEMP1} "$PLUGINSDIR\Components.ini" "Field 6" "State"
		StrLen $9 ${TEMP1}
		${If} $9 <= 3
			MessageBox MB_ICONEXCLAMATION|MB_OK "You must provide InCopy application path."
			Abort
		${Endif}
		StrCpy $8 1
	${Endif}
	
	${If} $8 == 0
		MessageBox MB_ICONEXCLAMATION|MB_OK "Atleast one option must be selected."
		Abort
	${Endif}

FunctionEnd

Function UpdateComponentFile

	StrLen $9 $RegInstPath_InDn
		
	${If} $9 > 0
		WriteINIStr "$PLUGINSDIR\Components.ini" "Field 2" "State" 1
		WriteINIStr "$PLUGINSDIR\Components.ini" "Field 3" "State" $RegInstPath_InDn
	${Endif}
	
	StrLen $9 $RegInstPath_InCp
			
	${If} $9 > 0
		WriteINIStr "$PLUGINSDIR\Components.ini" "Field 5" "State" 1
		WriteINIStr "$PLUGINSDIR\Components.ini" "Field 6" "State" $RegInstPath_InCp
	${Endif}
	
FunctionEnd

#----------------------------------------------------------------------------------------------#
# default section start
section
	
	DetailPrint "InDesign CS3 dir is : $RegInstPath_InDn"
	DetailPrint "InCopy CS3 dir is : $RegInstPath_InCp"
	
	
!If 0	# Enable this to test copying of files to registry read path
	# InstallDir is not allowed inside section, so updating the variable directly
	StrCpy $INSTDIR $RegInstPath_InDn
	
	Call CopyInstallationFile
!Else	
	ReadINIStr ${TEMP1} "$PLUGINSDIR\Components.ini" "Field 2" "State"
	${If} ${TEMP1} == 1
		ReadINIStr ${TEMP1} "$PLUGINSDIR\Components.ini" "Field 3" "State"
		${If} $9 > 3
			StrCpy $INSTDIR ${TEMP1}
				
			Call CopyInstallationFile
		${Endif}
	${Endif}
	
	ReadINIStr ${TEMP1} "$PLUGINSDIR\Components.ini" "Field 5" "State"
	${If} ${TEMP1} == 1
		ReadINIStr ${TEMP1} "$PLUGINSDIR\Components.ini" "Field 6" "State"
		StrLen $9 ${TEMP1}
		${If} $9 > 3
			StrCpy $INSTDIR ${TEMP1}
							
			Call CopyInstallationFile
		${Endif}
	${Endif}
!Endif
	
# default section end
sectionEnd

InstallDir $RegInstPath_InDn

#----------------------------------------------------------------------------------------------#
# create a section to define what the uninstaller does.
# the section will always be named "Uninstall"
section "Uninstall"
 
	# Always delete uninstaller first
		delete "$INSTDIR\unInstaller iZinePublish.exe"

	# now delete installed file
		delete $INSTDIR\libeay32.dll
		delete $INSTDIR\ssleay32.dll
		delete $INSTDIR\zlibwapi.dll

		delete $INSTDIR\Plug-Ins\iZinePublish.pln
		delete $INSTDIR\Plug-Ins\iZinePublishUI.pln

sectionEnd
#----------------------------------------------------------------------------------------------#