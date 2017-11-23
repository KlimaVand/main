function amount = dataManagementWithMorePoints(point,ManureSetXLSX,ManureXLSX,CarbonXLSX,PlantSetXLSX,dataSetXLSX,inputXLSX, dataXLSX,kvadratnetsdataXLSX)

pointList=xlsread(point);
lenght=size(pointList);
for j=1:lenght(1,1)
nr=pointList(j);

dataManagement(nr,ManureSetXLSX,ManureXLSX,CarbonXLSX,PlantSetXLSX,dataSetXLSX,inputXLSX, dataXLSX,kvadratnetsdataXLSX)
pause(1)
end

end