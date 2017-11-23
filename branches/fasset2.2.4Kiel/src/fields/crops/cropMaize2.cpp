/*
 * [-1..0] sown, but not emerged:
 * [ 0..1] emerge - flowering:
 * [ 1..2] flowering - end of grain filling
 * [ 2..3] end of grain filling until ripe
 * [ 3.. ] Ripe
 *
 * if      (DS < 0) DS += max(SoilTemp - TB0, 0) / TS0
 * else if (DS < 1) DS += max(AirTemp  - TB1, 0) / TS1
 * else if (DS < 2) DS += max(AirTemp  - TB2, 0) / TS2
 * else if (DS < 3) DS += max(AirTemp  - TB3, 0) / TS3
 * else if (DS > 3) DS = 3;
 *
 */

#include <math.h>
#include "../../base/common.h"
#include "cropMaize2.h"
#include "crop.h"

#include "../../base/climate.h"
#include "../../products/products.h"
#include "../../tools/compare.h"
#include <iostream>
/**
 *
 * @param aName
 * @param aIndex
 * @param aOwner
 * @param cropName
 */
cropMaize2::cropMaize2(const char * aName, const int aIndex,
		const base * aOwner) :
		crop(aName, aIndex, aOwner) {

	CropName = "Maize";
	PlantItemName = "MAIZE";           // These names map to products.dat !
	StrawItemName = "MAIZESTRAW";      // These names map to products.dat !
	WinterSeed = false;
	C4Photosynthesis = true;

	commonData data;
	if (data.readFileWithoutExit("CropParameters.dat")) {
		int sectionName = data.FindSection(CropName);
		//int sectionCrop = data.FindSection("Crop");
		//int maxes = max(sectionName, sectionCrop);
		int maxes = sectionName;

		data.FindItem("v_maxroot1", maxes, v_maxroot1);
		data.FindItem("v_maxroot2", maxes, v_maxroot2);
		data.FindItem("v_maxroot3", maxes, v_maxroot3);
		data.FindItem("v_minroot1", maxes, v_minroot1);
		data.FindItem("v_minroot2", maxes, v_minroot2);
		data.FindItem("v_minroot3", maxes, v_minroot3);
		data.FindItem("v_maxtop1", maxes, v_maxtop1);
		data.FindItem("v_maxtop2", maxes, v_maxtop2);
		data.FindItem("v_maxtop3", maxes, v_maxtop3);
		data.FindItem("v_mintop1", maxes, v_mintop1);
		data.FindItem("v_mintop2", maxes, v_mintop2);
		data.FindItem("v_mintop3", maxes, v_mintop3);
		data.FindItem("v_crittop1", maxes, v_crittop1);
		data.FindItem("v_crittop2", maxes, v_crittop2);
		data.FindItem("v_crittop3", maxes, v_crittop3);
		data.FindItem("a_sr1", maxes, a_sr1);
		data.FindItem("a_sr2", maxes, a_sr2);
		data.FindItem("a_sr3", maxes, a_sr3);
		data.FindItem("k_t1", maxes, k_t1);
		data.FindItem("k_t2", maxes, k_t2);
		data.FindItem("a_sk1", maxes, a_sk1);
		data.FindItem("a_sk2", maxes, a_sk2);
		data.FindItem("r_pool", maxes, r_pool);
		data.FindItem("r_rate", maxes, r_rate);
		data.FindItem("q1", maxes, q1);
		data.FindItem("q2", maxes, q2);
		data.FindItem("z_1", maxes, z_1);
		data.FindItem("z_2", maxes, z_2);
		data.FindItem("z_3", maxes, z_3);
		data.FindItem("f_crop", maxes, f_crop);
		data.FindItem("k_l", maxes, k_l);
		data.FindItem("k_lm", maxes, k_lm);
		data.FindItem("beta", maxes, beta);
		data.FindItem("Qxs", maxes, Qxs);
		data.FindItem("Qxc", maxes, Qxc);
		data.FindItem("a_ra", maxes, a_ra);
		data.FindItem("n_k1", maxes, n_k1);
		data.FindItem("n_k2", maxes, n_k2);
		data.FindItem("r_npool", maxes, r_npool);
		data.FindItem("r_nrate", maxes, r_nrate);
		data.FindItem("Slope_PAR", maxes, Slope_PAR);
		data.FindItem("GAIcrit", maxes, GAIcrit);
		data.FindItem("T_1", maxes, T_1);
		data.FindItem("T_2", maxes, T_2);
		data.FindItem("T_3", maxes, T_3);
		data.FindItem("T_4", maxes, T_4);
		data.FindItem("KAPPA", maxes, KAPPA);
		data.FindItem("gL", maxes, gL);
		data.FindItem("Ws", maxes, Ws);
		data.FindItem("Ns", maxes, Ns);
		data.FindItem("eps", maxes, eps);

		data.FindItem("konor_linearlinear", maxes, konor_linearlinear);
		data.FindItem("LAI_a1", maxes, LAI_a1);
		data.FindItem("LAI_b1", maxes, LAI_b1);
		data.FindItem("LAI_b2", maxes, LAI_b2);
		data.FindItem("LAI_c", maxes, LAI_c);

	}

// cout << LAI_a1 << " " << LAI_b1 << " " << LAI_b2 << " " << LAI_c << endl;

	DryMatterTot = 0.0;
	NitrogenRoot = 0.0;
	NitrogenTop = 0.0;
	DryMatterTop = 0.0;
	NitrogenVegTop = 0.0;
	NitrogenStorage = 0.0;
	fN = 0;
	gN = 0;

	DMREPA = 0; // max amount that can be re-allocated during ripening
	NREPA = 0; // amount of NTIL available for redistribution

}

/**
 *
 * @param SeedDensDryMatt
 * @param aRelativeDensity
 * @param NitrogenInSeed
 */
void cropMaize2::Sow(double SeedDensDryMatt, double aRelativeDensity,
		double NitrogenInSeed) {

	crop::Sow(SeedDensDryMatt, aRelativeDensity, NitrogenInSeed);
	Phenology->Sow();
}

/**
 * Returns daily DM growth
 */
double cropMaize2::Update(double ActivePar) {
	temp = theClimate->tmean;

	double DailyDMGrowth = 0.0; // daily dm growth
	DS = Phenology->DS;

	if (Phenology->Sown() && !Phenology->Ripe() && !terminated) {
		double soilTemp = aSoil->GetTemperature(200);
		double DayLength = theClimate->PhotoPeriod();
		Phenology->Update(temp, soilTemp, -1);
		TempSumRoot += max(0.0, temp);
		// the same termal time as linear leaf growth
		if (Phenology->TempSumForLeaf <= Phenology->LinearLeafPhase) {
			double DMTransfer = 2.0 * TopFraction * InitialSeedDM
					* (max(0.0, temp) / Phenology->LinearLeafPhase);
			DMTransfer = min(DMTransfer, SeedDM);
			DryMatterVegTop += 0.5 * DMTransfer;
			DryMatterTop = DryMatterVegTop;
			DryMatterRoot += 0.5 * DMTransfer;
			DryMatterTot = DryMatterRoot + DryMatterTop;
			SeedDM -= DMTransfer;
			CalcRootGrowth();
		}
	}

	if (DS >= 0 && !Phenology->Ripe() && !terminated) {
		//CalcLeafAreaIndices();

		double dDM = DeltaDryMatter();
		DMbudget.AddInput(dDM);

		DeltaDMTop = dDM;
		DailyDMGrowth = dDM;
		DryMatterTot += dDM;
		TransferDryMatterToRoot(&dDM); // dDM = dDM - DMroot
		TransferDryMatterToStorage(&dDM); // dDM = dDM - DMStorage

		DryMatterVegTop += dDM; // rest of dry matter
		DryMatterTop = DryMatterStorage + DryMatterVegTop;

		// update nitrogen
		/* A relative N concentration  relNcons  is calculated as
		 * The value of  relNcons  is used to find the current partitioning
		 * of total plant nitrogen to the aboveground organs and to the root
		 */
		double relNcons = (Nitrogen.n - Nmin()) / (Nmax() - Nmin());
		NitrogenRoot = NminRoot() + relNcons * (NmaxRoot() - NminRoot());
		double Ntop_act = NminTop() + relNcons * (NmaxTop() - NminTop());
		double dNtop = Ntop_act - NitrogenTop;
		// update NitrogenStorage and NitrogenStorage
		UpdateNitrogenDistribution(dNtop);

		NitrogenTop = NitrogenStorage + NitrogenVegTop;
		Nitrogen.n = NitrogenRoot + NitrogenTop;

		CalcRootGrowth();
		CalcLeafAreaIndices();
		UpdateHeight();

	}

	return DailyDMGrowth;
}

cropMaize2::cropMaize2(const cropMaize2& acrop) :
		crop(acrop) {
	NitrogenVegTop = acrop.NitrogenVegTop;
	NitrogenStorage = acrop.NitrogenStorage;
}

double cropMaize2::LeafAreaFraction(double height) const {
	double a = LAIDistributionPar;
	if (PlantHeight == 0)
		return 0;
	if (height >= PlantHeight)
		return 1.0;
	return (a + 1) * (a + 2) * (pow(height, a + 1)) / (pow(PlantHeight, a + 2))
			* (1 / (a + 1) - height / ((a + 2) * PlantHeight));
}

double cropMaize2::GiveEvapFactor() {
	// New method according to Joergen Olesen, et al. Clim. Res. (2000)
	return 1.0 + 0.02 * (GreenCropAreaIndex + YellowCropAreaIndex);
}

/*
 * Maximum ( NMAXTOP% ), critical (NCTOP) and minimum (NMINTOP%) values of
 * nitrogen concentration (% of dry weight) are decreasing functions
 * of the standing biomass DMTOP = DMTIL + DMSTOR. Below a certain level of
 * DMTOP the functions are truncated in order to avoid extrapolation11
 * to unreasonably high nitrogen levels
 *
 * v_maxtop1 and v_maxtop2: parameters regulating the decrease
 * in maximum nitrogen concentration as dry matter increases
 * u maxtop3: dry matter level below which maximum nitrogen concentration
 * is assumed to be constant.
 */

double cropMaize2::NmaxTop() {
	double NMaxTop_percent = v_maxtop1
			* pow(max(DryMatterTop / 100, v_maxtop3), -v_maxtop2);
	return NMaxTop_percent / 100 * DryMatterTop;
}

double cropMaize2::NcritTop() {
	double NCritTop_percent = v_crittop1
			* pow(max(DryMatterTop / 100, v_crittop3), -v_crittop2);
	return NCritTop_percent / 100 * DryMatterTop;
}

double cropMaize2::NminTop() {
	double NMinTop_percent = v_mintop1
			* pow(max(DryMatterTop / 100, v_mintop3), -v_mintop2);
	return NMinTop_percent / 100 * DryMatterTop;
}

/*
 *  Analogous maximum and minimum nitrogen
 *  concentrations (% of dry matter) are found for roots
 */
double cropMaize2::NmaxRoot() {
	double NMaxRoot_percent = v_maxroot1
			* pow(max(DryMatterRoot / 100, v_maxroot3), -v_maxroot2);
	return NMaxRoot_percent / 100 * DryMatterRoot;
}

double cropMaize2::NminRoot() {
	double NMinRoot_percent = v_minroot1
			* pow(max(DryMatterRoot / 100, v_minroot3), -v_minroot2);
	return NMinRoot_percent / 100 * DryMatterRoot;
}

double cropMaize2::Nmax() {
	return NmaxRoot() + NmaxTop();
}

double cropMaize2::Nmin() {
	return NminRoot() + NminTop();
}

/**
 * The nitrogen concentration in the plant dry matter cannot exceed
 * a maximum value. The amount of nitrogen needed to raise the concentration
 * from the present value to the maximum level is defined as the N demand
 * by the crop. Nitrogen concentrations below a critical value reduce
 * the dry matter production. The nitrogen concentration cannot fall below
 * a minimum value, at which dry matter production ceases completely.
 *
 * For each time step, a nitrogen demand  (g m-2) is calculated as
 */
double cropMaize2::NitrogenDemand() {
	return max(0.0, Nmax() - Nitrogen.n); // Bleken Konor Eq. (4.26)
}

/**
 *
 * calculation of the maximum daily rate of N uptake by roots given the present
 * soil conditions and root development, calculation of N demand depending
 * on the biomass of the crop and its nitrogen content, restriction of N uptake
 * to the amount that is actually available in the soil. At each time step,
 * ammonium and nitrogen concentrations in the soil simulated by SOIL_NO are
 * read and updated according to plant uptake.
 *
 * The root and the above ground parts of the plant are assumed to be equally
 * strong sinks, meaning that in the case of nitrogen deficiency, this is
 * assumed to be the same in the root and aerial parts, relative to their
 * maximum and minimum content as defined earlier.
 *
 * The amount of nitrogen above ground is further distributed between the
 * tiller and the grain (storage organs). This is accomplished in a similar
 * way to the allocation and redistribution of dry matter. Before a certain
 * development stage, usually heading, there is no direct distribution of
 * nitrogen to the grain. After a short transition period, all nitrogen
 * increment above ground dNTOP is allocated to the grain. In addition,
 * part of the nitrogen present in the tiller at heading is re-allocated to
 * the grain. As in the case of dry matter allocation, the fraction of nitrogen
 * transported to the grain is influenced by air temperature. Any nitrogen not
 * transported to the grain due to low temperature, neither directly nor
 * through re-allocation, remains in the NTILLER compartment.
 */
void cropMaize2::UpdateNitrogenDistribution(double dNtop) {
	// Bleken Konor p17ff Eq. (5.5) - Nitrogen distribution

	double f_nk = 0; // frac of nitrogen increment in dNTOP allocated to grain
	double K_LIM = 0; // temp factor limiting allocation of DM and N to grain

	if (temp < k_t1)
		K_LIM = 0;
	else if (temp < k_t2)
		K_LIM = (temp - k_t1) / (k_t2 - k_t1);
	else
		K_LIM = 1;

	if (DS < n_k1) // nk_1:  DS when nitrogen allocation to grain starts
		f_nk = 0;
	else if (DS < n_k2)
		// nk_2: DS when the whole nitrogen increment is allocated to grain
		f_nk = K_LIM * (DS - n_k1) / (n_k2 - n_k1);
	else
		f_nk = K_LIM;

	double R_n = 0; // amount of nitrogen translocated from tiller to grain

	// to the grain during ripening.
	if (DS <= 1)
		NREPA = r_npool * NitrogenVegTop;
	else if (DS <= 3) {
		NREPA = NREPA - r_nrate * ((DS - 1) * NREPA);
		R_n = K_LIM * r_nrate * (DS - 1) * NREPA;
	}

	// If temperature is above a threshold value, and thus LIM K = 1
	// during the whole grain filling period, NREPA0 is entirely moved from
	// the tiller to the grain compartment. Thus r_nrate influences the final
	// nitrogen allocation only when K_LIM < 1.

	double dNStor = f_nk * dNtop + R_n; // Bleken Konor Eq (5.4)
	double dNTil = (1 - f_nk) * dNtop - R_n; // Bleken Konor Eq (5.9)

	NitrogenStorage += dNStor;
	NitrogenVegTop += dNTil;

}

/**
 * nitrogen factor f(N)
 * f (N) < 1 when the nitrogen content (g N m-2 of field ares) of the canopy
 * is less than a defined critical value  NCTOP
 *
 * The value of  f(N)  is used to estimate  g(N) , the nitrogen factor
 * limiting dry matter production according to the nitrogen status of the plant.
 * The Matlab version of the model can also utilise a priori knowledge of f(N),
 * calculated from observed above ground dry mater and nitrogen concentration.
 * Usually observations are recorded only on a few occasions during the growing
 * period, and intermediate  values are estimated by linear interpolation.
 */
double cropMaize2::funcN() { // Bleken Konor Eq (5.10)
	double Ncrittop = NcritTop();

	if (NitrogenTop < Ncrittop)
		fN = (NitrogenTop - NminTop()) / (Ncrittop - NminTop());
	else
		fN = 1;

	return max(0.0, fN);
}

/**
 * modifying temperature function, taking values between 0 and 1  in
 * relation to four specific threshold temperatures T_1, T_2, T_3, T_4 in °C
 */
double cropMaize2::fTW(double temp) { // Diss Babette Chap 2 Eq 18 page 16
	double fT = 0;

	if (temp < T_1)
		fT = 0;
	else if (temp < T_2)
		fT = (temp - T_1) / (T_2 - T_1);
	else if (temp < T_3)
		fT = 1;
	else if (temp < T_4)
		fT = (T_4 - temp) / (T_4 - T_3);
	else
		fT = 0;

	return fT;
}

/**
 * extinction coefficient for photosynthetic active radiation
 */
double cropMaize2::GiveExtCoeff() const {
	// Diss Babette - SlopePAR is the slope of the negative linear part of
	// the equation which switches to a constant value (0PAR) at GAIcrit
	double v = 0;
	if (GreenCropAreaIndex <= GAIcrit)
		v = Slope_PAR * (GreenCropAreaIndex - GAIcrit) + k;
	else
		v = k;
	return v;
}

/**
 * The result of photosynthetic activity is estimated as daily net dry matter
 * increment. (g/m2)
 */
double cropMaize2::DeltaDryMatter() { // Bleken Konor Eq (2.1)
	//  daily incident photosynthetic active radiation, PAR (MJ m-2 )
	double R_p = 0.48 * theClimate->rad;
	//  fraction of PAR intercepted by the canopy according to Beer's law
	double f_par = 1 - exp(-GiveExtCoeff() * GreenCropAreaIndex);
	// gN in [0,1]: effect of nitrogen status on radiation use efficiency,
	// accounting for the fact that radiation use efficiency is more sensitive
	// to changes at the  the lower range of nitrogen content.
	gN = 1 - exp(-KAPPA * funcN());

	return R_p * f_par * eps * TranspirationRatio * gN * fTW(temp);
}

/**
 * transfer dry matter from  the Top to DryMatterRoot. The fraction diverted to
 * root decreases linearly with phenological development, while it is increased
 * by water stress and nitrogen limitation
 *
 */
void cropMaize2::TransferDryMatterToRoot(double *dDryMatt) {
	// Bleken Konor Eq (3.2)
	double f_r = 0; // fraction [0,1] of total DM increment diverted to the root
	if (0 < DS && DS < a_sr3) { // a_sr3: DS when partitioning to the root ceases
		double gN = 1 - exp(-KAPPA * funcN());
		// a_sr1:  maximum fraction of  dDMTOT to be partitioned to the root
		// a_sr2:  maximum reduction of f_r, when there is neither nitrogen
		// nor water limitation
		f_r = (a_sr1 - a_sr2 * TranspirationRatio * gN) * (a_sr3 - DS)
				/ (a_sr3 - 0);
	} else {
		f_r = 0.0;
	}

	double dDMRoot = *dDryMatt * f_r; // Bleken Konor Eq (3.2)

	DryMatterRoot += dDMRoot;
	*dDryMatt -= dDMRoot;
}

/**
 *  partition dry matter between VegTop (tiller) and storage (grain). After a
 *  given developmental stage, normally around heading, the fraction of dry
 *  matter allocated to grain  increases rapidly at the expense of tiller,
 *  until a later developmental stage; thereafter all dry matter is allocated
 *  to grain. In addition, a given fraction of tiller present at heading is
 *  reallocated to grain during grain filling and ripening. Low temperature
 *  during this phase reduces the rate of grain filling.
 *
 *  When air temperature (T) is above a certain threshold, the whole fraction
 *  of the dry matter increment of the above ground organs is allocated to
 *  DryMatterStorage. Otherwise the fraction allocated to the grain is reduced,
 *  and below a critical temperature no dry matter is distributed to
 *  DryMatterStorage
 */
void cropMaize2::TransferDryMatterToStorage(double * dDryMatt) {
	// Dry matter distribution Bleken Konor  p. 7ff
	double K_Lim = 0; // K_Lim: temperature factor  (dimensionless)

	if (temp < k_t1) // k_t1 and  k_t2  are threshold temperatures
		K_Lim = 0;
	else if (temp < k_t2)
		K_Lim = (temp - k_t1) / (k_t2 - k_t1);
	else
		K_Lim = 1;

	double f_k = 0; // fraction of top DM increment diverted to the grain

	if (DS <= a_sk1) // a_sk1: DS when grain filling begins
		f_k = 0;
	else if (DS <= a_sk2)
		// a_sk2: DS above which whole top DM increment reaches storage
		f_k = K_Lim * (DS - a_sk1) / (a_sk2 - a_sk1);
	else
		f_k = K_Lim;

	/* Part of DryMatterVegTop present at heading is available for
	 * reallocation to grain. The maximum total amount that can be re-allocated
	 * during ripening, DMREPA , is assumed to be a constant fraction  (pool r)
	 * of DMTIL at heading
	 */

	// max amount that can be re-allocated during ripening
	if (DS <= 1)
		DMREPA = r_pool * DryMatterVegTop;
	else
		DMREPA = DMREPA - r_rate * (DS - 1) * DMREPA;

	double R = 0;
	/* amount re-allocated on each one-day time step
	 * r_rate is a constant determining the rate of the reduction.
	 * When temperature is above the threshold value kt2 during the whole
	 * period from heading to yellow ripeness, the size of r_rate has no effect
	 * on the total amount reallocated to the grains, which is thus equal
	 * to DMREPA0.
	 */

	if (1 < DS && DS < 3)
		R = K_Lim * r_rate * (DS - 1) * DMREPA;
	else
		R = 0;

	DryMatterStorage += f_k * *dDryMatt + R; // Bleken Konor Eq. (3.4)
	*dDryMatt = (1 - f_k) * *dDryMatt - R; // Bleken Konor Eq. (3.5)
}

/**
 * The canopy area is described by a green area index (LAIG) and a yellow area
 * index (LAIY), which are the one-sided area of respectively green and yellow
 * above ground plant parts per unit ground area (m2 m-2 ). LAI = LAIG + LAIY
 *
 * The green  area index is computed as a double Michaelis-Menten function of
 * dry matter and nitrogen amount above ground before heading, when LAIG
 * attains its maximum value. Thereafter the reduction in green leaf area is a
 * function of the development index, according to a polynomial of the type
 * ( f(x) = L_m (1-x^klm) ), modified to give a faster decay when there is
 * shortage of water:
 */
void cropMaize2::CalcLeafAreaIndices() {
	// Green area index, Bleken Konor p. 20ff
	double dDS = Phenology->DSIncrease;
	double L_m = 0; // maximum green area index - Bleken Konor Eq (7.3)
	double LAIG = 0; // Bleken Konor (7.2)

	if (DS < 1)
		maxGLAI = max(maxGLAI, GreenCropAreaIndex);
	L_m = maxGLAI; // maximum green area index

	if (!Phenology->FloweringPassed) {
		if (konor_linearlinear == 1.0) {
			// LAI according to Konor (Bleken)
			// q1, q2: by non-linear regression of observed data
			LAIG = DryMatterTop / (q1 + q2 * DryMatterTop);
		} else if (konor_linearlinear == 2.0) {
			// LAI with linear-plateau function (or linear - linear function)

			if (LAIG < LAI_c) {
				LAIG = LAI_a1 + LAI_b1 * DryMatterTop;
			} else {
				LAIG = LAI_a1 + LAI_b1 * LAI_c + LAI_b2 * DryMatterTop;
			}
		}

	} else {
		// k_lm - in {0,1}, rate of green area decrease
		// k_l  - regulating the relative influence of water stress on green
		// area decline
		double l = GreenCropAreaIndex
				- k_lm * L_m * dDS * (DS - 1)
						/ ((1 - k_l) + k_l * TranspirationRatio);
		LAIG = max(0.0, l);
	}

	/*
	 * The increments in yellow area index is proportional to the decrease in
	 * green area index. Since LAIG is not allowed to decrease before heading,
	 * the model neglects yellow leaves before this stage. The yellow area
	 * index is calculated as
	 */

	double dLAIG = LAIG - GreenCropAreaIndex;
	GreenCropAreaIndex = LAIG;

	if (dLAIG < 0)
		YellowCropAreaIndex = YellowCropAreaIndex + beta * dLAIG;
	/*
	 * where beta is a dimensionless coefficient regulating the increase in
	 * yellow leaves relative to the decline in green ones, and  dLAIG is the
	 * change in green area index at the given time step.
	 * The Matlab version of the model can also utilise observed data of green
	 * leaf area index. This allows for example to test the effect of nitrogen
	 * on radiation use efficiency, without confounding it with the nitrogen
	 * effect on green leaf area. Usually observations are recorded only on a
	 * few occasions during the growing period, and intermediate values are
	 * estimated by linear interpolation.
	 */

}

/**
 * Root distribution is described by root density, the total length of roots
 * in a soil layer under a unit soil area (m×m-1 m-2 ).  The root density in
 * each layer depends on a root density distribution  function d (z) describing
 * the continuous variation of root density with depth z under the soil surface.
 * The root density distribution function d (z) is assumed to decrease linearly
 * from the surface down to a certain depth level  z3  (m), whereafter d (z)
 * declines exponentially with depth (Snipen and Bleken, unpublished):
 *
 * z1, z2 are constants describing the decrease of root density with depth.
 * The value RLD_0, denoting RLD(z) at the soil surface, must be calculated
 * for each new value of the root length l. This is due to the fact that the
 * equation must be satisfied for every time step. This integral condition
 * together with the definition of the function d (z)  yields
 *
 */
double cropMaize2::RLD(double z) { // Bleken Konor p.19 Eq. (6.4)
	double RLD_0 = RootDensAtSurf;
	double v;
	if ((0 <= z) && (z <= z_3))
		v = RLD_0 * (1 - z_1 / z_3 * z);
	else
		v = RLD_0 * (1 - z_1) * exp(-z_2 * (z - z_3));
	return v;
}

double cropMaize2::RLD_antiderivative(double z) { // Bleken Konor p.19ff
	double RLD_0 = RootDensAtSurf;

	double v;
	if ((0 <= z) && (z < z_3))
		v = ((z - (z * z * z_1) / (2 * z_3)) * RLD_0);
	else
		v = ((z_3 - (z_3 * z_3 * z_1) / (2 * z_3)) * RLD_0)
				+ ((-1 * (1 - z_1) * exp(-z_2 * (z - z_3)) * RLD_0 / z_2)
						+ ((z_3 - (z_3 * z_3 * z_1) / (2 * z_3)) * RLD_0));

	return v;
}

/**
 *  the root development is described by dry matter, depth, total length, and
 *  distribution in the soil profile.
 */
void cropMaize2::CalcRootGrowth() { // Root expansion p. 18ff

	double TSUMA = TempSumRoot; // sum temp above 0°C since sowing
	// Qxs: max root depth in the given soil (m)
	// Qxc: max root depth of present crop (m)
	// a_ra: temperature coefficient for root growth (m °C-1 day)

	if (0 < DS && DS < a_sr3) {
		// Root depth is the distance from the soil surface to the deepest
		// part of the root (m). Root depth for day i is estimated as a simple
		// function of air temperature sum from sowing (Haugen, unpublished)
		RootDepth = min(Qxs, min(Qxc, a_ra * TSUMA)); // Bleken Konor (6.1)
	}

	// The total root length l per unit soil area is assumed to be proportional
	// to the root dry matter (DryMatterRoot)
	// total root length per unit area of soil surface (m m-2 )
	// specific root length (root length per unit of root dry matter (m g-1 ))
	TotalRootLength = DryMatterRoot * SpecificRootLength; // Bleken Konor (6.3)

	double Q = RootDepth;
	double RLD_0; // Bleken Konor (6.6)

	// This integral condition together with the definition
	// of the function RLD(z) yields
	if (Q <= z_3)
		RLD_0 = TotalRootLength / (Q - 0.5 * z_1 / z_3 * Q * Q);
	else
		RLD_0 = TotalRootLength
				/ (z_3 - 0.5 * z_1 * z_3
						+ (1 - z_1) / z_2 * (1 - exp(-z_2 * (Q - z_3))));

	RootDensAtSurf = RLD_0;
	CalcRootDistribution();
}

/*
 * With the root density distribution function d (z) given in this way,
 * the root density for soil layer no. j is calculated as the mean value of d(z)
 * in the layer:
 */
double cropMaize2::RootLengthInInterval(double startdepth, double thickness) {
	// Root distribution  Bleken, Konor p. 19 Eq (6.5)
	startdepth = min(RootDepth, startdepth);
	double enddepth = min(RootDepth, startdepth + thickness);

	double s = RLD_antiderivative(startdepth);
	double e = RLD_antiderivative(enddepth);
	return max(0.0, e - s); // CK
}

/**
 * Input is either DryMatterRoot or NitrogenInRoot
 */
double cropMaize2::GiveRootInInterval(double startDep, double thick,
		double input) {

	return TotalRootLength	/ (RootLengthInInterval(startDep, thick) * input);

}

/**
 * harvest the current crop.
 * The drymatter and straw is stored in the input parameters.
 * Nitroget is updated. The plant is still alive
 *
 * In Konor: The date of harvesting is estimated as the first day after
 * yellow ripeness when the moisture of the grain is equal or lower than
 * a maximum accepted value. At yellow ripeness the grain has a water
 * content characteristic of this plant stage. Thereafter daily changes
 * in water content of the grain are driven by potential evapotranspiration
 * and precipitation.
 *
 */
void cropMaize2::Harvest(decomposable* Storage, decomposable* Straw) {

	CutOrHarvested = true;
	Storage->Setname(PlantItemName);
	if (PlantItemName != "") {
		theProducts->GiveProductInformation(Storage);
		//converting from drymatter to fresh weight
		Storage->Setamount(DryMatterStorage / Storage->GetdryMatter()); //??
	}

	Straw->Setname(StrawItemName);
	theProducts->GiveProductInformation(Straw);
	//converting from drymatter to fresh weight
	Straw->Setamount(DryMatterVegTop / Straw->GetdryMatter()); //??

			// Nitrogen
	nitrogen RootN, StorageN, StrawN;

	//check amount of nitrogen at harvest
	double minimumN = Nmin();
	double maximumN = Nmax();

	if (minimumN > Nitrogen.n * 1.25) {
		theMessage->WarningWithDisplay(
				"crop::Harvest - not enough nitrogen in plant at harvest");
	}
	if (maximumN < Nitrogen.n * 0.75) {
		theMessage->WarningWithDisplay(
				"crop::Harvest - too much nitrogen in plant at harvest");
	}

	double TotalRootN = NitrogenRoot;

	double TotStorageN = NitrogenStorage;
	double N15Ratio = Nitrogen.Get15NRatio();
	double TotalStrawN = NitrogenVegTop;
	double Nrest = Nitrogen.n - TotalRootN - TotStorageN - TotalStrawN;

	/* rest of N (negative or positive) is allocated proportionally to the root,
	 * straw and storage
	 */
	if ((Nrest < -0.01) || (Nrest > 0.01)) {
		//if Nrest positive: there is some unallocated N in the plant
		theMessage->WarningWithDisplay(
				"crop::Harvest - Nitrogen content adjusted");
		TotalRootN = TotalRootN + Nrest * TotalRootN / Nitrogen.n;
		TotStorageN = TotStorageN + Nrest * TotStorageN / Nitrogen.n;
		TotalStrawN = TotalStrawN + Nrest * TotalStrawN / Nitrogen.n;
	}

	RootN.SetBoth(TotalRootN, N15Ratio * TotalRootN);
	StorageN.SetBoth(TotStorageN, N15Ratio * TotStorageN);
	StrawN = Nitrogen - RootN - StorageN;
	if (Storage->GetAmount() > 0)
		Storage->SetorgN_content(StorageN / Storage->GetAmount());
	if (Straw->GetAmount() > 0) {
		Straw->SetNO3_content(
				StrawN / Straw->GetAmount() * (0.5 * FractionNminInVegTop));
		Straw->SetNH4_content(
				StrawN / Straw->GetAmount() * (0.5 * FractionNminInVegTop));
		Straw->SetorgN_content(
				StrawN / Straw->GetAmount() * (1.0 - FractionNminInVegTop));
	}

	Nitrogen = Nitrogen - StorageN - StrawN;

	if (Nitrogen.n <= 0) {
		theMessage->FatalError(
				"cropMaize2::Harvest - content of nitrogen in roots is negative");
	}
	if (!Phenology->Ripe())
		theMessage->Warning("cropMaize2::Harvest - crop not ready for harvest");
	if (DryMatterVegTop < 0 || DryMatterStorage < 0 || StorageN.n < 0
			|| StrawN.n < 0 || Nitrogen.n < 0) {
		theMessage->WarningWithDisplay(
				"cropMaize2:: Harvest values should not be negative");
		StorageN.n = max(0.0, StorageN.n);
		StrawN.n = max(0.0, StrawN.n);
	}

	Nbudget.AddOutput(StorageN.n + StrawN.n); // Budgets
	N15budget.AddOutput(StorageN.n15 + StrawN.n15); // Budgets
	DMbudget.AddOutput(DryMatterVegTop + DryMatterStorage);

	DryMatterVegTop = 0.0;
	DryMatterStorage = 0.0;
	GreenCropAreaIndex = 0.0;
	YellowCropAreaIndex = 0.0;
	PlantHeight = 0.1; // Modify to real cutting height!!!!!
	topMatter = NULL;

	NitrogenTop = 0;
	NitrogenVegTop = 0;
	NitrogenStorage = 0;
	NitrateUptakeRate = 0;
	NitrogenRoot = 0;

	DryMatterStorage = 0;
	DryMatterTop = 0;
	DryMatterTotalStandVegTop = 0;
	DryMatterTot = 0;
	DryMatterVegTop = 0;
	AccumulatedNProduction = 0;
	fN = 0;
	gN = 0;
	double NRemain, DMRemain;
	EndBudget(&NRemain, &DMRemain);

}

/**
 * Kills the plant. The length of the root, the amount of root and amount of
 * straw is returned
 * @param Straw
 * @param DeadRoot
 * @param RootLengthList
 */
void cropMaize2::Terminate(decomposable* Straw, decomposable* DeadRoot,
		double *& RootLengthList) {
	//crop::Terminate(Straw, DeadRoot, RootLengthList);

	if (!Phenology->Ripe() && !GrowthContinue && !terminated)
		// warning if a non-permanent crop is not ripe
		theMessage->Warning(
				"crop::Terminate - should not terminate crop at this phase");
	if (!Phenology->Emerged())
		theMessage->WarningWithDisplay(
				"crop::Terminate - attempt to terminate a crop that has not even emerged!!!");

	if (DryMatterVegTop + DryMatterStorage > 0.0) {
		Straw->Setname(StrawItemName);
		theProducts->GiveProductInformation(Straw);
		Straw->Setamount(
				(DryMatterVegTop + DryMatterStorage) / Straw->GetdryMatter());

		nitrogen StrawN = Nitrogen - NitrogenInRoot();
		if (Straw->GetAmount() > 0) {
			Straw->SetNO3_content(
					StrawN / Straw->GetAmount() * (0.5 * FractionNminInVegTop));
			Straw->SetNH4_content(
					StrawN / Straw->GetAmount() * (0.5 * FractionNminInVegTop));
			Straw->SetorgN_content(
					StrawN / Straw->GetAmount() * (1.0 - FractionNminInVegTop));
		}
		Nitrogen = Nitrogen - StrawN;
	}
	RootLengthList = GiveRootLengthList();

	DeadRoot->Setname("ROOTMATTER");
	theProducts->GiveProductInformation(DeadRoot);

	DeadRoot->Setamount(DryMatterRoot / DeadRoot->GetdryMatter());

	if (DryMatterRoot > 0) {

		DeadRoot->SetNH4_content(
				Nitrogen / DeadRoot->GetAmount() * 0.5 * FractionNminInRoots);
		DeadRoot->SetNO3_content(
				Nitrogen / DeadRoot->GetAmount() * 0.5 * FractionNminInRoots);
		DeadRoot->SetorgN_content(
				Nitrogen / DeadRoot->GetAmount() * (1.0 - FractionNminInRoots));

	}

	Nbudget.AddOutput(Nitrogen.n); // Budgets
	N15budget.AddOutput(Nitrogen.n15);
	DMbudget.AddOutput(DryMatterRoot);

	Nitrogen.Clear();
	RootDensAtSurf = 0.0;
	TotalRootLength = 0.0;
	RootDepth = 0.0;
	DryMatterRoot = 0.0;
	PlantHeight = 0.0;

	NitrogenTop = 0;
	NitrogenVegTop = 0;
	NitrogenStorage = 0;
	NitrateUptakeRate = 0;
	NitrogenRoot = 0;

	DryMatterRoot = 0;
	DryMatterStorage = 0;
	DryMatterTop = 0;
	DryMatterTotalStandVegTop = 0;
	DryMatterTot = 0.0;
	DryMatterVegTop = 0;
	fN = 0;
	gN = 0;

	terminated = true;
//	for (int i = 0; i < MaxSoilLayers; i++) {
//		if (RootLengthList[i] < 0)
//			cout << RootLengthList[i] << endl;
//	}
}

double cropMaize2::InterceptionCapacity() {
	return InterceptCoeff * (GreenCropAreaIndex + YellowCropAreaIndex);
}

double cropMaize2::GreenLeafAreaIndex(double height, double thickness) {
	// assumes that all yellow area are situated at the bottom of the canopy
	// below the green area
	double UpperArea = (GreenCropAreaIndex + YellowCropAreaIndex)
			* LeafAreaFraction(height + thickness) - YellowCropAreaIndex;
	double LowerArea = (GreenCropAreaIndex + YellowCropAreaIndex)
			* LeafAreaFraction(height) - YellowCropAreaIndex;
	return max(0.00001, UpperArea - LowerArea);
}

nitrogen cropMaize2::NitrogenInRoot() {
	nitrogen retVal;
	retVal.SetBoth(NitrogenRoot, Nitrogen.Get15NRatio() * NitrogenRoot);
	return retVal;
}

nitrogen cropMaize2::NitrogenInStorage() {
	nitrogen n;
	n.SetBoth(NitrogenStorage, Nitrogen.Get15NRatio() * NitrogenStorage);
	return n;
}

nitrogen cropMaize2::NitrogenInVegTop() {
	// return Nitrogen - NitrogenInRoot() - NitrogenInStorage();
	nitrogen n;
	n.SetBoth(NitrogenVegTop, Nitrogen.Get15NRatio() * NitrogenVegTop);
	return n;
}

nitrogen cropMaize2::NitrogenInTop() {
	//return Nitrogen - NitrogenInRoot();
	nitrogen n;
	n.SetBoth(NitrogenTop, Nitrogen.Get15NRatio() * NitrogenTop);
	return n;
}

void cropMaize2::UpdateHeight() {
	PlantHeight = f_crop * maxGLAI;
}

/**
 * Gives the total leaf area index in a certain layer of the canopy
 */
double cropMaize2::TotalLeafAreaIndex(double height, double thickness) const {
	return (GreenCropAreaIndex + YellowCropAreaIndex)
			* (LeafAreaFraction(height + thickness) - LeafAreaFraction(height));
}

double cropMaize2::fNitrogen() {
	return fN;
}

double cropMaize2::fNitrogenCurve() {
	return gN;
}

