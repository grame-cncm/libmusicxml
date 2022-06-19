//
//  mxmlPartTimePositions.hpp
//  libmusicxml2
//
//  Created by Arshia Cont on 02/08/2021.
//

#ifndef mxmlTimePositions_hpp
#define mxmlTimePositions_hpp

#include <stdio.h>
#include <map>
#include <tuple>
#include <algorithm>

#include "xml.h"
#include "notevisitor.h"

class MusicXMLTimePositions
{
public:
    MusicXMLTimePositions() {
        timePositions.clear();
    };
    MusicXMLTimePositions(const MusicXMLTimePositions &p1) { timePositions = p1.timePositions; }
    
    // relative position in measure (double), default_x, voice number, staff number
    typedef std::tuple<double, int, int, int> VoicedTimePosition;
    typedef std::vector<VoicedTimePosition> TimePositionList;
    
    void addTimePosition(std::string measure, double positionInMeasure, const MusicXML2::notevisitor& nv);
    
    /// Finds appropriate Guido Dx based on default-x of a musicXML element by overlooking all elements in measure based on position and voice number
    /// @param element the MusicXML element
    /// @param position musical position in measure
    /// @param onMeasure measure number (from musicXML)
    /// @param voice voice number
    /// @param staff staff number
    /// @param xmlOffset MusicXML offset of the `element` if any
    /// @return a float containing Guido dx value based on available default-x in measure
    float getDxForElement(MusicXML2::xmlelement *element, double position, std::string onMeasure, int voice, int staff, double xmlOffset);
    
    /// Finds Guido Dx for XML Directions whose default horizontal position is from the Beginning of the Measure, such as Words and Tempo.
    float getDxRelativeToMeasureForElement(MusicXML2::xmlelement *element, std::string onMeasure, int voice, double xmlOffset);
    
    
    size_t size() { return timePositions.size(); }
    
protected:
    
    int getVoice(VoicedTimePosition e) { return std::get<2>(e); }
    int getDefaultX(VoicedTimePosition e) { return std::get<1>(e); }
    int getStaff(VoicedTimePosition e) { return std::get<3>(e); }
    
    /// Returns iterator in TimePositionList with equal voiceId and EQUAL position to query
    TimePositionList::iterator find(TimePositionList &timePos4measure, int voiceId, int staff, double position) {
//        cerr<<"Start Search for Position "<<position<<" voiceId:"<<voiceId<<endl;
        auto it = std::find_if(timePos4measure.begin(), timePos4measure.end(), [voiceId, staff, position] (const VoicedTimePosition& e) {
            if ((voiceId > 0) && (std::get<2>(e) != voiceId) ) {
                return false;
            }
            if ((staff > 0) && (std::get<3>(e) != staff) ) {
                return false;
            }
//            cerr<<"\t TimePos elementPos="<<std::get<0>(e)<<endl;
            return std::get<0>(e) == position;
        });
        
        return it;
    }

private:
    /// A map for measure number (first) to a vector Voiced Time Positions
    std::map<std::string, std::vector<VoicedTimePosition> > timePositions;
};


#endif /* mxmlPartTimePositions_hpp */
