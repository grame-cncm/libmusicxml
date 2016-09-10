/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __xml2lilypondvisitor__
#define __xml2lilypondvisitor__

#include <ostream>
#include <stack>
#include <map>
#include <string>

#include "exports.h"
#include "lilypond.h"
#include "typedefs.h"
#include "visitor.h"
#include "xml.h"

#include "musicxml2lilypond.h"

namespace MusicXML2 
{

/*!
\ingroup visitors lilypond
@{
*/

//______________________________________________________________________________
typedef struct {
  S_score_partwise        fScorePartwise; // may contain MusicXML version
  S_work_number           fWorkNumber;
  S_work_title            fWorkTitle;
  S_movement_number       fMovementNumber;
  S_movement_title        fMovementTitle;
  std::vector<S_creator>  fCreators;
  S_rights                fRights;
  std::vector<S_software> fSoftwares;
  S_encoding_date         fEncodingDate;
  S_score_instrument      fScoreInstrument;
} globalHeader;

typedef struct {
  S_part_name             fPartName;
} partHeader;
typedef std::map<std::string, partHeader> partHeaderMap;

/*!
\brief A score visitor to produce a LilyPond representation.
*/
//______________________________________________________________________________
class EXP xml2lilypondvisitor : 
  public visitor<S_score_partwise>,
  public visitor<S_work_number>,
  public visitor<S_work_title>,
  public visitor<S_movement_number>,
  public visitor<S_movement_title>,
  public visitor<S_creator>,
  public visitor<S_rights>,
  public visitor<S_software>,
  public visitor<S_encoding_date>,
  public visitor<S_instrument_name>,
  public visitor<S_score_part>,
  public visitor<S_part_name>,
  public visitor<S_part>
{
  private:
  
    // the translation switches
    translationSwitches fSwitches;
    
    // the lilypond elements stack
    std::stack<Slilypondelement>  fScoreStack;
     
    globalHeader  fGlobalHeader; // musicxml header elements (should be flushed at the beginning of the generated code)
    bool          fGlobalHeaderHasBeenFlushed;
    
    partHeaderMap fPartHeader;   // musicxml score-part elements (should be flushed at the beginning of each part)
    
    std::string   fCurrentPartID;
    int           fCurrentStaffIndex;   // the index of the current lilypond staff
  
    void startScoreStack   (Slilypondelement& elt);
    void addToScoreStack   (Slilypondelement& elt);
    void pushToScoreStack  (Slilypondelement& elt);
    void popFromScoreStack ();
  
    void flushGlobalHeader ( globalHeader& header );
    void flushPartHeader   ( partHeader& header );
  
  protected:

    virtual void visitStart( S_score_partwise& elt);
    virtual void visitStart( S_work_number& elt);
    virtual void visitStart( S_work_title& elt);
    virtual void visitStart( S_movement_number& elt);
    virtual void visitStart( S_movement_title& elt);
    virtual void visitStart( S_creator& elt);
    virtual void visitStart( S_rights& elt);
    virtual void visitStart( S_software& elt);
    virtual void visitStart( S_encoding_date& elt);
    virtual void visitStart( S_instrument_name& elt);
    virtual void visitStart( S_score_part& elt);
    virtual void visitStart( S_part_name& elt);
    virtual void visitStart( S_part& elt);

    Slilypondelement& scoreStackTop ();

  public:
    
    xml2lilypondvisitor( translationSwitches& sw );
    virtual ~xml2lilypondvisitor() {}

    Slilypondelement convert (const Sxmlelement& xml);

    // this is to control exact positionning of elements 
    // when information is present
    // ie converts relative-x/-y into dx/dy attributes
    void generatePositions (bool state) { fSwitches.fGeneratePositions = state; }

    static void addPosition ( 
      Sxmlelement elt, Slilypondelement& cmd, int yoffset);
};


} // namespace MusicXML2


#endif
