function amount = dataManagementWithMorePoints(point,clayContentXLSX,subSoil,ManureSetXLSX,ManureXLSX,CarbonXLSX,PlantSetXLSX,dataSetXLSX,inputXLSX, dataXLSX,kvadratnetsdataXLSX)

pointList=xlsread(point);
lenght=size(pointList);

       
runFile = fopen('output.txt','w');
for j=1:lenght(1,1)

  
nr=pointList(j);

result=dataManagement3(nr,clayContentXLSX,subSoil,ManureSetXLSX,ManureXLSX,CarbonXLSX,PlantSetXLSX,dataSetXLSX,inputXLSX, dataXLSX,kvadratnetsdataXLSX);
fprintf(runFile,'%s\t%d\r\n','point',nr);
fprintf(runFile,'%s\t%d\t%d\r\n','Result',result);
pause(1)

end
fclose(runFile);
end