
#include "nixcbits.h"
#include "common.h"
#include "constants.h"
#include "climate.h"
#include "message.h"
#include <math.h>
using namespace std;

// #pragma package(smart_init)
const double PI = 3.1415926;

double const GetgravitationalAcceleration() {
    return 9.81;
}

double const GetdynamicViscosityAir() {
    return 1.72 * 1E-5;
}

double const GetGasConstant() {
    return 8.31;    // K-1 mol-1
}

double const GetCconcInOrgmatter() {
    return 0.52;
}

double GetFrictionVelocity(double height,
                           double displacementHt,
                           double zeroPlane,
                           double windVelocity) {
    return (windVelocity * VonKarman) / (log((height - displacementHt) / zeroPlane));
}

template<typename T>
int partition(temp_double_array a,
              T                 b,
              int               left,
              int               right) {

    // { nested function inherits definitions of }
    // { "left" and "right" from enclosing procedure }
    double v;    // { pivot }
    int    l, r;

    v = a[right];    // { select pivot }
    l = left;
    r = right - 1;

    do {
        if ((a[l] < v)) {
            do {
                l = l + 1;
            } while (a[l] < v);
        }

        if ((a[r] >= v) && (l < r)) {
            do {
                r = r - 1;
            } while ((a[r] >= v) && (l < r));
        }

        swap(a[l], a[r]);
        swap(b[l], b[r]);
    } while (l < r);

    swap(a[l], a[r]);    // { undo that final swap }
    swap(a[l], a[right]);
    swap(b[l], b[r]);    // { undo that final swap }
    swap(b[l], b[right]);

    return l;
}

void quicksort2(temp_double_array a,
                temp_int_array    b,
                int               left,
                int               right) {
    int p;

    if (left < right) {
        p = partition(a, b, left, right);

        quicksort2(a, b, left, p - 1);
        quicksort2(a, b, p + 1, right);
    }
}

void quicksort2(temp_double_array a,
                temp_ptr_array    b,
                int               left,
                int               right) {
    int p;

    if (left < right) {
        p = partition(a, b, left, right);

        quicksort2(a, b, left, p - 1);
        quicksort2(a, b, p + 1, right);
    }
}

/*
 *
 * Given arrays x[n] and y[n] containing ordered function tabulations, this
 * rutine returns an array y2[n] that contains the second derivatives of the
 * interpolating functions at the tabulated points.
 *
 */
void Spline(double * x,
            double * y,
            int      n,
            double * y2) {
    int    i, k;
    double p, sig;
    double u[100];

    y2[0] = 0.0;
    u[0]  = 0.0;

    for (i = 1; i < n - 1; i++) {
        sig   = (x[i] - x[i - 1]) / (x[i + 1] - x[i - 1]);
        p     = sig * y2[i - 1] + 2.0;
        y2[i] = (sig - 1.0) / p;
        u[i]  = (y[i + 1] - y[i]) / (x[i + 1] - x[i]) - (y[i] - y[i - 1]) / (x[i] - x[i - 1]);
        u[i]  = (6.0 * u[i] / (x[i + 1] - x[i - 1]) - sig * u[i - 1]) / p;
    }

    y2[n - 1] = 0;

    for (k = n - 2; k >= 0; k--) {
        y2[k] = y2[k] * y2[k + 1] + u[k];
    }
}

/*
 * Given the arrays xa[n] and ya[n], containing ordered function tabulations,
 * and given the array y2a[n], which is the output of from the Spline() routine,
 * this routine returns a cubic-spline interpolated value.
 */
double Splint(double * xa,
              double * ya,
              double * y2a,
              int      n,
              double   x) {
    int    klo, khi, k;
    double h, b, a;

    klo = 0;
    khi = n - 1;

    while (khi - klo > 1) {
        k = (khi + klo) / 2;

        if (xa[k] > x) {
            khi = k;
        } else {
            klo = k;
        }
    }

    h = xa[khi] - xa[klo];
    a = (xa[khi] - x) / h;
    b = (x - xa[klo]) / h;

    return a * ya[klo] + b * ya[khi] + ((a * a * a - a) * y2a[klo] + (b * b * b - b) * y2a[khi]) * (h * h) / 6.0;
}

/*
 *
 * Linear Interpolation of ordered x data
 *
 */
double LinearInterpolation(double * x,
                           double * y,
                           int      n,
                           double   x0) {
    int i = 0;

    while ((i < n) && (x[i] < x0)) {
        i++;
    }

    if (i == n) {
        i--;
    }

    if (i == 0) {
        return y[0] + (y[1] - y[0]) / (x[1] - x[0]) * (x0 - x[0]);
    } else {
        return y[i - 1] + (y[i] - y[i - 1]) / (x[i] - x[i - 1]) * (x0 - x[i - 1]);
    }
}

/* Date last modified: 05-Jul-1997 */

/*
 *  CUBIC.C - Solve a cubic polynomial
 *  public domain by Ross Cottrell
 */
void SolveCubic(double   a,
                double   b,
                double   c,
                double   d,
                int *    solutions,
                double * x) {
    long double a1    = b / a,
                a2    = c / a,
                a3    = d / a;
    long double Q     = (a1 * a1 - 3.0 * a2) / 9.0;
    long double R     = (2.0 * a1 * a1 * a1 - 9.0 * a1 * a2 + 27.0 * a3) / 54.0;
    double      R2_Q3 = R * R - Q * Q * Q;
    double      theta;

    if (R2_Q3 <= 0) {
        *solutions = 3;
        theta      = acos(R / sqrt(Q * Q * Q));
        x[0]       = -2.0 * sqrt(Q) * cos(theta / 3.0) - a1 / 3.0;
        x[1]       = -2.0 * sqrt(Q) * cos((theta + 2.0 * PI) / 3.0) - a1 / 3.0;
        x[2]       = -2.0 * sqrt(Q) * cos((theta + 4.0 * PI) / 3.0) - a1 / 3.0;
    } else {
        *solutions = 1;

        double inputOne = sqrt(R2_Q3) + fabs(R);
        double inputTwo = 1 / 3.0;

        x[0] = pow(inputOne, inputTwo);
        x[0] += Q / x[0];
        x[0] *= (R < 0.0) ? 1 : -1;
        x[0] -= a1 / 3.0;
    }
}

double GetspecificHeatCapAir() {
    return 1200.0;
}

// J per K per kg - specific heat capacity of dry air
double GetdensityDryAir(double airPressure,
                        double temperature)    // air pressure in Pa, temperature in K
{
    temperature -= 273.15;

    if (temperature < -100.0) {
        cout << "Air temperature probably in Celsius not Kelvin" << endl;
    }

    return 1000.0 * airPressure / (287.05 * (temperature + 273.15));
}

// kg per cubic meter - density of air
double GetdensityAir(double airPressure,
                     double temperature,
                     double vapourPressure) {
    airPressure    /= 1000.0;
    vapourPressure /= 1000.0;

    return GetdensityDryAir(airPressure, temperature) * (1 - (vapourPressure * (1 - 0.622) / airPressure));
}
double e(double temprature)
{
	double ret_val = 0.6108*exp((17.27 *temprature) / (temprature + 237.3));

	return ret_val;
}
double GetsaturatedVapourPressure()    
{

	if (theClimate->Tdew != -99)
	{
		double ret_val = 0.6108*exp((17.27*theClimate->Tdew) / (theClimate->Tdew + 237.3));
		return ret_val;
	}
	if (theClimate->humidity != 0)
	{
		double ret_val = theClimate->humidity / 100.0 * ((e(theClimate->tmin) + e(theClimate->tmax)) / 2);
		return ret_val;
	}
	else
	{

		theMessage->FatalError("GetsaturatedVapourPressure: missing Data to calculate Vapour Pressure");
	}
}
// kg per cubic meter - density of air
double GetsaturatedWaterVapourPressure(double airTemperature)    // returns value in Pa
{
		airTemperature -= 273.3;

		if (airTemperature < -100.0) {
			theMessage->Warning("Air temperature probably in Celsius not Kelvin");
		}

		double ret_val;

		if (airTemperature > 0.0) {
			ret_val = 1000.0 * 0.61078 * exp((17.269 * airTemperature) / (airTemperature + 237.3));
		}
		else {
			ret_val = 1000.0 * 0.61078 * exp((21.87456 * airTemperature) / (airTemperature + 265.5));
		}

		return ret_val;
	
}

double GetlongWaveDown(double skyTemp) {
    return -119.0 + 1.06 * StephanBoltzmann * pow(skyTemp, 4);
}

double GetlongWaveUp(double emissivity,
                     double aTemperature) {
    return emissivity * StephanBoltzmann * pow(aTemperature, 4);
}

/*
 * Returns root of a quadratic
 */
bool SolveQuadratic(bool     posRoot,
                    double   a,
                    double   b,
                    double   c,
                    double * x) {
    if ((b * b - 4 * a * c) < 0) {
        return false;
    } else if (posRoot) {
        *x = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
    } else {
        *x = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
    }

    return true;
}

// in kJ per kg
double GetLatentHeatVaporisationWater(double temperature) {
    return -0.0000614342 * pow(temperature, 3) + 0.00158927 * pow(temperature, 2) - 2.36418 * temperature + 2500.79;
}

int newGap(int gap) {
    gap = (gap * 10) / 13;

    if ((gap == 9) || (gap == 10)) {
        gap = 11;
    }

    if (gap < 1) {
        gap = 1;
    }

    return gap;
}

void Comsort(temp_double_array double_array,
             temp_ptr_array    ptr_array,
             int               Hi) {
    int gap = Hi;

    while (true) {
        gap = newGap(gap);

        bool swapped = false;

        for (int k = 0; k < Hi - gap; k++) {
            int j = k + gap;

            if (double_array[k] > double_array[j]) {
                double a = double_array[k];

                double_array[k] = double_array[j];
                double_array[j] = a;

                void * p = ptr_array[k];

                ptr_array[k] = ptr_array[j];
                ptr_array[j] = p;
                swapped      = true;
            }
        }

        if ((gap == 1) &&!swapped) {
            break;
        }
    }
}

/*
 * Guess at FE from organic N content in DM (from values in feed table)
 */
double N_to_FE(double orgNcontent) {
    return min(1.1, 0.6 + 1000.0 * orgNcontent * N_to_protein * 0.001583);
}

double GetWindAtHeight(double origHt,
                       double origWind,
                       double newHt,
                       double roughnessLength) {
    return origWind * log(newHt / roughnessLength) / log(origHt / roughnessLength);
}

double GetGrossEnergyinDryMatter() {
    return 18.4;
}

//MJ per kg




