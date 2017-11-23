
#include "../base/common.h"
#include "addedPool.h"
#include <string.h>

addedPool::addedPool(string       Pname,
                     const int    Index,
                     const base * owner):
    matter(Pname,
           Index,
           owner)
{
    poolNameChar   = "AOM-subpool";
    poolName       = 7;
    ReduceTurnover = 0.0;
    Derived        = false;
}

addedPool::addedPool(const addedPool & source):
    matter(source)
{
    Derived        = false;
    poolNameChar   = source.poolNameChar;
    poolName       = source.poolName;
    ReduceTurnover = source.ReduceTurnover;
}

addedPool * addedPool::clone() const {
    addedPool * a = new addedPool(*this);

    return a;
}

void addedPool::Decay(double     environmentEffect,
                      cnMatter & out,
                      double     cnc) {
    TestState();

    if (!IsAOM() && (cnRatio <= 0)) {
        theMessage -> FatalError("matter::TestState - illegal state of cnRatio variable");
    }

    CNC = cnc;

    intermed.SetAll(0.0, 0.0, 0.0, 0.0);

    if (carbon.c > 1E-45) {

        // CK: clayEffect1 is 1 if not used...
        if (expDecay) {    // Exponential decay
            intermed = carbon * (1 - exp(-decompositionRate * environmentEffect * clayEffect1));
        } else {           // Euler integration
            intermed = carbon * decompositionRate * environmentEffect * clayEffect1;
        }

        if (intermed.c > carbon.c) {

            // CK: strange code. it can only be < carbon, if
            // decompositionRate, envEffect, clayEffect HAVE values in [0,1]
            // but it does not run without it
            intermed = carbon;
        }

        carbon = carbon - intermed;
    }

    if (intermed.c > 0.0) {
        double q = 1E30;

        if (intermed.n > 1E-30) {
            q = intermed.c / intermed.n;
        }

        if (q <= cnc) {
            out = intermed;

            intermed.SetAll(0.0, 0.0, 0.0, 0.0);
        }
    }

    TestState();
}

/*
 * Adds all import to the pool, and returns the current carbon content
 * content - C content in pool
 */
void addedPool::CatchInflux(cnMatter & content) {
    if (influx.c < 0.0) {
        theMessage -> FatalError("addedPool::CatchInflux - C influx to pool negative");
    }

    if (influx.n < 0.0) {
        theMessage -> FatalError("addedPool::CatchInflux - N influx to pool negative");
    }

    // if ((influx.c > 0.0) && (influx.n <= 0)) { // CK: unreachable code
    // theMessage -> FatalError("addedPool::CatchInflux - Influx to pool has C but no N");
    // }
    carbon = carbon + influx;
    carbon = carbon + added;

    influx.SetAll(0.0, 0.0, 0.0, 0.0);
    added.SetAll(0.0, 0.0, 0.0, 0.0);

    content = carbon + intermed;
}

void addedPool::FinalExportAOM(double     ratio,
                               cnMatter & out) {
    if ((ratio > 1.0) || (ratio < 0.0)) {
        theMessage -> FatalError("addedPool::FinalExportAOM - 'ratio' has illegal value");
    }

    if (intermed.c > 0.0) {
        carbon = carbon + intermed * (1.0 - ratio);
        out    = out + intermed * ratio;

        intermed.SetAll(0.0, 0.0, 0.0, 0.0);
    }
}

void addedPool::SetReduceTurnover(double Reduce) {
    ReduceTurnover = Reduce;

    if (Reduce > 0.0) {
        decompositionRate = decompositionRate * Reduce;
    }
}
