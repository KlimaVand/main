function finalResult = t(data)
sizeOfData=size(data);

M=MD(data);
result=0;

for j=1:sizeOfData(1,1)
    result=result+(data(j,1)-data(j,2)-M)^2;
end
result=result/(sizeOfData(1,1)-1);
finalResult=M/(sqrt(result)/sqrt(sizeOfData(1,1)));
