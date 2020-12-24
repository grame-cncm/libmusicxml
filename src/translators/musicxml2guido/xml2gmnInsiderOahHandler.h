/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2gmnInsiderOahGroup___
#define ___xml2gmnInsiderOahGroup___

#include "oahBasicTypes.h"


namespace MusicXML2
{
//_______________________________________________________________________________
#define K_REGULAR_OPTION_NAME "regular"
#define K_INSIDER_OPTION_NAME "insider"

//_______________________________________________________________________________
EXP string xml2gmnAboutInformation ();

//_______________________________________________________________________________
class EXP xml2gmnInsiderOahHandler : public oahHandler
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2gmnInsiderOahHandler> create (
      string executableName,
      string executableAboutInformation,
      string handlerHeader);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2gmnInsiderOahHandler (
      string executableName,
      string executableAboutInformation,
      string handlerHeader);

    virtual ~xml2gmnInsiderOahHandler ();

  private:

    // private initialization
    // ------------------------------------------------------

    void                  createTheXml2gmnPrefixes ();

    void                  createTheXml2gmnOptionGroups (
                            string executableName);

  public:

    // public services
    // ------------------------------------------------------

    string                fetchOutputFileNameFromTheOptions () const override;

    // quiet mode
    void                  enforceHandlerQuietness () override;

    // consistency check
    virtual void          checkHandlerOptionsConsistency () override;

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
typedef SMARTP<xml2gmnInsiderOahHandler> S_xml2gmnInsiderOahHandler;
EXP ostream& operator<< (ostream& os, const S_xml2gmnInsiderOahHandler& elt);

//______________________________________________________________________________
class EXP xml2gmnInsiderOahGroup : public oahGroup
{
  public:

    static SMARTP<xml2gmnInsiderOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2gmnInsiderOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2gmnInsiderOahGroup ();

    virtual ~xml2gmnInsiderOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // output file
    S_oahBooleanAtom      getAutoOutputFileNameAtom () const
                              { return fAutoOutputFileNameAtom; }

    S_oahStringAtom       getOutputFileNameStringAtom () const
                              { return fOutputFileNameStringAtom; }

    // quit after some passes
    bool                  getQuit2a () const
                              { return fQuitAfterPass2a; }
    bool                  getQuit2b () const
                              { return fQuitAfterPass2b; }

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    virtual void          checkGroupOptionsConsistency () override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printXml2gmnInsiderOahGroupHelp ();

    void                  printXml2gmnInsiderOahGroupValues (int fieldWidth);

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
typedef SMARTP<xml2gmnInsiderOahGroup> S_xml2gmnInsiderOahGroup;
EXP ostream& operator<< (ostream& os, const S_xml2gmnInsiderOahGroup& elt);

EXP extern S_xml2gmnInsiderOahGroup gGlobalXml2gmnInsiderOahGroup;

//______________________________________________________________________________
S_xml2gmnInsiderOahGroup createGlobalXml2gmnOahGroup ();


}


#endif
