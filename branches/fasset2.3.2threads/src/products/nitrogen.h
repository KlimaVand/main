// =============================================================================
// Nitrogen class, including N-15
// (c) Bjørn Molt Petersen, Danmarks JordbrugsForskning 2001
// =============================================================================

#ifndef NITROGEN_H
   #define NITROGEN_H



class nitrogen
{
   public:
      double
         n,
         n15;

      nitrogen();
      nitrogen(double d);
      nitrogen(const nitrogen& nit);
      ~nitrogen(){};
      void Add(nitrogen* addnit, double fraction);
      double Get15NAtomExcess();
      double Get15NRatio();
      void SetBoth(double n1,double n1_15);
      void Clear() {n=0.0; n15=0.0;};
      nitrogen& operator=(const nitrogen& nit);
      nitrogen& operator=(const double& d);
      nitrogen operator+(const nitrogen& nit);
      nitrogen operator-(const nitrogen& nit);
      nitrogen operator*(const double& factor);
      nitrogen operator/(const double& divisor);
      bool operator==(const nitrogen& nit);
      bool operator>(const nitrogen& nit);
      bool operator!=(const nitrogen& nit);
      double operator/(const nitrogen& nit);
};

#endif

