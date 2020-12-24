/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsrElements___
#define ___lpsrElements___

#include "msrElements.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP lpsrElement : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrElement> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrElement (
      int inputLineNumber);

    virtual ~lpsrElement ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;
};
typedef SMARTP<lpsrElement> S_lpsrElement;

//______________________________________________________________________________
template <typename T> class EXP lpsrBrowser : public browser<T>
{
  protected:

    basevisitor*  fVisitor;

    virtual void enter (T& t) { t.acceptIn  (fVisitor); }
    virtual void leave (T& t) { t.acceptOut (fVisitor); }

  public:

    lpsrBrowser (basevisitor* v) : fVisitor (v)
    {}

    virtual ~lpsrBrowser ()
    {}

    virtual void          set (basevisitor* v)
                            { fVisitor = v; }

    virtual               void browse (T& t)
                            {
                              enter (t);

                              t.browseData (fVisitor);

                              leave (t);
                            }
};


}


#endif
