function result = dataManagement3(Punktno,clayContentXLSX,lowerlayerXLSX,ManureSetXLSX,ManureXLSX,CarbonXLSX,PlantSetXLSX,dataSetXLSX,inputXLSX, dataXLSX,kvadratnetsdataXLSX)
addpath('..')

kvadratnetsdata =xlsread(kvadratnetsdataXLSX);
length=size(kvadratnetsdata);
j=1;
while j<=length(1,1)
    if(kvadratnetsdata(j,1)==Punktno)
        TemperatureXLSX=strcat(num2str(kvadratnetsdata(j,4)),'.xlsx');

        break;
    end
    j=j+1;
end
dataSet=xlsread(dataSetXLSX);
input=xlsread(inputXLSX);
lowerlayer=xlsread(lowerlayerXLSX);
defaultUpper=-1;
defaultLower=-1;
length=size(lowerlayer);
Data1986=-1;
Data2009=-1;
j=1;
while j<=length(1,1)
   if(lowerlayer(j,1)==Punktno);
       Data1986=lowerlayer(j,8);
       Data2009=lowerlayer(j+1,8);
       break;
   else
       if(lowerlayer(j,1)>Punktno)
           break;
       end
       while(lowerlayer(j,1)<Punktno)

           j=j+1;

           
       end
       
      
   end
end
length=size(dataSet);
TransportHum = fopen('errormessage.txt','w');
j=1;
soilNr=-1;
FractionLayer=-1;
while j<=length(1,1)
   if(dataSet(j,1)==Punktno);
       soilNr=dataSet(j,6);

       input(6)=-1;
       Data1986Upper=dataSet(j,8);
       Data1986Lower=dataSet(j+1,8)+Data1986;
       FractionLayer=Data1986Upper/(Data1986Upper+Data1986Lower);
       Data2009Upper=dataSet(j+4,8);
       Data2009Lower=dataSet(j+5,8)+Data2009;
       input(7)=(dataSet(j,3)/dataSet(j,4)+dataSet(j+2,3)/dataSet(j+2,4))/2;
       input(9)=FractionLayer;
       %input(1)=min(56.2*input(7)^-1.69,1);
       %input(4)=min(56.2*input(7)^-1.69,1);
       dirname=strcat('test',num2str(Punktno));
       mkdir(dirname);
       fileName=strcat(dirname,'/');
       fileName=strcat(fileName,dirname);
       fileName=strcat(fileName,'.xls');
       newdataXLSX=strcat(dirname,'/');
       newdataXLSX=strcat(newdataXLSX,dataXLSX);

       
       runFileName =strcat(dirname,'\run.txt');
       runFile = fopen(runFileName,'w');
       fprintf(runFile,'%s\t%s\r\n','input',fileName);
       fprintf(runFile,'%s\t%s\r\n','data',newdataXLSX);
       fprintf(runFile,'%s\t%s\r\n','TemperatureData',TemperatureXLSX);
       fprintf(runFile,'%s\t%s\r\n','outputDir',dirname);
       fclose(runFile);
        length2=size(dataSet);
        
       while(dataSet(j,1)==Punktno&&j<length2(1,1))
            j=j+1;
       end
        if(j==length2(1,1))
           break;
        end
   else
       if(dataSet(j,1)>Punktno)
           break;
       end
       try
       while(dataSet(j,1)<Punktno)

           j=j+1;
            
           
       end
       catch err
           result=err;
           break;
       end
       
      
   end
end
if(isnan(soilNr))
    soilNr=-1;
end
if (soilNr==-1)
fprintf(TransportHum,'%s','soilnr Missing');
'soilnr Missing'
fclose(TransportHum);
result='null';
end
if(soilNr~=-1)
    
    ClayDataSet=xlsread(clayContentXLSX);
    Clay=-1;
    fHum=-1;
    switch (soilNr)
                   case 1
                       Clay=ClayDataSet(1,1);
                        fHum=ClayDataSet(2,1);
                       case 2
                       Clay=ClayDataSet(1,1);
                        fHum=ClayDataSet(2,1);
     
                       case 3
                       Clay=ClayDataSet(1,1);
                        fHum=ClayDataSet(2,1);
           
                       case 4
                       Clay=ClayDataSet(1,1);
                        fHum=ClayDataSet(2,1);
                       case 5
                      
                        Clay=ClayDataSet(1,2);
                        fHum=ClayDataSet(2,2);
                       case 6
                       Clay=ClayDataSet(1,2);
                        fHum=ClayDataSet(2,2);
                       
                       case 7
                       Clay=ClayDataSet(1,3);
                        fHum=ClayDataSet(2,3);
                     
                       case 8
                       Clay=ClayDataSet(1,3);
                        fHum=ClayDataSet(2,3);
                       
                       case 9
                       Clay=ClayDataSet(1,3);
                        fHum=ClayDataSet(2,3);
                      
                       case 10
                       Clay=ClayDataSet(1,4);
                        fHum=ClayDataSet(2,4);
                       
                       case 11
                       Clay=ClayDataSet(1,4);
                        fHum=ClayDataSet(2,4);
                       
                       case 12
                       Clay=ClayDataSet(1,4);
                        fHum=ClayDataSet(2,4);
                       
        otherwise
            ;
    end
               
input(15)=Clay;
input(6)=Data1986Upper+Data1986Lower;
input(22)=fHum;  
input(25)=Clay;
xlswrite(fileName,input);
PlantSet=xlsread(PlantSetXLSX);
length=size(PlantSet);
Carbon=xlsread(CarbonXLSX);
j=1;
plantPlace=1;       
while j<=length(1,1)

   if(PlantSet(j,1)==Punktno);
       
       k=1;
       lengthCarbon=size(Carbon);
       plantID=PlantSet(j,5);

       while k<=lengthCarbon(1,1)
        yieldAmount1(plantPlace)=-1;
       yieldAmount2(plantPlace)=-1;
           if(plantID==Carbon(k,1))
               switch (soilNr)
                   case 1
                        yieldAmount1(plantPlace) = Carbon(k,2);
                        yieldAmount2(plantPlace) = Carbon(k,6);
                        break;
                    case 2
                        yieldAmount1(plantPlace) = Carbon(k,2);
                        yieldAmount2(plantPlace) = Carbon(k,6);
                        break;
                    case 3
                        yieldAmount1(plantPlace) = Carbon(k,2);
                        yieldAmount2(plantPlace) = Carbon(k,6);
                        break;
                    case 4
                        yieldAmount1(plantPlace) = Carbon(k,2);
                        yieldAmount2(plantPlace) = Carbon(k,6);
                        break;
                    case 5
                        yieldAmount1(plantPlace) = Carbon(k,3);
                        yieldAmount2(plantPlace) = Carbon(k,7);
                    case 6
                        yieldAmount1(plantPlace) = Carbon(k,3);
                        yieldAmount2(plantPlace) = Carbon(k,7);
                        break;
                    case 7
                        yieldAmount1(plantPlace) = Carbon(k,4);
                        yieldAmount2(plantPlace) = Carbon(k,8);
                        break;
                    case 8
                        yieldAmount1(plantPlace) = Carbon(k,4);
                        yieldAmount2(plantPlace) = Carbon(k,8);
                    case 9
                        yieldAmount1(plantPlace) = Carbon(k,4);
                        yieldAmount2(plantPlace) = Carbon(k,8);
                        break;
                   case 10
                       yieldAmount1(plantPlace) = Carbon(k,5);
                       yieldAmount2(plantPlace) = Carbon(k,9);
                       break;
                   case 11
                       yieldAmount1(plantPlace) = Carbon(k,5);
                       yieldAmount2(plantPlace) = Carbon(k,9);
                       break;
                   case 12
                       yieldAmount1(plantPlace) = Carbon(k,5);
                       yieldAmount2(plantPlace) = Carbon(k,9);
                       break;
                   otherwise

                                    break;
               end
           end
               k=k+1;
       end
       
           
           plantID=10;
           k=1;
           while k<=lengthCarbon(1,1)
           
           if(plantID==Carbon(k,1))
               switch (soilNr)
                    case 1
                        defaultUpper= Carbon(k,2);
                        defaultLower= Carbon(k,6);
                        if(yieldAmount1(plantPlace)==-1)
                        yieldAmount1(plantPlace) = Carbon(k,2);
                        yieldAmount2(plantPlace) = Carbon(k,6);
                        end
                    case 2
                        defaultUpper= Carbon(k,2);
                        defaultLower= Carbon(k,6);
                        if(yieldAmount1(plantPlace)==-1)
                        yieldAmount1(plantPlace) = Carbon(k,2);
                        yieldAmount2(plantPlace) = Carbon(k,6);
                        end
                    case 3
                        defaultUpper= Carbon(k,2);
                        defaultLower= Carbon(k,6);
                       if(yieldAmount1(plantPlace)==-1)
                        yieldAmount1(plantPlace) = Carbon(k,2);
                        yieldAmount2(plantPlace) = Carbon(k,6);
                        end
                    case 4
                        defaultUpper= Carbon(k,2);
                        defaultLower= Carbon(k,6);
                       if(yieldAmount1(plantPlace)==-1)
                        yieldAmount1(plantPlace) = Carbon(k,2);
                        yieldAmount2(plantPlace) = Carbon(k,6);
                        end
                        break;
                    case 5
                        defaultUpper= Carbon(k,3);
                        defaultLower= Carbon(k,7);
                        if(yieldAmount1(plantPlace)==-1)
                        yieldAmount1(plantPlace) = Carbon(k,3);
                        yieldAmount2(plantPlace) = Carbon(k,7);
                        end
                    case 6
                        defaultUpper= Carbon(k,3);
                        defaultLower= Carbon(k,7);
                       if(yieldAmount1(plantPlace)==-1)
                        yieldAmount1(plantPlace) = Carbon(k,3);
                        yieldAmount2(plantPlace) = Carbon(k,7);
                        end
                        break;
                    case 7
                        defaultUpper= Carbon(k,4);
                        defaultLower= Carbon(k,8);
                       if(yieldAmount1(plantPlace)==-1)
                        yieldAmount1(plantPlace) = Carbon(k,4);
                        yieldAmount2(plantPlace) = Carbon(k,8);
                        end
                    case 8
                        defaultUpper= Carbon(k,4);
                        defaultLower= Carbon(k,8);
                        if(yieldAmount1(plantPlace)==-1)
                        yieldAmount1(plantPlace) = Carbon(k,4);
                        yieldAmount2(plantPlace) = Carbon(k,8);
                        end
                    case 9
                        defaultUpper= Carbon(k,4);
                        defaultLower= Carbon(k,8);
                        if(yieldAmount1(plantPlace)==-1)
                        yieldAmount1(plantPlace) = Carbon(k,4);
                        yieldAmount2(plantPlace) = Carbon(k,8);
                        end
                        break;
                   case 10
                       defaultUpper= Carbon(k,5);
                        defaultLower= Carbon(k,9);
                       if(yieldAmount1(plantPlace)==-1)
                        yieldAmount1(plantPlace) = Carbon(k,5);
                        yieldAmount2(plantPlace) = Carbon(k,9);
                        end
                   case 11
                       defaultUpper= Carbon(k,5);
                        defaultLower= Carbon(k,9);
                      if(yieldAmount1(plantPlace)==-1)
                        yieldAmount1(plantPlace) = Carbon(k,5);
                        yieldAmount2(plantPlace) = Carbon(k,9);
                        end
                   case 12
                       defaultUpper= Carbon(k,5);
                        defaultLower= Carbon(k,9);
                       if(yieldAmount1(plantPlace)==-1)
                        yieldAmount1(plantPlace) = Carbon(k,5);
                        yieldAmount2(plantPlace) = Carbon(k,9);
                        end
                       break;
                   otherwise

                                    break;
               end
           end
               k=k+1;
               
           end 
           plantPlace=plantPlace+1;

   end
     j=j+1;       
end


PlantSet=xlsread(ManureSetXLSX);
ManureData=xlsread(ManureXLSX);
j=1;
manurePlace=1;    
manure=[];
oldYear=-1;
while j<=length(1,1)
    
   if(PlantSet(j,1)==Punktno)
       
       manure(manurePlace)=0;

       k=1;
       lengthCarbon=size(ManureData);
       ManureID1=PlantSet(j,4);
       ManureID2=PlantSet(j,6);
       year=PlantSet(j,2);
       while k<=lengthCarbon(1,1)

           if(ManureID1==ManureData(k,1)&&ManureID2==ManureData(k,3))
               if(oldYear~=year)
                    manure(manurePlace)=ManureData(k,5)*PlantSet(j,8);
               else
                    manure(manurePlace-1)=ManureData(k,5)*PlantSet(j,8)+ manure(manurePlace-1);
               end
               break;
           end
               k=k+1;
               
       end
       if(oldYear~=year)
            manurePlace=manurePlace+1;
       end
       oldYear=year;
   end
     j=j+1;       
end
outputData=xlsread(dataXLSX);
lengthCarbon=size(yieldAmount1);

if(lengthCarbon~=27)

    fprintf(TransportHum,'%s\r\n','Yield data might be missing');
    fclose(TransportHum);
   'Yield data might be missing'
end
warmUpdata=-1;
for i=1:lengthCarbon(1,2)

    if(yieldAmount1(i)~=0)
        warmUpdata=yieldAmount1(i);
        break;
    end
end
for j=1:100
outputData(j,7)= -1;
outputData(j,8)= -1;
end
while(lengthCarbon(1,2)<28)
    yieldAmount1(lengthCarbon(1,2)+1)=yieldAmount1(lengthCarbon(1,2));
    yieldAmount2(lengthCarbon(1,2)+1)=yieldAmount2(lengthCarbon(1,2));
    lengthCarbon=size(yieldAmount1);
end
for j=1:lengthCarbon(1,2)
outputData(j+100,7)= yieldAmount1(j);
outputData(j+100,8)=yieldAmount2(j);
end
outputData(102,9)=Data1986Upper;
outputData(125,9)=Data2009Upper;
outputData(102,24)=Data1986Lower;
outputData(125,24)=Data2009Lower;
outputData(125,25)=0;

if(isnan(Data1986Lower))
    TransportHum2 = fopen('errormessage2.txt','w');
 fprintf(TransportHum2,'%s\r\n','Data1986Lower is nan');
 'Data1986Lower is nan'
fclose(TransportHum2);
result = 'Data1986Lower is nan';
return;
end
if(isnan(Data2009Lower))
    TransportHum2 = fopen('errormessage2.txt','w');
 fprintf(TransportHum2,'%s\r\n','Data2009Lower is nan');
 'Data2009Lower is nan'
fclose(TransportHum2);
result = 'Data2009Lower is nan';
return;
end
lengthCarbonM=size(manure);
if(lengthCarbonM~=28)
    TransportHum2 = fopen('errormessage2.txt','w');
 fprintf(TransportHum2,'%s\r\n','Manure data might be missing');
 'Manure data might be missing'
fclose(TransportHum2);
end
if(lengthCarbonM(1,2)==0)
    manure(1)=0;
     lengthCarbonM=size(manure);
end
while(lengthCarbonM(1,2)<28)
    manure(lengthCarbonM(1,2)+1)=manure(lengthCarbonM(1,2));

    lengthCarbonM=size(manure);
end

for j=1:lengthCarbon(1,2)
outputData(1,10)=0;
outputData(2,10)=0;
end

for j=1:lengthCarbonM(1,2)
 outputData(j+100,10)=manure(j);
 
end
xlswrite(newdataXLSX,outputData);
       path=strcat(pwd ,'\');
       path=strcat(path ,runFileName);
result=calculateKandPviaLsqcurvefit(runFileName);
result
end

end
