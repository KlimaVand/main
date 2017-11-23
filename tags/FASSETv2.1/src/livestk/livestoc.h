/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
  Archive
   Author     J�rgen Berntsen
     Date     14/5-97
 Revision
 ----------------------------------------------------------------------------
      Log
\****************************************************************************/

#ifndef LIVESTOC_H
  #define LIVESTOC_H

#include <animstk.h>
#include <croprot.h>
#include <thebuild.h>

/****************************************************************************\
 Class: livestock
 This class contains a linklist of animalstocks. The class should only be instantiated
 once, and all common actions (daily DailyUpdate, gams comminucation, initializaton)
 on the livestock is call through this class.
\****************************************************************************/

class livestock : public base
{
	/* Attributes */
	private:

	   linkList <animalStock> *theStockList;    // Linked list of animals stocks

   public:

   /* Actions */
   private:
      static void UpdateAnimals(animalStock * aAnimalStock) {aAnimalStock->DailyUpdate();};
      static void CalcLP1(animalStock * aAnimalStock) {aAnimalStock->CalcLP1();};
      static void CalcLP3(animalStock * aAnimalStock) {aAnimalStock->CalcLP3();};

      // Gams function
		void writeFeedComponentsList(char* filename);
      void writeLivestockPigs(char* filename);
      void writeNotProduceableItems(char* filename);
      void writeEmptyFile(char* filename);
      void writeVariableCosts(char* filename);
      void writeReplacementRate(char* filename);

   public:
      // Default Constructor
      livestock();
      // Destructor
      ~livestock();
		void Initialize(char* filename, bool pigfarm, bool dairyfarm, bool beeffarm, buildings * theBuildings, char *filenameCattle);
      void ReceivePlan(char* fileExtension);
      void CalcLP(int period);
      void DailyUpdate();
      double GetDE();
      linkList <animalStock> * getanimalStock() {return theStockList;}
		void checkBalances(bool show = false);
		void GiveIndicator(int indicatorType);
      void FlushIndicator(indicatorTypes indicatorType);
};

#endif


