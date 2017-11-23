/*
 * Created by Nick Hutchings, Sept 2006
 */

#include "../base/common.h"
#include "forageCutter.h"

forageCutter::forageCutter(string       aName,
                           const int    aIndex,
                           const base * aOwner):
    techEquip(aName,
              aIndex,
              aOwner)
{
    residualHeight = 0.0;
}

forageCutter::forageCutter(const forageCutter & aforageCutter):
    techEquip(aforageCutter)
{
    residualHeight = 0;

    theMessage -> FatalError("tetemanure.cpp::attempt to use undefined copy constructor");
}

forageCutter::~forageCutter() {}

void forageCutter::ReadParameters(commonData *& file) {
    techEquip::ReadParameters(file);
    file -> FindItem("residualHeight", &residualHeight);
}
