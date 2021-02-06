/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsrScheme___
#define ___lpsrScheme___

#include <string>

#include "lpsrElements.h"

#include "lpsrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
class EXP lpsrSchemeVariable : public lpsrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum lpsrCommentedKind {
      kCommentedYes, kCommentedNo
    };

    static string commentedKindAsString (
      lpsrCommentedKind commentedKind);

    enum lpsrEndlKind {
      kEndlNone, kEndlOnce, kEndlTwice
    };

    static string endlKindAsString (
      lpsrEndlKind endlKind);

    static string const g_SchemeVariableNoUnit;
    static string const g_SchemeVariableNoComment;

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrSchemeVariable> create (
                            int               inputLineNumber,
                            lpsrCommentedKind commentedKind,
                            string            variableName,
                            string            value,
                            string            comment,
                            lpsrEndlKind      endlKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrSchemeVariable (
                            int               inputLineNumber,
                            lpsrCommentedKind commentedKind,
                            string            variableName,
                            string            value,
                            string            comment,
                            lpsrEndlKind      endlKind);

    virtual               ~lpsrSchemeVariable ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getVariableName  () const { return fVariableName; }

    void                  setVariableValue (string value)
                              { fVariableValue = value; }

    string                getVariableValue () const { return fVariableValue; }

    lpsrCommentedKind     getCommentedKind () const { return fCommentedKind; }

    string                getComment  () const
                              { return fComment; }

    lpsrEndlKind          getEndlKind () const
                              { return fEndlKind; }

  public:

    // public services
    // ------------------------------------------------------

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

    lpsrCommentedKind fCommentedKind;

    string                fVariableName;
    string                fVariableValue;

    string                fComment;

    lpsrEndlKind          fEndlKind;

};
typedef SMARTP<lpsrSchemeVariable> S_lpsrSchemeVariable;
EXP ostream& operator<< (ostream& os, const S_lpsrSchemeVariable& schemeVariable);

//______________________________________________________________________________
class EXP lpsrSchemeFunction : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrSchemeFunction> create (
                            int    inputLineNumber,
                            string functionName,
                            string functionDescription,
                            string functionCode);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrSchemeFunction (
                            int    inputLineNumber,
                            string functionName,
                            string functionDescription,
                            string functionCode);

    virtual               ~lpsrSchemeFunction ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getFunctionName () const
                              { return fFunctionName; }

    string                getFunctionDescription () const
                              { return fFunctionDescription; }

    string                getFunctionCode () const
                              { return fFunctionCode; }

  public:

    // public services
    // ------------------------------------------------------

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

    string            fFunctionName;

    string            fFunctionDescription;

    string            fFunctionCode;
};
typedef SMARTP<lpsrSchemeFunction> S_lpsrSchemeFunction;
EXP ostream& operator<< (ostream& os, const S_lpsrSchemeFunction& schemeFunction);


}


#endif
