#include "Slurry.h"
Slurry::Slurry()
{
amount=0.0;
DM=0.0;
Ash=0.0;
Tan=0.0;
RP=0.0;
fInert=0.0;
ADL=0.0;
NDF=0.0;
RL=0.0;
VFA=0.0;
TotalS=0.0;
SulphideS=0.0;
SulphateS=0.0;
pH=7.5;
Rem=0.0;
}

void Slurry::calcRem()
{
	Rem = 1.0 - (Ash + NDF + RP + RL);
};
Slurry::~Slurry() {
	
}

