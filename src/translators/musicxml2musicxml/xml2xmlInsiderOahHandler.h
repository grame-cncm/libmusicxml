/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2xmlInsiderOahGroup___
#define ___xml2xmlInsiderOahGroup___

#include "oahBasicTypes.h"


namespace MusicXML2
{
//_______________________________________________________________________________
#define K_REGULAR_OPTION_NAME "regular"
#define K_INSIDER_OPTION_NAME "insider"

//_______________________________________________________________________________
EXP string xml2xmlAboutInformation ();

//_______________________________________________________________________________
class EXP xml2xmlInsiderOahHandler : public oahHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2xmlInsiderOahHandler> create (
      string executableName,
      string executableAboutInformation,
      string handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2xmlInsiderOahHandler (
      string executableName,
      string executableAboutInformation,
      string handlerHeader);

    virtual ~xml2xmlInsiderOahHandler ();

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheXml2xmlPrefixes ();

    void                  createTheXml2xmlOptionGroups (
                            string executableName);

  public:

    // public services
    // ------------------------------------------------------

    string                fetchOutputFileNameFromTheOptions () const override;

    // quiet mode
    void                  enforceHandlerQuietness () override;

    // consistency check
    void                  checkHandlerOptionsConsistency () override;

  public:

    // visitors
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    S_oahPrefix           fShortTracePrefix;
    S_oahPrefix           fLongTracePrefix;

    S_oahPrefix           fShortIgnoreRedundantPrefix;
    S_oahPrefix           fLongIgnoreRedundantPrefix;

    S_oahPrefix           fShortDelayRestsPrefix;
    S_oahPrefix           fLongDelayRestsPrefix;
};
typedef SMARTP<xml2xmlInsiderOahHandler> S_xml2xmlInsiderOahHandler;
EXP ostream& operator<< (ostream& os, const S_xml2xmlInsiderOahHandler& elt);

//______________________________________________________________________________
class EXP xml2xmlInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<xml2xmlInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2xmlInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2xmlInsiderOahGroup ();

    virtual ~xml2xmlInsiderOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // output file
    S_oahBooleanAtom      getAutoOutputFileNameAtom () const
                              { return fAutoOutputFileNameAtom; }

    S_oahStringAtom       getOutputFileNameStringAtom () const
                              { return fOutputFileNameStringAtom; }

    // quit after some passes
    bool                  getQuitAfterPass2a () const
                              { return fQuitAfterPass2a; }
    bool                  getQuitAfterPass2b () const
                              { return fQuitAfterPass2b; }

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printXml2xmlInsiderOahGroupHelp ();

    void                  printXml2xmlInsiderOahGroupValues (unsigned int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createInsiderOutputSubGroup ();

    void                  createInsiderQuitSubGroup ();

  private:

    // private fields
    // ------------------------------------------------------

    // output file
    // we store the atoms for the needs of fetchOutputFileNameFromTheOptions()
    S_oahBooleanAtom      fAutoOutputFileNameAtom;

    S_oahStringAtom       fOutputFileNameStringAtom;

    // quit after some passes
    bool                  fQuitAfterPass2a;
    bool                  fQuitAfterPass2b;

    // MusicXML output file name
    // these private variables are the ones accessible through the atoms
    bool                  fAutoOutputFileName;

    string                fOutputFileName;
};
typedef SMARTP<xml2xmlInsiderOahGroup> S_xml2xmlInsiderOahGroup;
EXP ostream& operator<< (ostream& os, const S_xml2xmlInsiderOahGroup& elt);

EXP extern S_xml2xmlInsiderOahGroup gGlobalXml2xmlInsiderOahGroup;

//______________________________________________________________________________
S_xml2xmlInsiderOahGroup createGlobalXml2xmlOahGroup ();


}


#endif
