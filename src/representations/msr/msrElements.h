/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrElements___
#define ___msrElements___

#include "typedefs.h"
#include "tree_browser.h"
#include "rational.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msrElement : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    // cloning
    // ------------------------------------------------------

  protected:

    msrElement (
      int inputLineNumber);

    virtual ~msrElement ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getInputLineNumber () const
                              { return fInputLineNumber; }

  public:

    // public services
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

    virtual void          printShort (ostream& os) const;

    virtual void          printSummary (ostream& os) const {}

  protected:

    // protected fields
    // ------------------------------------------------------

    int                   fInputLineNumber;
};
typedef SMARTP<msrElement> S_msrElement;
EXP ostream& operator<< (ostream& os, const S_msrElement& elt);

//______________________________________________________________________________
template <typename T> class EXP msrBrowser : public browser <T>
{
  public:

    msrBrowser (basevisitor* v) : fVisitor (v) {}

    virtual ~msrBrowser () {}

  public:

    virtual void set (basevisitor* v) { fVisitor = v; }

    virtual void browse (T& t) {
      enter (t);

      t.browseData (fVisitor);

      leave (t);
    }

  protected:

    basevisitor*  fVisitor;

    virtual void enter (T& t) { t.acceptIn  (fVisitor); }
    virtual void leave (T& t) { t.acceptOut (fVisitor); }
};


} // namespace MusicXML2


#endif
