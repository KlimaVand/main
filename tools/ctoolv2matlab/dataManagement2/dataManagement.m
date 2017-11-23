function amount = dataManagement(Punktno,ManureSetXLSX,ManureXLSX,CarbonXLSX,PlantSetXLSX,dataSetXLSX,inputXLSX, dataXLSX,kvadratnetsdataXLSX)
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

defaultUpper=-1;
defaultLower=-1;
length=size(dataSet);
TransportHum = fopen('errormessage.txt','w');
j=1;
soilNr=-1;
while j<=length(1,1)
   if(dataSet(j,1)==Punktno);
       soilNr=dataSet(j,6);
       dataSet(j,8)
       dataSet(j,8)/(47/100)
       input(6)=dataSet(j,8)/(47/100);
       input(7)=(dataSet(j,3)/dataSet(j,4)+dataSet(j+2,3)/dataSet(j+2,4))/2;
       %input(1)=min(56.2*input(7)^-1.69,1);
       %input(4)=min(56.2*input(7)^-1.69,1);
       dirname=strcat('test',num2str(Punktno));
       mkdir(dirname);
       fileName=strcat(dirname,'/');
       fileName=strcat(fileName,dirname);
       fileName=strcat(fileName,'.xls');
       newdataXLSX=strcat(dirname,'/');
       newdataXLSX=strcat(newdataXLSX,dataXLSX);

       xlswrite(fileName,input);
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
                        yieldAmount2(plantPlace) = Carbon(k,6);
                    case 6
                        yieldAmount1(plantPlace) = Carbon(k,3);
                        yieldAmount2(plantPlace) = Carbon(k,6);
                        break;
                    case 7
                        yieldAmount1(plantPlace) = Carbon(k,4);
                        yieldAmount2(plantPlace) = Carbon(k,6);
                        break;
                    case 8
                        yieldAmount1(plantPlace) = Carbon(k,4);
                        yieldAmount2(plantPlace) = Carbon(k,6);
                    case 9
                        yieldAmount1(plantPlace) = Carbon(k,4);
                        yieldAmount2(plantPlace) = Carbon(k,6);
                        break;
                   case 10
                       yieldAmount1(plantPlace) = Carbon(k,5);
                       yieldAmount2(plantPlace) = Carbon(k,7);
                       break;
                   case 11
                       yieldAmount1(plantPlace) = Carbon(k,5);
                       yieldAmount2(plantPlace) = Carbon(k,7);
                       break;
                   case 12
                       yieldAmount1(plantPlace) = Carbon(k,5);
                       yieldAmount2(plantPlace) = Carbon(k,7);
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
outputData(1,7)= yieldAmount1(1);
outputData(1,8)=yieldAmount2(1);
outputData(2,7)= yieldAmount1(1);
outputData(2,8)=yieldAmount2(1);

while(lengthCarbon(1,2)<28)
    yieldAmount1(lengthCarbon(1,2)+1)=yieldAmount1(lengthCarbon(1,2));
    yieldAmount2(lengthCarbon(1,2)+1)=yieldAmount2(lengthCarbon(1,2));
    lengthCarbon=size(yieldAmount1);
end
for j=1:lengthCarbon(1,2)
outputData(j+2,7)= yieldAmount1(j);
outputData(j+2,8)=yieldAmount2(j);
 
 
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


outputData(1,10)=0;
outputData(2,10)=0;
for j=1:lengthCarbonM(1,2)
 outputData(j+2,10)=manure(j);
 
end
xlswrite(newdataXLSX,outputData);
       path=strcat(pwd ,'\');
       path=strcat(path ,runFileName);
ctool2(-1,-1,-1,-1,-1,-1,lengthCarbon(1,2)+2,0,0,1,2,path,lengthCarbon(1,2)+2,true)
end

end
