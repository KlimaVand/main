#include "separator.h"
#include "base/commonData.h"



Separation::Separation( )//constructor
{
        slurryVol=0.0; //Amount of slurry separated (Mg)
        slurryTAN=0.0; //koncentation of TAN in raw slurry (kg/Mg)
        interval=0; //interval of separation procedure
        sepindexTAN=0.0; //separation index TAN (Hjorth et al., 2010)
        sepindexVol=0.0; //separation index TAN (Hjorth et al., 2010)
	commonData data;
	data.readFile("StandardSeparator.dat");
	data.setCritical(true);
	data.FindSection("ScrewPress");
	data.FindItem("interval",&interval);
	data.FindItem("sepindexTAN",&sepindexTAN);
	data.FindItem("sepindexVol",&sepindexVol);
	data.FindItem("sepindexP",&sepindexVol);
	data.FindItem("sepindexDM",&sepindexVol);
	data.FindItem("sepindexOrgN",&sepindexTAN);
	data.FindItem("sepindexNO3",&sepindexVol);
	data.FindItem("sepindexK",&sepindexVol);
	data.FindItem("sepindexOrgM",&sepindexVol);
	data.FindItem("sepindexC",&sepindexVol);
	data.FindItem("sepindexS",&sepindexVol);
}

void Separation::receive(liveMatter slurry){ 
	
	liquid=slurry;
	liquid.Setamount(slurry.GetAmount()*sepindexVol);
	liquid.SetNH4_content(slurry.GetNH4_content() *sepindexTAN);
	liquid.SetP_content(slurry.GetP_content() *sepindexTAN);
	liquid.SetDryMatter(slurry.GetdryMatter() *sepindexTAN);
	liquid.SetorgN_content(slurry.GetorgN_content()*sepindexVol);
	liquid.SetNO3_content(slurry.GetNO3_content() *sepindexTAN);
	liquid.SetK_content(slurry.GetK_content() *sepindexTAN);
	liquid.Setorganic(slurry.Getorganic() *sepindexTAN);
	
	solid=slurry;
	solid.Setamount(slurry.GetAmount()*(1-sepindexVol));
	solid.SetNH4_content(slurry.GetNH4_content()*(1-sepindexTAN));
	solid.SetP_content(slurry.GetP_content() *(1-sepindexTAN));
	solid.SetDryMatter(slurry.GetdryMatter() *(1-sepindexTAN));
	solid.SetorgN_content(slurry.GetorgN_content()*(1-sepindexVol));
	solid.SetNO3_content(slurry.GetNO3_content() *(1-sepindexTAN));
	solid.SetK_content(slurry.GetK_content() *(1-sepindexTAN));
	solid.Setorganic(slurry.Getorganic() *(1-sepindexTAN));
}
void Separation::reset() {
    days=0;
 //   process();
}
/*void Separation::process (){
	
	ManureToNameSolid = slurryTAN*sepindexTAN;
	ManureToIndexSolid = slurryVol*sepindexVol;
	ManureToNameLiquid = slurryTAN*(1-sepindexTAN);
	ManureToIndexLiquid = slurryVol*(1-sepindexVol);
}*/

void Separation::update() {
	
    days++;
    if (days%interval==0)
		;
		//        process();
}
int Separation::output(){
	ofstream sepOut;
	sepOut.open ("sepOut.dat", ios::app );
	sepOut	<< solid.GetAmount()<< '\t' << '\t' << liquid.GetAmount() <<'\t'<< endl;
	sepOut.close();
	return 0;

}