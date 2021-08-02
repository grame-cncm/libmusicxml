/*
 MusicXML Library
 Copyright (C) Grame 2006-2013
 
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 
 Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 research@grame.fr
 */

#ifndef __xml2guidovisitor__
#define __xml2guidovisitor__

#include <ostream>
#include <stack>
#include <map>
#include <string>
#include <set>

#include "exports.h"
#include "guido.h"
#include "typedefs.h"
#include "visitor.h"
#include "xml.h"
#include "rational.h"


#include "partlistvisitor.h"
#include "transposevisitor.h"
#include "clefvisitor.h"

namespace MusicXML2 
{

/*!
 \ingroup visitors guido
 @{
 */

//______________________________________________________________________________
typedef struct {
    S_movement_title		fTitle;
    std::vector<S_creator>	fCreators;
} scoreHeader;

/*!
 \brief A score visitor to produce a Guido representation.
 */
//______________________________________________________________________________
class EXP xml2guidovisitor :
public partlistvisitor,
public transposevisitor,
public visitor<S_score_partwise>,
public visitor<S_movement_title>,
public visitor<S_creator>,
public visitor<S_defaults>,
public visitor<S_part>,
public visitor<S_staves>,
public clefvisitor
{
private:
    // the guido elements stack
    std::stack<Sguidoelement>	fStack;
    bool	fGenerateComments, fGenerateStem, fGenerateBars, fGeneratePositions;
    
    scoreHeader		fHeader;		// musicxml header elements (should be flushed at the beginning of the first voice)
    std::string		fCurrentPartID;
    int				fCurrentStaffIndex;		// the index of the current guido staff
    
    void start (Sguidoelement& elt)		{ fStack.push(elt); }
    void add (Sguidoelement& elt)		{ if (fStack.size()) fStack.top()->add(elt); }
    void push (Sguidoelement& elt)		{ add(elt); fStack.push(elt); }
    void pop ()	{
        if (fStack.size())
            fStack.pop();
        else
            cerr<<"xml2guido: Pop() called while EMPTY! Please REPORT!"<<endl;
    }
    
    void flushHeader	 ( scoreHeader& header );
    void flushPartHeader ( partHeader& header );
    void flushPartGroup (std::string partID);
    
protected:
    
    virtual void visitStart( S_score_partwise& elt);
    virtual void visitStart( S_movement_title& elt);
    virtual void visitStart( S_creator& elt);
    //virtual void visitStart( S_score_part& elt);
    virtual void visitStart( S_defaults& elt);
    virtual void visitStart( S_part& elt);
    virtual void visitStart ( S_staves& elt);
    virtual void visitEnd ( S_clef& elt );
    
    Sguidoelement& current ()				{ return fStack.top(); }
    
    bool previousStaffHasLyrics;
    
    int  fCurrentAccoladeIndex;
    
    int  fPartNum;  // 0 (default) to parse all score-parts. 1 for "P1" only, etc.
    int fPartsAvailable; // number of parts available in this XML
    
    int fBeginMeasure;      // Number of measure to begin parsing, used for partial conversion. (default = 0)
    int fEndMeasure;        // Measure Number where parsing ends, used for partial conversion. (default = 0 meaning go to the end)
    int fEndMeasureOffset;  // Ending the score conversion at this offset (but giving fEndMeasure for fEndPosition)
    
    /// Guido Position corresponding to fBeginMeasure
    rational fBeginPosition;
    /// Guido Position corresponding to fEndMeasure (without fEndMeasureOffset)
    rational fEndPosition;
    
    static int defaultStaffDistance;   // xml staff-distance value in defaults
    
    std::map<std::string, int> stavesInPart;
    std::map<std::string, std::set<std::string> > clefsInPart;
    std::string currentPart;
    
    /// Number of total measures in score
    int fTotalMeasures;
    
    /// Map containing measure number and Position (double output of Rational)
    std::map<double, int> measurePositionMap;
    
    double fTotalDuration;
    
public:
    xml2guidovisitor(bool generateComments, bool generateStem, bool generateBar=true, int partNum = 0, int beginMeasure = 0, int endMeasure = 0, int endMeasureOffset = 0);
    virtual ~xml2guidovisitor() {}
    
    Sguidoelement convert (const Sxmlelement& xml);
    
    // this is to control exact positionning of elements when information is present
    // ie converts relative-x/-y into dx/dy attributes
    void generatePositions (bool state)		{ fGeneratePositions = state; }
    
    /// MARK: Position Helpers

    static void addPosition	 ( Sxmlelement elt, Sguidoelement& tag, float yoffset);
    static void addPosition	 ( Sxmlelement elt, Sguidoelement& tag, float yoffset, float xoffset);
    static void addPosY	( Sxmlelement elt, Sguidoelement& tag, float yoffset, float ymultiplier);
    static void addPosX    ( Sxmlelement elt, Sguidoelement& tag, float xoffset);
    static void addRelativeX(Sxmlelement elt, Sguidoelement& tag, float xoffset);
    static void addPlacement    ( Sxmlelement elt, Sguidoelement& tag);
    static float getYposition    ( Sxmlelement elt, float yoffset, bool useDefault);
    static float getXposition	( Sxmlelement elt, float xoffset);
    
    static void addDirection( Sxmlelement elt, Sguidoelement& tag);
    
    /// MARK: Query methods
    
    /*! Provides Instrument Transposer of the score in Chromatic Scale
     
     The transpose element represents what must be added to the written pitch to get the correct sounding pitch.
     
     See (MusicXML Attribute Description)[http://www.musicxml.com/tutorial/the-midi-compatible-part/attributes/]
     
     \return steps as Int in chromatic steps count
     */
    int  getTransposeInstrumentChromatic ()    ;
    std::string  getTransposeInstrumentName ();
    
    /*!
     Return the number of staves for the first part
     */
    int getStavesForFirstPart();
    
    /*!
     Return the number of staves for the first part
     */
    std::vector<std::string> getAllClefsOfFirstPart();
    
    /*!
     Return the total number of staves
     */
    int getTotalStaves();
    
    std::pair<long, long> getStartPosition() {
        return std::pair<long, long>(fBeginPosition.getNumerator(), fBeginPosition.getDenominator());
    }
    
    std::pair<long, long> getEndPosition() {
        return std::pair<long, long>(fEndPosition.getNumerator(), fEndPosition.getDenominator());
    }
    
    int getTotalMeasures();
    
    /// Returns a map<double,int> where the first are beat positions and second are measure numbers
    std::map<double, int> getMeasureMap();
    
    double getTotalDuration();
    
    /// Returne the number of parts in this XML Score
    int getPartsAvailable();
    
};


} // namespace MusicXML2


#endif
