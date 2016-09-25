/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __xml2lpsrvisitor__
#define __xml2lpsrvisitor__

#include <ostream>
#include <stack>
#include <map>
#include <string>

#include "exports.h"
#include "lpsr.h"
#include "typedefs.h"
#include "visitor.h"
#include "xml.h"

#include "musicxml2lpsr.h"

namespace MusicXML2 
{

/*!
\ingroup visitors lpsr
@{
*/


/*!
\brief A score visitor to produce a LilyPond representation.
*/
//______________________________________________________________________________
class EXP xml2lpsrvisitor : 
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
  public:
    
    xml2lpsrvisitor( translationSwitches& ts );
    virtual ~xml2lpsrvisitor() {}

    SlpsrElement convertToLilyPond (const Sxmlelement& xml);

    // this is to control exact positionning of elements 
    // when information is present
    // ie converts relative-x/-y into dx/dy attributes
    void generatePositions (bool state) { fSwitches.fGeneratePositions = state; }

    static void addPosition ( 
      Sxmlelement elt, SlpsrElement& cmd, int yoffset);
  
  protected:

    virtual void visitStart( S_score_partwise& elt);
    virtual void visitEnd  ( S_score_partwise& elt);
    
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

  private:
  
    // the translation switches
    translationSwitches fSwitches;
    
    // the implicit sequence containing all the generated code
    SlpsrSequence   fLpsrseq; 
    
    // the header, paper and layout blocks to be generated
    SlpsrHeader     flpsrHeader;
    SlpsrPaper      fLpsrpaper;
    SlpsrLayout     fLpsrlayout;
    
    // the parts
    lpsrPartsmap    fLpsrpartsMap;
    
    // the score block to be generated
    SlpsrScore      fLpsrscore;
    
    std::string         fCurrentPartID;
   
    // the index of the current lpsr staff
    int                 fCurrentStaffIndex;
  
    void addElementToSequence (SlpsrElement& elt);
    
    void addPreamble ();
    void addPostamble ();
};


} // namespace MusicXML2


#endif
