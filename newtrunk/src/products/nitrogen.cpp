
#include "../base/common.h"
#include "nitrogen.h"

nitrogen::nitrogen() {
    n   = 0.0;
    n15 = 0.0;
}

nitrogen::nitrogen(double d) {
    n   = d;
    n15 = 0;
}

nitrogen::nitrogen(const nitrogen & nit) {
    if (&nit) {
        n   = nit.n;
        n15 = nit.n15;
    }
}

double nitrogen::Get15NRatio() {
    double retVal = 0.0;

    if (n + n15 > 1E-9) {
        retVal = n15 / n;
    }

    return retVal;
}

void nitrogen::SetBoth(double n1,
                       double n1_15) {
    n   = n1;
    n15 = n1_15;
}

nitrogen & nitrogen::operator = (const nitrogen & nit) {
    n   = nit.n;
    n15 = nit.n15;

    return *this;
}

nitrogen & nitrogen::operator = (const double & d) {
    n   = d;
    n15 = 0.0;

    return *this;
}

nitrogen nitrogen::operator +(const nitrogen & nit) {
    nitrogen copy = *this;

    copy.n   += nit.n;
    copy.n15 += nit.n15;

    return copy;
}

nitrogen nitrogen::operator -(const nitrogen & nit) {
    nitrogen copy = *this;

    copy.n   -= nit.n;
    copy.n15 -= nit.n15;

    return copy;
}

nitrogen nitrogen::operator *(const double & factor) {
    nitrogen copy = *this;

    copy.n   *= factor;
    copy.n15 *= factor;

    return copy;
}

nitrogen nitrogen::operator /(const double & divisor) {
    nitrogen copy = *this;

    copy.n   /= divisor;
    copy.n15 /= divisor;

    return copy;
}

bool nitrogen::operator !=(const nitrogen & nit) {
    return ((n != nit.n) && (n15 != nit.n15));
}

bool nitrogen::operator ==(const nitrogen & nit) {
    return ((n == nit.n) && (n15 == nit.n15));
}

bool nitrogen::operator >(const nitrogen & nit) {
    return (n > nit.n);
}

double nitrogen::operator /(const nitrogen & nit) {
    return n / nit.n;
}
