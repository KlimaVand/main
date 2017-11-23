function result = avarage(fileName)

data=runData(fileName);
allData=zeros(128,30);
for i=1:size(data)

    num = dlmread(char(data(i)),'\t',1);
    allData=num+allData;
end
numberOfFiles=size(data);
numberOfFiles=numberOfFiles(1);
endResult=allData/numberOfFiles;
xlswrite('endResult',endResult);
end