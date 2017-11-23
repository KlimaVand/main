/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
 \****************************************************************************/
#ifndef CROPMAIZE2_H
#define CROPMAIZE2_H

#include "crop.h"

class cropMaize2: public crop {
	double v_maxroot1, v_maxroot2, v_maxroot3;
	double v_minroot1, v_minroot2, v_minroot3;
	double v_maxtop1, v_maxtop2, v_maxtop3;
	double v_mintop1, v_mintop2, v_mintop3;
	double v_crittop1, v_crittop2, v_crittop3;
	double eps_a, eps_b;
	double glrlimit;
	double a_sr1, a_sr2, a_sr3;
	double k_t1, k_t2;
	double a_sk1, a_sk2;
	double r_pool, r_rate;
	double q1, q2;
	double z_1, z_2, z_3;
	double f_crop;
	double k_l;
	double beta;
	double Qxs, Qxc, a_ra;
	double n_k1;
	double n_k2;
	double r_nrate;
	double r_npool;
	double k_lm;
	double Slope_PAR;
	double GAIcrit;
	double T_1, T_2, T_3, T_4;
	double KAPPA, gL, Ws, Ns;
	double NitrogenVegTop;
	double NitrogenStorage;
	double NitrogenRoot, NitrogenTop, DryMatterTop, DryMatterTot;
	double DS;
	double NREPA; // amount of NTIL available for redistribution
	double eps;
	double gN;
	double fN;
	double DMREPA;
	double konor_linearlinear;
	double LAI_a1, LAI_b1, LAI_b2, LAI_c;

public:
	cropMaize2(const char * aName, const int aIndex, const base * aOwner);
	cropMaize2(const cropMaize2& acrop);
private:
	crop& operator=(const crop& c); // Dissable the compilers generation of default assignment operator. crop();
	virtual void UpdateNitrogenDistribution(double d);
protected:

	virtual double RLD(double z);
	virtual double RLD_antiderivative(double z);

	virtual double RootLengthInInterval(double startdepth, double thickness);
	virtual void CalcRootGrowth();
	virtual double DeltaDryMatter();

	virtual void TransferDryMatterToRoot(double * deltaDryMatt);
	virtual void TransferDryMatterToStorage(double * deltaDryMatt);
	virtual void CalcLeafAreaIndices();
	virtual double Nmax();
	virtual double Nmin();

	virtual double NmaxTop();
	virtual double NcritTop();
	virtual double NminTop();
	virtual double NmaxRoot();
	virtual double NminRoot();
	virtual nitrogen NitrogenInTop();

	virtual double NitrogenDemand();


public:

	virtual double Update(double ActivePar);
	virtual double GiveEvapFactor();

	virtual void Harvest(decomposable* Storage, decomposable* Straw);
	virtual void Terminate(decomposable* Straw, decomposable* DeadRoot,
			double *& RootLengthList);

	// N functions
	virtual nitrogen NitrogenInStorage();
	virtual nitrogen NitrogenInVegTop();
	virtual nitrogen NitrogenInRoot();

	virtual double LeafAreaFraction(double height) const;
	virtual double TotalLeafAreaIndex(double height, double thickness) const;
	virtual double GreenLeafAreaIndex(double height, double thickness);

	virtual double InterceptionCapacity();

	virtual double GiveRootInInterval(double startDep, double thick,
			double input);
	virtual double GiveExtCoeff() const;

	virtual double fTW(double temperature);
	virtual double funcN();

	virtual void UpdateHeight();

	  virtual bool ReadyForHarvest() {return Phenology->DS>1.0;};

	 virtual  double fNitrogen();
	 virtual double fNitrogenCurve();
	 void Sow(double SeedDensDryMatt, double aRelativeDensity,
	 		double NitrogenInSeed);

};

#endif

