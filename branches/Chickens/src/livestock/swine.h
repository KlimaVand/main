/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
Class "Swine"
Class for the Swine object. There is only one object for all swines.
(c) Jørgen Berntsen, Danmarks JordbrugsForskning.
First version: 13/5-97
Revised: 11/6
Modified 4/11/99 by NJH
Describes a sow + suckling piglets
\****************************************************************************/

#ifndef SWINE_H
  #define SWINE_H

#include "pig.h"

/****************************************************************************\
 Class: swine
\****************************************************************************/

class swine : public pig
{
   /* Attributes */
   private:
    double pigletsPerLitter;
    double FEperPiglet;
    double maxParity;		//number of litters produced per sow
   /* Actions */
   private:

   public:
      // Default Constructor
      swine();
      swine(const char *aName,const int aIndex,const base * aOwner);
      // Destructor
      ~swine();

		// Other functions

      void DailyUpdate();
      void ReceivePlan(char * FileName);
      double GetpigletsPerLitter() {return pigletsPerLitter;};


		virtual void ReadParameters(fstream * file);

		double GetSensibleHeatProduction(double weight, double n);
};



#endif


