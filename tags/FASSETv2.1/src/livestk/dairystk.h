/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
 NJH
\****************************************************************************/

#ifndef DAIRYSTK_H
  #define DAIRYSTK_H

#include <animstk.h>
#include <cattle.h>
#include <panimal.h>
#include <cattlegrp.h>
#include <thebuild.h>

/****************************************************************************\
\****************************************************************************/

class dairyStock  : public animalStock
{
   /* Attributes */
   protected:
      linkList <cattleGroup> * grouplist;
		linkList <cattle> * pointyCalves;  //holds pointers to the calves produced today
      linkList<cattle> * oldies;				//holds pointers to the animals that have out-aged their age group
      bool wasFemale;
      bool individual;
		int aggregateNumber;   //
      int animalModel;//
      int aggregation;
      int IDnumber;     // to identify individuals
		fstream * CattlefileStream;
   /* Actions */
   private:
		dairyStock& operator=(const dairyStock& a); // Dissable the compilers generation of default assignment operator.
		dairyStock(const dairyStock& a);            // Dissable the compilers generation of default copy constructor.
      // Constructor
      dairyStock(){};

   public:
      // Constructor with arguments
      dairyStock(const char *aName,const int aIndex,const base * aOwner);
      // Destructor
     virtual  ~dairyStock();

      // Set functions
		void Setindividual(bool isIndividual) {individual = isIndividual;};
      // Get functions
		linkList <animal> * GettheStock() {return theStock;};
		bool Getindividual() {return individual;};
      double GetDE();
		void ReceivePlan(char* fileExtension);
      void DailyUpdate();
		void InitCattle(fstream * file, int anAnimalModel, int anAggregation, char *filenameCattle);
		virtual void checkBalances(bool show = false);
      double GetAnimalN() {return Nbudget.GetInput()-Nbudget.GetOutput();}
		void GetData(char * FileName);
      linkList <cattle> * GetpointyCalves() const {return pointyCalves;};
      linkList <cattleGroup> * GetCattleGroup() const {return grouplist;};
      void CheckBirth();
		void CheckAge();
		void SeedGroups();
      void Cull(linkList<cattle> * victims);
		double GetTotalAnimals();
		void AutoRecruit();
		void GiveIndicator(int indicatorType);
		void CheckWean(bool stopLactation);
      double GetHerdN();
      double GetHerdC();
		void theSorting(bool canCull);
};
#endif


