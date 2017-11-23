function amount = ctool2erosionSimple(startingAmount,sizeOfObserved, usingRK, runNR, observationWithnan,fullOutput, runName,numberOfPossibleObserved,WithTransport)

 dataPlaces=runData(runName);

 num = xlsread(dataPlaces{1+runNR*5});
data=xlsread(dataPlaces{2+runNR*5});
 tempvar=size(data);
                withC14=tempvar(2);
                C14=false;
                if(withC14>12)
                    C14=true;
                end
TemperatureData=xlsread(dataPlaces{3+runNR*5});
amount =Ctool2CoreerosionSimple(sizeOfObserved, usingRK, runNR,num,data,TemperatureData,dataPlaces,C14,startingAmount,observationWithnan,fullOutput,numberOfPossibleObserved,WithTransport);
