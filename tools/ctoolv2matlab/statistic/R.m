function finalResult = R(data)
sizeOfData=size(data);
meanOfObservations=0;
meanOfSimulated=0;
for j=1:sizeOfData(1,1)
    meanOfObservations=meanOfObservations+data(j,1);
    meanOfSimulated=meanOfSimulated+data(j,2);
end
meanOfObservations=meanOfObservations/sizeOfData(1,1);
meanOfSimulated=meanOfSimulated/sizeOfData(1,1);
SumObserved=0;
SumSimulated=0;
SumCombined=0;
for j=1:sizeOfData(1,1)
    SumObserved=SumObserved+(data(j,1)-meanOfObservations)^2;
    SumSimulated=SumSimulated+(data(j,2)-meanOfSimulated)^2;
    SumCombined=SumCombined+(data(j,1)-meanOfObservations)*(data(j,2)-meanOfSimulated);
end
finalResult=(SumCombined)/(SumObserved^0.5*SumSimulated^0.5);