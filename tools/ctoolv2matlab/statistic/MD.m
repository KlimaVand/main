function finalResult = MD(data)
sizeOfData=size(data);
result=0;
for j=1:sizeOfData(1,1)
    result=result+data(j,1)-data(j,2);
end
if(result<0)
    result=result*-1;
end
finalResult=result/sizeOfData(1,1);