#ifndef ___msdrMusics___
#define ___msdrMusics___

#include <vector>
#include <list> // JMI ???

#include "smartpointer.h"
#include "visitable.h"

#include "msdrMeasures.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msdrMusic : public smartable, public visitable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdrMusic> create (
                            int    inputLineNumber,
                            string musicName);

    SMARTP<msdrMusic> createMusicNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdrMusic (
                            int    inputLineNumber,
                            string musicName);

                          msdrMusic ();

    virtual               ~msdrMusic ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMusic ();

  public:

    // set and get
    // ------------------------------------------------------

    // music name
    string                getMusicName () const
                              { return fMusicName; }

    // input line number
    int                   getInputLineNumber () const
                              { return fInputLineNumber; }

    // music measures vector
    const vector<S_msdrMeasure>&
                          getMusicMeasuresVector () const
                              { return fMusicMeasuresVector; }
    // measures list
    const list<S_msdrMeasure>&
                          getMusicMeasuresList () const
                              { return fMusicMeasuresList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addMeasureToMusic (
                            int           inputLineNumber,
                            int           measureNumber,
                            S_msdrMeasure measure);

  private:

    // private services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v);
    void                  acceptOut (basevisitor* v);

    void                  browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortString () const;
    string                asString () const;

    void                  displayMusic (
                            int    inputLineNumber,
                            string context) const;

    void                  print (ostream& os) const;

    void                  printShort (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // music name
    string                fMusicName;

    // input line number
    int                   fInputLineNumber;

    // measures vector
    vector<S_msdrMeasure> fMusicMeasuresVector;

  public:

    // public work services
    // ------------------------------------------------------

  private:

    // work fields
    // ------------------------------------------------------

    // measures list
    list<S_msdrMeasure>   fMusicMeasuresList; // JMI

    // voice finalization
    bool                  fMusicHasBeenFinalized;
};
typedef SMARTP<msdrMusic> S_msdrMusic;
EXP ostream& operator<< (ostream& os, const S_msdrMusic& elt);


}


#endif
