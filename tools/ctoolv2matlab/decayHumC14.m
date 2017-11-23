function result = decayHumC14(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,i,HUMdecompositionrate,humc,TransportHum,Co2Hum,AmountHum,ROMfraction,romc,outputInformation,DecayRate,WithTransport)
   humcLeft=zeros(numberOfLayers,1);
   result=zeros(numberOfLayers,2);
 for j=1:(numberOfLayers)

	if(j==1)
         depthInLayer=25/2;
     else
         depthInLayer=75/2+25/2;
     end
	tempCofficent=temperatureCoefficent(Temperature(avgTemperature,i,depthInLayer,amplitude,offset));

    if(usingRK==1)
        humAfterDecom=exp(-HUMdecompositionrate*t*tempCofficent)*humc(j,1)-DecayRate*humc(j,1);
    else
          humAfterDecom=rk4decay(i*t,humc(j,1),t,@func,HUMdecompositionrate,tempCofficent)-DecayRate*humc(j,1);
    end
    humcLeft(j,1)=humcLeft(j,1)+humAfterDecom;
	humc(j,1)=humc(j,1)-humAfterDecom; 
 
 % CO2, romification and transport

     CO2=humc(j,1)*0.628;
   if(WithTransport==true)
     transport=humc(j,1)*(1-0.628-ROMfraction);
   else
       transport=0;
   end
   
     romification=humc(j,1)*ROMfraction;
	 humc(j,1)=humc(j,1)-CO2-transport-romification;
     if(j~=numberOfLayers)
        humcLeft(j+1,1)=humcLeft(j+1,1)+transport;
     else
         humcLeft(j,1)=humcLeft(j,1)+transport;
     end
    result(j,2)=romc(j,1)+romification;
     if(outputInformation==1)
       fprintf(TransportHum,'%d\t',transport);
       fprintf(Co2Hum,'%d\t',CO2);
     end


 end

   for j=1:numberOfLayers
       result(j,1)=humcLeft(j,1);
       if(outputInformation==1)
            fprintf(AmountHum,'%d\t',result(j,1));
       end
   end
    if(outputInformation==1)
        fprintf(TransportHum,'\n');
        fprintf(Co2Hum,'\n');
        fprintf(AmountHum,'\n');
    end