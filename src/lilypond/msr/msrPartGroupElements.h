/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrPartGroupElements___
#define ___msrPartGroupElements___

#include "msrSegnosAndCodas.h"


namespace MusicXML2 
{

//______________________________________________________________________________
/*
  PartGroup elements, i.e. notes, chords and tuplets, can also be found in measures, hence:
*/
class msrPartGroupElement : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    // cloning
    // ------------------------------------------------------

  protected:

    msrPartGroupElement (
      int inputLineNumber);

    virtual ~msrPartGroupElement ();

  public:

/* JMI
    // set and get
    // ------------------------------------------------------

    void                  setPartGroupNumber (
                            string positionInPartGroup)
                              {
                                fPartGroupNumber =
                                  positionInPartGroup;
                              }
                      
    string                getPartGroupNumber ()
                              { return fPartGroupNumber; }

    void                  setPositionInPartGroup (
                            rational positionInPartGroup)
                              {
                                fPositionInPartGroup =
                                  positionInPartGroup;
                              }
                      
    rational              getPositionInPartGroup ()
                              { return fPositionInPartGroup; }

    // services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v) = 0;

  public:

    // print
    // ------------------------------------------------------

    virtual std::string   asShortString () const;
    virtual std::string   asString () const;    

    virtual void          print (ostream& os) const;

    virtual void          printSummary (ostream& os) {}
    
  protected:
     
    // fields
    // ------------------------------------------------------

    string                fPartGroupNumber;
    rational              fPositionInPartGroup;
    */
};
typedef SMARTP<msrPartGroupElement> S_msrPartGroupElement;
EXP ostream& operator<< (ostream& os, const S_msrPartGroupElement& elt);


} // namespace MusicXML2


#endif
