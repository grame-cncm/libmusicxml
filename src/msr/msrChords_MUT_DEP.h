/*
  This file is to be included only by msrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___msrChords_MUT_DEP___
#define ___msrChords_MUT_DEP___

//______________________________________________________________________________
class msrChordSlurLink;
typedef SMARTP<msrChordSlurLink> S_msrChordSlurLink;

class msrChordBeamLink;
typedef SMARTP<msrChordBeamLink> S_msrChordBeamLink;

class msrChordGraceNotesGroupLink;
typedef SMARTP<msrChordGraceNotesGroupLink> S_msrChordGraceNotesGroupLink;

//______________________________________________________________________________
class msrChord : public msrTupletElement
{
  public:

    static SMARTP<msrChord> create (
      int             inputLineNumber,
      rational        chordSoundingWholeNotes,
      rational        chordDisplayWholeNotes,
      msrDurationKind chordGraphicDurationKind);

    // creation from MusicXML
    // ------------------------------------------------------

    SMARTP<msrChord> createChordNewbornClone (
      S_msrPart containingPart);

    SMARTP<msrChord> createChordDeepCopy (
      S_msrPart containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrChord (
      int             inputLineNumber,
      rational        chordSoundingWholeNotes,
      rational        chordDisplayWholeNotes,
      msrDurationKind chordGraphicDurationKind);

    virtual ~msrChord ();

  public:

    // set and get
    // ------------------------------------------------------

     // whole notes
    void                  setChordSoundingWholeNotes (
                            rational wholeNotes);

    rational              getChordSoundingWholeNotes () const
                              { return fMeasureElementSoundingWholeNotes; }

    void                  setChordDisplayWholeNotes (
                            rational wholeNotes);

    rational              getChordDisplayWholeNotes () const
                             { return fChordDisplayWholeNotes; }

    // graphic duration
    msrDurationKind       getChordGraphicDurationKind () const
                              { return fChordGraphicDurationKind; }

    // chord notes
    const vector<S_msrNote>&
                          getChordNotesVector () const
                              { return fChordNotesVector; }

    // stems
    const list<S_msrStem>&
                          getChordStems () const
                              { return fChordStems; }

    // beams
    /*
    const list<S_msrBeam>&
                          getChordBeams () const
                              { return fChordBeams; }
                              */
    const list<S_msrChordBeamLink>&
                          getChordBeamLinks () const
                              { return fChordBeamLinks; }

    // articulations
    const list<S_msrArticulation>&
                          getChordArticulations () const
                              { return fChordArticulations; }

    // spanners
    const list<S_msrSpanner>&
                          getChordSpanners () const
                              { return fChordSpanners; }

    // technicals
    const list<S_msrTechnical>&
                          getChordTechnicals () const
                              { return fChordTechnicals; }

    const list<S_msrTechnicalWithInteger>&
                          getChordTechnicalWithIntegers () const
                              { return fChordTechnicalWithIntegers; }

    const list<S_msrTechnicalWithFloat>&
                          getChordTechnicalWithFloats () const
                              { return fChordTechnicalWithFloats; }

    const list<S_msrTechnicalWithString>&
                          getChordTechnicalWithStrings () const
                              { return fChordTechnicalWithStrings; }

    // ornaments
    const list<S_msrOrnament>&
                          getChordOrnaments () const
                              { return fChordOrnaments; }

    // glissandos
    const list<S_msrGlissando>&
                          getChordGlissandos () const
                              { return fChordGlissandos; }

    // slides
    const list<S_msrSlide>&
                          getChordSlides () const
                              { return fChordSlides; }

    // singleTremolo
    void                  setChordSingleTremolo (
                            S_msrSingleTremolo trem);

    S_msrSingleTremolo    getChordSingleTremolo () const
                              { return fChordSingleTremolo; }

    // dynamics
    const list<S_msrDynamics>&
                          getChordDynamics () const
                              { return fChordDynamics; }
    const list<S_msrOtherDynamics>&
                          getChordOtherDynamics () const
                              { return fChordOtherDynamics; }

    const list<S_msrWedge>&
                          getChordWedges () const
                              { return fChordWedges; }

    // segnos
    const list<S_msrSegno>&
                          getChordSegnos () const
                              { return fChordSegnos; }

    // dal segnos
    const list<S_msrDalSegno>&
                          getChordDalSegnos () const
                              { return fChordDalSegnos; }

    // coda
    const list<S_msrCoda>&
                          getChordCodas () const
                              { return fChordCodas; }

    // words
    const list<S_msrWords>&
                          getChordWords () const
                              { return fChordWords; }

    // ties
    const list<S_msrTie>&
                          getChordTies () const
                              { return fChordTies; }

    // slurs
                              /*
    const list<S_msrSlur>&
                          getChordSlurs () const
                              { return fChordSlurs; }
*/
    const list<S_msrChordSlurLink>&
                          getChordSlurLinks () const
                              { return fChordSlurLinks; }

    // ligatures
    const list<S_msrLigature>&
                          getChordLigatures () const
                              { return fChordLigatures; }

    // pedals
    const list<S_msrPedal>&
                          getChordPedals () const
                              { return fChordPedals; }

    // double tremolo
    void                  setChordIsFirstChordInADoubleTremolo ()
                              { fChordIsFirstChordInADoubleTremolo = true; }

    bool                  getChordIsFirstChordInADoubleTremolo () const
                              { return fChordIsFirstChordInADoubleTremolo; }

    void                  setChordIsSecondChordInADoubleTremolo ()
                              { fChordIsSecondChordInADoubleTremolo = true; }

    bool                  getChordIsSecondChordInADoubleTremolo () const
                              { return fChordIsSecondChordInADoubleTremolo; }

    // grace notes
/*
    void                  setChordGraceNotesGroupBefore (
                            S_msrGraceNotesGroup graceNotesGroupBefore)
                              {
                                fChordGraceNotesGroupBefore =
                                  graceNotesGroupBefore;
                              }

    S_msrGraceNotesGroup  getChordGraceNotesGroupBefore () const
                              { return fChordGraceNotesGroupBefore; }

    void                  setChordGraceNotesGroupAfter (
                            S_msrGraceNotesGroup graceNotesGroupAfter)
                              {
                                fChordGraceNotesGroupAfter =
                                  graceNotesGroupAfter;
                              }

    S_msrGraceNotesGroup  getChordGraceNotesGroupAfter () const
                              { return fChordGraceNotesGroupAfter; }
*/

    void                  setChordGraceNotesGroupLinkBefore (
                            S_msrChordGraceNotesGroupLink chordChordGraceNotesGroupLinkBefore)
                              {
                                fChordGraceNotesGroupLinkBefore =
                                  chordChordGraceNotesGroupLinkBefore;
                              }

    S_msrChordGraceNotesGroupLink
                          getChordGraceNotesGroupLinkBefore () const
                              { return fChordGraceNotesGroupLinkBefore; }

    void                  setChordGraceNotesGroupLinkAfter (
                            S_msrChordGraceNotesGroupLink chordChordGraceNotesGroupLinkAfter)
                              {
                                fChordGraceNotesGroupLinkAfter =
                                  chordChordGraceNotesGroupLinkAfter;
                              }

    S_msrChordGraceNotesGroupLink
                          getChordGraceNotesGroupLinkAfter () const
                              { return fChordGraceNotesGroupLinkAfter; }

    // harmony
    void                  appendHarmonyToChord (
                            S_msrHarmony harmony)
                              { fChordHarmoniesList.push_back (harmony); }

    // figured bass
    void                  setChordFiguredBass (
                            S_msrFiguredBass figuredBass)
                              { fChordFiguredBass = figuredBass; }

    const S_msrFiguredBass&
                          getChordFiguredBass () const
                              { return fChordFiguredBass; }

    // octave shift
    void                  setChordOctaveShift (
                            S_msrOctaveShift octaveShift)
                              { fChordOctaveShift = octaveShift; }

    S_msrOctaveShift      getChordOctaveShift () const
                            { return fChordOctaveShift; }

    // measure upLink
    void                  setChordMeasureUpLink (
                            const S_msrMeasure& measure)
                              { fChordMeasureUpLink = measure; }

    S_msrMeasure          getChordMeasureUpLink () const
                            { return fChordMeasureUpLink; }

    // tuplet upLink
    void                  setChordTupletUpLink (
                            const S_msrTuplet& tuplet)
                              { fChordTupletUpLink = tuplet; }

    S_msrTuplet           getChordTupletUpLink () const
                            { return fChordTupletUpLink; }

    // positions in measures
    void                  setChordMembersPositionInMeasure (
                            S_msrMeasure measure,
                            rational     positionInMeasure);

    // services
    // ------------------------------------------------------

    // notes
    void                  addFirstNoteToChord (
                            S_msrNote  note,
                            S_msrVoice voice);

    void                  addAnotherNoteToChord (
                            S_msrNote note,
                            S_msrVoice voice);

    S_msrNote             fetchChordFirstNonGraceNote () const;

    // articulations
    void                  appendArticulationToChord (S_msrArticulation art);

    // spanners
    void                  appendSpannerToChord (S_msrSpanner span);

    // technicals
    void                  appendTechnicalToChord (S_msrTechnical tech);

    void                  appendTechnicalWithIntegerToChord (
                            S_msrTechnicalWithInteger tech);

    void                  appendTechnicalWithFloatToChord (
                            S_msrTechnicalWithFloat tech);

    void                  appendTechnicalWithStringToChord (
                            S_msrTechnicalWithString tech);

    // ornaments
    void                  appendOrnamentToChord (S_msrOrnament orn);

    // glissandos
    void                  appendGlissandoToChord (S_msrGlissando gliss);

    // slides
    void                  appendSlideToChord (S_msrSlide slide);

    // dynamics
    void                  appendDynamicsToChord (S_msrDynamics dynamic)
                              { fChordDynamics.push_back (dynamic); }

    // other dynamics
    void                  appendOtherDynamicsToChord (
                            S_msrOtherDynamics otherDynamic)
                              { fChordOtherDynamics.push_back (otherDynamic); }

    // slashes
    void                  appendSlashToChord (S_msrSlash slash)
                              { fChordSlashes.push_back (slash); }

    // wedges
    void                  appendWedgeToChord (S_msrWedge wedge)
                              { fChordWedges.push_back (wedge); }

    // segnos
    void                  appendSegnoToChord (S_msrSegno segno)
                              { fChordSegnos.push_back (segno); }

    // dal segnos
    void                  appendDalSegnoToChord (S_msrDalSegno dalSegno);

    // codas
    void                  appendCodaToChord (S_msrCoda coda)
                              { fChordCodas.push_back (coda); }

    // words
    void                  appendWordsToChord (S_msrWords dynamic)
                              { fChordWords.push_back (dynamic); }

    // ties
    void                  appendTieToChord (S_msrTie tie)
                              { fChordTies.push_back (tie); }

    // slurs
//    void                  appendSlurToChord (S_msrSlur slur);
    void                  appendChordSlurLinkToChord (S_msrChordSlurLink chordSlurLink);

    // stems
    void                  appendStemToChord (S_msrStem stem);

    // beams
//    void                  appendBeamToChord (S_msrBeam beam);
    void                  appendChordBeamLinkToChord (S_msrChordBeamLink chordBeamLink);

    // ligatures
    void                  appendLigatureToChord (S_msrLigature ligature)
                              { fChordLigatures.push_back (ligature); }

    // pedals
    void                  appendPedalToChord (S_msrPedal pedal)
                              { fChordPedals.push_back (pedal);  }

    // tuplet members
    /* JMI
    void                  applyTupletMemberDisplayedFactorToChordMembers (
                            int actualNotes, int normalNotes);
*/

    // finalization
    void                  finalizeChord (
                            int inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asStringwithRawDivisions () const;

    string                asString () const override;

    string                asShortString () const override;

    virtual void          print (ostream& os) const override;

    virtual void          printShort (ostream& os) const override;

  private:

    // fields
    // ------------------------------------------------------

    // measure upLink
    S_msrMeasure          fChordMeasureUpLink;

    // tuplet uplink
    S_msrTuplet           fChordTupletUpLink;

    // sounding whole notes
    // no need for 'rational fChordSoundingWholeNotes;',
    // since fChordNotesVector [0] contains this information,
    // as do all the other elements in this vector

    // display whole notes
    rational              fChordDisplayWholeNotes;

    // graphic duration is needed for grace notes,
    // since they don't have any note (sounding) duration
    msrDurationKind       fChordGraphicDurationKind;

    vector<S_msrNote>     fChordNotesVector;

    // stems
    list<S_msrStem>       fChordStems;

    // beams
//    list<S_msrBeam>       fChordBeams;
    list<S_msrChordBeamLink>
                          fChordBeamLinks;

    // articulations
    list<S_msrArticulation>
                          fChordArticulations;

    // spanners
    list<S_msrSpanner>    fChordSpanners;

    // single tremolo
    S_msrSingleTremolo    fChordSingleTremolo;

    // double tremolo
    bool                  fChordIsFirstChordInADoubleTremolo;
    bool                  fChordIsSecondChordInADoubleTremolo;

    // technicals
    list<S_msrTechnical>  fChordTechnicals;

    list<S_msrTechnicalWithInteger>
                          fChordTechnicalWithIntegers;

    list<S_msrTechnicalWithFloat>
                          fChordTechnicalWithFloats;

    list<S_msrTechnicalWithString>
                          fChordTechnicalWithStrings;

    // ornaments
    list<S_msrOrnament>   fChordOrnaments;

    // glissandos
    list<S_msrGlissando>  fChordGlissandos;

    // slides
    list<S_msrSlide>      fChordSlides;

    // dynamics
    list<S_msrDynamics>   fChordDynamics;
    list<S_msrOtherDynamics>
                          fChordOtherDynamics;

    // slashes
    list<S_msrSlash>      fChordSlashes;

    // wedges
    list<S_msrWedge>      fChordWedges;

    // segnos
    list<S_msrSegno>      fChordSegnos;

    // dal segnos
    list<S_msrDalSegno>   fChordDalSegnos;

    // coda
    list<S_msrCoda>       fChordCodas;

    // octave shift
    S_msrOctaveShift      fChordOctaveShift;

    // words
    list<S_msrWords>      fChordWords;

    // ties
    list<S_msrTie>        fChordTies;

    // slurs
//    list<S_msrSlur>       fChordSlurs;
    list<S_msrChordSlurLink>
                          fChordSlurLinks;

    // ligatures
    list<S_msrLigature>   fChordLigatures;

    // pedals
    list<S_msrPedal>      fChordPedals;

    // grace notes
//    S_msrGraceNotesGroup  fChordGraceNotesGroupBefore;
//    S_msrGraceNotesGroup  fChordGraceNotesGroupAfter;

    S_msrChordGraceNotesGroupLink
                          fChordGraceNotesGroupLinkBefore;
    S_msrChordGraceNotesGroupLink
                          fChordGraceNotesGroupLinkAfter;

    // harmony
    list<S_msrHarmony>    fChordHarmoniesList;

    // figured bass
    S_msrFiguredBass      fChordFiguredBass;
};
typedef SMARTP<msrChord> S_msrChord;
EXP ostream& operator<< (ostream& os, const S_msrChord& elt);

//______________________________________________________________________________
class msrChordBeamLink : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrChordBeamLink> create (
      int        inputLineNumber,
      S_msrBeam  originalBeam,
      S_msrChord chordUpLink);

    SMARTP<msrChordBeamLink> createBeamNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrChordBeamLink (
      int        inputLineNumber,
      S_msrBeam  originalBeam,
      S_msrChord chordUpLink);

    virtual ~msrChordBeamLink ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrChord            getChordUpLink () const
                              { return fChordUpLink; }

    S_msrBeam             getOriginalBeam () const
                              { return fOriginalBeam; }

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortString () const override;
    string                asString () const override;

    virtual void          print (ostream& os) const override;

    virtual void          printShort (ostream& os) const override;

  private:

    // fields
    // ------------------------------------------------------

    S_msrChord            fChordUpLink;

    S_msrBeam             fOriginalBeam;
};
typedef SMARTP<msrChordBeamLink> S_msrChordBeamLink;
EXP ostream& operator<< (ostream& os, const S_msrChordBeamLink& elt);

//______________________________________________________________________________
class msrChordSlurLink : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrChordSlurLink> create (
      int        inputLineNumber,
      S_msrSlur  originalSlur,
      S_msrChord chordUpLink);

    SMARTP<msrChordSlurLink> createSlurNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrChordSlurLink (
      int        inputLineNumber,
      S_msrSlur  originalSlur,
      S_msrChord chordUpLink);

    virtual ~msrChordSlurLink ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrChord            getChordUpLink () const
                              { return fChordUpLink; }

    S_msrSlur             getOriginalSlur () const
                              { return fOriginalSlur; }

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortString () const override;
    string                asString () const override;

    virtual void          print (ostream& os) const override;

    virtual void          printShort (ostream& os) const override;

  private:

    // fields
    // ------------------------------------------------------

    S_msrChord            fChordUpLink;

    S_msrSlur             fOriginalSlur;
};
typedef SMARTP<msrChordSlurLink> S_msrChordSlurLink;
EXP ostream& operator<< (ostream& os, const S_msrChordSlurLink& elt);

//______________________________________________________________________________
class msrChordGraceNotesGroupLink : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrChordGraceNotesGroupLink> create (
      int                  inputLineNumber,
      S_msrGraceNotesGroup originalGraceNotesGroup,
      S_msrChord            chordUpLink);

    SMARTP<msrChordGraceNotesGroupLink> createChordGraceNotesGroupLinkNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrChordGraceNotesGroupLink (
      int                  inputLineNumber,
      S_msrGraceNotesGroup originalGraceNotesGroup,
      S_msrChord            chordUpLink);

    virtual ~msrChordGraceNotesGroupLink ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrChord            getChordUpLink () const
                              { return fChordUpLink; }

    S_msrGraceNotesGroup  getOriginalGraceNotesGroup () const
                              { return fOriginalGraceNotesGroup; }

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortString () const override;
    string                asString () const override;

    virtual void          print (ostream& os) const override;

    virtual void          printShort (ostream& os) const override;

  private:

    // fields
    // ------------------------------------------------------

    S_msrChord            fChordUpLink;

    S_msrGraceNotesGroup  fOriginalGraceNotesGroup;
};
typedef SMARTP<msrChordGraceNotesGroupLink> S_msrChordGraceNotesGroupLink;
EXP ostream& operator<< (ostream& os, const S_msrChordGraceNotesGroupLink& elt);


#endif
