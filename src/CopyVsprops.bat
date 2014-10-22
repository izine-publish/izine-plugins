@ echo off

cd iZinePublish\Project
IF NOT EXIST iZineCS3Debug_.vsprops echo iZineCS3Debug_.vsprops not found
IF NOT EXIST iZineCS3Debug_.vsprops GOTO replaceNext_UI
echo:
echo deleting iZineCS3Debug.vsprops and replacing it with iZineCS3Debug_.vsprops
echo:
pause
echo:
del iZineCS3Debug.vsprops
copy iZineCS3Debug_.vsprops	iZineCS3Debug.vsprops

:replaceNext_UI
cd ..\..

cd iZinePublishUI\Project
IF NOT EXIST iZineUICS3Debug_.vsprops echo iZineUICS3Debug_.vsprops not found
IF NOT EXIST iZineUICS3Debug_.vsprops GOTO replaceNext_Export
echo:
echo deleting iZineUICS3Debug.vsprops and replacing it with iZineUICS3Debug_.vsprops
echo:
pause
echo:
del iZineUICS3Debug.vsprops
copy iZineUICS3Debug_.vsprops iZineUICS3Debug.vsprops
echo:

:replaceNext_Export
cd ..\..

cd iZineExport\Project
IF NOT EXIST iZineExportCS3Debug_.vsprops echo iZineExportCS3Debug_.vsprops not found
IF NOT EXIST iZineExportCS3Debug_.vsprops GOTO exitBatch
echo:
echo deleting iZineExportCS3Debug.vsprops and replacing it with iZineExportCS3Debug_.vsprops
echo:
pause
echo:
del iZineExportCS3Debug.vsprops
copy iZineExportCS3Debug_.vsprops iZineExportCS3Debug.vsprops
echo:

:exitBatch
pause
