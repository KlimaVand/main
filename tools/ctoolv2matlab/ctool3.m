function amount = ctool3(anualAmountPlantSubsoil,anualAmountPlantTopsoil,HUMdecompositionrate, P1,P2,startingAmount,sizeOfObserved, usingRK, runNR, observationWithnan,fullOutput, runName,numberOfPossibleObserved,WithTransport)

 dataPlaces=runData(runName);

 num = xlsread(dataPlaces{1+runNR*4});
data=xlsread(dataPlaces{2+runNR*4});
 tempvar=size(data);
                withC14=tempvar(2);
                C14=false;
                if(withC14>20)
                    C14=true;
                end
TemperatureData=xlsread(dataPlaces{3+runNR*4});
amount =Ctool2Core(anualAmountPlantSubsoil,anualAmountPlantTopsoil,HUMdecompositionrate, P1,P2,sizeOfObserved, usingRK, runNR,num,data,TemperatureData,dataPlaces,C14,startingAmount,observationWithnan,fullOutput,numberOfPossibleObserved,WithTransport);
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
