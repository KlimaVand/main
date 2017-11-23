/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
//!Superclass for all buildings
/*!Superclass for all buildings.  It should not be instanced.
 * */
#ifndef BUILDING_H
  #define BUILDING_H

#include "../economy/genAsset.h"

/****************************************************************************\
 Class: building
\****************************************************************************/

class building: public base, public GenAsset
{
   /* Attributes */
   private:

   protected:
	   //! Usable groundarea in sqr(m), usable for whatever
      double   Area;
      //!Identifier to allow the building to be located by a GIS
      int location;

    /* Actions */
   private:

   public:
      //! Default Constructor
      building();
      //! Constructor with arguments
      building(const char *aName,const int Aindex,const base* aOwner);
      //! Copy constructor
      building(const building& aBuilding);
      //! Destructor
      ~building();
      //! initialize from file
      virtual void ReadParameters(fstream* file);
      //! Daily actions should be called every day by manager
      virtual void DailyUpdate();
      //!Generate indicator output
      /*!
       * \param indicatorType the type of indicating requested
       * */
		virtual void GiveIndicator(int indicatorType);

      //! Set area
      virtual void SetArea(int aArea) {Area=aArea;};
      //! Return area
      virtual double GetArea() {return Area;};

      //!Return the location identifier
      int Getlocation(){return location;}
      //!Set the location identifier
      /*!
       * \param aVal the value to which the location identifier should be set
       * */
      void Setlocation(int aVal){location = aVal;}
      void ReadDefaultParameters();
};

#endif


