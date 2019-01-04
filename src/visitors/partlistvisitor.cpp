//
//  partlistvisitor.cpp
//  libmusicxml2
//
//  Created by Arshia Cont on 05/01/17.
//
//

#include <algorithm>

#include "partlistvisitor.h"

#include "rational.h"
#include "xml_tree_browser.h"
#include "tree_browser.h"

using namespace std;

namespace MusicXML2
{
    partlistvisitor::partlistvisitor() : fPartGroupIncrementer(0), staffCreatorCounter(1)
    {
    }
    
    
    partGroup* partlistvisitor::find_first_of_partID_inGroup(std::string partID)
    {
        // search if this part ID exists in any grouping
        // Guido Limitation: One \accol tag per staff ONLY (for nested definitions)
        // IDEA: Treat the FIRST occurence of partID in grouping and get rid of it.
        std::map<int, partGroup>::iterator partGroupIt;
        for (partGroupIt=fPartGroups.begin();
             partGroupIt != fPartGroups.end();
             partGroupIt++)
        {
            if (partGroupIt->second.visited == false) {
                if (std::find(partGroupIt->second.partIDs.begin()
                              , partGroupIt->second.partIDs.end(),
                              partID) != partGroupIt->second.partIDs.end() )
                {
                    // this partID is in group number partGroupIt->first
                    //cerr << "\t ID found in group " << partGroupIt->first <<" "<<endl;
                    break;
                    
                }
            }
        }
        
        if (partGroupIt != fPartGroups.end())
        {
            return &partGroupIt->second;
        }
        
        return NULL;
    }
    
    void partlistvisitor::partID2range(partGroup &pGroup)
    {
        std::vector<int> staves;
        for (size_t i=0; i<pGroup.partIDs.size();i++)
        {
            staves.push_back(part2staffmap[pGroup.partIDs[i]]);
        }
        std::vector<int>::iterator rangeEnd = std::max_element(staves.begin(), staves.end());
        std::vector<int>::iterator rangeBegin = std::min_element(staves.begin(), staves.begin());
        
        stringstream rangeStream;
        rangeStream << "\""<< (*rangeBegin)<<"-"<<(*rangeEnd)<<"\"";
        pGroup.guidoRange = rangeStream.str();
        pGroup.guidoRangeStart = *rangeBegin;
        pGroup.guidoRangeStop = *rangeEnd;
    }
    
    bool partlistvisitor::checkLonelyBarFormat(int staffID)
    {
        std::map<int, partGroup>::iterator partGroupIt;
        for (partGroupIt=fPartGroups.begin();
             partGroupIt != fPartGroups.end();
             partGroupIt++)
        {
            if (partGroupIt->second.barlineGrouping == true)
            {
                // see if this staff is in range
                if ( (staffID>= partGroupIt->second.guidoRangeStart)&&(staffID<=partGroupIt->second.guidoRangeStop))
                {
                    return false;
                }
            }
        }
        
        return true;
    }
    
    /// Visitors
    
    void partlistvisitor::visitStart ( S_part_group& elt )
    {
        int partGroupNumber = elt->getAttributeIntValue("number", 0);
        /// IMPORTANT NOTE: the number attribute is NOT sequential and can be repeated! So we need to do further book-keeping.
        std::string partGroupType = elt->getAttributeValue("type");
        
        if (partGroupType=="start")
        {
            int groupIndex = fPartGroupIncrementer;
            fPartGroups[groupIndex].xmlGroupNumber = partGroupNumber;
            if (elt->getValue(k_group_symbol)=="bracket")
            {
                fPartGroups[groupIndex].bracket = true;
            } else
                fPartGroups[groupIndex].bracket = false;
            
            if (elt->getValue(k_group_barline)=="yes")
            {
                fPartGroups[groupIndex].barlineGrouping = true;
            } else
                fPartGroups[groupIndex].barlineGrouping = false;
            
            // Add optional names
            fPartGroups[groupIndex].fGroupName = elt->getValue(k_group_name);
            fPartGroups[groupIndex].visited = false;
            
            fCurrentPartGroupIndex.push_back(groupIndex);
            fPartGroupIncrementer++;
            //cerr << "Started group " << partGroupNumber << " index " << groupIndex<<" (" << fCurrentPartGroupIndex.size() << ")" << endl;
        }else
            if (partGroupType=="stop")
            {
                // Erase the INDEX whose xmlGroupNumber is equal to partGroupNumber
                std::vector<int>::iterator ito;
                for (ito = fCurrentPartGroupIndex.begin(); ito < fCurrentPartGroupIndex.end(); ito++)
                {
                    if (fPartGroups[*ito].xmlGroupNumber == partGroupNumber)
                        break;
                }
                
                // calculate Guido Range and set
                partID2range(fPartGroups[*ito]);
                
                // Do Erase
                if (ito != fCurrentPartGroupIndex.end())
                {
                    fCurrentPartGroupIndex.erase(ito);
                }else
                    cerr << "Something is really wrong in S_PART_GROUP visitor!" << endl;
            }
    }
    
    void partlistvisitor::visitStart( S_score_part& elt)
    {
        std::string PartID = elt->getAttributeValue("id");
        part2staffmap[PartID] = staffCreatorCounter;
        staffCreatorCounter++;
        
        fPartHeaders[PartID].fPartName = elt->getValue(k_part_name);
        fPartHeaders[PartID].fPartNameAbbr = elt->getValue(k_part_abbreviation);
        
        // add groupings if any
        if (fCurrentPartGroupIndex.size())
        {
            for (size_t ind=0; ind < fCurrentPartGroupIndex.size(); ind++)
            {
                fPartGroups[fCurrentPartGroupIndex[ind]].partIDs.push_back(PartID);
            }
        }
    }
    
    
    
}
