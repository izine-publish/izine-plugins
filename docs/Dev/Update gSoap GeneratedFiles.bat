@ echo off
set batchFileDir=%CD%
set wsdlUrl=http://192.168.1.19/iZINE.Web.MVC/server.svc?wsdl
Rem set wsdlurl=http://izine.redfive-labs.biz/server.svc?wsdl

Rem gSoap directory path from the current location, i.e from where batch file is being run.
set gSoapParDir=..\..\..\izine.ui\External\gsoap-2.7

Rem directory path where output will be copied.
set trunkDir=..\..\Codebase\Trunk

Rem user/machine specific settings are above this line.

echo Reminder: you mush have modified goap\ws\typemap.dat for the following line
echo "xsd__string = | wchar_t* | wchar_t*"

Rem following line will echo a empty line
echo:

cd %gSoapParDir%
cd gsoap\bin\win32

set generatedFilesDir=%CD%

:Ask1
	echo This will update the gSoap generated files from
	echo %wsdlUrl%
	echo:
	set /p userinp=Do you want to Continue? (Y/N/S(Skip)) :

	if "%userinp%" == "Y" goto Ask1Yes
	if "%userinp%" == "y" goto Ask1Yes
   	if "%userinp%" == "N" goto exitError
	if "%userinp%" == "n" goto exitError
	if "%userinp%" == "s" goto Ask2
	if "%userinp%" == "S" goto Ask2
	goto ask1

:Ask1Yes

Rem following line will echo a empty line
echo:

wsdl2h -egxy -s -o server.h -I ../../WS %wsdlUrl%
if not %ErrorLevel% == 0 goto exitErrorWsdl2h

echo server.h file created. Now modify it for 
ECHO * Import                                                                     *

Rem following line will echo a empty line
echo:

ECHO #import "WS-Header.h"
ECHO #import "wsse.h"
ECHO #import "xop.h"
ECHO #import "xmlmime.h"
Rem ECHO #import "dom.h"

echo:
echo Did you updated server.h?

start notepad server.h
:Ask2
	set userinp="n"
	echo:
	echo Generating soap client files.
	set /p userinp=Continue? (Y/N/S(Skip)) :
	if "%userinp%" == "Y" goto Ask2Yes
	if "%userinp%" == "y" goto Ask2Yes
   	if "%userinp%" == "N" goto exitError
	if "%userinp%" == "n" goto exitError
	if "%userinp%" == "s" goto :copyFiles
	if "%userinp%" == "S" goto :copyFiles
	goto Ask2		
:Ask2Yes

echo:

soapcpp2 -x -w -C -s -L -I ../../import server.h
if not %ErrorLevel% == 0 goto exitErrorsoapcpp2

echo gSoap files generated.

:copyFiles
rem Copy Generated files to trunk folder

echo:
echo Copying generated files to trunk folder
echo Press Ctrl+C to stop.
echo: 
pause

cd %batchFileDir%
set destDir=iZinePublish\Source\WS\gSoapGenerated


set file1=soapH.h
set file2=soapC.cpp
set file3=soapStub.h
set file4=soapClient.cpp
set file5=soapCustomBinding_USCOREserverProxy.h
set file6=CustomBinding_USCOREserver.nsmap

@ echo on

copy "%generatedFilesDir%\%file1%"	"%trunkDir%\%destDir%\"
copy "%generatedFilesDir%\%file2%"	"%trunkDir%\%destDir%\"
copy "%generatedFilesDir%\%file3%"	"%trunkDir%\%destDir%\"
copy "%generatedFilesDir%\%file4%"	"%trunkDir%\%destDir%\"
copy "%generatedFilesDir%\%file5%"	"%trunkDir%\%destDir%\"
copy "%generatedFilesDir%\%file6%"	"%trunkDir%\%destDir%\"

@ echo off
goto exitNormal


:exitErrorWsdl2h
echo:
echo server.h file not created.
echo wsdl2h retuned error, ErrorLevel is %errorlevel%
goto exitError

:exitErrorsoapcpp2
echo:
echo soap files not created.
echo soapcpp2 retuned error, ErrorLevel is %errorlevel%
goto exitError

:exitError
cd %batchFileDir%

echo:
echo Warning : gSoap files not updated.
pause
goto exitEnd

:exitNormal

echo:
echo gSoap files updated.
pause
goto exitEnd

:exitEnd