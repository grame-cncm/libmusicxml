/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrLayout___
#define ___msrLayout___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msrPageLayout : public msrElement
{
/*
<!--
	Page layout can be defined both in score-wide defaults
	and in the print element. Page margins are specified either
	for both even and odd pages, or via separate odd and even
	page number values. The type is not needed when used as
	part of a print element. If omitted when used in the
	defaults element, "both" is the default.
-->
<!ELEMENT page-layout ((page-height, page-width)?,
	(page-margins, page-margins?)?)>
<!ELEMENT page-height %layout-tenths;>
<!ELEMENT page-width %layout-tenths;>
<!ELEMENT page-margins (left-margin, right-margin,
	top-margin, bottom-margin)>
<!ATTLIST page-margins
    type (odd | even | both) #IMPLIED
>
*/

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPageLayout> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrPageLayout (
      int inputLineNumber);

    virtual ~msrPageLayout ();

  public:

    // set and get
    // ------------------------------------------------------

    // page size
    void                  setPageHeight (S_msrLength val)
                              { fPageHeight = val; }
    S_msrLength           getPageHeight () const
                              { return fPageHeight; }

    void                  setPageWidth (S_msrLength val)
                              { fPageWidth = val; }
    S_msrLength           getPageWidth () const
                              { return fPageWidth; }

    // margins groups
    void                  setOddMarginsGroup (S_msrMarginsGroup val)
                              { fOddMarginsGroup = val; }
    S_msrMarginsGroup     getOddMarginsGroup () const
                              { return fOddMarginsGroup; }

    void                  setEvenMarginsGroup (S_msrMarginsGroup val)
                              { fEvenMarginsGroup = val; }
    S_msrMarginsGroup     getEvenMarginsGroup () const
                              { return fEvenMarginsGroup; }

    void                  setBothMarginsGroup (S_msrMarginsGroup val)
                              { fBothMarginsGroup = val; }
    S_msrMarginsGroup     getBothMarginsGroup () const
                              { return fBothMarginsGroup; }

  public:

    // services
    // ------------------------------------------------------

    // LilyPond knows inner/outer margins and binding offset,
    // but no odd, even nor both margins settings,
    // so we need methods to obtain a single margin from
    // fOddMarginsGroup, fEvenMarginsGroup and fBothMarginsGroup
    S_msrMargin           getSingleLeftMargin ();
    S_msrMargin           getSingleRightMargin ();
    S_msrMargin           getSingleTopMargin ();
    S_msrMargin           getSingleBottomMargin ();

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    // page size
    S_msrLength           fPageHeight;
    S_msrLength           fPageWidth;

    // margins groups
    S_msrMarginsGroup     fOddMarginsGroup;
    S_msrMarginsGroup     fEvenMarginsGroup;
    S_msrMarginsGroup     fBothMarginsGroup;
};
typedef SMARTP<msrPageLayout> S_msrPageLayout;
EXP ostream& operator<< (ostream& os, const S_msrPageLayout& elt);

//______________________________________________________________________________
class msrSystemLayout : public msrElement
{
/*
<!--
	A system is a group of staves that are read and played
	simultaneously. System layout includes left and right
	margins, the vertical distance from the previous system,
	and the presence or absence of system dividers.

	Margins are relative to the page margins. Positive values
	indent and negative values reduce the margin size. The
	system distance is measured from the bottom line of the
	previous system to the top line of the current system.
	It is ignored for the first system on a page. The top
	system distance is measured from the page's top margin to
	the top line of the first system. It is ignored for all
	but the first system on a page.

	Sometimes the sum of measure widths in a system may not
	equal the system width specified by the layout elements due
	to roundoff or other errors. The behavior when reading
	MusicXML files in these cases is application-dependent.
	For instance, applications may find that the system layout
	data is more reliable than the sum of the measure widths,
	and adjust the measure widths accordingly.

	When used in the layout element, the system-layout element
	defines a default appearance for all systems in the score.
	When used in the print element, the system layout element
	affects the appearance of the current system only. All
	other systems use the default values provided in the
	defaults element. If any child elements are missing from
	the system-layout element in a print element, the values
	from the defaults element are used there as well.
-->
<!ELEMENT system-layout
	(system-margins?, system-distance?,
	 top-system-distance?, system-dividers?)>
<!ELEMENT system-margins (left-margin, right-margin)>
<!ELEMENT system-distance %layout-tenths;>
<!ELEMENT top-system-distance %layout-tenths;>
*/

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSystemLayout> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSystemLayout (
      int inputLineNumber);

    virtual ~msrSystemLayout ();

  public:

    // set and get
    // ------------------------------------------------------

    // margins
    void                  setLeftMargin (S_msrMargin val)
                              { fLeftMargin = val; }
    S_msrMargin           getLeftMargin () const
                              { return fLeftMargin; }

    void                  setRightMargin (S_msrMargin val)
                              { fRightMargin = val; }
    S_msrMargin           getRightMargin () const
                              { return fRightMargin; }

    // distances
    void                  setSystemDistance (S_msrLength val)
                              { fSystemDistance = val; }
    S_msrLength           getSystemDistance () const
                              { return fSystemDistance; }

    void                  setTopSystemDistance (S_msrLength val)
                              { fTopSystemDistance = val; }
    S_msrLength           getTopSystemDistance () const
                              { return fTopSystemDistance; }

  public:

    // services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    // margins
    S_msrMargin           fLeftMargin;
    S_msrMargin           fRightMargin;

    // distances
    S_msrLength           fSystemDistance;
    S_msrLength           fTopSystemDistance;
};
typedef SMARTP<msrSystemLayout> S_msrSystemLayout;
EXP ostream& operator<< (ostream& os, const S_msrSystemLayout& elt);

//______________________________________________________________________________
class msrSystemDividers : public msrElement
{
/*
<!--
	The system-dividers element indicates the presence or
	absence of system dividers (also known as system separation
	marks) between systems displayed on the same page. Dividers
	on the left and right side of the page are controlled by
	the left-divider and right-divider elements respectively.
	The default vertical position is half the system-distance
	value from the top of the system that is below the divider.
	The default horizontal position is the left and right
	system margin, respectively.

	When used in the print element, the system-dividers element
	affects the dividers that would appear between the current
	system and the previous system.
-->
<!ELEMENT system-dividers (left-divider, right-divider)>
<!ELEMENT left-divider EMPTY>
<!ATTLIST left-divider
    %print-object;
    %print-style-align;
>
<!ELEMENT right-divider EMPTY>
<!ATTLIST right-divider
    %print-object;
    %print-style-align;
>

    <system-layout>
      <system-margins>
        <left-margin>0</left-margin>
        <right-margin>0</right-margin>
      </system-margins>
      <system-distance>39</system-distance>
      <top-system-distance>39</top-system-distance>
      <system-dividers>
        <left-divider print-object="yes"/>
        <right-divider print-object="no"/>
      </system-dividers>
    </system-layout>
*/

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSystemDividers> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSystemDividers (
      int inputLineNumber);

    virtual ~msrSystemDividers ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setPrintLeftDivider (bool val)
                              { fPrintLeftDivider = val; }
    bool                  getPrintLeftDivider () const
                              { return fPrintLeftDivider; }

    void                  setPrintRightDivider (bool val)
                              { fPrintRightDivider = val; }
    bool                  getPrintRightDivider () const
                              { return fPrintRightDivider; }

  public:

    // services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    // staff number
    bool                  fPrintLeftDivider;
    bool                  fPrintRightDivider;
};
typedef SMARTP<msrSystemDividers> S_msrSystemDividers;
EXP ostream& operator<< (ostream& os, const S_msrSystemDividers& elt);

//______________________________________________________________________________
class msrStaffLayout : public msrElement
{
/*
<!--
	Staff layout includes the vertical distance from the bottom
	line of the previous staff in this system to the top line
	of the staff specified by the number attribute. The
	optional number attribute refers to staff numbers within
	the part, from top to bottom on the system. A value of 1
	is assumed if not present. When used in the defaults
	element, the values apply to all parts. This value is
	ignored for the first staff in a system.
-->
<!ELEMENT staff-layout (staff-distance?)>
<!ELEMENT staff-distance %layout-tenths;>
<!ATTLIST staff-layout
    number CDATA #IMPLIED
>
*/

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStaffLayout> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStaffLayout (
      int inputLineNumber);

    virtual ~msrStaffLayout ();

  public:

    // set and get
    // ------------------------------------------------------

    // staff number
    void                  setStaffNumber (int val)
                              { fStaffNumber = val; }
    int                   getStaffNumber () const
                              { return fStaffNumber; }

    // staff distance
    void                  setStaffDistance (S_msrLength val)
                              { fStaffDistance = val; }
    S_msrLength           getStaffDistance () const
                              { return fStaffDistance; }

  public:

    // services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    // staff number
    int                   fStaffNumber;

    // staff distance
    S_msrLength           fStaffDistance;
};
typedef SMARTP<msrStaffLayout> S_msrStaffLayout;
EXP ostream& operator<< (ostream& os, const S_msrStaffLayout& elt);

//______________________________________________________________________________
class msrMeasureLayout : public msrElement
{
/*
<!--
	Measure layout includes the horizontal distance from the
	previous measure. This value is only used for systems
	where there is horizontal whitespace in the middle of a
	system, as in systems with codas. To specify the measure
	width, use the width attribute of the measure element.
-->
<!ELEMENT measure-layout (measure-distance?)>
<!ELEMENT measure-distance %layout-tenths;>
*/

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasureLayout> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMeasureLayout (
      int inputLineNumber);

    virtual ~msrMeasureLayout ();

  public:

    // set and get
    // ------------------------------------------------------

    // measure distance
    void                  setMeasureDistance (S_msrLength val)
                              { fMeasureDistance = val; }
    S_msrLength           getMeasureDistance () const
                              { return fMeasureDistance; }

    // services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    // measure distance
    S_msrLength           fMeasureDistance;
};
typedef SMARTP<msrMeasureLayout> S_msrMeasureLayout;
EXP ostream& operator<< (ostream& os, const S_msrMeasureLayout& elt);


} // namespace MusicXML2


#endif
