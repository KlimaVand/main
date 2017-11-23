@echo off
multicoreFasset.exe
Set _InputFile=cores.txt
For /F "tokens=*" %%I IN (%_InputFile%) DO (
Set /a NumberOfDirs=%%I
)
echo read from file "%NumberOfDirs%"
FOR /L %%G IN (0,1,%NumberOfDirs% ) DO (
copy fasset.exe fasset%%G

)
FOR /L %%G IN (0,1,%NumberOfDirs% ) DO (
cd fasset%%G
call "cmd /c start fasset.exe"
cd ..
)

del cores.txt
pause