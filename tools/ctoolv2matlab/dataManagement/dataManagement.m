function amount = dataManagement(amtsNr,namesXLSX,dataSetXLSX,inputXLSX, dataXLSX,TemperatureXLSX)
addpath('..')
names=xlsread(namesXLSX);
dataSet=xlsread(dataSetXLSX);
input=xlsread(inputXLSX);
length=size(names);
Punktno=zeros;
lenghOfPunktno=1;
for j=1:length(1,1)
    if(names(j,4)==amtsNr)
        
        Punktno(lenghOfPunktno)=names(j,1);
        lenghOfPunktno=lenghOfPunktno+1;
    end
end
length=size(dataSet);
placeInList=1;
j=1;
while j<=length(1,1)
   if(dataSet(j,1)==Punktno(placeInList))
       input(6)=dataSet(j,8)/(41/100);
       input(7)=(dataSet(j,3)/dataSet(j,4)+dataSet(j,3)/dataSet(j,4))/2;
     %  input(1)=0.5/min(56.2*input(7)^-1.69,1);
     %  input(4)=0.5/min(56.2*input(7)^-1.69,1);
       dirname=strcat('test',num2str(Punktno(placeInList)));
       dirname=strcat(dirname,'_');
       dirname=strcat(dirname,num2str(dataSet(j,5)));
       mkdir(dirname);
       fileName=strcat(dirname,'/');
       fileName=strcat(fileName,dirname);
       fileName=strcat(fileName,'.xls');
       xlswrite(fileName,input);
       runFileName =strcat(dirname,'\run.txt');
       runFile = fopen(runFileName,'w');
       fprintf(runFile,'%s\t%s\r\n','input',fileName);
       fprintf(runFile,'%s\t%s\r\n','data',dataXLSX);
       fprintf(runFile,'%s\t%s\r\n','TemperatureData',TemperatureXLSX);
       fprintf(runFile,'%s\t%s\r\n','outputDir',dirname);
       fclose(runFile);
       path=strcat(pwd ,'\');
       path=strcat(path ,runFileName);
       ctool2(-1,-1,-1,-1,-1,-1,250,0,0,1,2,path,250,true)
       j=j+6;
   elseif (dataSet(j,1)>Punktno(placeInList))
       placeInList=placeInList+1;
       erro=size(Punktno);
       if(erro(2)+1==(placeInList))
           break
       end
   else
       j=j+1;
   end
   
    
end