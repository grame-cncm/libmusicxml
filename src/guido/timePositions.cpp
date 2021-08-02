//
//  mxmlPartTimePositions.cpp
//  libmusicxml2
//
//  Created by Arshia Cont on 02/08/2021.
//

#include "timePositions.h"

void MusicXMLTimePositions::addTimePosition(int measure, double positionInMeasure, const MusicXML2::notevisitor& nv) {
    auto timePos4measure = timePositions.find(measure);
    if (nv.x_default != -1) {
        if ( timePos4measure !=  timePositions.end())
        {
            VoicedTimePosition inner = std::make_tuple(positionInMeasure, nv.x_default, nv.getVoice());
            timePos4measure->second.push_back(inner);
        }else {
            VoicedTimePosition inner = std::make_tuple(positionInMeasure, nv.x_default, nv.getVoice());
            std::vector<VoicedTimePosition> innerVec(1, inner);
            timePositions.insert(std::make_pair(measure, innerVec));
        }
    }
}


float MusicXMLTimePositions::getDxForElement(MusicXML2::xmlelement *element, double position, int onMeasure, int voiceId, long xmlOffset) {
    auto timePos4measure = timePositions.find(onMeasure);
    float default_x = element->getAttributeFloatValue("default-x", 0),
    relative_x = element->getAttributeFloatValue("relative-x", 0);
    
    float xpos = default_x + relative_x;
    
    // MusicXML DOC: If an element within a <direction> includes a default-x attribute, the <offset> value will be ignored when determining the appearance of that element.
    double finalPosition = position + (default_x == 0 ? (double)xmlOffset : 0.0);

    if (timePos4measure != timePositions.end()) {
        auto it = std::find_if(timePos4measure->second.begin(), timePos4measure->second.end(), [voiceId, finalPosition] (const VoicedTimePosition& e) {
            if ((voiceId > 0) && (std::get<2>(e) != voiceId) ) {
                return false;
            }
            
            return std::get<0>(e) == finalPosition;
        });
        if (it != timePos4measure->second.end()) {
            float minXPos = getDefaultX(*it);
            if (xpos != minXPos) {
                int finalDx = (relative_x/10)*2;
                // apply default-x ONLY if it exists
                if (default_x!=0)
                    finalDx = ( (xpos - minXPos)/ 10 ) * 2;   // convert to half spaces
                
                return finalDx;
            }
        }
    }
    return -999;        // This is when the xpos can not be computed
}
