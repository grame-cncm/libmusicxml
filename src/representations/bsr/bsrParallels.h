/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrParallels___
#define ___bsrParallels___

#include "bsrElements.h"
#include "bsrLines.h"

#include "bsrOah.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP bsrParallel : public bsrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrParallelLayoutKind {
        kParallelLayoutBarOverBarKind,
        kParallelLayoutLineOverLineKind };

    static string parallelLayoutKindAsString (
      bsrParallelLayoutKind parallelLayoutKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrParallel> create (
                            int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrParallel (
                            int inputLineNumber);

    virtual               ~bsrParallel ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getPrintParallelNumber () const
                              { return fPrintParallelNumber; }

    int                   getBrailleParallelNumber () const
                              { return fBrailleParallelNumber; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendLineToParallel (S_bsrLine line)
                              { fParallelElementsList.push_back (line); }

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fPrintParallelNumber;
    int                   fBrailleParallelNumber;

    list<S_bsrElement>
                          fParallelElementsList;
};
typedef SMARTP<bsrParallel> S_bsrParallel;
EXP ostream& operator<< (ostream& os, const S_bsrParallel& elt);


}


#endif
