/*
 * pixiData.cpp
 *
 *  Created on: Jul 19, 2010
 *      Author: JOVE
 */

#ifdef NITROSCAPE

#include "pixiData.h"
#include "../tools/convert.h"
#include "../tools/fileAccess.h"
#include "../base/settings.h"

void pixiData::loadData(string path) {
    fileAccess hd;

    hd.changeDir(path);

    // assumes fasset.dist is two levels up in path hierarchy
    string thePath = hd.getCurrentPath();

    cout << " FASSET : fasset.dist path " << thePath << endl;

    commonData data(false);

    data.readFile("fasset.dist");

    convert conv;

    for (unsigned int i = 1; i < data.items.size(); i++) {

        // cout << " FASSET : item #" << i << endl;
        // cout << " FASSET : item " <<    data.items.at(i) << endl;
        if (data.items.at(i) == "NULL") {
            break;
        }

        unsigned int farm  = conv.StringToInt(conv.partOfAString(data.items.at(i), 1));
        unsigned int field = conv.StringToInt(conv.partOfAString(data.items.at(i), 2));
        unsigned int eco   = conv.StringToInt(conv.partOfAString(data.items.at(i), 3));

        if ((farm == -1) || (field == -1) || (eco == -1)) {
            cout << " FASSET : wrong format in fasset.dist. Line " << data.items.at(i) << " contains -1" << endl;

            // exit(99);
            // break;
        } else {

            // cout << " FASSET : farm=" << farm << endl;
            // cout << " FASSET : field=" << field << endl;
            // cout << " FASSET : eco=" << eco << endl;
            while (farmInformation.size() <= farm) {

                // cout << " FASSET : farmInformation.size() = " << farmInformation.size() << endl;
                vector<vector<int> > fieldVector;

                farmInformation.push_back(fieldVector);
            }

            while (farmInformation.at(farm).size() <= field) {
                vector<int> eco;

                farmInformation.at(farm).push_back(eco);
            }

            farmInformation.at(farm).at(field).push_back(eco);
        }
    }

#ifdef NITROSCAPE
    cout << " FASSET : fasset.dist is read." << endl;
#endif

}
#endif

