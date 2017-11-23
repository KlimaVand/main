/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __TEOPFIHA_H
   #define __TEOPFIHA_H
#include "../products/plantItem.h"
#include "../products/products.h"
#include "fieldOperationTech.h"

class harvestTech: public fieldOperationTech
{
   /* Attributes */
   protected:
      double loadClampCapacity;
      double reloadingCapacity;
      double unloadCapacity;
      double unloadRoothouseCapacity;
      double depositCapacity;
      double depositRoothouseCapacity;
      double prepUnloadingStorage;
      double prepUnloadingRoothouse;
      double prepReloading;
      double prepReloadingHarvester;
      double prepClamp;
      double uncoverClamp;
      double roadSpeed;
      double balesLost;
      double balesLoad;
      double strawBalerCapacity;
      double bindTime;
      double loadCapacity;
		double reloading;
		double moweCarriage;
      int stops;
      int operationID;
      int baleType; // 0: small bales 1: round bales

      plantItem* MainProduct[MaxPlants];
      plantItem* StrawProduct[MaxPlants];
      plantItem* ForageProduct[MaxPlants];

   /* Actions */
   private:
      harvestTech();                                // Dissable use of default constructor
      harvestTech(const harvestTech& h);            // Dissable the compilers generation of default copy constructor.
   	harvestTech& operator=(const harvestTech& h); // Dissable the compilers generation of default assignment operator.

   protected:
      virtual void CalcGangHours();
      virtual void CalcGangFuel();

   public:
      // Constructor with arguments
      harvestTech(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
      // Destructor
      virtual ~harvestTech();
      void SetoperationID(int aoperationID) {operationID = aoperationID;}
      virtual void ReadParameters(fstream * file);
      virtual void SetMainProduct(int i,plantItem* aMain) {MainProduct[i] = aMain;};
      virtual void SetStrawProduct(int i,plantItem* aStraw) {StrawProduct[i] = aStraw;};
      virtual void SetForageProduct(int i,plantItem* aForage) {ForageProduct[i] = aForage;};

      virtual void UpdateProducts(bool contractor=false);
};

#endif


