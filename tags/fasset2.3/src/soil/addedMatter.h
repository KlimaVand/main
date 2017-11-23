/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
// =============================================================================
// Soil added organic matter aggregation class
// (c) Bj�rn Molt Petersen, Danmarks JordbrugsForskning 1998
// =============================================================================

#ifndef ADDMATT_H
   #define ADDMATT_H
#include "matter.h"
#include "addedPool.h"

class addedMatter : public matter
{
   protected:
      enum {maxSubPools=2000};

      int numOfSubPools;

      addedPool * addPoolList[maxSubPools];

      virtual void CreateAndTransfer(int PoolNo,double Fraction,double TurnoverFactor);

   public:
      addedMatter(const char * Pname,const int Index,const base * owner);
      addedMatter(const addedMatter& source);
      virtual ~addedMatter();
      virtual addedMatter* clone() const;
      virtual void Add(matter * source,double fraction);
      virtual void Initialize(double clayEff1,double clayEff2,int clayResp);
      virtual void Update(double environmentEffect,int mode);
      virtual void CatchInfluxAOM(cnMatter &content,int mode, bool hungry);


      virtual void CatchInflux(cnMatter &content,int mode){CatchInfluxAOM(content,mode,false);};

      virtual void TestAddProduct(organicProduct * prod,double * check,double * CN,int * num,bool forAOM);

      virtual double GetCNcrit(){return -1.0;}; // Should never be called for this instance
      virtual double PotentialNitrogenMineralisation();
      virtual double PotentialNitrogenImmobilisation();
      virtual double ResidualCarbon();

      virtual void FinalExport(double ratio);
      virtual cnMatter GetCarbon();
      virtual bool IsAOM()const {return true;};
      virtual double PotentialCarbonTurnover();
      virtual void ReleaseReduceTurnover();
      virtual bool CloneAndModify(double Fraction,double TurnoverFactor);
};

#endif
