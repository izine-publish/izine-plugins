echo this will overwrite the cmdui.prf file in the current directory
pause
attrib -R CmdUI.PRF
copy /B /Y "%appdata%\Microsoft\VisualStudio\8.0\1033\CmdUI.PRF" .\
pause
