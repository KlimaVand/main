function a = calculateKandPviaLsqcurvefit2()
runNR=11;
runFile='runGrids.txt';
dataPlaces=runData(runFile);
withCarbonInSoilAsObservations=true;
data = xlsread(dataPlaces{2+runNR*4}); 
     num = xlsread(dataPlaces{1+runNR*4});
    sizeOfInput=size(data);
    sizeOfInput=sizeOfInput(1,1);
    numberOfPossibleObserved=data(sizeOfInput,1);
   
      start=1;
      observations=zeros;
    for j=(sizeOfInput-numberOfPossibleObserved+1):(sizeOfInput)
        if(withCarbonInSoilAsObservations==true)
            observations(start,1)=data(j,9);
        else
            observations(start,1)=data(j,7)+data(j,8);
        end
       
        start=start+1;
    end

  start=1;
      observationsWithoutNan=zeros;
    for j=(sizeOfInput-numberOfPossibleObserved+1):(sizeOfInput)
        if(withCarbonInSoilAsObservations==true)
            if(~isnan(data(j,9)))
                        observationsWithoutNan(start,1)=data(j,9);
                        start=start+1;
            end
        else
            if(~isnan(data(j,7)))
                        observationsWithoutNan(start,1)=data(j,7)+data(j,8);
                        start=start+1;
            end
        end
        
           
    end
       sizeOfInputwithoutNan=size(observationsWithoutNan);
    
      
        foo = @(x,xdata)ctool2(-1,x(1),-1,-1,-1,-1,sizeOfInputwithoutNan,0,runNR,observations,0,runFile,numberOfPossibleObserved,true);
       optimset lsqcurvefit;
       options = optimset(ans,'TolFun',1.0000e-20);
    topsoil=  lsqcurvefit(foo,[1],observationsWithoutNan,observationsWithoutNan,[0],[2],options);
       
       
       observationsWithoutNan=observationsWithoutNan*1.12;
       observations=observations*1.12;
         foo = @(x,xdata)ctool2(x(1),-1,-1,-1,-1,-1,sizeOfInputwithoutNan,0,runNR,observations,1,runFile,numberOfPossibleObserved,true);
       optimset lsqcurvefit;
       options = optimset(ans,'TolFun',1.0000e-20);
       
  subsoil= lsqcurvefit(foo,[1],observationsWithoutNan,observationsWithoutNan,[0],[2],options);
  
    topsoil
      subsoil