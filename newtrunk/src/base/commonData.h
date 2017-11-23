/*
 * commonData.h
 *
 *  Created on: 16/02/2010
 *      Author: beer
 */

#include "../tools/fileAccess.h"
#include "../tools/convert.h"
#include "../base/common.h"

using namespace std;

#ifndef COMMONDATA_H_
#define COMMONDATA_H_

#include "../base/message.h"

class commonData {
    private:
        int     sectionPlace;
        convert conv;
        string  sectionName;
        string  fileNameInUse;
        bool    secnodaryFileInUse;
        int     sectionPlace2;
        string  fileNameInUse2;
        bool    extraOut;

    protected:
        bool                critical;
        std::vector<string> itemsName;
        std::vector<string> itemsValue;
        std::vector<bool>   used;
        std::vector<string> itemsName2;
        std::vector<string> itemsValue2;
        std::vector<bool>   used2;

    public:

#ifdef NITROSCAPE
        std::vector<string> items;
#endif

        commonData(bool extraOuput = true);

        ~commonData();

        void setCritical(bool aCritical) {
            critical = aCritical;
        }

        int findSectionThatContains(string common,
                                    string item,
                                    string section);

        bool FindSection(string aSectionName,
                         int    sectionNumber = -1);

        bool FindVectorSize(string VectorName,
                            int *  small,
                            int *  large);

        bool FindItem(string   ItemName,
                      double * output,
                      int      inVector = -1);

        bool FindItem(string ItemName,
                      bool * output,
                      int    inVector = -1);

        bool FindItem(string ItemName,
                      int *  output,
                      int    inVector = -1);

        bool FindItem(string   ItemName,
                      string * output,
                      int      inVector = -1);

        bool readFile(string fileName);

        bool readSecondaryFile(string fileName);

        bool getSectionsNumbers(string sectionName,
                                int *  small,
                                int *  large);

        bool findSection(string   org,
                         int      base,
                         string   name,
                         string   Alias,
                         string * path,
                         int *    Index);

        bool findSectionRun(string   org,
                            int      base,
                            string   name,
                            string   Alias,
                            string * path,
                            int *    Index);
};
#endif /* COMMONDATA_H_ */

