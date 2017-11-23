function amount = Ctool2CoreerosionSimple(sizeOfObserved, usingRK, runNR,num,data,TemperatureData,dataPlaces, C14, startingAmountFromInput, ObservationWithNan, fullOutput, numberOfPossibleObservation, WithTransport )
withC14Manure =true;
i = 0;
 placeInOutput=1;
 placeInObservationWithNan=1;
 outputInformation=true;
    PLoweLayer=num(1);
offset=num(2);
depth=num(3);
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
extraCarbon=extraCarbon/12;
errosionDeph=num(9);
if(startingAmountFromInput==-1)
startAmountOfCarbon=startingAmountFromFile;
else
    startAmountOfCarbon=startingAmountFromInput;
end
startAmountLayer1=startAmountOfCarbon*0.47;
startAmountLayer2=startAmountOfCarbon*0.53;

numberOfLayers=2;
%plant
HUMfractionPlant=num(12);
HUMdecompositionratePlant=num(13);
FOMdecompositionratePlant=num(15);
ClayfractionPlant=num(16);
tFPlant=num(17);
ROMfractionPlant=num(19);
ROMdecompositionratePlant =num(20);
%manure
HUMfractionManure=num(23);
HUMdecompositionrateManure=num(24);
FOMdecompositionrateManure=num(26);
ClayfractionManure=num(27);
tFManure=num(28);
ROMfractionManure=num(30);
ROMdecompositionrateManure =num(31);

%plant C14
HUMfractionPlantC14=num(34);
HUMdecompositionratePlantC14=num(35);
FOMdecompositionratePlantC14=num(37);
ClayfractionPlantC14=num(38);
tFPlantC14=num(39);
ROMfractionPlantC14=num(41);
ROMdecompositionratePlantC14 =num(42);
DecayRateC14Plant =num(43);
%manure C14
HUMfractionManureC14=num(46);
HUMdecompositionrateManureC14=num(47);
FOMdecompositionrateManureC14=num(49);
ClayfractionManureC14=num(50);
tFManureC14=num(51);
ROMfractionManureC14=num(53);

ROMdecompositionrateManureC14 =num(54);
DecayRateC14Manuer =num(55);

amount=zeros(sizeOfObserved,1);


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
inputForErosion=dlmread(dataPlaces{5+runNR*5}, '\t');
output =strcat(dataPlaces{4+runNR*5},'\erosion.xls');
erosion = fopen(output,'w');
if(outputInformation==1)
%plant output
    output =strcat(dataPlaces{4+runNR*5},'\TransportHum.xls');
    TransportHum = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\Co2Hum.xls');
    Co2Hum = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\AmountHum.xls');
    AmountHum = fopen(output,'w');

    output =strcat(dataPlaces{4+runNR*5},'\TransportFom.xls');
    TransportFom = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\Co2Fom.xls');
    Co2Fom = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\AmountFom.xls');
    AmountFom = fopen(output,'w');

    output =strcat(dataPlaces{4+runNR*5},'\TransportRom.xls');
    TransportRom = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\Co2Rom.xls');
    Co2Rom = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\AmountRom.xls');
    AmountRom = fopen(output,'w');

    %manure output
    output =strcat(dataPlaces{4+runNR*5},'\TransportHumManure.xls');
    TransportHumManure = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\Co2HumManure.xls');
    Co2HumManure = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\AmountHumManure.xls');
    AmountHumManure = fopen(output,'w');

    output =strcat(dataPlaces{4+runNR*5},'\TransportFomManure.xls');
    TransportFomManure = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\Co2FomManure.xls');
    Co2FomManure = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\AmountFomManure.xls');
    AmountFomManure = fopen(output,'w');

    output =strcat(dataPlaces{4+runNR*5},'\TransportRomManure.xls');
    TransportRomManure = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\Co2RomManure.xls');
    Co2RomManure = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\AmountRomManure.xls');
    AmountRomManure = fopen(output,'w');

    %plant output C14
    output =strcat(dataPlaces{4+runNR*5},'\TransportHumC14.xls');
    TransportHumC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\Co2HumC14.xls');
    Co2HumC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\AmountHumC14.xls');
    AmountHumC14 = fopen(output,'w');

    output =strcat(dataPlaces{4+runNR*5},'\TransportFomC14.xls');
    TransportFomC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\Co2FomC14.xls');
    Co2FomC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\AmountFomC14.xls');
    AmountFomC14 = fopen(output,'w');

    output =strcat(dataPlaces{4+runNR*5},'\TransportRomC14.xls');
    TransportRomC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\Co2RomC14.xls');
    Co2RomC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\AmountRomC14.xls');
    AmountRomC14 = fopen(output,'w');

    %manure output
    output =strcat(dataPlaces{4+runNR*5},'\TransportHumManureC14.xls');
    TransportHumManureC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\Co2HumManureC14.xls');
    Co2HumManureC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\AmountHumManureC14.xls');
    AmountHumManureC14 = fopen(output,'w');

    output =strcat(dataPlaces{4+runNR*5},'\TransportFomManureC14.xls');
    TransportFomManureC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\Co2FomManureC14.xls');
    Co2FomManureC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\AmountFomManureC14.xls');
    AmountFomManureC14 = fopen(output,'w');

    output =strcat(dataPlaces{4+runNR*5},'\TransportRomManureC14.xls');
    TransportRomManureC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\Co2RomManureC14.xls');
    Co2RomManureC14 = fopen(output,'w');
    output =strcat(dataPlaces{4+runNR*5},'\AmountRomManureC14.xls');
    AmountRomManureC14 = fopen(output,'w');
end

fomcPlant=zeros(numberOfLayers,1);
romcPlant=zeros(numberOfLayers,1);


fomcManure=zeros(numberOfLayers,1);
romcManure=zeros(numberOfLayers,1);


fomcPlantC14=zeros(numberOfLayers,1);
romcPlantC14=zeros(numberOfLayers,1);

fomcManureC14=zeros(numberOfLayers,1);
romcManureC14=zeros(numberOfLayers,1);


sizeofDataset=size(data);
iterations=sizeofDataset(1,1)*12-1;
CNfraction=1;
romcPlant(1,1)=startAmountLayer1*PupperLayer*CNfraction;
romcPlant(2,1)=startAmountLayer2*PLoweLayer*CNfraction;

if(C14==true)
romcPlantC14(1,1)=(startAmountLayer1)*PupperLayer*(C14percent/100)*CNfraction;
romcPlantC14(2,1)=(startAmountLayer2)*PLoweLayer*(C14percent/100)*CNfraction;

end
sizeOfInput=size(data);
sizeOfInput=sizeOfInput(2);
while i < iterations 
  
  month=mod(i,12)+1;
  year=floor((i)/12)+1;
  julianday=month*30.4166;
  inputsurface=data(year,7);
  inputSubSoil=data(year,8);
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
  t=0.083;
  if(mod(month,12)==3)
      fomcManure(1,1)=fomcManure(1,1)+(inputManure*(1-HUMfractionManure));
      romcManure(1,1)=romcManure(1,1)+(inputManure*HUMfractionManure); 
      if(withC14Manure==true)
          fomcManureC14(1,1)=(fomcManureC14(1,1)+(inputManure*(1-HUMfractionManureC14)))*(inputManureC14percent/100);
          romcManureC14(1,1)=(romcManureC14(1,1)+(inputManure*HUMfractionManureC14))*(inputManureC14percent/100);  
        
      end
  end
  fomcPlant(1,1)=fomcPlant(1,1)+extraCarbon;
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
      fomcPlant(2,1)=fomcPlant(1,1)+(inputSubSoil)*0.64;

      
      fomcPlantC14(1,1)=fomcPlantC14(1,1)+(inputsurface*(inputSoilC14percent/100))*0.64;
      fomcPlantC14(2,1)=fomcPlantC14(1,1)+(inputSubSoil*(inputSoilC14percent/100))*0.64;

  end
  %plant 
    RfractionPlant=R(ClayfractionPlant);
  humificationPlant=1/(RfractionPlant+1); 

  % decomposition of FOM
result=decayFom(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,julianday,fomcPlant,FOMdecompositionratePlant,tFPlant,humificationPlant,romcPlant,TransportFom,Co2Fom,AmountFom,outputInformation,WithTransport);
fomcPlant=result(:,1);
romcPlant=result(:,2);
 %decomposition of ROM
romcPlant= decayRom(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,julianday,TransportRom,Co2Rom,AmountRom,romcPlant,ROMdecompositionratePlant,outputInformation,WithTransport);
%Manure
RfractionManure=R(ClayfractionManure);
humificationManure=1/(RfractionManure+1); 
result=decayFom(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,julianday,fomcManure,FOMdecompositionrateManure,tFManure,humificationManure,romcManure,TransportFomManure,Co2FomManure,AmountFomManure,outputInformation,WithTransport);
fomcManure=result(:,1);
romcManure=result(:,2);
 %decomposition of ROM
romcManure= decayRom(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,julianday,TransportRomManure,Co2RomManure,AmountRomManure,romcManure,ROMdecompositionrateManure,outputInformation,WithTransport);

 %plant C14
    RfractionPlantC14=R(ClayfractionPlantC14);
  humificationPlantC14=1/(RfractionPlantC14+1); 

  % decomposition of FOM
result=decayFomC14(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,julianday,fomcPlantC14,FOMdecompositionratePlantC14,tFPlantC14,humificationPlantC14,romcPlantC14,TransportFomC14,Co2FomC14,AmountFomC14, outputInformation,DecayRateC14Plant,WithTransport);
fomcPlantC14=result(:,1);
romcPlantC14=result(:,2);
 %decomposition of ROM
romcPlantC14= decayRomC14(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,julianday,TransportRomC14,Co2RomC14,AmountRomC14,romcPlantC14,ROMdecompositionratePlantC14,outputInformation,DecayRateC14Plant,WithTransport);
if(C14==true)
    if(withC14Manure==true)
        %Manure C14
        RfractionManureC14=R(ClayfractionManureC14);
        humificationManureC14=1/(RfractionManureC14+1); 
        result=decayFomC14(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,i,fomcManureC14,FOMdecompositionrateManureC14,tFManureC14,humificationManureC14,romcManureC14,TransportFomManureC14,Co2FomManureC14,AmountFomManureC14,outputInformation,DecayRateC14Manuer,WithTransport);
        fomcManureC14=result(:,1);
        romcManureC14=result(:,2);

         %decomposition of ROM
        romcManureC14= decayRomC14(numberOfLayers,depth,avgTemperature,amplitude,offset,usingRK,t,julianday,TransportRomManureC14,Co2RomManureC14,AmountRomManureC14,romcManureC14,ROMdecompositionrateManureC14,outputInformation,DecayRateC14Manuer,WithTransport);
    end
end
if(mod(month,12)==4)
    %calcualted total amout of Carbon in layer 1
    AmoutnCarbonPlant=fomcPlant(1,1)+romcPlant(1,1);
    AmoutnCarbonManure=fomcManure(1,1)+romcManure(1,1);

    AmoutnCarbonPlantC14=fomcPlantC14(1,1)+romcPlantC14(1,1);
    AmoutnCarbonManureC14=fomcManureC14(1,1)+romcManureC14(1,1);

    %removed becouse of errosion
    fomcPlant(1,1)=fomcPlant(1,1)-fomcPlant(1,1)/AmoutnCarbonPlant*AmoutnCarbonPlant*errosionDeph/25;
    fprintf(erosion,'%d\t',fomcPlant(1,1)/AmoutnCarbonPlant*AmoutnCarbonPlant*errosionDeph/25);
    romcPlant(1,1)=romcPlant(1,1)-romcPlant(1,1)/AmoutnCarbonPlant*AmoutnCarbonPlant*errosionDeph/25;
    fprintf(erosion,'%d\t',romcPlant(1,1)/AmoutnCarbonPlant*AmoutnCarbonPlant*errosionDeph/25);
    if(AmoutnCarbonManure~=0)
    fomcManure(1,1)=fomcManure(1,1)-fomcManure(1,1)/AmoutnCarbonManure*AmoutnCarbonManure*errosionDeph/25;
    fprintf(erosion,'%d\t',fomcManure(1,1)/AmoutnCarbonManure*AmoutnCarbonManure*errosionDeph/25);
    romcManure(1,1)=romcManure(1,1)-romcManure(1,1)/AmoutnCarbonManure*AmoutnCarbonManure*errosionDeph/25;
    fprintf(erosion,'%d\t',romcManure(1,1)/AmoutnCarbonManure*AmoutnCarbonManure*errosionDeph/25);
    else
        fprintf(erosion,'%d\t',0);
        fprintf(erosion,'%d\t',0);
    end
    if(AmoutnCarbonPlantC14~=0)
    fomcPlantC14(1,1)=fomcPlantC14(1,1)-fomcPlantC14(1,1)/AmoutnCarbonPlantC14*AmoutnCarbonPlantC14*errosionDeph/25;
    fprintf(erosion,'%d\t',fomcPlantC14(1,1)/AmoutnCarbonPlantC14*AmoutnCarbonPlantC14*errosionDeph/25);
    romcPlantC14(1,1)=romcPlantC14(1,1)-romcPlantC14(1,1)/AmoutnCarbonPlantC14*AmoutnCarbonPlantC14*errosionDeph/25;
    fprintf(erosion,'%d\t',romcPlantC14(1,1)/AmoutnCarbonPlantC14*AmoutnCarbonPlantC14*errosionDeph/25);
    else
        fprintf(erosion,'%d\t',0);
        fprintf(erosion,'%d\t',0);

    end

    if(AmoutnCarbonManureC14~=0)
    fomcManureC14(1,1)=fomcManureC14(1,1)-fomcManureC14(1,1)/AmoutnCarbonManureC14*AmoutnCarbonManureC14*errosionDeph/25;
    fprintf(erosion,'%d\t',fomcManureC14(1,1)/AmoutnCarbonManureC14*AmoutnCarbonManureC14*errosionDeph/25);
    romcManureC14(1,1)=romcManureC14(1,1)-romcManureC14(1,1)/AmoutnCarbonManureC14*AmoutnCarbonManureC14*errosionDeph/25;
    fprintf(erosion,'%d\t',romcManureC14(1,1)/AmoutnCarbonManureC14*AmoutnCarbonManureC14*errosionDeph/25);
    else
        fprintf(erosion,'%d\t',0);
        fprintf(erosion,'%d\t',0);

    end
    fprintf(erosion,'%d\n',0);
    %calcualted total amout of Carbon in layer 2
    AmoutnCarbonPlant=fomcPlant(2,1)+romcPlant(2,1);
    AmoutnCarbonManure=fomcManure(2,1)+romcManure(2,1);

    AmoutnCarbonPlantC14=fomcPlantC14(2,1)+romcPlantC14(2,1);
    AmoutnCarbonManureC14=fomcManureC14(2,1)+romcManureC14(2,1);

    %added to layer 1 becouse of errosion
    fomcPlant(1,1)=fomcPlant(1,1)+fomcPlant(2,1)/AmoutnCarbonPlant*AmoutnCarbonPlant*errosionDeph/75;
    romcPlant(1,1)=romcPlant(1,1)+romcPlant(2,1)/AmoutnCarbonPlant*AmoutnCarbonPlant*errosionDeph/75;
   

    if(AmoutnCarbonManure~=0)
    fomcManure(1,1)=fomcManure(1,1)+fomcManure(2,1)/AmoutnCarbonManure*AmoutnCarbonManure*errosionDeph/75;
    romcManure(1,1)=romcManure(1,1)+romcManure(2,1)/AmoutnCarbonManure*AmoutnCarbonManure*errosionDeph/75;
   
    end
    if(AmoutnCarbonPlantC14~=0)
    fomcPlantC14(1,1)=fomcPlantC14(1,1)+fomcPlantC14(2,1)/AmoutnCarbonPlantC14*AmoutnCarbonPlantC14*errosionDeph/75;
    romcPlantC14(1,1)=romcPlantC14(1,1)+romcPlantC14(2,1)/AmoutnCarbonPlantC14*AmoutnCarbonPlantC14*errosionDeph/75;
   
    end
    if(AmoutnCarbonManureC14~=0)
    fomcManureC14(1,1)=fomcManureC14(1,1)+fomcManureC14(2,1)/AmoutnCarbonManureC14*AmoutnCarbonManureC14*errosionDeph/75;
    romcManureC14(1,1)=romcManureC14(1,1)+romcManureC14(2,1)/AmoutnCarbonManureC14*AmoutnCarbonManureC14*errosionDeph/75;
    
    end
    
    %adding Carbon becouse of erosion

    fomcPlant(1,1)=fomcPlant(1,1)+inputForErosion(year,1);
    romcPlant(1,1)=romcPlant(1,1)+inputForErosion(year,2);
    fomcManure(1,1)=fomcManure(1,1)+inputForErosion(year,3);
    romcManure(1,1)=romcManure(1,1)+inputForErosion(year,4);
    fomcPlantC14(1,1)=fomcPlantC14(1,1)+inputForErosion(year,5);
    romcPlantC14(1,1)=romcPlantC14(1,1)+inputForErosion(year,6);
    fomcManureC14(1,1)=fomcManureC14(1,1)+inputForErosion(year,7);
    romcManureC14(1,1)=romcManureC14(1,1)+inputForErosion(year,8);
    %calcualted total amout of Carbon in layer 1
AmoutnCarbonPlant=fomcPlant(1,1)+romcPlant(1,1);
AmoutnCarbonManure=fomcManure(1,1)+romcManure(1,1);

AmoutnCarbonPlantC14=fomcPlantC14(1,1)+romcPlantC14(1,1);
AmoutnCarbonManureC14=fomcManureC14(1,1)+romcManureC14(1,1);
%moveing soil down
fomcPlant(2,1)=fomcPlant(2,1)+fomcPlant(1,1)/AmoutnCarbonPlant*AmoutnCarbonPlant*errosionDeph/25;
romcPlant(2,1)=romcPlant(2,1)+romcPlant(1,1)/AmoutnCarbonPlant*AmoutnCarbonPlant*errosionDeph/25;
if(AmoutnCarbonManure~=0)
fomcManure(2,1)=fomcManure(2,1)+fomcManure(1,1)/AmoutnCarbonManure*AmoutnCarbonManure*errosionDeph/25;
romcManure(2,1)=romcManure(2,1)+romcManure(1,1)/AmoutnCarbonManure*AmoutnCarbonManure*errosionDeph/25;
end
if(AmoutnCarbonPlantC14~=0)
fomcPlantC14(2,1)=fomcPlantC14(2,1)+fomcPlantC14(1,1)/AmoutnCarbonPlantC14*AmoutnCarbonPlantC14*errosionDeph/25;
romcPlantC14(2,1)=romcPlantC14(2,1)+romcPlantC14(1,1)/AmoutnCarbonPlantC14*AmoutnCarbonPlantC14*errosionDeph/25;
end
if(AmoutnCarbonManureC14~=0)
fomcManureC14(2,1)=fomcManureC14(2,1)+fomcManureC14(1,1)/AmoutnCarbonManureC14*AmoutnCarbonManureC14*errosionDeph/25;
romcManureC14(2,1)=romcManureC14(2,1)+romcManureC14(1,1)/AmoutnCarbonManureC14*AmoutnCarbonManureC14*errosionDeph/25;
end

%removed the carbon that has been moved down
fomcPlant(1,1)=fomcPlant(1,1)-fomcPlant(1,1)/AmoutnCarbonPlant*AmoutnCarbonPlant*errosionDeph/25;
romcPlant(1,1)=romcPlant(1,1)-romcPlant(1,1)/AmoutnCarbonPlant*AmoutnCarbonPlant*errosionDeph/25;
if(AmoutnCarbonManure~=0)
fomcManure(1,1)=fomcManure(1,1)-fomcManure(1,1)/AmoutnCarbonManure*AmoutnCarbonManure*errosionDeph/25;
romcManure(1,1)=romcManure(1,1)-romcManure(1,1)/AmoutnCarbonManure*AmoutnCarbonManure*errosionDeph/25;
end
if(AmoutnCarbonPlantC14~=0)
fomcPlantC14(1,1)=fomcPlantC14(1,1)-fomcPlantC14(1,1)/AmoutnCarbonPlantC14*AmoutnCarbonPlantC14*errosionDeph/25;
romcPlantC14(1,1)=romcPlantC14(1,1)-romcPlantC14(1,1)/AmoutnCarbonPlantC14*AmoutnCarbonPlantC14*errosionDeph/25;
end
if(AmoutnCarbonManureC14~=0)
fomcManureC14(1,1)=fomcManureC14(1,1)-fomcManureC14(1,1)/AmoutnCarbonManureC14*AmoutnCarbonManureC14*errosionDeph/25;
romcManureC14(1,1)=romcManureC14(1,1)-romcManureC14(1,1)/AmoutnCarbonManureC14*AmoutnCarbonManureC14*errosionDeph/25;
end
AmoutnCarbonPlant=fomcPlant(2,1)+romcPlant(2,1);
    AmoutnCarbonManure=fomcManure(2,1)+romcManure(2,1);

    AmoutnCarbonPlantC14=fomcPlantC14(2,1)+romcPlantC14(2,1);
    AmoutnCarbonManureC14=fomcManureC14(2,1)+romcManureC14(2,1);

 %removed carbon from 2. layer 
    fomcPlant(2,1)=fomcPlant(2,1)-fomcPlant(2,1)/AmoutnCarbonPlant*AmoutnCarbonPlant*errosionDeph/75;
    romcPlant(2,1)=romcPlant(2,1)-romcPlant(2,1)/AmoutnCarbonPlant*AmoutnCarbonPlant*errosionDeph/75;
  

    if(AmoutnCarbonManure~=0)
    fomcManure(2,1)=fomcManure(2,1)-fomcManure(2,1)/AmoutnCarbonManure*AmoutnCarbonManure*errosionDeph/75;
    romcManure(2,1)=romcManure(2,1)-romcManure(2,1)/AmoutnCarbonManure*AmoutnCarbonManure*errosionDeph/75;
   
    end
    if(AmoutnCarbonPlantC14~=0)
    fomcPlantC14(2,1)=fomcPlantC14(2,1)-fomcPlantC14(2,1)/AmoutnCarbonPlantC14*AmoutnCarbonPlantC14*errosionDeph/75;
    romcPlantC14(2,1)=romcPlantC14(2,1)-romcPlantC14(2,1)/AmoutnCarbonPlantC14*AmoutnCarbonPlantC14*errosionDeph/75;
   
    end
    if(AmoutnCarbonManureC14~=0)
    fomcManureC14(2,1)=fomcManureC14(2,1)-fomcManureC14(2,1)/AmoutnCarbonManureC14*AmoutnCarbonManureC14*errosionDeph/75;
    romcManureC14(2,1)=romcManureC14(2,1)-romcManureC14(2,1)/AmoutnCarbonManureC14*AmoutnCarbonManureC14*errosionDeph/75;
   
    end

end
if((iterations-numberOfPossibleObservation*12)<i)
    if(month==9)
        if(fullOutput==false)
            if(~isnan(ObservationWithNan(placeInObservationWithNan,1)))
                if(data(year,23)==0)
                    amount(placeInOutput,1)=romcPlant(1,1)+fomcPlant(1,1)+romcManureC14(1,1)+fomcManureC14(1,1);
                else
                    amount(placeInOutput,1)=romcPlantC14(1,1)+fomcPlantC14(1,1)+romcManureC14(1,1)+fomcManureC14(1,1);
                end
            %amount(placeInOutput,2)=romcManure(1,1)+romcManure(1,1)+fomcManure(1,1);
             %amount(placeInOutput,3)=  amount(placeInOutput,2)+  amount(placeInOutput,1);
             %amount(placeInOutput,4)=romcPlantC14(1,1)+romcPlantC14(1,1)+fomcPlantC14(1,1);
             %amount(placeInOutput,5)=romcManureC14(1,1)+romcManureC14(1,1)+fomcManureC14(1,1);
             %amount(placeInOutput,6)= amount(placeInOutput,4)+  amount(placeInOutput,5);
             %amount(placeInOutput,7)=amount(placeInOutput,6)/amount(placeInOutput,3)*100+100;
             %amount(placeInOutput,1)=romcPlant(2,1)+romcPlant(2,1)+fomcPlant(2,1)+amount(placeInOutput,1);
             %amount(placeInOutput,1)=romcPlant(3,1)+romcPlant(3,1)+fomcPlant(3,1)+amount(placeInOutput,1);
             %amount(placeInOutput,1)=romcPlant(4,1)+romcPlant(4,1)+fomcPlant(4,1)+amount(placeInOutput,1);
             placeInOutput=placeInOutput+1;
            end
        else
            amount(placeInOutput,1)=romcPlant(1,1)+fomcPlant(1,1);
            amount(placeInOutput,2)=romcManure(1,1)+fomcManure(1,1);
            amount(placeInOutput,3)=  amount(placeInOutput,2)+  amount(placeInOutput,1);
            amount(placeInOutput,4)=romcPlantC14(1,1)+fomcPlantC14(1,1);
            amount(placeInOutput,5)=romcManureC14(1,1)+fomcManureC14(1,1);
            amount(placeInOutput,6)= amount(placeInOutput,4)+  amount(placeInOutput,5);
            amount(placeInOutput,7)=amount(placeInOutput,6)/amount(placeInOutput,3)*100+100;
            amount(placeInOutput,8)=0;
            amount(placeInOutput,9)=romcPlantC14(1,1);
            amount(placeInOutput,10)=fomcPlantC14(1,1);
            placeInOutput=placeInOutput+1;
        end
        placeInObservationWithNan=placeInObservationWithNan+1;
    end
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
fclose(erosion);