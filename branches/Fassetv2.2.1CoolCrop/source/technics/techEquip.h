/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
   Author   Lars Noboe Andersen & Poul Lassen
     Date
 Revision   Bj�rn Molt Petersen, May-June 1997
\****************************************************************************/

#ifndef __TECHEQP_H
   #define __TECHEQP_H

#include "../base/message.h"
#include "../economy/TechAsse.h"
#include "../base/base.h"

/****************************************************************************\
 Class: techEquip
\****************************************************************************/

class techEquip : public base, public TechAsset
{
   /* Attributes */
   protected:
      string techEquipName;
      double size;
      double carryCapacity;
      int age;
      double hoursPrimo;
      double hoursUltimo;

   /* Actions */
   private:
      techEquip(); // Dissable use of default constructor

   public:
      // Constructor with arguments
      techEquip(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
      // Copy Constructor
      techEquip(const techEquip& atechEquip);
      // Destructor
      virtual ~techEquip();
      // Set functions
      void SettechEquipName(string& atechEquipName) {techEquipName = atechEquipName;}
      void Setsize(double asize) {size = asize;}
      void SetCarryCapacity(double acarryCapacity) {carryCapacity = acarryCapacity;}
      void SethoursUltimo(double ahoursUltimo) {hoursUltimo = ahoursUltimo;}
      void AddhoursUltimo(double ahoursUltimo) {hoursUltimo += ahoursUltimo;}
      // Get functions
      string GettechEquipName(void) {return techEquipName;}
      double Getsize(void) {return size;}
      double GetCarryCapacity(void) {return carryCapacity;}
      double GethoursPrimo(void) {return hoursPrimo;}
      double GethoursUltimo(void) {return hoursUltimo;}
		virtual double getHoursUsed(); //implements pure virtual from techAsset
      // Other functions
      virtual void ReadParameters(fstream * file,char * sectionName=NULL);
//      void UpdateAge();
      void UpdateHours();
      virtual void GiveIndicators();
};

#endif
