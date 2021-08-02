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

#include "xml.h"
#include "notevisitor.h"

class MusicXMLTimePositions
{
public:
    MusicXMLTimePositions() {
        timePositions.clear();
    };
    MusicXMLTimePositions(const MusicXMLTimePositions &p1) { timePositions = p1.timePositions; }
    
    // relative position in measure (double), default_x, voice number
    typedef std::tuple<double, int, int> VoicedTimePosition;
    
    void addTimePosition(int measure, double positionInMeasure, const MusicXML2::notevisitor& nv);
    
    /// Finds appropriate Guido Dx based on default-x of a musicXML element by overlooking all elements in measure based on position and voice number
    /// @param element the MusicXML element
    /// @param position musical position in measure
    /// @param onMeasure measure number (from musicXML)
    /// @param voice voice number
    /// @param xmlOffset MusicXML offset of the `element` if any
    /// @return a float containing Guido dx value based on available default-x in measure
    float getDxForElement(MusicXML2::xmlelement *element, double position, int onMeasure, int voice, long xmlOffset);
    
    int size() { return timePositions.size(); }
    
protected:
    
    
    int getVoice(VoicedTimePosition e) { return std::get<2>(e); }
    int getDefaultX(VoicedTimePosition e) { return std::get<1>(e); }

private:
    /// A map for measure number (first) to a vector Voiced Time Positions
    std::map<int, std::vector<VoicedTimePosition> > timePositions;
};


#endif /* mxmlPartTimePositions_hpp */
