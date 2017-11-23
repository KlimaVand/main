#include <iostream>
#include <cstring>
#include <string>
#include"products\liveMatter.h"
class Separation 
{

	
public:
		Separation();
		void receive (liveMatter slurry);
		void reset();
		void update();
	//	void process();
		int output();
private:
	// parameters
        double ManureToNameLiquid, ManureToIndexLiquid, ManureToNameSolid, ManureToIndexSolid;
			double slurryVol ;
        double slurryTAN ;
       int  interval;
	//   string separatortype;
        double sepindexTAN ;
		double sepindexVol;
		int days;
		liveMatter liquid;
		liveMatter solid;
		
};


