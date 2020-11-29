/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrPartNamesDisplays___
#define ___msrPartNamesDisplays___

#include "msrSegnosAndCodas.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrPartNameDisplay : public msrMeasureElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPartNameDisplay> create (
      int    inputLineNumber,
      string partNameDisplayValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrPartNameDisplay (
      int    inputLineNumber,
      string partNameDisplayValue);
      
    virtual ~msrPartNameDisplay ();
  
  public:

    // set and get
    // ------------------------------------------------------

    string                getPartNameDisplayValue () const
                              { return fPartNameDisplayValue; }
                        
  public:

    // public services
    // ------------------------------------------------------

    string                asString () const override;
         
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    string                fPartNameDisplayValue;
};
typedef SMARTP<msrPartNameDisplay> S_msrPartNameDisplay;
EXP ostream& operator<< (ostream& os, const S_msrPartNameDisplay& elt);

//______________________________________________________________________________
class msrPartAbbreviationDisplay : public msrMeasureElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPartAbbreviationDisplay> create (
      int    inputLineNumber,
      string partAbbreviationDisplayValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrPartAbbreviationDisplay (
      int    inputLineNumber,
      string partAbbreviationDisplayValue);
      
    virtual ~msrPartAbbreviationDisplay ();
  
  public:

    // set and get
    // ------------------------------------------------------

    string                getPartAbbreviationDisplayValue () const
                              { return fPartAbbreviationDisplayValue; }
                        
  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;
         
    virtual void          print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    string                fPartAbbreviationDisplayValue;
};
typedef SMARTP<msrPartAbbreviationDisplay> S_msrPartAbbreviationDisplay;
EXP ostream& operator<< (ostream& os, const S_msrPartAbbreviationDisplay& elt);


} // namespace MusicXML2


#endif
