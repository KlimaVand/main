function amount = ctool2(anualAmountPlant,startingAmount,sizeOfObserved, usingRK, runNR, observationWithnan,fullOutput, runName,numberOfPossibleObserved,WithTransport)

 dataPlaces=runData(runName);

 num = xlsread(dataPlaces{1+runNR*4});
data=xlsread(dataPlaces{2+runNR*4});
 tempvar=size(data);
                withC14=tempvar(2);
                C14=false;
                if(withC14>12)
                    C14=true;
                end
TemperatureData=xlsread(dataPlaces{3+runNR*4});
    sizeOfInput=size(data);
    sizeOfInput=sizeOfInput(1,1);
    numberOfYears=data(sizeOfInput,1);
amount =Ctool2Core(anualAmountPlant,sizeOfObserved, usingRK, runNR,num,data,TemperatureData,dataPlaces,C14,startingAmount,observationWithnan,fullOutput,numberOfPossibleObserved,WithTransport,numberOfYears);
