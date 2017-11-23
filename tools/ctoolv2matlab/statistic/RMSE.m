function finalResult = RMSE(data)
sizeOfData=size(data);
result=0;
mean=0;
for j=1:sizeOfData(1,1)
    result=result+(data(j,2)-data(j,1))^2/sizeOfData(1,1);
    mean=mean+data(j,1);
end
mean=mean/sizeOfData(1,1);


finalResult=100/mean*sqrt(result);