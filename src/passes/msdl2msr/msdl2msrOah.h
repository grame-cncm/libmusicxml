/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdl2msrOah___
#define ___msdl2msrOah___

#include "msr.h"

#include "generatorsBasicTypes.h"

#include "msrGeneratorsBasicTypes.h"

#include "bsr2brailleOah.h"
#include "bsr2brailleCodeGenerators.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msdl2msrOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2msrOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdl2msrOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2msrOahGroup ();

    virtual               ~msdl2msrOahGroup ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceGroupQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkGroupOptionsConsistency () override;

  public:

    // set and get
    // ------------------------------------------------------

    // fields
    // ------------------------------------------------------

    // generated output kind

    void                  setGeneratorOutputKind (generatorOutputKind value)
                              { fGeneratorOutputKind = value; }

    generatorOutputKind  getGeneratorOutputKind () const
                              { return fGeneratorOutputKind;  }

/* JMI
    // UTF encoding

    void                  setUTFKind (bsrUTFKind value)
                              { fUTFKind = value; }
    bsrUTFKind            getUTFKind () const
                              { return fUTFKind;  }

    // byte ordering

    void                  setByteOrderingKind (bsrByteOrderingKind value)
                              { fByteOrderingKind = value; }
    bsrByteOrderingKind   getByteOrderingKind () const
                              { return fByteOrderingKind;  }

    // braille music file name

    void                  setUseEncodingInFileName ()
                              { fUseEncodingInFileName = true; }
    bool                  getUseEncodingInFileName () const
                              { return fUseEncodingInFileName;  }

    // page parameters

    void                  setCellsPerLine (unsigned int value)
                              { fCellsPerLine = value; }
    unsigned int          getCellsPerLine () const
                              { return fCellsPerLine;  }

    void                  setMeasuresPerLine (unsigned int value)
                              { fMeasuresPerLine = value; }
    unsigned int          getMeasuresPerLine () const
                              { return fMeasuresPerLine;  }

    void                  setLinesPerPage (unsigned int value)
                              { fLinesPerPage = value; }
    unsigned int          getLinesPerPage () const
                              { return fLinesPerPage;  }

    // code generation

    void                  setXml2brlInfos ()
                              { fXml2brlInfos = true; }
    bool                  getXml2brlInfos () const
                              { return fXml2brlInfos;  }

    void                  setNoBrailleCode ()
                              { fNoBrailleCode = true; }
    bool                  getNoBrailleCode () const
                              { return fNoBrailleCode;  }
*/

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
// JMI    void                  initializeMsdl2msrTraceOah ();
#endif

    void                  initializeGenerateCodeOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsdl2msrOahHelp ();

    void                  printMsdl2msrOahValues (
                            unsigned int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // generator output kind

    generatorOutputKind   fGeneratorOutputKind;
    S_generatorOutputKindAtom
                          fGeneratorOutputKindAtom;

    // UTF encoding (8 or 16 bits)

    bsrUTFKind            fUTFKind;
    S_brailleUTFKindAtom  fUTFKindAtom;

    // braille output kind

    bsrBrailleOutputKind  fBrailleOutputKind;
    S_brailleOutputKindAtom
                          fBrailleOutputKindAtom;

    // byte ordering

    bsrByteOrderingKind   fByteOrderingKind;
    S_brailleByteOrderingKindAtom
                          fByteOrderingKindAtom;

/* JMI
    // braille music file name

    bool                  fUseEncodingInFileName;

    // page parameters

    unsigned int          fCellsPerLine;
    unsigned int          fMeasuresPerLine;
    unsigned int          fLinesPerPage;

    // code generation

    bool                  fXml2brlInfos;

    bool                  fNoBrailleCode;
    */
};
typedef SMARTP<msdl2msrOahGroup> S_msdl2msrOahGroup;
EXP ostream& operator<< (ostream& os, const S_msdl2msrOahGroup& elt);

EXP extern S_msdl2msrOahGroup gGlobalMsdl2msrOahGroup;

//______________________________________________________________________________
EXP S_msdl2msrOahGroup createGlobalMsdl2msrOahGroup ();


}


#endif
