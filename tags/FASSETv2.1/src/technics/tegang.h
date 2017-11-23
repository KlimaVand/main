/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
   Author   Lars Noboe Andersen & Poul Lassen
     Date
 Revision   Bjørn Molt Petersen, May-June 1997
\****************************************************************************/

#ifndef __TEGANG_H
   #define __TEGANG_H

#include <bstime.h>
#include <message.h>
#include "teteimpl.h"
#include "teteinst.h"
#include "tetetrac.h"
#include "teteself.h"
#include "tetemanure.h"
#include "tetecutter.h"
#include "techeqp.h"

/****************************************************************************\
 Class: gang
\****************************************************************************/

class gang: public base
{
   /* Attributes */
   private:
      int gangElectricity;

   protected:
      double gangHours; // Hours the gang is in action - limiting techEquipment decides
      double gangFuel; // Amount of fuel used by this gang during an operation
      double gangMaintenance; // Maintenance for this gang during an operation

      linkList<techEquip>* techEquipList;

   /* Actions */
   private:
      gang();                         // Dissable use of default constructor
      gang(const gang& g);            // Dissable the compilers generation of default copy constructor.
   	gang& operator=(const gang& g); // Dissable the compilers generation of default assignment operator.

   protected:
      void AppendTechEquipList(techEquip * aTechEquip);

   public:
      // Constructor with arguments
      gang(const char * aName, const int aIndex, const base * aOwner = NULL);
      // Destructor
      ~gang();
      void SetgangElectricity(int agangElectricity) {gangElectricity = agangElectricity;}
      void SetgangHours(double agangHours) {gangHours = agangHours;}
      void SetgangMaintenance(double agangMaintenance) {gangMaintenance = agangMaintenance;}
      void SetgangFuel(double agangFuel) {gangFuel = agangFuel;}
      double GetgangMaintenance(){return gangMaintenance;}
      int GetgangElectricity(){return gangElectricity;}
      double GetgangFuel(){return gangFuel;}
      double GetgangHours(){return gangHours;}
      int Initialize(linkList <string> * aStringList, linkList <techEquip> * aTechFarmList);
      techEquip * GetMainTechEquip();
      linkList <techEquip> * GetTechEquipList();
      void OutputGang(string aOperationName);
};

#endif

