function result = optimizing(runFile,runNR)


dataPlaces=runData(runFile);
dataPlaces{2+runNR*4}
    data = xlsread(dataPlaces{2+runNR*4}); 
     num = xlsread(dataPlaces{1+runNR*4});
       sizeOfInput=size(data);
       observationsWithoutNan=zeros;
         observations=zeros;
    sizeOfInput=sizeOfInput(1,1);
    numberOfPossibleObserved=data(sizeOfInput,1);
    start=1;
    start2=1;
    for j=(sizeOfInput-numberOfPossibleObserved+1):(sizeOfInput)
        if(~isnan(data(j,9)))
                    observationsWithoutNan(start,1)=data(j,9);
                    start=start+1;
        end
        observations(start2,1)=data(j,9);
        start2=start2+1;
        
        
    end
     sizeOfInputwithoutNan=size(observationsWithoutNan);
foo = @(x,xdata)ctool2(x(1),-1,sizeOfInputwithoutNan,0,runNR,observations,3,runFile,numberOfPossibleObserved,true);
        
       optimset lsqcurvefit;
       options = optimset(ans,'TolFun',1.0000e-20);

lsqcurvefit(foo,[2],observationsWithoutNan,observationsWithoutNan,[0.0],[15],options)

end


