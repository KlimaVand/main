function main(fileName)
dataFiles=runData(fileName);
countFinal=1;
DataWithoutNanFinal=zeros;

for i=(1):(size(dataFiles))
    data = xlsread(char(dataFiles(i))); 
    orgSize=size(data);
    DataWithoutNan=zeros;
    count=1;
    for j=1:orgSize(1,1)
        if(~isnan(data(j,1)))
            DataWithoutNan(count,1)=data(j,1);
            DataWithoutNan(count,2)=data(j,2);
            
            
            DataWithoutNanFinal(countFinal,1)=data(j,1);
            DataWithoutNanFinal(countFinal,2)=data(j,2);
            countFinal=countFinal+1;
           
            count=count+1;     
        end
    end
    txt =strcat('md for run nr ',num2str(i));
    txt=txt
    MD(DataWithoutNan)
    txt =strcat('RS for run nr ',num2str(i));
    txt=txt

    RS(DataWithoutNan)
    txt =strcat('EF for run nr ',num2str(i));
    txt=txt

    EF(DataWithoutNan)
    txt =strcat('R for run nr ',num2str(i));
    txt=txt
    R(DataWithoutNan)
    txt =strcat('RMSE for run nr ',num2str(i));
    txt=txt
    RMSE(DataWithoutNan)
    txt =strcat('RMSENULL for run nr ',num2str(i));
    txt=txt

    RMSENULL(DataWithoutNan)
    txt =strcat('t for run nr ',num2str(i));
    txt=txt
   
    t(DataWithoutNan)
    
    txt =strcat('MBE for run nr ',num2str(i));
    txt=txt
   
    MBE(DataWithoutNan)
end
'md total'
MD(DataWithoutNanFinal)
'RS total'
RS(DataWithoutNanFinal)
'EF total'
EF(DataWithoutNanFinal)
'R total'
R(DataWithoutNanFinal)
'RMSE total'
RMSE(DataWithoutNanFinal)
'RMSENULL total'
RMSENULL(DataWithoutNanFinal)
't total'
t(DataWithoutNanFinal)
'MBE total'
MBE(DataWithoutNanFinal)