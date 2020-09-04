/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___mxmlTree___
#define ___mxmlTree___

#include <string>

#include "elements.h"
#include "factory.h"
#include "xml.h"
#include "xmlfile.h"
#include "exports.h"


using namespace std;

namespace MusicXML2
{

//------------------------------------------------------------------------
string meiElementAsString (Sxmlelement elem);

//------------------------------------------------------------------------
Sxmlelement createMeiElement (int type, const string& value);

Sxmlelement createMeiIntegerElement (int type, int value);

Sxmlelement createMeiFloatElement (int type, float value);

//------------------------------------------------------------------------
Sxmlattribute createMeiAttribute (const string& name, const string& value);

Sxmlattribute createMeiIntegerAttribute (const string& name, int value);

Sxmlattribute createMeiFloatAttribute (const string& name, float value);

//------------------------------------------------------------------------
EXP SXMLFile createMeiFile ();

//------------------------------------------------------------------------
Sxmlelement createMeiScorePartWiseElement ();

}


#endif
