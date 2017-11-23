/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
// =============================================================================
// Soil added organic matter pool
// (c) Bj�rn Molt Petersen, Danmarks JordbrugsForskning 1998-1999
// =============================================================================

#ifndef ADDPOOL_H
   #define ADDPOOL_H

#include <matter.h>

class addedPool : public matter
{
   protected:
      double ReduceTurnover;
      bool Hungry();

   public:
      addedPool(const char * Pname,const int Index,const base * owner);
      addedPool(const addedPool &source);
      virtual addedPool* clone() const;
      virtual void Decay(double environmentEffect,cnMatter &out,int mode,double cnc);
      virtual void SetClayEffect1(double c){clayEffect1=c;};
      virtual void SetClayEffect2(double c){clayEffect2=c;};
      virtual void SetClayResponseType(int c){clayResponseType=c;};
      virtual void SetUseClayEffect(bool u){useClayEffect=u;};
      virtual void SetCNcrit(double cnc){CNC=cnc;};
      virtual bool AddedMatter(){return true;};
      virtual void CatchInfluxIfHungry(cnMatter &content,int mode);
      virtual void CatchInfluxIfNotHungry(cnMatter &content,int mode);
      virtual void CatchInflux(cnMatter &content,int mode);
      virtual void FinalDecayIfHungryAOM(double ratio,cnMatter &out);
      virtual void FinalExportAOM(double ratio,cnMatter &out);
      bool IsAOM() const {return true;};
      virtual bool Alike(addedPool * aP);
      virtual void SoakDry(addedPool * aP);
      virtual void SetReduceTurnover(double Reduce);
      virtual void ReleaseReduceTurnover();
      virtual double GetReduceTurnover() {return ReduceTurnover;};
};

#endif

