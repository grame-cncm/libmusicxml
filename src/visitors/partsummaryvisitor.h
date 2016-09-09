/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __partsummary__
#define __partsummary__

#include <map>
#include <vector>

#include "notevisitor.h"
#include "smartlist.h"

namespace MusicXML2
{

/*!
\addtogroup visitors
@{
*/

/*!
\brief Produces a summary of a MusicXML part.

  A part summary consists in 
  - a count of staves
  - a map that associates each stave with the corresponding number of notes
  - a map that associates each stave with the corresponding voices and notes
  - a map that associates each stave with the correcponding stanzas
*/
class EXP partsummaryvisitor : 
  public visitor<S_part>,
  public visitor<S_staves>,
  public visitor<S_staff>,
  public visitor<S_voice>,
  public visitor<S_note>,
  public visitor<S_lyric>,
  public visitor<S_syllabic>,
  public visitor<S_text>
{
  public:
  
    partsummaryvisitor() : fStavesCount(1) {};
    virtual ~partsummaryvisitor() {};
    
    //! returns the number of staves for the part
    int countStaves () const        { return fStavesCount; }
    //! returns the number of voices 
    int countVoices () const        { return fVoicesNotesCount.size(); }
    //! returns the number of voices on a staff 
    int countVoices (int staff) const;

    //! returns the staff ids list
    smartlist<int>::ptr getStaves() const;
    //! returns the staff ids list for one voice
    smartlist<int>::ptr getStaves (int voice) const;
    //! returns the count of notes on a staff
    int getStaffNotesCount (int id) const;

    //! returns the voices ids list
    smartlist<int>::ptr getVoices () const;
    //! returns the voices ids list for one staff
    smartlist<int>::ptr getVoices (int staff) const;
    //! returns the id of the staff that contains the more of the voice notes
    int getMainStaff (int voiceid) const;
    //! returns the count of notes on a voice
    int getVoiceNotesCount (int voiceid) const;
    //! returns the count of notes on a voice and a staff
    int getVoiceNotesCount (int staffid, int voiceid) const;
    
    // a stanza is represented as a list words,
    // represented as a list of their components
    typedef std::list<std::list<std::string> > stanzaContents;

    virtual std::map<std::string, partsummaryvisitor::stanzaContents>& 
                  getStanzas();
    void          clearStanzas ();
    virtual std::string 
                  getStanza (std::string name, std::string separator) const;
                                       
  protected:
  
    virtual void visitStart ( S_part& elt);
    virtual void visitStart ( S_staves& elt);
    virtual void visitStart ( S_staff& elt)      { fStaff = int(*elt); }
    virtual void visitStart ( S_voice& elt )     { fVoice = int(*elt); }
    virtual void visitEnd   ( S_note& elt);

    virtual void visitStart ( S_lyric& elt);
    virtual void visitStart ( S_syllabic& elt);
    virtual void visitEnd   ( S_text& elt );
    
  private:
  
    // count of staves (from the staves element)
    int                 fStavesCount;
    // staves and corresponding count of notes
    std::map<int, int>  fStavesNotesCount;
    // voices and corresponding count of notes
    std::map<int, int>  fVoicesNotesCount;
    // staves and corresponding voices + count of notes
    std::map<int, std::map<int, int> >  fStaffVoices;

    int                 fStaff, fVoice;

    // the last lyric number, i.e. stanza number
    S_lyric             fLastLyric;
    // the last sysllabic spec met (single, begin, middle or end)
    S_syllabic          fLastSyllabic; 
    
    // the stanzas are referred to by number and contains list of lists of strings
    // in the case of "single", the list contains only one string
    std::map<std::string, stanzaContents> 
                        fStanzas;    // <text /> occurs after <syllabic />
};

/*! @} */

}

#endif
