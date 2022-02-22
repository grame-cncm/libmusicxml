//
//  mxmlPartTimePositions.cpp
//  libmusicxml2
//
//  Created by Arshia Cont on 02/08/2021.
//

#include "timePositions.h"

void MusicXMLTimePositions::addTimePosition(std::string measure, double positionInMeasure, const MusicXML2::notevisitor& nv) {
    auto timePos4measure = timePositions.find(measure);
    if (nv.x_default != -1) {
        VoicedTimePosition inner = std::make_tuple(positionInMeasure, nv.x_default, nv.getVoice(), nv.getStaff());
        if ( timePos4measure !=  timePositions.end())
        {
            timePos4measure->second.push_back(inner);
        }else {
            std::vector<VoicedTimePosition> innerVec(1, inner);
            timePositions.insert(std::make_pair(measure, innerVec));
        }
    }
}


float MusicXMLTimePositions::getDxForElement(MusicXML2::xmlelement *element, double position,
                                             std::string onMeasure, int voiceId, int staff, double xmlOffset) {
    auto timePos4measure = timePositions.find(onMeasure);
    float default_x = element->getAttributeFloatValue("default-x", 0),
    relative_x = element->getAttributeFloatValue("relative-x", 0);
    
    if ((default_x==0.0)&&(xmlOffset==0.0)) {
        return (relative_x/10)*2;
    }
        
    // MusicXML DOC: If an element within a <direction> includes a default-x attribute, the <offset> value will be ignored when determining the appearance of that element.
    double finalPosition = position + (default_x == 0 ? (double)xmlOffset : 0.0);

    if (timePos4measure != timePositions.end()) {
        auto it = find(timePos4measure->second, voiceId, staff, finalPosition);
        if (it != timePos4measure->second.end()) {
            float minXPos = getDefaultX(*it);
            //if (xpos != minXPos) {
            int finalDx = (relative_x/10)*2;
            // apply default-x ONLY if it exists
            if (default_x != 0) {
                // Use default-x if it exists; convert to half spaces
                finalDx += ( (default_x - minXPos)/ 10 ) * 2;
            }
            else if (xmlOffset != 0) {
                finalDx += ( (default_x - minXPos)/ 10 ) * 2;   // convert to half spaces
            }else {
                return -999;
            }
                
            return finalDx;
        }
    }
    return -999;        // This is when the xpos can not be computed
}

float MusicXMLTimePositions::getDxRelativeToMeasureForElement(MusicXML2::xmlelement *element, std::string onMeasure, int voiceId, double xmlOffset) {
    auto timePos4measure = timePositions.find(onMeasure);
    float default_x = element->getAttributeFloatValue("default-x", 0),
    relative_x = element->getAttributeFloatValue("relative-x", 0);
    
    if ((default_x==0.0)&&(xmlOffset==0.0)) {
        return (relative_x/10)*2;
    }
        
    // MusicXML DOC: If an element within a <direction> includes a default-x attribute, the <offset> value will be ignored when determining the appearance of that element.
    double finalPosition = (default_x == 0 ? (double)xmlOffset : 0.0);
    
    if (timePos4measure != timePositions.end()) {
        auto it = find(timePos4measure->second, voiceId, 0, finalPosition);
        if (it != timePos4measure->second.end()) {
            float minXPos = getDefaultX(*it);
            //if (xpos != minXPos) {
            float finalDx = (relative_x/10)*2;
            // apply default-x ONLY if it exists
            if (default_x != 0) {
                // Use default-x if it exists; convert to half spaces
                finalDx += ( (default_x - minXPos)/ 10 ) * 2;
            }
            else if (xmlOffset != 0) {
                finalDx += ( (default_x - minXPos)/ 10 ) * 2;   // convert to half spaces
            }else {
                finalDx -= (minXPos/10 * 2);
            }
                
            return finalDx;
        }
    }
    
    return -999;        // This is when the xpos can not be computed
}
