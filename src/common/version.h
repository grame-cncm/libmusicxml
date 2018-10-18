/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___version___
#define ___version___

#include <string>
#include <list>

#include "exports.h"

namespace MusicXML2 
{

//______________________________________________________________________________
class EXP versionInfo
{
  public:

    versionInfo (
      std::string versionNumber,
      std::string versionDate,
      std::string versionDescription);

    virtual ~versionInfo ();
    
    void print (std::ostream& os) const;

  public:

    std::string           fVersionNumber;
    std::string           fVersionDate;
    std::string           fVersionDescription;
};

extern std::list<versionInfo> gVersionInfoList;

void enlistVersion (
  std::string versionNumber,
  std::string versionDate,
  std::string versionDescription);

void printVersionsHistory (std::ostream& os);

std::string EXP currentVersionNumber ();


}


#endif
