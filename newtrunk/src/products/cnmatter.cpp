
#include "../base/common.h"
#include "cnmatter.h"

cnMatter::cnMatter() {
    c     = 0.0;
    c_iso = 0.0;
    n     = 0.0;
    n15   = 0.0;
}

cnMatter::cnMatter(const cnMatter & source) {
    if (&source) {
        c     = source.c;
        c_iso = source.c_iso;
        n     = source.n;
        n15   = source.n15;
    }
}

cnMatter::cnMatter(double c1,
                   double c1_iso,
                   double n1,
                   double n1_15) {
    c     = c1;
    c_iso = c1_iso;
    n     = n1;
    n15   = n1_15;
}

void cnMatter::SetAll(double c1,
                      double c1_iso,
                      double n1,
                      double n1_15) {
    c     = c1;
    c_iso = c1_iso;
    n     = n1;
    n15   = n1_15;
}

double cnMatter::GetTotalC() {
    return c + c_iso;
}

double cnMatter::GetTotalN() {
    return n + n15;
}

double cnMatter::GetCNratio() {
    double ctot = GetTotalC();
    double ntot = GetTotalN();

    if (ntot < 1E-20) {
        return -1.0;
    } else {
        return ctot / ntot;
    }
}

cnMatter & cnMatter::operator = (const cnMatter & cnm) {
    c     = cnm.c;
    c_iso = cnm.c_iso;
    n     = cnm.n;
    n15   = cnm.n15;

    return *this;
}

cnMatter cnMatter::operator +(const cnMatter & cnm) {
    cnMatter copy = *this;

    copy.c     += cnm.c;
    copy.c_iso += cnm.c_iso;
    copy.n     += cnm.n;
    copy.n15   += cnm.n15;

    return copy;
}

cnMatter cnMatter::operator -(const cnMatter & cnm) {
    cnMatter copy = *this;

    copy.c     -= cnm.c;
    copy.c_iso -= cnm.c_iso;
    copy.n     -= cnm.n;
    copy.n15   -= cnm.n15;

    return copy;
}

cnMatter cnMatter::operator *(const double & factor) {
    cnMatter copy = *this;

    copy.c     *= factor;
    copy.c_iso *= factor;
    copy.n     *= factor;
    copy.n15   *= factor;

    return copy;
}

cnMatter cnMatter::operator /(const double & divisor) {
    cnMatter copy = *this;

    copy.c     /= divisor;
    copy.c_iso /= divisor;
    copy.n     /= divisor;
    copy.n15   /= divisor;

    return copy;
}
