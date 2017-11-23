
function amount = Ctool2Core(anualAmountPlant,sizeOfObserved, usingRK, runNR,num,data,TemperatureData,dataPlaces, C14, startingAmountFromInput, ObservationWithNan, fullOutput, numberOfPossibleObservation, WithTransport,numberOfYears )
withC14Manure =false;
EpeselonBjorn=0.1;
EpeselonHum=0.1;
EpeselonRom=0.1;
RomcPlantLastYear=0;
HumcPlantLastYear=0;
i = 0;
depth=100;
 placeInOutput=1;
 placeInObservationWithNan=1;
 outputInformation=false;

    PLoweLayer=num(1);

offset=num(2);

    HUMdecompositionrate=num(3);
    PupperLayer=num(4);


C14percent=num(5);
sizeOfTemp=size(TemperatureData);
min=999999;
max=0;
for j=1:sizeOfTemp(1,1)
    if(TemperatureData(j)<min)
        min=TemperatureData(j);
    end
    if(TemperatureData(j)>max)
        max=TemperatureData(j);
    end
end
amplitude=(max-min)/2;
startingAmountFromFile=num(6);
CNratio=num(7);
extraCarbon=num(8);
FractionLayer=num(9);
extraCarbon=extraCarbon/12;

if(startingAmountFromInput==-1)
startAmountOfCarbon=startingAmountFromFile;
else
    startAmountOfCarbon=startingAmountFromInput;
end
startAmountLayer1=startAmountOfCarbon*FractionLayer;
startAmountLayer2=startAmountOfCarbon*(1-FractionLayer);

numberOfLayers=2;
%plant
FOMfractionPlantTopLayer=num(12);
FOMfractionPlantLowerLayer=num(13);
FOMdecompositionratePlant=num(14);
ClayfractionPlant=num(15);
tFPlant=num(16);
ROMfractionPlant=num(18);
ROMdecompositionratePlant =num(19);
%manure
HUMfractionManure=num(22);
FOMdecompositionrateManure=num(24);
ClayfractionManure=num(25);
tFManure=num(26);
ROMfractionManure=num(28);
ROMdecompositionrateManure =num(29);

%plant C14
FOMfractionPlantTopLayerC14=num(32);
FOMfractionPlantLowerLayerC14=num(33);
FOMdecompositionratePlantC14=num(34);
ClayfractionPlantC14=num(35);
tFPlantC14=num(36);
ROMfractionPlantC14=num(38);
ROMdecompositionratePlantC14 =num(39);
DecayRateC14Plant =num(40);
%manure C14
HUMfractionManureC14=num(43);

FOMdecompositionrateManureC14=num(45);
ClayfractionManureC14=num(46);
tFManureC14=num(47);
ROMfractionManureC14=num(49);

ROMdecompositionrateManureC14 =num(50);
DecayRateC14Manuer =num(51);

amount=zeros(sizeOfObserved);


TransportHum = 0;
Co2Hum = 0;
AmountHum = 0;
TransportFom = 0;
Co2Fom = 0;
AmountFom = 0;
TransportRom = 0;
Co2Rom = 0;
AmountRom = 0;
TransportHumManure = 0;
Co2HumManure = 0;
AmountHumManure = 0;
TransportFomManure = 0;
Co2FomManure = 0;
AmountFomManure = 0;
TransportRomManure = 0;
Co2RomManure = 0;
AmountRomManure = 0;
TransportHumC14 = 0;
Co2HumC14 = 0;
AmountHumC14 = 0;
TransportFomC14 = 0;
Co2FomC14 = 0;
AmountFomC14 = 0;
TransportRomC14 = 0;
Co2RomC14 = 0;
AmountRomC14 = 0;
TransportHumManureC14 = 0;
Co2HumManureC14 = 0;
AmountHumManureC14 = 0;
TransportFomManureC14 = 0;
Co2FomManureC14 = 0;
AmountFomManureC14 = 0;
TransportRomManureC14 = 0;
Co2RomManureC14 = 0;
AmountRomManureC14 = 0;
totalAmount=0;
if(outputInformation==1)
%plant output
    output =strcat(dataPlaces{4+runNR*4},'\TransportHum.xls');
    TransportHum = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\Co2Hum.xls');
    Co2Hum = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\AmountHum.xls');
    AmountHum = fopen(output,'w');

    output =strcat(dataPlaces{4+runNR*4},'\TransportFom.xls');
    TransportFom = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\Co2Fom.xls');
    Co2Fom = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\AmountFom.xls');
    AmountFom = fopen(output,'w');

    output =strcat(dataPlaces{4+runNR*4},'\TransportRom.xls');
    TransportRom = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\Co2Rom.xls');
    Co2Rom = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\AmountRom.xls');
    AmountRom = fopen(output,'w');

    %manure output
    output =strcat(dataPlaces{4+runNR*4},'\TransportHumManure.xls');
    TransportHumManure = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\Co2HumManure.xls');
    Co2HumManure = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\AmountHumManure.xls');
    AmountHumManure = fopen(output,'w');

    output =strcat(dataPlaces{4+runNR*4},'\TransportFomManure.xls');
    TransportFomManure = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\Co2FomManure.xls');
    Co2FomManure = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\AmountFomManure.xls');
    AmountFomManure = fopen(output,'w');

    output =strcat(dataPlaces{4+runNR*4},'\TransportRomManure.xls');
    TransportRomManure = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\Co2RomManure.xls');
    Co2RomManure = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\AmountRomManure.xls');
    AmountRomManure = fopen(output,'w');

    %plant output C14
    output =strcat(dataPlaces{4+runNR*4},'\TransportHumC14.xls');
    TransportHumC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\Co2HumC14.xls');
    Co2HumC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\AmountHumC14.xls');
    AmountHumC14 = fopen(output,'w');

    output =strcat(dataPlaces{4+runNR*4},'\TransportFomC14.xls');
    TransportFomC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\Co2FomC14.xls');
    Co2FomC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\AmountFomC14.xls');
    AmountFomC14 = fopen(output,'w');

    output =strcat(dataPlaces{4+runNR*4},'\TransportRomC14.xls');
    TransportRomC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\Co2RomC14.xls');
    Co2RomC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\AmountRomC14.xls');
    AmountRomC14 = fopen(output,'w');

    %manure output
    output =strcat(dataPlaces{4+runNR*4},'\TransportHumManureC14.xls');
    TransportHumManureC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\Co2HumManureC14.xls');
    Co2HumManureC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\AmountHumManureC14.xls');
    AmountHumManureC14 = fopen(output,'w');

    output =strcat(dataPlaces{4+runNR*4},'\TransportFomManureC14.xls');
    TransportFomManureC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\Co2FomManureC14.xls');
    Co2FomManureC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\AmountFomManureC14.xls');
    AmountFomManureC14 = fopen(output,'w');

    output =strcat(dataPlaces{4+runNR*4},'\TransportRomManureC14.xls');
    TransportRomManureC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\Co2RomManureC14.xls');
    Co2RomManureC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*4},'\AmountRomManureC14.xls');
    AmountRomManureC14 = fopen(output,'w');
    
    

end
output =strcat(dataPlaces{4+runNR*4},'\totalAmount.xls');
totalAmount = fopen(output,'w');
fprintf(totalAmount,'%s\t','fomcPlant(1,1)');
            fprintf(totalAmount,'%s\t','humcPlant(1,1)');
            fprintf(totalAmount,'%s\t','romcPlant(1,1)');

            fprintf(totalAmount,'%s\t','fomcManure(1,1)');
            fprintf(totalAmount,'%s\t','humcManure(1,1)');
            fprintf(totalAmount,'%s\t','romcManure(1,1)');

            fprintf(totalAmount,'%s\t','fomcPlantC14(1,1)');
            fprintf(totalAmount,'%s\t','humcPlantC14(1,1)');
            fprintf(totalAmount,'%s\t','romcPlantC14(1,1)');
            
            fprintf(totalAmount,'%s\t','fomcManureC14(1,1)');
            fprintf(totalAmount,'%s\t','humcManureC14(1,1)');
            fprintf(totalAmount,'%s\t','romcManureC14(1,1)');
            
            fprintf(totalAmount,'%s\t','total(1,1)');
            
            fprintf(totalAmount,'%s\t','fomcPlant(2,1)');
            fprintf(totalAmount,'%s\t','humcPlant(2,1)');
            fprintf(totalAmount,'%s\t','romcPlant(2,1)');

            fprintf(totalAmount,'%s\t','fomcManure(2,1)');
            fprintf(totalAmount,'%s\t','humcManure(2,1)');
            fprintf(totalAmount,'%s\t','romcManure(2,1)');

            fprintf(totalAmount,'%s\t','fomcPlantC14(2,1)');
            fprintf(totalAmount,'%s\t','humcPlantC14(2,1)');
            fprintf(totalAmount,'%s\t','romcPlantC14(2,1)');

            fprintf(totalAmount,'%s\t','fomcManureC14(2,1)');
            fprintf(totalAmount,'%s\t','humcManureC14(2,1)');
            fprintf(totalAmount,'%s\t','romcManureC14(2,1)');
            
            fprintf(totalAmount,'%s\t','total(2,1)');
            
            fprintf(totalAmount,'%s\t','year)');
            fprintf(totalAmount,'%s\t','month');
            
            fprintf(totalAmount,'%s\t','p layer 1');
            fprintf(totalAmount,'%s\r\n','p layer 2');
            
fomcPlant=zeros(numberOfLayers,1);
humcPlant=zeros(numberOfLayers,1);
romcPlant=zeros(numberOfLayers,1);

fomcManure=zeros(numberOfLayers,1);
humcManure=zeros(numberOfLayers,1);
romcManure=zeros(numberOfLayers,1);

fomcPlantC14=zeros(numberOfLayers,1);
humcPlantC14=zeros(numberOfLayers,1);
romcPlantC14=zeros(numberOfLayers,1);

fomcManureC14=zeros(numberOfLayers,1);
humcManureC14=zeros(numberOfLayers,1);
romcManureC14=zeros(numberOfLayers,1);

sizeofDataset=size(data);
iterations=sizeofDataset(1,1)*12-1;
CNfraction=CN(CNratio);
fomcPlant(1,1)=startAmountLayer1*FOMfractionPlantTopLayer;
humcPlant(1,1)=startAmountLayer1*PupperLayer*CNfraction;
romcPlant(1,1)=startAmountLayer1-humcPlant(1,1)-fomcPlant(1,1);
fomcPlant(2,1)=startAmountLayer2*FOMfractionPlantLowerLayer;
humcPlant(2,1)=startAmountLayer2*PLoweLayer*CNfraction;
romcPlant(2,1)=startAmountLayer2-humcPlant(2,1)-fomcPlant(2,1);
humcPlant(1,1)+romcPlant(1,1)+fomcPlant(1,1)+humcPlant(2,1)+romcPlant(2,1)+fomcPlant(2,1)
HumcPlantLastYear=humcPlant(1,1);
RomcPlantLastYear=romcPlant(1,1);
if(C14==true)
    fomcPlantC14(1,1)=startAmountLayer1*(C14percent/100)*FOMfractionPlantTopLayerC14;
    humcPlantC14(1,1)=(startAmountLayer1)*PupperLayer*(C14percent/100)*CNfraction;
    romcPlantC14(1,1)=(startAmountLayer1)*(C14percent/100)-humcPlantC14(1,1)-fomcPlantC14(1,1);
    
    fomcPlantC14(2,1)=startAmountLayer2*(C14percent/100)*FOMfractionPlantLowerLayerC14;
    humcPlantC14(2,1)=(startAmountLayer2)*PLoweLayer*(C14percent/100)*CNfraction;
    romcPlantC14(2,1)=(startAmountLayer2)*(C14percent/100)-humcPlantC14(2,1)-fomcPlantC14(2,1);

end
sizeOfInput=size(data);
sizeOfInput=sizeOfInput(2);

numberOfIterationsWithExtraCarbon=iterations-numberOfYears*12;
while i <= iterations 


  month=mod(i,12)+1;
  year=floor((i)/12)+1;
  julianday=month*30.4166;
  if(data(year,7)~=-1)
     inputsurface=data(year,7);
  else
     inputsurface= anualAmountPlant*0.9;
  end
  if(data(year,8)~=-1)
      inputSubSoil=data(year,8);
  else
     inputSubSoil= anualAmountPlant*0.1;
  end


      if(sizeOfInput>=10&&~isnan(data(year,10)))
        inputManure= data(year,10);
      else
        inputManure=0;
      end
 
  if(sizeOfInput>=12&&~isnan(data(year,12)))
    inputSoilC14percent=data(year,11);
    inputManureC14percent=data(year,12);
  else
	inputSoilC14percent=0;
	inputManureC14percent=0;
  end


  avgTemperature=TemperatureData((i+1));
  t=1.0;
  if(mod(month,12)==3)
      fomcManure(1,1)=fomcManure(1,1)+(inputManure*(1-HUMfractionManure));
      humcManure(1,1)=humcManure(1,1)+(inputManure*HUMfractionManure); 
      if(withC14Manure==true)
          fomcManureC14(1,1)=fomcManureC14(1,1)+((inputManure*(1-HUMfractionManureC14)))*(inputManureC14percent/100);
          humcManureC14(1,1)=humcManureC14(1,1)+((inputManure*HUMfractionManureC14))*(inputManureC14percent/100);         
      end
  end
  if(iterations>numberOfIterationsWithExtraCarbon)
    fomcPlant(1,1)=fomcPlant(1,1)+extraCarbon;
  end
  if(mod(month,12)==4)
      fomcPlant(1,1)=fomcPlant(1,1)+(inputsurface)*0.08;
      fomcPlant(2,1)=fomcPlant(2,1)+(inputSubSoil)*0.08;

      fomcPlantC14(1,1)=fomcPlantC14(1,1)+(inputsurface*(inputSoilC14percent/100))*0.08;
      fomcPlantC14(2,1)=fomcPlantC14(2,1)+(inputSubSoil*(inputSoilC14percent/100))*0.08;

      
  end
  if(mod(month,12)==5)
      fomcPlant(1,1)=fomcPlant(1,1)+(inputsurface)*0.12;
      fomcPlant(2,1)=fomcPlant(2,1)+(inputSubSoil)*0.12;

      
      fomcPlantC14(1,1)=fomcPlantC14(1,1)+(inputsurface*(inputSoilC14percent/100))*0.12;
      fomcPlantC14(2,1)=fomcPlantC14(2,1)+(inputSubSoil*(inputSoilC14percent/100))*0.12;

      
  end
  if(mod(month,12)==6)
      fomcPlant(1,1)=fomcPlant(1,1)+(inputsurface)*0.16;
      fomcPlant(2,1)=fomcPlant(2,1)+(inputSubSoil)*0.16;

      
      fomcPlantC14(1,1)=fomcPlantC14(1,1)+(inputsurface*(inputSoilC14percent/100))*0.16;
      fomcPlantC14(2,1)=fomcPlantC14(2,1)+(inputSubSoil*(inputSoilC14percent/100))*0.16;

  end
  if(mod(month,12)==7)
      fomcPlant(1,1)=fomcPlant(1,1)+(inputsurface)*0.64;
      fomcPlant(2,1)=fomcPlant(2,1)+(inputSubSoil)*0.64;

      
      fomcPlantC14(1,1)=fomcPlantC14(1,1)+(inputsurface*(inputSoilC14percent/100))*0.64;
      fomcPlantC14(2,1)=fomcPlantC14(2,1)+(inputSubSoil*(inputSoilC14percent/100))*0.64;

  end
  %plant 
    RfractionPlant=R(ClayfractionPlant);
  humificationPlant=1/(RfractionPlant+1); 

  % decomposition of FOM
result=decompositionFOM(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,julianday,fomcPlant,FOMdecompositionratePlant,tFPlant,humificationPlant,humcPlant,TransportFom,Co2Fom,AmountFom,outputInformation,WithTransport,year,month);
fomcPlant=result(:,1);

humcPlant=result(:,2);
   % decomposition of hum
result=decompositionHUM(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,julianday,HUMdecompositionrate,humcPlant,TransportHum,Co2Hum,AmountHum,ROMfractionPlant,romcPlant,outputInformation,WithTransport,year,month);
humcPlant=result(:,1);
romcPlant=result(:,2);
 %decomposition of ROM
romcPlant= decompositionROM(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,julianday,TransportRom,Co2Rom,AmountRom,romcPlant,ROMdecompositionratePlant,outputInformation,WithTransport,year,month);
%Manure
RfractionManure=R(ClayfractionManure);
humificationManure=1/(RfractionManure+1); 
result=decompositionFOM(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,julianday,fomcManure,FOMdecompositionrateManure,tFManure,humificationManure,humcManure,TransportFomManure,Co2FomManure,AmountFomManure,outputInformation,WithTransport,year,month);
fomcManure=result(:,1);
humcManure=result(:,2);
   % decomposition of hum
result=decompositionHUM(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,julianday,HUMdecompositionrate,humcManure,TransportHumManure,Co2HumManure,AmountHumManure,ROMfractionManure,romcManure,outputInformation,WithTransport,year,month);
humcManure=result(:,1);
romcManure=result(:,2);
 %decomposition of ROM
romcManure= decompositionROM(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,julianday,TransportRomManure,Co2RomManure,AmountRomManure,romcManure,ROMdecompositionrateManure,outputInformation,WithTransport,year,month);

 %plant C14
    RfractionPlantC14=R(ClayfractionPlantC14);
  humificationPlantC14=1/(RfractionPlantC14+1); 

  % decomposition of FOM
result=decompositionFOMC14(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,julianday,fomcPlantC14,FOMdecompositionratePlantC14,tFPlantC14,humificationPlantC14,humcPlantC14,TransportFomC14,Co2FomC14,AmountFomC14, outputInformation,DecayRateC14Plant,WithTransport,year,month);
fomcPlantC14=result(:,1);
humcPlantC14=result(:,2);
   % decomposition of hum
result=decompositionHUMC14(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,julianday,HUMdecompositionrate,humcPlantC14,TransportHumC14,Co2HumC14,AmountHumC14,ROMfractionPlantC14,romcPlantC14,outputInformation,DecayRateC14Plant,WithTransport,year,month);
humcPlantC14=result(:,1);
romcPlantC14=result(:,2);
 %decomposition of ROM
romcPlantC14= decompositionROMC14(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,julianday,TransportRomC14,Co2RomC14,AmountRomC14,romcPlantC14,ROMdecompositionratePlantC14,outputInformation,DecayRateC14Plant,WithTransport,year,month);
if(C14==true)
    if(withC14Manure==true)
        %Manure C14
        RfractionManureC14=R(ClayfractionManureC14);
        humificationManureC14=1/(RfractionManureC14+1); 
        result=decompositionFOMC14(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,julianday,fomcManureC14,FOMdecompositionrateManureC14,tFManureC14,humificationManureC14,humcManureC14,TransportFomManureC14,Co2FomManureC14,AmountFomManureC14,outputInformation,DecayRateC14Manuer,WithTransport,year,month);
        fomcManureC14=result(:,1);
        humcManureC14=result(:,2);
           % decomposition of hum
        result=decompositionHUMC14(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,julianday,HUMdecompositionrate,humcManureC14,TransportHumManureC14,Co2HumManureC14,AmountHumManureC14,ROMfractionManureC14,romcManureC14,outputInformation,DecayRateC14Manuer,WithTransport,year,month);
        humcManureC14=result(:,1);
        romcManureC14=result(:,2);
         %decomposition of ROM
        romcManureC14= decompositionROMC14(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,julianday,TransportRomManureC14,Co2RomManureC14,AmountRomManureC14,romcManureC14,ROMdecompositionrateManureC14,outputInformation,DecayRateC14Manuer,WithTransport,year,month);
    end
end

if((iterations-numberOfPossibleObservation*12)<i)
    if(month==12)
        if(fullOutput==0)
            sizeofObserved=size(ObservationWithNan);
            if(placeInObservationWithNan<sizeofObserved(1))
                if(~isnan(ObservationWithNan(placeInObservationWithNan,1)))
                    if(data(year,23)==0)
                        amount(placeInOutput,1)=romcPlant(1,1)+humcPlant(1,1)+fomcPlant(1,1)+romcManureC14(1,1)+humcManureC14(1,1)+fomcManureC14(1,1);
                    else
                        amount(placeInOutput,1)=romcPlantC14(1,1)+humcPlantC14(1,1)+fomcPlantC14(1,1)+romcManureC14(1,1)+humcManureC14(1,1)+fomcManureC14(1,1);
                    end
                 placeInOutput=placeInOutput+1;
                end
            else
                sizeofObserved(1);
            end
        elseif(fullOutput==1)
            sizeofObserved=size(ObservationWithNan);
            if(placeInObservationWithNan<sizeofObserved(1))
                if(~isnan(ObservationWithNan(placeInObservationWithNan,1)))
                    if(data(year,23)==0)
                        amount(placeInOutput,1)=romcPlant(2,1)+humcPlant(2,1)+fomcPlant(2,1)+romcManureC14(2,1)+humcManureC14(2,1)+fomcManureC14(2,1);
                    else
                        amount(placeInOutput,1)=romcPlantC14(2,1)+humcPlantC14(2,1)+fomcPlantC14(2,1)+romcManureC14(2,1)+humcManureC14(2,1)+fomcManureC14(2,1);
                    end
                 placeInOutput=placeInOutput+1;
                end
            else
                sizeofObserved(1);
            end
        elseif(fullOutput==3)
            sizeofObserved=size(ObservationWithNan);

            halfway=sizeOfObserved/2;
            if(placeInObservationWithNan<=sizeofObserved(1))
                if(~isnan(ObservationWithNan(placeInObservationWithNan,1)))
                    amount(placeInOutput,1)=romcPlant(1,1)+humcPlant(1,1)+fomcPlant(1,1)+romcManure(1,1)+humcManure(1,1)+fomcManure(1,1);
                    placeInOutput=placeInOutput+1;
                end
            else
                sizeofObserved(1);
            end
            
        else
            amount(placeInOutput,1)=romcPlant(1,1)+humcPlant(1,1)+fomcPlant(1,1);
            amount(placeInOutput,2)=romcManure(1,1)+humcManure(1,1)+fomcManure(1,1);
            amount(placeInOutput,3)=  amount(placeInOutput,2)+  amount(placeInOutput,1);
            amount(placeInOutput,4)=romcPlantC14(1,1)+humcPlantC14(1,1)+fomcPlantC14(1,1);
            amount(placeInOutput,5)=romcManureC14(1,1)+humcManureC14(1,1)+fomcManureC14(1,1);
            amount(placeInOutput,6)= amount(placeInOutput,4)+  amount(placeInOutput,5);
            amount(placeInOutput,7)=amount(placeInOutput,6)/amount(placeInOutput,3)*100;
            amount(placeInOutput,8)=romcPlantC14(1,1);
            amount(placeInOutput,9)=humcPlantC14(1,1);
            amount(placeInOutput,10)=fomcPlantC14(1,1);
            amount(placeInOutput,11)=romcPlant(2,1)+humcPlant(2,1)+fomcPlant(2,1);
            amount(placeInOutput,12)=romcManure(2,1)+humcManure(2,1)+fomcManure(2,1);
            amount(placeInOutput,13)=  amount(placeInOutput,12)+  amount(placeInOutput,11);
            amount(placeInOutput,14)=romcPlantC14(2,1)+humcPlantC14(2,1)+fomcPlantC14(2,1);
            amount(placeInOutput,15)=romcManureC14(2,1)+humcManureC14(2,1)+fomcManureC14(2,1);
            amount(placeInOutput,16)= amount(placeInOutput,14)+  amount(placeInOutput,15);
            amount(placeInOutput,17)=amount(placeInOutput,16)/amount(placeInOutput,13)*100;
            amount(placeInOutput,18)=romcPlantC14(2,1);
            amount(placeInOutput,19)=humcPlantC14(2,1);
            amount(placeInOutput,20)=fomcPlantC14(2,1);
            if(abs(romcPlant(1,1)-humcPlant(1,1))<EpeselonBjorn)
                amount(placeInOutput,21)=true;
            else
                amount(placeInOutput,21)=false;
            end
            if(abs(HumcPlantLastYear-humcPlant(1,1))<EpeselonHum)
                amount(placeInOutput,22)=true;
            else
                amount(placeInOutput,22)=false;
            end
            if(abs(RomcPlantLastYear-romcPlant(1,1))<EpeselonRom)
                amount(placeInOutput,23)=true;
            else
                amount(placeInOutput,23)=false;
            end
            amount(placeInOutput,24)=humcPlant(1,1);
            amount(placeInOutput,25)=romcPlant(1,1);
            amount(placeInOutput,25)=romcPlant(1,1);
            PBeginningU=humcPlant(1,1)/( humcPlant(1,1)+romcPlant(1,1));
            PBeginningL=humcPlant(2,1)/( humcPlant(2,1)+romcPlant(2,1));
            amount(placeInOutput,26)=PBeginningU;
            amount(placeInOutput,27)=PBeginningL;
            RomcPlantLastYear=romcPlant(1,1);
            HumcPlantLastYear=humcPlant(1,1);
            
            
            placeInOutput=placeInOutput+1;
        end
        placeInObservationWithNan=placeInObservationWithNan+1;
    end
end
if(month==12)
fprintf(totalAmount,'%d\t',fomcPlant(1,1));
            fprintf(totalAmount,'%d\t',humcPlant(1,1));
            fprintf(totalAmount,'%d\t',romcPlant(1,1));

            fprintf(totalAmount,'%d\t',fomcManure(1,1));
            fprintf(totalAmount,'%d\t',humcManure(1,1));
            fprintf(totalAmount,'%d\t',romcManure(1,1));

            fprintf(totalAmount,'%d\t',fomcPlantC14(1,1));
            fprintf(totalAmount,'%d\t',humcPlantC14(1,1));
            fprintf(totalAmount,'%d\t',romcPlantC14(1,1));

            fprintf(totalAmount,'%d\t',fomcManureC14(1,1));
            fprintf(totalAmount,'%d\t',humcManureC14(1,1));
            fprintf(totalAmount,'%d\t',romcManureC14(1,1));
            
            fprintf(totalAmount,'%d\t',fomcPlant(1,1)+humcPlant(1,1)+romcPlant(1,1)+fomcManure(1,1)+humcManure(1,1)+romcManure(1,1));
            
            fprintf(totalAmount,'%d\t',fomcPlant(2,1));
            fprintf(totalAmount,'%d\t',humcPlant(2,1));
            fprintf(totalAmount,'%d\t',romcPlant(2,1));

            fprintf(totalAmount,'%d\t',fomcManure(2,1));
            fprintf(totalAmount,'%d\t',humcManure(2,1));
            fprintf(totalAmount,'%d\t',romcManure(2,1));

            fprintf(totalAmount,'%d\t',fomcPlantC14(2,1));
            fprintf(totalAmount,'%d\t',humcPlantC14(2,1));
            fprintf(totalAmount,'%d\t',romcPlantC14(2,1));

            fprintf(totalAmount,'%d\t',fomcManureC14(2,1));
            fprintf(totalAmount,'%d\t',humcManureC14(2,1));
            fprintf(totalAmount,'%d\t',romcManureC14(2,1));
            
            fprintf(totalAmount,'%d\t',fomcPlant(2,1)+humcPlant(2,1)+romcPlant(2,1)+fomcManure(2,1)+humcManure(2,1)+romcManure(2,1));
            fprintf(totalAmount,'%d\t',year);
            fprintf(totalAmount,'%d\t',month);
            PBeginningU=humcPlant(1,1)/( humcPlant(1,1)+romcPlant(1,1));
            PBeginningL=humcPlant(2,1)/( humcPlant(2,1)+romcPlant(2,1));
            fprintf(totalAmount,'%d\t',PBeginningU);
            fprintf(totalAmount,'%d\r\n',PBeginningL);
end
 i = i + 1;
end
if(outputInformation==1)
    fclose(TransportHum );
    fclose(Co2Hum );
    fclose(AmountHum );
    fclose(TransportFom );
    fclose(Co2Fom );
    fclose(AmountFom );
    fclose(TransportRom );
    fclose(Co2Rom );
    fclose(AmountRom );
    fclose(TransportHumManure );
    fclose(Co2HumManure );
    fclose(AmountHumManure );
    fclose(TransportFomManure );
    fclose(Co2FomManure );
    fclose(AmountFomManure );
    fclose(TransportRomManure );
    fclose(Co2RomManure );
    fclose(AmountRomManure );

    fclose(TransportHumC14 );
    fclose(Co2HumC14 );
    fclose(AmountHumC14 );
    fclose(TransportFomC14 );
    fclose(Co2FomC14 );
    fclose(AmountFomC14 );
    fclose(TransportRomC14 );
    fclose(Co2RomC14 );
    fclose(AmountRomC14 );
    fclose(TransportHumManureC14 );
    fclose(Co2HumManureC14 );
    fclose(AmountHumManureC14 );
    fclose(TransportFomManureC14 );
    fclose(Co2FomManureC14 );
    fclose(AmountFomManureC14 );
    fclose(TransportRomManureC14 );
    fclose(Co2RomManureC14 );
    fclose(AmountRomManureC14 );
    
end
fclose(totalAmount);