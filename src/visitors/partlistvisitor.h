//
//  partlistvisitor.hpp
//  libmusicxml2
//
//  Created by Arshia Cont on 05/01/17.
//
//

#ifndef partlistvisitor_h
#define partlistvisitor_h

#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>

#include "exports.h"
#include "guido.h"
#include "typedefs.h"
#include "visitor.h"
#include "xml.h"

#include "smartlist.h"

namespace MusicXML2
{
    
    class EXP partGroup {
    public:
        partGroup(): bracket(false), barlineGrouping(false), visited(false) {};
        virtual ~partGroup() {};
        
        std::vector<std::string> partIDs;
        std::string guidoRange;
        int guidoRangeStart, guidoRangeStop;
        bool bracket;
        bool barlineGrouping;
        std::string fGroupName;             // optional name of the group
        std::string fGroupNameDisplay;     // overrides the previous
        bool visited;
        int xmlGroupNumber;
    };
    
    class EXP partHeader {
    public:
        partHeader(): visited(false) {};
        partHeader(std::string name, std::string nameabbr):
            fPartName(name), fPartNameAbbr(nameabbr), visited(false)
        {};
        virtual ~partHeader()
        {
            fPartNameAbbr.clear(); fPartName.clear();
        };
        
        std::string		fPartName;
        std::string     fPartNameAbbr;
        bool visited;
    };
    
    /*!
     \addtogroup visitors
  @{
     */
    
    /*!
     \brief Produces a summary of a MusicXML parts for groupings.
     */
    class EXP partlistvisitor :
    public visitor<S_score_part>,
    public visitor<S_part_group>
    {
    public:
        partlistvisitor();
        virtual	~partlistvisitor() {};
        
        std::map<int, partGroup> fPartGroups;
        
        std::map<std::string, partHeader> fPartHeaders;
        
        /*!
         \brief Returns the part group with first occurence of partID (string)
         */
        partGroup* find_first_of_partID_inGroup(std::string partID);
        
        /*!
         \brief Converts XML Part ID list to Guido's Staff range string
         */
        void partID2range(partGroup &pGroup);
        
        /*!
         \brief Checks if current staff is lonely in terms of BarFormat definition
         */
        bool checkLonelyBarFormat(int staffID);
        
    protected:
        
        void visitStart( S_score_part& elt);
        void visitStart( S_part_group& elt);
        
        int fPartGroupIncrementer;
        std::vector<int>  fCurrentPartGroupIndex;
        
        std::map<std::string, int> part2staffmap;
        int staffCreatorCounter;
    };
    
}   // NAMESPACE MUSICXML2
#endif /* partlistvisitor_hpp */
