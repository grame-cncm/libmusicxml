//
//  musicxmlquery.cpp
//  GuidoKit
//
//  Created by Arshia Cont on 23/12/16.
//  Copyright © 2016 Antescofo SAS. All rights reserved.
//

#include "musicxmlquery.h"

#include "libmusicxml.h"
#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include <sstream>

// Transposition operations on MusicXML
#include "xml.h"
#include "xmlreader.h"
#include "xml_tree_browser.h"

namespace MusicXML2
{
    /// Constructors and Deconstructors
    musicxmlQuery::musicxmlQuery() {
        init();
    }
    
    musicxmlQuery::~musicxmlQuery() {
    }
    
    void musicxmlQuery::init() {
        beatCum = 0.0;
    }
    
    
    int musicxmlQuery::getStavesForFirstPart() {
        
        return stavesInPart.begin()->second;
    }
    
    int musicxmlQuery::getTotalStaves() {
        int totalStaves = 0;
        for (auto&& e : stavesInPart) {
            totalStaves += e.second;
        }
        return totalStaves;
    }

    /// Instance methods:
    int  musicxmlQuery::getTransposeInstrumentChromatic () {
        // The chromatic element, representing the number of chromatic steps to add to the written pitch, is the one required element. The diatonic, octave-change, and double elements are optional elements.
        return fChromatic + (fOctaveChange * 12);
    }
    
    std::string musicxmlQuery::getTransposeInstrumentName() {
        switch (fChromatic) {
            case -2:
                return "Bb";
                break;
                
            case -3:
                return "A";
                break;
                
            case -5:
                return "G";
                break;
                
            case 3:
                return "Eb";
                break;
                
            case 2:
                return "D";
                break;
                
            default:
                return "C";
                break;
        }
    }
    
    
    //________________________________________________________________________
    // The visit methods
    //________________________________________________________________________
    void musicxmlQuery::visitEnd ( S_note& elt )
    {
        notevisitor::visitEnd (elt);
        
        ctree<xmlelement>::iterator next;
        for (ctree<xmlelement>::iterator i = elt->begin(); i != elt->end(); ) {
            next = i;
            next++;
            switch (i->getType()) {
                case k_step:
                    break;
                case k_octave:
                    break;
                case k_alter:

                    break;
                case k_accidental:
                    break;
                case k_time_modification:
                case k_stem:
                case k_notehead:
                case k_staff:
                case k_beam:
                case k_notations:
                case k_lyric:

                    break;
            }
            i = next;
        }
    }
    
    
    void musicxmlQuery::visitEnd ( S_rehearsal& elt )
    {
        //cout<< " Rehearsal Mark \""<< elt->getValue() <<"\" at beat-pos "<<beatCum<<endl;
    }
    
    //________________________________________________________________________
    void musicxmlQuery::visitStart ( S_part& elt)
    {
        currentPart = elt->getAttributeValue("id");
        stavesInPart[currentPart] = 1;
    }
    
    //________________________________________________________________________
    void musicxmlQuery::visitStart ( S_staves& elt)
    {
        stavesInPart[currentPart] = int(*elt);
    }
    
}
