function a = calculateKandP()
%fid = fopen('output.txt','w')
%for j=1:(3)
%      for i=1:(3)
%          amound=ctool2(j*0.1,i*0.1);
%          fprintf(fid,'%d\t%d\n',j,i);
%      end
%end
% fclose(fid);

 dataPlaces=runData('Run-1988Askov.txt');
 numberOfExperiments=size(dataPlaces);
 numberOfExperiments=numberOfExperiments(1,1)/4;
avgObservations=zeros(numberOfExperiments);
avgObservationsC14=zeros(numberOfExperiments);
dataForSumOfSqrt=zeros;
dataForSumOfSqrtC14=zeros;
numberOfPossibleObserved=zeros(numberOfExperiments,1);
numberOfObservationsInTotal=zeros(numberOfExperiments);
numberOfObservationsInTotalC14=zeros(numberOfExperiments);
allData=cell(numberOfExperiments,1);
allInput=cell(numberOfExperiments,1);
     
allTemperatureData=cell(numberOfExperiments,1);
 for i=1:numberOfExperiments
    runNR=i-1;
    data = xlsread(dataPlaces{2+runNR*4});
 	Input = xlsread(dataPlaces{1+runNR*4});
	TemperatureData=xlsread(dataPlaces{3+runNR*4});   
    allData{i}=data;
    allInput{i}=Input;
    allTemperatureData{i}=TemperatureData;
    numberOfPossibleObservedTmp=size(data(:,1));
    numberOfPossibleObservedTmp=numberOfPossibleObservedTmp(1,1);
    numberOfItems=numberOfPossibleObservedTmp(1,1);
    numberOfPossibleObserved(i)=data(numberOfPossibleObservedTmp,1);
    ListOfObserved=zeros;
    ListOfObservedC14=zeros;

tempvar=size(data);
withC14=tempvar(2);
    start=1;
    for j=(numberOfItems-numberOfPossibleObserved(i)+1):(numberOfItems)
        dataForSumOfSqrt(start,i)=data(j,9);
        if(withC14>12&&~isnan(data(j,12)))
            dataForSumOfSqrtC14(start,i)=data(j,13);
        end
        start=start+1;
    end
    for j=1:(size(data(:,1)))
        if(~isnan(data(j,9)))
            numberOfObservationsInTotal(i)=numberOfObservationsInTotal(i)+1;
            ListOfObserved(numberOfObservationsInTotal(i),1)=data(j,9);
        end
        if(withC14>12)
            if(~isnan(data(j,13)))
                numberOfObservationsInTotalC14(i)=numberOfObservationsInTotalC14(i)+1;
                ListOfObservedC14(numberOfObservationsInTotalC14(i),1)=data(j,13);
            end
        end
    end
    for j=1:numberOfObservationsInTotal(i)
        avgObservations(i)=avgObservations(i)+ListOfObserved(j);
    end
    avgObservations(i)=avgObservations(i)/numberOfObservationsInTotal(i);
    for j=1:numberOfObservationsInTotalC14(i)
        avgObservationsC14(i)=avgObservationsC14(i)+ListOfObservedC14(j);
    end
    avgObservationsC14(i)=avgObservationsC14(i)/numberOfObservationsInTotalC14(i);

 end
 
 
 minimum=9e999;
 minfraction=0;
 minP=0;
 minstartingamount=0;
 for j=1:(100)
     for k=1:(200)
        for s=1:(25)
        
             P=0.01*j;
             fraction=0.00001*k;
             startingAmount=s*10+50;
             for expNr=1:numberOfExperiments
                 runNR=expNr-1;
                 data = allData{expNr,1};
                 input =allInput{expNr,1};
     
                 TemperatureData=allTemperatureData{expNr,1};
                 tempvar=size(data);
                withC14=tempvar(2);
                C14=false;
                if(withC14>12)
                    C14=true;
                end
                 result=Ctool2Core(fraction,P,numberOfPossibleObserved(expNr),0,runNR,input,data,TemperatureData,dataPlaces,C14,startingAmount,-1,true,numberOfPossibleObserved(expNr);
                 sum=0;

                 for i=1:numberOfPossibleObserved(expNr)
                     if(~isnan(dataForSumOfSqrt(i,expNr)))
                         sum=sum+((dataForSumOfSqrt(i,expNr)-result(i,3))^2)/(avgObservations(expNr,1)*numberOfObservationsInTotal(expNr,1))^2;
                     end
                 end
                 if(withC14>12)
                     for i=1:numberOfPossibleObserved(expNr)
                         if(~isnan(dataForSumOfSqrtC14(i,expNr)))
                             sum=sum+((dataForSumOfSqrtC14(i,expNr)-result(i,7))^2)/(avgObservationsC14(expNr,1)*numberOfObservationsInTotalC14(expNr,1))^2;
                         end
                     end
                 end
                j=j
                k=k
                expNr=expNr
                s=s
             end
             sum=sqrt(sum/numberOfExperiments);
             if(sum<minimum)
                 minimum=sum;
                 minfraction=fraction;
                 minP=P;
                 minstartingamount=startingAmount;
             end
        end
     end
 end
minfraction=minfraction
minP=minP
minstartingamount=minstartingamount
