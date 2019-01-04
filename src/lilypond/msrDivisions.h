/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrDivisions___
#define ___msrDivisions___

#include <list>

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrDivisions : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDivisions> create (
      int inputLineNumber,
      int divisionsPerQuarterNote);

    SMARTP<msrDivisions> createDivisionsNewbornClone (); // JMI

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeDivisions ();
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrDivisions (
      int inputLineNumber,
      int divisionsPerQuarterNote);
      
    virtual ~msrDivisions ();
  
  public:

    // set and get
    // ------------------------------------------------------

    void                  setDivisionsPerQuarterNote ( // JMI
                            int divisionsPerQuarterNote)
                              {
                                fDivisionsPerQuarterNote =
                                  divisionsPerQuarterNote;
                              }
                              
    int                   getDivisionsPerQuarterNote () const
                              { return fDivisionsPerQuarterNote; }
                        
    // services
    // ------------------------------------------------------

    string                divisionsAsString () const;

    // durations
    int                   durationKindAsDivisions (
                            int             inputLineNumber,
                            msrDurationKind durationKind);

    void                  printDurationKindsDivisions (ostream& os);

    // MSR strings
    string                divisionsAsMsrString (
                            int  inputLineNumber,
                            int  divisions,
                            int& numberOfDotsNeeded);

    string                divisionsAsMsrString (
                            int  inputLineNumber,
                            int  divisions);
/* JMI
    string                wholeNotesAsMsrString (
                            int      inputLineNumber,
                            rational wholeNotes,
                            int&     numberOfDotsNeeded);

    string                wholeNotesAsMsrString (
                            int      inputLineNumber,
                            rational wholeNotes);
*/
    string                tupletDivisionsAsMsrString (
                            int inputLineNumber,
                            int divisions,
                            int actualNotes,
                            int normalNotes);
                            
    string                tupletWholeNotesAsMsrString (
                            int      inputLineNumber,
                            rational wholeNotes,
                            int      actualNotes,
                            int      normalNotes);

/* JMI
    void                  testDivisionsAndDurations (); // JMI
    void                  testTupletSoundingWholeNotesAndDurations ();
*/

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    int                   fDivisionsPerQuarterNote;
    
    list<pair<msrDurationKind, int> >
                          fDurationKindsToDivisions;
};
typedef SMARTP<msrDivisions> S_msrDivisions;
EXP ostream& operator<< (ostream& os, const S_msrDivisions& elt);


} // namespace MusicXML2


#endif
