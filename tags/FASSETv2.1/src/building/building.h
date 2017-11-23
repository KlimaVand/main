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

#include "genAsset.h"

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
      //!probably obsolete
      virtual void CalcLP();
      //!Generate indicator output
      /*!
       * \param indicatorType the type of indicating requested
       * */
		virtual void GiveIndicator(int indicatorType);

      //! Set area
      virtual void SetArea(int aArea) {Area=aArea;};
      //! Return area
      virtual double GetArea() {return Area;};
      //! output streaming
      friend ostream& operator <<(ostream& os,building* b);

};

#endif


