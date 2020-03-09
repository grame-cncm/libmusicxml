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


using namespace std;

namespace MusicXML2
{

//------------------------------------------------------------------------
Sxmlelement createElement (int type, const string& value);

Sxmlelement createIntegerElement (int type, int value);

//------------------------------------------------------------------------
Sxmlattribute createAttribute (const string& name, const string& value);

Sxmlattribute createIntegerAttribute (const string& name, int value);

//------------------------------------------------------------------------
// JMISXMLFile createXMLFile ();

//------------------------------------------------------------------------
Sxmlelement createScorePartWiseElement ();

}


#endif
