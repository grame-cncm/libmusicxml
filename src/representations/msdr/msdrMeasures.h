#ifndef ___msdrMeasures___
#define ___msdrMeasures___

#include <vector>

#include "smartpointer.h"
#include "visitable.h"

#include "msdrLayers.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msdrMeasure : public smartable, public visitable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdrMeasure> create (
                            int    inputLineNumber,
                            string measureNumber);

    SMARTP<msdrMeasure> createMusicNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    // for regular voices
                          msdrMeasure (
                            int    inputLineNumber,
                            string measureNumber);

                          msdrMeasure ();

    // destructor
    virtual               ~msdrMeasure ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMeasure ();

  public:

    // set and get
    // ------------------------------------------------------

    // measure number
    string                getMeasureNumber () const
                              { return fMeasureNumber; }

    // input line number
    int                   getInputLineNumber () const
                              { return fInputLineNumber; }

    // music measures vector
    const vector<S_msdrLayer>&
                          getMeasureElementsVector () const
                              { return fMeasureElementsVector; }
    // measures flat list
    const list<S_msdrLayer>&
                          getMeasureLayersList () const
                              { return fMeasureLayersList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addLayerToMeasure (
                            int         inputLineNumber,
                            int         layerNumber,
                            S_msdrLayer layer);

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

    void                  displayMeasure (
                            int    inputLineNumber,
                            string context) const;

    void                  print (ostream& os) const;

    void                  printShort (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    // measure number
    string                fMeasureNumber;

    // input line number
    int                   fInputLineNumber;

    // measures elements list
    list<S_msdrLayer>
                          fMeasureLayersList;

  public:

    // public work services
    // ------------------------------------------------------

  private:

    // work fields
    // ------------------------------------------------------

    // measures elements vector
    vector<S_msdrLayer>
                          fMeasureElementsVector;

    // voice finalization
    bool                  fMeasureHasBeenFinalized;
};
typedef SMARTP<msdrMeasure> S_msdrMeasure;
EXP ostream& operator<< (ostream& os, const S_msdrMeasure& elt);


}


#endif
