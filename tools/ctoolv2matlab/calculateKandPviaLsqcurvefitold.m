function a = calculateKandPviaLsqcurvefit()
runNR=0;
runFile='run.txt';
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
    for j=(sizeOfInput-numberOfPossibleObserved+1):(sizeOfInput)
        if (data(j,23)==0)
            if(withScaling==true)
                observations(start,1)=data(j,18);
            else
                observations(start,1)=data(j,9);
            end
        else 
            if(withScaling==true)
                observations(start,1)=data(j,22);
            else
                observations(start,1)=data(j,13);
            end
        end
        start=start+1;
    end

  start=1;
      observationsWithoutNan=zeros;
    for j=(sizeOfInput-numberOfPossibleObserved+1):(sizeOfInput)
        
        if (data(j,23)==0)
            if(withScaling==true)
                if(~isnan(data(j,18)))
                    observationsWithoutNan(start,1)=data(j,18);
                    start=start+1;
                end
            else
                if(~isnan(data(j,9)))
                    observationsWithoutNan(start,1)=data(j,9);
                    start=start+1;
                end
            end
        else 
            if(withScaling==true)
                if(~isnan(data(j,22)))
                    observationsWithoutNan(start,1)=data(j,22);
                    start=start+1;
                end
            else
                if(~isnan(data(j,13)))
                    observationsWithoutNan(start,1)=data(j,13);
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
           tmpObservationsWithoutNan=observationsWithoutNan*1.12;
           tmpObservations=observations*1.12;
           observationsWithoutNan=cat(1,observationsWithoutNan,tmpObservationsWithoutNan);
           observations=cat(1,tmpObservations,observations);
           sizeOfInputwithoutNan=size(observationsWithoutNan);
           if(withScaling==true)
                foo = @(x,xdata)ctool3(-1,-1,x(1),x(2),x(3),x(4),sizeOfInputwithoutNan,0,runNR,observations,3,runFile,numberOfPossibleObserved,WithTransport);
           else
               foo = @(x,xdata)ctool2(-1,-1,x(1),x(2),x(3),x(4),sizeOfInputwithoutNan,0,runNR,observations,3,runFile,numberOfPossibleObserved,WithTransport);
           end
       else
           tmpObservationsWithoutNan=observationsWithoutNan*1.12;
           tmpObservations=observations*1.12;
           observationsWithoutNan=cat(1,observationsWithoutNan,tmpObservationsWithoutNan);
           observations=cat(1,tmpObservations,observations);
           sizeOfInputwithoutNan=size(observationsWithoutNan);
           if(withScaling==true)
                foo = @(x,xdata)ctool3(-1,-1,x(1),x(2),x(3),-1,sizeOfInputwithoutNan,0,runNR,observations,3,runFile,numberOfPossibleObserved,WithTransport);
           else
               foo = @(x,xdata)ctool2(-1,-1,x(1),x(2),x(3),-1,sizeOfInputwithoutNan,0,runNR,observations,3,runFile,numberOfPossibleObserved,WithTransport);
           end
       end
       optimset lsqcurvefit;
       options = optimset(ans,'TolFun',1.0000e-20);
       
lsqcurvefit(foo,[0.00155,0.8,0.9,158],observationsWithoutNan,observationsWithoutNan,[0.00155,0.1,0.1,120],[0.00156,0.9,0.9,240],options)


