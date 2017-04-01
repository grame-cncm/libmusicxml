/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msrVersion__
#define __msrVersion__

#include <string>
#include <list>

using namespace std;

namespace MusicXML2 
{

/*!
\addtogroup version
@{
*/


//______________________________________________________________________________
class versionInfo
{
  public:

    versionInfo (
      string  versionNumber,
      string  versionDate,
      string  versionDescription);

    virtual ~versionInfo ();
    
    void print (ostream& os) const;

  public:

    string  fVersionNumber;
    string  fVersionDate;
    string  fVersionDescription;
};

extern list<versionInfo> gVersionInfoList;

void enlistVersion (
  string  versionNumber,
  string  versionDate,
  string  versionDescription);

void printVersionsHistory (ostream& os);

string   currentVersionNumber ();

/*! @} */

}


#endif
