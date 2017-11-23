require("XML")

library(XML)
setwd("D:/main/tools/annimalChange/R")
xmlfile=xmlParse("outputFarm117051ScenarioNr1.xml")

xmltop = xmlRoot(xmlfile)
xmlName(xmltop) #give name of node, PubmedArticleSet
xmlSize(xmltop) #how many children in node, 19
i=0
for (i in 1:xmlSize(xmltop)) {
    if("CropSequenceClass"==xmlName(xmltop[[i]]))
    {
        j=0
        for (j in 1:xmlSize(xmltop[[i]])) {

            if("CropClass"==xmlName(xmltop[[i]][[j]]))
            {
                l=0
                for (l in 1:xmlSize(xmltop[[i]][[j]])) {

                    if("urineCCropClass"==xmlName(xmltop[[i]][[j]][[l]]))
                    {
                    k=0
                    for (k in 1:xmlSize(xmltop[[i]][[j]][[l]])) {
                    
                    if("Value"==xmlName(xmltop[[i]][[j]][[l]][[k]]))
                    {
                        print( xmlValue(xmltop[[i]][[j]][[l]][[k]]))
                    }
                    }
                    }
                }
            }
        }
    }
}


