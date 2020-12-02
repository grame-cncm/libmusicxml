/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___oahElements___
#define ___oahElements___

#include "browser.h"


using namespace std;

namespace MusicXML2
{
// layout settings
//______________________________________________________________________________
const int K_OAH_ELEMENTS_INDENTER_OFFSET = 3;
  // indent a bit more for readability

const int K_OAH_FIELD_WIDTH = 40;

// data types
// ------------------------------------------------------

enum oahElementKind {
  kElementWithoutValue,        // i.e. -cpu
  kElementWithMandatoryValue,  // i.e. -global-staff-size 30
  kElementWithOptionalValue }; // i.e. -name-help, -name-help=cpu

string elementKindAsString (
  oahElementKind elementKind);

enum oahElementVisibilityKind {
  kElementVisibilityNone, // default value
  kElementVisibilityWhole,
  kElementVisibilityHeaderOnly,
  kElementVisibilityHidden };

string elementVisibilityKindAsString (
  oahElementVisibilityKind elementVisibilityKind);

enum EXP oahElementHelpOnlyKind {
    kElementHelpOnlyYes,
    kElementHelpOnlyNo };

string elementHelpOnlyKindAsString (
  oahElementHelpOnlyKind elementHelpOnlyKind);

//______________________________________________________________________________
// PRE-declaration for class self dependency
class oahElement;
typedef SMARTP<oahElement> S_oahElement;

class oahElement : public smartable
{
  public:

/* JMI
    // creation
    // ------------------------------------------------------

    static SMARTP<oahElement> create (
      string                   shortName,
      string                   longName,
      string                   description,
      oahElementKind           elementKind,
      oahElementVisibilityKind elementVisibilityKind);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahElement (
      string                   shortName,
      string                   longName,
      string                   description,
      oahElementKind           elementKind,
      oahElementVisibilityKind elementVisibilityKind);

    virtual ~oahElement ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getShortName () const
                              { return fShortName; }

    string                getLongName () const
                              { return fLongName; }

    string                getDescription () const
                              { return fDescription; }

    void                  setElementKind (
                            oahElementKind elementKind)
                              { fElementKind = elementKind; }

    oahElementKind        getElementKind () const
                              { return fElementKind; }

    oahElementHelpOnlyKind
                          getElementHelpOnlyKind () const
                              { return fElementHelpOnlyKind; }

    void                  setElementVisibilityKind (
                            oahElementVisibilityKind elementVisibilityKind)
                              { fElementVisibilityKind = elementVisibilityKind; }

    oahElementVisibilityKind
                          getElementVisibilityKind () const
                              { return fElementVisibilityKind; }

    void                  setMultipleOccurrencesAllowed ()
                              { fMultipleOccurrencesAllowed = true; }

    bool                  getMultipleOccurrencesAllowed () const
                              { return fMultipleOccurrencesAllowed; }

  public:

    // public services
    // ------------------------------------------------------

    string                fetchNames () const;
    string                fetchNamesInColumns (
                            int subGroupsShortNameFieldWidth) const;

    string                fetchNamesBetweenQuotes () const;

    string                fetchNamesBetweenParentheses () const;
    string                fetchNamesInColumnsBetweenParentheses (
                            int subGroupsShortNameFieldWidth) const;

    string                fetchMostSignificantName () const
                              {
                                return
                                  fLongName.size ()
                                    ? fLongName
                                    : fShortName;
                              }

    bool                  nameIsANameForElement (
                            string name)
                              {
                                return
                                  name == fShortName
                                    ||
                                  name == fLongName;
                              }

    virtual int           fetchVariableNameLength () const
                              { return 0; }

    S_oahElement          thisElementIfItHasName (
                            string name);

    virtual void          applyElement (ostream& os) = 0;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual string        asShortNamedOptionString () const;
    virtual string        asActualLongNamedOptionString () const;

    string                asLongNamedOptionString () const;

    string                asString () const;

    virtual void          printOptionHeader (ostream& os) const;

    virtual void          printOahElementEssentials (
                            ostream& os,
                            int      fieldWidth) const;
    virtual void          printOahElementEssentialsShort (
                            ostream& os,
                            int      fieldWidth) const;

    virtual void          print (ostream& os) const;
    virtual void          printShort (ostream& os) const;

    virtual void          printHelp (ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    string                fShortName;
    string                fLongName;
    string                fDescription;

    oahElementKind        fElementKind;

    oahElementHelpOnlyKind
                          fElementHelpOnlyKind;

    oahElementVisibilityKind
                          fElementVisibilityKind;

    bool                  fMultipleOccurrencesAllowed;
};
typedef SMARTP<oahElement> S_oahElement;
EXP ostream& operator<< (ostream& os, const S_oahElement& elt);

//______________________________________________________________________________
/*
Because the set needs a comparison functor to work with. If you don't specify one, it will make a default-constructed one. In this case, since you're using a function-pointer type, the default-constructed one will be a null pointer, which can't be called; so instead, you have to provide the correct function pointer at run time.

A better approach might be to use a function class type (a.k.a. functor type); then the function call can be resolved at compile time, and a default-constructed object will do the right thing.
*/

struct compareOahElements {
  bool operator() (
    const S_oahElement firstElement,
    const S_oahElement secondElement) const;
};

//______________________________________________________________________________
class oahElementUse : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<oahElementUse> create (
      S_oahElement elementUsed,
      string       nameUsed,
      string       valueUsed);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahElementUse (
      S_oahElement elementUsed,
      string       nameUsed,
      string       valueUsed);

    virtual ~oahElementUse ();

  public:

    // set and get
    // ------------------------------------------------------

    S_oahElement          getElementUsed () const
                              { return fElementUsed; }

    string                getNameUsed () const
                              { return fNameUsed; }

    void                  setValueUsed (string value)
                              { fValueUsed = value; }

    string                getValueUsed () const
                              { return fValueUsed; }

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    S_oahElement          fElementUsed;

    string                fNameUsed;
    string                fValueUsed;
};
typedef SMARTP<oahElementUse> S_oahElementUse;
EXP ostream& operator<< (ostream& os, const S_oahElementUse& elt);

/* JMI
//______________________________________________________________________________
template <typename T> class oahBrowser : public browser<T>
{
  protected:

    basevisitor*  fVisitor;

    virtual void enter (T& t) { t.acceptIn  (fVisitor); }
    virtual void leave (T& t) { t.acceptOut (fVisitor); }

  public:

    oahBrowser (basevisitor* v) : fVisitor (v)
    {}

    virtual ~oahBrowser ()
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
*/

template <typename T> class oahBrowser : public browser <T>
{
  public:

    oahBrowser (basevisitor* v) : fVisitor (v) {}

    virtual ~oahBrowser () {}

  public:

    virtual void set (basevisitor* v) { fVisitor = v; }

    virtual void browse (T& t) {
/* JMI
#ifdef TRACING_IS_ENABLED
      if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
        cout <<
          endl <<
          ".\\\" --> browse()" <<
          endl;
      }
#endif
*/

      enter (t);

      t.browseData (fVisitor);

      leave (t);
    }

  protected:

    basevisitor*  fVisitor;

    virtual void enter (T& t) {
/* JMI
#ifdef TRACING_IS_ENABLED
      if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
        cout <<
          endl <<
          ".\\\" --> enter()" <<
          endl;
      }
#endif
*/

      t.acceptIn  (fVisitor);
    }
    virtual void leave (T& t) {
/* JMI
#ifdef TRACING_IS_ENABLED
      if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
        cout <<
          endl <<
          ".\\\" --> leave()" <<
          endl;
      }
#endif
*/

      t.acceptOut (fVisitor);
    }
};


}


#endif
