/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdlCompilerOah___
#define ___msdlCompilerOah___

#include "msr.h"

// JMI #include "multiGeneratorsOah.h"

#include "msrGeneratorsBasicTypes.h"

#include "bsr2brailleOah.h"
#include "bsr2brailleCodeGenerators.h"


namespace MusicXML2
{

//______________________________________________________________________________
#define MSDR_STANDARD_INPUT_NAME ""
//#define MSDR_STANDARD_INPUT_NAME "-"

//______________________________________________________________________________
class EXP msdlCompilerOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlCompilerOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdlCompilerOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlCompilerOahGroup ();

    virtual               ~msdlCompilerOahGroup ();

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

/* JMI
    // generated output kind

    void                  setGeneratorOutputKind (multiGeneratorOutputKind value)
                              { fGeneratorOutputKind = value; }

    multiGeneratorOutputKind
                          getGeneratorOutputKind () const
                              { return fGeneratorOutputKind;  }

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
// JMI    void                  initializeMsdlCompilerTraceOah ();
#endif

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsdlCompilerOahHelp ();

    void                  printMsdlCompilerOahValues (
                            unsigned int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

/* JMI
    // generator output kind

    multiGeneratorOutputKind   fGeneratorOutputKind;
    S_multiGeneratorOutputKindAtom
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
typedef SMARTP<msdlCompilerOahGroup> S_msdlCompilerOahGroup;
EXP ostream& operator<< (ostream& os, const S_msdlCompilerOahGroup& elt);

EXP extern S_msdlCompilerOahGroup gGlobalMsdlCompilerOahGroup;

//______________________________________________________________________________
EXP S_msdlCompilerOahGroup createGlobalMsdlCompilerOahGroup ();


}


#endif
