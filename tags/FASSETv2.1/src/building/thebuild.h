/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
  Archive
   Author  Lars Kroll Kristensen Lok. 1875
     Date  12/6 97
 Revision
 ----------------------------------------------------------------------------
      Log
\****************************************************************************/

#ifndef THEBUILDINGS_H
  #define THEBUILDINGS_H

class Manure_manager_class;
#ifdef _STANDALONE
	#include "base.h"
#endif
#include "stable.h"
#include "pigstabl.h"
#include "catstabl.h"
#include "slurrystore.h"
#include "dungheap.h"
#include <b_stora2.h>
#include <animal.h>

/****************************************************************************\
 Class: buildings
 Should only be instantiated ONCE. MUST be initialized AFTER theProducts
 This class encapsulates a graph of buildings. (more or less)
\****************************************************************************/

class buildings :public base
{
   /* Attributes */
   private:
   linkList<stable> *theStables;
   linkList<B_storage2> *theStores;
   linkList<manurestore> *theManurestores;
   int dummy;
   protected:
    /* Actions */
   private:

   public:
      // Default Constructor
      buildings();
      // Constructor with arguments
      buildings(char *aName,const int aIndex,const base * aOwner);
      // Copy constructor
      buildings(const buildings& someBuildings);
      // Destructor
      ~buildings();
      // Initialization of all the buildings
      void InitializeAll(char * InputFilename, bool pigs, bool dairy, bool beef);
      // Daily actions should be called every day by manager
		stable* GetFreeStable(animalStock * aHerd);
		pigstable* GetFreeStable(swineStock * aHerd);
		animalSection * PlaceAnimal(const char * name);
      // GettheStables allows animalstock to place animals in the correct building
      linkList<stable> * GettheStables() {return theStables;};
      //add cowstable* GetFreeStable(cowStock* aHerd); etc. as needed
		//Extractproduct expects a pointer to a product with a name and an amount
      //The function then finds that product in the buildigs and returns how much
      //is extracted, and inserts this into the object pointed to by p
      void StoreProduct(product* p);
      product *  ExtractProduct(product* p);
      double AmountProductStored(product* p);                   // JB did this
      virtual void DailyUpdate();
//      void GiveIndicator(int indicatorType);
      virtual void CalcLP();
      void CalcLPmanurestore(char* filename);
      void CalcLPmanurestorecap(char* filename);
      void CalcLPstables(char* filename);
      // Set functions
      // Get functions
      // Operators
      friend ostream& operator <<(ostream& os,buildings* b);
		void EndBudget(bool show = false);
      double GetNInput();
      double GetNOutput();
		product * GetCopyStoredFeedProduct(product* p);
      void ScaleBuildings(); //scales buildings to animal numbers NJH June 2001
      double GetTotalManureNStored();
      double GetTotalSlurryStored();
      double GetTotalSlurryCap();
		void GetAvailableStoredFeed(product *aProduct);
		product * GetStoredFeed(int type);
      product * EmptyManureStores();

};

extern buildings *theBuildings;

#endif

