function finalResult = EF(data)
sizeOfData=size(data);
meanOfObservations=0;
for j=1:sizeOfData(1,1)
    meanOfObservations=meanOfObservations+data(j,1);
end
meanOfObservations=meanOfObservations/sizeOfData(1,1);
firstSum=0;
secondSum=0;
for j=1:sizeOfData(1,1)
    firstSum=firstSum+(data(j,1)-meanOfObservations)^2;
    secondSum=secondSum+(data(j,2)-data(j,1))^2;
end
finalResult=(firstSum-secondSum)/firstSum;