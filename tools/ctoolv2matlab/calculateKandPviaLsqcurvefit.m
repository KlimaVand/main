function result = calculateKandPviaLsqcurvefit(runFile)
runNR=0;

withScaling=false;
optimizeAmount=true;
pFraction=1;
WithTransport=true;
dataPlaces=runData(runFile);
    data = xlsread(dataPlaces{2+runNR*4}); 
     num = xlsread(dataPlaces{1+runNR*4});
    sizeOfInput=size(data);
    sizeOfInput=sizeOfInput(1,1);
    numberOfPossibleObserved=data(sizeOfInput,1);
   
      start=1;
      observations=zeros;
       tmpObservations=zeros;
    for j=(sizeOfInput-numberOfPossibleObserved+1):(sizeOfInput)
        if (data(j,23)==0)
            if(withScaling==true)
                observations(start,1)=data(j,18);
                tmpObservations(start,1)=data(j,25);
            else
                observations(start,1)=data(j,9);
                tmpObservations(start,1)=data(j,24);
            end
        else 
            if(withScaling==true)
                observations(start,1)=data(j,22);
                tmpObservations(start,1)=data(j,26);
            else
                observations(start,1)=data(j,13);
                tmpObservations(start,1)=data(j,27);
            end
        end
        start=start+1;
    end

  start=1;
      observationsWithoutNan=zeros;
     tmpObservationsWithoutNan=zeros;
    for j=(sizeOfInput-numberOfPossibleObserved+1):(sizeOfInput)
        
        if (data(j,23)==0)
            if(withScaling==true)
                if(~isnan(data(j,18)))
                    observationsWithoutNan(start,1)=data(j,18);
                     tmpObservationsWithoutNan(start,1)=data(j,25);
                    start=start+1;
                end
            else
                if(~isnan(data(j,9)))
                    observationsWithoutNan(start,1)=data(j,9);
                    tmpObservationsWithoutNan(start,1)=data(j,24);
                    start=start+1;
                end
            end
        else 
            if(withScaling==true)
                if(~isnan(data(j,22)))
                    observationsWithoutNan(start,1)=data(j,22);
                     tmpObservationsWithoutNan(start,1)=data(j,26);
                    start=start+1;
                end
            else
                if(~isnan(data(j,13)))
                    observationsWithoutNan(start,1)=data(j,13);
                     tmpObservationsWithoutNan(start,1)=data(j,27);
                    start=start+1;
                end
            end
        end
    end
       
       if(pFraction==-1)
           sizeOfInputwithoutNan=size(observationsWithoutNan);
           if(withScaling==true)
                foo = @(x,xdata)ctool3(-1,-1,x(1),pFraction,pFraction,-1,sizeOfInputwithoutNan,0,runNR,observations,0,runFile,numberOfPossibleObserved,WithTransport);
           else
               foo = @(x,xdata)ctool2(-1,-1,x(1),pFraction,pFraction,-1,sizeOfInputwithoutNan,0,runNR,observations,0,runFile,numberOfPossibleObserved,WithTransport);
           end
       elseif(optimizeAmount==true)

           observationsWithoutNan=cat(1,observationsWithoutNan,tmpObservationsWithoutNan);
           observations=cat(1,tmpObservations,observations);
           sizeOfInputwithoutNan=size(observationsWithoutNan);
           if(withScaling==true)
                foo = @(x,xdata)ctool3(x(1),-1,sizeOfInputwithoutNan,0,runNR,observations,3,runFile,numberOfPossibleObserved,WithTransport);
           else
               foo = @(x,xdata)ctool2(x(1),-1,sizeOfInputwithoutNan,0,runNR,observations,3,runFile,numberOfPossibleObserved,WithTransport);
           end
       else
 
           observationsWithoutNan=cat(1,observationsWithoutNan,tmpObservationsWithoutNan);
           observations=cat(1,tmpObservations,observations);
           sizeOfInputwithoutNan=size(observationsWithoutNan);
           if(withScaling==true)
                foo = @(x,xdata)ctool3(x(1),-1,sizeOfInputwithoutNan,0,runNR,observations,3,runFile,numberOfPossibleObserved,WithTransport);
           else
               foo = @(x,xdata)ctool2(x(1),-1,sizeOfInputwithoutNan,0,runNR,observations,3,runFile,numberOfPossibleObserved,WithTransport);
           end
       end
       optimset lsqcurvefit;
       options = optimset(ans,'TolFun',1.0000e-20);
if(observationsWithoutNan(1,1)~=0)       
result=lsqcurvefit(foo,[2],observationsWithoutNan,observationsWithoutNan,[0.0],[15],options)
else
    
result='noObservation';
end


