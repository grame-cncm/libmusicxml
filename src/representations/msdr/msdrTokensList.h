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

#include <variant>


using namespace std;

namespace MusicXML2
{

/*
#include <variant>
#include <string>
#include <cassert>

int main()
{
    std::variant<int, float> v, w;
    v = 12; // v contains int
    int i = std::get<int>(v);
    w = std::get<int>(v);
    w = std::get<0>(v); // same effect as the previous line
    w = v; // same effect as the previous line

//  std::get<double>(v); // error: no double in [int, float]
//  std::get<3>(v);      // error: valid index values are 0 and 1

    try {
      std::get<float>(w); // w contains int, not float: will throw
    }
    catch (const std::bad_variant_access&) {}

    using namespace std::literals;

    std::variant<std::string> x("abc");
    // converting constructors work when unambiguous
    x = "def"; // converting assignment also works when unambiguous

    std::variant<std::string, void const*> y("abc");
    // casts to void const * when passed a char const *
    assert(std::holds_alternative<void const*>(y)); // succeeds
    y = "xyz"s;
    assert(std::holds_alternative<std::string>(y)); // succeeds
}*/

//________________________________________________________________________
enum msdrTokenDescrKind {
  kTokenDescrInteger,
  kTokenDescrDouble,
  kTokenDescrString
};

string msdrTokenDescrKindAsString (msdrTokenDescrKind tokenDescrKind);

//________________________________________________________________________
class EXP msdrTokenDescr
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdrTokenDescr ();

    virtual               ~msdrTokenDescr ();

    // set and get
    // ------------------------------------------------------

    void                  setInt (int value)
                              {
                                fInteger        = value;
                                fTokenDescrKind = kTokenDescrInteger;
                              }

    int                   getInt () const;

    void                  setDouble (double value)
                              {
                                fDouble         = value;
                                fTokenDescrKind = kTokenDescrDouble;
                              }

    double                getDouble () const;

    void                  setString (string value)
                              {
                                fString         = value;
                                fTokenDescrKind = kTokenDescrString;
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

    msdrTokenDescrKind    fTokenDescrKind;

    int                   fInteger;
    double                fDouble;
    string                fString;
};
EXP ostream& operator<< (ostream& os, const msdrTokenDescr& elt);

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

    const list<msdrTokenDescr>&
                          getMsdrTokenDescrList () const
                              { return fMsdrTokenDescrList; }

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

    list<msdrTokenDescr>  fMsdrTokenDescrList;
};
typedef SMARTP<msdrTokensList> S_msdrTokensList;
EXP ostream& operator<< (ostream& os, const S_msdrTokensList& elt);


}

#endif
