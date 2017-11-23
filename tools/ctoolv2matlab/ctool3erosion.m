function amount = ctool3erosion(startingAmount,sizeOfObserved, usingRK, runNR, observationWithnan,fullOutput, runName,numberOfPossibleObserved,WithTransport)

 dataPlaces=runData(runName);

 num = xlsread(dataPlaces{1+runNR*5});
data=xlsread(dataPlaces{2+runNR*5});
 tempvar=size(data);
                withC14=tempvar(2);
                C14=false;
                if(withC14>20)
                    C14=true;
                end
TemperatureData=xlsread(dataPlaces{3+runNR*5});
amount =Ctool2Coreerosion(sizeOfObserved, usingRK, runNR,num,data,TemperatureData,dataPlaces,C14,startingAmount,observationWithnan,fullOutput,numberOfPossibleObserved,WithTransport);
index=1;
for j=1:(tempvar(1))
    if(data(j,23)==0)
        if(~isnan(data(j,17)))
            amount(index)=amount(index)/data(j,17);
            index=index+1;
        end
    else
        if(~isnan(data(j,21)))
            amount(index)=amount(index)/data(j,21);
            index=index+1;
        end 
    end
    
end
