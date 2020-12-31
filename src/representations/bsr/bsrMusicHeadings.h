#ifndef ___bsrMusicHeadings___
#define ___bsrMusicHeadings___

#include "smartpointer.h"

#include "bsrLines.h"
#include "bsrCellsLists.h"
#include "bsrTimes.h"
#include "bsrKeys.h"
#include "bsrTempos.h"


namespace MusicXML2
{

//______________________________________________________________________________
/*
class bsrKey;
typedef SMARTP<bsrKey> S_bsrKey;

class bsrTime;
typedef SMARTP<bsrTime> S_bsrTime;

class bsrTempo;
typedef SMARTP<bsrTempo> S_bsrTempo;
*/

//______________________________________________________________________________
class EXP bsrMusicHeading : public bsrLine
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrMusicHeading> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrMusicHeading (
      int inputLineNumber);

    virtual ~bsrMusicHeading ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMusicHeadingTempo (S_bsrTempo tempo)
                              { fMusicHeadingTempo = tempo; }

    S_bsrTempo            getMusicHeadingTempo () const
                              { return fMusicHeadingTempo; }

    S_bsrKey              getMusicHeadingKey () const
                              { return fMusicHeadingKey; }

    void                  setMusicHeadingKey (S_bsrKey key)
                              { fMusicHeadingKey = key; }

    S_bsrTime             getMusicHeadingTime () const
                              { return fMusicHeadingTime; }

    void                  setMusicHeadingTime (S_bsrTime time)
                              { fMusicHeadingTime = time; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const // override JMI
                              { return buildCellsList (); }

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        buildCellsList () const;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    string                asDebugString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_bsrTempo            fMusicHeadingTempo;
    S_bsrKey              fMusicHeadingKey;
    S_bsrTime             fMusicHeadingTime;
};
typedef SMARTP<bsrMusicHeading> S_bsrMusicHeading;
EXP ostream& operator<< (ostream& os, const S_bsrMusicHeading& elt);


}


#endif
