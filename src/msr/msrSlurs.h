/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrSlurs___
#define ___msrSlurs___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msrSlur : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrSlurTypeKind {
      k_NoSlur,
      kRegularSlurStart, kPhrasingSlurStart,
      kSlurContinue,
      kRegularSlurStop, kPhrasingSlurStop};

    static string slurTypeKindAsString (
      msrSlurTypeKind slurKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSlur> create (
      int             inputLineNumber,
      int             slurNumber,
      msrSlurTypeKind slurKind,
      msrLineTypeKind slurLineTypeKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSlur (
      int             inputLineNumber,
      int             slurNumber,
      msrSlurTypeKind slurKind,
      msrLineTypeKind slurLineTypeKind);

    virtual ~msrSlur ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getSlurNumber () const
                              { return fSlurNumber; }

    void                  setSlurTypeKind (msrSlurTypeKind slurTypeKind)
                              { fSlurTypeKind = slurTypeKind; }

    msrSlurTypeKind       getSlurTypeKind () const
                              { return fSlurTypeKind; }

    msrLineTypeKind       getSlurLineTypeKind () const
                              { return fSlurLineTypeKind; }

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

    string                slurTypeKindAsString () const;

    string                asString () const override;

    virtual void          print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fSlurNumber;

    msrSlurTypeKind       fSlurTypeKind;

    msrLineTypeKind       fSlurLineTypeKind;
};
typedef SMARTP<msrSlur> S_msrSlur;
EXP ostream& operator<< (ostream& os, const S_msrSlur& elt);


} // namespace MusicXML2


#endif
