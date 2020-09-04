/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

//#include <iomanip>      // setw, setprecision, ...
//#include <cmath>
//#include <string>

#include <sstream>

#include "meiTree.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
string meiElementAsString (Sxmlelement elem)
{
  int                 elemType = elem->getType ();
  const std::string&  elemName = elem->getName ();
  const std::vector<Sxmlattribute>&
                      elemAttributes = elem->attributes();

  stringstream s;

  s <<
    "meielement " <<
    ", elemType: " << elemType <<
    ", elemName: " << elemName <<
    ", elemAttributes.size ():" << elemAttributes.size ();

  return s.str ();
}

//------------------------------------------------------------------------
Sxmlelement createMeiElement (int type, const string& value)
{
	Sxmlelement result = factory::instance().create(type);

	result->setValue (value);

	return result;
}

Sxmlelement createMeiIntegerElement (int type, int value)
{
	Sxmlelement result = factory::instance().create(type);

	result->setValue (value);

	return result;
}

Sxmlelement createMeiFloatElement (int type, float value)
{
	Sxmlelement result = factory::instance().create(type);

	result->setValue (value);

	return result;
}

//------------------------------------------------------------------------
Sxmlattribute createMeiAttribute (const string& name, const string& value)
{
	Sxmlattribute result = xmlattribute::create();

	result->setName(name);
	result->setValue(value);

	return result;
}

Sxmlattribute createMeiIntegerAttribute (const string& name, int value)
{
	Sxmlattribute result = xmlattribute::create();

	result->setName(name);
	result->setValue(value);

	return result;
}

Sxmlattribute createMeiFloatAttribute (const string& name, float value)
{
	Sxmlattribute result = xmlattribute::create();

	result->setName(name);
	result->setValue(value);

	return result;
}

//------------------------------------------------------------------------
SXMLFile createMeiFile ()
{
	SXMLFile result = TXMLFile::create ();

  TXMLDecl * xmlDecl = new TXMLDecl ("1.0", "UTF-8", TXMLDecl::kNo);
	result->set (xmlDecl);

	TDocType * docType = new TDocType ("score-partwise");
	result->set (docType);

	return result;
}

//------------------------------------------------------------------------
Sxmlelement createMeiScorePartWiseElement ()
{
  Sxmlelement result = factory::instance().create(k_score_partwise);

  Sxmlattribute versionAttribute = createMeiAttribute("version", "3.1");
  result->add (versionAttribute);

  return result;
}


} // namespace

