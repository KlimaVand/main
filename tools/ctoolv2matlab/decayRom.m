function romc = decayRom(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,i,TransportRom,Co2Rom,AmountRom,romc,ROMdecompositionrate,outputInformation,WithTransport)
 romcLeft=zeros(numberOfLayers,1);
 for j=1:(numberOfLayers)
     if(j==1)
         depthInLayer=25/2;
     else
         depthInLayer=75/2+25/2;
     end
     tempCofficent=temperatureCoefficent(Temperature(avgTemperature,i,depthInLayer,amplitude,offset));

     if(usingRK==1)
        romAfterDecom=exp(-ROMdecompositionrate*t*tempCofficent)*romc(j,1);
    else
          romAfterDecom=rk4decay(i*t,romc(j,1),t,@func,ROMdecompositionrate,tempCofficent);
    end
     romcLeft(j,1)= romcLeft(j,1)+romAfterDecom;
     romcLeftForInert=romc(j,1)-romAfterDecom;
    
     romc(j,1)=romc(j,1)-romAfterDecom;
 %CO2 ROM and transport

    CO2=romc(j,1)*0.628;
    if(WithTransport==true)
    transport=romc(j,1)*(1-0.628);
    else
        transport=0;
    end
	romc(j,1)=romc(j,1)-CO2-transport;

    if(j~=numberOfLayers)
        romcLeft(j+1,1)=romcLeft(j+1,1)+transport;
    else
        romcLeft(j,1)=romcLeft(j,1)+transport;
    end
    if(outputInformation==1)
       fprintf(TransportRom,'%d\t',transport);
       fprintf(Co2Rom,'%d\t',CO2);
    end
     

 end
    for j=1:numberOfLayers
       romc(j,1)=romcLeft(j,1);
       if(outputInformation==1)
         fprintf(AmountRom,'%d\t',romc(j,1));
       end
    end
    if(outputInformation==1)
       fprintf(TransportRom,'\n');
       fprintf(Co2Rom,'\n');
       fprintf(AmountRom,'\n');
    end