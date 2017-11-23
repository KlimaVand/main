function result = decayFom(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,i,fomc,FOMdecompositionrate,tF,humification,humc,TransportFom,Co2Fom,AmountFom,outputInformation,WithTransport)
  fomcLeft=zeros(numberOfLayers,1);
  result=zeros(numberOfLayers,2);
  for j=1:numberOfLayers
      if(j==1)
         depthInLayer=25/2;
     else
         depthInLayer=75/2+25/2;
     end
      tempCofficent=temperatureCoefficent(Temperature(avgTemperature,i,depthInLayer,amplitude,offset));
      if(usingRK==1)
        FomAfterDecom=exp(-FOMdecompositionrate*t*tempCofficent)*fomc(j,1);
      else
          FomAfterDecom=rk4decay(i*t,fomc(j,1),t,@func,FOMdecompositionrate,tempCofficent);
      end
      fomcLeft(j,1)= fomcLeft(j,1)+FomAfterDecom;
      fomc(j,1)=fomc(j,1)-FomAfterDecom;

  % transport of fom to lower layer
if(WithTransport==true)
            toLowerLayer=fomc(j,1)*tF;
            inCorrentLayer=fomc(j,1)*(1-tF);
else
    toLowerLayer=0;
    inCorrentLayer=0;
end
  
             if(j~=numberOfLayers)
                fomcLeft(j+1,1)=fomcLeft(j+1,1)+toLowerLayer;
            else
                fomcLeft(j,1)=fomcLeft(j,1)+toLowerLayer;
            end
			fomc(j,1)=inCorrentLayer;
  
   
 %form CO2 and humification
      CO2=fomc(j,1)*(1-humification);
      humificationAmount=fomc(j,1)*humification;
	  fomc(j,1)=fomc(j,1)-CO2-humificationAmount;%C02
       result(j,2)=humc(j,1)+humificationAmount;  % humification
      if(outputInformation==1)
           fprintf(TransportFom,'%d\t',toLowerLayer);
           fprintf(Co2Fom,'%d\t',CO2);
       end
      
  end

   for j=1:numberOfLayers
       result(j,1)=fomcLeft(j,1);
       if(outputInformation==1)
            fprintf(AmountFom,'%d\t',result(j,1));
       end
   end
    if(outputInformation==1)
       fprintf(TransportFom,'\n');
       fprintf(Co2Fom,'\n');
       fprintf(AmountFom,'\n');
    end

