

FOR /L %%G IN (0,1,30 ) DO (copy test.exe scenario%%G)


FOR /L %%G IN (0,1,30) DO (
cd scenario%%G
test.exe
echo done for %%G
cd ..
)

FOR /L %%G IN (0,1,30) DO (
cd scenario%%G
cp output.txt ../output%%G.txt
cd ..
)
del NumberOfDirs.txt
echo done