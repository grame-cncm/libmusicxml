/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrBrailleGenerators___
#define ___bsrBrailleGenerators___

#include "bsrBrailleElements.h"

#include "bsrBrailleSigns.h"

#include "bsrBasicTypes.h"

#include "brailleOptions.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class bsrBrailleGenerator : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

/* JMI
    static SMARTP<bsrBrailleGenerator> create (
      brailleOptions::bsrByteOrderingKind
                          byteOrderingKind,
      ostream&            brailleOutputStream);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrBrailleGenerator (
      brailleOptions::bsrByteOrderingKind
                          byteOrderingKind,
      ostream&            brailleOutputStream);
      
    virtual ~bsrBrailleGenerator ();
  
  public:

    // set and get
    // ------------------------------------------------------

    brailleOptions::bsrByteOrderingKind
                          getByteOrderingKind () const
                              { return fByteOrderingKind; }
                  
  public:

    // services
    // ------------------------------------------------------
      
    virtual void          generateCodeForBrailleCell (
                            bsrCellKind cellKind) = 0;
    
    virtual void          generateCodeForBrailleSign (
                            S_bsrBrailleSign brailleSign) = 0;
    
  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  protected:

    // fields
    // ------------------------------------------------------

    brailleOptions::bsrByteOrderingKind
                          fByteOrderingKind;
    ostream&              fBrailleOutputStream;
};
typedef SMARTP<bsrBrailleGenerator> S_bsrBrailleGenerator;
EXP ostream& operator<< (ostream& os, const S_bsrBrailleGenerator& elt);

//______________________________________________________________________________
class bsrUTF8BrailleGenerator : public bsrBrailleGenerator
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrUTF8BrailleGenerator> create (
      brailleOptions::bsrByteOrderingKind
                          byteOrderingKind,
      ostream&            brailleOutputStream);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrUTF8BrailleGenerator (
      brailleOptions::bsrByteOrderingKind
                          byteOrderingKind,
      ostream&            brailleOutputStream);
      
    virtual ~bsrUTF8BrailleGenerator ();
  
  public:

    // set and get
    // ------------------------------------------------------

  public:

    // services
    // ------------------------------------------------------
      
    virtual void          generateCodeForBrailleCell (
                            bsrCellKind cellKind);
    
    virtual void          generateCodeForBrailleSign (
                            S_bsrBrailleSign brailleSign);
    
  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<bsrUTF8BrailleGenerator> S_bsrUTF8BrailleGenerator;
EXP ostream& operator<< (ostream& os, const S_bsrUTF8BrailleGenerator& elt);

//______________________________________________________________________________
class bsrUTF16BigEndianBrailleGenerator : public bsrBrailleGenerator
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrUTF16BigEndianBrailleGenerator> create (
      brailleOptions::bsrByteOrderingKind
                          byteOrderingKind,
      ostream&            brailleOutputStream);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrUTF16BigEndianBrailleGenerator (
      brailleOptions::bsrByteOrderingKind
                          byteOrderingKind,
      ostream&            brailleOutputStream);
      
    virtual ~bsrUTF16BigEndianBrailleGenerator ();
  
  public:

    // set and get
    // ------------------------------------------------------

  public:

    // services
    // ------------------------------------------------------
      
    virtual void          generateCodeForBrailleCell (
                            bsrCellKind cellKind);
    
    virtual void          generateCodeForBrailleSign (
                            S_bsrBrailleSign brailleSign);
    
  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<bsrUTF16BigEndianBrailleGenerator> S_bsrUTF16BigEndianBrailleGenerator;
EXP ostream& operator<< (ostream& os, const S_bsrUTF16BigEndianBrailleGenerator& elt);

//______________________________________________________________________________
class bsrUTF16SmallEndianBrailleGenerator : public bsrBrailleGenerator
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrUTF16SmallEndianBrailleGenerator> create (
      brailleOptions::bsrByteOrderingKind
                          byteOrderingKind,
      ostream&            brailleOutputStream);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrUTF16SmallEndianBrailleGenerator (
      brailleOptions::bsrByteOrderingKind
                          byteOrderingKind,
      ostream&            brailleOutputStream);
      
    virtual ~bsrUTF16SmallEndianBrailleGenerator ();
  
  public:

    // set and get
    // ------------------------------------------------------

  public:

    // services
    // ------------------------------------------------------
      
    virtual void          generateCodeForBrailleCell (
                            bsrCellKind cellKind);
    
    virtual void          generateCodeForBrailleSign (
                            S_bsrBrailleSign brailleSign);
    
  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<bsrUTF16SmallEndianBrailleGenerator> S_bsrUTF16SmallEndianBrailleGenerator;
EXP ostream& operator<< (ostream& os, const S_bsrUTF16SmallEndianBrailleGenerator& elt);


} // namespace MusicXML2


#endif
