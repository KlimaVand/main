/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
 Revision      16/5-97 Jørgen Berntsen added support for Methionin, Threonin, lysin ,..
 					13/6 support for plantname JB
               	july major revision of all product's.         (JB)
               	All operators calls the superclass
                  operator check's
                  	if similar types, storage etc  added/subtracted
                     if amount=0 the object gets the same unit, storage etc
                     	as the object that is added to it
\****************************************************************************/

#ifndef __PFEEDIT_H
  #define __PFEEDIT_H
#include "plantItem.h"

class feedItem  : public liveMatter
{
   /* Attributes */
   private:
   protected:
	//! Ca fraction of amount
      double Ca_content;
      //! Na fraction of amount
      double Na_content;
      //! Mg fraction of amount
      double Mg_content;
      //! Cu fraction of amount
      double Cu_content;
      //! Fat fraction of amount
      double fat;
      //! suger fraction of amount
      double sugar;
      //! starch fraction of amount
      double starch;
      double chew;
      //! Amino Acid
      double aat;
      double pbv;
      //! carbohydrate in the cell wall. Fraction of amount
      double cell;
      //! Satiety for grown up
      double fill;
      //! Satiety for youngsters
      double fillU;
      //! Lysin fraction of amount
      double Lysin;
      //! Threonin fraction of amount
      double Threonin;
      //! Threonin fraction of amount
      double Methionin;
      double P_digest;
      double MaxPercentage;  // the maximum percentage in feed
      //! Organic matter Digestibility
      double OMD;	//added by NJH 20.06.00
      double pigFeedUnitsPerItemUnit; // Unit: fe

      //! proportion of crude protein N that is rumen degradable (RDP)
      double propRDP;
      //! proportion of amino acid in protein that is not degraded in rumen (UDP)
      double propAAinUDP;
      //! proportion of amino acid in UDP that is digestible in small intestine
      double propUDPdigestible;
      //! g amino acid per kg digestible carbohydrate
      double microAAFactor;
      //! proportion digestible carbohydrate in fresh weight
      double CHO_content;
      //! proportion of carbohydrate that is digestible
      double propDigestCHO;
      double ME;
      //! NONE implies that the product is not produced
      string plantName;
      //! proportion of protein N that is digestible
      double proteinN_digestibility;
      //! Unit: fe
      double feedUnitsPerItemUnit;
      //! 1 = concentrate, 2 = roughage, 3 = mineral supplement
      int feedCategory;

      void InitVariables();

   public:
      // Default constructor
      feedItem();
      // Constructor with arguments
      feedItem(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
      // Copy Constructor
      feedItem(const feedItem& afeedItem);
      // Destructor
      virtual ~feedItem();


      // Set functions
  //    void SetfodderUnitsPerItemUnit(double afodderUnitsPerItemUnit) {fodderUnitsPerItemUnit = afodderUnitsPerItemUnit;}
      void SetCa_content(double aCa_content) {Ca_content = aCa_content;}
      void SetNa_content(double aNa_content) {Na_content = aNa_content;}
      void SetMg_content(double aMg_content) {Mg_content = aMg_content;}
      void SetCu_content(double aCu_content) {Cu_content = aCu_content;}
      void Setfat(double afat) {fat = afat;}
      void Setsugar(double asugar) {sugar = asugar;}
      void Setstarch(double astarch) {starch = astarch;}
      void Setchew(double achew) {chew = achew;}
      void Setaat(double aaat) {aat = aaat;}
      void Setpbv(double apbv) {pbv = apbv;}
      void Setcell(double acell) {cell = acell;}
      void Setfill(double afill) {fill = afill;}
      void SetfillU(double afillU) {fillU = afillU;}
      void SetMethionin(double aMethionin) {Methionin = aMethionin;};
      void SetThreonin(double aThreonin) {Threonin = aThreonin;}
      void SetLysin(double alysin) {Lysin = alysin;}
      void SetproteinN_digestibility(double aproteinN_digestibility) {proteinN_digestibility = aproteinN_digestibility;}


   void SetOMD(double aOMD) {OMD = aOMD;}


   void SetME(double anME) {ME = anME;}
   void SetfeedCategory(int aValue) {feedCategory=aValue;}

      // Get functions
   double GetOMD(void) const {return OMD;}
      double GetproteinN_digestibility(void) const {return proteinN_digestibility;}
      double GetfeedUnitsPerItemUnit(void) const {return feedUnitsPerItemUnit;}
      double GetCa_content(void) const {return Ca_content;}
      double GetNa_content(void) const {return Na_content;}
      double GetMg_content(void) const {return Mg_content;}
      double GetCu_content(void) const {return Cu_content;}
      double Getprotein_N(void) const {return orgN_content.n;}
      double Getfat(void) const {return fat;}
      double Getsugar(void) const {return sugar;}
      double Getstarch(void) const {return starch;}
      double Getchew(void) const {return chew;}
      double Getaat(void) const {return aat;}
      double Getpbv(void) const {return pbv;}
      double Getcell(void) const {return cell;}
      double Getfill(void) const {return fill;}
      double GetfillU(void) const {return fillU;}
      double GetMethionin(void) const {return Methionin;};
      double GetThreonin(void) const {return Threonin;}
      double GetLysin(void) const {return Lysin;}
      string GetPlantName(void) const {return plantName;}
      double GetP_digest(void) const {return P_digest;}
      double GetMaxPercentage(void) const {return MaxPercentage;};
//      double GetfodderUnitsPerItemUnit_pregnant(void) const {return fodderUnitsPerItemUnit_pregnant;}
  //    double GetfodderUnitsPerItemUnit_nonpregnant(void) const {return fodderUnitsPerItemUnit_nonpregnant;}
      double GetpigFeedUnitsPerItemUnit(void) const {return pigFeedUnitsPerItemUnit;}
      double GetPropRDP() const {return propRDP;};
      double GetmicroAAFactor() const {return microAAFactor;};
      double GetpropAAinUDP() const {return propAAinUDP;};
      double GetpropUDPdigestible() const {return propUDPdigestible;};
      double GetCHOContent() const {return CHO_content;};
      double GetpropDigestCHO() const {return propDigestCHO;};
      double GetME() const {return ME;};
      virtual double GetProteinContent();
      virtual double GetdigestibleEnergy();


		// Other functions

      virtual product& operator=(const product& someFeedItem);
      virtual product& operator+(const product& someFeedItem);
      virtual product& operator-(const product& someFeedItem);
      product* clone() const;
//      virtual void ReadParameters(fstream * file);
		bool ReadParams(fstream * file);
		virtual void CalcDigestibility();
	 	void AddPlantItem(const plantItem& somePlantItem, int feedCat);
		virtual double CalcNFE();
		virtual double GetC();
		virtual double GetGrossEnergy();
		virtual void CalcFE();
		virtual void CalcME();
		virtual void CalcFill(int type);
		virtual void CalcAAT_PBV();
      virtual int GetfeedCategory() {return feedCategory;}
      virtual int GotItAll();
};

#endif

