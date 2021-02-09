/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdrTokensList___
#define ___msdrTokensList___

#include "smartpointer.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
enum msdrTokenKind {
  k_NoToken,  // 0, which is returned by yylex() at the end of file

  kTokenTitle,
  kTokenComposer,
  kTokenOpus,

  kTokenPitches,

  kTokenScore,
  kTokenPartGroup,
  kTokenPart,
  kTokenStaff,
  kTokenVoice,
  kTokenFragment,

  kTokenClef,

  kTokenKey,
  kTokenTreble,
  kTokenSoprano,
  kTokenAlto,
  kTokenBaritone,
  kTokenBass,

  kTokenTime,

  kTokenAnacrusis,

  kTokenFinalBar
};

string msdrTokenKindAsString (
  msdrTokenKind tokenKind);

enum msdrTokenDescriptionKind {
  kTokenDescriptionInteger,
  kTokenDescriptionDouble,
  kTokenDescriptionString
};

string msdrTokenDescriptionKindAsString (
  msdrTokenDescriptionKind tokenDescriptionKind);

//________________________________________________________________________
class EXP msdrTokenDescription : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdrTokenDescription> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdrTokenDescription ();

    virtual               ~msdrTokenDescription ();

    // set and get
    // ------------------------------------------------------

    void                  setInt (int value)
                              {
                                fTokenDescriptionKind = kTokenDescriptionInteger;
                                fInteger              = value;
                              }

    int                   getInt () const;

    void                  setDouble (double value)
                              {
                                fTokenDescriptionKind = kTokenDescriptionDouble;
                                fDouble               = value;
                              }

    double                getDouble () const;

    void                  setString (string value)
                              {
                                fTokenDescriptionKind = kTokenDescriptionString;
                                fString               = value;
                              }

    string                getString () const;

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // MSDL token descritption with variants handling (variants are not in C++-11)
    msdrTokenDescriptionKind
                          fTokenDescriptionKind;

    int                   fInteger;
    double                fDouble;
    string                fString;
};
typedef SMARTP<msdrTokenDescription> S_msdrTokenDescription;
EXP ostream& operator<< (ostream& os, const msdrTokenDescription& elt);

//________________________________________________________________________
class EXP msdrToken : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdrToken> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdrToken (
                            msdrTokenKind tokenKind);

                          msdrToken (
                            msdrTokenKind tokenKind,
                            int           value);

                          msdrToken (
                            msdrTokenKind tokenKind,
                            string        value);

                          msdrToken (
                            msdrTokenKind tokenKind,
                            double        value);

    virtual               ~msdrToken ();

    // set and get
    // ------------------------------------------------------

    msdrTokenKind         getTokenKind () const
                              { return fTokenKind; }


    const msdrTokenDescription&
                          getTokenDescription () const
                              { return fTokenDescription; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // the MSDL token
    msdrTokenKind         fTokenKind;


    // its descritption with variants handling
    msdrTokenDescription  fTokenDescription;
};
typedef SMARTP<msdrToken> S_msdrToken;
EXP ostream& operator<< (ostream& os, const msdrToken& elt);

//________________________________________________________________________
class EXP msdrTokensList : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdrTokensList> create ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdrTokensList ();

    virtual               ~msdrTokensList ();

  public:

    // set and get
    // ------------------------------------------------------

    const list<msdrToken>&
                          getTokensList () const
                              { return fTokensList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendTokenToTokensList (
                            const msdrToken& token);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    list<msdrToken>       fTokensList;
};
typedef SMARTP<msdrTokensList> S_msdrTokensList;
EXP ostream& operator<< (ostream& os, const S_msdrTokensList& elt);


}

#endif
