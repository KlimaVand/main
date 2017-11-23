/*
 * converters.cpp
 *
 *  Created on: Aug 21, 2009
 *      Author: JOVE
 */

#include "../tools/convert.h"
#include "../base/message.h"
#include "../base/unix_util.h"
#include "../base/typer.h"

convert::convert() {}

string convert::IntToString(int conv) {

#ifdef __BCplusplus__
    string myBuff;
    string strRetVal;

    // Create a new char array
    myBuff = new char[100];

    // Set it to empty
    memset(myBuff, '\0', 100);

    // Convert to string
    itoa(conv, myBuff, 10);

    // Copy the buffer into the string object
    strRetVal = myBuff;

    // Delete the buffer
    delete[] myBuff;

    return (strRetVal);
#else
    std::stringstream Num;
    std::string str;

    Num << conv;

    str = Num.str();

    return str;
#endif

}

double convert::StringToDouble(string conv) {
    return atof(conv.c_str());
}

int convert::StringToInt(string conv) {
    return atoi(conv.c_str());
}

string convert::DoubleToChar(double conv) {
    char output[200];

    sprintf(output, "%f", conv);

    return output;
}

string convert::EatSpaces(string s) {
    int source = 0;
    int dest   = 0;

    while (s[source] != '\0') {
        while (s[source] == ' ') {
            source++;
        }

        s[dest] = s[source];

        dest++;
        source++;
    }

    s[dest] = '\0';

    return s;
}

bool convert::StringToBool(string input) {
    bool output = false;

    transform(input.begin(), input.end(), input.begin(), ::tolower);

    if (input == "0") {
        output = false;
    } else if (input == "false") {
        output = false;
    } else if (input == "f") {
        output = false;
    } else if (input == "no") {
        output = false;
    } else if (input == "n") {
        output = false;
    } else if (input == "1") {
        output = true;
    } else if (input == "true") {
        output = true;
    } else if (input == "t") {
        output = true;
    } else if (input == "yes") {
        output = true;
    } else if (input == "y") {
        output = true;
    }

    return output;
}

string convert::GetUnitTypeIntToString(int aMainUnit) {
    if (aMainUnit == kwh) {
        return "kwh";
    } else if (aMainUnit == tfi) {
        return "tfi";
    } else if (aMainUnit == t) {
        return "t";
    } else if (aMainUnit == kg) {
        return "kg";
    } else if (aMainUnit == g) {
        return "g";
    } else if (aMainUnit == m3) {
        return "m3";
    } else if (aMainUnit == l) {
        return "l";
    } else if (aMainUnit == ha) {
        return "ha";
    } else if (aMainUnit == m2) {
        return "m2";
    } else if (aMainUnit == m) {
        return "m";
    } else if (aMainUnit == mm) {
        return "mm";
    } else if (aMainUnit == sfu) {
        return "sfu";
    } else if (aMainUnit == day) {
        return "day";
    } else if (aMainUnit == hour) {
        return "hour";
    } else if (aMainUnit == kr) {
        return "kr";
    } else if (aMainUnit == fraction) {
        return "fraction";
    } else if (aMainUnit == pcs) {
        return "pcs";
    }

    return "illegal unit";
}

int convert::GetChemType(string aChemType) {
    int typeRes = -1;

    if (aChemType == "herbicide") {
        typeRes = herbicide;
    } else if (aChemType == "fungicide") {
        typeRes = fungicide;
    } else if (aChemType == "pesticide") {
        typeRes = pesticide;
    } else if (aChemType == "microfertilizer") {
        typeRes = microfertilizer;
    } else if (aChemType == "insecticide") {
        typeRes = insecticide;
    }

    if (typeRes == -1) {
        theMessage -> WarningWithDisplay("chemical::GetChemType - the chemical unit type <",
                                         aChemType, "> does not exist");

        return -1;
    } else {
        return typeRes;
    }
}

string convert::partOfAString(string input,
                              int    number) {

#ifndef __BCplusplus__
    return partOfAStringC(input, number);
#endif

#ifdef __BCplusplus__
    return partOfAStringBorland(input, number);
#endif

}

#ifndef __BCplusplus__
string convert::partOfAStringC(string input,
                               int    number) {
    stringstream totalSString(input);
    string       nameOfItem;

    for (int i = 0; i < number; i++) {
        totalSString >> nameOfItem;
    }

    return nameOfItem;
}
#endif

#ifdef __BCplusplus__
string convert::partOfAStringBorland(string input,
        int                                 number) {
    string     fin = input;
    istrstream ins(fin, strlen(fin));
    string     text1;

    for (int i = 0; i < number; i++) {
        ins >> text1;
    }

    return text1;
}
#endif

