/*
 * pixiData.h
 *
 *  Created on: Jul 19, 2010
 *      Author: JOVE
 */

#ifdef NITROSCAPE

#ifndef PIXIDATA_H_
#define PIXIDATA_H_

#include <vector>
#include <iostream>
#include "../base/commonData.h"
#include <string>

class pixiData {
    private:
        pixiData();

    public:
        pixiData(string path) {
            loadData(path);
        }

        // return the numbers of farms
        int getNumberOfFarms() {
            return farmInformation.size();
        }

        // return the numbers of Fields at a given farm. The farm should be 0-index
        int getNumberOfFields(unsigned int farm) {
            if (farm < farmInformation.size()) {
                return farmInformation.at(farm).size();
            } else {
                return -1;
            }
        }

        // return the numbers of Ecosystems at a given farm and field. The farm and field should be 0-index
        int getNumberOfEco(unsigned int farm,
                           unsigned int field) {
            if ((farm <= farmInformation.size()) && (field <= farmInformation.at(farm).size())) {
                return farmInformation.at(farm).at(field).size();
            } else {
                return -1;
            }
        }

        // return the Eco number based on farm,field and patch. Those are 0-index
        int getEco(unsigned int farm,
                   unsigned int field,
                   unsigned int patch) {
            if ((farm <= farmInformation.size()) && (field <= farmInformation.at(farm).size())) {
                return farmInformation.at(farm).at(field).at(patch);
            } else {
                return -1;
            }
        }

    private:
        vector<vector<vector<int> > > farmInformation;

        // loading the data
        void loadData(string path);
};


extern pixiData * thePixiData;
#endif /* PIXIDATA_H_ */

#endif

