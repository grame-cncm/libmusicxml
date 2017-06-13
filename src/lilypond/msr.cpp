/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <climits>      /* INT_MIN, INT_MAX */
#include <iostream>
#include <list>
#include <algorithm>
#include <iomanip>      // setw, set::precision, ...

#include "msr.h"

using namespace std;

namespace MusicXML2 
{

// notes names languages
// ------------------------------------------------------

map<string, msrQuatertonesPitchesLanguage>
  gQuatertonesPitchesLanguagesMap;

map<msrQuartertonesPitch, string> gNederlandsPitchName;
map<msrQuartertonesPitch, string> gCatalanPitchName;
map<msrQuartertonesPitch, string> gDeutschPitchName;
map<msrQuartertonesPitch, string> gEnglishPitchName;
map<msrQuartertonesPitch, string> gEspanolPitchName;
map<msrQuartertonesPitch, string> gFrancaisPitchName;
map<msrQuartertonesPitch, string> gItalianoPitchName;
map<msrQuartertonesPitch, string> gNorskPitchName;
map<msrQuartertonesPitch, string> gPortuguesPitchName;
map<msrQuartertonesPitch, string> gSuomiPitchName;
map<msrQuartertonesPitch, string> gSvenskaPitchName;
map<msrQuartertonesPitch, string> gVlaamsPitchName;

void initializePitchesLanguages ()
{
  gQuatertonesPitchesLanguagesMap ["nederlands"] = kNederlands;
  gQuatertonesPitchesLanguagesMap ["catalan"]    = kCatalan;
  gQuatertonesPitchesLanguagesMap ["deutsch"]    = kDeutsch;
  gQuatertonesPitchesLanguagesMap ["english"]    = kEnglish;
  gQuatertonesPitchesLanguagesMap ["espanol"]    = kEspanol;
  gQuatertonesPitchesLanguagesMap ["italiano"]   = kItaliano;
  gQuatertonesPitchesLanguagesMap ["francais"]   = kFrancais;
  gQuatertonesPitchesLanguagesMap ["norsk"]      = kNorsk;
  gQuatertonesPitchesLanguagesMap ["portugues"]  = kPortugues;
  gQuatertonesPitchesLanguagesMap ["suomi"]      = kSuomi;
  gQuatertonesPitchesLanguagesMap ["svenska"]    = kSvenska;
  gQuatertonesPitchesLanguagesMap ["vlaams"]     = kVlaams;

  // nederlands
  gNederlandsPitchName [k_aDoubleFlat]  = "aeses";
  gNederlandsPitchName [k_aSesquiFlat]  = "aeseh";
  gNederlandsPitchName [k_aFlat]        = "aes";
  gNederlandsPitchName [k_aSemiFlat]    = "aeh";
  gNederlandsPitchName [k_aNatural]     = "a";
  gNederlandsPitchName [k_aSemiSharp]   = "aih";
  gNederlandsPitchName [k_aSharp]       = "ais";
  gNederlandsPitchName [k_aSesquiSharp] = "aisih";
  gNederlandsPitchName [k_aDoubleSharp] = "aisis";
    
  gNederlandsPitchName [k_bDoubleFlat]  = "beses";
  gNederlandsPitchName [k_bSesquiFlat]  = "beseh";
  gNederlandsPitchName [k_bFlat]        = "bes";
  gNederlandsPitchName [k_bSemiFlat]    = "beh";
  gNederlandsPitchName [k_bNatural]     = "b";
  gNederlandsPitchName [k_bSemiSharp]   = "bih";
  gNederlandsPitchName [k_bSharp]       = "bis";
  gNederlandsPitchName [k_bSesquiSharp] = "bisih";
  gNederlandsPitchName [k_bDoubleSharp] = "bisis";
    
  gNederlandsPitchName [k_cDoubleFlat]  = "ceses";
  gNederlandsPitchName [k_cSesquiFlat]  = "ceseh";
  gNederlandsPitchName [k_cFlat]        = "ces";
  gNederlandsPitchName [k_cSemiFlat]    = "ceh";
  gNederlandsPitchName [k_cNatural]     = "c";
  gNederlandsPitchName [k_cSemiSharp]   = "cih";
  gNederlandsPitchName [k_cSharp]       = "cis";
  gNederlandsPitchName [k_cSesquiSharp] = "cisih";
  gNederlandsPitchName [k_cDoubleSharp] = "cisis";
    
  gNederlandsPitchName [k_dDoubleFlat]  = "deses";
  gNederlandsPitchName [k_dSesquiFlat]  = "deseh";
  gNederlandsPitchName [k_dFlat]        = "des";
  gNederlandsPitchName [k_dSemiFlat]    = "deh";
  gNederlandsPitchName [k_dNatural]     = "d";
  gNederlandsPitchName [k_dSemiSharp]   = "dih";
  gNederlandsPitchName [k_dSharp]       = "dis";
  gNederlandsPitchName [k_dSesquiSharp] = "disih";
  gNederlandsPitchName [k_dDoubleSharp] = "disis";

  gNederlandsPitchName [k_eDoubleFlat]  = "eeses";
  gNederlandsPitchName [k_eSesquiFlat]  = "eeseh";
  gNederlandsPitchName [k_eFlat]        = "ees";
  gNederlandsPitchName [k_eSemiFlat]    = "eeh";
  gNederlandsPitchName [k_eNatural]     = "e";
  gNederlandsPitchName [k_eSemiSharp]   = "eih";
  gNederlandsPitchName [k_eSharp]       = "eis";
  gNederlandsPitchName [k_eSesquiSharp] = "eisih";
  gNederlandsPitchName [k_eDoubleSharp] = "eisis";
    
  gNederlandsPitchName [k_fDoubleFlat]  = "feses";
  gNederlandsPitchName [k_fSesquiFlat]  = "feseh";
  gNederlandsPitchName [k_fFlat]        = "fes";
  gNederlandsPitchName [k_fSemiFlat]    = "feh";
  gNederlandsPitchName [k_fNatural]     = "f";
  gNederlandsPitchName [k_fSemiSharp]   = "fih";
  gNederlandsPitchName [k_fSharp]       = "fis";
  gNederlandsPitchName [k_fSesquiSharp] = "fisih";
  gNederlandsPitchName [k_fDoubleSharp] = "fisis";
    
  gNederlandsPitchName [k_gDoubleFlat]  = "geses";
  gNederlandsPitchName [k_gSesquiFlat]  = "geseh";
  gNederlandsPitchName [k_gFlat]        = "ges";
  gNederlandsPitchName [k_gSemiFlat]    = "geh";
  gNederlandsPitchName [k_gNatural]     = "g";
  gNederlandsPitchName [k_gSemiSharp]   = "gih";
  gNederlandsPitchName [k_gSharp]       = "gis";
  gNederlandsPitchName [k_gSesquiSharp] = "gisih";
  gNederlandsPitchName [k_gDoubleSharp] = "gisis";

  // catalan
  gCatalanPitchName [k_aDoubleFlat]  = "labb";
  gCatalanPitchName [k_aSesquiFlat]  = "labSesquiFlat???";
  gCatalanPitchName [k_aFlat]        = "lab";
  gCatalanPitchName [k_aSemiFlat]    = "aSemiFlat???";
  gCatalanPitchName [k_aNatural]     = "la";
  gCatalanPitchName [k_aSemiSharp]   = "aSemiSharp???";
  gCatalanPitchName [k_aSharp]       = "lad";
  gCatalanPitchName [k_aSesquiSharp] = "laSesquiSharp???";
  gCatalanPitchName [k_aDoubleSharp] = "ladd";
    
  gCatalanPitchName [k_bDoubleFlat]  = "sibb";
  gCatalanPitchName [k_bSesquiFlat]  = "sibSesquiFlat???";
  gCatalanPitchName [k_bFlat]        = "sib";
  gCatalanPitchName [k_bSemiFlat]    = "bSemiFlat???";
  gCatalanPitchName [k_bNatural]     = "b";
  gCatalanPitchName [k_bSemiSharp]   = "bSemiSharp???";
  gCatalanPitchName [k_bSharp]       = "sid";
  gCatalanPitchName [k_bSesquiSharp] = "siSesquiSharp???";
  gCatalanPitchName [k_bDoubleSharp] = "sidd";
    
  gCatalanPitchName [k_cDoubleFlat]  = "dobb";
  gCatalanPitchName [k_cSesquiFlat]  = "doSesquiFlat???";
  gCatalanPitchName [k_cFlat]        = "dob";
  gCatalanPitchName [k_cSemiFlat]    = "cSemiFlat???";
  gCatalanPitchName [k_cNatural]     = "do";
  gCatalanPitchName [k_cSemiSharp]   = "cSemiSharp???";
  gCatalanPitchName [k_cSharp]       = "dod";
  gCatalanPitchName [k_cSesquiSharp] = "doSesquiSharp???";
  gCatalanPitchName [k_cDoubleSharp] = "dodd";
    
  gCatalanPitchName [k_dDoubleFlat]  = "rebb";
  gCatalanPitchName [k_dSesquiFlat]  = "reSesquiFlat???";
  gCatalanPitchName [k_dFlat]        = "reb";
  gCatalanPitchName [k_dSemiFlat]    = "dSemiFlat???";
  gCatalanPitchName [k_dNatural]     = "re";
  gCatalanPitchName [k_dSemiSharp]   = "dSemiSharp???";
  gCatalanPitchName [k_dSharp]       = "red";
  gCatalanPitchName [k_dSesquiSharp] = "reSesquiSharp???";
  gCatalanPitchName [k_dDoubleSharp] = "redd";

  gCatalanPitchName [k_eDoubleFlat]  = "mibb";
  gCatalanPitchName [k_eSesquiFlat]  = "miSesquiFlat???";
  gCatalanPitchName [k_eFlat]        = "mib";
  gCatalanPitchName [k_eSemiFlat]    = "eSemiFlat???";
  gCatalanPitchName [k_eNatural]     = "mi";
  gCatalanPitchName [k_eSemiSharp]   = "eSemiSharp???";
  gCatalanPitchName [k_eSharp]       = "mid";
  gCatalanPitchName [k_eSesquiSharp] = "miSesquiSharp???";
  gCatalanPitchName [k_eDoubleSharp] = "midd";
    
  gCatalanPitchName [k_fDoubleFlat]  = "fabb";
  gCatalanPitchName [k_fSesquiFlat]  = "faSesquiFlat???";
  gCatalanPitchName [k_fFlat]        = "fab";
  gCatalanPitchName [k_fSemiFlat]    = "fSemiFlat???";
  gCatalanPitchName [k_fNatural]     = "fa";
  gCatalanPitchName [k_fSemiSharp]   = "fSemiSharp???";
  gCatalanPitchName [k_fSharp]       = "fad";
  gCatalanPitchName [k_fSesquiSharp] = "faSesquiSharp???";
  gCatalanPitchName [k_fDoubleSharp] = "fadd";
    
  gCatalanPitchName [k_gDoubleFlat]  = "solbb";
  gCatalanPitchName [k_gSesquiFlat]  = "solSesquiFlat???";
  gCatalanPitchName [k_gFlat]        = "solb";
  gCatalanPitchName [k_gSemiFlat]    = "gSemiFlat???";
  gCatalanPitchName [k_gNatural]     = "sol";
  gCatalanPitchName [k_gSemiSharp]   = "gSemiSharp???";
  gCatalanPitchName [k_gSharp]       = "sold";
  gCatalanPitchName [k_gSesquiSharp] = "solSesquiSharp???";
  gCatalanPitchName [k_gDoubleSharp] = "soldd";

  // deutsch
  gDeutschPitchName [k_aDoubleFlat]  = "asas";
  gDeutschPitchName [k_aSesquiFlat]  = "asah";
  gDeutschPitchName [k_aFlat]        = "as";
  gDeutschPitchName [k_aSemiFlat]    = "aeh";
  gDeutschPitchName [k_aNatural]     = "a";
  gDeutschPitchName [k_aSemiSharp]   = "aih";
  gDeutschPitchName [k_aSharp]       = "ais";
  gDeutschPitchName [k_aSesquiSharp] = "aisih";
  gDeutschPitchName [k_aDoubleSharp] = "aisis";
    
  gDeutschPitchName [k_bDoubleFlat]  = "heses";
  gDeutschPitchName [k_bSesquiFlat]  = "heseh";
  gDeutschPitchName [k_bFlat]        = "b";
  gDeutschPitchName [k_bSemiFlat]    = "beh";
  gDeutschPitchName [k_bNatural]     = "h";
  gDeutschPitchName [k_bSemiSharp]   = "hih";
  gDeutschPitchName [k_bSharp]       = "his";
  gDeutschPitchName [k_bSesquiSharp] = "hisih";
  gDeutschPitchName [k_bDoubleSharp] = "hisis";
    
  gDeutschPitchName [k_cDoubleFlat]  = "ceses";
  gDeutschPitchName [k_cSesquiFlat]  = "ceseh";
  gDeutschPitchName [k_cFlat]        = "ces";
  gDeutschPitchName [k_cSemiFlat]    = "ceh";
  gDeutschPitchName [k_cNatural]     = "c";
  gDeutschPitchName [k_cSemiSharp]   = "cih";
  gDeutschPitchName [k_cSharp]       = "cis";
  gDeutschPitchName [k_cSesquiSharp] = "cisih";
  gDeutschPitchName [k_cDoubleSharp] = "cisis";
    
  gDeutschPitchName [k_dDoubleFlat]  = "deses";
  gDeutschPitchName [k_dSesquiFlat]  = "deseh";
  gDeutschPitchName [k_dFlat]        = "des";
  gDeutschPitchName [k_dSemiFlat]    = "deh";
  gDeutschPitchName [k_dNatural]     = "d";
  gDeutschPitchName [k_dSemiSharp]   = "dih";
  gDeutschPitchName [k_dSharp]       = "dis";
  gDeutschPitchName [k_dSesquiSharp] = "disih";
  gDeutschPitchName [k_dDoubleSharp] = "disis";

  gDeutschPitchName [k_eDoubleFlat]  = "eses";
  gDeutschPitchName [k_eSesquiFlat]  = "esseh";
  gDeutschPitchName [k_eFlat]        = "es";
  gDeutschPitchName [k_eSemiFlat]    = "eeh";
  gDeutschPitchName [k_eNatural]     = "e";
  gDeutschPitchName [k_eSemiSharp]   = "eih";
  gDeutschPitchName [k_eSharp]       = "eis";
  gDeutschPitchName [k_eSesquiSharp] = "eisih";
  gDeutschPitchName [k_eDoubleSharp] = "eisis";
    
  gDeutschPitchName [k_fDoubleFlat]  = "feses";
  gDeutschPitchName [k_fSesquiFlat]  = "feseh";
  gDeutschPitchName [k_fFlat]        = "fes";
  gDeutschPitchName [k_fSemiFlat]    = "feh";
  gDeutschPitchName [k_fNatural]     = "f";
  gDeutschPitchName [k_fSemiSharp]   = "fih";
  gDeutschPitchName [k_fSharp]       = "fis";
  gDeutschPitchName [k_fSesquiSharp] = "fisih";
  gDeutschPitchName [k_fDoubleSharp] = "fisis";
    
  gDeutschPitchName [k_gDoubleFlat]  = "geses";
  gDeutschPitchName [k_gSesquiFlat]  = "geseh";
  gDeutschPitchName [k_gFlat]        = "ges";
  gDeutschPitchName [k_gSemiFlat]    = "geh";
  gDeutschPitchName [k_gNatural]     = "g";
  gDeutschPitchName [k_gSemiSharp]   = "gih";
  gDeutschPitchName [k_gSharp]       = "gis";
  gDeutschPitchName [k_gSesquiSharp] = "gisih";
  gDeutschPitchName [k_gDoubleSharp] = "gisis";

  // english
  gEnglishPitchName [k_aDoubleFlat]  = "aff";
  gEnglishPitchName [k_aSesquiFlat]  = "atqf";
  gEnglishPitchName [k_aFlat]        = "af";
  gEnglishPitchName [k_aSemiFlat]    = "aqf";
  gEnglishPitchName [k_aNatural]     = "a";
  gEnglishPitchName [k_aSemiSharp]   = "aqs";
  gEnglishPitchName [k_aSharp]       = "as";
  gEnglishPitchName [k_aSesquiSharp] = "atqs";
  gEnglishPitchName [k_aDoubleSharp] = "a";
    
  gEnglishPitchName [k_bDoubleFlat]  = "bfqf";
  gEnglishPitchName [k_bSesquiFlat]  = "btqf";
  gEnglishPitchName [k_bFlat]        = "bf";
  gEnglishPitchName [k_bSemiFlat]    = "bqf";
  gEnglishPitchName [k_bNatural]     = "b";
  gEnglishPitchName [k_bSemiSharp]   = "bqs";
  gEnglishPitchName [k_bSharp]       = "bs";
  gEnglishPitchName [k_bSesquiSharp] = "btqs";
  gEnglishPitchName [k_bDoubleSharp] = "bx";
    
  gEnglishPitchName [k_cDoubleFlat]  = "cff";
  gEnglishPitchName [k_cSesquiFlat]  = "ctqf";
  gEnglishPitchName [k_cFlat]        = "cf";
  gEnglishPitchName [k_cSemiFlat]    = "cqf";
  gEnglishPitchName [k_cNatural]     = "c";
  gEnglishPitchName [k_cSemiSharp]   = "cqs";
  gEnglishPitchName [k_cSharp]       = "cs";
  gEnglishPitchName [k_cSesquiSharp] = "ctqs";
  gEnglishPitchName [k_cDoubleSharp] = "cx";
    
  gEnglishPitchName [k_dDoubleFlat]  = "dff";
  gEnglishPitchName [k_dSesquiFlat]  = "dtqf";
  gEnglishPitchName [k_dFlat]        = "df";
  gEnglishPitchName [k_dSemiFlat]    = "dqf";
  gEnglishPitchName [k_dNatural]     = "d";
  gEnglishPitchName [k_dSemiSharp]   = "dqs";
  gEnglishPitchName [k_dSharp]       = "ds";
  gEnglishPitchName [k_dSesquiSharp] = "dtqs";
  gEnglishPitchName [k_dDoubleSharp] = "dx";

  gEnglishPitchName [k_eDoubleFlat]  = "eff";
  gEnglishPitchName [k_eSesquiFlat]  = "etqf";
  gEnglishPitchName [k_eFlat]        = "ef";
  gEnglishPitchName [k_eSemiFlat]    = "eqf";
  gEnglishPitchName [k_eNatural]     = "e";
  gEnglishPitchName [k_eSemiSharp]   = "eqs";
  gEnglishPitchName [k_eSharp]       = "es";
  gEnglishPitchName [k_eSesquiSharp] = "etqs";
  gEnglishPitchName [k_eDoubleSharp] = "ex";
    
  gEnglishPitchName [k_fDoubleFlat]  = "fff";
  gEnglishPitchName [k_fSesquiFlat]  = "ftqf";
  gEnglishPitchName [k_fFlat]        = "ff";
  gEnglishPitchName [k_fSemiFlat]    = "fqf";
  gEnglishPitchName [k_fNatural]     = "f";
  gEnglishPitchName [k_fSemiSharp]   = "fqs";
  gEnglishPitchName [k_fSharp]       = "fs";
  gEnglishPitchName [k_fSesquiSharp] = "ftqs";
  gEnglishPitchName [k_fDoubleSharp] = "fx";
    
  gEnglishPitchName [k_gDoubleFlat]  = "gff";
  gEnglishPitchName [k_gSesquiFlat]  = "gtqf";
  gEnglishPitchName [k_gFlat]        = "gf";
  gEnglishPitchName [k_gSemiFlat]    = "gqf";
  gEnglishPitchName [k_gNatural]     = "g";
  gEnglishPitchName [k_gSemiSharp]   = "gqs";
  gEnglishPitchName [k_gSharp]       = "gs";
  gEnglishPitchName [k_gSesquiSharp] = "gtqs";
  gEnglishPitchName [k_gDoubleSharp] = "gx";

  // espanol
  gEspanolPitchName [k_aDoubleFlat]  = "labb";
  gEspanolPitchName [k_aSesquiFlat]  = "latcb";
  gEspanolPitchName [k_aFlat]        = "lab";
  gEspanolPitchName [k_aSemiFlat]    = "lacb";
  gEspanolPitchName [k_aNatural]     = "la";
  gEspanolPitchName [k_aSemiSharp]   = "lacs";
  gEspanolPitchName [k_aSharp]       = "las";
  gEspanolPitchName [k_aSesquiSharp] = "latcs";
  gEspanolPitchName [k_aDoubleSharp] = "lax";
    
  gEspanolPitchName [k_bDoubleFlat]  = "sibb";
  gEspanolPitchName [k_bSesquiFlat]  = "sitcb";
  gEspanolPitchName [k_bFlat]        = "sib";
  gEspanolPitchName [k_bSemiFlat]    = "sicb";
  gEspanolPitchName [k_bNatural]     = "si";
  gEspanolPitchName [k_bSemiSharp]   = "sics";
  gEspanolPitchName [k_bSharp]       = "sis";
  gEspanolPitchName [k_bSesquiSharp] = "sitcs";
  gEspanolPitchName [k_bDoubleSharp] = "six";
    
  gEspanolPitchName [k_cDoubleFlat]  = "dobb";
  gEspanolPitchName [k_cSesquiFlat]  = "dotcb";
  gEspanolPitchName [k_cFlat]        = "dob";
  gEspanolPitchName [k_cSemiFlat]    = "docb";
  gEspanolPitchName [k_cNatural]     = "do";
  gEspanolPitchName [k_cSemiSharp]   = "docs";
  gEspanolPitchName [k_cSharp]       = "dos";
  gEspanolPitchName [k_cSesquiSharp] = "dotcs";
  gEspanolPitchName [k_cDoubleSharp] = "dox";
    
  gEspanolPitchName [k_dDoubleFlat]  = "rebb";
  gEspanolPitchName [k_dSesquiFlat]  = "retcb";
  gEspanolPitchName [k_dFlat]        = "reb";
  gEspanolPitchName [k_dSemiFlat]    = "recb";
  gEspanolPitchName [k_dNatural]     = "re";
  gEspanolPitchName [k_dSemiSharp]   = "recs";
  gEspanolPitchName [k_dSharp]       = "res";
  gEspanolPitchName [k_dSesquiSharp] = "retcs";
  gEspanolPitchName [k_dDoubleSharp] = "rex";

  gEspanolPitchName [k_eDoubleFlat]  = "mibb";
  gEspanolPitchName [k_eSesquiFlat]  = "mitcb";
  gEspanolPitchName [k_eFlat]        = "mib";
  gEspanolPitchName [k_eSemiFlat]    = "micb";
  gEspanolPitchName [k_eNatural]     = "mi";
  gEspanolPitchName [k_eSemiSharp]   = "mics";
  gEspanolPitchName [k_eSharp]       = "mis";
  gEspanolPitchName [k_eSesquiSharp] = "mitcs";
  gEspanolPitchName [k_eDoubleSharp] = "mix";
    
  gEspanolPitchName [k_fDoubleFlat]  = "fabb";
  gEspanolPitchName [k_fSesquiFlat]  = "fatcb";
  gEspanolPitchName [k_fFlat]        = "fab";
  gEspanolPitchName [k_fSemiFlat]    = "facb";
  gEspanolPitchName [k_fNatural]     = "fa";
  gEspanolPitchName [k_fSemiSharp]   = "facs";
  gEspanolPitchName [k_fSharp]       = "fas";
  gEspanolPitchName [k_fSesquiSharp] = "fatcs";
  gEspanolPitchName [k_fDoubleSharp] = "fax";
    
  gEspanolPitchName [k_gDoubleFlat]  = "solbb";
  gEspanolPitchName [k_gSesquiFlat]  = "soltcb";
  gEspanolPitchName [k_gFlat]        = "solb";
  gEspanolPitchName [k_gSemiFlat]    = "solcb";
  gEspanolPitchName [k_gNatural]     = "sol";
  gEspanolPitchName [k_gSemiSharp]   = "solcs";
  gEspanolPitchName [k_gSharp]       = "sols";
  gEspanolPitchName [k_gSesquiSharp] = "soltcs";
  gEspanolPitchName [k_gDoubleSharp] = "solx";

  // francais
  gFrancaisPitchName [k_aDoubleFlat]  = "labb";
  gFrancaisPitchName [k_aSesquiFlat]  = "labtqt";
  gFrancaisPitchName [k_aFlat]        = "lab";
  gFrancaisPitchName [k_aSemiFlat]    = "labqt";
  gFrancaisPitchName [k_aNatural]     = "la";
  gFrancaisPitchName [k_aSemiSharp]   = "lasqt";
  gFrancaisPitchName [k_aSharp]       = "lad";
  gFrancaisPitchName [k_aSesquiSharp] = "lastqt";
  gFrancaisPitchName [k_aDoubleSharp] = "lass";
    
  gFrancaisPitchName [k_bDoubleFlat]  = "sibb";
  gFrancaisPitchName [k_bSesquiFlat]  = "sibtqt";
  gFrancaisPitchName [k_bFlat]        = "sib";
  gFrancaisPitchName [k_bSemiFlat]    = "sibqt";
  gFrancaisPitchName [k_bNatural]     = "si";
  gFrancaisPitchName [k_bSemiSharp]   = "sisqt";
  gFrancaisPitchName [k_bSharp]       = "sid";
  gFrancaisPitchName [k_bSesquiSharp] = "sistqt";
  gFrancaisPitchName [k_bDoubleSharp] = "siss";
    
  gFrancaisPitchName [k_cDoubleFlat]  = "dobb";
  gFrancaisPitchName [k_cSesquiFlat]  = "dobtqt";
  gFrancaisPitchName [k_cFlat]        = "dob";
  gFrancaisPitchName [k_cSemiFlat]    = "dobqt";
  gFrancaisPitchName [k_cNatural]     = "do";
  gFrancaisPitchName [k_cSemiSharp]   = "dosqt";
  gFrancaisPitchName [k_cSharp]       = "dod";
  gFrancaisPitchName [k_cSesquiSharp] = "dostqt";
  gFrancaisPitchName [k_cDoubleSharp] = "doss";
    
  gFrancaisPitchName [k_dDoubleFlat]  = "rebb";
  gFrancaisPitchName [k_dSesquiFlat]  = "rebtqt";
  gFrancaisPitchName [k_dFlat]        = "reb";
  gFrancaisPitchName [k_dSemiFlat]    = "rebqt";
  gFrancaisPitchName [k_dNatural]     = "re";
  gFrancaisPitchName [k_dSemiSharp]   = "resqt";
  gFrancaisPitchName [k_dSharp]       = "red";
  gFrancaisPitchName [k_dSesquiSharp] = "restqt";
  gFrancaisPitchName [k_dDoubleSharp] = "ress";

  gFrancaisPitchName [k_eDoubleFlat]  = "mibb";
  gFrancaisPitchName [k_eSesquiFlat]  = "mibtqt";
  gFrancaisPitchName [k_eFlat]        = "mib";
  gFrancaisPitchName [k_eSemiFlat]    = "mibqt";
  gFrancaisPitchName [k_eNatural]     = "mi";
  gFrancaisPitchName [k_eSemiSharp]   = "misqt";
  gFrancaisPitchName [k_eSharp]       = "mid";
  gFrancaisPitchName [k_eSesquiSharp] = "mistqt";
  gFrancaisPitchName [k_eDoubleSharp] = "miss";
    
  gFrancaisPitchName [k_fDoubleFlat]  = "fabb";
  gFrancaisPitchName [k_fSesquiFlat]  = "fabtqt";
  gFrancaisPitchName [k_fFlat]        = "fab";
  gFrancaisPitchName [k_fSemiFlat]    = "fabqt";
  gFrancaisPitchName [k_fNatural]     = "fa";
  gFrancaisPitchName [k_fSemiSharp]   = "fasqt";
  gFrancaisPitchName [k_fSharp]       = "fad";
  gFrancaisPitchName [k_fSesquiSharp] = "fastqt";
  gFrancaisPitchName [k_fDoubleSharp] = "fass";
    
  gFrancaisPitchName [k_gDoubleFlat]  = "solbb";
  gFrancaisPitchName [k_gSesquiFlat]  = "solbtqt";
  gFrancaisPitchName [k_gFlat]        = "solb";
  gFrancaisPitchName [k_gSemiFlat]    = "solbqt";
  gFrancaisPitchName [k_gNatural]     = "sol";
  gFrancaisPitchName [k_gSemiSharp]   = "solsqt";
  gFrancaisPitchName [k_gSharp]       = "sold";
  gFrancaisPitchName [k_gSesquiSharp] = "solstqt";
  gFrancaisPitchName [k_gDoubleSharp] = "solss";

  // italiano
  gItalianoPitchName [k_aDoubleFlat]  = "labb";
  gItalianoPitchName [k_aSesquiFlat]  = "labsb";
  gItalianoPitchName [k_aFlat]        = "lab";
  gItalianoPitchName [k_aSemiFlat]    = "lasb";
  gItalianoPitchName [k_aNatural]     = "la";
  gItalianoPitchName [k_aSemiSharp]   = "lasd";
  gItalianoPitchName [k_aSharp]       = "lad";
  gItalianoPitchName [k_aSesquiSharp] = "ladsd";
  gItalianoPitchName [k_aDoubleSharp] = "ladd";
    
  gItalianoPitchName [k_bDoubleFlat]  = "sibb";
  gItalianoPitchName [k_bSesquiFlat]  = "sibsb";
  gItalianoPitchName [k_bFlat]        = "sib";
  gItalianoPitchName [k_bSemiFlat]    = "sisb";
  gItalianoPitchName [k_bNatural]     = "si";
  gItalianoPitchName [k_bSemiSharp]   = "sisd";
  gItalianoPitchName [k_bSharp]       = "sid";
  gItalianoPitchName [k_bSesquiSharp] = "sidsd";
  gItalianoPitchName [k_bDoubleSharp] = "sidd";
    
  gItalianoPitchName [k_cDoubleFlat]  = "dobb";
  gItalianoPitchName [k_cSesquiFlat]  = "dobsb";
  gItalianoPitchName [k_cFlat]        = "dob";
  gItalianoPitchName [k_cSemiFlat]    = "dosb";
  gItalianoPitchName [k_cNatural]     = "do";
  gItalianoPitchName [k_cSemiSharp]   = "dosd";
  gItalianoPitchName [k_cSharp]       = "dod";
  gItalianoPitchName [k_cSesquiSharp] = "dodsd";
  gItalianoPitchName [k_cDoubleSharp] = "dodd";
    
  gItalianoPitchName [k_dDoubleFlat]  = "rebb";
  gItalianoPitchName [k_dSesquiFlat]  = "rebsb";
  gItalianoPitchName [k_dFlat]        = "reb";
  gItalianoPitchName [k_dSemiFlat]    = "resb";
  gItalianoPitchName [k_dNatural]     = "re";
  gItalianoPitchName [k_dSemiSharp]   = "resd";
  gItalianoPitchName [k_dSharp]       = "red";
  gItalianoPitchName [k_dSesquiSharp] = "redsd";
  gItalianoPitchName [k_dDoubleSharp] = "redd";

  gItalianoPitchName [k_eDoubleFlat]  = "mibb";
  gItalianoPitchName [k_eSesquiFlat]  = "mibsb";
  gItalianoPitchName [k_eFlat]        = "mib";
  gItalianoPitchName [k_eSemiFlat]    = "misb";
  gItalianoPitchName [k_eNatural]     = "mi";
  gItalianoPitchName [k_eSemiSharp]   = "misd";
  gItalianoPitchName [k_eSharp]       = "mid";
  gItalianoPitchName [k_eSesquiSharp] = "midsd";
  gItalianoPitchName [k_eDoubleSharp] = "midd";
    
  gItalianoPitchName [k_fDoubleFlat]  = "fabb";
  gItalianoPitchName [k_fSesquiFlat]  = "fabsb";
  gItalianoPitchName [k_fFlat]        = "fab";
  gItalianoPitchName [k_fSemiFlat]    = "fasb";
  gItalianoPitchName [k_fNatural]     = "fa";
  gItalianoPitchName [k_fSemiSharp]   = "fasd";
  gItalianoPitchName [k_fSharp]       = "fad";
  gItalianoPitchName [k_fSesquiSharp] = "fadsd";
  gItalianoPitchName [k_fDoubleSharp] = "fadd";
    
  gItalianoPitchName [k_gDoubleFlat]  = "solbb";
  gItalianoPitchName [k_gSesquiFlat]  = "solbsb";
  gItalianoPitchName [k_gFlat]        = "solb";
  gItalianoPitchName [k_gSemiFlat]    = "solsb";
  gItalianoPitchName [k_gNatural]     = "sol";
  gItalianoPitchName [k_gSemiSharp]   = "solsd";
  gItalianoPitchName [k_gSharp]       = "sold";
  gItalianoPitchName [k_gSesquiSharp] = "soldsd";
  gItalianoPitchName [k_gDoubleSharp] = "soldd";

  // norsk
  gNorskPitchName [k_aDoubleFlat]  = "aeses";
  gNorskPitchName [k_aSesquiFlat]  = "aSesquiFlat???";
  gNorskPitchName [k_aFlat]        = "aes";
  gNorskPitchName [k_aSemiFlat]    = "aSemiFlat???";
  gNorskPitchName [k_aNatural]     = "a";
  gNorskPitchName [k_aSemiSharp]   = "aSemiSharp???";
  gNorskPitchName [k_aSharp]       = "ais";
  gNorskPitchName [k_aSesquiSharp] = "aSesquiSharp???";
  gNorskPitchName [k_aDoubleSharp] = "aisis";
    
  gNorskPitchName [k_bDoubleFlat]  = "beses";
  gNorskPitchName [k_bSesquiFlat]  = "bSesquiFlat???";
  gNorskPitchName [k_bFlat]        = "bes";
  gNorskPitchName [k_bSemiFlat]    = "bSemiFlat???";
  gNorskPitchName [k_bNatural]     = "b";
  gNorskPitchName [k_bSemiSharp]   = "bSemiSharp???";
  gNorskPitchName [k_bSharp]       = "bis";
  gNorskPitchName [k_bSesquiSharp] = "bSesquiSharp???";
  gNorskPitchName [k_bDoubleSharp] = "bisis";
    
  gNorskPitchName [k_cDoubleFlat]  = "ceses";
  gNorskPitchName [k_cSesquiFlat]  = "cSesquiFlat???";
  gNorskPitchName [k_cFlat]        = "ces";
  gNorskPitchName [k_cSemiFlat]    = "cSemiFlat???";
  gNorskPitchName [k_cNatural]     = "c";
  gNorskPitchName [k_cSemiSharp]   = "cSemiSharp???";
  gNorskPitchName [k_cSharp]       = "cis";
  gNorskPitchName [k_cSesquiSharp] = "cSesquiSharp???";
  gNorskPitchName [k_cDoubleSharp] = "cisis";
    
  gNorskPitchName [k_dDoubleFlat]  = "deses";
  gNorskPitchName [k_dSesquiFlat]  = "dSesquiFlat???";
  gNorskPitchName [k_dFlat]        = "des";
  gNorskPitchName [k_dSemiFlat]    = "dSemiFlat???";
  gNorskPitchName [k_dNatural]     = "d";
  gNorskPitchName [k_dSemiSharp]   = "dSemiSharp???";
  gNorskPitchName [k_dSharp]       = "dis";
  gNorskPitchName [k_dSesquiSharp] = "dSesquiSharp???";
  gNorskPitchName [k_dDoubleSharp] = "disis";

  gNorskPitchName [k_eDoubleFlat]  = "eeses";
  gNorskPitchName [k_eSesquiFlat]  = "eSesquiFlat???";
  gNorskPitchName [k_eFlat]        = "ees";
  gNorskPitchName [k_eSemiFlat]    = "eSemiFlat???";
  gNorskPitchName [k_eNatural]     = "e";
  gNorskPitchName [k_eSemiSharp]   = "eSemiSharp???";
  gNorskPitchName [k_eSharp]       = "eis";
  gNorskPitchName [k_eSesquiSharp] = "eSesquiSharp???";
  gNorskPitchName [k_eDoubleSharp] = "eisis";
    
  gNorskPitchName [k_fDoubleFlat]  = "feses";
  gNorskPitchName [k_fSesquiFlat]  = "fSesquiFlat???";
  gNorskPitchName [k_fFlat]        = "fes";
  gNorskPitchName [k_fSemiFlat]    = "fSemiFlat???";
  gNorskPitchName [k_fNatural]     = "f";
  gNorskPitchName [k_fSemiSharp]   = "fSemiSharp???";
  gNorskPitchName [k_fSharp]       = "fis";
  gNorskPitchName [k_fSesquiSharp] = "fSesquiSharp???";
  gNorskPitchName [k_fDoubleSharp] = "fisis";
    
  gNorskPitchName [k_gDoubleFlat]  = "geses";
  gNorskPitchName [k_gSesquiFlat]  = "gSesquiFlat???";
  gNorskPitchName [k_gFlat]        = "ges";
  gNorskPitchName [k_gSemiFlat]    = "gSemiFlat???";
  gNorskPitchName [k_gNatural]     = "g";
  gNorskPitchName [k_gSemiSharp]   = "gSemiSharp???";
  gNorskPitchName [k_gSharp]       = "gis";
  gNorskPitchName [k_gSesquiSharp] = "gSesquiSharp???";
  gNorskPitchName [k_gDoubleSharp] = "gisis";

  // portugues
  gPortuguesPitchName [k_aDoubleFlat]  = "labb";
  gPortuguesPitchName [k_aSesquiFlat]  = "labtqt";
  gPortuguesPitchName [k_aFlat]        = "lab";
  gPortuguesPitchName [k_aSemiFlat]    = "lasb";
  gPortuguesPitchName [k_aNatural]     = "la";
  gPortuguesPitchName [k_aSemiSharp]   = "lasd";
  gPortuguesPitchName [k_aSharp]       = "lad";
  gPortuguesPitchName [k_aSesquiSharp] = "ladsd";
  gPortuguesPitchName [k_aDoubleSharp] = "ladd";
    
  gPortuguesPitchName [k_bDoubleFlat]  = "sibb";
  gPortuguesPitchName [k_bSesquiFlat]  = "sibtqt";
  gPortuguesPitchName [k_bFlat]        = "sib";
  gPortuguesPitchName [k_bSemiFlat]    = "sisb";
  gPortuguesPitchName [k_bNatural]     = "si";
  gPortuguesPitchName [k_bSemiSharp]   = "sisd";
  gPortuguesPitchName [k_bSharp]       = "sid";
  gPortuguesPitchName [k_bSesquiSharp] = "sidsd";
  gPortuguesPitchName [k_bDoubleSharp] = "sidd";
    
  gPortuguesPitchName [k_cDoubleFlat]  = "dobb";
  gPortuguesPitchName [k_cSesquiFlat]  = "dobtqt";
  gPortuguesPitchName [k_cFlat]        = "dob";
  gPortuguesPitchName [k_cSemiFlat]    = "dosb";
  gPortuguesPitchName [k_cNatural]     = "do";
  gPortuguesPitchName [k_cSemiSharp]   = "dosd";
  gPortuguesPitchName [k_cSharp]       = "dod";
  gPortuguesPitchName [k_cSesquiSharp] = "dodsd";
  gPortuguesPitchName [k_cDoubleSharp] = "dodd";
    
  gPortuguesPitchName [k_dDoubleFlat]  = "rebb";
  gPortuguesPitchName [k_dSesquiFlat]  = "rebtqt";
  gPortuguesPitchName [k_dFlat]        = "reb";
  gPortuguesPitchName [k_dSemiFlat]    = "resb";
  gPortuguesPitchName [k_dNatural]     = "re";
  gPortuguesPitchName [k_dSemiSharp]   = "resd";
  gPortuguesPitchName [k_dSharp]       = "red";
  gPortuguesPitchName [k_dSesquiSharp] = "redsd";
  gPortuguesPitchName [k_dDoubleSharp] = "redd";

  gPortuguesPitchName [k_eDoubleFlat]  = "mibb";
  gPortuguesPitchName [k_eSesquiFlat]  = "mibtqt";
  gPortuguesPitchName [k_eFlat]        = "mib";
  gPortuguesPitchName [k_eSemiFlat]    = "misb";
  gPortuguesPitchName [k_eNatural]     = "mi";
  gPortuguesPitchName [k_eSemiSharp]   = "misd";
  gPortuguesPitchName [k_eSharp]       = "mid";
  gPortuguesPitchName [k_eSesquiSharp] = "midsd";
  gPortuguesPitchName [k_eDoubleSharp] = "midd";
    
  gPortuguesPitchName [k_fDoubleFlat]  = "fabb";
  gPortuguesPitchName [k_fSesquiFlat]  = "fabtqt";
  gPortuguesPitchName [k_fFlat]        = "fab";
  gPortuguesPitchName [k_fSemiFlat]    = "fasb";
  gPortuguesPitchName [k_fNatural]     = "fa";
  gPortuguesPitchName [k_fSemiSharp]   = "fasd";
  gPortuguesPitchName [k_fSharp]       = "fad";
  gPortuguesPitchName [k_fSesquiSharp] = "fadsd";
  gPortuguesPitchName [k_fDoubleSharp] = "fadd";
    
  gPortuguesPitchName [k_gDoubleFlat]  = "solbb";
  gPortuguesPitchName [k_gSesquiFlat]  = "solbtqt";
  gPortuguesPitchName [k_gFlat]        = "solb";
  gPortuguesPitchName [k_gSemiFlat]    = "solsb";
  gPortuguesPitchName [k_gNatural]     = "sol";
  gPortuguesPitchName [k_gSemiSharp]   = "solsd";
  gPortuguesPitchName [k_gSharp]       = "sold";
  gPortuguesPitchName [k_gSesquiSharp] = "soldsd";
  gPortuguesPitchName [k_gDoubleSharp] = "soldd";

  // suomi
  gSuomiPitchName [k_aDoubleFlat]  = "asas";
  gSuomiPitchName [k_aSesquiFlat]  = "aSesquiFlat???";
  gSuomiPitchName [k_aFlat]        = "as";
  gSuomiPitchName [k_aSemiFlat]    = "aSemiFlat???";
  gSuomiPitchName [k_aNatural]     = "a";
  gSuomiPitchName [k_aSemiSharp]   = "aSemiSharp???";
  gSuomiPitchName [k_aSharp]       = "ais";
  gSuomiPitchName [k_aSesquiSharp] = "aSesquiSharp???";
  gSuomiPitchName [k_aDoubleSharp] = "aisis";
    
  gSuomiPitchName [k_bDoubleFlat]  = "bes";
  gSuomiPitchName [k_bSesquiFlat]  = "bSesquiFlat???";
  gSuomiPitchName [k_bFlat]        = "b";
  gSuomiPitchName [k_bSemiFlat]    = "bSemiFlat???";
  gSuomiPitchName [k_bNatural]     = "h";
  gSuomiPitchName [k_bSemiSharp]   = "bSemiSharp???";
  gSuomiPitchName [k_bSharp]       = "his";
  gSuomiPitchName [k_bSesquiSharp] = "bSesquiSharp???";
  gSuomiPitchName [k_bDoubleSharp] = "hisis";
    
  gSuomiPitchName [k_cDoubleFlat]  = "ceses";
  gSuomiPitchName [k_cSesquiFlat]  = "cSesquiFlat???";
  gSuomiPitchName [k_cFlat]        = "ces";
  gSuomiPitchName [k_cSemiFlat]    = "cSemiFlat???";
  gSuomiPitchName [k_cNatural]     = "c";
  gSuomiPitchName [k_cSemiSharp]   = "cSemiSharp???";
  gSuomiPitchName [k_cSharp]       = "cis";
  gSuomiPitchName [k_cSesquiSharp] = "cSesquiSharp???";
  gSuomiPitchName [k_cDoubleSharp] = "cisis";
    
  gSuomiPitchName [k_dDoubleFlat]  = "deses";
  gSuomiPitchName [k_dSesquiFlat]  = "dSesquiFlat???";
  gSuomiPitchName [k_dFlat]        = "des";
  gSuomiPitchName [k_dSemiFlat]    = "dSemiFlat???";
  gSuomiPitchName [k_dNatural]     = "d";
  gSuomiPitchName [k_dSemiSharp]   = "dSemiSharp???";
  gSuomiPitchName [k_dSharp]       = "dis";
  gSuomiPitchName [k_dSesquiSharp] = "dSesquiSharp???";
  gSuomiPitchName [k_dDoubleSharp] = "disis";

  gSuomiPitchName [k_eDoubleFlat]  = "eses";
  gSuomiPitchName [k_eSesquiFlat]  = "eSesquiFlat???";
  gSuomiPitchName [k_eFlat]        = "es";
  gSuomiPitchName [k_eSemiFlat]    = "eSemiFlat???";
  gSuomiPitchName [k_eNatural]     = "e";
  gSuomiPitchName [k_eSemiSharp]   = "eSemiSharp???";
  gSuomiPitchName [k_eSharp]       = "eis";
  gSuomiPitchName [k_eSesquiSharp] = "eSesquiSharp???";
  gSuomiPitchName [k_eDoubleSharp] = "eisis";
    
  gSuomiPitchName [k_fDoubleFlat]  = "feses";
  gSuomiPitchName [k_fSesquiFlat]  = "fSesquiFlat???";
  gSuomiPitchName [k_fFlat]        = "fes";
  gSuomiPitchName [k_fSemiFlat]    = "fSemiFlat???";
  gSuomiPitchName [k_fNatural]     = "f";
  gSuomiPitchName [k_fSemiSharp]   = "fSemiSharp???";
  gSuomiPitchName [k_fSharp]       = "fis";
  gSuomiPitchName [k_fSesquiSharp] = "fSesquiSharp???";
  gSuomiPitchName [k_fDoubleSharp] = "fisis";
    
  gSuomiPitchName [k_gDoubleFlat]  = "geses";
  gSuomiPitchName [k_gSesquiFlat]  = "gSesquiFlat???";
  gSuomiPitchName [k_gFlat]        = "ges";
  gSuomiPitchName [k_gSemiFlat]    = "gSemiFlat???";
  gSuomiPitchName [k_gNatural]     = "g";
  gSuomiPitchName [k_gSemiSharp]   = "gSemiSharp???";
  gSuomiPitchName [k_gSharp]       = "gis";
  gSuomiPitchName [k_gSesquiSharp] = "gSesquiSharp???";
  gSuomiPitchName [k_gDoubleSharp] = "gisis";

  // svenska
  gSvenskaPitchName [k_aDoubleFlat]  = "assess";
  gSvenskaPitchName [k_aSesquiFlat]  = "aSesquiFlat???";
  gSvenskaPitchName [k_aFlat]        = "ass";
  gSvenskaPitchName [k_aSemiFlat]    = "aSemiFlat???";
  gSvenskaPitchName [k_aNatural]     = "a";
  gSvenskaPitchName [k_aSemiSharp]   = "aSemiSharp???";
  gSvenskaPitchName [k_aSharp]       = "aiss";
  gSvenskaPitchName [k_aSesquiSharp] = "aSesquiSharp???";
  gSvenskaPitchName [k_aDoubleSharp] = "aississ";
    
  gSvenskaPitchName [k_bDoubleFlat]  = "hessess";
  gSvenskaPitchName [k_bSesquiFlat]  = "bSesquiFlat???";
  gSvenskaPitchName [k_bFlat]        = "b";
  gSvenskaPitchName [k_bSemiFlat]    = "bSemiFlat???";
  gSvenskaPitchName [k_bNatural]     = "h";
  gSvenskaPitchName [k_bSemiSharp]   = "bSemiSharp???";
  gSvenskaPitchName [k_bSharp]       = "hiss";
  gSvenskaPitchName [k_bSesquiSharp] = "bSesquiSharp???";
  gSvenskaPitchName [k_bDoubleSharp] = "hississ";
    
  gSvenskaPitchName [k_cDoubleFlat]  = "cessess";
  gSvenskaPitchName [k_cSesquiFlat]  = "cSesquiFlat???";
  gSvenskaPitchName [k_cFlat]        = "cess";
  gSvenskaPitchName [k_cSemiFlat]    = "cSemiFlat???";
  gSvenskaPitchName [k_cNatural]     = "c";
  gSvenskaPitchName [k_cSemiSharp]   = "cSemiSharp???";
  gSvenskaPitchName [k_cSharp]       = "ciss";
  gSvenskaPitchName [k_cSesquiSharp] = "cSesquiSharp???";
  gSvenskaPitchName [k_cDoubleSharp] = "cississ";
    
  gSvenskaPitchName [k_dDoubleFlat]  = "dessess";
  gSvenskaPitchName [k_dSesquiFlat]  = "dSesquiFlat???";
  gSvenskaPitchName [k_dFlat]        = "dess";
  gSvenskaPitchName [k_dSemiFlat]    = "dSemiFlat???";
  gSvenskaPitchName [k_dNatural]     = "d";
  gSvenskaPitchName [k_dSemiSharp]   = "dSemiSharp???";
  gSvenskaPitchName [k_dSharp]       = "diss";
  gSvenskaPitchName [k_dSesquiSharp] = "dSesquiSharp???";
  gSvenskaPitchName [k_dDoubleSharp] = "dississ";

  gSvenskaPitchName [k_eDoubleFlat]  = "essess";
  gSvenskaPitchName [k_eSesquiFlat]  = "eSesquiFlat???";
  gSvenskaPitchName [k_eFlat]        = "ess";
  gSvenskaPitchName [k_eSemiFlat]    = "eSemiFlat???";
  gSvenskaPitchName [k_eNatural]     = "e";
  gSvenskaPitchName [k_eSemiSharp]   = "eSemiSharp???";
  gSvenskaPitchName [k_eSharp]       = "eiss";
  gSvenskaPitchName [k_eSesquiSharp] = "eSesquiSharp???";
  gSvenskaPitchName [k_eDoubleSharp] = "eississ";
    
  gSvenskaPitchName [k_fDoubleFlat]  = "fessess";
  gSvenskaPitchName [k_fSesquiFlat]  = "fSesquiFlat???";
  gSvenskaPitchName [k_fFlat]        = "fess";
  gSvenskaPitchName [k_fSemiFlat]    = "fSemiFlat???";
  gSvenskaPitchName [k_fNatural]     = "f";
  gSvenskaPitchName [k_fSemiSharp]   = "fSemiSharp???";
  gSvenskaPitchName [k_fSharp]       = "fiss";
  gSvenskaPitchName [k_fSesquiSharp] = "fSesquiSharp???";
  gSvenskaPitchName [k_fDoubleSharp] = "fississ";
    
  gSvenskaPitchName [k_gDoubleFlat]  = "gessess";
  gSvenskaPitchName [k_gSesquiFlat]  = "gSesquiFlat???";
  gSvenskaPitchName [k_gFlat]        = "gess";
  gSvenskaPitchName [k_gSemiFlat]    = "gSemiFlat???";
  gSvenskaPitchName [k_gNatural]     = "g";
  gSvenskaPitchName [k_gSemiSharp]   = "gSemiSharp???";
  gSvenskaPitchName [k_gSharp]       = "giss";
  gSvenskaPitchName [k_gSesquiSharp] = "gSesquiSharp???";
  gSvenskaPitchName [k_gDoubleSharp] = "gississ";

  // vlaams
  gVlaamsPitchName [k_aDoubleFlat]  = "labb";
  gVlaamsPitchName [k_aSesquiFlat]  = "laSesquiFlat???";
  gVlaamsPitchName [k_aFlat]        = "lab";
  gVlaamsPitchName [k_aSemiFlat]    = "laSemiFlat???";
  gVlaamsPitchName [k_aNatural]     = "la";
  gVlaamsPitchName [k_aSemiSharp]   = "laSemiSharp???";
  gVlaamsPitchName [k_aSharp]       = "lak";
  gVlaamsPitchName [k_aSesquiSharp] = "laSesquiSharp???";
  gVlaamsPitchName [k_aDoubleSharp] = "lakk";
    
  gVlaamsPitchName [k_bDoubleFlat]  = "sibb";
  gVlaamsPitchName [k_bSesquiFlat]  = "siSesquiFlat???";
  gVlaamsPitchName [k_bFlat]        = "sib";
  gVlaamsPitchName [k_bSemiFlat]    = "siSemiFlat???";
  gVlaamsPitchName [k_bNatural]     = "si";
  gVlaamsPitchName [k_bSemiSharp]   = "siSemiSharp???";
  gVlaamsPitchName [k_bSharp]       = "sik";
  gVlaamsPitchName [k_bSesquiSharp] = "siSesquiSharp???";
  gVlaamsPitchName [k_bDoubleSharp] = "sikk";
    
  gVlaamsPitchName [k_cDoubleFlat]  = "dobb";
  gVlaamsPitchName [k_cSesquiFlat]  = "doSesquiFlat???";
  gVlaamsPitchName [k_cFlat]        = "dob";
  gVlaamsPitchName [k_cSemiFlat]    = "doSemiFlat???";
  gVlaamsPitchName [k_cNatural]     = "do";
  gVlaamsPitchName [k_cSemiSharp]   = "doSemiSharp???";
  gVlaamsPitchName [k_cSharp]       = "dok";
  gVlaamsPitchName [k_cSesquiSharp] = "doSesquiSharp???";
  gVlaamsPitchName [k_cDoubleSharp] = "dokk";
    
  gVlaamsPitchName [k_dDoubleFlat]  = "rebb";
  gVlaamsPitchName [k_dSesquiFlat]  = "reSesquiFlat???";
  gVlaamsPitchName [k_dFlat]        = "reb";
  gVlaamsPitchName [k_dSemiFlat]    = "reSemiFlat???";
  gVlaamsPitchName [k_dNatural]     = "re";
  gVlaamsPitchName [k_dSemiSharp]   = "reSemiSharp???";
  gVlaamsPitchName [k_dSharp]       = "rek";
  gVlaamsPitchName [k_dSesquiSharp] = "reSesquiSharp???";
  gVlaamsPitchName [k_dDoubleSharp] = "rekk";

  gVlaamsPitchName [k_eDoubleFlat]  = "mibb";
  gVlaamsPitchName [k_eSesquiFlat]  = "miSesquiFlat???";
  gVlaamsPitchName [k_eFlat]        = "mib";
  gVlaamsPitchName [k_eSemiFlat]    = "miSemiFlat???";
  gVlaamsPitchName [k_eNatural]     = "mi";
  gVlaamsPitchName [k_eSemiSharp]   = "miSemiSharp???";
  gVlaamsPitchName [k_eSharp]       = "mik";
  gVlaamsPitchName [k_eSesquiSharp] = "miSesquiSharp???";
  gVlaamsPitchName [k_eDoubleSharp] = "mikk";
    
  gVlaamsPitchName [k_fDoubleFlat]  = "fabb";
  gVlaamsPitchName [k_fSesquiFlat]  = "faSesquiFlat???";
  gVlaamsPitchName [k_fFlat]        = "fab";
  gVlaamsPitchName [k_fSemiFlat]    = "faSemiFlat???";
  gVlaamsPitchName [k_fNatural]     = "fa";
  gVlaamsPitchName [k_fSemiSharp]   = "faSemiSharp???";
  gVlaamsPitchName [k_fSharp]       = "fak";
  gVlaamsPitchName [k_fSesquiSharp] = "faSesquiSharp???";
  gVlaamsPitchName [k_fDoubleSharp] = "fakk";
    
  gVlaamsPitchName [k_gDoubleFlat]  = "solbb";
  gVlaamsPitchName [k_gSesquiFlat]  = "solSesquiFlat???";
  gVlaamsPitchName [k_gFlat]        = "solb";
  gVlaamsPitchName [k_gSemiFlat]    = "solSemiFlat???";
  gVlaamsPitchName [k_gNatural]     = "sol";
  gVlaamsPitchName [k_gSemiSharp]   = "solSemiSharp???";
  gVlaamsPitchName [k_gSharp]       = "solk";
  gVlaamsPitchName [k_gSesquiSharp] = "solSesquiSharp???";
  gVlaamsPitchName [k_gDoubleSharp] = "solkk";
}

string msrDiatonicPitchAsString (
  msrDiatonicPitch diatonicPitch)
{
  string result;

  switch (diatonicPitch) {
    case kA:
      result = "A";
      break;
    case kB:
      result = "B";
      break;
    case kC:
      result = "C";
      break;
    case kD:
      result = "D";
      break;
    case kE:
      result = "E";
      break;
    case kF:
      result = "F";
      break;
    case kG:
      result = "G";
      break;
    case k_NoDiatonicPitch:
      result = "k_NoDiatonicPitch";
      break;
  } // switch
  return result;
}

msrDiatonicPitch msrDiatonicPitchFromString (
  char diatonicNoteName)
{
  msrDiatonicPitch result;
  
  switch (diatonicNoteName) {
    case 'a':
    case 'A':
      result = kA;
       break;
    case 'b':
    case 'B':
      result = kB;
      break;
    case 'c':
    case 'C':
      result = kC;
      break;
    case 'd':
    case 'D':
      result = kD;
      break;
    case 'e':
    case 'E':
      result = kE;
      break;
    case 'f':
    case 'F':
      result = kF;
      break;
    case 'g':
    case 'G':
      result = kG;
      break;
    default: {}
  } // switch

  return result;
}

string msrDiatonicPitchAsString (
  msrQuatertonesPitchesLanguage language,
  msrDiatonicPitch              diatonicPitch)
{
  string result;
  
  switch (diatonicPitch) {
    case kA:
      result = "a";
      break;
    case kB:
      result = "b";
      break;
    case kC:
      result = "c";
      break;
    case kD:
      result = "d";
      break;
    case kE:
      result = "e";
      break;
    case kF:
      result = "f";
      break;
    case kG:
      result = "g";
      break;
    case k_NoDiatonicPitch:
      result = "k_NoDiatonicPitch";
      break;
  } // switch

  return result;
}

string msrQuatertonesPitchesLanguageAsString (
  msrQuatertonesPitchesLanguage language)
{
  string result;
  
  switch (language) {
    case kNederlands:
      result = "nederlands";
      break;
    case kCatalan:
      result = "catalan";
      break;
    case kDeutsch:
      result = "deutsch";
      break;
    case kEnglish:
      result = "english";
      break;
    case kEspanol:
      result = "espanol";
      break;
    case kFrancais:
      result = "francais";
      break;
    case kItaliano:
      result = "italiano";
      break;
    case kNorsk:
      result = "norsk";
      break;
    case kPortugues:
      result = "portugues";
      break;
    case kSuomi:
      result = "suomi";
      break;
    case kSvenska:
      result = "svenska";
      break;
    case kVlaams:
      result = "vlaams";
      break;
  } // switch

  return result;
}

msrAlteration msrAlterationFromMusicXMLAlter (
  float alter)
{

/*
  The alter element represents chromatic alteration
  in number of semitones (e.g., -1 for flat, 1 for sharp).
  Decimal values like 0.5 (quarter tone sharp) are used for microtones:

  semi-sharp semi-flat sesqui-sharp sesqui-flat double-sharp double-flat
      +0.5      -0.5        +1.5       -1.5         +2.0        -2.0
*/

  msrAlteration result;
  
  if      (alter == 0 ) {
    result = kNatural;
  }
  
  else if (alter == -1 ) {
    result = kFlat;
  }
  
  else if (alter == 1 ) {
    result = kSharp;
  }
  
  else if (alter == -0.5 ) {
    result = kSemiFlat;
  }
  
  else if (alter == +0.5 ) {
    result = kSemiSharp;
  }
  
  else if (alter == -1.5 ) {
    result = kSesquiFlat;
  }
  
  else if (alter == +1.5 ) {
    result = kSesquiSharp;
  }
  
  else if (alter == -2 ) {
    result = kDoubleFlat;
  }
  
  else if (alter == +2 ) {
    result = kDoubleSharp;
  }
  
  else {
    result = k_NoAlteration;
  }

  return result;
}

string msrAlterationAsString (
  msrAlteration alteration)
{
  string result;
  
  switch (alteration) {
    case kDoubleFlat:
      result = "DoubleFlat";
      break;
    case kSesquiFlat:
      result = "SesquiFlat";
      break;
    case kFlat:
      result = "Flat";
      break;
    case kSemiFlat:
      result = "SemiFlat";
      break;
    case kNatural:
      result = "Natural";
      break;
    case kSemiSharp:
      result = "SemiSharp";
      break;
    case kSharp:
      result = "Sharp";
      break;
    case kSesquiSharp:
      result = "SesquiSharp";
      break;
    case kDoubleSharp:
      result = "DoubleSharp";
      break;
    case k_NoAlteration:
      result = "alteration???";
      break;
  } // switch

  return result;
}

void setDiatonicPitchAndAlteration (
  msrQuartertonesPitch quartertonesPitch,
  msrDiatonicPitch& diatonicPitch,
  msrAlteration& alteration)
{
  switch (quartertonesPitch) {
    case k_aDoubleFlat:
      diatonicPitch = kA;
      alteration    = kDoubleFlat;
      break;
    case k_aSesquiFlat:
      diatonicPitch = kA;
      alteration    = kSesquiFlat;
      break;
    case k_aFlat:
      diatonicPitch = kA;
      alteration    = kFlat;
      break;
    case k_aSemiFlat:
      diatonicPitch = kA;
      alteration    = kSemiFlat;
      break;
    case k_aNatural:
      diatonicPitch = kA;
      alteration    = kNatural;
      break;
    case k_aSemiSharp:
      diatonicPitch = kA;
      alteration    = kSemiSharp;
      break;
    case k_aSharp:
      diatonicPitch = kA;
      alteration    = kSharp;
      break;
    case k_aSesquiSharp:
      diatonicPitch = kA;
      alteration    = kSesquiSharp;
      break;
    case k_aDoubleSharp:
      diatonicPitch = kA;
      alteration    = kDoubleSharp;
      break;

    case k_bDoubleFlat:
      diatonicPitch = kB;
      alteration    = kDoubleFlat;
      break;
    case k_bSesquiFlat:
      diatonicPitch = kB;
      alteration    = kSesquiFlat;
      break;
    case k_bFlat:
      diatonicPitch = kB;
      alteration    = kFlat;
      break;
    case k_bSemiFlat:
      diatonicPitch = kB;
      alteration    = kSemiFlat;
      break;
    case k_bNatural:
      diatonicPitch = kB;
      alteration    = kNatural;
      break;
    case k_bSemiSharp:
      diatonicPitch = kB;
      alteration    = kSemiSharp;
      break;
    case k_bSharp:
      diatonicPitch = kB;
      alteration    = kSharp;
      break;
    case k_bSesquiSharp:
      diatonicPitch = kB;
      alteration    = kSesquiSharp;
      break;
    case k_bDoubleSharp:
      diatonicPitch = kB;
      alteration    = kDoubleSharp;
      break;

    case k_cDoubleFlat:
      diatonicPitch = kC;
      alteration    = kDoubleFlat;
      break;
    case k_cSesquiFlat:
      diatonicPitch = kC;
      alteration    = kSesquiFlat;
      break;
    case k_cFlat:
      diatonicPitch = kC;
      alteration    = kFlat;
      break;
    case k_cSemiFlat:
      diatonicPitch = kC;
      alteration    = kSemiFlat;
      break;
    case k_cNatural:
      diatonicPitch = kC;
      alteration    = kNatural;
      break;
    case k_cSemiSharp:
      diatonicPitch = kC;
      alteration    = kSemiSharp;
      break;
    case k_cSharp:
      diatonicPitch = kC;
      alteration    = kSharp;
      break;
    case k_cSesquiSharp:
      diatonicPitch = kC;
      alteration    = kSesquiSharp;
      break;
    case k_cDoubleSharp:
      diatonicPitch = kC;
      alteration    = kDoubleSharp;
      break;

    case k_dDoubleFlat:
      diatonicPitch = kD;
      alteration    = kDoubleFlat;
      break;
    case k_dSesquiFlat:
      diatonicPitch = kD;
      alteration    = kSesquiFlat;
      break;
    case k_dFlat:
      diatonicPitch = kD;
      alteration    = kFlat;
      break;
    case k_dSemiFlat:
      diatonicPitch = kD;
      alteration    = kSemiFlat;
      break;
    case k_dNatural:
      diatonicPitch = kD;
      alteration    = kNatural;
      break;
    case k_dSemiSharp:
      diatonicPitch = kD;
      alteration    = kSemiSharp;
      break;
    case k_dSharp:
      diatonicPitch = kD;
      alteration    = kSharp;
      break;
    case k_dSesquiSharp:
      diatonicPitch = kD;
      alteration    = kSesquiSharp;
      break;
    case k_dDoubleSharp:
      diatonicPitch = kD;
      alteration    = kDoubleSharp;
      break;

    case k_eDoubleFlat:
      diatonicPitch = kE;
      alteration    = kDoubleFlat;
      break;
    case k_eSesquiFlat:
      diatonicPitch = kE;
      alteration    = kSesquiFlat;
      break;
    case k_eFlat:
      diatonicPitch = kE;
      alteration    = kFlat;
      break;
    case k_eSemiFlat:
      diatonicPitch = kE;
      alteration    = kSemiFlat;
      break;
    case k_eNatural:
      diatonicPitch = kE;
      alteration    = kNatural;
      break;
    case k_eSemiSharp:
      diatonicPitch = kE;
      alteration    = kSemiSharp;
      break;
    case k_eSharp:
      diatonicPitch = kE;
      alteration    = kSharp;
      break;
    case k_eSesquiSharp:
      diatonicPitch = kE;
      alteration    = kSesquiSharp;
      break;
    case k_eDoubleSharp:
      diatonicPitch = kE;
      alteration    = kDoubleSharp;
      break;

    case k_fDoubleFlat:
      diatonicPitch = kF;
      alteration    = kDoubleFlat;
      break;
    case k_fSesquiFlat:
      diatonicPitch = kF;
      alteration    = kSesquiFlat;
      break;
    case k_fFlat:
      diatonicPitch = kF;
      alteration    = kFlat;
      break;
    case k_fSemiFlat:
      diatonicPitch = kF;
      alteration    = kSemiFlat;
      break;
    case k_fNatural:
      diatonicPitch = kF;
      alteration    = kNatural;
      break;
    case k_fSemiSharp:
      diatonicPitch = kF;
      alteration    = kSemiSharp;
      break;
    case k_fSharp:
      diatonicPitch = kF;
      alteration    = kSharp;
      break;
    case k_fSesquiSharp:
      diatonicPitch = kF;
      alteration    = kSesquiSharp;
      break;
    case k_fDoubleSharp:
      diatonicPitch = kF;
      alteration    = kDoubleSharp;
      break;

    case k_gDoubleFlat:
      diatonicPitch = kG;
      alteration    = kDoubleFlat;
      break;
    case k_gSesquiFlat:
      diatonicPitch = kG;
      alteration    = kSesquiFlat;
      break;
    case k_gFlat:
      diatonicPitch = kG;
      break;
    case k_gSemiFlat:
      diatonicPitch = kG;
      alteration    = kSemiFlat;
      break;
    case k_gNatural:
      diatonicPitch = kG;
      alteration    = kNatural;
      break;
    case k_gSemiSharp:
      diatonicPitch = kG;
      alteration    = kSemiSharp;
      break;
    case k_gSharp:
      diatonicPitch = kG;
      alteration    = kSharp;
      break;
    case k_gSesquiSharp:
      diatonicPitch = kG;
      alteration    = kSesquiSharp;
      break;
    case k_gDoubleSharp:
      diatonicPitch = kG;
      alteration    = kDoubleSharp;
      break;
      
    case k_Rest:
      diatonicPitch = kA; // any value would fit JMI
      alteration    = k_NoAlteration;
      
      break;
    case k_NoQuaterTonesPitch:
      diatonicPitch = kA; // any value would fit
      alteration    = k_NoAlteration;
      break;
  } // switch
}

msrQuartertonesPitch quatertonesPitchFromDiatonicPitchAndAlteration (
  int              inputLineNumber,
  msrDiatonicPitch diatonicPitch,
  msrAlteration    alteration)
{
  msrQuartertonesPitch result;
  
  switch (diatonicPitch) {
    case kA:
      switch (alteration) {
        case kDoubleFlat:
          result = k_aDoubleFlat;
          break;
        case kSesquiFlat:
          result = k_aSesquiFlat;
          break;
        case kFlat:
          result = k_aFlat;
          break;
        case kSemiFlat:
          result = k_aSemiFlat;
          break;
        case kNatural:
          result = k_aNatural;
          break;
        case kSemiSharp:
          result = k_aSemiSharp;
          break;
        case kSharp:
          result = k_aSharp;
          break;
        case kSesquiSharp:
          result = k_aSesquiSharp;
          break;
        case kDoubleSharp:
          result = k_aDoubleSharp;
          break;
        case k_NoAlteration:
          {
            stringstream s;

            s <<
              "'A' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              inputLineNumber,
              s.str());
          }
          break;
      } // switch
      break;
      
    case kB:
      switch (alteration) {
        case kDoubleFlat:
          result = k_bDoubleFlat;
          break;
        case kSesquiFlat:
          result = k_bSesquiFlat;
          break;
        case kFlat:
          result = k_bFlat;
          break;
        case kSemiFlat:
          result = k_bSemiFlat;
          break;
        case kNatural:
          result = k_bNatural;
          break;
        case kSemiSharp:
          result = k_bSemiSharp;
          break;
        case kSharp:
          result = k_bSharp;
          break;
        case kSesquiSharp:
          result = k_bSesquiSharp;
          break;
        case kDoubleSharp:
          result = k_bDoubleSharp;
          break;
        case k_NoAlteration:
          {
            stringstream s;

            s <<
              "'B' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              inputLineNumber,
              s.str());
          }
          break;
      } // switch
      break;
      
      break;
      
    case kC:
      switch (alteration) {
        case kDoubleFlat:
          result = k_cDoubleFlat;
          break;
        case kSesquiFlat:
          result = k_cSesquiFlat;
          break;
        case kFlat:
          result = k_cFlat;
          break;
        case kSemiFlat:
          result = k_cSemiFlat;
          break;
        case kNatural:
          result = k_cNatural;
          break;
        case kSemiSharp:
          result = k_cSemiSharp;
          break;
        case kSharp:
          result = k_cSharp;
          break;
        case kSesquiSharp:
          result = k_cSesquiSharp;
          break;
        case kDoubleSharp:
          result = k_cDoubleSharp;
          break;
        case k_NoAlteration:
          {
            stringstream s;

            s <<
              "'C' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              inputLineNumber,
              s.str());
          }
          break;
      } // switch
      break;
      
    case kD:
      switch (alteration) {
        case kDoubleFlat:
          result = k_dDoubleFlat;
          break;
        case kSesquiFlat:
          result = k_dSesquiFlat;
          break;
        case kFlat:
          result = k_dFlat;
          break;
        case kSemiFlat:
          result = k_dSemiFlat;
          break;
        case kNatural:
          result = k_dNatural;
          break;
        case kSemiSharp:
          result = k_dSemiSharp;
          break;
        case kSharp:
          result = k_dSharp;
          break;
        case kSesquiSharp:
          result = k_dSesquiSharp;
          break;
        case kDoubleSharp:
          result = k_dDoubleSharp;
          break;
        case k_NoAlteration:
          {
            stringstream s;

            s <<
              "'D' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              inputLineNumber,
              s.str());
          }
          break;
      } // switch
      break;
      
    case kE:
      switch (alteration) {
        case kDoubleFlat:
          result = k_eDoubleFlat;
          break;
        case kSesquiFlat:
          result = k_eSesquiFlat;
          break;
        case kFlat:
          result = k_eFlat;
          break;
        case kSemiFlat:
          result = k_eSemiFlat;
          break;
        case kNatural:
          result = k_eNatural;
          break;
        case kSemiSharp:
          result = k_eSemiSharp;
          break;
        case kSharp:
          result = k_eSharp;
          break;
        case kSesquiSharp:
          result = k_eSesquiSharp;
          break;
        case kDoubleSharp:
          result = k_eDoubleSharp;
          break;
        case k_NoAlteration:
          {
            stringstream s;

            s <<
              "'E' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              inputLineNumber,
              s.str());
          }
          break;
      } // switch
      break;
      
    case kF:
      switch (alteration) {
        case kDoubleFlat:
          result = k_fDoubleFlat;
          break;
        case kSesquiFlat:
          result = k_fSesquiFlat;
          break;
        case kFlat:
          result = k_fFlat;
          break;
        case kSemiFlat:
          result = k_fSemiFlat;
          break;
        case kNatural:
          result = k_fNatural;
          break;
        case kSemiSharp:
          result = k_fSemiSharp;
          break;
        case kSharp:
          result = k_fSharp;
          break;
        case kSesquiSharp:
          result = k_fSesquiSharp;
          break;
        case kDoubleSharp:
          result = k_fDoubleSharp;
          break;
        case k_NoAlteration:
          {
            stringstream s;

            s <<
              "'F' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              inputLineNumber,
              s.str());
          }
          break;
      } // switch
      break;
      
    case kG:
      switch (alteration) {
        case kDoubleFlat:
          result = k_gDoubleFlat;
          break;
        case kSesquiFlat:
          result = k_gSesquiFlat;
          break;
        case kFlat:
          result = k_gFlat;
          break;
        case kSemiFlat:
          result = k_gSemiFlat;
          break;
        case kNatural:
          result = k_gNatural;
          break;
        case kSemiSharp:
          result = k_gSemiSharp;
          break;
        case kSharp:
          result = k_gSharp;
          break;
        case kSesquiSharp:
          result = k_gSesquiSharp;
          break;
        case kDoubleSharp:
          result = k_gDoubleSharp;
          break;
        case k_NoAlteration:
          {
            stringstream s;

            s <<
              "'G' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              inputLineNumber,
              s.str());
          }
          break;
      } // switch
      break;

    case k_NoDiatonicPitch:
      {
        result = k_NoQuaterTonesPitch;
        /* JMI
        stringstream s;

        s <<
          "cannot convert k_NoDiatonicPitch to a quarter notes pitch"
          ", line = " << inputLineNumber;

        msrInternalError (
          inputLineNumber,
          s.str());
          */
      }
      break;    
  } // switch

  return result;
}

msrDiatonicPitch msrDiatonicPitchFromQuatertonesPitch (
  int                  inputLineNumber,
  msrQuartertonesPitch quartertonesPitch)
{
  msrDiatonicPitch result;
  
  switch (quartertonesPitch) {
    case k_aDoubleFlat:
    case k_aSesquiFlat:
    case k_aFlat:
    case k_aSemiFlat:
    case k_aNatural:
    case k_aSemiSharp:
    case k_aSharp:
    case k_aSesquiSharp:
    case k_aDoubleSharp:
      result = kA;
      break;

    case k_bDoubleFlat:
    case k_bSesquiFlat:
    case k_bFlat:
    case k_bSemiFlat:
    case k_bNatural:
    case k_bSemiSharp:
    case k_bSharp:
    case k_bSesquiSharp:
    case k_bDoubleSharp:
      result = kB;
      break;

    case k_cDoubleFlat:
    case k_cSesquiFlat:
    case k_cFlat:
    case k_cSemiFlat:
    case k_cNatural:
    case k_cSemiSharp:
    case k_cSharp:
    case k_cSesquiSharp:
    case k_cDoubleSharp:
      result = kC;
      break;

    case k_dDoubleFlat:
    case k_dSesquiFlat:
    case k_dFlat:
    case k_dSemiFlat:
    case k_dNatural:
    case k_dSemiSharp:
    case k_dSharp:
    case k_dSesquiSharp:
    case k_dDoubleSharp:
      result = kD;
      break;

    case k_eDoubleFlat:
    case k_eSesquiFlat:
    case k_eFlat:
    case k_eSemiFlat:
    case k_eNatural:
    case k_eSemiSharp:
    case k_eSharp:
    case k_eSesquiSharp:
    case k_eDoubleSharp:
      result = kE;
      break;

    case k_fDoubleFlat:
    case k_fSesquiFlat:
    case k_fFlat:
    case k_fSemiFlat:
    case k_fNatural:
    case k_fSemiSharp:
    case k_fSharp:
    case k_fSesquiSharp:
    case k_fDoubleSharp:
      result = kF;
      break;

    case k_gDoubleFlat:
    case k_gSesquiFlat:
    case k_gFlat:
    case k_gSemiFlat:
    case k_gNatural:
    case k_gSemiSharp:
    case k_gSharp:
    case k_gSesquiSharp:
    case k_gDoubleSharp:
      result = kG;
      break;

    case k_Rest:
      {
        stringstream s;

        s <<
          "cannot get the diatonic pitch of a rest"
          ", line = " << inputLineNumber;

        msrInternalError (
          inputLineNumber,
          s.str());
      }
      
    case k_NoQuaterTonesPitch:
      {
        result = k_NoDiatonicPitch;
        
        /* JMI
        stringstream s;

        s <<
          "cannot get the diatonic pitch of a k_NoQuaterTonesPitch"
          ", line = " << inputLineNumber;

        msrInternalError (
          inputLineNumber,
          s.str());
          */
      }
  } // switch

  return result;
}

string msrQuartertonesPitchAsString (
  msrQuatertonesPitchesLanguage language,
  msrQuartertonesPitch          quartertonesPitch)
{
   string result;
  
  switch (language) {
    case kNederlands:
      result = gNederlandsPitchName [quartertonesPitch];
      break;
    case kCatalan:
      result = gCatalanPitchName [quartertonesPitch];
      break;
    case kDeutsch:
      result = gDeutschPitchName [quartertonesPitch];
      break;
    case kEnglish:
      result = gEnglishPitchName [quartertonesPitch];
      break;
    case kEspanol:
      result = gEspanolPitchName [quartertonesPitch];
      break;
    case kFrancais:
      result = gFrancaisPitchName [quartertonesPitch];
      break;
    case kItaliano:
      result = gItalianoPitchName [quartertonesPitch];
      break;
    case kNorsk:
      result = gNorskPitchName [quartertonesPitch];
      break;
    case kPortugues:
      result = gPortuguesPitchName [quartertonesPitch];
      break;
    case kSuomi:
      result = gSuomiPitchName [quartertonesPitch];
      break;
    case kSvenska:
      result = gSvenskaPitchName [quartertonesPitch];
      break;
    case kVlaams:
      result = gVlaamsPitchName [quartertonesPitch];
      break;
  } // switch

  return result;
}

string existingQuartertonesPitchesLanguages ()
{
  stringstream s;
  
  map<string, msrQuatertonesPitchesLanguage>::const_iterator
    iBegin = gQuatertonesPitchesLanguagesMap.begin(),
    iEnd   = gQuatertonesPitchesLanguagesMap.end(),
    i      = iBegin;
  for ( ; ; ) {
    s << (*i).first;
    if (++i == iEnd) break;
    s << " ";
  } // for

  return s.str();
}

// durations
//______________________________________________________________________________
string msrDurationAsString (msrDuration duration)
{
  string result;

  switch (duration) {
    case k1024th:
      result = "1024";
      break;
    case k512th:
      result = "512";
      break;
    case k256th:
      result = "256";
      break;
    case k128th:
      result = "128";
      break;
    case k64th:
      result = "64";
      break;
    case k32nd:
      result = "32";
      break;
    case k16th:
      result = "16";
      break;
    case kEighth:
      result = "8";
      break;
    case kQuarter:
      result = "4";
      break;
    case kHalf:
      result = "2";
      break;
    case kWhole:
      result = "1";
      break;
    case kBreve:
      result = "Breve";
      break;
    case kLong:
      result = "Long";
      break;
    case kMaxima:
      result = "Maxima";
      break;
    case k_NoDuration:
      result = "k_NoDuration???";
      break;
  } // switch

  return result;
}

//_______________________________________________________________________________
S_msrOptions gMsrOptions;
S_msrOptions gMsrOptionsUserChoices;
S_msrOptions gMsrOptionsWithDetailedTrace;

S_msrOptions msrOptions::create ()
{
  msrOptions* o = new msrOptions();
  assert(o!=0);
  return o;
}

msrOptions::msrOptions ()
{
  initializeMsrOptions (false);
}

msrOptions::~msrOptions () {}

void msrOptions::initializeMsrOptions (
  bool boolOptionsInitialValue)
{
  // trace and display

  fTraceMsr          = boolOptionsInitialValue;
  
  fTraceMsrVisitors  = boolOptionsInitialValue;

  fDisplayMsr        = boolOptionsInitialValue;

  fDisplayMsrSummary = boolOptionsInitialValue;

  // languages
  
  if (! setMsrQuartertonesPitchesLanguage ("nederlands")) {
    stringstream s;

    s <<
      "INTERNAL INITIALIZATION ERROR: "
      "MSR pitches language 'nederlands' is unknown" <<
      endl <<
      "The " <<
      gQuatertonesPitchesLanguagesMap.size () <<
      " known MSR pitches languages are:" <<
      endl;

    idtr++;
  
    s <<
      existingQuartertonesPitchesLanguages ();

    idtr--;

    optionError (s.str());
  }
  
  // parts

  // voices

  fCreateStaffRelativeVoiceNumbers = boolOptionsInitialValue;
  
  fShowMasterVoices = boolOptionsInitialValue;
  fKeepMasterVoices = boolOptionsInitialValue;

  // notes

  fDelayRestsDynamics  = boolOptionsInitialValue;
  fDelayRestsWords     = boolOptionsInitialValue; // JMI
  fDelayRestsSlurs     = boolOptionsInitialValue; // JMI
  fDelayRestsLigatures = boolOptionsInitialValue; // JMI
  fDelayRestsWedges    = boolOptionsInitialValue; // JMI

  // lyrics

  fShowMsrStanzas    = boolOptionsInitialValue;
  fKeepMasterStanzas = boolOptionsInitialValue;

  // harmonies

  fShowHarmonyVoices       = boolOptionsInitialValue;  
  fKeepEmptyHarmoniesVoice = boolOptionsInitialValue;  
}

S_msrOptions msrOptions::createCloneWithDetailedTrace ()
{
  S_msrOptions
    clone =
      msrOptions::create ();

  // trace and display
  // --------------------------------------

  clone->fTraceMsr =
    true;

  clone->fTraceMsrVisitors =
    true;

  clone->fDisplayMsr =
    true;

  clone->fDisplayMsrSummary =
    fDisplayMsrSummary;

  // languages
    
  clone->fMsrQuatertonesPitchesLanguage =
    fMsrQuatertonesPitchesLanguage;

  // parts

  clone->fPartsRenaming =
    fPartsRenaming;

  // voices
  
  clone->fCreateStaffRelativeVoiceNumbers =
    fCreateStaffRelativeVoiceNumbers;

  clone->fShowMasterVoices =
    fShowMasterVoices;
  clone->fKeepMasterVoices =
    fKeepMasterVoices;

  // notes

  clone->fDelayRestsDynamics =
    fDelayRestsDynamics;
  clone->fDelayRestsWords =
    fDelayRestsWords;
  clone->fDelayRestsSlurs =
    fDelayRestsSlurs;
  clone->fDelayRestsLigatures =
    fDelayRestsLigatures;
  clone->fDelayRestsWedges =
    fDelayRestsWedges;

  // lyrics
    
  clone->fShowMsrStanzas =
    fShowMsrStanzas;

  clone->fKeepMasterStanzas =
    fKeepMasterStanzas;

  // harmonies

  clone->fShowHarmonyVoices =
    fShowHarmonyVoices;
  clone->fKeepEmptyHarmoniesVoice =
    fKeepEmptyHarmoniesVoice;

  return clone;
}  

bool msrOptions::setMsrQuartertonesPitchesLanguage (string language)
{
  // is language in the pitches languages map?
  map<string, msrQuatertonesPitchesLanguage>::const_iterator
    it =
      gQuatertonesPitchesLanguagesMap.find (language);
        
  if (it == gQuatertonesPitchesLanguagesMap.end ()) {
    // no, language is unknown in the map
    return false;
  }

  fMsrQuatertonesPitchesLanguage = (*it).second;
  
  return true;
}

void msrOptions::printMsrOptionsHelp ()
{
  idtr++;

  cerr <<
    idtr << "MSR:" <<
    endl <<
    idtr << "---" <<
    endl <<
    endl;

  idtr++;

  // trace and display
  // --------------------------------------
  cerr <<
    idtr << "Trace and display:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _TRACE_MSR_SHORT_NAME_ ", --" _TRACE_MSR_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Write a trace of the LPSR graphs visiting activity to standard error." <<
      endl <<
    endl <<
    
    idtr <<
      "--" _TRACE_MSR_VISITORS_SHORT_NAME_ ", --" _TRACE_MSR_VISITORS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Write a trace of the MSR graphs visiting activity to standard error." <<
      endl <<
    endl <<

    idtr <<
      "--" _DISPLAY_MSR_SHORT_NAME_ ", --" _DISPLAY_MSR_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Write the contents of the MSR data to standard error." <<
      endl <<
    endl <<

    idtr <<
      "--" _DISPLAY_MSR_SUMMARY_SHORT_NAME_ ", --" _DISPLAY_MSR_SUMMARY_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Only write a summary of the MSR to standard error." <<
      endl <<
    idtr << tab << tab << tab <<
      "This implies that no LilyPond code is generated." <<
      endl <<
    endl;
   
  idtr--;

  // languages
  // --------------------------------------
  cerr <<
    idtr << "Languages:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _MSR_PITCHES_LANGUAGE_SHORT_NAME_ ", --" _MSR_PITCHES_LANGUAGE_LONG_NAME_ " language" <<
      endl <<
    idtr << tab << tab << tab <<
      "Use 'language' to display note pitches in the MSR logs and text views." <<
      endl <<
    idtr << tab << tab << tab <<
      "The 12 LilyPond pitches languages are available:" <<
      endl <<
    idtr << tab << tab << tab <<
      "nederlands, catalan, deutsch, english, espanol, français, " <<
      endl <<
    idtr << tab << tab << tab <<
      "italiano, norsk, portugues, suomi, svenska and vlaams." <<
      endl <<
    idtr << tab << tab << tab <<
      "The default is to use 'nederlands'." <<
      endl <<
    endl;

  idtr--;

  // parts
  // --------------------------------------
  cerr <<
    idtr << "Parts:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _MSR_PITCHES_LANGUAGE_SHORT_NAME_ ", --" _MSR_PITCHES_LANGUAGE_LONG_NAME_ " 'originalName = newName'" <<
      endl <<
    idtr <<
      "--" _MSR_PITCHES_LANGUAGE_SHORT_NAME_ ", --" _MSR_PITCHES_LANGUAGE_LONG_NAME_ " \"originalName = newName\"" <<
      endl <<
    idtr << tab << tab << tab <<
      "Rename part 'original' to 'newName', for example after " <<
      endl <<
    idtr << tab << tab << tab <<
      "displaying a summary of the score in a first xml2lilypond run." <<
      endl <<
    idtr << tab << tab << tab <<
      "There can be several occurrences of this option." <<
      endl <<
    endl;

  idtr--;

  // voices
  // --------------------------------------
  cerr <<
    idtr << "Voices:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _STAFF_RELATIVE_VOICE_NUMBERS_SHORT_NAME_ ", --" _STAFF_RELATIVE_VOICE_NUMBERS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Generate voices names with numbers relative to their staff." <<
      endl <<
    idtr << tab << tab << tab <<
      "By default, the voice numbers found are used, " <<
      endl <<
    idtr << tab << tab << tab <<
      "which may be global to the score." <<
      endl <<
    endl <<
    
    idtr <<
      "--" _SHOW_MASTER_VOICES_SHORT_NAME_ ", --" _SHOW_MASTER_VOICES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Show the staves master voices used intertally event though the're empty" <<
      endl <<
    endl <<
    
    idtr <<
      "--" _KEEP_MASTER_VOICES_SHORT_NAME_ ", --" _KEEP_MASTER_VOICES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Keep the master voices used intertally. By default, there are removed after usage." <<
      endl <<
    endl;

  idtr--;

  // notes
  // --------------------------------------
  cerr <<
    idtr << "Notes:" <<
    endl <<
    endl;

  idtr++;

  cerr <<    
    idtr <<
      "--" _DELAY_RESTS_DYNAMICS_SHORT_NAME_ ", --" _DELAY_RESTS_DYNAMICS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "dynamics" <<
      endl <<
    idtr <<
      "--" _DELAY_RESTS_WORDS_SHORT_NAME_ ", --" _DELAY_RESTS_WORDS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "words" <<
      endl <<
    idtr <<
      "--" _DELAY_RESTS_SLURS_SHORT_NAME_ ", --" _DELAY_RESTS_SLURS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "slurs" <<
      endl <<
    idtr <<
      "--" _DELAY_RESTS_LIGATURES_SHORT_NAME_ ", --" _DELAY_RESTS_LIGATURES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "<bracket/> in MusicXML, '\\[... \\}' in LilyPond" <<
      endl <<
    idtr <<
      "--" _DELAY_RESTS_WEDGES_SHORT_NAME_ ", --" _DELAY_RESTS_WEDGES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "'<wedge/>' in MusicXML, '<!' in LilyPond" <<
      endl <<
    endl;

   idtr--;
   
  // lyrics
  // --------------------------------------
  cerr <<
    idtr << "Lyrics:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _SHOW_MSR_STANZAS_SHORT_NAME_ ", --" _SHOW_MSR_STANZAS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Show MSR stanzas even though they're empty'." <<
      endl <<
    endl <<

    idtr <<
      "--" _KEEP_MASTER_STANZAS_SHORT_NAME_ ", --" _KEEP_MASTER_STANZAS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Keep the master stanzas used intertally. By default, there are removed after usage." <<
      endl <<
    endl;

  idtr--;

  // harmonies
  // --------------------------------------
  cerr <<
    idtr << "Harmonies:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _SHOW_HARMONY_VOICES_SHORT_NAME_ ", --" _SHOW_HARMONY_VOICES_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Show the parts harmony voices in the MSR data even though is does not contain music." <<
      endl <<
    endl <<
    
    idtr <<
      "--" _KEEP_EMPTY_HARMONIES_VOICE_SHORT_NAME_ ", --" _KEEP_EMPTY_HARMONIES_VOICE_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Keep the harmonies voice in the MSR data even though is does not contain music." <<
      endl <<
    idtr << tab << tab << tab <<
      "It is thrown away in such a case by default." <<
      endl <<
    endl;

  idtr--;
  
  idtr--;

  idtr--; 
}

void msrOptions::printMsrOptionsValues (int fieldWidth)
{
  cerr << idtr <<
    "The MSR options are:" <<
    endl;

  idtr++;
  
  // trace and display
  // --------------------------------------
  
  cerr <<
    idtr << "Trace and display:" <<
    endl;

  idtr++;
  
  cerr << left <<
    idtr <<
      setw(fieldWidth) << "traceMsr" << " : " <<
      booleanAsString (fTraceMsr) <<
      endl <<
    
    idtr <<
      setw(fieldWidth) << "traceMsrVisitors" << " : " <<
      booleanAsString (fTraceMsrVisitors) <<
      endl <<

    idtr << setw(fieldWidth) << "displayMsr" << " : " <<
      booleanAsString (fDisplayMsr) <<
      endl <<

    idtr << setw(fieldWidth) << "displayMsrSummary" << " : " <<
      booleanAsString (fDisplayMsrSummary) <<
      endl;    

  idtr--;

  // languages
  // --------------------------------------
  
  cerr <<
    idtr << "Languages:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw(fieldWidth) << "msrPitchesLanguage" << " : \"" <<
      msrQuatertonesPitchesLanguageAsString (
        fMsrQuatertonesPitchesLanguage) <<
        "\"" <<
      endl;

  idtr--;

  // parts
  // --------------------------------------
  
  cerr <<
    idtr << "Parts:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw(fieldWidth) << "parts renaming" << " : ";

  if (fPartsRenaming.empty ()) {
    cerr <<
      "none";
  }
  
  else {
    for (
      map<string, string>::const_iterator i =
        fPartsRenaming.begin();
      i != fPartsRenaming.end();
      i++) {
        cerr <<
          "\"" << ((*i).first) << " = " << ((*i).second) << "\" ";
    } // for
  }

  cerr <<
    endl;

  idtr--;

  // voices
  // --------------------------------------
  
  cerr <<
    idtr << "Voices:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw(fieldWidth) << "createStaffRelativeVoiceNumbers" << " : " <<
      booleanAsString (fCreateStaffRelativeVoiceNumbers) <<
      endl <<
    
    idtr << setw(fieldWidth) << "showMasterVoices" << " : " <<
      booleanAsString (fShowMasterVoices) <<
      endl <<
    idtr << setw(fieldWidth) << "keepMasterVoices" << " : " <<
      booleanAsString (fKeepMasterVoices) <<
      endl;

  idtr--;

  // notes
  // --------------------------------------
  
  cerr <<
    idtr << "Notes:" <<
    endl;

  idtr++;

  cerr <<    
    idtr << setw(fieldWidth) << "delayRestsDynamics" << " : " <<
      booleanAsString (fDelayRestsDynamics) <<
      endl <<
      
    idtr << setw(fieldWidth) << "delayRestsWords" << " : " <<
      booleanAsString (fDelayRestsWords) <<
      endl <<
      
    idtr << setw(fieldWidth) << "delayRestsSlurs" << " : " <<
      booleanAsString (fDelayRestsSlurs) <<
      endl <<
      
    idtr << setw(fieldWidth) << "delayRestsLigatures" << " : " <<
      booleanAsString (fDelayRestsLigatures) <<
      endl <<
      
    idtr << setw(fieldWidth) << "delayRestsWedges" << " : " <<
      booleanAsString (fDelayRestsWedges) <<
      endl;

  idtr--;
   
  // lyrics
  // --------------------------------------
  
  cerr <<
    idtr << "Lyrics:" <<
    endl;

  idtr++;

  cerr <<
    idtr << setw(fieldWidth) << "showMsrStanzas" << " : " <<
      booleanAsString (fShowMsrStanzas) <<
      endl <<

    idtr << setw(fieldWidth) << "keepMasterStanzas" << " : " <<
      booleanAsString (fKeepMasterStanzas) <<
      endl;

  idtr--;

  // harmonies
  // --------------------------------------
  
  cerr <<
    idtr << "Harmonies:" <<
    endl;

  idtr++;    

  cerr <<
    idtr << setw(fieldWidth) << "showHarmonyVoices" << " : " <<
      booleanAsString (fShowHarmonyVoices) <<
      endl <<
    idtr << setw(fieldWidth) << "keepEmptyHarmoniesVoice" << " : " <<
      booleanAsString (fKeepEmptyHarmoniesVoice) <<
      endl;
  
  idtr--;
  
  idtr--;
}

//______________________________________________________________________________
msrElement::msrElement (
  int inputLineNumber)
{
  fInputLineNumber = inputLineNumber;  
}

msrElement::~msrElement() {}

void msrElement::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr <<
      "% ==> msrElement::acceptIn()" <<
      endl;
      
  if (visitor<S_msrElement>*
    p =
      dynamic_cast<visitor<S_msrElement>*> (v)) {
        S_msrElement elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr <<
            "% ==> Launching msrElement::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrElement::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr <<
      "% ==> msrElement::acceptOut()" <<
      endl;

  if (visitor<S_msrElement>*
    p =
      dynamic_cast<visitor<S_msrElement>*> (v)) {
        S_msrElement elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr <<
            "% ==> Launching msrElement::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

ostream& operator<< (ostream& os, const S_msrElement& elt)
{
  elt->print (os);
  return os;
}

void msrElement::print (ostream& os)
{
  os << "??? Element ???" << endl;
}


//______________________________________________________________________________
void msrBeatData::print (ostream& os)
{
  const int fieldWidth = 9;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "fBeatUnit" << " = " << fBeatUnit <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fDots" << " = " << fDots <<
      endl;
};

//______________________________________________________________________________
S_msrOctaveShift msrOctaveShift::create (
  int                inputLineNumber,
  msrOctaveShiftKind octaveShiftKind,
  int                octaveShiftSize)
{
  msrOctaveShift* o =
    new msrOctaveShift (
      inputLineNumber, octaveShiftKind, octaveShiftSize);
  assert(o!=0);
  return o;
}

msrOctaveShift::msrOctaveShift (
  int                inputLineNumber,
  msrOctaveShiftKind octaveShiftKind,
  int                octaveShiftSize)
    : msrElement (inputLineNumber)
{
  fOctaveShiftKind = octaveShiftKind;

  fOctaveShiftSize = octaveShiftSize;
}

msrOctaveShift::~msrOctaveShift() {}

void msrOctaveShift::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrOctaveShift::acceptIn()" <<
      endl;
      
  if (visitor<S_msrOctaveShift>*
    p =
      dynamic_cast<visitor<S_msrOctaveShift>*> (v)) {
        S_msrOctaveShift elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrOctaveShift::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrOctaveShift::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrOctaveShift::acceptOut()" <<
      endl;

  if (visitor<S_msrOctaveShift>*
    p =
      dynamic_cast<visitor<S_msrOctaveShift>*> (v)) {
        S_msrOctaveShift elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrOctaveShift::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrOctaveShift::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrOctaveShift& elt)
{
  elt->print (os);
  return os;
}

string msrOctaveShift::octaveShiftKindAsString () const
{
  string result;
  
  switch (fOctaveShiftKind) {
    case kOctaveShiftUp:
      result = "up";
      break;
    case kOctaveShiftDown:
      result = "down";
      break;
    case kOctaveShiftStop:
      result = "stop";
      break;
  } // switch

  return result;
}

void msrOctaveShift::print (ostream& os)
{
  idtr++;
  
  os <<
    "OctaveShift" <<
    ", kind: " << octaveShiftKindAsString () <<
    ", size: " << fOctaveShiftSize <<
    endl;

  idtr--;
}

//______________________________________________________________________________
S_msrStem msrStem::create (
  int           inputLineNumber,
  msrStemKind   stemKind)
{
  msrStem* o =
    new msrStem (
      inputLineNumber, stemKind);
  assert(o!=0);
  return o;
}

msrStem::msrStem (
  int           inputLineNumber,
  msrStemKind   stemKind)
    : msrElement (inputLineNumber)
{
  fStemKind   = stemKind; 
}

msrStem::~msrStem() {}

void msrStem::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrStem::acceptIn()" <<
      endl;
      
  if (visitor<S_msrStem>*
    p =
      dynamic_cast<visitor<S_msrStem>*> (v)) {
        S_msrStem elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrStem::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrStem::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrStem::acceptOut()" <<
      endl;

  if (visitor<S_msrStem>*
    p =
      dynamic_cast<visitor<S_msrStem>*> (v)) {
        S_msrStem elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrStem::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrStem::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrStem& elt)
{
  elt->print (os);
  return os;
}

string msrStem::stemKindAsString (
  msrStemKind stemKind)
{
  string result;
  
  switch (stemKind) {
    case k_NoStem:
      result = "none";
      break;
    case kStemUp:
      result = "up";
      break;
    case kStemDown:
      result = "down";
      break;
    case kStemNone:
      result = "none";
      break;
    case kStemDouble:
      result = "double";
      break;
  } // switch

  return result;
}

void msrStem::print (ostream& os)
{
  idtr++;
  
  os <<
    "Stem" " " <<
    stemKindAsString (fStemKind) <<
    ", line " << fInputLineNumber <<
    endl;

  idtr--;
}

//______________________________________________________________________________
S_msrBeam msrBeam::create (
  int           inputLineNumber,
  int           number,
  msrBeamKind   beamKind)
{
  msrBeam* o =
    new msrBeam (
      inputLineNumber, number, beamKind);
  assert(o!=0);
  return o;
}

msrBeam::msrBeam (
  int           inputLineNumber,
  int           number,
  msrBeamKind   beamKind)
    : msrElement (inputLineNumber)
{
  fBeamNumber = number;
  fBeamKind   = beamKind; 
}

msrBeam::~msrBeam() {}

void msrBeam::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrBeam::acceptIn()" <<
      endl;
      
  if (visitor<S_msrBeam>*
    p =
      dynamic_cast<visitor<S_msrBeam>*> (v)) {
        S_msrBeam elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrBeam::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrBeam::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrBeam::acceptOut()" <<
      endl;

  if (visitor<S_msrBeam>*
    p =
      dynamic_cast<visitor<S_msrBeam>*> (v)) {
        S_msrBeam elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrBeam::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrBeam::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrBeam& elt)
{
  elt->print (os);
  return os;
}

string msrBeam::beamKindAsString (
  msrBeamKind beamKind)
{
  string result;
  
  switch (beamKind) {
    case kBeginBeam:
      result = "begin";
      break;
    case kContinueBeam:
      result = "continue";
      break;
    case kEndBeam:
      result = "end";
      break;
    case kForwardHookBeam:
      result = "forward";
      break;
    case kBackwardHookBeam:
      result = "backward";
      break;
    case k_NoBeam:
      result = "### none ###";
      break;
  } // switch

  return result;
}

void msrBeam::print (ostream& os)
{  
  os <<
    "Beam" <<
    " number " << fBeamNumber <<
    ", line " << fInputLineNumber << " " <<
    beamKindAsString (fBeamKind) <<
    endl;
}

//______________________________________________________________________________
S_msrArticulation msrArticulation::create (
  int                 inputLineNumber,
  msrArticulationKind articulationKind)
{
  msrArticulation* o =
    new msrArticulation (
      inputLineNumber, articulationKind);
  assert (o!=0);
  return o;
}

msrArticulation::msrArticulation (
  int                 inputLineNumber,
  msrArticulationKind articulationKind)
    : msrElement (inputLineNumber)
{
  fArticulationKind = articulationKind;
}

msrArticulation::~msrArticulation() {}

string msrArticulation::articulationKindAsString () const
{
  string result;
  
  switch (fArticulationKind) {
    case msrArticulation::kAccent:
      result = "accent";
      break;
    case msrArticulation::kBreathMark:
      result = "breathMark";
      break;
    case msrArticulation::kCaesura:
      result = "caesura";
      break;
    case msrArticulation::kSpiccato:
      result = "spiccato";
      break;
    case msrArticulation::kStaccato:
      result = "staccato";
      break;
    case msrArticulation::kStaccatissimo:
      result = "staccatissimo";
      break;
    case msrArticulation::kStress:
      result = "stress";
      break;
    case msrArticulation::kUnstress:
      result = "unstress";
      break;
    case msrArticulation::kDetachedLegato:
      result = "detached legato";
      break;
    case msrArticulation::kStrongAccent:
      result = "strong accent";
      break;
    case msrArticulation::kTenuto:
      result = "tenuto";
      break;
    case msrArticulation::kFermata:
      result = "fermata";
      break;
    case msrArticulation::kArpeggiato:
      result = "arpeggiato";
      break;
    case msrArticulation::kDoit:
      result = "doit";
      break;
    case msrArticulation::kFalloff:
      result = "falloff";
      break;
    case msrArticulation::kPlop:
      result = "plop";
      break;
    case msrArticulation::kScoop:
      result = "scoop";
      break;
  } // switch

  return result;
}

void msrArticulation::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrArticulation::acceptIn()" <<
      endl;
      
  if (visitor<S_msrArticulation>*
    p =
      dynamic_cast<visitor<S_msrArticulation>*> (v)) {
        S_msrArticulation elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrArticulation::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrArticulation::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrArticulation::acceptOut()" <<
      endl;

  if (visitor<S_msrArticulation>*
    p =
      dynamic_cast<visitor<S_msrArticulation>*> (v)) {
        S_msrArticulation elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrArticulation::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrArticulation::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrArticulation& elt)
{
  elt->print (os);
  return os;
}

void msrArticulation::print (ostream& os)
{
  os <<
    "Articulation" " " <<
    articulationKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrTechnical msrTechnical::create (
  int                       inputLineNumber,
  msrTechnicalKind          technicalKind,
  msrTechnicalPlacementKind technicalPlacementKind)
{
  msrTechnical* o =
    new msrTechnical (
      inputLineNumber,
      technicalKind,
      technicalPlacementKind);
  assert (o!=0);
  return o;
}

msrTechnical::msrTechnical (
  int                       inputLineNumber,
  msrTechnicalKind          technicalKind,
  msrTechnicalPlacementKind technicalPlacementKind)
    : msrElement (inputLineNumber)
{
  fTechnicalKind = technicalKind;

  fTechnicalPlacementKind = technicalPlacementKind;
}

msrTechnical::~msrTechnical() {}

string msrTechnical::technicalKindAsString () const
{
  string result;
  
  switch (fTechnicalKind) {
    case msrTechnical::kArrow:
      result = "Arrow";
      break;
    case msrTechnical::kDoubleTongue:
      result = "DoubleTongue";
      break;
    case msrTechnical::kDownBow:
      result = "DownBow";
      break;
    case msrTechnical::kFingernails:
      result = "Fingernails";
      break;
    case msrTechnical::kHarmonic:
      result = "Harmonic";
      break;
    case msrTechnical::kHeel:
      result = "Heel";
      break;
    case msrTechnical::kHole:
      result = "Hole";
      break;
    case msrTechnical::kOpenString:
      result = "OpenString";
      break;
    case msrTechnical::kSnapPizzicato:
      result = "SnapPizzicato";
      break;
    case msrTechnical::kStopped:
      result = "Stopped";
      break;
    case msrTechnical::kTap:
      result = "Tap";
      break;
    case msrTechnical::kThumbPosition:
      result = "ThumbPosition";
      break;
    case msrTechnical::kToe:
      result = "Toe";
      break;
    case msrTechnical::kTripleTongue:
      result = "TripleTongue";
      break;
    case msrTechnical::kUpBow:
      result = "UpBow";
      break;
  } // switch

  return result;
}

string msrTechnical::technicalPlacementKindAsString () const
{
  string result;
  
  switch (fTechnicalPlacementKind) {
    case msrTechnical::k_NoTechnicalPlacement:
      result = "none";
      break;
    case msrTechnical::kTechnicalPlacementAbove:
      result = "above";
      break;
    case msrTechnical::kTechnicalPlacementBelow:
      result = "below";
      break;
  } // switch

  return result;
}

/*
string msrTechnical::technicalAccidentalMarkKindAsString () const
{
  string result;
  
  switch (fTechnicalAccidentalMarkKind) {
    case msrTechnical::kNatural:
      result = "natural";
      break;
    case msrTechnical::kSharp:
      result = "sharp";
      break;
    case msrTechnical::kFlat:
      result = "flat";
      break;
  } // switch

  return result;
}
*/

void msrTechnical::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrTechnical::acceptIn()" <<
      endl;
      
  if (visitor<S_msrTechnical>*
    p =
      dynamic_cast<visitor<S_msrTechnical>*> (v)) {
        S_msrTechnical elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrTechnical::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrTechnical::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrTechnical::acceptOut()" <<
      endl;

  if (visitor<S_msrTechnical>*
    p =
      dynamic_cast<visitor<S_msrTechnical>*> (v)) {
        S_msrTechnical elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrTechnical::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrTechnical::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTechnical& elt)
{
  elt->print (os);
  return os;
}

string msrTechnical::technicalAsString () const
{
  string result;
  
  switch (fTechnicalPlacementKind) {
    case msrTechnical::k_NoTechnicalPlacement:
      result = "none";
      break;
    case msrTechnical::kTechnicalPlacementAbove:
      result = "above";
      break;
    case msrTechnical::kTechnicalPlacementBelow:
      result = "below";
      break;
  } // switch

  return result;
}

void msrTechnical::print (ostream& os)
{
  os <<
    "Technical" " " <<
    technicalAsString () <<
    ", line " << fInputLineNumber <<
//    ", accidental mark" << " = " << technicalAccidentalMarkKindAsString () <<
    ", note uplink" << " = " <<
    fTechnicalNoteUplink->noteAsShortString () <<
    endl;
}

//______________________________________________________________________________
S_msrTechnicalWithInteger msrTechnicalWithInteger::create (
  int                         inputLineNumber,
  msrTechnicalWithIntegerKind technicalWithIntegerKind,
  int                         technicalWithIntegerValue,
  msrTechnicalWithIntegerPlacementKind
                              technicalWithIntegerPlacementKind)
{
  msrTechnicalWithInteger* o =
    new msrTechnicalWithInteger (
      inputLineNumber,
      technicalWithIntegerKind,
      technicalWithIntegerValue,
      technicalWithIntegerPlacementKind);
  assert (o!=0);
  return o;
}

msrTechnicalWithInteger::msrTechnicalWithInteger (
  int                         inputLineNumber,
  msrTechnicalWithIntegerKind technicalWithIntegerKind,
  int                         technicalWithIntegerValue,
  msrTechnicalWithIntegerPlacementKind
                              technicalWithIntegerPlacementKind)
    : msrElement (inputLineNumber)
{
  fTechnicalWithIntegerKind = technicalWithIntegerKind;

  fTechnicalWithIntegerValue = technicalWithIntegerValue;

  fTechnicalWithIntegerPlacementKind = technicalWithIntegerPlacementKind;
}

msrTechnicalWithInteger::~msrTechnicalWithInteger() {}

string msrTechnicalWithInteger::technicalWithIntegerKindAsString () const
{
  string result;
  
  switch (fTechnicalWithIntegerKind) {
    case msrTechnicalWithInteger::kBend:
      result = "Bend";
      break;
    case msrTechnicalWithInteger::kFingering:
      result = "Fingering";
      break;
    case msrTechnicalWithInteger::kFret:
      result = "Fret";
      break;
    case msrTechnicalWithInteger::kString:
      result = "String";
      break;
  } // switch

  return result;
}

string msrTechnicalWithInteger::technicalWithIntegerPlacementKindAsString () const
{
  string result;
  
  switch (fTechnicalWithIntegerPlacementKind) {
    case msrTechnicalWithInteger::k_NoTechnicalWithIntegerPlacement:
      result = "none";
      break;
    case msrTechnicalWithInteger::kTechnicalWithIntegerPlacementAbove:
      result = "above";
      break;
    case msrTechnicalWithInteger::kTechnicalWithIntegerPlacementBelow:
      result = "below";
      break;
  } // switch

  return result;
}

void msrTechnicalWithInteger::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrTechnicalWithInteger::acceptIn()" <<
      endl;
      
  if (visitor<S_msrTechnicalWithInteger>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithInteger>*> (v)) {
        S_msrTechnicalWithInteger elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrTechnicalWithInteger::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrTechnicalWithInteger::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrTechnicalWithInteger::acceptOut()" <<
      endl;

  if (visitor<S_msrTechnicalWithInteger>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithInteger>*> (v)) {
        S_msrTechnicalWithInteger elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrTechnicalWithInteger::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrTechnicalWithInteger::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTechnicalWithInteger& elt)
{
  elt->print (os);
  return os;
}

string msrTechnicalWithInteger::technicalWithIntegerAsString () const
{
  stringstream s;

  s <<
    technicalWithIntegerKindAsString () <<
    " " <<
    fTechnicalWithIntegerValue <<
    ", placement " <<
    technicalWithIntegerPlacementKindAsString ();

  return s.str();
}

void msrTechnicalWithInteger::print (ostream& os)
{
  os <<
    "TechnicalWithInteger" " " <<
    technicalWithIntegerAsString () <<
    ", line " << fInputLineNumber <<
    ", note uplink" << " = " <<
    fTechnicalWithIntegerNoteUplink->noteAsShortString () <<
    endl;
}

//______________________________________________________________________________
S_msrTechnicalWithString msrTechnicalWithString::create (
  int                         inputLineNumber,
  msrTechnicalWithStringKind technicalWithStringKind,
  string                      technicalWithStringValue,
  msrTechnicalWithStringPlacementKind
                              technicalWithStringPlacementKind)
{
  msrTechnicalWithString* o =
    new msrTechnicalWithString (
      inputLineNumber,
      technicalWithStringKind,
      technicalWithStringValue,
      technicalWithStringPlacementKind);
  assert (o!=0);
  return o;
}

msrTechnicalWithString::msrTechnicalWithString (
  int                         inputLineNumber,
  msrTechnicalWithStringKind technicalWithStringKind,
  string                      technicalWithStringValue,
  msrTechnicalWithStringPlacementKind
                              technicalWithStringPlacementKind)
    : msrElement (inputLineNumber)
{
  fTechnicalWithStringKind = technicalWithStringKind;

  fTechnicalWithStringValue = technicalWithStringValue;

  fTechnicalWithStringPlacementKind = technicalWithStringPlacementKind;
}

msrTechnicalWithString::~msrTechnicalWithString() {}

string msrTechnicalWithString::technicalWithStringKindAsString () const
{
  string result;
  
  switch (fTechnicalWithStringKind) {
    case msrTechnicalWithString::kHammerOn:
      result = "HammerOn";
      break;
    case msrTechnicalWithString::kHandbell:
      result = "Handbell";
      break;
    case msrTechnicalWithString::kOtherTechnical:
      result = "OtherTechnical";
      break;
    case msrTechnicalWithString::kPluck:
      result = "Pluck";
      break;
    case msrTechnicalWithString::kPullOff:
      result = "PullOff";
      break;
  } // switch

  result +=
    " " + fTechnicalWithStringValue;

  return result;
}

string msrTechnicalWithString::technicalWithStringPlacementKindAsString () const
{
  string result;
  
  switch (fTechnicalWithStringPlacementKind) {
    case msrTechnicalWithString::k_NoTechnicalWithStringPlacement:
      result = "none";
      break;
    case msrTechnicalWithString::kTechnicalWithStringPlacementAbove:
      result = "above";
      break;
    case msrTechnicalWithString::kTechnicalWithStringPlacementBelow:
      result = "below";
      break;
  } // switch

  return result;
}

void msrTechnicalWithString::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrTechnicalWithString::acceptIn()" <<
      endl;
      
  if (visitor<S_msrTechnicalWithString>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithString>*> (v)) {
        S_msrTechnicalWithString elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrTechnicalWithString::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrTechnicalWithString::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrTechnicalWithString::acceptOut()" <<
      endl;

  if (visitor<S_msrTechnicalWithString>*
    p =
      dynamic_cast<visitor<S_msrTechnicalWithString>*> (v)) {
        S_msrTechnicalWithString elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrTechnicalWithString::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrTechnicalWithString::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTechnicalWithString& elt)
{
  elt->print (os);
  return os;
}

string msrTechnicalWithString::technicalWithStringAsString () const
{
  stringstream s;

  s <<
    technicalWithStringKindAsString () <<
    " " <<
    fTechnicalWithStringValue <<
    ", placement " <<
    technicalWithStringPlacementKindAsString ();

  return s.str();
}

void msrTechnicalWithString::print (ostream& os)
{
  os <<
    "TechnicalWithString" " " <<
    technicalWithStringAsString () <<
    ", line " << fInputLineNumber <<
    ", note uplink" << " = " <<
    fTechnicalWithStringNoteUplink->noteAsShortString () <<
    endl;
}

//______________________________________________________________________________
S_msrOrnament msrOrnament::create (
  int             inputLineNumber,
  msrOrnamentKind ornamentKind)
{
  msrOrnament* o =
    new msrOrnament (
      inputLineNumber, ornamentKind);
  assert (o!=0);
  return o;
}

msrOrnament::msrOrnament (
  int             inputLineNumber,
  msrOrnamentKind ornamentKind)
    : msrElement (inputLineNumber)
{
  fOrnamentKind = ornamentKind;

  fOrnamentPlacementKind = k_NoOrnamentPlacement;

  fOrnamentAccidentalMarkKind = kNatural;
}

msrOrnament::~msrOrnament() {}

string msrOrnament::ornamentKindAsString () const
{
  string result;
  
  switch (fOrnamentKind) {
    case msrOrnament::kTrillMark:
      result = "trill";
      break;
    case msrOrnament::kWavyLine:
      result = "wayvy line";
      break;
    case msrOrnament::kTurn:
      result = "turn";
      break;
    case msrOrnament::kInvertedTurn:
      result = "inverted turn";
      break;
    case msrOrnament::kDelayedTurn:
      result = "delayed turn";
      break;
    case msrOrnament::kDelayedInvertedTurn:
      result = "delayed inverted turn";
      break;
    case msrOrnament::kVerticalTurn:
      result = "vertical turn";
      break;
    case msrOrnament::kMordent:
      result = "mordent";
      break;
    case msrOrnament::kInvertedMordent:
      result = "inverted mordent";
      break;
    case msrOrnament::kSchleifer:
      result = "schleifer";
      break;
    case msrOrnament::kShake:
      result = "shake";
      break;
  } // switch

  return result;
}

string msrOrnament::ornamentPlacementKindAsString () const
{
  string result;
  
  switch (fOrnamentPlacementKind) {
    case msrOrnament::k_NoOrnamentPlacement:
      result = "none";
      break;
    case msrOrnament::kOrnamentPlacementAbove:
      result = "above";
      break;
    case msrOrnament::kOrnamentPlacementBelow:
      result = "below";
      break;
  } // switch

  return result;
}

string msrOrnament::ornamentAccidentalMarkKindAsString () const
{
  string result;
  
  switch (fOrnamentAccidentalMarkKind) {
    case msrOrnament::kDoubleFlat:
      result = "doubleFlat";
      break;
    case msrOrnament::kSesquiFlat:
      result = "sesquiFlat";
      break;
    case msrOrnament::kFlat:
      result = "flat";
      break;
    case msrOrnament::kSemiFlat:
      result = "semiFlat";
      break;
    case msrOrnament::kNatural:
      result = "natural";
      break;
    case msrOrnament::kSemiSharp:
      result = "semiSharp";
      break;
    case msrOrnament::kSharp:
      result = "sharp";
      break;
    case msrOrnament::kSesquiSharp:
      result = "sesquiSharp";
      break;
    case msrOrnament::kDoubleSharp:
      result = "doubleSharp";
      break;
  } // switch

  return result;
}

void msrOrnament::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrOrnament::acceptIn()" <<
      endl;
      
  if (visitor<S_msrOrnament>*
    p =
      dynamic_cast<visitor<S_msrOrnament>*> (v)) {
        S_msrOrnament elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrOrnament::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrOrnament::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrOrnament::acceptOut()" <<
      endl;

  if (visitor<S_msrOrnament>*
    p =
      dynamic_cast<visitor<S_msrOrnament>*> (v)) {
        S_msrOrnament elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrOrnament::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrOrnament::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrOrnament& elt)
{
  elt->print (os);
  return os;
}

void msrOrnament::print (ostream& os)
{
  os <<
    "Ornament" " " <<
    ornamentKindAsString () <<
    ", line " << fInputLineNumber <<
    ", placement" << " = " << ornamentPlacementKindAsString () <<
    ", accidental mark" << " = " << ornamentAccidentalMarkKindAsString () <<
    ", note uplink" << " = " << fOrnamentNoteUplink->noteAsShortString () <<
    endl;
}

//______________________________________________________________________________
S_msrSingleTremolo msrSingleTremolo::create (
  int                     inputLineNumber,
  int                     singleTremoloMarksNumber,
  msrSingleTremoloPlacementKind singleTremoloPlacementKind)
{
  msrSingleTremolo* o =
    new msrSingleTremolo (
      inputLineNumber,
      singleTremoloMarksNumber, singleTremoloPlacementKind);
  assert (o!=0);
  return o;
}

msrSingleTremolo::msrSingleTremolo (
  int                     inputLineNumber,
  int                     singleTremoloMarksNumber,
  msrSingleTremoloPlacementKind singleTremoloPlacementKind)
    : msrElement (inputLineNumber)
{
  fSingleTremoloMarksNumber   = singleTremoloMarksNumber;
  fSingleTremoloPlacementKind = singleTremoloPlacementKind;
}

msrSingleTremolo::~msrSingleTremolo() {}

string msrSingleTremolo::singleTremoloPlacementKindAsString (
  msrSingleTremoloPlacementKind singleTremoloPlacementKind)
{
  string result;
  
  switch (singleTremoloPlacementKind) {
    case msrSingleTremolo::k_NoSingleTremoloPlacement:
      result = "none";
      break;
    case msrSingleTremolo::kSingleTremoloPlacementAbove:
      result = "above";
      break;
    case msrSingleTremolo::kSingleTremoloPlacementBelow:
      result = "below";
      break;
  } // switch

  return result;
}

string msrSingleTremolo::singleTremoloPlacementKindAsString () const
{
  return
    singleTremoloPlacementKindAsString (
      fSingleTremoloPlacementKind);
}

void msrSingleTremolo::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrSingleTremolo::acceptIn()" <<
      endl;
      
  if (visitor<S_msrSingleTremolo>*
    p =
      dynamic_cast<visitor<S_msrSingleTremolo>*> (v)) {
        S_msrSingleTremolo elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrSingleTremolo::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrSingleTremolo::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrSingleTremolo::acceptOut()" <<
      endl;

  if (visitor<S_msrSingleTremolo>*
    p =
      dynamic_cast<visitor<S_msrSingleTremolo>*> (v)) {
        S_msrSingleTremolo elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrSingleTremolo::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrSingleTremolo::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrSingleTremolo& elt)
{
  elt->print (os);
  return os;
}

string msrSingleTremolo::singleTremoloAsString () const
{
  stringstream s;
  
  s <<
    "SingleTremolo" <<
    ", line " << fInputLineNumber <<
    ", " << fSingleTremoloMarksNumber << " marks" <<
    ", placement" << " = " << singleTremoloPlacementKindAsString ();

  if (fSingleTremoloNoteUplink) // it may not yet be set
    s <<
      ", note uplink" << " = " <<
      fSingleTremoloNoteUplink->noteAsShortStringWithRawDivisions ();

  return s.str();
}

void msrSingleTremolo::print (ostream& os)
{
  os <<
    singleTremoloAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrDoubleTremolo msrDoubleTremolo::create (
  int                  inputLineNumber,
  msrDoubleTremoloKind doubleTremoloKind,
  int                  doubleTremoloMarksNumber,
  msrDoubleTremoloPlacementKind
                       doubleTremoloPlacementKind,
  S_msrVoice           voiceUplink)
{
  msrDoubleTremolo* o =
    new msrDoubleTremolo (
      inputLineNumber,
      doubleTremoloKind,
      doubleTremoloMarksNumber,
      doubleTremoloPlacementKind,
      voiceUplink);
  assert (o!=0);
  return o;
}

msrDoubleTremolo::msrDoubleTremolo (
  int                  inputLineNumber,
  msrDoubleTremoloKind doubleTremoloKind,
  int                  doubleTremoloMarksNumber,
  msrDoubleTremoloPlacementKind
                       doubleTremoloPlacementKind,
  S_msrVoice           voiceUplink)
    : msrElement (inputLineNumber)
{
  fDoubleTremoloKind          = doubleTremoloKind;
  fDoubleTremoloMarksNumber   = doubleTremoloMarksNumber;
  fDoubleTremoloPlacementKind = doubleTremoloPlacementKind;
  
  fDoubleTremoloVoiceUplink = voiceUplink;

  fDoubleTremoloSoundingDivisions = -1; // will be set later
}

S_msrDoubleTremolo msrDoubleTremolo::createDoubleTremoloShallowClone (
  S_msrVoice voiceClone)
{
  if (gGeneralOptions->fTraceTremolos)
    cerr << idtr <<
      "Creating a shallow clone of a double tremolo" <<
      endl;
  
  msrAssert(
    voiceClone != 0,
    "voiceClone is null");
    
  S_msrDoubleTremolo
    clone =
      msrDoubleTremolo::create (
        fInputLineNumber,
        fDoubleTremoloKind,
        fDoubleTremoloMarksNumber,
        fDoubleTremoloPlacementKind,
        voiceClone);

  clone->fDoubleTremoloSoundingDivisions =
    fDoubleTremoloSoundingDivisions;
        
  return clone;
}

msrDoubleTremolo::~msrDoubleTremolo() {}

void msrDoubleTremolo::setDoubleTremoloNoteFirstElement (S_msrNote note)
{
  if (gGeneralOptions->fTraceTremolos) {
    cerr << idtr <<
      "Setting note " << note->noteAsShortString () <<
      " as first element of double tremolo " <<
      doubleTremoloAsShortString () <<
      endl;
  }

  // register note as first element of this double tremolo
  fDoubleTremoloFirstElement = note;

  // mark it as being a double tremolo first element
  note->
    setNoteIsFirstNoteInADoubleTremolo ();

  // fetch note displayed divisions
  int noteDisplayedDivisions =
    note->getNoteDisplayedDivisions ();

  // set double tremolo displayed divisions to the note's displayed divisions
  if (fDoubleTremoloSoundingDivisions > 0) {
    if (noteDisplayedDivisions != fDoubleTremoloSoundingDivisions) { // JMI
      stringstream s;

      s <<
        "attempt to set double tremolo sounding divisions both to " <<
        fDoubleTremoloSoundingDivisions << " (existing)" <<
        " and " <<
        noteDisplayedDivisions <<
        " on note first element:" << " (note)" <<
        endl;

      idtr++;

      s <<
        note <<
        endl;
      
      idtr--;
        
      msrInternalError (
        note->getInputLineNumber (),
        s.str ());
    }
  }
  
  else {
    fDoubleTremoloSoundingDivisions = noteDisplayedDivisions;
  }
}

void msrDoubleTremolo::setDoubleTremoloChordFirstElement (S_msrChord chord)
{
  if (gGeneralOptions->fTraceTremolos || gGeneralOptions->fTraceChords) {
    cerr << idtr <<
      "Setting chord " << chord->chordAsString () <<
      " as first element of double tremolo " <<
      doubleTremoloAsShortString () <<
      endl;
  }

  // register chord as first element of this double tremolo
  fDoubleTremoloFirstElement = chord;

  // mark it as being a double tremolo first element
  chord->
    setChordIsFirstChordInADoubleTremolo ();

  // fetch chord displayed divisions
  int chordDisplayedDivisions =
    chord->getChordDisplayedDivisions ();
    
  // set double tremolo divisions to the chords's displayed divisions
  if (fDoubleTremoloSoundingDivisions > 0) {
    if (chordDisplayedDivisions != fDoubleTremoloSoundingDivisions) { // JMI
      stringstream s;

      s <<
        "attempt to set double tremolo sounding divisions both to " <<
        fDoubleTremoloSoundingDivisions << " (existing)" <<
        " and " <<
        chordDisplayedDivisions << " (chord)" <<
        " on chord first element:" <<
        endl;

      idtr++;

      s <<
        chord <<
        endl;
      
      idtr--;
                
      msrInternalError (
        chord->getInputLineNumber (),
        s.str ());
    }
  }
  
  else {
    fDoubleTremoloSoundingDivisions = chordDisplayedDivisions;
  }
}

void msrDoubleTremolo::setDoubleTremoloNoteSecondElement (S_msrNote note)
{
  if (gGeneralOptions->fTraceTremolos) {
    cerr << idtr <<
      "Setting note " << note->noteAsShortString () <<
      " as second element of double tremolo " <<
      doubleTremoloAsShortString () <<
      endl;
  }

  // register note as second element of this double tremolo
  fDoubleTremoloSecondElement = note;

  // mark it as being a double tremolo second element
  note->
    setNoteIsSecondNoteInADoubleTremolo ();

  // fetch note displayed divisions
  int noteDisplayedDivisions =
    note->getNoteDisplayedDivisions ();

  // set double tremolo divisions to the note's displayed divisions
  if (fDoubleTremoloSoundingDivisions > 0) {
    if (noteDisplayedDivisions != fDoubleTremoloSoundingDivisions) { // JMI
      stringstream s;

      s <<
        "attempt to set double tremolo divisions both to " <<
        fDoubleTremoloSoundingDivisions << " (existing)" <<
        " and " <<
        noteDisplayedDivisions <<
        " on note second element:" << " (note)" <<
        endl;

      idtr++;

      s <<
        note <<
        endl;
      
      idtr--;
        
      msrInternalError (
        note->getInputLineNumber (),
        s.str ());
    }
  }
  
  else {
    fDoubleTremoloSoundingDivisions = noteDisplayedDivisions;
  }
}

void msrDoubleTremolo::setDoubleTremoloChordSecondElement (S_msrChord chord)
{
  if (gGeneralOptions->fTraceTremolos || gGeneralOptions->fTraceChords) {
    cerr << idtr <<
      "Setting chord " << chord->chordAsString () <<
      " as second element of double tremolo " <<
      doubleTremoloAsShortString () <<
      endl;
  }

  // register chord as second element of this double tremolo
  fDoubleTremoloSecondElement = chord;

  // mark it as being a double tremolo second element
  chord->
    setChordIsSecondChordInADoubleTremolo ();

  // fetch chord displayed divisions
  int chordDisplayedDivisions =
    chord->getChordDisplayedDivisions ();
    
  // set double tremolo divisions to the chords's displayed divisions
  if (fDoubleTremoloSoundingDivisions > 0) {
    if (chordDisplayedDivisions != fDoubleTremoloSoundingDivisions) { // JMI
      stringstream s;

     s <<
        "attempt to set double tremolo divisions both to " <<
        fDoubleTremoloSoundingDivisions << " (existing)" <<
        " and " <<
        chordDisplayedDivisions <<
        " on chord second element:" << " (chord)" <<
        endl;

      idtr++;

      s <<
        chord <<
        endl;
      
      idtr--;
        
      msrInternalError (
        chord->getInputLineNumber (),
        s.str ());
    }
  }
  
  else {
    fDoubleTremoloSoundingDivisions = chordDisplayedDivisions;
  }
}

string msrDoubleTremolo::msrDoubleTremoloKindAsString (
  msrDoubleTremolo::msrDoubleTremoloKind doubleTremolotKind)
{
  string result;
  
  switch (doubleTremolotKind) {
    case msrDoubleTremolo::kNotesDoubleTremolo:
      result = "Notes";
      break;
    case msrDoubleTremolo::kChordsDoubleTremolo:
      result = "Chords";
      break;
  } // switch

  return result;
}

string msrDoubleTremolo::doubleTremoloPlacementKindAsString (
  msrDoubleTremoloPlacementKind doubleTremoloPlacementKind)
{
  string result;
  
  switch (doubleTremoloPlacementKind) {
    case msrDoubleTremolo::k_NoDoubleTremoloPlacement:
      result = "none";
      break;
    case msrDoubleTremolo::kDoubleTremoloPlacementAbove:
      result = "above";
      break;
    case msrDoubleTremolo::kDoubleTremoloPlacementBelow:
      result = "below";
      break;
  } // switch

  return result;
}

string msrDoubleTremolo::doubleTremoloPlacementKindAsString () const
{
  return
    doubleTremoloPlacementKindAsString (
      fDoubleTremoloPlacementKind);
}

void msrDoubleTremolo::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrDoubleTremolo::acceptIn()" <<
      endl;
      
  if (visitor<S_msrDoubleTremolo>*
    p =
      dynamic_cast<visitor<S_msrDoubleTremolo>*> (v)) {
        S_msrDoubleTremolo elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrDoubleTremolo::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrDoubleTremolo::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrDoubleTremolo::acceptOut()" <<
      endl;

  if (visitor<S_msrDoubleTremolo>*
    p =
      dynamic_cast<visitor<S_msrDoubleTremolo>*> (v)) {
        S_msrDoubleTremolo elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrDoubleTremolo::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrDoubleTremolo::browseData (basevisitor* v)
{
  if (fDoubleTremoloFirstElement) {
    // browse the first element
    msrBrowser<msrElement> browser (v);
    browser.browse (*fDoubleTremoloFirstElement);
  }

  if (fDoubleTremoloSecondElement) {
    // browse the second element
    msrBrowser<msrElement> browser (v);
    browser.browse (*fDoubleTremoloSecondElement);
  }
}

ostream& operator<< (ostream& os, const S_msrDoubleTremolo& elt)
{
  elt->print (os);
  return os;
}

string msrDoubleTremolo::doubleTremoloAsShortString () const
{
  stringstream s;
  
  s <<
    "DoubleTremolo"<<
 //   ", " << msrDoubleTremoloKindAsString (fDoubleTremoloKind) <<
    ", line " << fInputLineNumber <<
    ", " <<
    singularOrPlural (
      fDoubleTremoloMarksNumber, "mark", "marks") <<
    ", placement" << " = " << doubleTremoloPlacementKindAsString ();

  return s.str();
}

string msrDoubleTremolo::doubleTremoloAsString () const
{
  stringstream s;
  
  s <<
    "DoubleTremolo" " " <<
//    ", " << msrDoubleTremoloKindAsString (fDoubleTremoloKind) <<
    ", line " << fInputLineNumber <<
    fDoubleTremoloMarksNumber << " marks" <<
    ", placement" << " = " << doubleTremoloPlacementKindAsString () <<
    ", " << fDoubleTremoloSoundingDivisions << " sounddivs";

  if (fDoubleTremoloFirstElement) // it may not yet be set
    s <<
      ", first element " << " = ";
      
    switch (fDoubleTremoloKind) {
      case msrDoubleTremolo::kNotesDoubleTremolo:
        if (
          S_msrNote note = dynamic_cast<msrNote*>(&(*fDoubleTremoloFirstElement))
          ) {    
          note->noteAsShortString ();
        }
        else {
          msrInternalError (
            fInputLineNumber,
            "notes double tremolo first element should be a note");
        }
        break;
        
      case msrDoubleTremolo::kChordsDoubleTremolo:
        if (
          S_msrChord chord = dynamic_cast<msrChord*>(&(*fDoubleTremoloFirstElement))
          ) {
          chord->chordAsString ();
        }
        else {
          msrInternalError (
            fInputLineNumber,
            "chords double tremolo first element should be a chord");
        }
        break;
    } // switch
      
  if (fDoubleTremoloSecondElement) // it may not yet be set
    s <<
      ", second element " << " = ";
      
    switch (fDoubleTremoloKind) {
      case msrDoubleTremolo::kNotesDoubleTremolo:
        if (
          S_msrNote note = dynamic_cast<msrNote*>(&(*fDoubleTremoloSecondElement))
          ) {    
          note->noteAsShortString ();
        }
        else {
          msrInternalError (
            fInputLineNumber,
            "notes double tremolo second element should be a note");
        }
        break;
        
      case msrDoubleTremolo::kChordsDoubleTremolo:
        if (
          S_msrChord chord = dynamic_cast<msrChord*>(&(*fDoubleTremoloSecondElement))
          ) {
          chord->chordAsString ();
        }
        else {
          msrInternalError (
            fInputLineNumber,
            "chords double tremolo second element should be a chord");
        }
        break;
    } // switch

  return s.str();
}

void msrDoubleTremolo::print (ostream& os)
{
  os <<
    "DoubleTremolo" <<
    ", " << msrDoubleTremoloKindAsString (fDoubleTremoloKind) <<
    ", line " << fInputLineNumber <<
    ", " <<
    singularOrPlural (
      fDoubleTremoloMarksNumber, "mark", "marks") <<
    ", placement: " << doubleTremoloPlacementKindAsString () <<
    ", " << fDoubleTremoloSoundingDivisions << " sounddivs" <<
    endl;

  idtr++;
  
  os <<
    idtr <<
    "First element:";
  if (fDoubleTremoloFirstElement) { // it may not yet be set
    os <<
      endl;
      
    idtr++;
    os << idtr <<
      fDoubleTremoloFirstElement;
    idtr--;
  }
  else {
    os <<
      " none" <<
      endl;
  }
      
  os <<
    idtr <<
    "Second element:";
  if (fDoubleTremoloSecondElement) { // it may not yet be set
    os <<
      endl;

    idtr++;
    os << idtr <<
      fDoubleTremoloSecondElement;
    idtr--;
  }
  else {
    os <<
      " none" <<
      endl;
  }

  idtr--;
}

//______________________________________________________________________________
S_msrRehearsal msrRehearsal::create (
  int              inputLineNumber,
  msrRehearsalKind rehearsalKind,
  string           rehearsalText)
{
  msrRehearsal* o =
    new msrRehearsal (
      inputLineNumber, rehearsalKind, rehearsalText);
  assert (o!=0);
  return o;
}

msrRehearsal::msrRehearsal (
  int              inputLineNumber,
  msrRehearsalKind rehearsalKind,
  string           rehearsalText)
    : msrElement (inputLineNumber)
{
  fRehearsalKind = rehearsalKind;

  fRehearsalText = rehearsalText;
}

msrRehearsal::~msrRehearsal() {}

void msrRehearsal::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrRehearsal::acceptIn()" <<
      endl;
      
  if (visitor<S_msrRehearsal>*
    p =
      dynamic_cast<visitor<S_msrRehearsal>*> (v)) {
        S_msrRehearsal elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrRehearsal::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrRehearsal::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrRehearsal::acceptOut()" <<
      endl;

  if (visitor<S_msrRehearsal>*
    p =
      dynamic_cast<visitor<S_msrRehearsal>*> (v)) {
        S_msrRehearsal elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrRehearsal::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrRehearsal::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrRehearsal& elt)
{
  elt->print (os);
  return os;
}

string msrRehearsal::rehearsalKindAsString (
  msrRehearsalKind rehearsalKind)
{
  string result;
  
  switch (rehearsalKind) {
    case kNone:
      result = "none";
      break;
    case kRectangle:
      result = "rectangle";
      break;
    case kOval:
      result = "oval";
      break;
    case kCircle:
      result = "circle";
      break;
    case kBracket:
      result = "bracket";
      break;
    case kTriangle:
      result = "triangle";
      break;
    case kDiamond:
      result = "diamond";
      break;
  } // switch

  return result;
}

void msrRehearsal::print (ostream& os)
{
  os <<
    "Rehearsal" << " " << fRehearsalText <<
    " kind: " <<
    rehearsalKindAsString (fRehearsalKind) <<
    endl;
}

//______________________________________________________________________________
S_msrDynamics msrDynamics::create (
  int             inputLineNumber,
  msrDynamicsKind dynamicsKind)
{
  msrDynamics* o =
    new msrDynamics (
      inputLineNumber, dynamicsKind);
    assert(o!=0);
  return o;
}

msrDynamics::msrDynamics (
  int             inputLineNumber,
  msrDynamicsKind dynamicsKind)
    : msrElement (inputLineNumber)
{
  fDynamicsKind = dynamicsKind; 
}
msrDynamics::~msrDynamics() {}

string msrDynamics::dynamicsKindAsString ()
{
  stringstream s;
  
  switch (fDynamicsKind) {
    case kF:
      s << "f";
      break;
    case kFF:
      s << "ff"; break;
    case kFFF:
      s << "fff"; break;
    case kFFFF:
      s << "ffff"; break;
    case kFFFFF:
      s << "fffff"; break;
    case kFFFFFF:
      s << "ffffff"; break;

    case kP:
      s << "p"; break;
    case kPP:
      s << "pp"; break;
    case kPPP:
      s << "ppp"; break;
    case kPPPP:
      s << "pppp"; break;
    case kPPPPP:
      s << "ppppp"; break;
    case kPPPPPP:
      s << "pppppp"; break;

    case kMF:
      s << "mf"; break;
    case kMP:
      s << "mp"; break;
    case kFP:
      s << "fp"; break;
    case kFZ:
      s << "fz"; break;
    case kRF:
      s << "rf"; break;
    case kSF:
      s << "sf"; break;

    case kRFZ:
      s << "rfz"; break;
    case kSFZ:
      s << "sfz"; break;
    case kSFP:
      s << "sfp"; break;
    case kSFPP:
      s << "sfpp"; break;
    case kSFFZ:
      s << "sffz"; break;

    default:
      {
      stringstream s;
      
      s << "Dynamics " << fDynamicsKind << " is unknown";
      
      msrMusicXMLError (
        fInputLineNumber,
        s.str());
      }
  } // switch
  
  return s.str();
}

void msrDynamics::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrDynamics::acceptIn()" <<
      endl;
      
  if (visitor<S_msrDynamics>*
    p =
      dynamic_cast<visitor<S_msrDynamics>*> (v)) {
        S_msrDynamics elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrDynamics::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrDynamics::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrDynamics::acceptOut()" <<
      endl;

  if (visitor<S_msrDynamics>*
    p =
      dynamic_cast<visitor<S_msrDynamics>*> (v)) {
        S_msrDynamics elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrDynamics::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}


void msrDynamics::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrDynamics& elt)
{
  elt->print (os);
  return os;
}

void msrDynamics::print (ostream& os)
{
  os <<
    "Dynamics" " " << dynamicsKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrOtherDynamics msrOtherDynamics::create (
  int    inputLineNumber,
  string otherDynamicsString)
{
  msrOtherDynamics* o =
    new msrOtherDynamics (
      inputLineNumber, otherDynamicsString);
    assert(o!=0);
  return o;
}

msrOtherDynamics::msrOtherDynamics (
  int    inputLineNumber,
  string otherDynamicsString)
    : msrElement (inputLineNumber)
{
  fOtherDynamicsString = otherDynamicsString; 
}
msrOtherDynamics::~msrOtherDynamics() {}

void msrOtherDynamics::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrOtherDynamics::acceptIn()" <<
      endl;
      
  if (visitor<S_msrOtherDynamics>*
    p =
      dynamic_cast<visitor<S_msrOtherDynamics>*> (v)) {
        S_msrOtherDynamics elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrOtherDynamics::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrOtherDynamics::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrOtherDynamics::acceptOut()" <<
      endl;

  if (visitor<S_msrOtherDynamics>*
    p =
      dynamic_cast<visitor<S_msrOtherDynamics>*> (v)) {
        S_msrOtherDynamics elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrOtherDynamics::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}


void msrOtherDynamics::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrOtherDynamics& elt)
{
  elt->print (os);
  return os;
}

string msrOtherDynamics::otherDynamicsAsString ()
{
  stringstream s;
  
  s <<
    "OtherDynamics '" << fOtherDynamicsString <<
    "', line " << fInputLineNumber;

  return s.str();
}

void msrOtherDynamics::print (ostream& os)
{
  os <<
    otherDynamicsAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrWedge msrWedge::create (
  int           inputLineNumber,
  msrWedgeKind  wedgeKind)
{
  msrWedge* o =
    new msrWedge (
      inputLineNumber, wedgeKind);
  assert(o!=0);
  return o;
}

msrWedge::msrWedge (
  int           inputLineNumber,
  msrWedgeKind  wedgeKind)
    : msrElement (inputLineNumber)
{
  fWedgeKind = wedgeKind; 
}
msrWedge::~msrWedge() {}

string msrWedge::wedgeKindAsString ()
{
  stringstream s;
  
  switch (fWedgeKind) {
    case msrWedge::kCrescendoWedge:
      s << "crescendo";
      break;
    case msrWedge::kDecrescendoWedge:
      s << "decrescendo";
      break;
    case msrWedge::kStopWedge:
      s << "stop";
      break;
  } // switch
    
  return s.str();
}

void msrWedge::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrWedge::acceptIn()" <<
      endl;
      
  if (visitor<S_msrWedge>*
    p =
      dynamic_cast<visitor<S_msrWedge>*> (v)) {
        S_msrWedge elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrWedge::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrWedge::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrWedge::acceptOut()" <<
      endl;

  if (visitor<S_msrWedge>*
    p =
      dynamic_cast<visitor<S_msrWedge>*> (v)) {
        S_msrWedge elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrWedge::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrWedge::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrWedge& elt)
{
  elt->print (os);
  return os;
}

void msrWedge::print (ostream& os)
{
  os <<
    "Wedge" << " " << wedgeKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrTie msrTie::create (
  int           inputLineNumber,
  msrTieKind    tieKind)
{
  msrTie* o =
    new msrTie (
      inputLineNumber, tieKind);
  assert(o!=0);
  return o;
}

msrTie::msrTie (
  int           inputLineNumber,
  msrTieKind    tieKind)
    : msrElement (inputLineNumber)
{
  fTieKind = tieKind; 
}
msrTie::~msrTie() {}

string msrTie::tieKindAsString (msrTieKind tieKind)
{
  stringstream s;
  
  switch (tieKind) {
    case kStartTie:
      s << "start";
      break;
    case kContinueTie:
      s << "continue";
      break;
    case kStopTie:
      s << "stop";
      break;
    default:
      s << "Tie" << tieKind << "???";
  } // switch
    
  return s.str();
}

void msrTie::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrTie::acceptIn()" <<
      endl;
      
  if (visitor<S_msrTie>*
    p =
      dynamic_cast<visitor<S_msrTie>*> (v)) {
        S_msrTie elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrTie::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrTie::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrTie::acceptOut()" <<
      endl;

  if (visitor<S_msrTie>*
    p =
      dynamic_cast<visitor<S_msrTie>*> (v)) {
        S_msrTie elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrTie::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrTie::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTie& elt)
{
  elt->print (os);
  return os;
}

void msrTie::print (ostream& os)
{
  os <<
    "Tie" << " " << tieKindAsString () <<
    ", line " << fInputLineNumber;
}

//______________________________________________________________________________
S_msrSlur msrSlur::create (
  int           inputLineNumber,
  int           slurNumber,
  msrSlurKind   slurKind)
{
  msrSlur* o =
    new msrSlur (
      inputLineNumber, slurNumber, slurKind);
  assert(o!=0);
  return o;
}

msrSlur::msrSlur (
  int           inputLineNumber,
  int           slurNumber,
  msrSlurKind   slurKind)
    : msrElement (inputLineNumber)
{
  fSlurNumber = slurNumber;
  fSlurKind   = slurKind; 
}
msrSlur::~msrSlur() {}

string msrSlur::slurKindAsString (
  msrSlurKind slurKind)
{
  stringstream s;
  
  switch (slurKind) {
    case msrSlur::kStartSlur:
      s << "start";
      break;
    case msrSlur::kContinueSlur:
      s << "continue";
      break;
    case msrSlur::kStopSlur:
      s << "stop";
      break;
    default:
      s << "Slur" << slurKind << "???";
  } // switch
    
  return s.str();
  
}
      
string msrSlur::slurKindAsString ()
{
  return slurKindAsString (fSlurKind);
}

void msrSlur::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrSlur::acceptIn()" <<
      endl;
      
  if (visitor<S_msrSlur>*
    p =
      dynamic_cast<visitor<S_msrSlur>*> (v)) {
        S_msrSlur elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrSlur::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrSlur::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrSlur::acceptOut()" <<
      endl;

  if (visitor<S_msrSlur>*
    p =
      dynamic_cast<visitor<S_msrSlur>*> (v)) {
        S_msrSlur elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrSlur::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}


void msrSlur::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrSlur& elt)
{
  elt->print (os);
  return os;
}

void msrSlur::print (ostream& os)
{
  os <<
    "Slur" " " << slurKindAsString () <<
    ", line " << fInputLineNumber;
}

//______________________________________________________________________________
S_msrLigature msrLigature::create (
  int             inputLineNumber,
  int             ligatureNumber,
  msrLigatureKind ligatureKind)
{
  msrLigature* o =
    new msrLigature (
      inputLineNumber, ligatureNumber, ligatureKind);
  assert(o!=0);
  return o;
}

msrLigature::msrLigature (
  int             inputLineNumber,
  int             ligatureNumber,
  msrLigatureKind ligatureKind)
    : msrElement (inputLineNumber)
{
  fLigatureNumber = ligatureNumber;
  fLigatureKind   = ligatureKind; 
}
msrLigature::~msrLigature() {}

string msrLigature::ligatureKindAsString (
  msrLigatureKind ligatureKind)
{
  stringstream s;
  
  switch (ligatureKind) {
    case msrLigature::kStartLigature:
      s << "start";
      break;
    case msrLigature::kContinueLigature:
      s << "continue";
      break;
    case msrLigature::kStopLigature:
      s << "stop";
      break;
    default:
      s << "Ligature" << ligatureKind << "???";
  } // switch
    
  return s.str();
  
}
      
string msrLigature::ligatureKindAsString ()
{
  return ligatureKindAsString (fLigatureKind);
}

void msrLigature::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrLigature::acceptIn()" <<
      endl;
      
  if (visitor<S_msrLigature>*
    p =
      dynamic_cast<visitor<S_msrLigature>*> (v)) {
        S_msrLigature elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrLigature::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrLigature::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrLigature::acceptOut()" <<
      endl;

  if (visitor<S_msrLigature>*
    p =
      dynamic_cast<visitor<S_msrLigature>*> (v)) {
        S_msrLigature elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrLigature::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}


void msrLigature::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrLigature& elt)
{
  elt->print (os);
  return os;
}

void msrLigature::print (ostream& os)
{
  os <<
    "Ligature" " " << ligatureKindAsString () <<
    ", line " << fInputLineNumber;
}

//______________________________________________________________________________
S_msrGraceNotes msrGraceNotes::create (
  int        inputLineNumber,
  S_msrPart  graceNotesDirectPartUplink,
  bool       gracenoteIsSlashed,
  S_msrVoice graceNotesVoiceUplink)
{
  msrGraceNotes* o =
    new msrGraceNotes (
      inputLineNumber,
      graceNotesDirectPartUplink,
      gracenoteIsSlashed,
      graceNotesVoiceUplink);
  assert(o!=0);

  return o;
}

msrGraceNotes::msrGraceNotes (
  int        inputLineNumber,
  S_msrPart  graceNotesDirectPartUplink,
  bool       gracenoteIsSlashed,
  S_msrVoice graceNotesVoiceUplink)
    : msrElement (inputLineNumber)
{
  // set gracenote's direct part uplink
  msrAssert(
    graceNotesDirectPartUplink != 0,
    "graceNotesDirectPartUplink is null");
    
  fGraceNotesDirectPartUplink =
    graceNotesDirectPartUplink;
    
  fGraceNotesIsSlashed = gracenoteIsSlashed;

  fGraceNotesVoiceUplink =
    graceNotesVoiceUplink;
}

msrGraceNotes::~msrGraceNotes() {}

S_msrGraceNotes msrGraceNotes::createGraceNotesShallowClone (
  S_msrVoice voiceClone)
{
  if (gGeneralOptions->fTraceGraceNotes) {
    cerr << idtr <<
      "Creating a shallow clone of grace notes" <<
      graceNotesAsShortString () <<
      endl;
  }
  
  msrAssert(
    voiceClone != 0,
    "voiceClone is null");
    
  S_msrGraceNotes
    clone =
      msrGraceNotes::create (
        fInputLineNumber,
        voiceClone->getVoiceDirectPartUplink (),
        fGraceNotesIsSlashed,
        voiceClone);

  clone->fGraceNotesIsSlashed =
    fGraceNotesIsSlashed;
    
  return clone;
}

S_msrGraceNotes msrGraceNotes::createSkipGraceNotesClone (
  S_msrVoice voiceClone)
{
  if (gGeneralOptions->fTraceGraceNotes) {
    cerr << idtr <<
      "Creating a skip clone of grace notes" <<
      graceNotesAsShortString () <<
      endl;
  }
  
  S_msrGraceNotes
    clone =
      msrGraceNotes::create (
        fInputLineNumber,
        voiceClone->getVoiceDirectPartUplink (),
        fGraceNotesIsSlashed,
        voiceClone);

  clone->fGraceNotesIsSlashed =
    fGraceNotesIsSlashed;

  // populating the clone with skips
  for (
    list<S_msrNote>::const_iterator i=fGraceNotesNotesList.begin();
    i!=fGraceNotesNotesList.end();
    i++) {
    S_msrNote note = (*i);
    
    // create skip with same duration as note
    S_msrNote
      skip =
        msrNote::createSkipNote (
          note->getInputLineNumber (),
          voiceClone->getVoiceDirectPartUplink (),
          note->getNoteSoundingDivisions (),
          note->getNoteDotsNumber (),
          voiceClone->getStaffRelativeVoiceNumber (), // JMI
          voiceClone->getExternalVoiceNumber ());

     clone->
      appendNoteToGraceNotes (skip);
  } // for
    
  return clone;
}

void msrGraceNotes::appendNoteToGraceNotes (S_msrNote note)
{
  fGraceNotesNotesList.push_back (note);
}

void msrGraceNotes::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrGraceNotes::acceptIn()" <<
      endl;
      
  if (visitor<S_msrGraceNotes>*
    p =
      dynamic_cast<visitor<S_msrGraceNotes>*> (v)) {
        S_msrGraceNotes elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrGraceNotes::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrGraceNotes::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrGraceNotes::acceptOut()" <<
      endl;

  if (visitor<S_msrGraceNotes>*
    p =
      dynamic_cast<visitor<S_msrGraceNotes>*> (v)) {
        S_msrGraceNotes elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrGraceNotes::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrGraceNotes::browseData (basevisitor* v)
{
  list<S_msrNote>::const_iterator i;

  for (
    i=fGraceNotesNotesList.begin();
    i!=fGraceNotesNotesList.end();
    i++) {
    // browse the note
    msrBrowser<msrNote> browser (v);
    browser.browse (*(*i));
  } // for
}

ostream& operator<< (ostream& os, const S_msrGraceNotes& elt)
{
  elt->print (os);
  return os;
}

string msrGraceNotes::graceNotesAsShortString () const
{
  stringstream s;

  s <<
    "GraceNotes" " ";

  list<S_msrNote>::const_iterator
    iBegin = fGraceNotesNotesList.begin(),
    iEnd   = fGraceNotesNotesList.end(),
    i      = iBegin;
  for ( ; ; ) {
    s << (*i)->noteAsShortString ();
    if (++i == iEnd) break;
    s << " ";
  } // for

  return s.str();
}

void msrGraceNotes::print (ostream& os)
{
  os <<
    "GraceNotes" <<
    ", line " << fInputLineNumber <<
    ", " <<
    singularOrPlural (
      fGraceNotesNotesList.size (), "note", "notes") <<
    ", slashed: " <<
    booleanAsString (fGraceNotesIsSlashed) <<
    endl;
  
  idtr++;
            
  list<S_msrNote>::const_iterator
    iBegin = fGraceNotesNotesList.begin(),
    iEnd   = fGraceNotesNotesList.end(),
    i      = iBegin;
    
  for ( ; ; ) {
    os << idtr << (*i);
    if (++i == iEnd) break;
    os << endl;
  } // for

  idtr--;
}

//______________________________________________________________________________
S_msrAfterGraceNotes msrAfterGraceNotes::create (
  int        inputLineNumber,
  S_msrPart  afterGraceNotesDirectPartUplink,
  S_msrNote  afterGraceNotesNote,
  bool       aftergracenoteIsSlashed,
  S_msrVoice afterGraceNotesVoiceUplink)
{
  msrAfterGraceNotes* o =
    new msrAfterGraceNotes (
      inputLineNumber,
      afterGraceNotesDirectPartUplink,
      afterGraceNotesNote,
      aftergracenoteIsSlashed,
      afterGraceNotesVoiceUplink);
  assert(o!=0);

  return o;
}

msrAfterGraceNotes::msrAfterGraceNotes (
  int        inputLineNumber,
  S_msrPart  afterGraceNotesDirectPartUplink,
  S_msrNote  afterGraceNotesNote,
  bool       aftergracenoteIsSlashed,
  S_msrVoice afterGraceNotesVoiceUplink)
    : msrElement (inputLineNumber)
{
  // set gracenote's direct part uplink
  msrAssert(
    afterGraceNotesDirectPartUplink != 0,
    "afterGraceNotesDirectPartUplink is null");

  fAfterGraceNotesDirectPartUplink =
    afterGraceNotesDirectPartUplink;

  fAfterGraceNotesNote =
    afterGraceNotesNote;
    
  fAfterGraceNotesIsSlashed =
    aftergracenoteIsSlashed;
  
  fAfterGraceNotesVoiceUplink =
    afterGraceNotesVoiceUplink;
}

msrAfterGraceNotes::~msrAfterGraceNotes() {}

S_msrAfterGraceNotes msrAfterGraceNotes::createAfterGraceNotesShallowClone (
  S_msrNote  noteClone,
  S_msrVoice voiceClone)
{
  if (gGeneralOptions->fTraceGraceNotes) {
    cerr << idtr <<
      "Creating a shallow clone of after grace notes" <<
      endl;
  }

  msrAssert(
    noteClone != 0,
    "noteClone is null");
    
  msrAssert(
    voiceClone != 0,
    "voiceClone is null");
    
  /* JMI ???
  // set after grace note's direct part uplink
  o->fAfterGraceNotesDirectPartUplink =
    afterGraceNotesDirectPartUplink;
    */
    
  S_msrAfterGraceNotes
    clone =
      msrAfterGraceNotes::create (
        fInputLineNumber,
        voiceClone->getVoiceDirectPartUplink (),
        noteClone,
        fAfterGraceNotesIsSlashed,
        voiceClone);
  
  clone->fAfterGraceNotesIsSlashed =
    fAfterGraceNotesIsSlashed;
  
  return clone;
}

void msrAfterGraceNotes::appendNoteToAfterGraceNotes (S_msrNote note)
{
  fAfterGraceNotesNotesList.push_back (note);
}

void msrAfterGraceNotes::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrAfterGraceNotes::acceptIn()" <<
      endl;
      
  if (visitor<S_msrAfterGraceNotes>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotes>*> (v)) {
        S_msrAfterGraceNotes elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrAfterGraceNotes::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrAfterGraceNotes::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrAfterGraceNotes::acceptOut()" <<
      endl;

  if (visitor<S_msrAfterGraceNotes>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotes>*> (v)) {
        S_msrAfterGraceNotes elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrAfterGraceNotes::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrAfterGraceNotes::browseData (basevisitor* v)
{
  list<S_msrNote>::const_iterator i;

  // browse the afterGraceNotes note
  msrBrowser<msrNote> browser (v);
  browser.browse (*fAfterGraceNotesNote);

  for (
    i=fAfterGraceNotesNotesList.begin();
    i!=fAfterGraceNotesNotesList.end();
    i++) {
    // browse the note
    msrBrowser<msrNote> browser (v);
    browser.browse (*(*i));
  } // for
}

ostream& operator<< (ostream& os, const S_msrAfterGraceNotes& elt)
{
  elt->print (os);
  return os;
}

string msrAfterGraceNotes::afterGraceNotesAsShortString () const
{
  stringstream s;

  s <<
    "AfterGraceNotes" " ";

  list<S_msrNote>::const_iterator
    iBegin = fAfterGraceNotesNotesList.begin(),
    iEnd   = fAfterGraceNotesNotesList.end(),
    i      = iBegin;
    
  for ( ; ; ) {
    s << (*i)->noteAsShortString ();
    if (++i == iEnd) break;
    s << " ";
  } // for

  return s.str();
}

void msrAfterGraceNotes::print (ostream& os)
{
  os <<
    "AfterGraceNotes" <<
    ", line " << fInputLineNumber <<
    ", " <<
    singularOrPlural (
      fAfterGraceNotesNotesList.size (), "note", "notes") <<
    ", slashed: " <<
    booleanAsString (fAfterGraceNotesIsSlashed) <<
    endl;
  
  idtr++;

  // print the afterGraceNotes note
  os <<
    idtr <<
      "Note:" <<
      endl;
  idtr++;
  os <<
    idtr <<
      fAfterGraceNotesNote;
  idtr--;

  // print the afterGraceNotes contents
  os <<
    idtr <<
      "Contents:" <<
      endl;

  idtr++;

  list<S_msrNote>::const_iterator
    iBegin = fAfterGraceNotesNotesList.begin(),
    iEnd   = fAfterGraceNotesNotesList.end(),
    i      = iBegin;
    
  for ( ; ; ) {
    os << idtr << (*i);
    if (++i == iEnd) break;
 // JMI   os << endl;
  } // for

  idtr--;
      
  idtr--;
}

//______________________________________________________________________________
S_msrNote msrNote::create (
  int                  inputLineNumber,
  S_msrPart            noteDirectPartUplink,

  msrNoteKind          noteKind,

  msrQuartertonesPitch noteQuatertonesPitch,
  int                  noteSoundingDivisions,
  int                  noteDisplayedDivisions,
  int                  noteDotsNumber,
  msrDuration          noteGraphicDuration,
  
  int                  noteOctave,
  
  bool                 noteIsARest,
  bool                 noteIsUnpitched,

  bool                 noteIsAGraceNote)
{  
  msrNote * o =
    new msrNote (
      inputLineNumber,
      noteDirectPartUplink,
      
      noteKind,
      
      noteQuatertonesPitch,
      noteSoundingDivisions,
      noteDisplayedDivisions,
      noteDotsNumber,
      noteGraphicDuration,
      
      noteOctave,
      
      noteIsARest,
      noteIsUnpitched,
      
      noteIsAGraceNote);
  assert(o!=0);

  return o;
}

S_msrNote msrNote::createSkipNote (
  int       inputLineNumber,
  S_msrPart noteDirectPartUplink,
  int       divisions,
  int       dotsNumber,
  int       staffNumber,
  int       externalVoiceNumber)
{    
  msrNote * o =
    new msrNote (
      inputLineNumber,
      noteDirectPartUplink,
      
      kSkipNote, // noteKind
      
      k_NoQuaterTonesPitch, // noteQuatertonesPitch
      divisions, // noteSoundingDivisions
      divisions, // noteDisplayedDivisions
      dotsNumber,
      k_NoDuration, // noteGraphicDuration
      
      -1, // noteOctave,
      
      false, // noteIsARest
      false, // noteIsUnpitched
      
      false); // noteIsAGraceNote
  assert(o!=0);

  return o;
}    

msrNote::msrNote (
  int                  inputLineNumber,
  S_msrPart            noteDirectPartUplink,

  msrNoteKind          noteKind,

  msrQuartertonesPitch noteQuatertonesPitch,
  int                  noteSoundingDivisions,
  int                  noteDisplayedDivisions,
  int                  noteDotsNumber,
  msrDuration          noteGraphicDuration,
  
  int                  noteOctave,
  
  bool                 noteIsARest,
  bool                 noteIsUnpitched,

  bool                 noteIsAGraceNote)
  : msrElement (inputLineNumber)
{
  // set note's direct part uplink
  msrAssert(
    noteDirectPartUplink != 0,
    "noteDirectPartUplink is null");
    
  fNoteDirectPartUplink =
    noteDirectPartUplink;
  
  // basic note description
  // ------------------------------------------------------

  fNoteKind = noteKind;

  fNoteQuatertonesPitch   = noteQuatertonesPitch;
  fNoteSoundingDivisions  = noteSoundingDivisions;
  fNoteDisplayedDivisions = noteDisplayedDivisions;
  fNoteDotsNumber         = noteDotsNumber;
  fNoteGraphicDuration    = noteGraphicDuration;

  fNoteOctave = noteOctave;

  fNoteIsARest     = noteIsARest;
  fNoteIsUnpitched = noteIsUnpitched;
  
  fNoteIsAGraceNote = noteIsAGraceNote;

  initializeNote ();
}

void msrNote::initializeNote ()
{
  // note context
  // ------------------------------------------------------

  fNoteStaffNumber = 0;
  fNoteVoiceNumber = 0;

  fNoteBelongsToAChord = false;
  fNoteBelongsToATuplet = false;

  // note lyrics
  // ------------------------------------------------------

  fNoteSyllableExtendKind =
    msrSyllable::k_NoSyllableExtend;
  
  if (gGeneralOptions->fTraceNotes) {
    cerr << idtr <<
      "Creating a note, kind: ";
    if (fNoteKind == k_NoNoteKind)
      cerr <<
        "not yet known";
    else
      cerr <<
        noteKindAsString (fNoteKind);
    cerr <<
      ", line " << fInputLineNumber << ":" <<
      endl;

    idtr++;
    
    const int fieldWidth = 23;
    
    cerr <<
      idtr << left <<
        setw(fieldWidth) <<
        "fNoteQuatertonesPitch" << " = " <<
        msrQuartertonesPitchAsString (
          gMsrOptions->fMsrQuatertonesPitchesLanguage,
          fNoteQuatertonesPitch) <<
        endl <<
      idtr << left <<
        setw(fieldWidth) <<
        "fNoteSoundingDivisions" << " = " <<
        fNoteSoundingDivisions <<
        endl <<
      idtr << left <<
        setw(fieldWidth) <<
        "fNoteDisplayedDivisions" << " = " <<
        fNoteDisplayedDivisions <<
        endl <<
      idtr << left <<
        setw(fieldWidth) <<
        "fNoteDotsNumber" << " = " <<
        fNoteDotsNumber <<
        endl <<

/*
    if (fNoteDirectPartUplink) // it may not have been set yet
    cerr <<
      idtr << left <<
        setw(fieldWidth) << "fNoteGraphicDuration" << " = " <<
        fNoteDirectPartUplink->
          divisionsAsMsrString (
            fInputLineNumber,
            fNoteGraphicDuration) <<
        endl <<
*/
      idtr << left <<
        setw(fieldWidth) <<
        "fNoteGraphicDuration" << " = ";
      if (fNoteGraphicDuration != k_NoDuration)
        cerr <<
          msrDurationAsString (
            fNoteGraphicDuration);
      else
        cerr << "none";

    cerr <<
        endl <<

      idtr << left <<
        setw(fieldWidth) <<
        "fNoteOctave" << " = " <<
        fNoteOctave <<
        endl <<

      idtr << left <<
        setw(fieldWidth) <<
        "fNoteIsARest" << " = " <<
        fNoteIsARest <<
        endl <<
      idtr << left <<
        setw(fieldWidth) <<
        "fNoteIsUnpitched" << " = " <<
        fNoteIsUnpitched <<
        endl <<

      idtr << left <<
        setw(fieldWidth) <<
        "fNoteIsAGraceNote" << " = " <<
        fNoteIsAGraceNote <<
        endl <<

        // fNoteOctaveShift JMI ???
        
            
      idtr << left <<
        setw(fieldWidth) <<
        "fStaffNumber" << " = " <<
        fNoteStaffNumber <<
        endl <<
      idtr <<
        setw(fieldWidth) <<
        "fVoiceNumber" << " = " <<
        fNoteVoiceNumber <<
        endl <<

      idtr << left <<
        setw(fieldWidth) <<
        "fNoteBelongsToAChord" << " = " <<
        fNoteBelongsToAChord <<
        endl <<
            
      idtr << left <<
        setw(fieldWidth) <<
        "fNoteBelongsToATuplet" << " = " <<
        fNoteBelongsToATuplet <<
        endl <<
          
      idtr << left <<
        setw(fieldWidth) <<
        "fNoteSyllableExtendKind" << " = " <<
        msrSyllable::syllableExtendKindAsString (
          fNoteSyllableExtendKind) <<
        endl;
          
    idtr--;

/*
    cerr <<
      idtr <<
        "% <==" <<
        endl << endl;
*/
  }

  // note measure information
  // ------------------------------------------------------

  fNoteMeasureNumber = "-10011";
  fNotePositionInMeasure = -20022;
  fNoteOccupiesAFullMeasure = false;

  // note redundant information (for speed)
  // ------------------------------------------------------

  fNoteIsStemless = false;
  
  fNoteHasATrill = false;
  fNoteIsFollowedByGraceNotes = false;
  
  fNoteHasADelayedOrnament = false;
}

msrNote::~msrNote()
{}

S_msrNote msrNote::createNoteShallowClone (
  S_msrPart partClone)
{
  if (gGeneralOptions->fTraceNotes) {
    cerr << idtr <<
      "Creating a shallow clone of note " <<
      noteAsString () <<
      endl;
  }

  msrAssert(
    partClone != 0,
    "partClone is null");
    
  S_msrNote
    clone =
      msrNote::create (
        fInputLineNumber,
        partClone,
        
        fNoteKind,
        
        fNoteQuatertonesPitch,
        fNoteSoundingDivisions,
        fNoteDisplayedDivisions,
        fNoteDotsNumber,
        fNoteGraphicDuration,
        
        fNoteOctave,
        
        fNoteIsARest,
        fNoteIsUnpitched,
        
        fNoteIsAGraceNote);

  // basic note description
  // ------------------------------------------------------

/* JMI
  clone->fNoteKind =
    fNoteKind;

  clone->
    fNoteQuatertonesPitch =
      fNoteQuatertonesPitch;
  clone->
    fNoteSoundingDivisions =
      fNoteSoundingDivisions;
  clone->
    fNoteDisplayedDivisions =
      fNoteDisplayedDivisions;
  clone->
    fNoteDotsNumber =
      fNoteDotsNumber;
  clone->
    fNoteGraphicDuration =
      fNoteGraphicDuration;

  clone->
    fNoteOctave =
      fNoteOctave;

  clone->
    fNoteIsARest =
      fNoteIsARest;
  clone->
    fNoteIsUnpitched =
      fNoteIsUnpitched;
  
  clone->
    fNoteIsAGraceNote =
      fNoteIsAGraceNote;
      */

  clone->fNoteOctaveShift =
    fNoteOctaveShift;
  
  // note context
  // ------------------------------------------------------

  clone->
    fNoteStaffNumber =
      fNoteStaffNumber;
  clone->
    fNoteVoiceNumber =
      fNoteVoiceNumber;

  clone->
    fNoteBelongsToAChord =
      fNoteBelongsToAChord;

  clone->
    fNoteBelongsToATuplet =
      fNoteBelongsToATuplet;

  // note lyrics
  // ------------------------------------------------------

  clone->
    fNoteSyllableExtendKind =
      fNoteSyllableExtendKind;

  // elements attached to the note
  // ------------------------------------------------------

  clone->fNoteStem = // JMI
    fNoteStem;

  clone->fNoteTie = // JMI
    fNoteTie;

  clone->fNoteHarmony =
    fNoteHarmony;

  // note measure information
  // ------------------------------------------------------

  clone->
    fNoteMeasureNumber =
      fNoteMeasureNumber;
  clone->
    fNotePositionInMeasure =
      fNotePositionInMeasure;
  clone->
    fNoteOccupiesAFullMeasure =
      fNoteOccupiesAFullMeasure;

  // note redundant information (for speed)
  // ------------------------------------------------------

  clone->fNoteIsStemless =
    fNoteIsStemless;

  clone->fNoteIsFirstNoteInADoubleTremolo =
    fNoteIsFirstNoteInADoubleTremolo;
  clone->fNoteIsSecondNoteInADoubleTremolo =
    fNoteIsSecondNoteInADoubleTremolo;

  clone->fNoteHasATrill =
    fNoteHasATrill;
  clone->fNoteIsFollowedByGraceNotes =
    fNoteIsFollowedByGraceNotes;

  clone->fNoteHasADelayedOrnament =
    fNoteHasADelayedOrnament;

  return clone;
}

string msrNote::noteKindAsString (
  msrNoteKind noteKind)
{
  string result;
  
  switch (noteKind) {
    case msrNote::kRestNote:
      result = "rest";
      break;
      
    case msrNote::kSkipNote:
      result = "skip";
      break;
      
    case msrNote::kStandaloneNote:
      result = "standalone";
      break;
      
    case msrNote::kDoubleTremoloMemberNote:
      result = "double tremolo member note";
      break;
      
    case msrNote::kGraceNote:
      result = "grace note";
      break;
      
    case msrNote::kChordMemberNote:
      result = "chord member";
      break;
      
    case msrNote::kTupletMemberNote:
      result = "tuplet member";
      break;

    case msrNote::k_NoNoteKind:
      result = "k_NoNoteKind???";
      break;
  } // switch

  return result;
}

msrDiatonicPitch msrNote::noteDiatonicPitch (
  int inputLineNumber) const
{
  return
    msrDiatonicPitchFromQuatertonesPitch (
      inputLineNumber,
      fNoteQuatertonesPitch);
}

void msrNote::setNoteStem (S_msrStem stem)
{
  // register note stem
  fNoteStem = stem;

  // mark note as stemless if relevant
  if (stem->getStemKind () == msrStem::kStemNone)
    fNoteIsStemless = true;
}

void msrNote::setNoteBelongsToAChord ()
{
  if (gGeneralOptions->fTraceChords)
    cerr << idtr <<
      "Setting note '" << noteAsShortStringWithRawDivisions () <<
      "' to belong to a chord"
      ", line " << fInputLineNumber <<
      endl;

  fNoteBelongsToAChord = true;
  fNoteKind = msrNote::kChordMemberNote;
}

void msrNote::applyTupletMemberDisplayFactorToNote (
  int actualNotes, int normalNotes)
{
  if (gGeneralOptions->fTraceTuplets || gGeneralOptions->fTraceNotes)
    cerr << idtr <<
      "Applying tuplet display factor '" <<
      actualNotes << "/" << normalNotes <<
      "' to note '" << noteAsShortStringWithRawDivisions () <<
      "', line " << fInputLineNumber <<
      endl;

  fNoteDisplayedDivisions =
    fNoteDisplayedDivisions
      *
    actualNotes
      /
    normalNotes;
}

void msrNote::addBeamToNote (S_msrBeam beam)
{
  fNoteBeams.push_back (beam);
}

void msrNote::addArticulationToNote (S_msrArticulation art)
{
  fNoteArticulations.push_back (art);
}

void msrNote::addTechnicalToNote (S_msrTechnical technical)
{
  if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceTechnicals)
    cerr << idtr <<
      "Adding technical '" <<
      technical->technicalAsString () <<
      "' to note '" << noteAsShortString () <<
      "', line " << fInputLineNumber <<
      endl;

  // append the technical to the note technicals list
  fNoteTechnicals.push_back (technical);

  // set technical's note uplink
  technical->
    setTechnicalNoteUplink (this);
}

void msrNote::addTechnicalWithIntegerToNote (
  S_msrTechnicalWithInteger technicalWithInteger)
{
  if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceTechnicals)
    cerr << idtr <<
      "Adding technical with integer '" <<
      technicalWithInteger->technicalWithIntegerAsString () <<
      "' to note '" << noteAsShortString () <<
      "', line " << fInputLineNumber <<
      endl;

  // append the technical with integer to the note technicals with integers list
  fNoteTechnicalWithIntegers.push_back (technicalWithInteger);

  // set technical's note uplink
  technicalWithInteger->
    setTechnicalWithIntegerNoteUplink (this);
}

void msrNote::addTechnicalWithStringToNote (
  S_msrTechnicalWithString technicalWithString)
{
  if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceTechnicals)
    cerr << idtr <<
      "Adding technical with string'" <<
      technicalWithString->technicalWithStringAsString () <<
      "' to note '" << noteAsShortString () <<
      "', line " << fInputLineNumber <<
      endl;

  // append the technical with integer to the note technicals with integers list
  fNoteTechnicalWithStrings.push_back (technicalWithString);

  // set technical's note uplink
  technicalWithString->
    setTechnicalWithStringNoteUplink (this);
}

void msrNote::addOrnamentToNote (S_msrOrnament ornament)
{
  // append the ornament to the note ornaments list
  fNoteOrnaments.push_back (ornament);

  switch (ornament->getOrnamentKind ()) {
    case msrOrnament::kTrillMark:
      fNoteHasATrill = true;
      break;

    case msrOrnament::kDelayedTurn:
    case msrOrnament::kDelayedInvertedTurn:
      fNoteHasADelayedOrnament = true;
      break;

    default:
      {}
  } // switch

  // set ornament's note uplink
  ornament->
    setOrnamentNoteUplink (this);
}

void msrNote::addSingleTremoloToNote (S_msrSingleTremolo trem)
{
  if (gGeneralOptions->fTraceTremolos)
    cerr << idtr <<
      "Adding singleTremolo " << trem->singleTremoloAsString () <<
      " to note " << noteAsShortString () <<
      ", line " << trem->getInputLineNumber () <<
      endl;

  // register the singleTremolo in the note
  fNoteSingleTremolo = trem;

  // set singleTremolo's note uplink
  trem->
    setSingleTremoloNoteUplink (this);
}

void msrNote::addDynamicsToNote (S_msrDynamics dynamics)
{
  fNoteDynamics.push_back (dynamics);
}
void msrNote::addOtherDynamicsToNote (S_msrOtherDynamics otherDynamics)
{
  fNoteOtherDynamics.push_back (otherDynamics);
}

void msrNote::addWordsToNote (S_msrWords words)
{
  fNoteWords.push_back (words);
}

void msrNote::addSlurToNote (S_msrSlur slur)
{
  if (gGeneralOptions->fTraceSlurs)
    cerr << idtr <<
      "Adding slur " << slur << " to note " << noteAsString ()
       << endl;

  if (fNoteSlurs.size ()) {
    if (
      fNoteSlurs.back ()->getSlurKind () == msrSlur::kStartSlur
        &&
      slur->getSlurKind () == msrSlur::kStopSlur
        &&
      fNoteSlurs.back ()->getSlurNumber () == slur->getSlurNumber ()
      ) {
      // it may happen that a given note has a 'slur start'
      // and a 'slur stop' in sequence, ignore both

      stringstream s;
      
      s <<
        "a 'slur start' is immediately followed by a 'slur stop'" <<
        endl <<
        "with the same number, ignoring both of them at line " <<
        slur->getInputLineNumber ();
        
      msrMusicXMLWarning (
        slur->getInputLineNumber (),
        s.str());
        
      // rmeove 'slur start'
      fNoteSlurs.pop_back ();

      // don't register 'slur stop'
    }

    else
      fNoteSlurs.push_back (slur);
  }

  else
    fNoteSlurs.push_back (slur);
}

void msrNote::addLigatureToNote (S_msrLigature ligature)
{
  if (gGeneralOptions->fTraceLigatures)
    cerr << idtr <<
      "Adding ligature " << ligature << " to note " << noteAsString ()
       << endl;

  if (fNoteLigatures.size ()) {
    if (
      fNoteLigatures.back ()->getLigatureKind () == msrLigature::kStartLigature
        &&
      ligature->getLigatureKind () == msrLigature::kStopLigature
        &&
      fNoteLigatures.back ()->getLigatureNumber () == ligature->getLigatureNumber ()
      ) {
      // it may happen that a given note has a 'ligature start'
      // and a 'ligature stop' in sequence, ignore both

      stringstream s;
      
      s <<
        "a 'ligature start' is immediately followed by a 'ligature stop'" <<
        endl <<
        "with the same number, ignoring both of them at line " <<
        ligature->getInputLineNumber ();
        
      msrMusicXMLWarning (
        ligature->getInputLineNumber (),
        s.str());
        
      // rmeove 'ligature start'
      fNoteLigatures.pop_back ();

      // don't register 'ligature stop'
    }

    else
      fNoteLigatures.push_back (ligature);
  }

  else
    fNoteLigatures.push_back (ligature);
}

void msrNote::addWedgeToNote (S_msrWedge wedge)
{
  fNoteWedges.push_back (wedge);
}

S_msrDynamics msrNote::removeFirstDynamics () // JMI
{
  S_msrDynamics dyn = fNoteDynamics.front ();
  fNoteDynamics.pop_front ();
  return dyn;
}

S_msrWedge msrNote::removeFirstWedge () // JMI
{
  S_msrWedge wedge = fNoteWedges.front ();
  fNoteWedges.pop_front ();
  return wedge;
}

void msrNote::appendSyllableToNote (S_msrSyllable syllable)
{
  if (gGeneralOptions->fTraceLyrics)
    cerr << idtr <<
      "Appending syllable " <<
      syllable->syllableAsString () <<
      " to note " << noteAsString () <<
      endl;

  fNoteSyllables.push_back (syllable);
}

void msrNote::setNoteHarmony (S_msrHarmony harmony)
{
  if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceHarmonies)
    cerr << idtr <<
      "Setting note '" << noteAsShortString ()  << "'" <<
      " harmony to '" << harmony->harmonyAsString () << "'" <<
      endl;
      
  fNoteHarmony = harmony;
}

void msrNote::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrNote::acceptIn()" <<
      endl;
      
  if (visitor<S_msrNote>*
    p =
      dynamic_cast<visitor<S_msrNote>*> (v)) {
        S_msrNote elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrNote::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrNote::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrNote::acceptOut()" <<
      endl;

  if (visitor<S_msrNote>*
    p =
      dynamic_cast<visitor<S_msrNote>*> (v)) {
        S_msrNote elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrNote::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrNote::browseData (basevisitor* v)
{
  if (fNoteOctaveShift) {
    // browse the octave shift
    msrBrowser<msrOctaveShift> browser (v);
    browser.browse (*fNoteOctaveShift);
  }

  if (fNoteStem) {
    // browse the stem
    msrBrowser<msrStem> browser (v);
    browser.browse (*fNoteStem);
  }

  // browse the beams if any
  if (fNoteBeams.size()) {
    idtr++;
    list<S_msrBeam>::const_iterator i;
    for (i=fNoteBeams.begin(); i!=fNoteBeams.end(); i++) {
      // browse the beam
      msrBrowser<msrBeam> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }
  
  // browse the articulations if any
  if (fNoteArticulations.size()) {
    idtr++;
    list<S_msrArticulation>::const_iterator i;
    for (i=fNoteArticulations.begin(); i!=fNoteArticulations.end(); i++) {
      // browse the articulation
      msrBrowser<msrArticulation> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }
  
  // browse the technicals if any
  if (fNoteTechnicals.size()) {
    idtr++;
    list<S_msrTechnical>::const_iterator i;
    for (
      i=fNoteTechnicals.begin();
      i!=fNoteTechnicals.end();
      i++) {
      // browse the technical
      msrBrowser<msrTechnical> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }
  
  // browse the technicals with integer if any
  if (fNoteTechnicalWithIntegers.size()) {
    idtr++;
    list<S_msrTechnicalWithInteger>::const_iterator i;
    for (
      i=fNoteTechnicalWithIntegers.begin();
      i!=fNoteTechnicalWithIntegers.end();
      i++) {
      // browse the technical
      msrBrowser<msrTechnicalWithInteger> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }
  
  // browse the technicals with string if any
  if (fNoteTechnicalWithStrings.size()) {
    idtr++;
    list<S_msrTechnicalWithString>::const_iterator i;
    for (
      i=fNoteTechnicalWithStrings.begin();
      i!=fNoteTechnicalWithStrings.end();
      i++) {
      // browse the technical
      msrBrowser<msrTechnicalWithString> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }
  
  // browse the ornaments if any
  if (fNoteOrnaments.size()) {
    idtr++;
    list<S_msrOrnament>::const_iterator i;
    for (i=fNoteOrnaments.begin(); i!=fNoteOrnaments.end(); i++) {
      // browse the ornament
      msrBrowser<msrOrnament> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }
  
  if (fNoteSingleTremolo) {
    // browse the singleTremolo
    msrBrowser<msrSingleTremolo> browser (v);
    browser.browse (*fNoteSingleTremolo);
  }
  
  if (fNoteTie) {
    // browse the tie
    msrBrowser<msrTie> browser (v);
    browser.browse (*fNoteTie);
  }

  // browse the dynamics if any
  if (fNoteDynamics.size()) {
    idtr++;
    list<S_msrDynamics>::const_iterator i;
    for (i=fNoteDynamics.begin(); i!=fNoteDynamics.end(); i++) {
      // browse the dynamics
      msrBrowser<msrDynamics> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }

  // browse the other dynamics if any
  if (fNoteOtherDynamics.size()) {
    idtr++;
    list<S_msrOtherDynamics>::const_iterator i;
    for (i=fNoteOtherDynamics.begin(); i!=fNoteOtherDynamics.end(); i++) {
      // browse the other dynamics
      msrBrowser<msrOtherDynamics> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }

  // browse the words if any
  if (fNoteWords.size()) {
    idtr++;
    list<S_msrWords>::const_iterator i;
    for (i=fNoteWords.begin(); i!=fNoteWords.end(); i++) {
      // browse the words
      msrBrowser<msrWords> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }
  
  // browse the slurs if any
  if (fNoteSlurs.size()) {
    idtr++;
    list<S_msrSlur>::const_iterator i;
    for (i=fNoteSlurs.begin(); i!=fNoteSlurs.end(); i++) {
      // browse the slur
      msrBrowser<msrSlur> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }

  // browse the ligatures if any
  if (fNoteLigatures.size()) {
    idtr++;
    list<S_msrLigature>::const_iterator i;
    for (i=fNoteLigatures.begin(); i!=fNoteLigatures.end(); i++) {
      // browse the ligature
      msrBrowser<msrLigature> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }

  // browse the wedges if any
  if (fNoteWedges.size()) {
    idtr++;
    list<S_msrWedge>::const_iterator i;
    for (i=fNoteWedges.begin(); i!=fNoteWedges.end(); i++) {
      // browse the wedge
      msrBrowser<msrWedge> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }

  // browse the syllables if any
  if (fNoteSyllables.size()) {
    idtr++;
    list<S_msrSyllable>::const_iterator i;
    for (i=fNoteSyllables.begin(); i!=fNoteSyllables.end(); i++) {
      // browse the wedge
      msrBrowser<msrSyllable> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }

  if (fNoteHarmony) {
    // browse the harmony
    msrBrowser<msrHarmony> browser (v);
    browser.browse (*fNoteHarmony);
  }
}

string msrNote::notePitchAsString () const
{
  stringstream s;
  
  /*
  cerr << "msrNote::notePitchAsString (), isRest = " <<
    fNoteIsARest <<
    ", fQuartertonesPitch = " << fQuartertonesPitch << endl;
  */
  
  if (fNoteIsARest) {
    if (fNoteOccupiesAFullMeasure) // JMI
      s << "R";
    else
      s << "r";
  }
  
  else if (fNoteIsUnpitched)

    s << "unpitched ";

  else {

    s <<
      msrQuartertonesPitchAsString (
        gMsrOptions->fMsrQuatertonesPitchesLanguage,
        fNoteQuatertonesPitch);  

  }
  
  return s.str();
}

string msrNote::noteSoundingDivisionsAsMsrString () const
{
  string result;
  int    computedNumberOfDots = 0;

  result =
    fNoteDirectPartUplink->
      divisionsAsMsrString (
        fInputLineNumber,
        fNoteSoundingDivisions,
        computedNumberOfDots);

  if (computedNumberOfDots != fNoteDotsNumber) {
    stringstream s;

    s <<
      "note " << noteAsShortStringWithRawDivisions () <<
      " needs " <<
      singularOrPlural (
        computedNumberOfDots, "dot", "dots") <<
      " for " << fNoteSoundingDivisions << " sounding divisions," <<
      endl <<
      "but '" << fNoteDotsNumber << "' is found in MusicXML data" <<
      endl;

    fNoteDirectPartUplink->
      printDurationsDivisions (s);

    msrMusicXMLWarning (
      fInputLineNumber,
      s.str());

    abort ();
  }

  return result;
}

string msrNote::noteDisplayedDivisionsAsMsrString () const
{
  string result;
  int    computedNumberOfDots = 0;

  result =
    fNoteDirectPartUplink->
      divisionsAsMsrString (
        fInputLineNumber,
        fNoteDisplayedDivisions,
        computedNumberOfDots);

  if (computedNumberOfDots != fNoteDotsNumber) {
    stringstream s;

    s <<
      "note " << noteAsShortStringWithRawDivisions () <<
      " needs " <<
      singularOrPlural (
        computedNumberOfDots, "dot", "dots") <<
      " for " << fNoteDisplayedDivisions << " displayed divisions," <<
      endl <<
      "but '" << fNoteDotsNumber << "' is found in MusicXML data" <<
      endl;

    fNoteDirectPartUplink->
      printDurationsDivisions (s);

    msrMusicXMLWarning (
      fInputLineNumber,
      s.str());

    abort ();
  }

  return result;
}

string msrNote::skipOrRestDivisionsAsMsrString () const
{
  string result;
//  int    computedNumberOfDots;
//  string errorMessage;

  result =
    fNoteDirectPartUplink->
      divisionsAsMsrString (
        fInputLineNumber,
        fNoteSoundingDivisions);
/* JMI
  if (errorMessage.size ())
    msrMusicXMLError (
      fInputLineNumber,
      errorMessage);
*/
  return result;
}

string msrNote::noteGraphicDurationAsMsrString () const
{
  string
    result =
      msrDurationAsString (
        fNoteGraphicDuration);
    
  return result;
}

string msrNote::tupletNoteGraphicDurationAsMsrString ( // JMI
  int actualNotes, int normalNotes) const
{
  string result;

  result =
    fNoteDirectPartUplink->
      tupletDivisionsAsMsrString (
        fInputLineNumber,
        fNoteSoundingDivisions,
        actualNotes,
        normalNotes);

  return result;
}

string msrNote::noteDiatonicPitchAsString (
  int inputLineNumber) const
{
  return
    msrDiatonicPitchAsString (
      gMsrOptions->fMsrQuatertonesPitchesLanguage,
      noteDiatonicPitch (
        fInputLineNumber));
}

string msrNote::noteAsShortStringWithRawDivisions () const
{
  stringstream s;

  switch (fNoteKind) {
    case msrNote::k_NoNoteKind:
      s <<
        "???";
      break;
      
    case msrNote::kStandaloneNote:
      s <<
        notePitchAsString () <<
        "[" << fNoteOctave << "]" <<
        ":" <<
        ", divs:" <<
        fNoteSoundingDivisions <<
        " sound, " <<
        fNoteDisplayedDivisions <<
        " disp";
      break;
      
    case msrNote::kDoubleTremoloMemberNote:
      s <<
        notePitchAsString () <<
        "[" << fNoteOctave << "]" <<
        ":" <<
        " divs:" <<
        fNoteSoundingDivisions <<
        " sound, " <<
        fNoteDisplayedDivisions <<
        " disp";
      break;
      
    case msrNote::kGraceNote:
      s <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        "[" << fNoteOctave << "]";
        
      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << "."; // JMI
      } // for
      break;
      
    case msrNote::kRestNote:
      s <<
        "R" <<
        ":" <<
        fNoteSoundingDivisions <<
        " sounddivs";
      break;
      
    case msrNote::kSkipNote:
      s <<
        "S" <<
        ":" <<
        ", divs:" <<
        fNoteSoundingDivisions <<
        " sound, " <<
        fNoteDisplayedDivisions <<
        " disp";
      break;
      
    case msrNote::kChordMemberNote:
      s <<
        notePitchAsString () <<
        "[" << fNoteOctave << "]" <<
        ", divs:" <<
        fNoteSoundingDivisions <<
        " sound, " <<
        fNoteDisplayedDivisions <<
        " disp";
      break;
      
    case msrNote::kTupletMemberNote:
      s <<
        notePitchAsString ();

      if (! fNoteIsARest)
        s <<
          "[" << fNoteOctave << "]";

      s <<
        ", divs:" <<
        fNoteSoundingDivisions <<
        " sound, " <<
        fNoteDisplayedDivisions <<
        " disp";
      break;
  } // switch

  return s.str();
}

string msrNote::noteAsShortString () const
{
  stringstream s;

  switch (fNoteKind) {
    case msrNote::k_NoNoteKind:
      s <<
        "???";
      break;
      
    case msrNote::kStandaloneNote:
      s <<
        notePitchAsString () <<
        noteSoundingDivisionsAsMsrString () <<
        "[" << fNoteOctave << "]";
      break;
      
    case msrNote::kDoubleTremoloMemberNote:
      s <<
        notePitchAsString () <<
        noteSoundingDivisionsAsMsrString () <<
        "[" << fNoteOctave << "]";
      break;
      
    case msrNote::kGraceNote:
      s <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        "[" << fNoteOctave << "]";
        
      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << "."; // JMI
      } // for
      break;
      
    case msrNote::kRestNote:
      s <<
        "R" <<
        ":" <<
        skipOrRestDivisionsAsMsrString ();
      break;
      
    case msrNote::kSkipNote:
      s <<
        "S" <<
        ":" <<
        skipOrRestDivisionsAsMsrString ();
      break;
      
    case msrNote::kChordMemberNote:
      s <<
        notePitchAsString () <<
        ", " <<
        noteSoundingDivisionsAsMsrString () <<
        "[" << fNoteOctave << "]";
      break;
      
    case msrNote::kTupletMemberNote:
      s <<
        notePitchAsString () <<
        ", divs:" <<
        fNoteSoundingDivisions <<
        " sound, " <<
        fNoteDisplayedDivisions <<
        " disp";
        /* JMI
        fNoteDirectPartUplink->
          tupletSoundingDivisionsAsMsrString (
            fInputLineNumber,
            fNoteSoundingDivisions,
            fNoteTupletUplink->getTupletActualNotes (),
            fNoteTupletUplink->getTupletNormalNotes ());
            */

      if (! fNoteIsARest)
        s <<
          "[" << fNoteOctave << "]";
      break;
  } // switch

  return s.str();
}

string msrNote::noteAsString () const
{
  stringstream s;

  s <<
    "[=== ";
    
  switch (fNoteKind) {
    case msrNote::k_NoNoteKind:
      s <<
        "k_NoNoteKind???";
      break;
      
    case msrNote::kStandaloneNote:
      s <<
        "Standalone note" " "<<
        notePitchAsString () <<
        noteSoundingDivisionsAsMsrString () <<
        "[" << fNoteOctave << "]";
      break;
      
    case msrNote::kDoubleTremoloMemberNote:
      s <<
        "Double tremolo note" " "<<
        notePitchAsString () <<
        noteSoundingDivisionsAsMsrString () <<
        "[" << fNoteOctave << "]";
      break;
      
    case msrNote::kGraceNote:
      s <<
        "Grace note" " "<<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        "[" << fNoteOctave << "]";
        
      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << "."; // JMI
      } // for
      break;
      
    case msrNote::kRestNote:
      s <<
        "Rest" <<
        ":" <<
        skipOrRestDivisionsAsMsrString ();
      break;
      
    case msrNote::kSkipNote:
      s <<
        "Skip" <<
        ":" <<
        skipOrRestDivisionsAsMsrString ();
      break;
      
    case msrNote::kChordMemberNote:
      s <<
        "Chord member note" " "<<
        notePitchAsString () <<
 // JMI       ", " << fNoteSoundingDivisions << " sounddivs, " <<
        "[" << fNoteOctave << "]";
      break;
      
    case msrNote::kTupletMemberNote:
      s <<
        "Tuplet member note"  " "<<
        notePitchAsString () <<
        ", divs:" <<
        fNoteSoundingDivisions <<
        " sound, " <<
        fNoteDisplayedDivisions <<
        " disp";
/* JMI
        fNoteDirectPartUplink->
          tupletSoundingDivisionsAsMsrString (
            fInputLineNumber,
            fNoteSoundingDivisions,
            fNoteTupletUplink->getTupletActualNotes (),
            fNoteTupletUplink->getTupletNormalNotes ())
            */

      if (! fNoteIsARest)
        s <<
          "[" << fNoteOctave << "]";
      break;
  } // switch

  if (fNoteOccupiesAFullMeasure)
    s <<
      ", full measure";

  if (fNoteHasATrill)
    s <<
      ", has a trill";
  
  if (fNoteIsFollowedByGraceNotes)
    s <<
      ", followed by grace notes";
  
/* JMI
  if (fNoteTie) {
    if (fNoteTie->getTieKind () != msrTie::k_NoTie ) {
      s <<
        ", " << fNoteTie->tieKindAsString ();
    }
  }
*/

  s <<
    " ===]";
  
  return s.str();
}

ostream& operator<< (ostream& os, const S_msrNote& elt)
{
  elt->print (os);
  return os;
}

void msrNote::print (ostream& os)
{
  int divisionsPerFullMeasure =
    fNoteMeasureUplink
      ? 
        fNoteMeasureUplink->
          getMeasureDivisionsPerFullMeasure ()
      : 0; // JMI
  
  // print the note itself and its position
  os <<
    noteAsString ();

  os <<
      ", line " << fInputLineNumber;

  // print displayed divisions
  os <<
    ", ";
    
  switch (fNoteKind) {
    case msrNote::kGraceNote:
      os <<
        fNoteDisplayedDivisions <<
        " dispdivs";
      break;

    case msrNote::kTupletMemberNote:
      os <<
        "divs: " <<
        fNoteSoundingDivisions <<
        " sound, " <<
        fNoteDisplayedDivisions<<
        " disp";
/* JMI
      if (
          fNoteSoundingDivisions
            ==
          fNoteDisplayedDivisions) {
        os <<
          fNoteSoundingDivisions <<
          " divs";
      }
      */
      break;
      
    case msrNote::k_NoNoteKind:
    case msrNote::kStandaloneNote:
    case msrNote::kDoubleTremoloMemberNote:
    case msrNote::kRestNote:
    case msrNote::kSkipNote:
    case msrNote::kChordMemberNote:
      os <<
        "divs: " <<
        fNoteSoundingDivisions <<
        " sound, " <<
        fNoteDisplayedDivisions<<
        " disp";
  } // switch

  if (divisionsPerFullMeasure > 0)
    os <<
      "/" <<
      divisionsPerFullMeasure <<
      " dpfm";

  // print measure related information
  os <<
    ", meas " <<
    fNoteMeasureNumber <<
    ":";
    
  if (fNotePositionInMeasure < 0)
    os << "?";
  else
    os << fNotePositionInMeasure;
    
  if (divisionsPerFullMeasure > 0)
    os <<
      "/" <<
      divisionsPerFullMeasure;

  // print simplified position in measure if relevant
  if (fNoteMeasureUplink) {
    // the note measure uplink may not have been set yet
    rational
      notePosition (
        fNotePositionInMeasure,
        divisionsPerFullMeasure); // JMI
    notePosition.rationalise ();
    
    if (notePosition.getNumerator () != fNotePositionInMeasure)
      // print simplified rational view
      os <<
        " (" <<
        notePosition.getNumerator () <<
        "/" <<
        notePosition.getDenominator() <<
        ")";
  }

  // note redundant information (for speed)
  if (fNoteIsStemless)
    os <<
      ", stemless";

  if (fNoteIsFirstNoteInADoubleTremolo)
    os <<
      ", first note in a double tremolo";
  if (fNoteIsSecondNoteInADoubleTremolo)
    os <<
      ", second note in a double tremolo";

  if (fNoteHasATrill)
    os <<
      ", has a trill";
  if (fNoteIsFollowedByGraceNotes)
    os <<
      ", is followed by graceNotes";

  if (fNoteHasADelayedOrnament)
    os <<
      ", has a delayed ornament";

  os <<
      endl;
    
  // are there syllables associated to this note?
  if (fNoteSyllables.size ()) {
    idtr++;

    os <<
      idtr <<
      "Syllables:" <<
      endl;
      
    idtr++;
    
    list<S_msrSyllable>::const_iterator
      iBegin = fNoteSyllables.begin(),
      iEnd   = fNoteSyllables.end(),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        idtr <<
        " \"" << (*i)->getSyllableText () << "\"" <<
        ", line " << (*i)->getInputLineNumber ();
      if (++i == iEnd) break;
      os << endl;
    } // for

    idtr--;
    
    os <<
      endl;

    idtr--;
  }

/*
  // print the note's voice uplink JMI
  idtr++;
  os <<
    idtr <<
    "NoteVoiceDirectUplink" " = " <<
    fNoteMeasureUplink->getMeasureVoiceDirectUplink () <<
    endl;
  idtr--;
*/

  // print the extend kind if relevant
  switch (fNoteSyllableExtendKind) {
    
    case msrSyllable::kStandaloneSyllableExtend:
    case msrSyllable::kStartSyllableExtend:
    case msrSyllable::kContinueSyllableExtend:
    case msrSyllable::kStopSyllableExtend:
      idtr++;
      os <<
        idtr <<
        "NoteSyllableExtendKind" " = " <<
        msrSyllable::syllableExtendKindAsString (
          fNoteSyllableExtendKind) <<
        endl;
      idtr--;
      break;
      
    case msrSyllable::k_NoSyllableExtend:
      break;
  } // switch

  // print the octave shift if any
  if (fNoteOctaveShift) {
    idtr++;
    os <<
      idtr << fNoteOctaveShift;
    idtr--;
  }

  // print the stem if any
  if (fNoteStem) {
    idtr++;
    os <<
      idtr << fNoteStem;
    idtr--;
  }
    
  // print the beams if any
  if (fNoteBeams.size()) {
    idtr++;

    list<S_msrBeam>::const_iterator
      iBegin = fNoteBeams.begin(),
      iEnd   = fNoteBeams.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
        
    idtr--;
  }
  
  // print the articulations if any
  if (fNoteArticulations.size()) {
    idtr++;

    list<S_msrArticulation>::const_iterator
      iBegin = fNoteArticulations.begin(),
      iEnd   = fNoteArticulations.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
// JMI      os << endl;
    } // for
        
    idtr--;
  }
  
  // print the technicals if any
  if (fNoteTechnicals.size()) {
    idtr++;

    list<S_msrTechnical>::const_iterator
      iBegin = fNoteTechnicals.begin(),
      iEnd   = fNoteTechnicals.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
        
    idtr--;
  }
  
  // print the technicals with integer if any
  if (fNoteTechnicalWithIntegers.size()) {
    idtr++;

    list<S_msrTechnicalWithInteger>::const_iterator
      iBegin = fNoteTechnicalWithIntegers.begin(),
      iEnd   = fNoteTechnicalWithIntegers.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
        
    idtr--;
  }
  
  // print the technicals with string if any
  if (fNoteTechnicalWithStrings.size()) {
    idtr++;

    list<S_msrTechnicalWithString>::const_iterator
      iBegin = fNoteTechnicalWithStrings.begin(),
      iEnd   = fNoteTechnicalWithStrings.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
// JMI      os << endl;
    } // for
        
    idtr--;
  }
  
  // print the ornaments if any
  if (fNoteOrnaments.size()) {
    idtr++;

    list<S_msrOrnament>::const_iterator
      iBegin = fNoteOrnaments.begin(),
      iEnd   = fNoteOrnaments.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
        
    idtr--;
  }
  
  // print the singleTremolo if any
  if (fNoteSingleTremolo) {
    idtr++;
    os << idtr << fNoteSingleTremolo;
    idtr--;
  }
  
  // print the tie if any
  if (fNoteTie) {
    idtr++;
    os <<
      idtr << fNoteTie;
    idtr--;
  }
    
  // print the dynamics if any
  if (fNoteDynamics.size()) {
    idtr++;
    
    list<S_msrDynamics>::const_iterator
      iBegin = fNoteDynamics.begin(),
      iEnd   = fNoteDynamics.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
// JMI    os << endl;

    idtr--;
  }

  // print the other dynamics if any
  if (fNoteOtherDynamics.size()) {
    idtr++;
    
    list<S_msrOtherDynamics>::const_iterator
      iBegin = fNoteOtherDynamics.begin(),
      iEnd   = fNoteOtherDynamics.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
// JMI      os << endl;
    } // for
// JMI    os << endl;

    idtr--;
  }

  // print the words if any
  if (fNoteWords.size()) {
    idtr++;
    
    list<S_msrWords>::const_iterator
      iBegin = fNoteWords.begin(),
      iEnd   = fNoteWords.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
// JMI      os << endl;
    } // for
// JMI    os << endl;

    idtr--;
  }

  // print the slurs if any
  if (fNoteSlurs.size()) {
    idtr++;
    
    list<S_msrSlur>::const_iterator
      iBegin = fNoteSlurs.begin(),
      iEnd   = fNoteSlurs.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
// JMI    os << endl;
    
    idtr--;
  }
  
  // print the ligatures if any
  if (fNoteLigatures.size()) {
    idtr++;
    
    list<S_msrLigature>::const_iterator
      iBegin = fNoteLigatures.begin(),
      iEnd   = fNoteLigatures.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
// JMI    os << endl;
    
    idtr--;
  }
  
  // print the wedges if any
  if (fNoteWedges.size()) {
    idtr++;
    
    list<S_msrWedge>::const_iterator
      iBegin = fNoteWedges.begin(),
      iEnd   = fNoteWedges.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
// JMI    os << endl;
    
    idtr--;
  }

  // print the harmony if any
  if (fNoteHarmony) {
    idtr++;
    os <<
      idtr <<
        fNoteHarmony <<
        endl;
    idtr--;
  }
}

//______________________________________________________________________________
S_msrChord msrChord::create (
  int         inputLineNumber,
  S_msrPart   chordDirectPartUplink,
  int         chordSoundingDivisions,
  int         chordDisplayedDivisions,
  msrDuration chordGraphicDuration)
{
  if (gGeneralOptions->fTraceChords) {
    cerr <<
      idtr <<
      "Creating a chord" <<
      ", chordSoundingDivisions = " << chordSoundingDivisions <<
      ", chordDisplayedDivisions = " << chordDisplayedDivisions <<
      ", chordGraphicDuration = " <<
      msrDurationAsString (chordGraphicDuration) <<
      endl;
 }
   
  msrChord* o =
    new msrChord (
      inputLineNumber,
      chordDirectPartUplink,
      chordSoundingDivisions, chordDisplayedDivisions,
      chordGraphicDuration);
  assert(o!=0);

  return o;
}

msrChord::msrChord (
  int         inputLineNumber,
  S_msrPart   chordDirectPartUplink,
  int         chordSoundingDivisions,
  int         chordDisplayedDivisions,
  msrDuration chordGraphicDuration)
    : msrElement (inputLineNumber)
{
  // set chord's direct part uplink
  msrAssert(
    chordDirectPartUplink != 0,
    "chordDirectPartUplink is null");
    
  fChordDirectPartUplink =
    chordDirectPartUplink;
    
  fChordSoundingDivisions  = chordSoundingDivisions;
  fChordDisplayedDivisions = chordDisplayedDivisions;
  
  fChordGraphicDuration = chordGraphicDuration;

  fChordIsFirstChordInADoubleTremolo  = false;
  fChordIsSecondChordInADoubleTremolo = false;
}

msrChord::~msrChord() {}

S_msrChord msrChord::createChordShallowClone (
  S_msrPart partClone)
{
  if (gGeneralOptions->fTraceChords) {
    cerr << idtr <<
      "Creating a shallow clone of chord '" <<
      chordAsString () <<
      "'" <<
      endl;
  }

  msrAssert(
    partClone != 0,
    "partClone is null");
    
  S_msrChord
    clone =
      msrChord::create (
        fInputLineNumber,
        partClone,
        fChordSoundingDivisions,
        fChordDisplayedDivisions,
        fChordGraphicDuration);

  clone->
    fChordPositionInMeasure =
      fChordPositionInMeasure;
    
  clone->
    fChordIsFirstChordInADoubleTremolo =
      fChordIsFirstChordInADoubleTremolo;
    
  clone->
    fChordIsSecondChordInADoubleTremolo =
      fChordIsSecondChordInADoubleTremolo;
    
  clone->fChordTie =
    fChordTie; // JMI

  return clone;
}

void msrChord::setChordSoundingDivisions (int divisions)
{
  if (gGeneralOptions->fTraceChords)
    cerr << idtr <<
      "Setting chord sounding divisions to '" <<
      divisions <<
      "' for chord '" <<
      chordAsString () <<
      "'" <<
      endl;

  fChordSoundingDivisions = divisions;
}

void msrChord::setChordDisplayedDivisions (int divisions)
{
  if (gGeneralOptions->fTraceChords)
    cerr << idtr <<
      "Setting chord displayed divisions to '" <<
      divisions <<
      "' for chord '" <<
      chordAsString () <<
      "'" <<
      endl;

  fChordDisplayedDivisions = divisions;
}

string msrChord::chordGraphicDurationAsMsrString () const
{
  string result;

  result =
    msrDurationAsString (
      fChordGraphicDuration);

  return result;
}

void msrChord::addFirstNoteToChord (S_msrNote note)
{
  if (gGeneralOptions->fTraceChords)
    cerr << idtr <<
      "Adding first note '" <<
      note->noteAsString () <<
      "' to chord '" <<
      chordAsString () <<
      "'" <<
      endl;

  fChordNotes.push_back (note);
  
  note->setNoteBelongsToAChord ();
  
  // populate note's measure number
  note->setNoteMeasureNumber (
    fChordMeasureNumber);

  // populate note's position in measure // JMI
  note->setNotePositionInMeasure (
    fChordPositionInMeasure);
}

void msrChord::addAnotherNoteToChord (S_msrNote note)
{
  if (gGeneralOptions->fTraceChords)
    cerr << idtr <<
      "Adding another note '" <<
      note->noteAsShortStringWithRawDivisions () <<
      "' to chord '" <<
      chordAsString () <<
      "'" <<
      endl;

  fChordNotes.push_back (note);
  
  note->setNoteBelongsToAChord ();
  
  // populate note's measure number
  note->setNoteMeasureNumber (
    fChordMeasureNumber);

  // populate note's position in measure
  note->setNotePositionInMeasure (
    fChordPositionInMeasure);
}

void msrChord::setChordFirstNotePositionInMeasure (
  int position)
{
  if (fChordNotes.size ()) {
    fChordNotes.front ()->
      setNotePositionInMeasure (position);
  }
  else {
    // JMI XXL
   // abort ();
 }
}
                    
void msrChord::setChordFirstNoteMeasureNumber (
  string measureNumber)
{
  if (fChordNotes.size ()) {
    fChordNotes.front ()->
      setNoteMeasureNumber (measureNumber);
  }
  else {
    // JMI XXL
   // abort ();
 }
}
                    
void msrChord::addArticulationToChord (S_msrArticulation art)
{
  msrArticulation::msrArticulationKind
    articulationKind =
      art->
        getArticulationKind ();

  // don't add the same articulation several times
  for (
    list<S_msrArticulation>::const_iterator i = fChordArticulations.begin();
    i!=fChordArticulations.end();
    i++) {
      if ((*i)->getArticulationKind () == articulationKind)
        return;
  } // for

  if (gGeneralOptions->fTraceTremolos || gGeneralOptions->fTraceChords) // JMI
    cerr << idtr <<
      "Adding articulation '" <<
      art->articulationKindAsString () <<
      "' to chord" <<
      endl;

  fChordArticulations.push_back (art);
}

void msrChord::addSingleTremoloToChord (S_msrSingleTremolo trem)
{
  if (gGeneralOptions->fTraceTremolos || gGeneralOptions->fTraceChords)
    cerr << idtr <<
      "Adding singleTremolo '" <<
      trem->singleTremoloAsString () <<
      "' to chord" <<
      endl;

  fChordSingleTremolo = trem;
}

void msrChord::addTechnicalToChord (S_msrTechnical tech)
{
  msrTechnical::msrTechnicalKind
    technicalKind =
      tech->
        getTechnicalKind ();

  // don't add the same technical several times
  for (
    list<S_msrTechnical>::const_iterator i =
      fChordTechnicals.begin();
    i!=fChordTechnicals.end();
    i++) {
      if ((*i)->getTechnicalKind () == technicalKind)
        return;
  } // for

  if (gGeneralOptions->fTraceChords)
    cerr << idtr <<
      "Adding technical '" <<
      tech->technicalAsString () <<
      "' to chord" <<
      endl;

  fChordTechnicals.push_back (tech);
}

void msrChord::addTechnicalWithIntegerToChord (
  S_msrTechnicalWithInteger tech)
{
  msrTechnicalWithInteger::msrTechnicalWithIntegerKind
    technicalWithIntegerKind =
      tech->
        getTechnicalWithIntegerKind ();

  // don't add the same technical several times
  for (
    list<S_msrTechnicalWithInteger>::const_iterator i =
      fChordTechnicalWithIntegers.begin();
    i!=fChordTechnicalWithIntegers.end();
    i++) {
      if ((*i)->getTechnicalWithIntegerKind () == technicalWithIntegerKind)
        return;
  } // for

  if (gGeneralOptions->fTraceChords)
    cerr << idtr <<
      "Adding technical with integer '" <<
      tech->technicalWithIntegerAsString () <<
      "' to chord" <<
      endl;

  fChordTechnicalWithIntegers.push_back (tech);
}

void msrChord::addTechnicalWithStringToChord (
  S_msrTechnicalWithString tech)
{
  msrTechnicalWithString::msrTechnicalWithStringKind
    technicalWithStringKind =
      tech->
        getTechnicalWithStringKind ();

  // don't add the same technical several times
  for (
    list<S_msrTechnicalWithString>::const_iterator i =
      fChordTechnicalWithStrings.begin();
    i!=fChordTechnicalWithStrings.end();
    i++) {
      if ((*i)->getTechnicalWithStringKind () == technicalWithStringKind)
        return;
  } // for

  if (gGeneralOptions->fTraceChords)
    cerr << idtr <<
      "Adding technical with integer '" <<
      tech->technicalWithStringAsString () <<
      "' to chord" <<
      endl;

  fChordTechnicalWithStrings.push_back (tech);
}

void msrChord::addOrnamentToChord (S_msrOrnament orn)
{
  msrOrnament::msrOrnamentKind
    ornamentKind =
      orn->
        getOrnamentKind ();

  // don't add the same ornament several times
  for (
    list<S_msrOrnament>::const_iterator i = fChordOrnaments.begin();
    i!=fChordOrnaments.end();
    i++) {
      if ((*i)->getOrnamentKind () == ornamentKind)
        return;
  } // for

  if (gGeneralOptions->fTraceChords)
    cerr << idtr <<
      "Adding ornament '" <<
      orn->ornamentKindAsString () <<
      "' to chord" <<
      endl;

  fChordOrnaments.push_back (orn);
}

void msrChord::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrChord::acceptIn()" <<
      endl;
      
  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrChord::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrChord::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrChord::acceptOut()" <<
      endl;

  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrChord::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrChord::browseData (basevisitor* v)
{
  for (
    vector<S_msrNote>::const_iterator i = fChordNotes.begin();
    i != fChordNotes.end();
    i++ ) {
    // browse chord member note
    msrBrowser<msrNote> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrArticulation>::const_iterator i = fChordArticulations.begin();
    i != fChordArticulations.end();
    i++ ) {
    // browse the articulation
    msrBrowser<msrArticulation> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrTechnical>::const_iterator i = fChordTechnicals.begin();
    i != fChordTechnicals.end();
    i++ ) {
    // browse the technical
    msrBrowser<msrTechnical> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrOrnament>::const_iterator i = fChordOrnaments.begin();
    i != fChordOrnaments.end();
    i++ ) {
    // browse the ornament
    msrBrowser<msrOrnament> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrDynamics>::const_iterator i = fChordDynamics.begin();
    i != fChordDynamics.end();
    i++ ) {
    // browse the dynamics
    msrBrowser<msrDynamics> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrOtherDynamics>::const_iterator i = fChordOtherDynamics.begin();
    i != fChordOtherDynamics.end();
    i++ ) {
    // browse the other dynamics
    msrBrowser<msrOtherDynamics> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrWords>::const_iterator i = fChordWords.begin();
    i != fChordWords.end();
    i++ ) {
    // browse the words
    msrBrowser<msrWords> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrSlur>::const_iterator i = fChordSlurs.begin();
    i != fChordSlurs.end();
    i++ ) {
    // browse the slur
    msrBrowser<msrSlur> browser (v);
    browser.browse (*(*i));
  } // for
  
  for (
    list<S_msrLigature>::const_iterator i = fChordLigatures.begin();
    i != fChordLigatures.end();
    i++ ) {
    // browse the ligature
    msrBrowser<msrLigature> browser (v);
    browser.browse (*(*i));
  } // for
  
  for (
    list<S_msrWedge>::const_iterator i = fChordWedges.begin();
    i != fChordWedges.end();
    i++ ) {
    // browse the wedge
    msrBrowser<msrWedge> browser (v);
    browser.browse (*(*i));
  } // for

  if (fChordHarmony) {
    // browse the harmony
    msrBrowser<msrHarmony> browser (v);
    browser.browse (*fChordHarmony);
  }
}

ostream& operator<< (ostream& os, const S_msrChord& elt)
{
  elt->print (os);
  return os;
}

string msrChord::chordSoundingDivisionsAsMsrString () const
{
  string result;

/* JMI
  int inputSourceSuppliedNumberOfDots =
    fChordNotes [0]-> 
      getNoteDotsNumber (); // any chord member note is fine
  */
  
  result =
    fChordDirectPartUplink->
      divisionsAsMsrString (
        fInputLineNumber,
        fChordSoundingDivisions);

  return result;
}

string msrChord::chordDisplayedDivisionsAsMsrString () const
{
  string result;

/* JMI
  int inputSourceSuppliedNumberOfDots =
    fChordNotes [0]-> 
      getNoteDotsNumber (); // any chord member note is fine
  */
  
  result =
    fChordDirectPartUplink->
      divisionsAsMsrString (
        fInputLineNumber,
        fChordDisplayedDivisions);

  return result;
}

/* JMI
void msrChord::applyTupletMemberDisplayFactorToChordMembers (
  int actualNotes, int normalNotes)
{
  if (gGeneralOptions->fTraceTuplets || gGeneralOptions->fTraceChords)
    cerr << idtr <<
      "Applying tuplet member display factor '" <<
      actualNotes << "/" << normalNotes <<
      "' to the members of chord '" << chordAsStringwithRawDivisions () <<
      "', line " << fInputLineNumber <<
      endl;

  for (
    vector<S_msrNote>::const_iterator i = fChordNotes.begin();
    i != fChordNotes.end();
    ++i) {
    (*i)->
      applyTupletMemberDisplayFactorToNote (
        actualNotes, normalNotes);
  } // for
}
*/

string msrChord::chordAsStringwithRawDivisions () const
{
  stringstream s;

  s << "<";

  if (fChordNotes.size ()) {
    vector<S_msrNote>::const_iterator
      iBegin = fChordNotes.begin(),
      iEnd   = fChordNotes.end(),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote
        note = (*i);
        
      s <<
      /* JMI
        note->notePitchAsString () <<
        note->noteSoundingDivisionsAsMsrString () <<
        "[" << note->getNoteOctave () << "]"
        */

        note->noteAsShortStringWithRawDivisions ();
        
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  s << ">";
  
  return s.str();
}

string msrChord::chordAsString () const
{
  stringstream s;

  s << "<";

  if (fChordNotes.size ()) {
    vector<S_msrNote>::const_iterator
      iBegin = fChordNotes.begin(),
      iEnd   = fChordNotes.end(),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote
        note = (*i);
        
      s <<
        note->notePitchAsString () <<
        ", divs: " <<
        note->getNoteSoundingDivisions () <<
        " sound, " <<
        note->getNoteDisplayedDivisions () <<
        " disp," <<
        "[" << note->getNoteOctave () << "]";
        
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  s << ">";
  
  return s.str();
}

void msrChord::print (ostream& os)
{
  int divisionsPerFullMeasure =
    fChordMeasureUplink
      ? 
        fChordMeasureUplink->
          getMeasureDivisionsPerFullMeasure ()
      : 0; // JMI
    
  os <<
    "Chord:" <<
    chordSoundingDivisionsAsMsrString () <<
    ", " <<
    singularOrPlural (
      fChordNotes.size (), "note", "notes") <<
    ", divs: " <<
    fChordSoundingDivisions << " sounddivs" <<
    ", " <<
    fChordDisplayedDivisions << " dispdivs" <<
    ", meas "<<
    getChordMeasureNumber () <<
    ":" <<
    fChordPositionInMeasure <<
    "/" <<
    divisionsPerFullMeasure;

  // print simplified position in measure if relevant
// JMI  if (fChordMeasureUplink) {
    // the chord measure uplink may not have been set yet
    rational
      chordPosition (
        fChordPositionInMeasure,
        divisionsPerFullMeasure); // JMI
    chordPosition.rationalise ();
    
    if (chordPosition.getNumerator () != fChordPositionInMeasure)
      // print simplified rational view
      os <<
        " (" <<
        chordPosition.getNumerator () <<
        "/" <<
        chordPosition.getDenominator() <<
        ")";
 // JMI }

  if (fChordIsFirstChordInADoubleTremolo)
    os <<
      ", chord is first chord in a double tremolo";
      
  if (fChordIsSecondChordInADoubleTremolo)
    os <<
      ", chord is second chord in a double tremolo";
      
  os <<
    endl;
    
  idtr++;
  
  // print the member notes if any
  if (fChordNotes.size()) {
    vector<S_msrNote>::const_iterator i;
    for (i=fChordNotes.begin(); i!=fChordNotes.end(); i++) {
      os << idtr << (*i);
    } // for

/* JMI   vector<S_msrNote>::const_iterator
      iBegin = fChordNotes.begin(),
      iEnd   = fChordNotes.end(),
      i      = iBegin;
    for ( ; ; ) {
   //   os << (*i)->notePitchAsString (); JMI
      os << idtr << (*i);
      if (++i == iEnd) break;
 //     os << endl;
    } // for
    */
  }
  
  // print the articulations if any
  if (fChordArticulations.size()) {
    list<S_msrArticulation>::const_iterator i;
    for (i=fChordArticulations.begin(); i!=fChordArticulations.end(); i++) {
      os << idtr << (*i);
    } // for
  }

  // print the technicals if any
  if (fChordTechnicals.size()) {
    list<S_msrTechnical>::const_iterator i;
    for (i=fChordTechnicals.begin(); i!=fChordTechnicals.end(); i++) {
      os << idtr << (*i);
    } // for
  }

  // print the ornaments if any
  if (fChordOrnaments.size()) {
    list<S_msrOrnament>::const_iterator i;
    for (i=fChordOrnaments.begin(); i!=fChordOrnaments.end(); i++) {
      os << idtr << (*i);
    } // for
  }

  // print the dynamics if any
  if (fChordDynamics.size()) {
    list<S_msrDynamics>::const_iterator i;
    for (i=fChordDynamics.begin(); i!=fChordDynamics.end(); i++) {
      os << idtr << (*i);
    } // for
  }

  // print the other dynamics if any
  if (fChordOtherDynamics.size()) {
    list<S_msrOtherDynamics>::const_iterator i;
    for (i=fChordOtherDynamics.begin(); i!=fChordOtherDynamics.end(); i++) {
      os << idtr << (*i);
    } // for
  }

  // print the words if any
  if (fChordWords.size()) {
    list<S_msrWords>::const_iterator i;
    for (i=fChordWords.begin(); i!=fChordWords.end(); i++) {
      os << idtr << (*i);
    } // for
  }

  // print the slurs if any
  if (fChordSlurs.size()) {
    list<S_msrSlur>::const_iterator i;
    for (i=fChordSlurs.begin(); i!=fChordSlurs.end(); i++) {
      os << idtr << (*i);
    } // for
  }

  // print the ligatures if any
  if (fChordLigatures.size()) {
    list<S_msrLigature>::const_iterator i;
    for (i=fChordLigatures.begin(); i!=fChordLigatures.end(); i++) {
      os << idtr << (*i);
    } // for
  }

  // print the wedges if any
  if (fChordWedges.size()) {
    list<S_msrWedge>::const_iterator i;
    for (i=fChordWedges.begin(); i!=fChordWedges.end(); i++) {
      os << idtr << (*i);
    } // for
  }

  // print the harmony if any
  if (fChordHarmony) {
    os << idtr <<
      "Chord harmony: " <<
      endl;
        
    idtr++;

    os << idtr <<
      fChordHarmony->harmonyAsString () <<
      endl;
      
    idtr--;
  }

  idtr--;
}

//______________________________________________________________________________
S_msrComment msrComment::create (
  int               inputLineNumber,
  string            contents,
  msrCommentGapKind commentGapKind)
{
  msrComment* o =
    new msrComment (
      inputLineNumber, contents, commentGapKind);
  assert(o!=0);
  return o;
}

msrComment::msrComment (
  int               inputLineNumber,
  string            contents,
  msrCommentGapKind commentGapKind)
    : msrElement (inputLineNumber)
{
  fContents       = contents;
  fCommentGapKind = commentGapKind;
}
msrComment::~msrComment() {}

void msrComment::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrComment::acceptIn()" <<
      endl;
      
  if (visitor<S_msrComment>*
    p =
      dynamic_cast<visitor<S_msrComment>*> (v)) {
        S_msrComment elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrComment::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrComment::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrComment::acceptOut()" <<
      endl;

  if (visitor<S_msrComment>*
    p =
      dynamic_cast<visitor<S_msrComment>*> (v)) {
        S_msrComment elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrComment::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrComment::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrComment& elt)
{
  elt->print (os);
  return os;
}

void msrComment::print (ostream& os)
{
  os << "Comment" << endl;
  
  idtr++;
  
  os << idtr <<
    "% " << fContents <<
    endl;
    
  if (fCommentGapKind == kGapAfterwards)
    os <<
      endl;
      
  idtr--;
}

//______________________________________________________________________________
S_msrBreak msrBreak::create (
  int    inputLineNumber,
  string nextBarNumber)
{
  msrBreak* o =
    new msrBreak (
      inputLineNumber, nextBarNumber);
  assert(o!=0);
  return o;
}

msrBreak::msrBreak (
  int    inputLineNumber,
  string nextBarNumber)
    : msrElement (inputLineNumber)
{
  fNextBarNumber = nextBarNumber; 

  if (gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Creating a break before measure " << fNextBarNumber <<
      endl;
}

msrBreak::~msrBreak() {}

string msrBreak::breakAsString () const
{
  stringstream s;

  s <<
    "Break" << ", next bar number = " << fNextBarNumber;

  return s.str();
}

void msrBreak::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrBreak::acceptIn()" <<
      endl;
      
  if (visitor<S_msrBreak>*
    p =
      dynamic_cast<visitor<S_msrBreak>*> (v)) {
        S_msrBreak elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrBreak::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrBreak::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrBreak::acceptOut()" <<
      endl;

  if (visitor<S_msrBreak>*
    p =
      dynamic_cast<visitor<S_msrBreak>*> (v)) {
        S_msrBreak elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrBreak::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrBreak::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrBreak& elt)
{
  elt->print (os);
  return os;
}

void msrBreak::print (ostream& os)
{
  os <<
    breakAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrBarCheck msrBarCheck::create (
  int           inputLineNumber,
  int           nextBarNumber)
{
  msrBarCheck* o =
    new msrBarCheck (
      inputLineNumber, nextBarNumber);
  assert(o!=0);
  return o;
}

msrBarCheck::msrBarCheck (
  int           inputLineNumber,
  int           nextBarNumber)
    : msrElement (inputLineNumber)
{
  fNextBarNumber = nextBarNumber; 

  if (gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Creating a bar check before measure " << fNextBarNumber <<
      endl;
}
msrBarCheck::~msrBarCheck() {}

string msrBarCheck::barCheckAsString () const
{
  stringstream s;

  s <<
    "BarCheck" << ", next bar number = " << fNextBarNumber;

  return s.str();
}

void msrBarCheck::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrBarCheck::acceptIn()" <<
      endl;
      
  if (visitor<S_msrBarCheck>*
    p =
      dynamic_cast<visitor<S_msrBarCheck>*> (v)) {
        S_msrBarCheck elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrBarCheck::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrBarCheck::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrBarCheck::acceptOut()" <<
      endl;

  if (visitor<S_msrBarCheck>*
    p =
      dynamic_cast<visitor<S_msrBarCheck>*> (v)) {
        S_msrBarCheck elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrBarCheck::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrBarCheck::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrBarCheck& elt)
{
  elt->print (os);
  return os;
}

void msrBarCheck::print (ostream& os)
{
  os <<
    barCheckAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrBarNumberCheck msrBarNumberCheck::create (
  int    inputLineNumber,
  string nextBarNumber)
{
  msrBarNumberCheck* o =
    new msrBarNumberCheck (
      inputLineNumber, nextBarNumber);
  assert(o!=0);
  return o;
}

msrBarNumberCheck::msrBarNumberCheck (
  int    inputLineNumber,
  string nextBarNumber)
    : msrElement (inputLineNumber)
{
  fNextBarNumber=nextBarNumber; 
}
msrBarNumberCheck::~msrBarNumberCheck() {}

string msrBarNumberCheck::barNumberCheckAsString () const
{
  stringstream s;

  s <<
    "BarNumberCheck" << ", next bar number = " << fNextBarNumber;

  return s.str();
}

void msrBarNumberCheck::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrBarNumberCheck::acceptIn()" <<
      endl;
      
  if (visitor<S_msrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_msrBarNumberCheck>*> (v)) {
        S_msrBarNumberCheck elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrBarNumberCheck::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrBarNumberCheck::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrBarNumberCheck::acceptOut()" <<
      endl;

  if (visitor<S_msrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_msrBarNumberCheck>*> (v)) {
        S_msrBarNumberCheck elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrBarNumberCheck::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrBarNumberCheck::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrBarNumberCheck& elt)
{
  elt->print (os);
  return os;
}

void msrBarNumberCheck::print (ostream& os)
{
  os <<
    barNumberCheckAsString ();
}

//______________________________________________________________________________
S_msrTuplet msrTuplet::create (
  int inputLineNumber,
  int number,
  int actualNotes,
  int normalNotes,
  int notePositionInMeasure)
{
  msrTuplet* o =
    new msrTuplet (
      inputLineNumber,
      number, actualNotes, normalNotes, notePositionInMeasure);
  assert(o!=0);
  return o;
}

msrTuplet::msrTuplet (
  int inputLineNumber,
  int number,
  int actualNotes,
  int normalNotes,
  int notePositionInMeasure)
    : msrElement (inputLineNumber)
{  
  fTupletNumber = number;
  
  fTupletActualNotes = actualNotes;
  fTupletNormalNotes = normalNotes;
  
  fTupletSoundingDivisions  = 0;
  fTupletDisplayedDivisions = 0;

  fTupletPositionInMeasure = notePositionInMeasure;
}

msrTuplet::~msrTuplet() {}

S_msrTuplet msrTuplet::createTupletShallowClone ()
{
  if (gGeneralOptions->fTraceTuplets) {
    cerr << idtr <<
      "Creating a shallow clone of tuplet '" <<
      tupletAsShortString () <<
      "'" <<
      endl;
  }

  S_msrTuplet
    clone =
      msrTuplet::create (
        fInputLineNumber,
        fTupletNumber,
        fTupletActualNotes,
        fTupletNormalNotes,
        fTupletPositionInMeasure);

  clone->fTupletSoundingDivisions =
    fTupletSoundingDivisions;

  clone->fTupletDisplayedDivisions =
    fTupletDisplayedDivisions;

  clone->fTupletMeasureNumber =
    fTupletMeasureNumber;

  return clone;
}

void msrTuplet::addNoteToTuplet (S_msrNote note)
{
  if (gGeneralOptions->fTraceTuplets)
    cerr << idtr <<
      "Adding note '" <<
      note->noteAsShortStringWithRawDivisions () <<
      // the information is missing to display it the normal way
      "' to tuplet '" <<
      tupletAsShortString () <<
      "'" <<
      endl;

  fTupletElements.push_back (note);

  // register note's tuplet uplink
  note->
    setNoteTupletUplink (this);

  // account for note duration
  fTupletSoundingDivisions +=
    note->getNoteSoundingDivisions ();
  fTupletDisplayedDivisions += // JMI
    note->getNoteDisplayedDivisions ();  
    
  // populate note's measure number
  note->setNoteMeasureNumber (
    fTupletMeasureNumber);

  // populate note's position in measure
  note->setNotePositionInMeasure (
    fTupletPositionInMeasure);
}

void msrTuplet::addChordToTuplet (S_msrChord chord)
{
  if (gGeneralOptions->fTraceChords || gGeneralOptions->fTraceTuplets)
    cerr << idtr <<
      "Adding chord '" <<
      chord->chordAsString () <<
      "' to tuplet '" <<
      tupletAsString () <<
      "'" <<
      endl;

  fTupletElements.push_back (chord);
    
  // account for chord duration
  fTupletSoundingDivisions +=
    chord->getChordSoundingDivisions ();
  fTupletDisplayedDivisions += // JMI
    chord->getChordDisplayedDivisions ();  
    
  // populate chord's measure number
  chord->setChordMeasureNumber (
    fTupletMeasureNumber);

  // populate chord's position in measure
  chord->setChordPositionInMeasure (
    fTupletPositionInMeasure);
}

void msrTuplet::addTupletToTuplet (S_msrTuplet tuplet)
{
  if (gGeneralOptions->fTraceTuplets)
    cerr << idtr <<
      "Adding tuplet '" <<
      tuplet->tupletAsString () <<
      "' to tuplet '" <<
      tupletAsString () <<
      "'" <<
      endl;

/* JMI
  // unapply containing tuplet factor,
  // i.e 3/2 inside 5/4 becomes 15/8 in MusicXML...
  tuplet->
    unapplyDisplayFactorToTupletMembers (
      this->fTupletActualNotes,
      this->fTupletNormalNotes);
  */
  
  // register tuplet in elements list
  fTupletElements.push_back (tuplet);
    
  // account for tuplet duration
  fTupletSoundingDivisions +=
    tuplet->getTupletSoundingDivisions ();
  fTupletDisplayedDivisions += // JMI
    tuplet->getTupletDisplayedDivisions ();
    
    /*
  fTupletDisplayedDivisions += // JMI
    tuplet->getTupletDisplayedDivisions ();  
    */

  // don't populate tuplet's measure number nor position in measure,
  // this will be done in setTupletMeasureNumber()
  /* JMI
  tuplet->setTupletMeasureNumber (
    fTupletMeasureNumber);

  // populate tuplet's position in measure
  tuplet->setTupletPositionInMeasure (
    fTupletPositionInMeasure);
    */
}

void msrTuplet::addTupletToTupletClone (S_msrTuplet tuplet)
{
  if (gGeneralOptions->fTraceTuplets)
    cerr << idtr <<
      "Adding tuplet '" <<
      tuplet->tupletAsString () <<
      "' to tuplet '" <<
      tupletAsString () <<
      "'" <<
      endl;

  // dont' unapply containing tuplet factor,
  // this has been done when building the MSR from MusicXML
  
  // register tuplet in elements list
  fTupletElements.push_back (tuplet);
    
  // account for tuplet duration
  fTupletSoundingDivisions +=
    tuplet->getTupletSoundingDivisions ();
  fTupletDisplayedDivisions +=
    tuplet->getTupletDisplayedDivisions ();
}

void msrTuplet::removeFirstNoteFromTuplet (
  int       inputLineNumber,
  S_msrNote note)
{
  if (gGeneralOptions->fTraceTuplets)
    cerr << idtr <<
      "Removing first note '" <<
      note->noteAsShortStringWithRawDivisions () <<
      "' from tuplet '" <<
      tupletAsString () <<
      "'" <<
      endl;

  if (fTupletElements.size ()) {
    S_msrElement
      firstTupletElement =
        fTupletElements.front ();

    for (
      list<S_msrElement>::const_iterator i=fTupletElements.begin();
      i!=fTupletElements.end();
      ++i) {
      if ((*i) == note) {
        // found note, erase it
        fTupletElements.erase (i);
        
        // account for note duration
        fTupletSoundingDivisions -=
          note->getNoteSoundingDivisions ();
        fTupletDisplayedDivisions -= // JMI
          note->getNoteDisplayedDivisions ();  

        // don't update measure number nor position in measure: // JMI
        // they have not been set yet
  
        // return from function
        return;
      }
    } // for
    
    msrInternalError (
      inputLineNumber,
      "cannot remove note " <<
      note <<
      " from tuplet " << tupletAsString () <<
      "' in voice \"" <<
      fTupletMeasureUplink->
        getMeasureSegmentUplink ()->
          getSegmentVoiceUplink ()->
            getVoiceName () <<
      "\"," <<
      " since it has not been found");
  }
  
  else {
    msrInternalError (
      inputLineNumber,
      "cannot remove note " <<
      note <<
      " from empty tuplet " <<
      "' in voice \"" <<
      fTupletMeasureUplink->
        getMeasureSegmentUplink ()->
          getSegmentVoiceUplink ()->
            getVoiceName () <<
      "\"," <<
      " since it has not been found");
  }
}

void msrTuplet::setTupletMeasureNumber (string measureNumber) // JMI
{
  fTupletMeasureNumber = measureNumber;

  // propagate measure number to the tuplets elements
  for (
    list<S_msrElement>::const_iterator i = fTupletElements.begin();
    i != fTupletElements.end();
    i++ ) {
    // set tuplet element measure number

  //  SMARTP<msrNote>* note = dynamic_cast<SMARTP<msrNote>*>(&(*tupletMember));

  // BON   SMARTP<msrNote> note = dynamic_cast<msrNote*>(&(*tupletMember));

    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(**i))
      ) {    
      note->setNoteMeasureNumber (measureNumber);
    }
  
    else if (
      S_msrChord chord = dynamic_cast<msrChord*>(&(**i))
      ) {
      chord->setChordMeasureNumber (measureNumber);
    }
    
    else if (
      S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(**i))
      ) {
      tuplet->setTupletMeasureNumber (measureNumber);
    }
    
    else {
      msrInternalError (
        fInputLineNumber,
        "tuplet member should be a note, a chord or another tuplet");
    }
  } // for
}

int msrTuplet::setTupletPositionInMeasure (int position)
  // returns the position after the tuplet
{
  fTupletPositionInMeasure = position;

  int currentPosition = position;
  
  // compute position in measure for the tuplets elements
  for (
    list<S_msrElement>::const_iterator i = fTupletElements.begin();
    i != fTupletElements.end();
    i++ ) {
    // set tuplet element position in measure
    
    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(**i))
      ) {    
      note->
        setNotePositionInMeasure (currentPosition);
        
      currentPosition +=
        note->getNoteSoundingDivisions ();
    }
  
    else if (
      S_msrChord chord = dynamic_cast<msrChord*>(&(**i))
      ) {
      chord->
        setChordPositionInMeasure (currentPosition);
        
      currentPosition +=
        chord->getChordSoundingDivisions ();
    }
    
    else if (
      S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(**i))
      ) {
      currentPosition =
        tuplet->
          setTupletPositionInMeasure (currentPosition);
    }
    
    else {
      msrInternalError (
        fInputLineNumber,
        "tuplet member should be a note, a chord or another tuplet");
    }

  } // for

  return currentPosition;
}

/* JMI
void msrTuplet::applyDisplayFactorToTupletMembers ()
{
  if (gGeneralOptions->fTraceTuplets) {
    cerr <<
      idtr <<
        "% ==> applyDisplayFactorToTupletMembers()" <<
        endl;

    idtr++;
    
    cerr <<
      idtr <<
        "% fTupletActualNotes = " <<
        fTupletActualNotes <<
        ", fTupletNormalNotes = " <<
        fTupletNormalNotes <<
        endl <<
      endl;

    idtr--;
  }

  for (
    list<S_msrElement>::const_iterator i = fTupletElements.begin();
    i != fTupletElements.end();
    i++ ) {
    // apply display factor to tuplet element
    
    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(**i))
      ) {    
      note->
        applyTupletMemberDisplayFactorToNote (
          fTupletActualNotes, fTupletNormalNotes);
     }
  
    else if (
      S_msrChord chord = dynamic_cast<msrChord*>(&(**i))
      ) {
      chord->
        applyTupletMemberDisplayFactorToChordMembers (
          fTupletActualNotes, fTupletNormalNotes);
    }
    
    else if (
      S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(**i))
      ) {
      // don't apply the display factor to nested tuplets
 // JMI     tuplet->
       // applyDisplayFactorToTupletMembers ();
    }
    
    else {
      msrInternalError (
        fInputLineNumber,
        "tuplet member should be a note, a chord or another tuplet");
    }

  } // for
}
*/

void msrTuplet::unapplyDisplayFactorToTupletMembers (
  int containingTupletActualNotes,
  int containingTupletNormalNotes)
{
  if (gGeneralOptions->fTraceTuplets) {
    cerr <<
      idtr <<
        "UnapplyDisplayFactorToTupletMembers()" <<
        endl;

    idtr++;
    
    cerr <<
      idtr <<
        "% fTupletActualNotes = " <<
        fTupletActualNotes <<
        ", fTupletNormalNotes = " <<
        fTupletNormalNotes <<
        endl <<
      idtr <<
        "% containingTupletActualNotes = " <<
        containingTupletActualNotes <<
        ", containingTupletNormalNotes = " <<
        containingTupletNormalNotes <<
        endl <<
      endl;

    idtr--;
  }

  fTupletActualNotes /=
    containingTupletActualNotes;
  fTupletNormalNotes /=
    containingTupletNormalNotes;
}

void msrTuplet::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrTuplet::acceptIn()" <<
      endl;
      
  if (visitor<S_msrTuplet>*
    p =
      dynamic_cast<visitor<S_msrTuplet>*> (v)) {
        S_msrTuplet elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrTuplet::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrTuplet::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrTuplet::acceptOut()" <<
      endl;

  if (visitor<S_msrTuplet>*
    p =
      dynamic_cast<visitor<S_msrTuplet>*> (v)) {
        S_msrTuplet elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrTuplet::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrTuplet::browseData (basevisitor* v)
{
  for (
    list<S_msrElement>::const_iterator i = fTupletElements.begin();
    i != fTupletElements.end();
    i++ ) {
    // browse tuplet element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for
}

ostream& operator<< (ostream& os, const S_msrTuplet& elt)
{
  elt->print (os);
  return os;
}

string msrTuplet::tupletAsShortString () const
{
  stringstream s;

  s <<
    "Tuplet " <<
    fTupletActualNotes << "/" << fTupletNormalNotes <<
    " " << fTupletSoundingDivisions << " sounddivs" <<
    " @meas "<<
    fTupletMeasureNumber <<
    ":";

  if (fTupletPositionInMeasure < 0)
    s << "?";
  else
    s << fTupletPositionInMeasure;
  
  s << "[[";

  if (fTupletElements.size ()) {
    list<S_msrElement>::const_iterator
      iBegin = fTupletElements.begin(),
      iEnd   = fTupletElements.end(),
      i      = iBegin;
    for ( ; ; ) {
      
      if (
        S_msrNote note = dynamic_cast<msrNote*>(&(**i))
        ) {    
        s <<
          note->noteAsShortStringWithRawDivisions ();
      }
    
      else if (
        S_msrChord chord = dynamic_cast<msrChord*>(&(**i))
        ) {
        s <<
          chord->chordAsString ();
      }
      
      else if (
        S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(**i))
        ) {
        s <<
          tuplet->tupletAsString ();
      }
      
      else {
        msrInternalError (
          fInputLineNumber,
          "tuplet member should be a note, a chord or another tuplet");
      }
  
      if (++i == iEnd) break;
      s << " ";
      
    } // for
  }

  s << "]]";
  
  return s.str();
}

string msrTuplet::tupletAsString () const
{
  stringstream s;

  s <<
    "Tuplet " <<
    fTupletActualNotes << "/" << fTupletNormalNotes <<
    " " << fTupletSoundingDivisions << " sounddivs" <<
    " @meas "<<
    fTupletMeasureNumber <<
    ":";

  if (fTupletPositionInMeasure < 0)
    s << "?";
  else
    s << fTupletPositionInMeasure;
  
  s << "[[";

  if (fTupletElements.size ()) {
    list<S_msrElement>::const_iterator
      iBegin = fTupletElements.begin(),
      iEnd   = fTupletElements.end(),
      i      = iBegin;
    for ( ; ; ) {

      s << idtr;
      
      if (
        S_msrNote note = dynamic_cast<msrNote*>(&(**i))
        ) {    
        s <<
          note->noteAsShortStringWithRawDivisions ();
      }
    
      else if (
        S_msrChord chord = dynamic_cast<msrChord*>(&(**i))
        ) {
        s <<
          chord->chordAsString ();
      }
      
      else if (
        S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(**i))
        ) {
        s <<
          tuplet->tupletAsString ();
      }
      
      else {
        msrInternalError (
          fInputLineNumber,
          "tuplet member should be a note, a chord or another tuplet");
      }
  
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  s << "]]";
  
  return s.str();
}

void msrTuplet::print (ostream& os)
{
  os <<
    "Tuplet " <<
    fTupletActualNotes << "/" << fTupletNormalNotes <<
    ", " <<
    singularOrPlural (
      fTupletElements.size (), "element", "elements") <<
    ", divs: " <<
    fTupletSoundingDivisions << " sound, " <<
    fTupletDisplayedDivisions << " disp" <<
    ", meas "<<
    fTupletMeasureNumber <<
    ":";
    
  if (fTupletPositionInMeasure < 0)
    os << "?";
  else
    os << fTupletPositionInMeasure;
  os <<
    endl;
  
  if (fTupletElements.size ()) {
    idtr++;

    list<S_msrElement>::const_iterator
      iBegin = fTupletElements.begin(),
      iEnd   = fTupletElements.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for
    
    idtr--;
    
  // JMI  os << endl;
  }
}

//______________________________________________________________________________
S_msrPageGeometry msrPageGeometry::create (
  int           inputLineNumber)
{
  msrPageGeometry* o =
    new msrPageGeometry (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrPageGeometry::msrPageGeometry (
  int           inputLineNumber)
    : msrElement (inputLineNumber)
{
  fPaperWidth   = -1.0;
  fPaperHeight  = -1.0;
  fTopMargin    = -1.0;
  fBottomMargin = -1.0;
  fLeftMargin   = -1.0;
  fRightMargin  = -1.0;
    
  fBetweenSystemSpace = -1.0;
  fPageTopSpace       = -1.0;

  fMillimeters = -1;
  fTenths      = -1;
}

msrPageGeometry::~msrPageGeometry() {}

float msrPageGeometry::globalStaffSize () const
{
  if (fMillimeters > 0)
    return fMillimeters * 72.27 / 25.4;
  else
    return 20.0; // LilyPond default
}

void msrPageGeometry::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrPageGeometry::acceptIn()" <<
      endl;
      
  if (visitor<S_msrPageGeometry>*
    p =
      dynamic_cast<visitor<S_msrPageGeometry>*> (v)) {
        S_msrPageGeometry elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrPageGeometry::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrPageGeometry::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrPageGeometry::acceptOut()" <<
      endl;

  if (visitor<S_msrPageGeometry>*
    p =
      dynamic_cast<visitor<S_msrPageGeometry>*> (v)) {
        S_msrPageGeometry elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrPageGeometry::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrPageGeometry::browseData (basevisitor* v)
{
  /* JMI
  if (fPaperWidth) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fPaperWidth);
  }

  if (fPaperHeight) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fPaperHeight);
  }

  if (fTopMargin) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fTopMargin);
  }

  if (fBottomMargin) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fPaperWidth);
  }

  if (fLeftMargin) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fPaperWidth);
  }

  if (fRightMargin) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fRightMargin);
  }
  */
}

ostream& operator<< (ostream& os, const S_msrPageGeometry& elt) {
  elt->print (os);
  return os;
}

void msrPageGeometry::print (ostream& os) {
  os <<
    "PageGeometry" <<
    endl;

  bool emptyGeometry = true;
  
  idtr++;
  
    const int fieldWidth = 12;

  if (fPaperWidth > 0) {
    os << left <<
      idtr <<
        setw(fieldWidth) <<
        "PaperWidth" << " = " <<
        setprecision(4) << fPaperWidth << " cm" <<
        endl;
    emptyGeometry = false;
  }
  
  if (fPaperHeight > 0) {
    os << left <<
      idtr <<
        setw(fieldWidth) <<
        "PaperHeight" << " = " <<
        setprecision(4) << fPaperHeight << " cm" <<
        endl;
    emptyGeometry = false;
  }
  
  if (fTopMargin > 0) {
    os << left <<
      idtr <<
        setw(fieldWidth) <<
        "TopMargin" << " = " <<
        setprecision(4) << fTopMargin << " cm" <<
        endl;
    emptyGeometry = false;
  }
  
  if (fBottomMargin > 0) {
    os << left <<
      idtr <<
        setw(fieldWidth) <<
        "BottomMargin" << " = " <<
        setprecision(4) << fBottomMargin << " cm" <<
        endl;
    emptyGeometry = false;
  }
  
  if (fLeftMargin > 0) {
    os << left <<
      idtr <<
        setw(fieldWidth) <<
        "LeftMargin" << " = " <<
        setprecision(4) << fLeftMargin << " cm" <<
        endl;
    emptyGeometry = false;
  }
  
  if (fRightMargin > 0) {
    os << left <<
      idtr <<
        setw(fieldWidth) <<
        "RightMargin" << " = " <<
        setprecision(4) << fRightMargin << " cm" <<
        endl;
    emptyGeometry = false;
  }


  if (fMillimeters > 0) {
    os << left <<
      idtr <<
        setw(fieldWidth) <<
        "Millimeters" << " = " <<
        setprecision(4) << fMillimeters <<
        endl;
    emptyGeometry = false;
  }

  if (fTenths > 0) {
    os << left <<
      idtr <<
        setw(fieldWidth) <<
        "Tenths" << " = " <<
        setprecision(4) << fTenths <<
        endl;
    emptyGeometry = false;
  }

  if (emptyGeometry)
    os << idtr <<
      " " << "nothing specified" <<
      endl;
  
  os <<
    endl;

  idtr--;
}

/*
  if (fBetweenSystemSpace > 0) {
    os << idtr <<
      "between-system-space = " <<
      setprecision(4) << fBetweenSystemSpace << "\\cm" << endl;
  }

  if (fPageTopSpace > 0) {
    os << idtr <<
      "page-top-space = " <<
      setprecision(4) << fPageTopSpace << "\\cm" << endl;
  }
*/

//______________________________________________________________________________
S_msrCreditWords msrCreditWords::create (
  int    inputLineNumber,
  string creditWordsContents,
  string creditWordsFontFamily,
  int    creditWordsFontSize,
  string creditWordsFontWeight,
  string creditWordsFontJustify,
  string creditWordsFontHAlign,
  string creditWordsFontVAlign,
  string creditWordsFontXMLLanguage)
{
  msrCreditWords* o =
    new msrCreditWords (
      inputLineNumber,
      creditWordsContents,
      creditWordsFontFamily,
      creditWordsFontSize,
      creditWordsFontWeight,
      creditWordsFontJustify,
      creditWordsFontHAlign,
      creditWordsFontVAlign,
      creditWordsFontXMLLanguage);
  assert(o!=0);
  return o;
}

msrCreditWords::msrCreditWords (
  int    inputLineNumber,
  string creditWordsContents,
  string creditWordsFontFamily,
  int    creditWordsFontSize,
  string creditWordsFontWeight,
  string creditWordsFontJustify,
  string creditWordsFontHAlign,
  string creditWordsFontVAlign,
  string creditWordsFontXMLLanguage)
    : msrElement (inputLineNumber)
{
  fCreditWordsContents        = creditWordsContents;

  fCreditWordsFontFamily      = creditWordsFontFamily;
  fCreditWordsFontSize        = creditWordsFontSize;
  fCreditWordsFontWeight      = creditWordsFontWeight;
  fCreditWordsFontJustify     = creditWordsFontJustify;
  fCreditWordsFontHAlign      = creditWordsFontHAlign;
  fCreditWordsFontVAlign      = creditWordsFontVAlign;
  fCreditWordsFontXMLLanguage = creditWordsFontXMLLanguage;
}

msrCreditWords::~msrCreditWords() {}

void msrCreditWords::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrCreditWords::acceptIn()" <<
      endl;
      
  if (visitor<S_msrCreditWords>*
    p =
      dynamic_cast<visitor<S_msrCreditWords>*> (v)) {
        S_msrCreditWords elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrCreditWords::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrCreditWords::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrCreditWords::acceptOut()" <<
      endl;

  if (visitor<S_msrCreditWords>*
    p =
      dynamic_cast<visitor<S_msrCreditWords>*> (v)) {
        S_msrCreditWords elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrCreditWords::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrCreditWords::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrCreditWords& elt)
{
  elt->print (os);
  return os;
}

void msrCreditWords::print (ostream& os)
{
  os <<
    "CreditWords" <<
    endl;

  idtr++;

  const int fieldWidth = 26;
  
  os <<
    idtr <<
      setw(fieldWidth) <<
        "fCreditWordsContents" << " = \"" <<
        fCreditWordsContents <<
        "\"" <<
      endl <<
      
    idtr <<
      setw(fieldWidth) <<
        "creditWordsFontFamily" << " = \"" <<
        fCreditWordsFontFamily <<
        "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
        "creditWordsFontSize" << " = " <<
        fCreditWordsFontSize <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
        "creditWordsFontWeight" << " = \"" <<
        fCreditWordsFontWeight <<
        "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
        "creditWordsFontJustify" << " = \"" <<
        fCreditWordsFontJustify <<
        "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
        "creditWordsFontHAlign" << " = \"" <<
        fCreditWordsFontHAlign <<
        "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
        "creditWordsFontVAlign" << " = \"" <<
        fCreditWordsFontVAlign <<
        "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
        "creditWordsFontXMLLanguage" << " = \"" <<
        fCreditWordsFontXMLLanguage <<
        "\"" <<
      endl <<
    endl;

  idtr--;
}

//______________________________________________________________________________
S_msrCredit msrCredit::create (
  int inputLineNumber,
  int creditPageNumber)
{
  msrCredit* o =
    new msrCredit (
      inputLineNumber, creditPageNumber);
  assert(o!=0);
  return o;
}

msrCredit::msrCredit (
  int inputLineNumber,
  int creditPageNumber)
    : msrElement (inputLineNumber)
{
  fCreditPageNumber = creditPageNumber;
}

msrCredit::~msrCredit() {}

void msrCredit::appendCreditWordsToCredit (
  S_msrCreditWords creditWords)
{
  fCreditWordsList.push_back (creditWords);
}

void msrCredit::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrCredit::acceptIn()" <<
      endl;
      
  if (visitor<S_msrCredit>*
    p =
      dynamic_cast<visitor<S_msrCredit>*> (v)) {
        S_msrCredit elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrCredit::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrCredit::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrCredit::acceptOut()" <<
      endl;

  if (visitor<S_msrCredit>*
    p =
      dynamic_cast<visitor<S_msrCredit>*> (v)) {
        S_msrCredit elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrCredit::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrCredit::browseData (basevisitor* v)
{
  int n1 = fCreditWordsList.size();
  
  for (int i = 0; i < n1; i++ ) {
    // browse the credit words
    msrBrowser<msrCreditWords> browser (v);
    browser.browse (*fCreditWordsList [i]);
  } // for
}

ostream& operator<< (ostream& os, const S_msrCredit& elt)
{
  elt->print (os);
  return os;
}

void msrCredit::print (ostream& os)
{
  os <<
    "Credit" <<
    endl;

  idtr++;
  
  os <<
    idtr <<
      "fCreditPageNumber" << " = " << fCreditPageNumber <<
      endl <<
    endl;

  vector<S_msrCreditWords>::const_iterator
    iBegin = fCreditWordsList.begin(),
    iEnd   = fCreditWordsList.end(),
    i      = iBegin;

  for ( ; ; ) {
    os << idtr << (*i);
    if (++i == iEnd) break;
// JMI    os << endl;
  } // for
  
  idtr--;
}

//______________________________________________________________________________

S_msrVarValAssoc msrVarValAssoc::create (
  int           inputLineNumber,
  string        variableName,
  string        value)
{
  msrVarValAssoc* o =
    new msrVarValAssoc(
      inputLineNumber,
      variableName, value);
  assert(o!=0);
  return o;
}

msrVarValAssoc::msrVarValAssoc (
  int           inputLineNumber,
  string        variableName,
  string        value)
    : msrElement (inputLineNumber)
{
  fVariableName    = variableName;
  fVariableValue   = value;
}

msrVarValAssoc::~msrVarValAssoc() {}

void msrVarValAssoc::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrVarValAssoc::acceptIn()" <<
      endl;
      
  if (visitor<S_msrVarValAssoc>*
    p =
      dynamic_cast<visitor<S_msrVarValAssoc>*> (v)) {
        S_msrVarValAssoc elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrVarValAssoc::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrVarValAssoc::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrVarValAssoc::acceptOut()" <<
      endl;

  if (visitor<S_msrVarValAssoc>*
    p =
      dynamic_cast<visitor<S_msrVarValAssoc>*> (v)) {
        S_msrVarValAssoc elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrVarValAssoc::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}


void msrVarValAssoc::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrVarValAssoc& elt) {
  elt->print (os);
  return os;
}

void msrVarValAssoc::print (ostream& os)
{
  os <<
    "MSR VarValAssoc" <<
    endl;
  
  idtr++;

  // escape quotes if any
  string variableName;

  for_each (
    fVariableName.begin(),
    fVariableName.end(),
    stringQuoteEscaper (variableName));
    
  string variableValue;

  for_each (
    fVariableValue.begin(),
    fVariableValue.end(),
    stringQuoteEscaper (variableValue));

  // print resulting strings
  os <<
    idtr << "variable name : " <<
    "\"" << variableName << "\"" << endl <<
    idtr << "variable value: " <<
    "\"" << variableValue << "\"" << endl <<
    endl;
  
  idtr--;
}

//______________________________________________________________________________
S_msrLayout msrLayout::create (
  int           inputLineNumber)
{
  msrLayout* o =
    new msrLayout (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrLayout::msrLayout (
  int           inputLineNumber)
    : msrElement (inputLineNumber)
{}
msrLayout::~msrLayout() {}

void msrLayout::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrLayout::acceptIn()" <<
      endl;
      
  if (visitor<S_msrLayout>*
    p =
      dynamic_cast<visitor<S_msrLayout>*> (v)) {
        S_msrLayout elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrLayout::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrLayout::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrLayout::acceptOut()" <<
      endl;

  if (visitor<S_msrLayout>*
    p =
      dynamic_cast<visitor<S_msrLayout>*> (v)) {
        S_msrLayout elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrLayout::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrLayout::browseData (basevisitor* v)
{
  int n1 = fVarValAssocs.size();
  
  for (int i = 0; i < n1; i++ ) {
    // browse the stanza
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fVarValAssocs [i]);
  } // for
}

ostream& operator<< (ostream& os, const S_msrLayout& elt)
{
  elt->print (os);
  return os;
}

void msrLayout::print (ostream& os)
{
  os << "Layout" << endl;

  idtr++;

  int n1 = fVarValAssocs.size();
  
  for (int i = 0; i < n1; i++ ) {
    os << idtr << fVarValAssocs [i];
  } // for

    /* JMI
  int n2 = fMsrSchemeVarValAssocs.size();
  for (int i = 0; i < n2; i++ ) {
    os << idtr << fMsrSchemeVarValAssocs[i];
  } // for
  */
  
  idtr--;
}

//______________________________________________________________________________
S_msrClef msrClef::create (
  int          inputLineNumber,
  msrClefKind  clefKind)
{
  msrClef* o =
    new msrClef (
      inputLineNumber, clefKind);
  assert(o!=0);
  return o;
}

msrClef::msrClef (
  int          inputLineNumber,
  msrClefKind  clefKind)
    : msrElement (inputLineNumber)
{
  fClefKind = clefKind;
}

msrClef::~msrClef() {}

bool msrClef::clefIsATablatureClef () const
{
  switch (fClefKind) {
    case msrClef::kTablature4Clef:
    case msrClef::kTablature5Clef:
    case msrClef::kTablature6Clef:
    case msrClef::kTablature7Clef:
      return true;
      break;
    default:
      return false;
  } // switch
}

bool msrClef::clefIsAPercussionClef () const
{
  switch (fClefKind) {
    case msrClef::kPercussionClef:
      return true;
      break;
    default:
      return false;
  } // switch
}

void msrClef::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrClef::acceptIn()" <<
      endl;
      
  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrClef::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrClef::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrClef::acceptOut()" <<
      endl;

  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrClef::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrClef::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrClef& elt)
{
  elt->print (os);
  return os;
}

string msrClef::clefAsString () const
{
  stringstream s;

  s <<
    "Clef" " \"";
   
  switch (fClefKind) {
    case msrClef::k_NoClef:
      s << "none";
      break;
    case msrClef::kTrebleClef:
      s << "treble";
      break;
    case msrClef::kSopranoClef:
      s << "soprano";
      break;
    case msrClef::kMezzoSopranoClef:
      s << "mezzo soprano";
      break;
    case msrClef::kAltoClef:
      s << "alto";
      break;
    case msrClef::kTenorClef:
      s << "tenor";
      break;
    case msrClef::kBaritoneClef:
      s << "baritone";
      break;
    case msrClef::kBassClef:
      s << "bass";
      break;
    case msrClef::kTrebleLine1Clef:
      s << "treble line 1";
      break;
    case msrClef::kTrebleMinus15Clef:
      s << "treble -15";
      break;
    case msrClef::kTrebleMinus8Clef:
      s << "treble -8";
      break;
    case msrClef::kTreblePlus8Clef:
      s << "treble +8";
      break;
    case msrClef::kTreblePlus15Clef:
      s << "treble +15";
      break;
    case msrClef::kBassMinus15Clef:
      s << "bass -15";
      break;
    case msrClef::kBassMinus8Clef:
      s << "bass -8";
      break;
    case msrClef::kBassPlus8Clef:
      s << "bass +8";
      break;
    case msrClef::kBassPlus15Clef:
      s << "bass +15";
      break;
    case msrClef::kVarbaritoneClef:
      s << "varbaritone";
      break;
    case msrClef::kTablature4Clef:
      s << "tablature 4 lines";
      break;
    case msrClef::kTablature5Clef:
      s << "tablature 5 lines";
      break;
    case msrClef::kTablature6Clef:
      s << "tablature 6 lines";
      break;
    case msrClef::kTablature7Clef:
      s << "tablature 7 lines";
      break;
    case msrClef::kPercussionClef:
      s << "percussion";
      break;
  } // switch

  s <<
    "\", line " << fInputLineNumber;

  return s.str();
}

void msrClef::print (ostream& os)
{
  os <<
    clefAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrHumdrumScotKeyItem msrHumdrumScotKeyItem::create (
  int inputLineNumber)
{
  msrHumdrumScotKeyItem* o =
    new msrHumdrumScotKeyItem (
      inputLineNumber);
  assert (o!=0);

  return o;
}

msrHumdrumScotKeyItem::msrHumdrumScotKeyItem (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  if (gGeneralOptions->fTraceKeys) {
    cerr << idtr <<
      "Creating Humdrum/Scot key item" <<
      ", line = " << inputLineNumber <<
      endl;
  }

  fKeyDiatonicPitch = k_NoDiatonicPitch;
  fKeyAlteration    = k_NoAlteration;
  fKeyOctave        = -1; // actual MusicXML octaves are non-negative
}

msrHumdrumScotKeyItem::~msrHumdrumScotKeyItem() {}

void msrHumdrumScotKeyItem::setKeyItemDiatonicPitch (
  msrDiatonicPitch diatonicPitch)
{
  if (gGeneralOptions->fTraceKeys) {
    cerr << idtr <<
      "Setting Humdrum/Scot key item diatonic pitch to '" <<
      msrDiatonicPitchAsString (diatonicPitch) <<
      "'" <<
      endl;
  }
  
  fKeyDiatonicPitch = diatonicPitch;
}

void msrHumdrumScotKeyItem::setKeyItemAlteration (
  msrAlteration alteration)
{
  if (gGeneralOptions->fTraceKeys) {
    cerr << idtr <<
      "Setting Humdrum/Scot key item alteration to '" <<
      msrAlterationAsString (alteration) <<
      "'" <<
      endl;
  }
  
  fKeyAlteration = alteration;
}

void msrHumdrumScotKeyItem::setKeyItemOctave (int keyOctave)
{
  if (gGeneralOptions->fTraceKeys) {
    cerr << idtr <<
      "Setting Humdrum/Scot key item octave to '" <<
      keyOctave <<
      "'" <<
      endl;
  }
  
  fKeyOctave = keyOctave;
}

void msrHumdrumScotKeyItem::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrHumdrumScotKeyItem::acceptIn()" <<
      endl;
      
  if (visitor<S_msrHumdrumScotKeyItem>*
    p =
      dynamic_cast<visitor<S_msrHumdrumScotKeyItem>*> (v)) {
        S_msrHumdrumScotKeyItem elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrHumdrumScotKeyItem::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrHumdrumScotKeyItem::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrHumdrumScotKeyItem::acceptOut()" <<
      endl;

  if (visitor<S_msrHumdrumScotKeyItem>*
    p =
      dynamic_cast<visitor<S_msrHumdrumScotKeyItem>*> (v)) {
        S_msrHumdrumScotKeyItem elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrHumdrumScotKeyItem::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrHumdrumScotKeyItem::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrHumdrumScotKeyItem& elt)
{
  elt->print (os);
  return os;
}

string msrHumdrumScotKeyItem::humdrumScotKeyItemAsString () const
{
  stringstream s;

  s <<
    "HumdrumScotKeyItem" <<
    ", KeyDiatonicPitch" << ": " <<
    msrDiatonicPitchAsString (fKeyDiatonicPitch) <<
    ", KeyAlteration" << ": " <<
    msrAlterationAsString (fKeyAlteration) <<
    ", KeyOctave" << ": " << fKeyOctave <<
    ", line " << fInputLineNumber;
     
  return s.str();
}

void msrHumdrumScotKeyItem::print (ostream& os)
{
  os <<
    humdrumScotKeyItemAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrKey msrKey::createTraditional (
  int                  inputLineNumber,
  msrQuartertonesPitch keyTonicQuartertonesPitch,
  msrKeyModeKind       keyModeKind,
  int                  keyCancel)
{
  msrKey* o =
    new msrKey (
      inputLineNumber,
      keyTonicQuartertonesPitch, keyModeKind,
      keyCancel);
  assert (o!=0);

  return o;
}

S_msrKey msrKey::createHumdrumScot (
  int                  inputLineNumber)
{
  msrKey* o =
    new msrKey (
      inputLineNumber);
  assert (o!=0);

  return o;
}

msrKey::msrKey ( // for traditional keys
  int                  inputLineNumber,
  msrQuartertonesPitch keyTonicQuartertonesPitch,
  msrKeyModeKind       keyModeKind,
  int                  keyCancel)
    : msrElement (inputLineNumber)
{
  // this is a traditional key
  fKeyKind = kTraditionalKind;
  
  fKeyTonicQuartertonesPitch = keyTonicQuartertonesPitch;
  fKeyModeKind               = keyModeKind;
  
  fKeyCancel     = keyCancel;

  // initialization in all cases
  fKeyItemsOctavesAreSpecified = false;
}

msrKey::msrKey ( // for Humdrum/Scot keys
  int                  inputLineNumber)
    : msrElement (inputLineNumber)
{
  // this is a Humdrum/Scot key
  fKeyKind = kHumdrumScotKind;

  // initialization in all cases
  fKeyItemsOctavesAreSpecified = false;
}

msrKey::~msrKey()
{}

void msrKey::appendHumdrumScotKeyItem (
  S_msrHumdrumScotKeyItem item)
{
  if (gGeneralOptions->fTraceKeys) {
    cerr << idtr <<
      "Append item '" <<
      item->humdrumScotKeyItemAsString () <<
      "' to key '" <<
      "'" <<
      endl;
    }

  fHumdrumScotKeyItemsVector.insert (
    fHumdrumScotKeyItemsVector.end(), item);
}

void msrKey::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrKey::acceptIn()" <<
      endl;
      
  if (visitor<S_msrKey>*
    p =
      dynamic_cast<visitor<S_msrKey>*> (v)) {
        S_msrKey elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrKey::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrKey::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrKey::acceptOut()" <<
      endl;

  if (visitor<S_msrKey>*
    p =
      dynamic_cast<visitor<S_msrKey>*> (v)) {
        S_msrKey elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrKey::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrKey::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrKey& elt)
{
  elt->print (os);
  return os;
}

string msrKey::keyKindAsString (
  msrKeyKind keyKind)
{
  string result;
  
  switch (keyKind) {
    case msrKey::kTraditionalKind:
      result = "traditional";
      break;
    case msrKey::kHumdrumScotKind:
      result = "Humdrum/Scot";
      break;
  } // switch

  return result;
}

string msrKey::keyModeKindAsString (
  msrKeyModeKind keyModeKind)
{
  string result;
  
  switch (keyModeKind) {
    case msrKey::kMajorMode:
      result = "major";
      break;
    case msrKey::kMinorMode:
      result = "minor";
      break;
    case msrKey::kIonianMode:
      result = "ionian";
      break;
    case msrKey::kDorianMode:
      result = "dorian";
      break;
    case msrKey::kPhrygianMode:
      result = "phrygian";
      break;
    case msrKey::kLydianMode:
      result = "lydian";
      break;
    case msrKey::kMixolydianMode:
      result = "mixolydian";
      break;
    case msrKey::kAeolianMode:
      result = "aeolian";
      break;
    case msrKey::kLocrianMode:
      result = "locrian";
      break;
  } // switch

  return result;
}

string msrKey::keyAsString () const
{
  stringstream s;

  s <<
    "Key" <<
    ", " << keyKindAsString (fKeyKind) <<
    ", ";

  switch (fKeyKind) {
    case msrKey::kTraditionalKind:
      s <<
        msrQuartertonesPitchAsString (
          gMsrOptions->fMsrQuatertonesPitchesLanguage,
          fKeyTonicQuartertonesPitch) <<
        " " <<
        keyModeKindAsString (fKeyModeKind);
      break;
      
    case msrKey::kHumdrumScotKind:
      s <<
        fHumdrumScotKeyItemsVector.size () << "items" <<
        ", keyItemsOctavesAreSpecified: " <<
        booleanAsString (
          fKeyItemsOctavesAreSpecified);
      break;
  } // switch

  s <<
    ", line " << fInputLineNumber;

  return s.str();
}

void msrKey::print (ostream& os)
{
  os <<
    "Key" <<
    ", " << keyKindAsString (fKeyKind) <<
    ",";

  switch (fKeyKind) {
    case msrKey::kTraditionalKind:
      os <<
        " " <<
        msrQuartertonesPitchAsString (
          gMsrOptions->fMsrQuatertonesPitchesLanguage,
          fKeyTonicQuartertonesPitch) <<
        " " <<
        keyModeKindAsString (fKeyModeKind) <<
        endl;
      break;
      
    case msrKey::kHumdrumScotKind:
      os <<
        ", keyItemsOctavesAreSpecified: " <<
        booleanAsString (
          fKeyItemsOctavesAreSpecified) <<
        ", " <<
        fHumdrumScotKeyItemsVector.size () <<
        " items:";

      if (fHumdrumScotKeyItemsVector.size ()) {
        os <<
          endl;
          
        idtr++;
        
        vector<S_msrHumdrumScotKeyItem>::const_iterator
          iBegin = fHumdrumScotKeyItemsVector.begin(),
          iEnd   = fHumdrumScotKeyItemsVector.end(),
          i      = iBegin;
          
        for ( ; ; ) {
          os << idtr << (*i);
          if (++i == iEnd) break;
    // JMI     os << endl;
        } // for
        
        idtr--;
      }
      
      else
        {
          os <<
            " none";
        }
      break;
  } // switch
}

//______________________________________________________________________________
S_msrTimeItem msrTimeItem::create (
  int inputLineNumber,
  int timeBeatsNumber,
  int timeBeatsValue)
{
  msrTimeItem* o =
    new msrTimeItem (
      inputLineNumber,
      timeBeatsNumber, timeBeatsValue);
  assert (o!=0);

  return o;
}

msrTimeItem::msrTimeItem (
  int inputLineNumber,
  int timeBeatsNumber,
  int timeBeatsValue)
    : msrElement (inputLineNumber)
{
  fTimeBeatsNumber = timeBeatsNumber;
  fTimeBeatsValue  = timeBeatsValue;
  
  if (gGeneralOptions->fTraceTimes) {
    cerr << idtr <<
      "Creating time item containing '" <<
      "timeBeatsNumber = " << fTimeBeatsNumber <<
      " and timeBeatsValue = " << fTimeBeatsValue <<
      ", line = " << inputLineNumber <<
      endl;
  }
}

msrTimeItem::~msrTimeItem() {}

void msrTimeItem::setKeyItemDiatonicPitch (
  msrDiatonicPitch diatonicPitch)
{
  
  fKeyDiatonicPitch = diatonicPitch;
}

void msrTimeItem::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrTimeItem::acceptIn()" <<
      endl;
      
  if (visitor<S_msrTimeItem>*
    p =
      dynamic_cast<visitor<S_msrTimeItem>*> (v)) {
        S_msrTimeItem elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrTimeItem::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrTimeItem::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrTimeItem::acceptOut()" <<
      endl;

  if (visitor<S_msrTimeItem>*
    p =
      dynamic_cast<visitor<S_msrTimeItem>*> (v)) {
        S_msrTimeItem elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrTimeItem::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrTimeItem::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTimeItem& elt)
{
  elt->print (os);
  return os;
}

string msrTimeItem::TimeItemAsString () const
{
  stringstream s;

  s <<
    "TimeItem" <<
    ", timeBeatsNumber: " << fTimeBeatsNumber <<
    ", timeBeatsValue: " << fTimeBeatsValue <<
    ", line " << fInputLineNumber;
     
  return s.str();
}

void msrTimeItem::print (ostream& os)
{
  os <<
    TimeItemAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrTime msrTime::create (
  int inputLineNumber)
{
  msrTime* o =
    new msrTime (
      inputLineNumber);
  assert (o!=0);
  return o;
}

msrTime::msrTime (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  fTimeItemsBeatTypessAreDifferent = false;
}

msrTime::~msrTime() {}

void msrTime::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrTime::acceptIn()" <<
      endl;
      
  if (visitor<S_msrTime>*
    p =
      dynamic_cast<visitor<S_msrTime>*> (v)) {
        S_msrTime elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrTime::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrTime::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrTime::acceptOut()" <<
      endl;

  if (visitor<S_msrTime>*
    p =
      dynamic_cast<visitor<S_msrTime>*> (v)) {
        S_msrTime elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrTime::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrTime::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTime& elt)
{
  elt->print (os);
  return os;
}

void msrKey::appendTimeItem (
  S_msrTimeItem item)
{
  if (gGeneralOptions->fTraceTimes) {
    cerr << idtr <<
      "Append item '" <<
      item->TimeItemAsString () <<
      "' to key '" <<
      "'" <<
      endl;
    }

  fTimeItemsVector.insert (
    fTimeItemsVector.end(), item);
}

string msrTime::timeAsShortString () const
{
  stringstream s;

  s <<
    fBeatsNumber << "/" << fBeatsValue;
  /* JMI
    "Time \"" << 
    fBeatsNumber << "/" << fBeatsValue <<
    "\"";
    */

  return s.str();
}

string msrTime::timeAsString () const
{
  stringstream s;

  s <<
    "Time \"" << 
    fTimeItemsBeatTypessAreDifferent << <<
    booleanAsString (
      fTimeItemsBeatTypessAreDifferent) <<
    ", line " << fInputLineNumber;

  return s.str();
}

void msrTime::print (ostream& os)
{
  os <<
    "Time" <<
    ", timeItemsBeatTypessAreDifferent: " <<
    booleanAsString (
      fTimeItemsBeatTypessAreDifferent) <<
    ", " <<
    fTimeItemsVector.size () <<
    " items:";

  if (fTimeItemsVector.size ()) {
    os <<
      endl;
      
    idtr++;
    
    vector<S_msrTimeItem>::const_iterator
      iBegin = fTimeItemsVector.begin(),
      iEnd   = fTimeItemsVector.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
// JMI     os << endl;
    } // for
    
    idtr--;
  }
  
  else
    {
      os <<
        " none";
    }
}

//______________________________________________________________________________
S_msrTranspose msrTranspose::create (
  int           inputLineNumber,
  int           transposeDiatonic,
  int           transposeChromatic)
{
  msrTranspose* o =
    new msrTranspose (
      inputLineNumber,
      transposeDiatonic, transposeChromatic);
  assert (o!=0);
  return o;
}

msrTranspose::msrTranspose (
  int           inputLineNumber,
  int           transposeDiatonic,
  int           transposeChromatic)
    : msrElement (inputLineNumber)
{
  fTransposeDiatonic  = transposeDiatonic;
  fTransposeChromatic = transposeChromatic;
}

msrTranspose::~msrTranspose() {}

void msrTranspose::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrTranspose::acceptIn()" <<
      endl;
      
  if (visitor<S_msrTranspose>*
    p =
      dynamic_cast<visitor<S_msrTranspose>*> (v)) {
        S_msrTranspose elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrTranspose::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrTranspose::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrTranspose::acceptOut()" <<
      endl;

  if (visitor<S_msrTranspose>*
    p =
      dynamic_cast<visitor<S_msrTranspose>*> (v)) {
        S_msrTranspose elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrTranspose::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrTranspose::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTranspose& elt)
{
  elt->print (os);
  return os;
}

string msrTranspose::transposeAsString () const
{
  stringstream s;

  s <<
    "Transpose " << 
    "diatonic = " << fTransposeDiatonic <<
    ", chromatic= " << fTransposeChromatic;

  return s.str();
}

void msrTranspose::print (ostream& os)
{
  os << transposeAsString () << endl;
}

//______________________________________________________________________________
S_msrWords msrWords::create (
  int                    inputLineNumber,
  msrWordsPlacementKind  wordsPlacementKind,
  string                 wordsContents,
  msrWordsFontStyleKind  wordsFontStyleKind,
  string                 wordsFontSize,
  msrWordsFontWeightKind wordsFontWeightKind,
  msrWordsXMLLangKind    wordsXMLLangKind)
{
  msrWords* o =
    new msrWords (
      inputLineNumber,
      wordsPlacementKind,
      wordsContents,
      wordsFontStyleKind,
      wordsFontSize,
      wordsFontWeightKind,
      wordsXMLLangKind);
  assert(o!=0);
  return o;
}

msrWords::msrWords (
  int                    inputLineNumber,
  msrWordsPlacementKind  wordsPlacementKind,
  string                 wordsContents,
  msrWordsFontStyleKind  wordsFontStyleKind,
  string                 wordsFontSize,
  msrWordsFontWeightKind wordsFontWeightKind,
  msrWordsXMLLangKind    wordsXMLLangKind)
    : msrElement (inputLineNumber)
{
  fWordsPlacementKind  = wordsPlacementKind;
  
  fWordsContents       = wordsContents;

  fWordsFontStyleKind  = wordsFontStyleKind;
  fWordsFontSize       = wordsFontSize;
  fWordsFontWeightKind = wordsFontWeightKind;
  
  fWordsXMLLangKind    = wordsXMLLangKind;
}
msrWords::~msrWords() {}

void msrWords::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrWords::acceptIn()" <<
      endl;
      
  if (visitor<S_msrWords>*
    p =
      dynamic_cast<visitor<S_msrWords>*> (v)) {
        S_msrWords elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrWords::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrWords::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrWords::acceptOut()" <<
      endl;

  if (visitor<S_msrWords>*
    p =
      dynamic_cast<visitor<S_msrWords>*> (v)) {
        S_msrWords elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrWords::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}


void msrWords::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrWords& elt)
{
  elt->print (os);
  return os;
}

string msrWords::wordsPlacementKindAsString (
  msrWordsPlacementKind wordsPlacementKind)
{
  string result;

  switch (wordsPlacementKind) {
    case msrWords::kWordsPlacementAbove:
      result = "above";
      break;
    case msrWords::kWordsPlacementBelow:
      result = "below";
      break;
  } // switch

  return result;
}

string msrWords::wordsFontStyleKindAsString (
  msrWordsFontStyleKind wordsFontStyleKind)
{
  string result;

  switch (wordsFontStyleKind) {
    case kNormalStyle:
      result = "NormalStyle";
      break;
    case KItalicStyle:
      result = "ItalicStyle";
      break;
  } // switch

  return result;
}

string msrWords::msrWordsFontWeightKindAsString (
  msrWordsFontWeightKind wordsFontWeightKind)
{
  string result;

  switch (wordsFontWeightKind) {
    case kNormalWeight:
      result = "NormalWeight";
      break;
    case kBoldWeight:
      result = "BoldWeight";
      break;
  } // switch

  return result;
}

string msrWords::msrWordsXMLLangKindAsString (
  msrWordsXMLLangKind wordsXMLLangKind)
{
  string result;

  switch (wordsXMLLangKind) {
    case kItLang:
      result = "it";
      break;
    case kEnLang:
      result = "en";
      break;
    case kDeLang:
      result = "de";
      break;
    case kFrLang:
      result = "fr";
    case kJaLang:
      result = "ja";
      break;
  } // switch

  return result;
}

string msrWords::wordsAsString () const
{
  stringstream s;

  s <<
    "Words" << " " <<
    fWordsContents << ", placement = " <<
    wordsPlacementKindAsString (fWordsPlacementKind);

  return s.str();
}

void msrWords::print (ostream& os)
{
// JMI  os << wordsAsString () << endl;

  os <<
    "Words" " \"" << fWordsContents << "\"" <<
    endl;

  idtr++;

  const int fieldWidth = 16;

  os <<
    idtr << left <<
      setw(fieldWidth) <<
      "Placement" << " = " <<
      wordsPlacementKindAsString (fWordsPlacementKind) <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "WordsFontStyle" << " = " <<
      wordsFontStyleKindAsString (fWordsFontStyleKind) <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "WordsFontSize" << " = " << fWordsFontSize <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "WordsFontWeight" << " = " <<
      msrWordsFontWeightKindAsString (fWordsFontWeightKind) <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "WordsFontXMLLang" << " = " <<
      msrWordsXMLLangKindAsString (fWordsXMLLangKind) <<
      endl;
  
  idtr--;
}

//______________________________________________________________________________
S_msrTempo msrTempo::create (
  int           inputLineNumber,
  int           tempoUnit,
  int           perMinute)
{
  msrTempo* o =
    new msrTempo (
      inputLineNumber, tempoUnit, perMinute);
  assert(o!=0);
  return o;
}

msrTempo::msrTempo (
  int           inputLineNumber,
  int           tempoUnit,
  int           perMinute)
    : msrElement (inputLineNumber)
{
  fTempoIndication = "";
  
  fTempoUnit = tempoUnit;
  fPerMinute = perMinute;
}
msrTempo::~msrTempo() {}

void msrTempo::setTempoIndication (string indication)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "Setting indication of tempo " <<
      fTempoUnit << " = " << fPerMinute <<
      " to \"" << indication << "\"" <<
      endl;
      
  fTempoIndication = indication;
}

void msrTempo::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrTempo::acceptIn()" <<
      endl;
      
  if (visitor<S_msrTempo>*
    p =
      dynamic_cast<visitor<S_msrTempo>*> (v)) {
        S_msrTempo elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrTempo::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrTempo::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrTempo::acceptOut()" <<
      endl;

  if (visitor<S_msrTempo>*
    p =
      dynamic_cast<visitor<S_msrTempo>*> (v)) {
        S_msrTempo elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrTempo::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}


void msrTempo::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTempo& elt)
{
  elt->print (os);
  return os;
}

string msrTempo::tempoAsString () const
{
  stringstream s;

  s <<
    "Tempo" << " " <<
    ", indication = \"" << fTempoIndication << "\"" <<
    ", " << fTempoUnit << " = " << fPerMinute;

  return s.str();
}

void msrTempo::print (ostream& os)
{
  os << tempoAsString () << endl;
}

//______________________________________________________________________________
S_msrSyllable msrSyllable::create (
  int                   inputLineNumber,
  S_msrPart             syllableDirectPartUplink,
  msrSyllableKind       syllableKind,
  string                syllableText,
  msrSyllableExtendKind syllableExtendKind,
  int                   divisions,
  S_msrStanza           syllableStanzaUplink)
{
  msrSyllable* o =
    new msrSyllable (
      inputLineNumber,
      syllableDirectPartUplink,
      syllableKind, syllableText, syllableExtendKind,
      divisions,
      syllableStanzaUplink);
  assert(o!=0);

  return o;
}

msrSyllable::msrSyllable (
  int                   inputLineNumber,
  S_msrPart             syllableDirectPartUplink,
  msrSyllableKind       syllableKind,
  string                syllableText,
  msrSyllableExtendKind syllableExtendKind,
  int                   divisions,
  S_msrStanza           syllableStanzaUplink)
    : msrElement (inputLineNumber)
{
  // set syllable's direct part uplink
  msrAssert(
    syllableDirectPartUplink != 0,
    "syllableDirectPartUplink is null");
    
  fSyllableDirectPartUplink =
    syllableDirectPartUplink;
    
  fSyllableKind = syllableKind;
  fSyllableText = syllableText;
  fSyllableDivisions  = divisions;

  fSyllableExtendKind = syllableExtendKind;
  
  fSyllableStanzaUplink = syllableStanzaUplink;

  // fSyllableNoteUplink will be set
  // by setSyllableNoteUplink() later
}

msrSyllable::~msrSyllable()
{}

S_msrSyllable msrSyllable::createSyllableShallowClone (
  S_msrPart partClone)
{
  if (gGeneralOptions->fTraceLyrics) {
    cerr << idtr <<
      "Creating a shallow clone of syllable '" <<
      syllableAsString () <<
      "'" <<
      endl;
  }

  msrAssert(
    partClone != 0,
    "partClone is null");
    
  S_msrSyllable
    clone =
      msrSyllable::create (
        fInputLineNumber,
        partClone,
        fSyllableKind,
        fSyllableText,
        fSyllableExtendKind,
        fSyllableDivisions,
        fSyllableStanzaUplink);
    
  // dont't set 'clone->fSyllableStanzaUplink'
  // nor 'clone->fSyllableNoteUplink',
  // this will be done by the caller

   clone->fSyllableNoteUplink = fSyllableNoteUplink; // TEMP
  
  return clone;
}

void msrSyllable::setSyllableNoteUplink (S_msrNote note)
{
  fSyllableNoteUplink = note;

  // register syllable in note if its text is not empty
  if (fSyllableText.size ())
     note->
      appendSyllableToNote (this); // JMI
  
  if (gGeneralOptions->fTraceLyrics) {
    cerr <<
      idtr <<
      "Setting syllable note uplink for:" <<
      endl;

    idtr++;
    
    cerr <<
      idtr <<
        syllableAsString () <<
    // JMI    "to '" << note->noteAsString () <<
      ", line " << note->getInputLineNumber () <<
      endl;

    idtr--;
      
/*
    if (note) { // JMI
      cerr <<
        note->noteAsString () <<
        ", line " << note->getInputLineNumber ();
    }
    else {
      cerr << "none";
    }
    
    cerr <<
      endl;
      */
  }
}

void msrSyllable::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrSyllable::acceptIn()" <<
      endl;
      
  if (visitor<S_msrSyllable>*
    p =
      dynamic_cast<visitor<S_msrSyllable>*> (v)) {
        S_msrSyllable elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrSyllable::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrSyllable::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrSyllable::acceptOut()" <<
      endl;

  if (visitor<S_msrSyllable>*
    p =
      dynamic_cast<visitor<S_msrSyllable>*> (v)) {
        S_msrSyllable elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrSyllable::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrSyllable::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrSyllable& elt)
{
  elt->print (os);
  return os;
}

string msrSyllable::syllableKindAsString (
  msrSyllableKind syllableKind)
{
  string result;
    
  switch (syllableKind) {
    case msrSyllable::kSingleSyllable:
      result = "single";
      break;
    case msrSyllable::kBeginSyllable:
      result = "begin";
      break;
    case msrSyllable::kMiddleSyllable:
      result = "middle";
      break;
    case msrSyllable::kEndSyllable:
      result = "end";
      break;
      
    case msrSyllable::kRestSyllable:
      result = "rest";
      break;
      
    case msrSyllable::kSkipSyllable:
      result = "skip";
      break;
      
    case msrSyllable::kSlurSyllable:
      result = "slur";
      break;
    case msrSyllable::kSlurBeyondEndSyllable:
      result = "slur beyond end";
      break;
      
    case msrSyllable::kLigatureSyllable:
      result = "ligature";
      break;
    case msrSyllable::kLigatureBeyondEndSyllable:
      result = "ligature beyond end";
      break;
      
    case msrSyllable::kTiedSyllable:
      result = "tied";
      break;
      
    case msrSyllable::kBarcheckSyllable:
      result = "bar check";
      break;
      
    case msrSyllable::kBarNumberCheckSyllable:
      result = "barnumber check";
      break;
      
    case msrSyllable::kBreakSyllable:
      result = "break";
      break;
      
    case msrSyllable::k_NoSyllable:
      result = "k_NoSyllable ???";
      break;
  } // switch

  return result;
}

string msrSyllable::syllableKindAsString () const
{
  return syllableKindAsString (fSyllableKind);
}

string msrSyllable::syllableExtendKindAsString (
  msrSyllableExtendKind syllableExtendKind)
{
  string result;
  
  switch (syllableExtendKind) {
    case msrSyllable::kStandaloneSyllableExtend:
      result = "standalone";
      break;
    case msrSyllable::kStartSyllableExtend:
      result = "start";
      break;
    case msrSyllable::kContinueSyllableExtend:
      result = "continue";
      break;
    case msrSyllable::kStopSyllableExtend:
      result = "stop";
      break;
    case msrSyllable::k_NoSyllableExtend:
      result = "none";
      break;
  } // switch

  return result;
}

string msrSyllable::syllableDivisionsAsString () const
{
  return
    fSyllableDirectPartUplink->
      divisionsAsMsrString (
        fInputLineNumber,
        fSyllableDivisions);
}

string msrSyllable::syllableNoteUplinkAsString () const
{
  string result;

  if (fSyllableNoteUplink)
    result =
      "% ==> "
        +
      fSyllableNoteUplink->noteAsString ();
  else
    result =
      "no syllable note uplink";

  return result;
}

string msrSyllable::syllableAsString () const
{
  stringstream s;
  
  switch (fSyllableKind) {
    case kSingleSyllable:
      s <<
        "single" <<
        ", " << "\"" << fSyllableText << "\"" <<
        ":" << syllableDivisionsAsString () <<
        " (" << fSyllableDivisions << ")" <<
        ", line " << fInputLineNumber <<
        ", " <<
        syllableNoteUplinkAsString () <<
        ", NoteSyllableExtendKind: " <<
        syllableExtendKindAsString (fSyllableExtendKind);
      break;
      
    case kBeginSyllable:
      s << 
        "begin" <<
        ", " << "\"" << fSyllableText << "\"" <<
         ":" << syllableDivisionsAsString () <<
        " (" << fSyllableDivisions << ")" <<
        ", line " << fInputLineNumber <<
        ", " <<
        syllableNoteUplinkAsString () <<
        ", NoteSyllableExtendKind: " <<
        syllableExtendKindAsString (fSyllableExtendKind);
      break;
      
    case kMiddleSyllable:
      s << 
        "middle" <<
        ", " << "\"" << fSyllableText << "\"" <<
        ":" << syllableDivisionsAsString () <<
        " (" << fSyllableDivisions << ")" <<
        ", line " << fInputLineNumber <<
        ", " <<
        syllableNoteUplinkAsString () <<
        ", NoteSyllableExtendKind: " <<
        syllableExtendKindAsString (fSyllableExtendKind);
      break;
      
    case kEndSyllable:
      s << 
        "end" <<
        ", " << "\"" << fSyllableText << "\"" <<
        ":" << syllableDivisionsAsString () <<
        " (" << fSyllableDivisions << ")" <<
        ", line " << fInputLineNumber <<
        ", " <<
        syllableNoteUplinkAsString () <<
        ", NoteSyllableExtendKind: " <<
        syllableExtendKindAsString (fSyllableExtendKind);
      break;
      
    case kRestSyllable:
       s << 
        "rest" << ":" << syllableDivisionsAsString () <<
        " (" << fSyllableDivisions << ")" <<
        ", line " << fInputLineNumber <<
        ", " <<
        syllableNoteUplinkAsString ();
      break;
      
    case kSkipSyllable:
      s << 
        "skip" << ":" << syllableDivisionsAsString () <<
        " (" << fSyllableDivisions << ")" <<
        ", line " << fInputLineNumber;
      break;
      
    case kSlurSyllable:
      s << 
        "slur" << ":" << syllableDivisionsAsString () <<
        " (" << fSyllableDivisions << ")" <<
        ", line " << fInputLineNumber <<
        ", " <<
        syllableNoteUplinkAsString ();
      break;
      
    case kSlurBeyondEndSyllable:
      s << 
        "slur beyond end" << ":" << syllableDivisionsAsString () <<
        ", line " << fInputLineNumber <<
        ", " <<
        syllableNoteUplinkAsString ();
      break;
      
    case kLigatureSyllable:
      s << 
        "ligature" << ":" << syllableDivisionsAsString () <<
        " (" << fSyllableDivisions << ")" <<
        ", line " << fInputLineNumber <<
        ", " <<
        syllableNoteUplinkAsString ();
      break;
      
    case kLigatureBeyondEndSyllable:
      s << 
        "ligature beyond end" << ":" << syllableDivisionsAsString () <<
        ", line " << fInputLineNumber <<
        ", " <<
        syllableNoteUplinkAsString ();
      break;
      
    case kTiedSyllable:
      s << 
        "tied" << ":" << syllableDivisionsAsString () <<
        " (" << fSyllableDivisions << ")" <<
        ", line " << fInputLineNumber <<
        ", " <<
        syllableNoteUplinkAsString () <<
        " " << "\"" << fSyllableText << "\"";
      break;
      
    case kBarcheckSyllable:
      // fSyllableText contains the measure number
      s << 
        "bar check" <<
        " measure " << fSyllableText;
      break;
      
    case kBarNumberCheckSyllable:
      // fSyllableText contains the measure number
      s << 
        "bar number check" <<
        " measure " << fSyllableText;
      break;
      
    case kBreakSyllable:
      // fSyllableText contains the measure number
      s << 
        "break" <<
        " measure " << fSyllableText;
      break;
      
    case k_NoSyllable:
      msrInternalError (
        fInputLineNumber,
        "syllable type has not been set");
      break;
  } // switch

  return s.str();
}


void msrSyllable::print (ostream& os)
{  
  os <<
    "Syllable" " " <<
    syllableAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrStanza msrStanza::create (
  int           inputLineNumber,
  S_msrPart     stanzaDirectPartUplink,
  int           stanzaNumber,
  msrStanzaKind stanzaKind,
  S_msrVoice    stanzaVoiceUplink)
{
  msrStanza* o =
    new msrStanza (
      inputLineNumber,
      stanzaDirectPartUplink,
      stanzaNumber,
      stanzaKind,
      stanzaVoiceUplink);
  assert(o!=0);

  return o;
}

msrStanza::msrStanza (
  int           inputLineNumber,
  S_msrPart     stanzaDirectPartUplink,
  int           stanzaNumber,
  msrStanzaKind stanzaKind,
  S_msrVoice    stanzaVoiceUplink)
    : msrElement (inputLineNumber)
{
  // set stanza's direct part uplink
  msrAssert(
    stanzaDirectPartUplink != 0,
    "stanzaDirectPartUplink is null");

  fStanzaDirectPartUplink =
    stanzaDirectPartUplink;
    
  fStanzaNumber = stanzaNumber;
  fStanzaKind   = stanzaKind;

  // set stanza's voice uplink
  msrAssert(
    stanzaVoiceUplink != 0,
    "stanzaVoiceUplink is null");

  fStanzaVoiceUplink =
    stanzaVoiceUplink;
  
  initializeStanza ();
}

void msrStanza::initializeStanza ()
{
  switch (fStanzaKind) {
    case kRegularStanza:
      fStanzaName =
        fStanzaVoiceUplink->getVoiceName() +
        "_Stanza_" +
        int2EnglishWord (fStanzaNumber);
      break;
      
    case kMasterStanza:
      fStanzaName =
        fStanzaVoiceUplink->getVoiceName() +
        "_Stanza_" +
        "MASTER";
      break;
  } // switch
 
  if (gGeneralOptions->fTraceLyrics)
    cerr << idtr <<
      "Creating stanza " << getStanzaName () << endl;

  fStanzaTextPresent = false;
}

string msrStanza::getStanzaName () const
{
  return fStanzaName;
}

msrStanza::~msrStanza() {}

S_msrStanza msrStanza::createStanzaShallowClone (
  S_msrVoice voiceClone)
{
  if (gGeneralOptions->fTraceLyrics) {
    cerr << idtr <<
      "Creating a shallow clone of stanza \"" <<
      getStanzaName () <<
      "\" in voice \"" <<
      voiceClone->getVoiceName () <<
      "\"" <<
      endl;
  }

  msrAssert(
    voiceClone != 0,
    "voiceClone is null");
    
  S_msrStanza
    clone =
      msrStanza::create (
        fInputLineNumber,
        voiceClone->getVoiceDirectPartUplink (),
        fStanzaNumber,
        fStanzaKind,
        voiceClone);

  clone->fStanzaTextPresent =
    fStanzaTextPresent;

  // add the stanza clone to the voice clone
  voiceClone->
    addStanzaToVoiceWithoutCatchUp (clone);
    
  return clone;
}

void msrStanza::appendSyllableToStanza (
  S_msrSyllable syllable)
{
  if (gGeneralOptions->fTraceLyrics)
    cerr <<
      idtr <<
        "% Appending syllable " << syllable->syllableAsString () <<
      idtr <<
        "to stanza" << getStanzaName () <<
      endl;
      
  // append the syllable to this stanza
  fSyllables.push_back (syllable);

  // does this stanza contain text?
  switch (syllable->getSyllableKind ()) {
    
    case msrSyllable::kSingleSyllable:
    case msrSyllable::kBeginSyllable:
    case msrSyllable::kMiddleSyllable:
    case msrSyllable::kEndSyllable:
      // only now, in case addSyllableToStanza() is called
      // from LPSR for example
      fStanzaTextPresent = true;
      break;
      
    case msrSyllable::kRestSyllable:
    case msrSyllable::kSkipSyllable:
    case msrSyllable::kSlurSyllable:
    case msrSyllable::kSlurBeyondEndSyllable:
    case msrSyllable::kLigatureSyllable:
    case msrSyllable::kLigatureBeyondEndSyllable:
    case msrSyllable::kTiedSyllable:
    case msrSyllable::kBarcheckSyllable:
    case msrSyllable::kBarNumberCheckSyllable:
    case msrSyllable::kBreakSyllable:
      break;
      
    case msrSyllable::k_NoSyllable:
      msrInternalError (
        fInputLineNumber,
        "syllable type has not been set");
      break;
  } // switch
}

S_msrSyllable msrStanza::appendRestSyllableToStanza (
  int inputLineNumber,
  int divisions)
{
  if (gGeneralOptions->fTraceLyrics) {
    cerr << idtr <<
      "% Appending 'Rest' syllable:" << divisions <<
      " to stanza " << getStanzaName () <<
      ", divisions = " << divisions <<
      endl;
  }
  
  // create stanza rest syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        fStanzaDirectPartUplink,
        msrSyllable::kRestSyllable, "",
        msrSyllable::k_NoSyllableExtend,
        divisions,
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendSkipSyllableToStanza (
  int inputLineNumber,
  int divisions)
{
  if (gGeneralOptions->fTraceLyrics) {
    cerr << idtr <<
      "% Appending 'Skip' syllable, " <<
      divisions << " divs" <<
      " to stanza " << getStanzaName () <<
      endl;
  }
  
  // create stanza skip syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        fStanzaDirectPartUplink,
        msrSyllable::kSkipSyllable,
        "",
        msrSyllable::k_NoSyllableExtend,
        divisions,
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendTiedSyllableToStanza (
  int inputLineNumber,
  int divisions)
{
  if (gGeneralOptions->fTraceLyrics) {
    cerr << idtr <<
      "% Appending 'Tied' syllable: " << divisions <<
      " to stanza " << getStanzaName () << endl;
  }
  
  // create stanza tied syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        fStanzaDirectPartUplink,
        msrSyllable::kTiedSyllable,
        "",
        msrSyllable::k_NoSyllableExtend,
        divisions,
        this);
        
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendSlurSyllableToStanza (
  int inputLineNumber,
  int divisions)
{
  if (gGeneralOptions->fTraceLyrics) {
    cerr << idtr <<
      "% Appending 'Slur' syllable:" << divisions <<
      " to stanza " << getStanzaName () << endl;
  }
  
  // create stanza slur syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        fStanzaDirectPartUplink,
        msrSyllable::kSlurSyllable, "",
        msrSyllable::k_NoSyllableExtend,
        divisions,
        this);
        
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendSlurBeyondEndSyllableToStanza (
  int inputLineNumber,
  int divisions)
{
  if (gGeneralOptions->fTraceLyrics) {
    cerr << idtr <<
      "% Appending a 'SlurBeyondEnd' syllable: " << divisions <<
      " to stanza " << getStanzaName () << endl;
  }
  
  // create stanza slur syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        fStanzaDirectPartUplink,
        msrSyllable::kSlurBeyondEndSyllable, "",
        msrSyllable::k_NoSyllableExtend,
        divisions,
        this);
        
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendLigatureSyllableToStanza (
  int inputLineNumber,
  int divisions)
{
  if (gGeneralOptions->fTraceLyrics) {
    cerr << idtr <<
      "% Appending 'Ligature' syllable:" << divisions <<
      " to stanza " << getStanzaName () << endl;
  }
  
  // create stanza ligature syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        fStanzaDirectPartUplink,
        msrSyllable::kLigatureSyllable, "",
        msrSyllable::k_NoSyllableExtend,
        divisions,
        this);
        
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendLigatureBeyondEndSyllableToStanza (
  int inputLineNumber,
  int divisions)
{
  if (gGeneralOptions->fTraceLyrics) {
    cerr << idtr <<
      "% Appending a 'LigatureBeyondEnd' syllable: " << divisions <<
      " to stanza " << getStanzaName () << endl;
  }
  
  // create stanza ligature syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        fStanzaDirectPartUplink,
        msrSyllable::kLigatureBeyondEndSyllable, "",
        msrSyllable::k_NoSyllableExtend,
        divisions,
        this);
        
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendBarcheckSyllableToStanza (
  int inputLineNumber,
  int nextMeasureNumber)
{
  if (gGeneralOptions->fTraceLyrics) {
    cerr << idtr <<
      "% Appending a 'barcheck' syllable" <<
      " to stanza " << getStanzaName () << endl;
  }

  // create stanza break syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        fStanzaDirectPartUplink,
        msrSyllable::kBarcheckSyllable,
        to_string (nextMeasureNumber),
        msrSyllable::k_NoSyllableExtend,
        0,
        this);
       
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendBarNumberCheckSyllableToStanza (
  int    inputLineNumber,
  string nextMeasureNumber)
{
  if (gGeneralOptions->fTraceLyrics) {
    cerr << idtr <<
      "% Appending a 'bar number check' syllable" <<
      " to stanza " << getStanzaName () << endl;
  }

  // convert nextMeasureNumber to string
  stringstream s;
  s << nextMeasureNumber;
  
  // create stanza break syllable        
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        fStanzaDirectPartUplink,
        msrSyllable::kBarNumberCheckSyllable,
        s.str(),
        msrSyllable::k_NoSyllableExtend,
        0,
        this);
       
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendBreakSyllableToStanza (
  int    inputLineNumber,
  string nextMeasureNumber)
{
  if (gGeneralOptions->fTraceLyrics) {
    cerr << idtr <<
      "% Appending a 'Break' syllable" <<
      " to stanza " << getStanzaName () << endl;
  }

  // convert nextMeasureNumber to string
  stringstream s;
  s << nextMeasureNumber;
  
  // create stanza break syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        fStanzaDirectPartUplink,
        msrSyllable::kBreakSyllable,
        s.str(),
        msrSyllable::k_NoSyllableExtend,
        0,
        this);
        
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // and return it
  return syllable;
}

void msrStanza::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrStanza::acceptIn()" <<
      endl;
      
  if (visitor<S_msrStanza>*
    p =
      dynamic_cast<visitor<S_msrStanza>*> (v)) {
        S_msrStanza elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrStanza::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrStanza::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrStanza::acceptOut()" <<
      endl;

  if (visitor<S_msrStanza>*
    p =
      dynamic_cast<visitor<S_msrStanza>*> (v)) {
        S_msrStanza elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrStanza::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrStanza::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrStanza::browseData()" <<
      endl;

  idtr++;
  
  // browse the syllables
  int n = fSyllables.size ();
  for (int i = 0; i < n; i++) {
    // browse the syllable
    msrBrowser<msrSyllable> browser (v);
    browser.browse (*fSyllables [i]);
  } // for
  cerr <<
    endl;

  idtr--;

  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% <== msrStanza::browseData()" <<
      endl;
}

ostream& operator<< (ostream& os, const S_msrStanza& elt)
{
  elt->print (os);
  return os;
}

string msrStanza::stanzaKindAsString (
  msrStanzaKind stanzaKind)
{
 string result;
  
  switch (stanzaKind) {
    case msrStanza::kRegularStanza:
      result = "regular";
      break;
      
    case msrStanza::kMasterStanza:
      result = "master";
      break;
  } // switch

  return result;
}

void msrStanza::print (ostream& os)
{
  // getch stanza kind with an initial uppper case letter
  string stanzaKind =
    stanzaKindAsString (fStanzaKind);

  stanzaKind [0] = toupper (stanzaKind [0]);
  
  os <<
    stanzaKind << " " "stanza" " " << getStanzaName () <<
    " (" << fSyllables.size () << " syllables)";
    
  if (! fStanzaTextPresent)
    os << " (No actual text)";

  os << endl;

//  if (fStanzaTextPresent) {  JMI
    idtr++;

    int n = fSyllables.size();
    for (int i = 0; i < n; i++) {
      os << idtr << fSyllables [i];
    } // for
    os << endl;

    idtr--;
 // }
}

//______________________________________________________________________________
S_msrHarmony msrHarmony::create (
  int                  inputLineNumber,
  S_msrPart            harmonyDirectPartUplink,
  msrQuartertonesPitch harmonyRootQuartertonesPitch,
  msrHarmonyKind       harmonyKind,
  string               harmonyKindText,
  msrQuartertonesPitch harmonyBassQuartertonesPitch,
  int                  harmonySoundingDivisions)
{
  msrHarmony* o =
    new msrHarmony (
      inputLineNumber,
      harmonyDirectPartUplink,
      harmonyRootQuartertonesPitch,
      harmonyKind, harmonyKindText,
      harmonyBassQuartertonesPitch,
      harmonySoundingDivisions);
  assert(o!=0);

  return o;
}

msrHarmony::msrHarmony (
  int                  inputLineNumber,
  S_msrPart            harmonyDirectPartUplink,
  msrQuartertonesPitch harmonyRootQuartertonesPitch,
  msrHarmonyKind       harmonyKind,
  string               harmonyKindText,
  msrQuartertonesPitch harmonyBassQuartertonesPitch,
  int                  harmonySoundingDivisions)
    : msrElement (inputLineNumber)
{
  // set harmony's direct part uplink
  msrAssert(
    harmonyDirectPartUplink != 0,
     "harmonyDirectPartUplink is null");
     
  fHarmonyDirectPartUplink =
    harmonyDirectPartUplink;
    
  fHarmonyRootQuartertonesPitch = harmonyRootQuartertonesPitch;
 
  fHarmonyKind     = harmonyKind;
  fHarmonyKindText = harmonyKindText;
 
  fHarmonyBassQuartertonesPitch = harmonyBassQuartertonesPitch;
 
  fHarmonySoundingDivisions = harmonySoundingDivisions;
  
  if (gGeneralOptions->fTraceHarmonies) {
    cerr << idtr <<
      "Creating harmony '" <<
      harmonyAsString () <<
      "'" <<
      endl;
  }
}

msrHarmony::~msrHarmony() {}

S_msrHarmony msrHarmony::createHarmonyShallowClone (S_msrPart partClone)
{
  if (gGeneralOptions->fTraceHarmonies) {
    cerr << idtr <<
      "Creating a shallow clone or harmony '" <<
      harmonyKindAsShortString () <<
      "'" <<
      endl;
  }

  msrAssert(
    partClone != 0,
    "partClone is null");
    
  S_msrHarmony
    clone =
      msrHarmony::create (
        fInputLineNumber,
        partClone,
        fHarmonyRootQuartertonesPitch,
        fHarmonyKind, fHarmonyKindText,
        fHarmonyBassQuartertonesPitch,
        fHarmonySoundingDivisions);
  
  return clone;
}

string msrHarmony::harmonyKindAsString (
  msrHarmonyKind harmonyKind)
{
  string result;
  
  switch (harmonyKind) {
    case msrHarmony::kMajor:
      result = "Major";
      break;
    case msrHarmony::kMinor:
      result = "Minor";
      break;
    case msrHarmony::kDominant:
      result = "Dominant";
      break;
    case msrHarmony::kAugmented:
      result = "Augmented";
      break;
    case msrHarmony::kDiminished:
      result = "Diminished";
      break;
    case msrHarmony::kSuspendedFourth:
      result = "SuspendedFourth";
      break;
    case msrHarmony::kMajorSeventh:
      result = "MajorSeventh";
      break;
    case msrHarmony::kMinorSeventh:
      result = "MinorSeventh";
      break;
    case msrHarmony::kMajorNinth:
      result = "MajorNinth";
      break;
    case msrHarmony::kMinorNinth:
      result = "MinorNinth";
      break;
    case msrHarmony::k_NoHarmony:
      result = "Harmony???";
      break;
  } // switch

  return result;
}

string msrHarmony::harmonyKindAsShortString () const
{
  string result;
  
  switch (fHarmonyKind) {
    case msrHarmony::kMajor:
      break;
    case msrHarmony::kMinor:
      result = "m";
      break;
    case msrHarmony::kDominant:
      result = "7";
      break;
    case msrHarmony::kAugmented:
      result = "+";
      break;
    case msrHarmony::kDiminished:
      result = "dim";
      break;
    case msrHarmony::kSuspendedFourth:
      result = "sus4";
      break;
    case msrHarmony::kMajorSeventh:
      result = "∆7";
      break;
    case msrHarmony::kMinorSeventh:
      result = "m7";
      break;
    case msrHarmony::kMajorNinth:
      result = "9";
      break;
    case msrHarmony::kMinorNinth:
      result = "-9";
      break;
    case msrHarmony::k_NoHarmony:
      result = "Harmony???";
      break;
  } // switch

  return result;
}

string msrHarmony::harmonyAsString () const
{
  stringstream s;

  s <<
    msrQuartertonesPitchAsString (
      gMsrOptions->fMsrQuatertonesPitchesLanguage,
      fHarmonyRootQuartertonesPitch) <<          
    harmonyKindAsShortString ();

  if (fHarmonyDirectPartUplink) // JMI ???
    s <<
      fHarmonyDirectPartUplink->
        divisionsAsMsrString (
          fInputLineNumber,
          fHarmonySoundingDivisions);

  if (fHarmonyKindText.size ())
    s <<
      " (" <<fHarmonyKindText << ") ";

  if (fHarmonyBassQuartertonesPitch != k_NoQuaterTonesPitch)
    s <<
      "/" <<
    msrQuartertonesPitchAsString (
      gMsrOptions->fMsrQuatertonesPitchesLanguage,
      fHarmonyBassQuartertonesPitch);    

  return s.str();
}

void msrHarmony::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrHarmony::acceptIn()" <<
      endl;
      
  if (visitor<S_msrHarmony>*
    p =
      dynamic_cast<visitor<S_msrHarmony>*> (v)) {
        S_msrHarmony elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrHarmony::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrHarmony::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrHarmony::acceptOut()" <<
      endl;

  if (visitor<S_msrHarmony>*
    p =
      dynamic_cast<visitor<S_msrHarmony>*> (v)) {
        S_msrHarmony elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrHarmony::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrHarmony::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrHarmony& elt)
{
  elt->print (os);
  return os;
}

void msrHarmony::print (ostream& os)
{  
  os <<
    "Harmony" <<
    ", " <<
    fHarmonyDirectPartUplink->
      divisionsAsMsrString (
        fInputLineNumber,
        fHarmonySoundingDivisions) <<
    " (" << fHarmonySoundingDivisions << " sounddivs)" <<
     ", line " << fInputLineNumber <<
    endl;
    
  idtr++;

  const int fieldWidth = 15;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "HarmonyRoot" << " = " <<
      msrQuartertonesPitchAsString (
        gMsrOptions->fMsrQuatertonesPitchesLanguage,
        fHarmonyRootQuartertonesPitch) <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "HarmonyKind" << " = " <<
      harmonyKindAsString (fHarmonyKind) <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "HarmonyKindText" << " = " <<
      fHarmonyKindText <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "HarmonyBass" << " = " <<
      msrQuartertonesPitchAsString (
        gMsrOptions->fMsrQuatertonesPitchesLanguage,
        fHarmonyBassQuartertonesPitch) <<
      endl;

  idtr--;
}

//______________________________________________________________________________
S_msrSegno msrSegno::create (
  int                       inputLineNumber)
{
  msrSegno* o =
    new msrSegno (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrSegno::msrSegno (
  int                       inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrSegno::~msrSegno() {}

void msrSegno::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrSegno::acceptIn()" <<
      endl;
      
  if (visitor<S_msrSegno>*
    p =
      dynamic_cast<visitor<S_msrSegno>*> (v)) {
        S_msrSegno elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrSegno::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrSegno::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrSegno::acceptOut()" <<
      endl;

  if (visitor<S_msrSegno>*
    p =
      dynamic_cast<visitor<S_msrSegno>*> (v)) {
        S_msrSegno elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrSegno::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrSegno::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrSegno& elt)
{
  elt->print (os);
  return os;
}

void msrSegno::print (ostream& os)
{
  os <<
    "Segno" <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrCoda msrCoda::create (
  int                       inputLineNumber)
{
  msrCoda* o =
    new msrCoda (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrCoda::msrCoda (
  int                       inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrCoda::~msrCoda() {}

void msrCoda::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrCoda::acceptIn()" <<
      endl;
      
  if (visitor<S_msrCoda>*
    p =
      dynamic_cast<visitor<S_msrCoda>*> (v)) {
        S_msrCoda elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrCoda::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrCoda::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrCoda::acceptOut()" <<
      endl;

  if (visitor<S_msrCoda>*
    p =
      dynamic_cast<visitor<S_msrCoda>*> (v)) {
        S_msrCoda elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrCoda::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrCoda::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrCoda& elt)
{
  elt->print (os);
  return os;
}

void msrCoda::print (ostream& os)
{
  os <<
    "Coda" <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrEyeGlasses msrEyeGlasses::create (
  int                       inputLineNumber)
{
  msrEyeGlasses* o =
    new msrEyeGlasses (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrEyeGlasses::msrEyeGlasses (
  int                       inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrEyeGlasses::~msrEyeGlasses() {}

void msrEyeGlasses::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrEyeGlasses::acceptIn()" <<
      endl;
      
  if (visitor<S_msrEyeGlasses>*
    p =
      dynamic_cast<visitor<S_msrEyeGlasses>*> (v)) {
        S_msrEyeGlasses elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrEyeGlasses::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrEyeGlasses::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrEyeGlasses::acceptOut()" <<
      endl;

  if (visitor<S_msrEyeGlasses>*
    p =
      dynamic_cast<visitor<S_msrEyeGlasses>*> (v)) {
        S_msrEyeGlasses elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrEyeGlasses::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrEyeGlasses::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrEyeGlasses& elt)
{
  elt->print (os);
  return os;
}

void msrEyeGlasses::print (ostream& os)
{
  os <<
    "EyeGlasses" <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrPedal msrPedal::create (
  int              inputLineNumber,
  msrPedalType     pedalType,
  msrPedalLine     pedalLine)
{
  msrPedal* o =
    new msrPedal (
      inputLineNumber,
      pedalType, pedalLine);
  assert(o!=0);
  return o;
}

msrPedal::msrPedal (
  int              inputLineNumber,
  msrPedalType     pedalType,
  msrPedalLine     pedalLine)
    : msrElement (inputLineNumber)
{
  fPedalType = pedalType;
  fPedalLine = pedalLine;
}

msrPedal::~msrPedal() {}

string msrPedal::pedalTypeAsString ()
{
  string result;
  
  switch (fPedalType) {
    case kPedalStart:
      result = "start pedal";
      break;
    case kPedalContinue:
      result = "continue pedal";
      break;
    case kPedalChange:
      result = "change pedal";
      break;
    case kPedalStop:
      result = "stop pedal";
      break;
  } // switch

  return result;
}
      
string msrPedal::pedalLineAsString ()
{
  string result;
  
  switch (fPedalLine) {
    case kPedalLineYes:
      result = "pedal line: yes";
      break;
    case kPedalLineNo:
      result = "pedal line: no";
      break;
  } // switch

  return result;
}

void msrPedal::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrPedal::acceptIn()" <<
      endl;
      
  if (visitor<S_msrPedal>*
    p =
      dynamic_cast<visitor<S_msrPedal>*> (v)) {
        S_msrPedal elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrPedal::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrPedal::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrPedal::acceptOut()" <<
      endl;

  if (visitor<S_msrPedal>*
    p =
      dynamic_cast<visitor<S_msrPedal>*> (v)) {
        S_msrPedal elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrPedal::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrPedal::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrPedal& elt)
{
  elt->print (os);
  return os;
}

void msrPedal::print (ostream& os)
{
  os <<
    "Pedal" <<
    ", line " << fInputLineNumber <<
    pedalTypeAsString () << ", " <<
    pedalLineAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrBarline msrBarline::create (
  int                       inputLineNumber,
  bool                      barlineHasSegno,
  bool                      barlineHasCoda,
  msrBarlineLocation        location,
  msrBarlineStyle           style,
  msrBarlineEndingType      endingType,
  string                    endingNumber,
  msrBarlineRepeatDirection repeatDirection,
  msrBarlineRepeatWinged    repeatWinged)
{
  msrBarline* o =
    new msrBarline (
      inputLineNumber,
      barlineHasSegno, barlineHasCoda,
      location, style,
      endingType, endingNumber,
      repeatDirection, repeatWinged);
  assert(o!=0);
  return o;
}

msrBarline::msrBarline (
  int                       inputLineNumber,
  bool                      barlineHasSegno,
  bool                      barlineHasCoda,
  msrBarlineLocation        location,
  msrBarlineStyle           style,
  msrBarlineEndingType      endingType,
  string                    endingNumber,
  msrBarlineRepeatDirection repeatDirection,
  msrBarlineRepeatWinged    repeatWinged)
    : msrElement (inputLineNumber)
{
  fBarlineHasSegno = barlineHasSegno;
  fBarlineHasCoda  = barlineHasCoda;
  
  fLocation        = location;
  fStyle           = style;
  fEndingType      = endingType;
  fEndingNumber    = endingNumber;
  fRepeatDirection = repeatDirection;
  fRepeatWinged    = repeatWinged;
  
  // JMI cerr << "fEndingNumber = " << fEndingNumber << endl;
  
  // extract individual numbers from fEndingNumber
  // that may contain "1, 2"
  fEndingNumbersList =
    extractNumbersFromString (
      fEndingNumber, false); // 'true' to debug it
}

msrBarline::~msrBarline() {}

void msrBarline::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrBarline::acceptIn()" <<
      endl;
      
  if (visitor<S_msrBarline>*
    p =
      dynamic_cast<visitor<S_msrBarline>*> (v)) {
        S_msrBarline elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrBarline::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrBarline::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrBarline::acceptOut()" <<
      endl;

  if (visitor<S_msrBarline>*
    p =
      dynamic_cast<visitor<S_msrBarline>*> (v)) {
        S_msrBarline elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrBarline::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}


void msrBarline::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrBarline& elt)
{
  elt->print (os);
  return os;
}

string msrBarline::barlineLocationAsString (
  msrBarlineLocation barlineLocation)
{
  string result;
  
  switch (barlineLocation) {
    case k_NoLocation:
      result = "none";
      break;
    case kLeftLocation:
      result = "Left";
      break;
    case kMiddleLocation:
      result = "Middle";
      break;
    case kRightLocation:
      result = "Right";
      break;
  } // switch

  return result;
}

string msrBarline::barlineCategoryAsString (
  msrBarlineCategory barlineCategory)
{
  string result;
  
  switch (barlineCategory) {
    case kStandaloneBarline:
      result = "standalone";
      break;
    case kRepeatStartBarline:
      result = "repeat start";
      break;
    case kRepeatEndBarline:
      result = "repeat end";
      break;
    case kHookedEndingStartBarline:
      result = "hooked ending start";
      break;
    case kHookedEndingEndBarline:
      result = "hooked ending end";
      break;
    case kHooklessEndingStartBarline:
      result = "hookless ending start";
      break;
    case kHooklessEndingEndBarline:
      result = "hookless ending end";
      break;
  } // switch

  return result;
}

string msrBarline::barlineStyleAsString (
  msrBarlineStyle barlineStyle)
{
  string result;
  
  switch (barlineStyle) {
    case k_NoStyle:
      result = "none";
      break;
    case kRegularStyle:
      result = "Regular";
      break;
    case kDottedStyle:
      result = "Dotted";
      break;
    case kDashedStyle:
      result = "Dashed";
      break;
    case kHeavyStyle:
      result = "Heavy";
      break;
    case kLightLightStyle:
      result = "LightLight";
      break;
    case kLightHeavyStyle:
      result = "LightHeavy";
      break;
    case kHeavyLightStyle:
      result = "HeavyLight";
      break;
    case kHeavyHeavyStyle:
      result = "HeavyHeavy";
      break;
    case kTickStyle:
      result = "Tick";
      break;
    case kShortStyle:
      result = "Short";
      break;
    case kNoneStyle:
      result = "None";
      break;
  } // switch

  return result;
}

string msrBarline::barlineEndingTypeAsString (
  msrBarlineEndingType barlineEndingType)
{
  string result;
  
  switch (barlineEndingType) {
    case k_NoEndingType:
      result = "none";
      break;
    case kStartEndingType:
      result = "Start";
      break;
    case kStopEndingType:
      result = "Stop";
      break;
    case kDiscontinueEndingType:
      result = "Discontinue";
      break;
  } // switch

  return result;
}

string msrBarline::barlineRepeatDirectionAsString (
  msrBarlineRepeatDirection barlineRepeatDirection)
{
  string result;
  
  switch (barlineRepeatDirection) {
    case k_NoRepeatDirection:
      result ="none";
      break;
    case kForwardRepeatDirection:
      result ="Forward";
      break;
    case kBackwardRepeatDirection:
      result ="Backward";
      break;
  } // switch

  return result;
}

string msrBarline::barlineRepeatWingedAsString (
  msrBarlineRepeatWinged barlineRepeatWinged)
{
  string result;
  
  switch (barlineRepeatWinged) {
    case k_NoRepeatWinged:
      result = "none"; // ??? JMI
      break;
    case kNoneRepeatWinged:
      result = "none";
      break;
    case kStraightRepeatWinged:
      result = "Straight";
      break;
    case kCurvedRepeatWinged:
      result = "Curved";
      break;
    case kDoubleStraightRepeatWinged:
      result = "DoubleStraight";
      break;
    case kDoubleCurvedRepeatWinged:
      result = "DoubleCurved";
      break;
  } // switch

  return result;
}

string msrBarline::barlineAsString () const
{
  stringstream s;

  s <<
    "Barline " << barlineCategoryAsString (fBarlineCategory) <<
    ", line " << fInputLineNumber <<
    ", EndingType" << " : " <<
    barlineEndingTypeAsString (fEndingType) <<
    ", RepeatDirection" << " : " <<
    barlineRepeatDirectionAsString (fRepeatDirection);
    
  return s.str();
}

void msrBarline::print (ostream& os)
{
  os <<
    "Barline, " << barlineCategoryAsString (fBarlineCategory) <<
    ", line " << fInputLineNumber;

  if (fBarlineHasSegno)
    os << ", has segno";
    
  if (fBarlineHasCoda)
    os << ", has coda";

  os <<
    endl;

  idtr++;

  const int fieldWidth = 15;

  os <<
    idtr << left <<
      setw(fieldWidth) <<
      "Location" << " : " <<
      barlineLocationAsString (fLocation) <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "Style" << " : " <<
      barlineStyleAsString (fStyle) <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "EndingType" << " : " <<
      barlineEndingTypeAsString (fEndingType) <<
      endl;
  
  os <<
    idtr <<
      setw(fieldWidth) <<
      "Ending number" << " : ";
      
  list<int>::const_iterator i;
  for (i=fEndingNumbersList.begin(); i!=fEndingNumbersList.end(); i++) {
    os << (*i) << " ";
  } // for
  
  os <<
    endl;
 
  os <<
    idtr <<
      setw(fieldWidth) <<
       "RepeatDirection" << " : " <<
      barlineRepeatDirectionAsString (fRepeatDirection) <<
      endl;
  
  os <<
    idtr <<
      setw(fieldWidth) <<
      "RepeatWinged" << " : " <<
      barlineRepeatWingedAsString (fRepeatWinged) <<
      endl;
     
  idtr--;
}

//______________________________________________________________________________
S_msrMeasure msrMeasure::create (
  int           inputLineNumber,
  S_msrPart     measureDirectPartUplink,
  string        measureNumber,
  S_msrSegment  measureSegmentUplink)
{
  msrMeasure* o =
    new msrMeasure (
      inputLineNumber,
      measureDirectPartUplink,
      measureNumber,
      measureSegmentUplink);
  assert(o!=0);

  return o;
}

msrMeasure::msrMeasure (
  int           inputLineNumber,
  S_msrPart     measureDirectPartUplink,
  string        measureNumber,
  S_msrSegment  measureSegmentUplink)
    : msrElement (inputLineNumber)
{
  // set measure's direct part uplink
  msrAssert(
    measureDirectPartUplink != 0,
    "measureDirectPartUplink is null");

  fMeasureDirectPartUplink =
    measureDirectPartUplink;
    
  fMeasureNumber = measureNumber;
  
  // set measure's segment uplink
  msrAssert(
    measureSegmentUplink != 0,
    "measureSegmentUplink is null");

  fMeasureSegmentUplink =
    measureSegmentUplink;

  initializeMeasure ();
}

void msrMeasure::initializeMeasure ()
{
  // set measure voice direct uplink
  fMeasureVoiceDirectUplink =
    fMeasureSegmentUplink->
      getSegmentVoiceUplink ();

  if (gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Creating measure " << fMeasureNumber <<
      " in segment " <<
      fMeasureSegmentUplink->getSegmentAbsoluteNumber () <<
      " in voice \"" <<
      fMeasureVoiceDirectUplink->getVoiceName () <<
      "\"" <<
      ", line " << fInputLineNumber <<
      endl;

  fMeasureKind = kUnknownMeasureKind;

  // fetch measure time from the part,
  // which will set fMeasureDivisionsPerFullMeasure
  setMeasureCurrentTime (
    fMeasureDirectPartUplink->
      getPartCurrentTime ());
        
  setMeasurePosition (
    fInputLineNumber, 1); // ready to receive the first note

  // initialize measure position high tide
  fMeasureDirectPartUplink->
    setPartMeasurePositionHighTide (
      fInputLineNumber, 1);
}

msrMeasure::~msrMeasure()
{}

S_msrMeasure msrMeasure::createMeasureShallowClone (
  S_msrSegment segmentClone)
{
  if (gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Creating a shallow clone of measure '" <<
      fMeasureNumber <<
      "'" <<
      " in segment " << segmentClone->segmentAsString () <<
      " in voice \"" <<
        segmentClone->
          getSegmentVoiceUplink ()->
            getVoiceName () <<
      "\"" <<
      endl;
  
  msrAssert(
    segmentClone != 0,
    "segmentClone is null");

  // get direct part uplink
  S_msrPart
    directPartUplink =
      segmentClone->
        getSegmentDirectPartUplink ();

  // create clone
  S_msrMeasure
    clone =
      msrMeasure::create (
        fInputLineNumber,
        directPartUplink,
        fMeasureNumber,
        segmentClone);

  // set clone measure kind
  clone->fMeasureKind =
    fMeasureKind;

  // set clone time
  clone->
    setMeasureCurrentTime (
      directPartUplink->
        getPartCurrentTime ());

  // set clone 'first in segment' kind
  clone->fMeasureFirstInSegmentKind =
    fMeasureFirstInSegmentKind;

  return clone;
}

void msrMeasure::setMeasurePosition (
  int inputLineNumber,
  int measurePosition)
{
  if (gGeneralOptions->fTraceDivisions)
    cerr << idtr <<
      "Setting measure " << fMeasureNumber <<
      " measure position to '"  << measurePosition <<
      "', line " << inputLineNumber <<
      endl;
  
  fMeasurePosition = measurePosition;
}

void msrMeasure::setMeasureCurrentTime (S_msrTime time)
{
  msrAssert(
    time != 0, "time is null");
    
  fMeasureCurrentTime = time;
  
  fMeasureDivisionsPerFullMeasure =
    fMeasureDirectPartUplink->
      getPartDivisionsPerQuarterNote () * 4 // hence a whole note
      *
    time->getBeatsNumber ()
      /
    time->getBeatsValue ();

  if (gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      " has " <<
      singularOrPlural (
        fMeasureDivisionsPerFullMeasure,
        "divisions per full measure",
        "division per full measure") <<
      endl;
}

void msrMeasure::appendClefToMeasure (S_msrClef clef)
{
  // append it to the measure elements list
  fMeasureElementsList.push_back (clef);
}

void msrMeasure::appendKeyToMeasure (S_msrKey key)
{
  // append it to the measure elements list
  fMeasureElementsList.push_back (key);
}

void msrMeasure::appendTimeToMeasure (S_msrTime time)
{
  // append it to the measure elements list
  fMeasureElementsList.push_back (time);
}

void msrMeasure::appendTransposeToMeasure (
  S_msrTranspose transpose)
{
  // append it to the measure elements list
  fMeasureElementsList.push_back (transpose);
}

void msrMeasure::appendBarlineToMeasure (S_msrBarline barline)
{
  // append it to the measure elements list
  fMeasureElementsList.push_back (barline);
}

void msrMeasure::prependBarlineToMeasure (S_msrBarline barline)
{
  // append it to the measure elements list
  fMeasureElementsList.push_front (barline);
}

void msrMeasure::appendSegnoToMeasure (S_msrSegno segno)
{
  // append it to the measure elements list
  fMeasureElementsList.push_back (segno);
}

void msrMeasure::appendCodaToMeasure (S_msrCoda coda)
{
  // append it to the measure elements list
  fMeasureElementsList.push_back (coda);
}

void msrMeasure::appendEyeGlassesToMeasure (
  S_msrEyeGlasses eyeGlasses)
{
  // append it to the measure elements list
  fMeasureElementsList.push_back (eyeGlasses);
}

void msrMeasure::appendPedalToMeasure (S_msrPedal pedal)
{
  // append it to the measure elements list
  fMeasureElementsList.push_back (pedal);
}

void msrMeasure::appendBarCheckToMeasure (S_msrBarCheck barCheck)
{
  // append it to the measure elements list
  fMeasureElementsList.push_back (barCheck);
}

void msrMeasure::appendNoteToMeasure (S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

  if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Appending note '" << note->noteAsShortString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;

  // populate measure uplink
  note->setNoteMeasureUplink (this);

  // register note measure number
  note->setNoteMeasureNumber (fMeasureNumber);
  
  // register note measure position
  int noteMeasurePosition = fMeasurePosition; // for harmony voice
  
  note->setNotePositionInMeasure (noteMeasurePosition);
  
  // fetch note sounding divisions
  int noteSoundingDivisions =
    note->getNoteSoundingDivisions ();

  // account for note duration in measure position
  setMeasurePosition (
    inputLineNumber,
    fMeasurePosition + noteSoundingDivisions);

  // update part measure position high tide if need be
  fMeasureDirectPartUplink->
    updatePartMeasurePositionHighTide (
      inputLineNumber, fMeasurePosition);

  // determine if the note occupies a full measure
  if (noteSoundingDivisions == fMeasureDivisionsPerFullMeasure)
    note->setNoteOccupiesAFullMeasure ();

  // append the note to the measure elements list
// JMI  // only now to make it possible to remove it afterwards
  // if it happens to be the first note of a chord
  fMeasureElementsList.push_back (note);

  // fetch part harmony voice
  S_msrVoice
    partHarmonyVoice =
      fMeasureDirectPartUplink->
        getPartHarmonyVoice ();

  // fetch part harmony supplier voice
  S_msrVoice
    partHarmoniesSupplierVoice =
      fMeasureDirectPartUplink->
        getPartHarmoniesSupplierVoice ();

  // fetch note harmony
  S_msrHarmony
    noteHarmony =
      note->getNoteHarmony ();
      
  // don't handle the note harmony here,
  // this has been done after harmony::create ()
  if (! noteHarmony) {
    if (partHarmoniesSupplierVoice) {
      if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceMeasures)
        cerr << idtr <<
          "fMeasureVoiceDirectUplink = \"" <<
          fMeasureVoiceDirectUplink->getVoiceName () <<
          "\"" <<
          endl <<
          "partHarmoniesSupplierVoice = \"" <<
          partHarmoniesSupplierVoice->getVoiceName () <<
          "\"" <<
          endl;

/* JMI
      // bring harmony voice to the same measure position
      partHarmonyVoice->
        bringVoiceToMeasurePosition (
          inputLineNumber,
          noteMeasurePosition);
*/

      // is fMeasureVoiceDirectUplink the part harmonies suppplier voice?
      if (
        fMeasureVoiceDirectUplink
          ==
        partHarmoniesSupplierVoice) {
        // yes, create a skip note of the same duration as the note
        S_msrNote
          skipNote =
            msrNote::createSkipNote (
              inputLineNumber,
              fMeasureDirectPartUplink,
              noteSoundingDivisions,
              note->getNoteDotsNumber (),
              partHarmonyVoice->
                getVoiceStaffUplink ()->
                  getStaffNumber (),
              partHarmonyVoice->
                getExternalVoiceNumber ());
  
        // append the skip to the part harmony voice
        if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceMeasures)
          cerr << idtr <<
            "Appending skip '" << skipNote->noteAsShortString () <<
            "' to measure '" << fMeasureNumber <<
            "' in harmony voice \"" <<
            partHarmonyVoice->getVoiceName () <<
            "\"" <<
            endl;

        partHarmonyVoice->
          appendNoteToVoice (skipNote);
      }
    }
  }

  // register note as the last one in this measure
  fMeasureLastHandledNote = note;
}

void msrMeasure::appendNoteToMeasureClone (S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

  if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Appending note '" << note->noteAsShortString () <<
      "' to measure clone '" << fMeasureNumber <<
      "' in voice clone \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;

    /* JMI
  if (appendMeasureIfOverflow (inputLineNumber)) {
    // a new measure has been appended to the segment
    // append note to it via the segment
    fMeasureSegmentUplink->
      appendNoteToSegment (note);
  }

  else {
  */
    // regular insertion in current measure
    
    // populate measure uplink
    note->setNoteMeasureUplink (this);

    // register note measure number
    note->setNoteMeasureNumber (fMeasureNumber);
    
    // register note measure position
    int noteMeasurePosition = fMeasurePosition; // for harmony voice
    
    note->setNotePositionInMeasure (noteMeasurePosition);
    
    // fetch note divisions
    int noteSoundingDivisions =
      note->getNoteSoundingDivisions ();

    // account for note duration in measure position
    setMeasurePosition (
      inputLineNumber, fMeasurePosition + noteSoundingDivisions);
  
    // update part measure position high tide if need be
    fMeasureDirectPartUplink->
      updatePartMeasurePositionHighTide (
        inputLineNumber, fMeasurePosition);
  
    // determine if the note occupies a full measure
    if (noteSoundingDivisions == fMeasureDivisionsPerFullMeasure)
      note->setNoteOccupiesAFullMeasure ();
  
    // append the note to the measure elements list
  // JMI  // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord
    fMeasureElementsList.push_back (note);

    // fetch part harmony voice
    S_msrVoice
      partHarmonyVoice =
        fMeasureDirectPartUplink->
          getPartHarmonyVoice ();

    // fetch note harmony
    S_msrHarmony
      noteHarmony =
        note->getNoteHarmony ();


   /*     
    if (noteHarmony) {
      // append the harmony to the harmony voice
      if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceMeasures)
        cerr << idtr <<
          "Appending harmony '" << noteHarmony->harmonyAsString () <<
          "' to measure '" << fMeasureNumber <<
          "' in harmony voice \"" <<
          partHarmonyVoice->getVoiceName () <<
          "\"" <<
          endl;

      partHarmonyVoice->
        appendHarmonyToVoice (
          noteHarmony);
    }

    else {
      // is fMeasureVoiceDirectUplink the part harmonies suppplier voice?
      if (
        fMeasureVoiceDirectUplink
          ==
        fMeasureDirectPartUplink->getPartHarmoniesSupplierVoice ()) {
        // yes, create a skip note of the same duration as the note
        S_msrNote
          skipNote =
            msrNote::createSkipNote (
              inputLineNumber,
              fMeasureDirectPartUplink,
              noteSoundingDivisions,
              note->getNoteDotsNumber (),
              partHarmonyVoice->
                getVoiceStaffUplink ()->
                  getStaffNumber (),
              partHarmonyVoice->
                getExternalVoiceNumber ());
  
        // append the skip to the part harmony voice
        if (gGeneralOptions->fTraceNotes || gGeneralOptions->fTraceMeasures)
          cerr << idtr <<
            "Appending skip '" << skipNote->noteAsShortString () <<
            "' to measure '" << fMeasureNumber <<
            "' in harmony voice \"" <<
            partHarmonyVoice->getVoiceName () <<
            "\"" <<
            endl;

        partHarmonyVoice->
          appendNoteToVoice (skipNote);
      }
    }
*/

    // register note as the last one in this measure
    fMeasureLastHandledNote = note;
 // JMI }
}

void msrMeasure::appendDoubleTremoloToMeasure (
  S_msrDoubleTremolo doubleTremolo)
{
  int inputLineNumber =
    doubleTremolo->getInputLineNumber ();
  
  // populate measure uplink
  doubleTremolo->setDoubleTremoloMeasureUplink (this);

  if (gGeneralOptions->fTraceTremolos || gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Appending double tremolo '" <<
      doubleTremolo->doubleTremoloAsShortString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;

  // register doubleTremolo measure number
  doubleTremolo->
    setDoubleTremoloMeasureNumber (fMeasureNumber);
  
  // register doubleTremolo measure position
  doubleTremolo->
    setDoubleTremoloPositionInMeasure (
      fMeasurePosition);

  // copy measure number to first note, that was created beforehand
  doubleTremolo->
    setDoubleTremoloMeasureNumber (
      fMeasureNumber);
  
  // copy measure position to first note, that was created beforehand
  doubleTremolo->
    setDoubleTremoloPositionInMeasure (
      fMeasurePosition);

  // fetch doubleTremolo divisions
  int doubleTremoloSoundingDivisions =
    doubleTremolo->getDoubleTremoloSoundingDivisions ();
    
  // account for doubleTremolo duration in measure position
  setMeasurePosition (
    inputLineNumber,
    fMeasurePosition + doubleTremoloSoundingDivisions);

  // update part measure position high tide if need be
  fMeasureDirectPartUplink->
    updatePartMeasurePositionHighTide (
      inputLineNumber, fMeasurePosition);

  // determine if the doubleTremolo occupies a full measure
// XXL  JMI  if (doubleTremoloSoundingDivisions == fMeasureDivisionsPerWholeMeasure)
    // doubleTremolo->setDoubleTremoloOccupiesAFullMeasure ();

  // append the doubleTremolo to the measure elements list
  fMeasureElementsList.push_back (doubleTremolo);

  // bring harmony voice to the same measure position
  fMeasureDirectPartUplink->
    getPartHarmonyVoice ()->
      bringVoiceToMeasurePosition (
        inputLineNumber,
        fMeasurePosition);
}

void msrMeasure::appendMeasureRepeatToMeasure (
  S_msrMeasureRepeat measureRepeat)
{
  int inputLineNumber =
    measureRepeat->getInputLineNumber ();
    
/* JMI  
  // populate measure uplink
  measureRepeat->setMeasureRepeatMeasureUplink (this);
*/

  if (gGeneralOptions->fTraceRepeats || gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Appending multiple rest '" <<
      measureRepeat->measureRepeatAsString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;

/* JMI
  // register measureRepeat measure number
  measureRepeat->
    setmeasureRepeatMeasureNumber (fMeasureNumber);
  
  // register measureRepeat measure position
  measureRepeat->
    setmeasureRepeatPositionInMeasure (
      fMeasurePosition);

  // copy measure number to first note, that was created beforehand
  measureRepeat->
    setmeasureRepeatMeasureNumber (
      fMeasureNumber);
  
  // copy measure position to first note, that was created beforehand
  measureRepeat->
    setmeasureRepeatPositionInMeasure (
      fMeasurePosition);

  // fetch measureRepeat divisions
  int measureRepeatSoundingDivisions =
    measureRepeat->getmeasureRepeatSoundingDivisions ();
    
  // account for measureRepeat duration in measure position
  setMeasurePosition (
    inputLineNumber,
    fMeasurePosition + measureRepeatSoundingDivisions);

  // update part measure position high tide if need be
  fMeasureDirectPartUplink->
    updatePartMeasurePositionHighTide (
      inputLineNumber, fMeasurePosition);

  // determine if the measureRepeat occupies a full measure
// XXL  JMI  if (measureRepeatSoundingDivisions == fMeasureDivisionsPerWholeMeasure)
    // measureRepeat->setmeasureRepeatOccupiesAFullMeasure ();
*/

  // append the measureRepeat to the measure elements list
  fMeasureElementsList.push_back (measureRepeat);

/* JMI
  // bring harmony voice to the same measure position
  fMeasureDirectPartUplink->
    getPartHarmonyVoice ()->
      bringVoiceToMeasurePosition (
        inputLineNumber,
        fMeasurePosition);
        */
}

void msrMeasure::appendMultipleRestToMeasure (
  S_msrMultipleRest multipleRest)
{
  int inputLineNumber =
    multipleRest->getInputLineNumber ();
    
  /* JMI  
    // populate measure uplink
    multipleRest->setMultipleRestMeasureUplink (this);
*/

  if (gGeneralOptions->fTraceRepeats || gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Appending multiple rest '" <<
      multipleRest->multipleRestAsString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;

/* JMI
  // register multipleRest measure number
  multipleRest->
    setMultipleRestMeasureNumber (fMeasureNumber);
  
  // register multipleRest measure position
  multipleRest->
    setMultipleRestPositionInMeasure (
      fMeasurePosition);

  // copy measure number to first note, that was created beforehand
  multipleRest->
    setMultipleRestMeasureNumber (
      fMeasureNumber);
  
  // copy measure position to first note, that was created beforehand
  multipleRest->
    setMultipleRestPositionInMeasure (
      fMeasurePosition);

  // fetch multipleRest divisions
  int multipleRestSoundingDivisions =
    multipleRest->getmultipleRestSoundingDivisions ();
    
  // account for multipleRest duration in measure position
  setMeasurePosition (
    inputLineNumber,
    fMeasurePosition + multipleRestSoundingDivisions);

  // update part measure position high tide if need be
  fMeasureDirectPartUplink->
    updatePartMeasurePositionHighTide (
      inputLineNumber, fMeasurePosition);

  // determine if the multipleRest occupies a full measure
// XXL  JMI  if (multipleRestSoundingDivisions == fMeasureDivisionsPerWholeMeasure)
    // multipleRest->setmultipleRestOccupiesAFullMeasure ();
*/

  // append the multipleRest to the measure elements list
  fMeasureElementsList.push_back (multipleRest);

/* JMI
  // bring harmony voice to the same measure position
  fMeasureDirectPartUplink->
    getPartHarmonyVoice ()->
      bringVoiceToMeasurePosition (
        inputLineNumber,
        fMeasurePosition);
        */
}

void msrMeasure::appendChordToMeasure (S_msrChord chord) // JMI XXL
{
  int inputLineNumber =
    chord->getInputLineNumber ();
    
  // populate measure uplink
  chord->setChordMeasureUplink (this);

  if (gGeneralOptions->fTraceChords || gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Appending chord '" << chord->chordAsString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;

  // register chord measure number
  chord->
    setChordMeasureNumber (fMeasureNumber);
  
  // register chord measure position
  chord->
    setChordPositionInMeasure (
      fMeasurePosition);

  // copy measure number to first note, that was created beforehand
  chord->
    setChordFirstNoteMeasureNumber (
      fMeasureNumber);
  
  // copy measure position to first note, that was created beforehand
  chord->
    setChordFirstNotePositionInMeasure (
      fMeasurePosition);

  // fetch chord divisions
  int chordSoundingDivisions =
    chord->getChordSoundingDivisions ();
    
  // account for chord duration in measure position
  setMeasurePosition (
    inputLineNumber,
    fMeasurePosition + chordSoundingDivisions);

  // update part measure position high tide if need be
  fMeasureDirectPartUplink->
    updatePartMeasurePositionHighTide (
      inputLineNumber, fMeasurePosition);

  // determine if the chord occupies a full measure
// XXL  JMI  if (chordSoundingDivisions == fMeasureDivisionsPerWholeMeasure)
    // chord->setChordOccupiesAFullMeasure ();

  // append the chord to the measure elements list
  fMeasureElementsList.push_back (chord);

  // bring harmony voice to the same measure position
  fMeasureDirectPartUplink->
    getPartHarmonyVoice ()->
      bringVoiceToMeasurePosition (
        inputLineNumber,
        fMeasurePosition);
}

void msrMeasure::appendTupletToMeasure (S_msrTuplet tuplet)
{
  int inputLineNumber =
    tuplet->getInputLineNumber ();
    
  if (gGeneralOptions->fTraceTuplets || gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Appending tuplet '" << tuplet->tupletAsString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;

  // populate measure uplink
  tuplet->setTupletMeasureUplink (this);

  // register tuplet measure number
  tuplet->
    setTupletMeasureNumber (fMeasureNumber);
  
  // register tuplet measure position
  int dummy = // JMI
    tuplet->
      setTupletPositionInMeasure (fMeasurePosition);

/* JMI
  // copy measure number to first note, that was created beforehand
  tuplet->
    setTupletFirstNoteMeasureNumber (fMeasureNumber);
  
  // copy measure position to first note, that was created beforehand
  tuplet->
    setTupletFirstNotePositionInMeasure (fMeasurePosition);
 */
  
  // fetch tuplet divisions
  int tupletSoundingDivisions =
    tuplet->getTupletSoundingDivisions ();
    
  // account for tuplet duration in measure position
  setMeasurePosition (
    inputLineNumber,
    fMeasurePosition + tupletSoundingDivisions);

  // update part measure position high tide if need be
  fMeasureDirectPartUplink->
    updatePartMeasurePositionHighTide (
      inputLineNumber, fMeasurePosition);

/* JMI
  // set tuplet members' displayed divisions
  tuplet->
    applyDisplayFactorToTupletMembers ();
    */

  // determine if the tuplet occupies a full measure
// XXL    if (tupletSoundingDivisions == fMeasureDivisionsPerWholeMeasure)
    // tuplet->setTupletOccupiesAFullMeasure ();

  // append the tuplet to the measure elements list
  fMeasureElementsList.push_back (tuplet);

    // bring harmony voice to the same measure position
    fMeasureDirectPartUplink->
      getPartHarmonyVoice ()->
        bringVoiceToMeasurePosition (
          inputLineNumber,
          fMeasurePosition);
}

void msrMeasure::appendHarmonyToMeasure (S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();
    
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Appending harmony '" << harmony->harmonyAsString () <<
      "' to measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      ", fMeasurePosition = " << fMeasurePosition <<
      endl;

  // populate measure uplink
// JMI   harmony->setHarmonyMeasureUplink (this);

  // register harmony measure number
//   harmony->
// JMI     setHarmonyMeasureNumber (fMeasureNumber);
  
  // register harmony measure position
//  int dummy = // JMI
 //   harmony->
  //    setHarmonyPositionInMeasure (fMeasurePosition);


/* JMI
  // fetch voice
  S_msrVoice
    voice =
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ();

  // register voice as part harmonies supplied
  // this will abort if another voice is already supplying harmonies
  fMeasureDirectPartUplink->
    setPartHarmoniesSupplierVoice (
      voice);

      */

      
  // fetch harmony divisions
  int harmonySoundingDivisions =
    harmony->getHarmonySoundingDivisions ();
    
//* JMI FOO
/*
  // append a skip syllable of the same duration to the part harmony voice
  S_msrNote
    skip =
        msrNote::createSkipNote (
            inputLineNumber,
            fMeasureDirectPartUplink,
            harmonySoundingDivisions,
            voice->
              getVoiceStaffUplink ()->getStaffNumber (),
            voice->
              getExternalVoiceNumber ());
  
    fMeasureDirectPartUplink->
      getPartHarmonyVoice ()->
        appendNoteToVoice (skip);
  */

    // account for harmony duration in measure position
    setMeasurePosition (
      inputLineNumber,
      fMeasurePosition + harmonySoundingDivisions);
  
    // update part measure position high tide if need be
    fMeasureDirectPartUplink->
      updatePartMeasurePositionHighTide (
        inputLineNumber, fMeasurePosition);
    
    // append the harmony to the measure elements list
    fMeasureElementsList.push_back (harmony);
}

void msrMeasure::appendHarmonyToMeasureClone (S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();
    
  // regular insertion in current measure
  
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Appending harmony '" << harmony->harmonyAsString () <<
      "' to measure clone '" << fMeasureNumber <<
      "' in voice clone \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      ", fMeasurePosition = " << fMeasurePosition <<
      endl;
      
  // fetch harmony divisions
  int harmonySoundingDivisions =
    harmony->getHarmonySoundingDivisions ();
    
  // account for harmony duration in measure position
  setMeasurePosition (
    inputLineNumber,
    fMeasurePosition + harmonySoundingDivisions);

  // update part measure position high tide if need be
  fMeasureDirectPartUplink->
    updatePartMeasurePositionHighTide (
      inputLineNumber, fMeasurePosition);
  
  // append the harmony to the measure elements list
  fMeasureElementsList.push_back (harmony);
}

void msrMeasure::bringMeasureToMeasurePosition (
  int inputLineNumber,
  int measurePosition)
{
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceDivisions)
    cerr << idtr <<
      "Bringing measure position for measure '" <<
      fMeasureNumber <<
      "' in segment " <<
      fMeasureSegmentUplink->getSegmentAbsoluteNumber () << 
      " from " << fMeasurePosition <<
      " to " << measurePosition <<
      ", line " << inputLineNumber <<
      endl;

  if (fMeasurePosition < measurePosition) {
    // appending a skip to this measure to reach measurePosition
    int skipDuration =
      measurePosition - fMeasurePosition;
    
    // fetch the voice
    S_msrVoice
      voice =
        fMeasureSegmentUplink->
          getSegmentVoiceUplink ();
      
    // create the skip
    S_msrNote
      skip =
        msrNote::createSkipNote (
          inputLineNumber,
          fMeasureDirectPartUplink,
          skipDuration,
          0, // JMI ???
          voice->
            getVoiceStaffUplink ()->getStaffNumber (),
          voice->
            getExternalVoiceNumber ());

    // does the skip occupy a full measure?
    if (skipDuration == fMeasureDivisionsPerFullMeasure)
      skip->
        setNoteOccupiesAFullMeasure ();
  
    // register skip's measure position
    skip->
      setNotePositionInMeasure (
        fMeasurePosition);
           
    if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceDivisions)
      cerr << idtr <<
       "Appending " << skip->noteAsString () <<
       " (" << skipDuration <<
       " divs) to bring voice \"" << voice->getVoiceName () <<
       "\" measure '" << fMeasureNumber << "'" <<
       " from position " << fMeasurePosition <<
       " to position '" << measurePosition << "'" <<
        ", skipDuration = " << skipDuration <<
       endl;

    // append the skip to the measure elements list
    // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord
    appendNoteToMeasure (skip);

/*
    // account for skip duration in measure position
    setMeasurePosition (
      inputLineNumber, fMeasurePosition + skipDuration);
*/
  }
}

void msrMeasure::appendGraceNotesToMeasure (
  S_msrGraceNotes graceNotes)
{
  fMeasureElementsList.push_back (graceNotes);
}
  
void msrMeasure::prependGraceNotesToMeasure (
  S_msrGraceNotes graceNotes)
{
  // in order to work around LilyPond issue 34,
  // we need to insert the skip grace notes
  // after clef, key and time signature if any

  for (
    list<S_msrElement>::const_iterator i = fMeasureElementsList.begin();
    i != fMeasureElementsList.end();
    i++ ) {

    if (
      S_msrClef clef = dynamic_cast<msrClef*>(&(**i))
      ) {
    }
  
    else if (
      S_msrKey key = dynamic_cast<msrKey*>(&(**i))
      ) {
    }
    
    else if (
      S_msrTime time = dynamic_cast<msrTime*>(&(**i))
      ) {
    }
    
    else {
       // insert graceNotes before (*i) in the list
      fMeasureElementsList.insert (
        i, graceNotes);

      break;
    }
  } // for
}
  
void msrMeasure::appendAfterGraceNotesToMeasure (
  S_msrAfterGraceNotes afterGraceNotes)
{
  fMeasureElementsList.push_back (afterGraceNotes);
}
  
void msrMeasure::prependAfterGraceNotesToMeasure (
  S_msrAfterGraceNotes afterGraceNotes)
{
  // in order to work around LilyPond issue 34,
  // we need to insert the skip after grace notes
  // after clef, key and time signature if any

  for (
    list<S_msrElement>::const_iterator i = fMeasureElementsList.begin();
    i != fMeasureElementsList.end();
    i++ ) {

    if (
      S_msrClef clef = dynamic_cast<msrClef*>(&(**i))
      ) {
    }
  
    else if (
      S_msrKey key = dynamic_cast<msrKey*>(&(**i))
      ) {
    }
    
    else if (
      S_msrTime time = dynamic_cast<msrTime*>(&(**i))
      ) {
    }
    
    else {
       // insert afterGraceNotes before (*i) in the list
      fMeasureElementsList.insert (
        i, afterGraceNotes);

      break;
    }
  } // for
}

void msrMeasure::appendTempoToMeasure (
  S_msrTempo tempo)
{
  fMeasureElementsList.push_back (tempo);
}

void msrMeasure::appendRehearsalToMeasure (
  S_msrRehearsal rehearsal)
{
  fMeasureElementsList.push_back (rehearsal);
}

void msrMeasure::appendOctaveShiftToMeasure (
  S_msrOctaveShift octaveShift)
{
  fMeasureElementsList.push_back (octaveShift);
}

void msrMeasure::appendBreakToMeasure (S_msrBreak break_)
{
  fMeasureElementsList.push_back (break_);
}

void msrMeasure::appendStaffDetailsToMeasure (
  S_msrStaffDetails staffDetails)
{
  fMeasureElementsList.push_back (staffDetails);
}

void msrMeasure::appendBarNumberCheckToMeasure (
  S_msrBarNumberCheck barNumberCheck)
{
  fMeasureElementsList.push_back (barNumberCheck);
}

void msrMeasure::prependOtherElementToMeasure (S_msrElement elem)
{
  fMeasureElementsList.push_front (elem); // JMI
}

void msrMeasure::appendOtherElementToMeasure  (S_msrElement elem)
{
  fMeasureElementsList.push_back (elem);
}

void msrMeasure::removeNoteFromMeasure (
  int       inputLineNumber,
  S_msrNote note)
{  
  if (gGeneralOptions->fTraceChords || gGeneralOptions->fTraceMeasures) {
    cerr << idtr <<
      "Removing note:" <<
      endl;

    idtr++;
    cerr <<
      idtr <<
      note->noteAsShortString ();
    idtr--;
    
    cerr <<
      endl <<
      idtr <<
      " from measure '" << fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"," <<
      endl;

    idtr++;
    cerr <<
      idtr <<
      "fMeasureLastHandledNote:" <<
      endl <<
      idtr <<
      fMeasureLastHandledNote <<
      endl;
    idtr--;
  }

  for (
    list<S_msrElement>::const_iterator i=fMeasureElementsList.begin();
    i!=fMeasureElementsList.end();
    ++i) {
    if ((*i) == note) {
      // found note, erase it
      fMeasureElementsList.erase (i);
      
      // dont update position in measure: // JMI
      // the chord will replace its first note

      // update position in measure
      setMeasurePosition (
        inputLineNumber,
        fMeasurePosition
          -
        fMeasureLastHandledNote->getNoteSoundingDivisions ());

      // return from function
      return;
    }
  } // for
  
  msrInternalError (
    inputLineNumber,
    "cannot remove note " <<
    note <<
    " from measure " << fMeasureNumber <<
    "' in voice \"" <<
    fMeasureSegmentUplink->
      getSegmentVoiceUplink ()->
        getVoiceName () <<
    "\"," <<
    " since it has not been found");
}

/* JMI
S_msrElement msrMeasure::removeLastElementFromMeasure (
  int inputLineNumber)
{
  // fetching measure last element
  S_msrElement
    measureLastElement =
      fMeasureElementsList.back ();
      
  if (gGeneralOptions->fDebug) {
//  if (gGeneralOptions->fDebug) {
    cerr << idtr <<
      "% --> removing last element:" <<
      endl;

    idtr++;
    cerr <<
      idtr <<
      measureLastElement;
    idtr--;
    
    cerr <<
      endl <<
      idtr <<
      " from measure '" <<
      fMeasureNumber <<
      "' in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"," <<
      endl;

    idtr++;
    cerr <<
      idtr <<
      "fMeasureLastHandledNote:" <<
      endl <<
      idtr <<
      fMeasureLastHandledNote <<
      endl;
    idtr--;
  }
  
  if (fMeasureElementsList.size ()) {

    if (fMeasureLastHandledNote) {
      // there's at least a note in the meastuer
      
      if (fMeasureLastHandledNote == measureLastElement) {
        // remove last element
        fMeasureElementsList.pop_back ();

        // update position in measure
        fMeasurePosition -=
          fMeasureLastHandledNote->getNoteSoundingDivisions ();
/ *
// JMI
        // set note's measure position, needed for chord handling
        fMeasureLastHandledNote->
          setNotePositionInMeasure (fMeasurePosition);
* /
      }

      else {
        msrInternalError (
          inputLineNumber,
          "cannot removeLastElementFromMeasure () since "
          "fMeasureLastHandledNote is not the last element");
        }
    }

    else {
      msrInternalError (
        inputLineNumber,
        "cannot removeLastElementFromMeasure () since "
        "fMeasureLastHandledNote is null");
    }
  }
  
  else {
    msrInternalError (
      inputLineNumber,
      "cannot removeLastElementFromMeasure () "
      "since fMeasureElementsList is empty");
  }

  return measureLastElement;
}
*/

/*
bool msrMeasure::checkForIncompleteMeasure (
  int                        inputLineNumber,
  msrMeasure::msrMeasureKind measureKind)
{
  if (gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Checking for incompleteness of measure  " <<
      fMeasureNumber <<
      ", line " << inputLineNumber <<
      ", in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;

  if (gGeneralOptions->fTraceMeasures) {
    idtr++;

    const int fieldWidth = 31;
    
    cerr << left <<
      idtr <<
        setw(fieldWidth) << "MeasureDivisionsPerFullMeasure" << " = " <<
        fMeasureDivisionsPerFullMeasure <<
        endl <<
      idtr <<
        setw(fieldWidth) << "MeasurePosition" << " = " <<
        fMeasurePosition <<
        endl <<
      idtr <<
        setw(fieldWidth) << "MeasureLength" << " = " <<
        getMeasureLength () <<
        endl;

    idtr--;
  }

  bool measureIsIncomplete =
    // positions start at 1
    fMeasurePosition <= fMeasureDivisionsPerFullMeasure;

  if (measureIsIncomplete) {    
    if (gGeneralOptions->fTraceMeasures) {
      cerr <<
        idtr <<
          "Measure " << fMeasureNumber <<
          " of segment " <<
          fMeasureSegmentUplink->segmentAsString () <<
          " in voice \"" <<
          fMeasureSegmentUplink->
            getSegmentVoiceUplink ()->
              getVoiceName () <<
          "\"" <<
          " is " <<
          string(
            fMeasurePosition == 1
              ? "incomplete"
              : "empty") <<
          ", line " << inputLineNumber <<
          ": position = " << fMeasurePosition <<
          ", divisionsPerWholeMeasure = " <<
          fMeasureDivisionsPerFullMeasure <<
        endl;
    }
    
    // set measure kind
    setMeasureKind (
      measureKind);
  }

  return measureIsIncomplete;
}

bool msrMeasure::checkForOverfullMeasure (
  int inputLineNumber)
{
  if (gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Checking for overfull measure  " <<
      fMeasureNumber <<
      ", line " << inputLineNumber <<
      ", in voice \"" <<
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ()->
          getVoiceName () <<
      "\"" <<
      endl;

  if (gGeneralOptions->fTraceMeasures) {
    idtr++;

    const int fieldWidth = 31;
    
    cerr <<
      idtr <<
        setw(fieldWidth) << "MeasureDivisionsPerFullMeasure" << " = " <<
        fMeasureDivisionsPerFullMeasure <<
        endl <<
      idtr <<
        setw(fieldWidth) << "MeasurePosition" << " = " <<
        fMeasurePosition <<
        endl <<
      idtr <<
        setw(fieldWidth) << "MeasureLength" << " = " <<
        getMeasureLength () <<
        endl;

    idtr--;
  }

  bool measureIsOverfull =
    // positions start at 1
    fMeasurePosition > fMeasureDivisionsPerFullMeasure + 1;

  if (measureIsOverfull) {    
    if (gGeneralOptions->fTraceMeasures) {
      cerr <<
        idtr <<
          "Measure " << fMeasureNumber <<
          ", in segment " <<
          fMeasureSegmentUplink->segmentAsString () <<
          " in voice \"" <<
          fMeasureSegmentUplink->
            getSegmentVoiceUplink ()->
              getVoiceName () <<
          "\"" <<
          " is overfull" <<
          ", line " << inputLineNumber <<
          ": position = " << fMeasurePosition <<
          ", divisionsPerWholeMeasure = " <<
          fMeasureDivisionsPerFullMeasure <<
        endl;
    }
    
    // set measure kind
    setMeasureKind (
      kOverfullMeasureKind);
  }

  return measureIsOverfull;
}
*/

void msrMeasure::finalizeMeasure (
  int     inputLineNumber)
{
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ();
    
  // fetch the part measure position high tide
  int partMeasurePositionHighTide =
    fMeasureDirectPartUplink->
      getPartMeasurePositionHighTide ();
    
  if (gGeneralOptions->fTraceMeasures) {
    cerr <<
      idtr <<
        "Finalizing measure " << fMeasureNumber <<
        " in voice \"" << voice->getVoiceName () <<
        "\", line " << inputLineNumber <<
        endl;

    idtr++;

    cerr <<
      idtr <<
        "fMeasurePosition = " << fMeasurePosition <<
        endl <<
      idtr <<
        "partMeasurePositionHighTide = " <<
        partMeasurePositionHighTide <<
        endl;
        
    idtr--;
  }

  if (false && fMeasurePosition < partMeasurePositionHighTide) {
    // appending a skip to this measure to reach measurePosition
    int skipDuration =
      partMeasurePositionHighTide - fMeasurePosition;
    /* JMI
      partMeasurePositionHighTide > fMeasureDivisionsPerFullMeasure // + 1 // JMI ???
        ? partMeasurePositionHighTide - fMeasurePosition
        : fMeasureDivisionsPerFullMeasure - fMeasurePosition;
        */
    
    // create the skip
    S_msrNote
      skip =
        msrNote::createSkipNote (
          inputLineNumber,
          fMeasureDirectPartUplink,
          skipDuration,
          0, // JMI ???
          voice->
            getVoiceStaffUplink ()->getStaffNumber (),
          voice->
            getExternalVoiceNumber ());

    // does the skip occupy a full measure?
    if (skipDuration == fMeasureDivisionsPerFullMeasure)
      skip->setNoteOccupiesAFullMeasure ();
  
    // register skip's measure position
    skip->setNotePositionInMeasure (fMeasurePosition);
           
    if (gGeneralOptions->fTraceMeasures)
      cerr << idtr <<
       "Appending '" << skip->noteAsString () <<
       " (" << skipDuration << " divs)'" <<
       " to finalize \"" << voice->getVoiceName () <<
       "\" measure: @" << fMeasureNumber << ":" << fMeasurePosition <<
       " % --> @" << fMeasureNumber <<
       ":" << partMeasurePositionHighTide <<
        ", skipDuration = " << skipDuration <<
       endl;

    // append the skip to the measure elements list
    // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord
    appendNoteToMeasure (skip);
  }

  // determine the measure kind
  // positions start at 1
  if (fMeasurePosition == fMeasureDivisionsPerFullMeasure + 1) {
    // full measure
    if (gGeneralOptions->fTraceMeasures) {
      cerr << idtr <<
      "Measure '" << fMeasureNumber <<
      "' in voice \"" << voice->getVoiceName () <<
      "\", is full" <<
      ", line " << inputLineNumber <<
      endl;
    }

    setMeasureKind (
      kFullMeasureKind);
  }
  
  else if (fMeasurePosition == 1) {
    // overfull measure
    if (gGeneralOptions->fTraceMeasures) {
      cerr << idtr <<
      "Measure '" << fMeasureNumber <<
      "' in voice \"" << voice->getVoiceName () <<
      "\", is **empty**" <<
      ", line " << inputLineNumber <<
      endl;
    }

    setMeasureKind (
      kEmptyMeasureKind);
  }
  
  else if (fMeasurePosition <= fMeasureDivisionsPerFullMeasure) {
    //  incomplete measure
    switch (fMeasureFirstInSegmentKind) {
      case msrMeasure::kMeasureFirstInSegmentYes:
        if (gGeneralOptions->fTraceMeasures) {
          cerr << idtr <<
          "Measure '" << fMeasureNumber <<
          "' in voice \"" << voice->getVoiceName () <<
          "\", is **incomplete left**" <<
          ", line " << inputLineNumber <<
          endl;
        }
    
        setMeasureKind (
          kUpbeatMeasureKind);
        break;
        
      case msrMeasure::kMeasureFirstInSegmentNo:
        if (gGeneralOptions->fTraceMeasures) {
          cerr << idtr <<
          "Measure '" << fMeasureNumber <<
          "' in voice \"" << voice->getVoiceName () <<
          "\", is **underfull**" <<
          ", line " << inputLineNumber <<
          endl;
        }
    
        setMeasureKind (
          kUnderfullMeasureKind);
        break;
    } // switch
  }

  else if (fMeasurePosition > fMeasureDivisionsPerFullMeasure + 1) {
    // overfull measure
    if (gGeneralOptions->fTraceMeasures) {
      cerr << idtr <<
      "Measure '" << fMeasureNumber <<
      "' in voice \"" << voice->getVoiceName () <<
      "\", is **overfull**" <<
      ", line " << inputLineNumber <<
      endl;
    }

    setMeasureKind (
      kOverfullMeasureKind);
  }
}

/*
void msrMeasure::finalizeUltimateMeasure (
  int inputLineNumber)
{
  // fetch the voice
  S_msrVoice
    voice =
      fMeasureSegmentUplink->
        getSegmentVoiceUplink ();
    
  // fetch the part measure position high tide
  int partMeasurePositionHighTide =
    fMeasureDirectPartUplink->
      getPartMeasurePositionHighTide ();
    
  if (gGeneralOptions->fTraceMeasures) {
    cerr <<
      idtr <<
        "Finalizing ultimate measure " << fMeasureNumber <<
        " in voice \"" << voice->getVoiceName () <<
        "\", line " << inputLineNumber <<
        endl;

    idtr++;

    const int fieldWidth = 30;
    
    cerr << left <<
      idtr <<
        setw(fieldWidth) <<
        "fMeasurePosition" << " = " <<
        fMeasurePosition <<
        endl <<
      idtr <<
        setw(fieldWidth) <<
        "MeasureDivisionsPerFullMeasure" << " = " <<
        fMeasureDivisionsPerFullMeasure <<
        endl <<
      idtr <<
        setw(fieldWidth) <<
        "partMeasurePositionHighTide" << " = " <<
        partMeasurePositionHighTide <<
        endl;
        
    idtr--;
  }

/ * JMI
  if (false && fMeasurePosition < partMeasurePositionHighTide) {
    // appending a skip to this measure to reach measurePosition // JMI ???
    int skipDuration =
      partMeasurePositionHighTide - fMeasurePosition;
    / * JMI
      partMeasurePositionHighTide > fMeasureDivisionsPerFullMeasure // + 1 // JMI ???
        ? partMeasurePositionHighTide - fMeasurePosition
        : fMeasureDivisionsPerFullMeasure - fMeasurePosition;
        * /
    
    // create the skip
    S_msrNote
      skip =
        msrNote::createSkipNote (
          inputLineNumber,
          fMeasureDirectPartUplink,
          skipDuration,
          0, // JMI ???
          voice->
            getVoiceStaffUplink ()->getStaffNumber (),
          voice->
            getExternalVoiceNumber ());

    // does the skip occupy a full measure?
    if (skipDuration == fMeasureDivisionsPerFullMeasure)
      skip->setNoteOccupiesAFullMeasure ();
  
    // register skip's measure position
    skip->setNotePositionInMeasure (fMeasurePosition);
           
    if (gGeneralOptions->fTraceMeasures)
      cerr << idtr <<
       "Appending '" << skip->noteAsString () <<
       " (" << skipDuration << " divs)'" <<
       " to finalize \"" << voice->getVoiceName () <<
       "\" measure: @" << fMeasureNumber << ":" << fMeasurePosition <<
       " % --> @" << fMeasureNumber <<
       ":" << partMeasurePositionHighTide <<
        ", skipDuration = " << skipDuration <<
       endl;

    // append the skip to the measure elements list
    // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord
    appendNoteToMeasure (skip);
  }
* /

  // determine the measure kind
  // positions start at 1
  if (fMeasurePosition == fMeasureDivisionsPerFullMeasure + 1) {
    // full measure
   if (gGeneralOptions->fTraceMeasures) {
      cerr << idtr <<
      "Measure '" << fMeasureNumber <<
      "' in voice \"" << voice->getVoiceName () <<
      "\", is full" <<
      ", line " << inputLineNumber <<
      endl;
    }

    setMeasureKind (
      kFullMeasureKind);
  }
  
  else if (fMeasurePosition == 1) {
    // empty measure
    if (gGeneralOptions->fTraceMeasures) {
      cerr << idtr <<
      "Measure '" << fMeasureNumber <<
      "' in voice \"" << voice->getVoiceName () <<
      "\", is **empty**" <<
      ", line " << inputLineNumber <<
      endl;
    }

    setMeasureKind (
      kEmptyMeasureKind);
  }

  else if (fMeasurePosition <= fMeasureDivisionsPerFullMeasure) {
    //  incomplete measure
    / * JMI
    switch (measureFirstInSegmentKind) {
      case msrMeasure::kMeasureFirstInSegmentYes:
        if (gGeneralOptions->fTraceMeasures) {
          cerr << idtr <<
          "Measure '" << fMeasureNumber <<
          "' in voice \"" << voice->getVoiceName () <<
          "\", is **incomplete left**" <<
          ", line " << inputLineNumber <<
          endl;
        }
    
        setMeasureKind (
          kUpbeatMeasureKind);
        break;
        
      case msrMeasure::kMeasureFirstInSegmentNo:
        if (gGeneralOptions->fTraceMeasures) {
          cerr << idtr <<
          "Measure '" << fMeasureNumber <<
          "' in voice \"" << voice->getVoiceName () <<
          "\", is **incomplete right**" <<
          ", line " << inputLineNumber <<
          endl;
        }
    
        setMeasureKind (
          kUnderfullMeasureKind);
        break;
    } // switch
    * /
  }

  else if (fMeasurePosition > fMeasureDivisionsPerFullMeasure + 1) {
    // overfull measure
    if (gGeneralOptions->fTraceMeasures) {
      cerr << idtr <<
      "Measure '" << fMeasureNumber <<
      "' in voice \"" << voice->getVoiceName () <<
      "\", is **overfull**" <<
      ", line " << inputLineNumber <<
      endl;
    }

    setMeasureKind (
      kOverfullMeasureKind);
  }
}
*/

void msrMeasure::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrMeasure::acceptIn()" <<
      endl;
      
  if (visitor<S_msrMeasure>*
    p =
      dynamic_cast<visitor<S_msrMeasure>*> (v)) {
        S_msrMeasure elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrMeasure::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrMeasure::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrMeasure::acceptOut()" <<
      endl;

  if (visitor<S_msrMeasure>*
    p =
      dynamic_cast<visitor<S_msrMeasure>*> (v)) {
        S_msrMeasure elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrMeasure::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrMeasure::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrMeasure::browseData()" <<
      endl;

  for (
    list<S_msrElement>::const_iterator i = fMeasureElementsList.begin();
    i != fMeasureElementsList.end();
    i++) {
    // browse the element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% <== msrMeasure::browseData()" <<
      endl;
}

ostream& operator<< (ostream& os, const S_msrMeasure& elt)
{
  elt->print (os);
  return os;
}

string msrMeasure::getMeasureLengthAsString () const
{
  string result;
  string errorMessage;

  int
    measureLength =
      this->getMeasureLength (); 
  
  if (false && gGeneralOptions->fTraceMeasures)
    cerr <<
      endl <<
      idtr <<
        "% --> measure " << fMeasureNumber <<
        ", measureLength = " << measureLength <<
        ", fMeasureDivisionsPerFullMeasure = " <<
        fMeasureDivisionsPerFullMeasure <<
      endl;

  if (measureLength > 0) {
    result =
      fMeasureDirectPartUplink->
        divisionsAsMsrString (
          fInputLineNumber,
          measureLength);
  }
  
  else
    result = "0";

  if (false && gGeneralOptions->fTraceMeasures)
    cerr <<
      endl <<
      idtr <<
      "%% --> result = " << result <<
      endl;

  return result;
}

string msrMeasure::measureKindAsString (
  msrMeasureKind measureKind)
{
  string result;

  switch (measureKind) {
    case msrMeasure::kUnknownMeasureKind:
      result = "**unkwnown kind**";
      break;
    case msrMeasure::kFullMeasureKind:
      result = "full";
      break;
    case msrMeasure::kUpbeatMeasureKind:
      result = "**upbeat**";
      break;
    case msrMeasure::kUnderfullMeasureKind:
      result = "**under full**";
      break;
    case msrMeasure::kOverfullMeasureKind:
      result = "**over full**";
      break;
    case msrMeasure::kEmptyMeasureKind:
      result = "empty";
      break;
  } // switch

  return result;
}

string msrMeasure::measureImplicitKindAsString (
  msrMeasureImplicitKind measureImplicitKind)
{
  string result;

  switch (measureImplicitKind) {
    case msrMeasure::kMeasureImplicitYes:
      result = "implicit";
      break;
    case msrMeasure::kMeasureImplicitNo:
      result = "not implicit";
      break;
  } // switch

  return result;
}

string msrMeasure::measureFirstInSegmentKindAsString (
    msrMeasureFirstInSegmentKind measureFirstInSegmentKind
)
{
  string result;

  switch (measureFirstInSegmentKind) {
    case msrMeasure::kMeasureFirstInSegmentYes:
      result = "first in segment";
      break;
    case msrMeasure::kMeasureFirstInSegmentNo:
      result = "not first in segment";
      break;
  } // switch

  return result;
}

string msrMeasure::getMeasureKindAsString () const
{
  return
    measureKindAsString (fMeasureKind);
}

void msrMeasure::print (ostream& os)
{
  /* JMI
  // fetch the part measure position high tide
  int partMeasurePositionHighTide =
    fMeasureDirectPartUplink->
      getPartMeasurePositionHighTide ();
   */

  os <<
    endl <<
    idtr <<
      "Measure " << fMeasureNumber;

  if (fMeasureCurrentTime)
    os <<
      ", " << fMeasureCurrentTime->timeAsShortString ();
  else
    os <<
      ", time unknown";

  os <<
    ", " << getMeasureKindAsString () <<
    ", " <<
    msrMeasure::measureFirstInSegmentKindAsString (
      fMeasureFirstInSegmentKind) << 
    ", line " << fInputLineNumber <<
    ", length: " << getMeasureLength () << " divs" <<
    " (" << getMeasureLengthAsString () << ")" <<
    ", " << fMeasureDivisionsPerFullMeasure << " dpfm" <<
 // JMI   ", pos = " << fMeasurePosition << ", " <<
    ", " <<
    singularOrPlural (
      fMeasureElementsList.size (), "element", "elements") <<
// JMI      ", part high tide = " <<
// JMI      fMeasureDirectPartUplink->
// JMI        getPartMeasurePositionHighTide () <<
    endl;
      
  if (fMeasureElementsList.size ()) {
    idtr++;
    
    list<S_msrElement>::const_iterator
      iBegin = fMeasureElementsList.begin(),
      iEnd   = fMeasureElementsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        idtr << (*i);
        // JMI << endl;
      if (++i == iEnd) break;
  //    os << idtr;
    } // for
    
    idtr--;
  }
} 

//______________________________________________________________________________
int msrSegment::gSegmentsCounter = 0;

S_msrSegment msrSegment::create (
  int        inputLineNumber,
  S_msrPart  segmentDirectPartUplink,
  S_msrVoice segmentVoicekUplink)
{
  msrSegment* o =
    new msrSegment (
      inputLineNumber,
      segmentDirectPartUplink,
      segmentVoicekUplink);
  assert(o!=0);

/* JMI
  // create the segment initial measure
  o->
    createSegmentInitialMeasure ();
  */
  
  return o;
}

msrSegment::msrSegment (
  int          inputLineNumber,
  S_msrPart    segmentDirectPartUplink,
  S_msrVoice   segmentVoicekUplink)
    : msrElement (inputLineNumber)
{
  // set segment's direct part uplink
  msrAssert(
    segmentDirectPartUplink != 0,
    "segmentDirectPartUplink is null");
    
  fSegmentDirectPartUplink =
    segmentDirectPartUplink;

  // set segment's voice uplink
  msrAssert(
    segmentVoicekUplink != 0,
    "segmentVoicekUplink is null");
    
  fSegmentVoiceUplink =
    segmentVoicekUplink;

  // initialize segment
  initializeSegment ();
}

msrSegment::~msrSegment() {}

void msrSegment::createSegmentInitialMeasure () // JMI
{
  string firstMeasureNumber = // JMI
    fSegmentDirectPartUplink->
      getPartMeasureNumber (); // JMI "?";
  
  if (gGeneralOptions->fTraceSegments) {
    cerr <<
      idtr <<
        "Creating segment " << segmentAsString () <<
        "'s first measure with number " <<
        firstMeasureNumber <<
        ", in voice \"" <<
        fSegmentVoiceUplink->getVoiceName () <<
        "\"" <<
        endl;
  }

  // create a first measure
  S_msrMeasure
    firstMeasure =
      msrMeasure::create (
        fInputLineNumber,
        fSegmentDirectPartUplink,
        firstMeasureNumber,
        this);

/* jmi
  // set the measure clef JMI
  firstMeasure->
    setMeasureCurrentClef (
      fSegmentVoiceUplink->getVoiceClef ());

  // set the measure key JMI
  firstMeasure->
    setMeasureCurrentKey (
      fSegmentVoiceUplink->getVoiceKey ());
*/

  // set the measure time JMI
  fSegmentCurrentTime =
    fSegmentVoiceUplink->
      getVoiceCurrentTime ();

  if (! fSegmentCurrentTime) {
    // use the implicit initial 4/4 time signature
    fSegmentCurrentTime =
      msrTime::create (
        fInputLineNumber,
        4, 4);
  }

  firstMeasure->
    setMeasureCurrentTime (
      fSegmentCurrentTime);        

  // append firstMeasure to the segment
  appendMeasureToSegment (firstMeasure);
}

S_msrSegment msrSegment::createWithFirstMeasure (
  int          inputLineNumber,
  S_msrPart    segmentDirectPartUplink,
  S_msrVoice   segmentVoicekUplink,
  S_msrMeasure firstMeasure)
{
  S_msrSegment
    segment =
      msrSegment::create (
        inputLineNumber,
        segmentDirectPartUplink,
        segmentVoicekUplink);
  assert(segment!=0);

  // append firstMeasure to the segment
  segment->
    appendMeasureToSegment (firstMeasure);
  
  return segment;
}

void msrSegment::initializeSegment ()
{
  fSegmentAbsoluteNumber = ++gSegmentsCounter;
  
  if (gGeneralOptions->fTraceSegments) {
    cerr << idtr <<
      "% --> new segment gets absolute number " <<
      fSegmentAbsoluteNumber <<
      endl;
  }

  fMeasureNumberHasBeenSetInSegment = false;
}

S_msrSegment msrSegment::createSegmentShallowClone (
  S_msrVoice voiceClone)
{
  if (gGeneralOptions->fTraceSegments) {
    cerr << idtr <<
      "Creating a shallow clone of segment " <<
      segmentAsString () <<
      endl;
  }

  msrAssert(
    voiceClone != 0,
    "voiceClone is null");
    
  S_msrSegment
    clone =
      msrSegment::create (
        fInputLineNumber,
        voiceClone->getVoiceDirectPartUplink (),
        voiceClone);

  clone->fSegmentCurrentTime =
    fSegmentCurrentTime;
  
  return clone;
}

void msrSegment::createAndAppendMeasureToSegment (
  int    inputLineNumber,
  string measureNumber)
{
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceSegments)
    cerr << idtr <<
      "Append a new measure '" << measureNumber <<
      "', line " << inputLineNumber <<
      ", to segment " << segmentAsString () <<
      endl;

  fSegmentMeasureNumber = measureNumber; // JMI

/* JMI
  // make sure there's at least one measure in the segment
  appendMeasureToSegmentIfNeeded (
    inputLineNumber,
    measureNumber);
*/

  // determine new measure 'first in segment' kind
  msrMeasure::msrMeasureFirstInSegmentKind
    measureFirstInSegmentKind;

  if (fSegmentMeasuresList.size () == 0) {
    // this is the first measure in the segment
    measureFirstInSegmentKind =
      msrMeasure::kMeasureFirstInSegmentYes;
  }
  else {
    // this is not the first measure in the segment
    measureFirstInSegmentKind =
      msrMeasure::kMeasureFirstInSegmentNo;
  }

/* JMI
  // fetch segment last measure
  S_msrMeasure
    lastMeasure =
      fSegmentMeasuresList.back ();

  // finalize last measure
  lastMeasure->
    finalizeMeasure (
      inputLineNumber,
      measureFirstInSegmentKind);
      */
      
  // create a measure
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceSegments)
    cerr << idtr <<
      "Creating measure " << measureNumber <<
      " in segment " << segmentAsString () <<
      ", line " << inputLineNumber <<
      ", measure is " <<
      msrMeasure::measureFirstInSegmentKindAsString (
        measureFirstInSegmentKind) <<
      endl;

  S_msrMeasure
    newMeasure =
      msrMeasure::create (
        inputLineNumber,
        fSegmentDirectPartUplink,
        measureNumber,
        this);

  // set
  newMeasure->
    setMeasureFirstInSegmentKind (
      measureFirstInSegmentKind);
  
  // append it to the segment's measures list
  fSegmentMeasuresList.push_back (
    newMeasure);
  
  fMeasureNumberHasBeenSetInSegment = true;
}

void msrSegment::finalizeCurrentMeasureInSegment (
  int inputLineNumber)
{
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceSegments)
    cerr << idtr <<
      "Finalizing segment " <<
      segmentAsString () <<
      ", line " << inputLineNumber <<
      endl;

  if (fSegmentMeasuresList.size ())
    fSegmentMeasuresList.back ()->
      finalizeMeasure (
        inputLineNumber);
        
  else {
    stringstream s;

    s <<
      "cannot finalize current measure in segment " <<
      segmentAsString () <<
      " since it doesn't contain any measure";

    msrInternalError (
      inputLineNumber, s.str ());
  }
}

void msrSegment::appendClefToSegment (S_msrClef clef)
{
  if (gGeneralOptions->fTraceSegments)
    cerr <<
      idtr <<
        "Appending clef " << clef->clefAsString () <<
        " to segment " << segmentAsString () <<
        endl;
      
  // register clef in segment
// JMI  fSegmentClef = clef;

  // register clef in segments's current measure
  fSegmentMeasuresList.back ()->
    appendClefToMeasure (clef);
}

void msrSegment::appendKeyToSegment (S_msrKey key)
{
  if (gGeneralOptions->fTraceSegments)
    cerr <<
      idtr <<
        "Appending key " << key->keyAsString () <<
        " to segment " << segmentAsString () <<
        endl;
      
  // register key in segment
// JMI  fSegmentKey = key;

  // register key in segments's current measure
  fSegmentMeasuresList.back ()->
    appendKeyToMeasure (key);
}
    
void msrSegment::appendTimeToSegment (S_msrTime time)
{
  if (gGeneralOptions->fTraceTimes || gGeneralOptions->fTraceSegments)
    cerr <<
      idtr <<
        "Appending time " << time->timeAsString () <<
        " to segment " << segmentAsString () <<
        endl;
      
  // register time in segment
  fSegmentCurrentTime = time;

  // register time in segments's current measure
  fSegmentMeasuresList.back ()->
    appendTimeToMeasure (time);
}

void msrSegment::appendHarmonyToSegment (S_msrHarmony harmony)
{
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    cerr <<
      idtr <<
        "Appending harmony " << harmony->harmonyAsString () <<
        " to segment " << segmentAsString () <<
        "' in voice \"" <<
        fSegmentVoiceUplink->
          getVoiceName () <<
        "\"" <<
        endl;
      
  // append it to this segment
  fSegmentMeasuresList.back ()-> // JMI ???
    appendHarmonyToMeasure (harmony);
}

void msrSegment::appendHarmonyToSegmentClone (S_msrHarmony harmony)
{
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    cerr <<
      idtr <<
        "Appending harmony " << harmony->harmonyAsString () <<
        " to segment clone " << segmentAsString () <<
        "' in voice clone \"" <<
        fSegmentVoiceUplink->
          getVoiceName () <<
        "\"" <<
        endl;
      
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendHarmonyToMeasureClone (harmony);
}

void msrSegment::appendSegnoToSegment (S_msrSegno segno)
{
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    cerr <<
      idtr <<
        "Appending segno " <<
        " to segment clone " << segmentAsString () <<
        "' in voice clone \"" <<
        fSegmentVoiceUplink->
          getVoiceName () <<
        "\"" <<
        endl;
      
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendSegnoToMeasure (segno);
}

void msrSegment::appendCodaToSegment (S_msrCoda coda)
{
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    cerr <<
      idtr <<
        "Appending coda " <<
        " to segment clone " << segmentAsString () <<
        "' in voice clone \"" <<
        fSegmentVoiceUplink->
          getVoiceName () <<
        "\"" <<
        endl;
      
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendCodaToMeasure (coda);
}

void msrSegment::appendEyeGlassesToSegment (
  S_msrEyeGlasses eyeGlasses)
{
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    cerr <<
      idtr <<
        "Appending eyeglasses " <<
        " to segment clone " << segmentAsString () <<
        "' in voice clone \"" <<
        fSegmentVoiceUplink->
          getVoiceName () <<
        "\"" <<
        endl;
      
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendEyeGlassesToMeasure (eyeGlasses);
}

void msrSegment::appendPedalToSegment (S_msrPedal pedal)
{
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    cerr <<
      idtr <<
        "Appending pedal " <<
        " to segment clone " << segmentAsString () <<
        "' in voice clone \"" <<
        fSegmentVoiceUplink->
          getVoiceName () <<
        "\"" <<
        endl;
      
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendPedalToMeasure (pedal);
}

void msrSegment::appendTransposeToSegment (
  S_msrTranspose transpose)
{
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    cerr <<
      idtr <<
        "Appending transpose " <<
        " to segment clone " << segmentAsString () <<
        "' in voice clone \"" <<
        fSegmentVoiceUplink->
          getVoiceName () <<
        "\"" <<
        endl;
      
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendTransposeToMeasure (transpose);
}

void msrSegment::appendStaffDetailsToSegment (
  S_msrStaffDetails staffDetails)
{
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    cerr <<
      idtr <<
        "Appending staff details " <<
        " to segment clone " << segmentAsString () <<
        "' in voice clone \"" <<
        fSegmentVoiceUplink->
          getVoiceName () <<
        "\"" <<
        endl;
      
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendStaffDetailsToMeasure (staffDetails);
}

void msrSegment::appendBreakToSegment (S_msrBreak break_)
{
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    cerr <<
      idtr <<
        "Appending break " <<
        " to segment clone " << segmentAsString () <<
        "' in voice clone \"" <<
        fSegmentVoiceUplink->
          getVoiceName () <<
        "\"" <<
        endl;
      
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendBreakToMeasure (break_);
}

void msrSegment::appendBarNumberCheckToSegment (
  S_msrBarNumberCheck barNumberCheck)
{
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    cerr <<
      idtr <<
        "Appending bar number check " <<
        " to segment clone " << segmentAsString () <<
        "' in voice clone \"" <<
        fSegmentVoiceUplink->
          getVoiceName () <<
        "\"" <<
        endl;
      
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendBarNumberCheckToMeasure (barNumberCheck);
}

void msrSegment::appendTempoToSegment (
  S_msrTempo tempo)
{
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    cerr <<
      idtr <<
        "Appending tempo " <<
        " to segment clone " << segmentAsString () <<
        "' in voice clone \"" <<
        fSegmentVoiceUplink->
          getVoiceName () <<
        "\"" <<
        endl;
      
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendTempoToMeasure (tempo);
}

void msrSegment::appendRehearsalToSegment (
  S_msrRehearsal rehearsal)
{
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    cerr <<
      idtr <<
        "Appending rehearsal " <<
        " to segment clone " << segmentAsString () <<
        "' in voice clone \"" <<
        fSegmentVoiceUplink->
          getVoiceName () <<
        "\"" <<
        endl;
      
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendRehearsalToMeasure (rehearsal);
}

void msrSegment::appendOctaveShiftToSegment (
  S_msrOctaveShift octaveShift)
{
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceSegments)
    cerr <<
      idtr <<
        "Appending octave shift " <<
        " to segment clone " << segmentAsString () <<
        "' in voice clone \"" <<
        fSegmentVoiceUplink->
          getVoiceName () <<
        "\"" <<
        endl;
      
  // append it to this segment
  fSegmentMeasuresList.back ()->
    appendOctaveShiftToMeasure (octaveShift);
}

void msrSegment::bringSegmentToMeasurePosition (
  int inputLineNumber,
  int measurePosition)
{
  if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceDivisions)
    cerr << idtr <<
      "Bringing measure position for segment '" <<
      fSegmentAbsoluteNumber <<
      "' in voice \"" <<
      fSegmentVoiceUplink->
        getVoiceName () <<
      "\"," <<
      "' to " << measurePosition <<
      ", line " << inputLineNumber <<
      endl;

  // append last measure to this position
  fSegmentMeasuresList.back ()->
    bringMeasureToMeasurePosition (
      inputLineNumber, measurePosition);
}

void msrSegment::appendMeasureToSegment (S_msrMeasure measure)
{
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceSegments)
    cerr << idtr <<
      "Appending measure " << measure->getMeasureNumber () <<
      " to segment " << segmentAsString () <<
 // JMI     ", lastMeasureNumber = " << lastMeasureNumber <<
      ", line " << measure->getInputLineNumber () <<
      endl;

  // append measure to the segment
  fSegmentMeasuresList.push_back (measure);
}

void msrSegment::appendMeasureToSegmentIfNeeded ( // JMI
  int    inputLineNumber,
  string measureNumber)
{
  if (! fSegmentMeasuresList.size ()) {
    // create a new measure
    S_msrMeasure
      newMeasure =
        msrMeasure::create (
          inputLineNumber,
          fSegmentDirectPartUplink,
          measureNumber,
          this);

    // append it to the segment
    appendMeasureToSegment (
      newMeasure);
  }
}

void msrSegment::prependBarlineToSegment (S_msrBarline barline)
{
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceSegments)
    cerr <<
      idtr <<
        "Prepending barline '" << barline->barlineAsString () <<
        "' to segment '" << segmentAsString () << "'" <<
      endl;

  fSegmentMeasuresList.front ()->
    prependBarlineToMeasure (barline);
}

void msrSegment::appendBarlineToSegment (S_msrBarline barline)
{
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceSegments)
    cerr <<
      idtr <<
        "Appending barline '" << barline->barlineAsString () <<
        "' to segment '" << segmentAsString () << "'" <<
      endl;

  if (! fSegmentMeasuresList.size ()) {// JMI
  }
  
  fSegmentMeasuresList.back ()->
    appendBarlineToMeasure (barline);
}

void msrSegment::appendBarCheckToSegment (S_msrBarCheck barCheck)
{
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceSegments)
    cerr <<
      idtr <<
        "Appending bar check " << barCheck->barCheckAsString () <<
        " to segment '" << segmentAsString () << "'" <<
      endl;
      
  fSegmentMeasuresList.back ()->
    appendBarCheckToMeasure (barCheck);
}

void msrSegment::appendNoteToSegment (S_msrNote note)
{
  appendMeasureToSegmentIfNeeded (
    note->getInputLineNumber (),
    fSegmentMeasureNumber);
  
  fSegmentMeasuresList.back ()->
    appendNoteToMeasure (note);
}

void msrSegment::appendNoteToSegmentClone (S_msrNote note)
{
  fSegmentMeasuresList.back ()->
    appendNoteToMeasureClone (note);
}

void msrSegment::appendDoubleTremoloToSegment ( // XXL
  S_msrDoubleTremolo doubleTremolo)
{
  if (gGeneralOptions->fTraceTremolos || gGeneralOptions->fTraceSegments)
    cerr <<
      idtr <<
        "Appending double tremolo " <<
        doubleTremolo->doubleTremoloAsShortString () <<
        " to segment '" << segmentAsString () << "'" <<
      endl;
      
  fSegmentMeasuresList.back ()->
    appendDoubleTremoloToMeasure (doubleTremolo);
}

void msrSegment::appendMeasureRepeatToSegment (
  S_msrMeasureRepeat measureRepeat)
{
  if (gGeneralOptions->fTraceRepeats || gGeneralOptions->fTraceSegments)
    cerr <<
      idtr <<
        "Appending measure repeat '" <<
        measureRepeat->measureRepeatAsString () <<
        "' to segment '" << segmentAsString () << "'" <<
      endl;
      
  fSegmentMeasuresList.back ()->
    appendMeasureRepeatToMeasure (measureRepeat);
}

void msrSegment::appendMultipleRestToSegment (
  S_msrMultipleRest multipleRest)
{
  if (gGeneralOptions->fTraceRepeats || gGeneralOptions->fTraceSegments)
    cerr <<
      idtr <<
        "Appending multiple rest '" <<
        multipleRest->multipleRestAsString () <<
        "' to segment '" << segmentAsString () << "'" <<
      endl;
      
  fSegmentMeasuresList.back ()->
    appendMultipleRestToMeasure (multipleRest);
}

void msrSegment::appendChordToSegment (S_msrChord chord) // XXL
{
  fSegmentMeasuresList.back ()->
    appendChordToMeasure (chord);
}

void msrSegment::appendTupletToSegment (S_msrTuplet tuplet) // XXL
{
  fSegmentMeasuresList.back ()->
    appendTupletToMeasure (tuplet);
}

void msrSegment::appendGraceNotesToSegment (
  S_msrGraceNotes graceNotes)
{
  fSegmentMeasuresList.back ()->
    appendGraceNotesToMeasure (graceNotes);
}

void msrSegment::prependGraceNotesToSegment (
  S_msrGraceNotes graceNotes)

{
  fSegmentMeasuresList.front ()->
    prependGraceNotesToMeasure (graceNotes); // JMI
}

void msrSegment::appendAfterGraceNotesToSegment (
  S_msrAfterGraceNotes afterGraceNotes)
{
  fSegmentMeasuresList.back ()->
    appendAfterGraceNotesToMeasure (afterGraceNotes);
}

void msrSegment::prependAfterGraceNotesToSegment (
  S_msrAfterGraceNotes afterGraceNotes)

{
  fSegmentMeasuresList.front ()->
    prependAfterGraceNotesToMeasure (afterGraceNotes); // JMI
}

void msrSegment::appendOtherElementToSegment (S_msrElement elem)
{
  appendMeasureToSegmentIfNeeded ( // JMI
    elem->getInputLineNumber (),
    fSegmentMeasureNumber); // +1??? JMI
  
  fSegmentMeasuresList.back ()->
    appendOtherElementToMeasure (elem);
}

/* JMI
void msrSegment::removeElementFromSegment (
  S_msrElement elem)
{
  for (
    list<S_msrElement>::const_iterator i = fSegmentMeasuresList.begin();
    i != fSegmentMeasuresList.end();
    i++) {
    if ((*i) == elem) {
      fSegmentMeasuresList.erase (i);
      break;
    }
  } // for
}
*/

/*
S_msrElement msrSegment::removeLastElementFromSegment (
  int inputLineNumber)
{
  // this last element can be a note or a tuplet,
  // this method is used when the seconde note of a chord is mest
  
  if (fSegmentMeasuresList.size ()) {
    return
      fSegmentMeasuresList.back ()->
        removeLastElementFromMeasure (
          inputLineNumber);
  }
  
  else {
    msrInternalError (
      inputLineNumber,
      "cannot removeLastElementFromSegment () " <<
      segmentAsString () <<
      " since it is empty");
  }
}
*/

void msrSegment::removeNoteFromSegment (
  int       inputLineNumber,
  S_msrNote note)
{  
  if (fSegmentMeasuresList.size ()) {
    fSegmentMeasuresList.back ()->
      removeNoteFromMeasure (
        inputLineNumber,
        note);
  }
  
  else {
    msrInternalError (
      inputLineNumber,
      "cannot remove note from segment " <<
      segmentAsString () <<
      " since it is empty");
  }
}

S_msrMeasure msrSegment::removeLastMeasureFromSegment (
  int inputLineNumber)
{
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceSegments) {
    cerr << idtr <<
      "Removing last measure from segment '" <<
      fSegmentAbsoluteNumber <<
      "'" <<
      endl;
  }

  if (! fSegmentMeasuresList.size ()) {
    stringstream s;

    s <<
      "cannot remove last measure from from segment '" <<
      fSegmentAbsoluteNumber <<
      "' since it is empty";

    msrInternalError (
      inputLineNumber, s.str());
  }

  S_msrMeasure
    result =
      fSegmentMeasuresList.back ();

  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceSegments) {
    cerr << idtr <<
      "This measure contains:";

    idtr++;

    cerr <<
      result;

    idtr--;
  }

  fSegmentMeasuresList.pop_back ();
  
  return result;
}

/*
bool msrSegment::checkForIncompleteSegmentLastMeasure (
  int                        inputLineNumber,
  msrMeasure::msrMeasureKind measureKind)
{
  if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Checking for incomplete last measure in segment '" <<
      segmentAsString () <<
      ", in voice \"" <<
      fSegmentVoiceUplink->
        getVoiceName () <<
      "\"" <<
      "', line " << inputLineNumber <<
      endl;

  // fetch segment last measure
  S_msrMeasure
    lastMeasure =
      fSegmentMeasuresList.back ();

  return
    lastMeasure->
      checkForIncompleteMeasure (
        inputLineNumber,
        measureKind);
}
*/

void msrSegment::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrSegment::acceptIn()" <<
      endl;
      
  if (visitor<S_msrSegment>*
    p =
      dynamic_cast<visitor<S_msrSegment>*> (v)) {
        S_msrSegment elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrSegment::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrSegment::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrSegment::acceptOut()" <<
      endl;

  if (visitor<S_msrSegment>*
    p =
      dynamic_cast<visitor<S_msrSegment>*> (v)) {
        S_msrSegment elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrSegment::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrSegment::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrSegment::browseData()" <<
      endl;

/* JMI
  cerr <<
    idtr << tab <<
    "==========>>> " <<
    fSegmentMeasuresList.size () << " measures" << endl;
    */
    
  for (
    list<S_msrMeasure>::const_iterator i = fSegmentMeasuresList.begin();
    i != fSegmentMeasuresList.end();
    i++) {
    // browse the element
    msrBrowser<msrMeasure> browser (v);
    browser.browse (*(*i));
  } // for

  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% <== msrSegment::browseData()" <<
      endl;
}

string msrSegment::segmentAsString ()
{
  stringstream s;

  s << "Segment " << fSegmentAbsoluteNumber;
  
  if (! fSegmentMeasuresList.size ())
    s <<
      " (0 measures)";
  else
    s <<
      " (" <<
      singularOrPlural (
        fSegmentMeasuresList.size (), "measure", " measures") <<
      ")";

  return s.str();
}

ostream& operator<< (ostream& os, const S_msrSegment& elt)
{
  elt->print (os);
  return os;
}

void msrSegment::print (ostream& os)
{  
  os << idtr <<
    "Segment " <<
    fSegmentAbsoluteNumber <<
    ", " <<
    singularOrPlural (
      fSegmentMeasuresList.size(), "measure", "measures");

  if (! fSegmentCurrentTime) {
    // use the implicit initial 4/4 time signature
    fSegmentCurrentTime =
      msrTime::create (
        fInputLineNumber,
        4, 4);
  }

  os <<
    ", SegmentCurrentTime" << " = " <<
    fSegmentCurrentTime->timeAsShortString () <<
    endl;
  
  if (! fSegmentMeasuresList.size ()) {
    os << idtr <<
      "Measures: none";
  }
  
  else {        
    idtr++;
    
    list<S_msrMeasure>::const_iterator
      iBegin = fSegmentMeasuresList.begin(),
      iEnd   = fSegmentMeasuresList.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
    
    idtr--;
  }
}

//______________________________________________________________________________
S_msrRepeatEnding msrRepeatEnding::create (
  int                 inputLineNumber,
  string              repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind repeatEndingKind,
  S_msrSegment        segment,
  S_msrRepeat         repeatUplink)
{
  msrRepeatEnding* o =
    new msrRepeatEnding (
      inputLineNumber,
      repeatEndingNumber,
      repeatEndingKind,
      segment,
      repeatUplink);
  assert(o!=0);
  return o;
}

msrRepeatEnding::msrRepeatEnding (
  int                 inputLineNumber,
  string              repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind repeatEndingKind,
  S_msrSegment        segment,
  S_msrRepeat         repeatUplink)
    : msrElement (inputLineNumber)
{
  fRepeatEndingNumber = repeatEndingNumber;
  
  fRepeatEndingInternalNumber = 0;
    // will be set by msrRepeat::addRepeatEnding ()
  
  fRepeatEndingKind = repeatEndingKind;
  
  fRepeatEndingSegment = segment;
  
  fRepeatEndingRepeatUplink = repeatUplink;
}

msrRepeatEnding::~msrRepeatEnding() {}

S_msrRepeatEnding msrRepeatEnding::createRepeatEndingShallowClone (
  S_msrRepeat repeatClone)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Creating a shallow clone of a " <<
      repeatEndingAsString () <<
      endl;
  
  msrAssert(
    repeatClone != 0,
    "repeatClone is null");
    
  S_msrRepeatEnding
    clone =
      msrRepeatEnding::create (
        fInputLineNumber,
        fRepeatEndingNumber,
        fRepeatEndingKind,
        repeatClone->getRepeatCommonSegment (), // JMI
        repeatClone);
  
  return clone;
}

/* JMI
void msrRepeatEnding::appendElementToRepeatEnding (
  S_msrElement elem) // JMI ???
{
  fRepeatEndingSegment->
    appendOtherElementToSegment (elem);
}
*/

void msrRepeatEnding::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrRepeatEnding::acceptIn()" <<
      endl;
      
  if (visitor<S_msrRepeatEnding>*
    p =
      dynamic_cast<visitor<S_msrRepeatEnding>*> (v)) {
        S_msrRepeatEnding elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrRepeatEnding::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrRepeatEnding::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrRepeatEnding::acceptOut()" <<
      endl;

  if (visitor<S_msrRepeatEnding>*
    p =
      dynamic_cast<visitor<S_msrRepeatEnding>*> (v)) {
        S_msrRepeatEnding elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrRepeatEnding::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrRepeatEnding::browseData (basevisitor* v)
{
  // browse the segment
  msrBrowser<msrSegment> browser (v);
  browser.browse (*fRepeatEndingSegment);
}

ostream& operator<< (ostream& os, const S_msrRepeatEnding& elt)
{
  elt->print (os);
  return os;
}

string msrRepeatEnding::repeatEndingKindAsString (
  msrRepeatEndingKind repeatEndingKind)
{
  string result;
  
  switch (repeatEndingKind) {
    case msrRepeatEnding::kHookedEnding:
      result = "hooked";
      break;
    case msrRepeatEnding::kHooklessEnding:
      result = "hookless";
      break;
  } // switch
  
  return result;
}

string msrRepeatEnding::repeatEndingAsString () const
{
  stringstream s;

  s <<
    "RepeatEnding, " <<
    repeatEndingKindAsString (
      fRepeatEndingKind) <<
    ", line " << fInputLineNumber <<
    ", number " << fRepeatEndingNumber <<
    ", internal number " << fRepeatEndingInternalNumber <<
    endl;

  return s.str();
}

void msrRepeatEnding::print (ostream& os)
{
  os <<
    endl <<
    idtr <<
    repeatEndingAsString () <<
    endl;
 
  idtr++;

  os <<
    fRepeatEndingSegment;

  idtr--;
}

//______________________________________________________________________________
S_msrRepeat msrRepeat::create (
  int          inputLineNumber,
  S_msrVoice   voiceUplink)
{
  msrRepeat* o =
    new msrRepeat (
      inputLineNumber, voiceUplink);
  assert(o!=0);
  return o;
}

msrRepeat::msrRepeat (
  int          inputLineNumber,
  S_msrVoice   voiceUplink)
    : msrElement (inputLineNumber)
{
  fRepeatEndingsInternalCounter = 0;
  
  fRepeatVoiceUplink = voiceUplink;
}

msrRepeat::~msrRepeat() {}

S_msrRepeat msrRepeat::createRepeatShallowClone (S_msrVoice voiceClone)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Creating a shallow clone of a repeat" <<
      endl;
  
  msrAssert(
    voiceClone != 0,
    "voiceClone is null");
    
  S_msrRepeat
    clone =
      msrRepeat::create (
        fInputLineNumber,
        voiceClone);
  
  return clone;
}

void msrRepeat::setRepeatCommonSegment (
  S_msrSegment repeatCommonSegment)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Setting repeat common segment containing " <<
      singularOrPlural (
        repeatCommonSegment->getSegmentMeasuresList ().size (),
        "measure",
        "measures") <<
      endl;
      
  fRepeatCommonSegment = repeatCommonSegment;
}

void msrRepeat::addRepeatEnding (S_msrRepeatEnding repeatEnding)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Adding ending '" <<
      repeatEnding->repeatEndingAsString () <<
      "' to repeat" <<
      endl;
      
  fRepeatEndings.push_back (repeatEnding);

  // set repeat ending internal number
  repeatEnding->
    setRepeatEndingInternalNumber (
      ++ fRepeatEndingsInternalCounter);
}

void msrRepeat::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrRepeat::acceptIn()" <<
      endl;
      
  if (visitor<S_msrRepeat>*
    p =
      dynamic_cast<visitor<S_msrRepeat>*> (v)) {
        S_msrRepeat elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrRepeat::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrRepeat::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrRepeat::acceptOut()" <<
      endl;

  if (visitor<S_msrRepeat>*
    p =
      dynamic_cast<visitor<S_msrRepeat>*> (v)) {
        S_msrRepeat elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrRepeat::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrRepeat::browseData (basevisitor* v)
{
  if (fRepeatCommonSegment) {
  // browse the common segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fRepeatCommonSegment);
  }
  
  // browse the alternatives
  for (
    vector<S_msrRepeatEnding>::const_iterator i = fRepeatEndings.begin();
    i != fRepeatEndings.end();
    i++) {
    // browse the alternative
    msrBrowser<msrRepeatEnding> browser (v);
    browser.browse (*(*i));
  } // for
}

ostream& operator<< (ostream& os, const S_msrRepeat& elt)
{
  elt->print (os);
  return os;
}

void msrRepeat::print (ostream& os)
{
  os <<
    endl <<
    idtr << "Repeat" <<
    ", line " << fInputLineNumber <<
    " (" << fRepeatEndings.size() << " repeat endings)" <<
    endl;
  
  idtr++;
  
  // print the repeat common segment
  os << idtr <<
    "Common segment: ";
  if (! fRepeatCommonSegment)
    os << "none";
  os << endl;

  if (fRepeatCommonSegment) {
    idtr++;
    
    os <<
      fRepeatCommonSegment;

    idtr--;
  }

  // print the repeat endings
  int endingsNumber =
    fRepeatEndings.size ();
    
  os <<
    endl <<
    idtr <<
    "Repeat endings: ";
  if (endingsNumber)
    os << "(" << endingsNumber << ")";
  else
    os << "none";
  os << endl;
    
  if (endingsNumber) {
    idtr++;
    
    vector<S_msrRepeatEnding>::const_iterator
      iBegin = fRepeatEndings.begin(),
      iEnd   = fRepeatEndings.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      // print the repeat ending
      os << idtr << (*i);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for

    idtr--;
  }

  os << endl;
      
  idtr--;
}

//______________________________________________________________________________
S_msrMeasureRepeatPattern msrMeasureRepeatPattern::create (
  int        inputLineNumber,
  S_msrVoice voiceUplink)
{
  msrMeasureRepeatPattern* o =
    new msrMeasureRepeatPattern (
      inputLineNumber,
      voiceUplink);
  assert(o!=0);
  return o;
}

msrMeasureRepeatPattern::msrMeasureRepeatPattern (
  int        inputLineNumber,
  S_msrVoice voiceUplink)
    : msrElement (inputLineNumber)
{
  fMeasureRepeatPatternVoiceUplink = voiceUplink;
}

msrMeasureRepeatPattern::~msrMeasureRepeatPattern() {}

S_msrMeasureRepeatPattern msrMeasureRepeatPattern::createMeasureRepeatPatternShallowClone (
  S_msrVoice voiceClone)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Creating a shallow clone of a measure repeat pattern" <<
      endl;
  
  msrAssert(
    voiceClone != 0,
    "voiceClone is null");
    
  S_msrMeasureRepeatPattern
    clone =
      msrMeasureRepeatPattern::create (
        fInputLineNumber,
        voiceClone);

  return clone;
}

void msrMeasureRepeatPattern::setMeasureRepeatPatternSegment (
    S_msrSegment measureRepeatPatternSegment)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Setting measure repeat pattern segment containing " <<
      singularOrPlural (
          measureRepeatPatternMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
      
  msrAssert (
    measureRepeatPatternSegment != 0,
    "measureRepeatPatternSegment is null");

  fMeasureRepeatPatternSegment =
    measureRepeatPatternSegment;
}

int msrMeasureRepeatPattern::measureRepeatPatternMeasuresNumber () const
{
  int result;

  if (fMeasureRepeatPatternSegment)
    result =
      fMeasureRepeatPatternSegment->
      getSegmentMeasuresList ().size ();
  else
    result = 0;

  return result;
}

void msrMeasureRepeatPattern::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrMeasureRepeatPattern::acceptIn()" <<
      endl;
      
  if (visitor<S_msrMeasureRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatPattern>*> (v)) {
        S_msrMeasureRepeatPattern elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrMeasureRepeatPattern::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrMeasureRepeatPattern::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrMeasureRepeatPattern::acceptOut()" <<
      endl;

  if (visitor<S_msrMeasureRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatPattern>*> (v)) {
        S_msrMeasureRepeatPattern elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrMeasureRepeatPattern::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrMeasureRepeatPattern::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrMeasureRepeatPattern::browseData()" <<
      endl;

  if (fMeasureRepeatPatternSegment) {
    // browse the pattern segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fMeasureRepeatPatternSegment);
  }
}

ostream& operator<< (ostream& os, const S_msrMeasureRepeatPattern& elt)
{
  elt->print (os);
  return os;
}

string msrMeasureRepeatPattern::measureRepeatPatternAsString () const
{
  stringstream s;

  s <<

    "MeasureRepeatPattern" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      measureRepeatPatternMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ")"; 

  return s.str();
}

void msrMeasureRepeatPattern::print (ostream& os)
{
  os <<
    endl <<
    idtr <<
    measureRepeatPatternAsString () <<
    "MeasureRepeatPattern" <<
    endl <<
    endl;
  
  idtr++;
  
  // print the pattern segment
  os << idtr <<
    "Pattern segment:";

  if (! fMeasureRepeatPatternSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os <<
      endl;
      
    idtr++;
    
    os <<
      fMeasureRepeatPatternSegment;

    idtr--;
  }
      
  idtr--;
}

//______________________________________________________________________________
S_msrMeasureRepeatReplicas msrMeasureRepeatReplicas::create (
  int          inputLineNumber,
  S_msrVoice   voiceUplink)
{
  msrMeasureRepeatReplicas* o =
    new msrMeasureRepeatReplicas (
      inputLineNumber,
      voiceUplink);
  assert(o!=0);
  return o;
}

msrMeasureRepeatReplicas::msrMeasureRepeatReplicas (
  int        inputLineNumber,
  S_msrVoice voiceUplink)
    : msrElement (inputLineNumber)
{
  fMeasureRepeatReplicasVoiceUplink = voiceUplink;
}

msrMeasureRepeatReplicas::~msrMeasureRepeatReplicas() {}

S_msrMeasureRepeatReplicas msrMeasureRepeatReplicas::createMeasureRepeatReplicasShallowClone (
  S_msrVoice voiceClone)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Creating a shallow clone of a measure repeat replicas" <<
      endl;
  
  msrAssert(
    voiceClone != 0,
    "voiceClone is null");
    
  S_msrMeasureRepeatReplicas
    clone =
      msrMeasureRepeatReplicas::create (
        fInputLineNumber,
        voiceClone);

  return clone;
}

void msrMeasureRepeatReplicas::setMeasureRepeatReplicasSegment (
    S_msrSegment measureRepeatReplicasSegment)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Setting measure repeat replicas segment containing " <<
      singularOrPlural (
          measureRepeatReplicasMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
      
  msrAssert (
    measureRepeatReplicasSegment != 0,
    "measureRepeatReplicasSegment is null");

  fMeasureRepeatReplicasSegment =
    measureRepeatReplicasSegment;
}

int msrMeasureRepeatReplicas::measureRepeatReplicasMeasuresNumber () const
{
  int result;

  if (fMeasureRepeatReplicasSegment)
    result =
      fMeasureRepeatReplicasSegment->
      getSegmentMeasuresList ().size ();
  else
    result = 0;

  return result;
}

void msrMeasureRepeatReplicas::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrMeasureRepeatReplicas::acceptIn()" <<
      endl;
      
  if (visitor<S_msrMeasureRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatReplicas>*> (v)) {
        S_msrMeasureRepeatReplicas elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrMeasureRepeatReplicas::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrMeasureRepeatReplicas::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrMeasureRepeatReplicas::acceptOut()" <<
      endl;

  if (visitor<S_msrMeasureRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatReplicas>*> (v)) {
        S_msrMeasureRepeatReplicas elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrMeasureRepeatReplicas::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrMeasureRepeatReplicas::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrMeasureRepeatReplicas::browseData()" <<
      endl;

  if (fMeasureRepeatReplicasSegment) {
  // browse the replicas segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fMeasureRepeatReplicasSegment);
  }
}

ostream& operator<< (ostream& os, const S_msrMeasureRepeatReplicas& elt)
{
  elt->print (os);
  return os;
}

string msrMeasureRepeatReplicas::measureRepeatReplicasAsString () const
{
  stringstream s;

  s <<

    "MeasureRepeatReplicas" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      measureRepeatReplicasMeasuresNumber (),
      "replicas measure",
      "replicas measures") <<
    ")"; 

  return s.str();
}

void msrMeasureRepeatReplicas::print (ostream& os)
{
  os <<
    endl <<
    idtr <<
    measureRepeatReplicasAsString () <<
    endl <<
    endl;
  
  idtr++;
  
  // print the replicas segment
  os << idtr <<
    "Replicas segment:";

  if (! fMeasureRepeatReplicasSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os <<
      endl;
      
    idtr++;
    
    os <<
      fMeasureRepeatReplicasSegment;

    idtr--;
  }

  idtr--;
}

//______________________________________________________________________________
S_msrMeasureRepeat msrMeasureRepeat::create (
  int          inputLineNumber,
  int          measureRepeatMeasuresNumber,
  int          measureRepeatSlashesNumber,
  S_msrVoice   voiceUplink)
{
  msrMeasureRepeat* o =
    new msrMeasureRepeat (
      inputLineNumber,
      measureRepeatMeasuresNumber, measureRepeatSlashesNumber,
      voiceUplink);
  assert(o!=0);
  return o;
}

msrMeasureRepeat::msrMeasureRepeat (
  int          inputLineNumber,
  int          measureRepeatMeasuresNumber,
  int          measureRepeatSlashesNumber,
  S_msrVoice   voiceUplink)
    : msrElement (inputLineNumber)
{
  fMeasureRepeatMeasuresNumber = measureRepeatMeasuresNumber;
  fMeasureRepeatSlashesNumber  = measureRepeatSlashesNumber;

  fMeasureRepeatVoiceUplink = voiceUplink;
}

msrMeasureRepeat::~msrMeasureRepeat() {}

S_msrMeasureRepeat msrMeasureRepeat::createMeasureRepeatShallowClone (
  S_msrVoice   voiceClone)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Creating a shallow clone of a measure repeat" <<
      endl;
  
  msrAssert(
    voiceClone != 0,
    "voiceClone is null");
    
  S_msrMeasureRepeat
    clone =
      msrMeasureRepeat::create (
        fInputLineNumber,
        fMeasureRepeatMeasuresNumber,
        fMeasureRepeatSlashesNumber,
        voiceClone);

  return clone;
}

void msrMeasureRepeat::setMeasureRepeatPattern (
  S_msrMeasureRepeatPattern measureRepeatPattern)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Setting measure repeat pattern containing " <<
      singularOrPlural (
        measureRepeatPattern->
          measureRepeatPatternMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
      
  msrAssert (
    measureRepeatPattern != 0,
    "measureRepeatPattern is null");

  fMeasureRepeatPattern = measureRepeatPattern;
}

void msrMeasureRepeat::setMeasureRepeatReplicas (
  S_msrMeasureRepeatReplicas measureRepeatReplicas)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Setting measure repeat replicas containing " <<
      singularOrPlural (
        measureRepeatReplicas->
          measureRepeatReplicasMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
      
  msrAssert (
    measureRepeatReplicas != 0,
    "measureRepeatReplicas is null");

  fMeasureRepeatReplicas = measureRepeatReplicas;
}

int msrMeasureRepeat::measureRepeatReplicasNumber () const
{
  // compute replicas number
  return
    measureRepeatReplicasMeasuresNumber ()
      /
    measureRepeatPatternMeasuresNumber ();    
}

void msrMeasureRepeat::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrMeasureRepeat::acceptIn()" <<
      endl;
      
  if (visitor<S_msrMeasureRepeat>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeat>*> (v)) {
        S_msrMeasureRepeat elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrMeasureRepeat::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrMeasureRepeat::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrMeasureRepeat::acceptOut()" <<
      endl;

  if (visitor<S_msrMeasureRepeat>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeat>*> (v)) {
        S_msrMeasureRepeat elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrMeasureRepeat::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrMeasureRepeat::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrMeasureRepeat::browseData()" <<
      endl;

  if (fMeasureRepeatPattern) {
  // browse the measdure repeat pattern
    msrBrowser<msrMeasureRepeatPattern> browser (v);
    browser.browse (*fMeasureRepeatPattern);
  }

  // fetch the score
  S_msrScore
    score =
      fMeasureRepeatVoiceUplink->
        getVoiceDirectPartUplink ()->
          getPartPartGroupUplink ()->
            getPartGroupScoreUplink ();
              
  bool inhibitMeasureRepeatReplicasBrowsing =
    score->getInhibitMeasureRepeatReplicasBrowsing ();

  if (inhibitMeasureRepeatReplicasBrowsing) {
    if (gMsrOptions->fTraceMsrVisitors || gGeneralOptions->fTraceGeneral)
      cerr << idtr <<
        "% ==> visiting measure repeat replicas is inhibited" <<
        endl;
  }

  if (fMeasureRepeatReplicas) {
    if (! inhibitMeasureRepeatReplicasBrowsing) {
      // browse the measure repeat replicas
      msrBrowser<msrMeasureRepeatReplicas> browser (v);
      browser.browse (*fMeasureRepeatReplicas);
    }
  }
}

ostream& operator<< (ostream& os, const S_msrMeasureRepeat& elt)
{
  elt->print (os);
  return os;
}

string msrMeasureRepeat::measureRepeatAsString () const
{
  stringstream s;

  s <<

    "MeasureRepeat" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      measureRepeatPatternMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ", " <<
    singularOrPlural (
      measureRepeatReplicasMeasuresNumber (),
      "replicas measure",
      "replicas measures") <<
    ", " <<
    measureRepeatReplicasNumber () << " replicas" <<
    ")"; 

  return s.str();
}

void msrMeasureRepeat::print (ostream& os)
{
  os <<
    endl <<
    idtr << "MeasureRepeat" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      measureRepeatPatternMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ", " <<
    singularOrPlural (
      measureRepeatReplicasMeasuresNumber (),
      "replicas measure",
      "replicas measures") <<
    ", " <<
    measureRepeatReplicasNumber () << " replicas" <<
    ")" <<
    endl <<
    endl;
  
  idtr++;
  
  // print the measure repeat pattern
  if (! fMeasureRepeatPattern) {
    os << idtr <<
      "Measure repeat pattern: none" <<
      endl;
  }

  else {
    os <<
      fMeasureRepeatPattern;
  }

  os << endl;
  
  // print the measure repeat replicas
  if (! fMeasureRepeatReplicas) {
    os << idtr <<
      "Measure repeat replicas: none" <<
      endl;
  }

  else {
    os <<
      fMeasureRepeatReplicas;
  }
      
  idtr--;
}

//______________________________________________________________________________
S_msrMultipleRestContents msrMultipleRestContents::create (
  int        inputLineNumber,
  S_msrVoice voiceUplink)
{
  msrMultipleRestContents* o =
    new msrMultipleRestContents (
      inputLineNumber,
      voiceUplink);
  assert(o!=0);
  return o;
}

msrMultipleRestContents::msrMultipleRestContents (
  int        inputLineNumber,
  S_msrVoice voiceUplink)
    : msrElement (inputLineNumber)
{
  fMultipleRestContentsVoiceUplink = voiceUplink;
}

msrMultipleRestContents::~msrMultipleRestContents() {}

S_msrMultipleRestContents msrMultipleRestContents::createMultipleRestContentsShallowClone (
  S_msrVoice voiceClone)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Creating a shallow clone of a multiple rest contents" <<
      endl;
  
  msrAssert(
    voiceClone != 0,
    "voiceClone is null");
    
  S_msrMultipleRestContents
    clone =
      msrMultipleRestContents::create (
        fInputLineNumber,
        voiceClone);

  return clone;
}

void msrMultipleRestContents::setMultipleRestContentsSegment (
    S_msrSegment multipleRestContentsSegment)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Setting multiple rest contents segment containing " <<
      singularOrPlural (
          multipleRestContentsMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
      
  msrAssert (
    multipleRestContentsSegment != 0,
    "multipleRestContentsSegment is null");

  fMultipleRestContentsSegment =
    multipleRestContentsSegment;
}

int msrMultipleRestContents::multipleRestContentsMeasuresNumber () const
{
  int result;

  if (fMultipleRestContentsSegment)
    result =
      fMultipleRestContentsSegment->
      getSegmentMeasuresList ().size ();
  else
    result = 0;

  return result;
}

void msrMultipleRestContents::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrMultipleRestContents::acceptIn()" <<
      endl;
      
  if (visitor<S_msrMultipleRestContents>*
    p =
      dynamic_cast<visitor<S_msrMultipleRestContents>*> (v)) {
        S_msrMultipleRestContents elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrMultipleRestContents::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrMultipleRestContents::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrMultipleRestContents::acceptOut()" <<
      endl;

  if (visitor<S_msrMultipleRestContents>*
    p =
      dynamic_cast<visitor<S_msrMultipleRestContents>*> (v)) {
        S_msrMultipleRestContents elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrMultipleRestContents::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrMultipleRestContents::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrMultipleRestContents::browseData()" <<
      endl;

  if (fMultipleRestContentsSegment) {
  // browse the pattern segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fMultipleRestContentsSegment);
  }
}

ostream& operator<< (ostream& os, const S_msrMultipleRestContents& elt)
{
  elt->print (os);
  return os;
}

string msrMultipleRestContents::multipleRestContentsAsString () const
{
  stringstream s;

  s <<

    "MultipleRestContents" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      multipleRestContentsMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ")"; 

  return s.str();
}

void msrMultipleRestContents::print (ostream& os)
{
  os <<
    endl <<
    idtr <<
    multipleRestContentsAsString () <<
    "MultipleRestContents" <<
    endl <<
    endl;
  
  idtr++;
  
  // print the pattern segment
  os << idtr <<
    "Contents segment:";

  if (! fMultipleRestContentsSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os <<
      endl;
      
    idtr++;
    
    os <<
      fMultipleRestContentsSegment;

    idtr--;
  }
      
  idtr--;
}

//______________________________________________________________________________
S_msrMultipleRest msrMultipleRest::create (
  int        inputLineNumber,
  int        multipleRestMeasuresNumber,
  S_msrVoice voiceUplink)
{
  msrMultipleRest* o =
    new msrMultipleRest (
      inputLineNumber,
      multipleRestMeasuresNumber,
      voiceUplink);
  assert(o!=0);
  return o;
}

msrMultipleRest::msrMultipleRest (
  int        inputLineNumber,
  int        multipleRestMeasuresNumber,
  S_msrVoice voiceUplink)
    : msrElement (inputLineNumber)
{
  fMultipleRestMeasuresNumber = multipleRestMeasuresNumber;

  fMultipleRestVoiceUplink = voiceUplink;
}

msrMultipleRest::~msrMultipleRest() {}

S_msrMultipleRest msrMultipleRest::createMultipleRestShallowClone (
  S_msrVoice voiceClone)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Creating a shallow clone of multiple rest " <<
      multipleRestAsString () <<
      endl;
  
  msrAssert(
    voiceClone != 0,
    "voiceClone is null");
    
  S_msrMultipleRest
    clone =
      msrMultipleRest::create (
        fInputLineNumber,
        fMultipleRestMeasuresNumber,
        voiceClone);

  return clone;
}

void msrMultipleRest::setMultipleRestContents (
  S_msrMultipleRestContents multipleRestContents)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Setting multiple rest segment containing " <<
      singularOrPlural (
        multipleRestContents->
          multipleRestContentsMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
      
  msrAssert (
    multipleRestContents != 0,
    "multipleRestContents is null");

  fMultipleRestContents = multipleRestContents;
}

void msrMultipleRest::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrMultipleRest::acceptIn()" <<
      endl;
      
  if (visitor<S_msrMultipleRest>*
    p =
      dynamic_cast<visitor<S_msrMultipleRest>*> (v)) {
        S_msrMultipleRest elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrMultipleRest::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrMultipleRest::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrMultipleRest::acceptOut()" <<
      endl;

  if (visitor<S_msrMultipleRest>*
    p =
      dynamic_cast<visitor<S_msrMultipleRest>*> (v)) {
        S_msrMultipleRest elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrMultipleRest::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrMultipleRest::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrMultipleRest::browseData()" <<
      endl;

  // fetch the score
  S_msrScore
    score =
      fMultipleRestVoiceUplink->
        getVoiceDirectPartUplink ()->
          getPartPartGroupUplink ()->
            getPartGroupScoreUplink ();

  bool inhibitMultipleRestMeasuresBrowsing =
    score->getInhibitMultipleRestMeasuresBrowsing ();

  if (inhibitMultipleRestMeasuresBrowsing) {
    if (gMsrOptions->fTraceMsrVisitors || gGeneralOptions->fTraceGeneral)
      cerr << idtr <<
        "% ==> visiting multiple rest measures is inhibited" <<
        endl;
  }

  if (fMultipleRestContents) {
    if (! inhibitMultipleRestMeasuresBrowsing) {
      // browse the multiple rest contents
      msrBrowser<msrMultipleRestContents> browser (v);
      browser.browse (*fMultipleRestContents);
    }
  }
}

string msrMultipleRest::multipleRestAsString () const
{
  stringstream s;

  s <<
    "Multiple rest" <<
    ", line " << fInputLineNumber <<
    ", " <<
    singularOrPlural (
      fMultipleRestMeasuresNumber,
        "rest measure",
        "rest measures");
    
  return s.str();
}

ostream& operator<< (ostream& os, const S_msrMultipleRest& elt)
{
  elt->print (os);
  return os;
}

void msrMultipleRest::print (ostream& os)
{
  os <<
    endl <<
    idtr <<
    multipleRestAsString () <<
    endl;
  
  idtr++;
  
  // print the rests contents
  if (! fMultipleRestContents) {
    os << idtr <<
      "Multiple rests contents: none" <<
      endl;
  }

  else {
    os <<
      fMultipleRestContents;
  }
      
  idtr--;
}

//______________________________________________________________________________ 
int msrVoice::gVoicesCounter = 0;

S_msrVoice msrVoice::create (
  int          inputLineNumber,
  S_msrPart    voiceDirectPartUplink,
  msrVoiceKind voiceKind,
  int          externalVoiceNumber,
  S_msrStaff   voiceStaffUplink)
{
  msrVoice* o =
    new msrVoice (
      inputLineNumber,
      voiceDirectPartUplink,
      voiceKind,
      externalVoiceNumber,
      voiceStaffUplink);
  assert(o!=0);
    
  return o;
}

// for regular voices JMI
msrVoice::msrVoice (
  int          inputLineNumber,
  S_msrPart    voiceDirectPartUplink,
  msrVoiceKind voiceKind,
  int          externalVoiceNumber,
  S_msrStaff   voiceStaffUplink)
    : msrElement (inputLineNumber)
{
  fVoiceAbsoluteNumber = ++gVoicesCounter;
  
  // set voice's direct part uplink
  msrAssert(
    voiceDirectPartUplink != 0,
    "voiceDirectPartUplink is null");
    
  fVoiceDirectPartUplink =
    voiceDirectPartUplink;
    
  // set voice staff uplink
  msrAssert(
    voiceStaffUplink != 0,
    "voiceStaffUplink is null");

  fVoiceStaffUplink = voiceStaffUplink;

  fVoiceKind = voiceKind;
  
  fExternalVoiceNumber = externalVoiceNumber;
  
  // initialize the voice
  initializeVoice ();
}

msrVoice::~msrVoice() {}

void msrVoice::initializeVoice ()
{
  fStaffRelativeVoiceNumber = fExternalVoiceNumber;
    // may be changed afterwards JMI ???

  // compute voice number
  int voiceNumber =
    gMsrOptions-> fCreateStaffRelativeVoiceNumbers // JMI use
      ? fStaffRelativeVoiceNumber
      : fExternalVoiceNumber;
  
  // set voice name
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
      fVoiceName =
        fVoiceStaffUplink->getStaffName() +
        "_Voice_" +
        int2EnglishWord (voiceNumber);
      break;
      
    case msrVoice::kHarmonyVoice:
      fVoiceName =
        fVoiceStaffUplink->getStaffName() +
        "_HarmonyVoice";
      break;
      
    case msrVoice::kMasterVoice:
      fVoiceName =
        fVoiceStaffUplink->getStaffName() +
        "_MasterVoice";
      break;
  } // switch
  
  /*
    CAUTION:
      the following makes the lilypond not compilable, but may be useful
      to disambiguate the voice and voice clones names

  fVoiceName += // JMI
    "[" + to_string (fVoiceAbsoluteNumber) + "]";
  */

  if (gGeneralOptions->fTraceVoices)
    cerr << idtr <<
      "Creating voice \"" << fVoiceName <<
      "\" in staff \"" <<
      fVoiceStaffUplink->getStaffName () <<
      "\"" <<
      endl;

  // check external voice number
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
      // the external voice number should not be negative
      // (-99 is used for the staves master voices)
      if (fExternalVoiceNumber < 0) {
        stringstream s;
    
        s <<
          "regular voice number " << fExternalVoiceNumber <<
          " is not in the 0..4 range";
          
        msrMusicXMLError (
          fInputLineNumber, s.str());
      }
      break;
      
    case msrVoice::kHarmonyVoice:
      if (fExternalVoiceNumber != -1) {
        stringstream s;
    
        s <<
          "harmony voice number " << fExternalVoiceNumber <<
          " is not equal to -1";
          
        msrInternalError (
          fInputLineNumber, s.str());
      }
      break;
      
    case msrVoice::kMasterVoice:
      if (fExternalVoiceNumber != -99) {
        stringstream s;
    
        s <<
          "master voice number " << fExternalVoiceNumber <<
          " is not equal to " << -99;
          
        msrInternalError (
          fInputLineNumber, s.str());
      }
      break;
  } // switch

  // set voice number
  fVoiceMeasureNumber = // JMI "??";
    fVoiceDirectPartUplink->
      getPartMeasureNumber ();

  // music has not been inserted in voice yet
  fMusicHasBeenInsertedInVoice = false;

  // counters
  fVoiceActualNotesCounter     = 0;
  fVoiceActualHarmoniesCounter = 0;

  // get the initial staff details from the staff if any
  S_msrStaffDetails
    staffStaffDetails =
      fVoiceStaffUplink->
        getCurrentStaffStaffDetails ();

  if (staffStaffDetails) {
    // append it to the voice
    appendStaffDetailsToVoice (staffStaffDetails);
  }
    
  // add the master stanza for this voice,
  // to collect skips along the way that are used as a 'prelude'
  // by actual stanza that start at later points
  fVoiceMasterStanza =
    msrStanza::create (
      fInputLineNumber,
      fVoiceDirectPartUplink,
      0,    // this stanza number is unused anyway
      msrStanza::kMasterStanza,
      this);
}

void msrVoice::appendAFirstMeasureToVoiceIfNeeded (
  int inputLineNumber)
{
  if (! fVoiceLastSegment) {
    // create the initial segment for this voice
    if (gGeneralOptions->fTraceSegments)
      cerr << idtr <<
        "Creating the initial last segment for voice \"" <<
        getVoiceName () << "\"" <<
        endl;
        
    fVoiceLastSegment =
      msrSegment::create (
        fInputLineNumber,
        fVoiceDirectPartUplink,
        this);

    // create and append a new measure to last segment
    fVoiceLastSegment->
      createAndAppendMeasureToSegment (
        inputLineNumber,
        fVoiceMeasureNumber);
      
    // register voice first segment for LilyPond issue 34
    fVoiceFirstSegment =
      fVoiceLastSegment;
  
    // get the initial clef from the staff if any
    {
      S_msrClef
        clef =
          fVoiceStaffUplink->getStaffCurrentClef ();
    
      if (clef) {
        // append it to the last segment
        fVoiceLastSegment->
          appendClefToSegment (clef);
        }
    }
      
    // get the initial key from the staff if any
    {
      S_msrKey
        key =
          fVoiceStaffUplink->getStaffCurrentKey ();
  
      if (key) {
        // append it to the last segment
        fVoiceLastSegment->
          appendKeyToSegment (key);
      }
    }
    
    // get the initial time from the staff if any
    {
      S_msrTime
        time =
          fVoiceStaffUplink->getStaffCurrentTime ();
  
      if (time) {
        // append it to the last segment
        fVoiceLastSegment->
          appendTimeToSegment (time);
      }
    }
    
    // get the initial transpose from the staff if any
    {
      S_msrTranspose
        transpose =
          fVoiceStaffUplink->getStaffTranspose ();
          
      if (transpose) {
        // append it to the last segment
        S_msrElement t = transpose;
        fVoiceLastSegment->
          appendTransposeToSegment (transpose); //JMI
      }
    }
  }

  else {
    if (! fVoiceFirstSegment) {
      // register voice first segment for LilyPond issue 34
      fVoiceFirstSegment =
        fVoiceLastSegment;
    }
  }
}
  
S_msrVoice msrVoice::createVoiceShallowClone (S_msrStaff staffClone)
{
  if (gGeneralOptions->fTraceVoices) {
    cerr << idtr <<
      "Creating a shallow clone of voice \"" <<
      getVoiceName () <<
      "\"" <<
      endl;
  }

  msrAssert(
    staffClone != 0,
    "staffClone is null");
    
  S_msrVoice
    clone =
      msrVoice::create (
        fInputLineNumber,
        staffClone->
          getStaffDirectPartUplink (),
        fVoiceKind,
        fExternalVoiceNumber,
        staffClone);

  // populate the voice clone
  clone->fVoiceName =
    fVoiceName;

  // set external voice number
  clone->fExternalVoiceNumber =
    fExternalVoiceNumber;

  // has music been inserted in voice?
  clone->fMusicHasBeenInsertedInVoice =
    fMusicHasBeenInsertedInVoice;
  
  return clone;
}

string msrVoice::getVoiceName () const
{
  return fVoiceName;

  /* 
  int voiceNumber =
    gMsrOptions-> fCreateStaffRelativeVoiceNumbers // JMI use
      ? fStaffRelativeVoiceNumber
      : fExternalVoiceNumber;

  string suffix =
    fStaffRelativeVoiceNumber == 0
      ? "MASTER"
      : int2EnglishWord (voiceNumber);
      
  return
    fVoiceStaffUplink->getStaffName() +
    "_Voice_" +
    suffix;
    */
}

void msrVoice::createAndAppendMeasureToVoice (
  int    inputLineNumber,
  string measureNumber)
{
  fVoiceMeasureNumber = measureNumber;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNeeded (
    inputLineNumber);

  // set the last segment measure number
  fVoiceLastSegment->
    createAndAppendMeasureToSegment (
      inputLineNumber,
      measureNumber);
}

/* JMI
bool msrVoice::checkForIncompleteVoiceLastMeasure (
  int inputLineNumber)
{
  if (gGeneralOptions->fTraceVoices || gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Checking for incomplete last measure in voice \"" <<
      getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;

  return
    fVoiceLastSegment->
      checkForIncompleteSegmentLastMeasure (
        inputLineNumber,
        msrMeasure::kUnderfullMeasureKind);
}
*/

void msrVoice::createNewLastSegmentForVoice (
  int inputLineNumber)
{
  /* JMI
  // check for incomplete last measure
  // before creating the new last measure
  bool lastMeasureIsIncomplete =
    checkForIncompleteVoiceLastMeasure (
      inputLineNumber);
    */
    
  // create the segment
  if (gGeneralOptions->fTraceVoices) {
    cerr << idtr <<
      "Creating a new segment for voice \"" <<
      getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }

  fVoiceLastSegment =
    msrSegment::create (
      inputLineNumber,
      fVoiceDirectPartUplink,
      this);

/* JMI BOF
  // the new last measure keeps the measure number
  // of the preceeding one whenever the latter is incomplete
  if (! lastMeasureIsIncomplete)
    // increment new segment's first measure's number
    fVoiceLastSegment->
      incrementSegmentLastMeasureNumber ( // JMI
        inputLineNumber);
        */
}

void msrVoice::createNewLastSegmentWithFirstMeasureForVoice (
  int          inputLineNumber,
  S_msrMeasure firstMeasure)
{
  // create the segment
  if (gGeneralOptions->fTraceVoices) {
    cerr << idtr <<
      "Creating a new segment with first measure '" <<
      firstMeasure->getMeasureNumber () <<
      "'for voice \"" <<
      getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }

  fVoiceLastSegment =
    msrSegment::createWithFirstMeasure (
      inputLineNumber,
      fVoiceDirectPartUplink,
      this,
      firstMeasure);
}

S_msrStanza msrVoice::addStanzaToVoiceByItsNumber (
  int inputLineNumber,
  int stanzaNumber)
{
  if (fVoiceStanzasMap.count (stanzaNumber)) {
    stringstream s;
    
    s <<
      "stanza " << stanzaNumber <<
      " already exists in voice \"" <<
      getVoiceName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;

    msrInternalError (
      inputLineNumber,
      s.str());

// JMI    return fVoiceStanzasMap [stanzaNumber];
  }

  // create the stanza
  S_msrStanza
    stanza =
      msrStanza::create (
        inputLineNumber,
        fVoiceDirectPartUplink,
        stanzaNumber,
        msrStanza::kRegularStanza,
        this);

  // add the stanza to this voice
  addStanzaToVoiceWithCatchUp (stanza);

  // return it
  return stanza;
}

void msrVoice::addStanzaToVoiceWithoutCatchUp (S_msrStanza stanza)
{
  // get stanza number
  int stanzaNumber =
    stanza->getStanzaNumber ();
    
  // register stanza in this voice
  if (gGeneralOptions->fTraceLyrics) {
    cerr << idtr <<
      "Adding stanza " << stanza->getStanzaName () <<
      " (" << stanzaNumber <<
      ") to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // add the stanza to this voice
  fVoiceStanzasMap [stanzaNumber] = stanza;
}

void msrVoice::catchUpWithVoiceMasterStanza (S_msrStanza stanza)
{
  vector<S_msrSyllable>
    masterSyllables =
      fVoiceMasterStanza->
        getSyllables ();

  if (masterSyllables.size()) {
    if (gGeneralOptions->fTraceLyrics)
      cerr << idtr <<
        "Copying current contents of voice master stanza to " <<
        stanza->getStanzaName () <<
        endl;
        
    for (
      vector<S_msrSyllable>::const_iterator i =
        masterSyllables.begin();
      i != masterSyllables.end();
      i++) {
      // add syllable to stanza
      stanza->appendSyllableToStanza ((*i));
    } // for
  }
}

void msrVoice::addStanzaToVoiceWithCatchUp (S_msrStanza stanza)
{
  // get stanza number
  int stanzaNumber =
    stanza->getStanzaNumber ();
    
  // register stanza in this voice
  if (gGeneralOptions->fTraceLyrics) {
    cerr << idtr <<
      "Adding stanza " << stanza->getStanzaName () <<
      " (" << stanzaNumber <<
      ") to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  fVoiceStanzasMap [stanzaNumber] = stanza;

  // catch up with fVoiceMasterStanza
  // in case the stanza does not start upon the first voice note
  catchUpWithVoiceMasterStanza (stanza);
}

S_msrStanza msrVoice::createStanzaInVoiceIfNeeded (
  int inputLineNumber,
  int stanzaNumber)
{
  if (gGeneralOptions->fTraceVoices || gGeneralOptions->fTraceLyrics)
    cerr << idtr <<
      "### % --> createStanzaInVoiceIfNeeded (" << inputLineNumber <<
      ", " << stanzaNumber << ")" <<
      ", fVoiceStanzasMap.size() = " << fVoiceStanzasMap.size () <<
      endl;

  S_msrStanza stanza;

  // is stanzaNumber already known in voice?
  if (fVoiceStanzasMap.count (stanzaNumber)) {
    // yes, use it
    stanza = fVoiceStanzasMap [stanzaNumber];
  }
  
  else {
    // no, add it to the voice
    stanza =
      addStanzaToVoiceByItsNumber (
        inputLineNumber, stanzaNumber);
  }

  return stanza;
}

void msrVoice::appendClefToVoice (S_msrClef clef)
{
  if (gMsrOptions->fTraceMsr)
    cerr << idtr <<
      "Appending clef '" << clef->clefAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNeeded (
    clef->getInputLineNumber ());

  // append clef to last segment
  fVoiceLastSegment->
    appendClefToSegment (clef);
}

void msrVoice::appendKeyToVoice (S_msrKey key)
{
  if (gGeneralOptions->fTraceKeys || gGeneralOptions->fTraceVoices)
    cerr << idtr <<
      "Appending key '" << key->keyAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNeeded (
    key->getInputLineNumber ());

  // append key to last segment
  fVoiceLastSegment->
    appendKeyToSegment (key);
}

void msrVoice::appendTimeToVoice (S_msrTime time)
{
  if (gGeneralOptions->fTraceTimes || gGeneralOptions->fTraceVoices)
    cerr << idtr <<
      "Appending time '" << time->timeAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  // register time in voice
  fVoiceCurrentTime = time;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNeeded (
    time->getInputLineNumber ());

  // append time to the last segment
  fVoiceLastSegment->
    appendTimeToSegment (time);
}

void msrVoice::appendHarmonyToVoice (S_msrHarmony harmony)
{
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceVoices)
    cerr << idtr <<
      "Appending harmony '" << harmony->harmonyAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
      
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
      {
        stringstream s;

        s <<
          "cannot append a harmony to " <<
          voiceKindAsString () <<
          " voice \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          harmony->getInputLineNumber (),
          s.str());
      }
      break;
      
    case msrVoice::kHarmonyVoice:
      // create the voice last segment and first measure if needed
      appendAFirstMeasureToVoiceIfNeeded (
        harmony->getInputLineNumber ());

      fVoiceLastSegment->
        appendHarmonyToSegment (harmony);
    
      // register harmony
      fVoiceActualHarmoniesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrVoice::kMasterVoice:
      {
        stringstream s;

        s <<
          "cannot append a harmony to " <<
          voiceKindAsString () <<
          " voice \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          harmony->getInputLineNumber (),
          s.str());
      }
      break;
  } // switch
}

void msrVoice::appendHarmonyToVoiceClone (S_msrHarmony harmony)
{
  if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceVoices)
    cerr << idtr <<
      "Appending harmony '" << harmony->harmonyAsString () <<
      "' to voice clone \"" << getVoiceName () << "\"" <<
      endl;
      
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
      {
        stringstream s;

        s <<
          "cannot append a harmony to " <<
          voiceKindAsString () <<
          " voice \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          harmony->getInputLineNumber (),
          s.str());
      }
      break;
      
    case msrVoice::kHarmonyVoice:
      // create the voice last segment and first measure if needed
      appendAFirstMeasureToVoiceIfNeeded (
        harmony->getInputLineNumber ());

      fVoiceLastSegment->
        appendHarmonyToSegmentClone (harmony);
    
      // register harmony
      fVoiceActualHarmoniesCounter++;
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrVoice::kMasterVoice:
      {
        stringstream s;

        s <<
          "cannot append a harmony to " <<
          voiceKindAsString () <<
          " voice \"" <<
          getVoiceName () <<
          "\"";

        msrInternalError (
          harmony->getInputLineNumber (),
          s.str());
      }
      break;
  } // switch
}

void msrVoice::bringVoiceToMeasurePosition (
  int inputLineNumber,
  int measurePosition)
{
  if (gGeneralOptions->fTraceVoices || gGeneralOptions->fTraceDivisions)
    cerr << idtr <<
      "Bringing measure position for voice \"" <<
      getVoiceName () <<
      "\" to " << measurePosition <<
      ", line " << inputLineNumber <<
      endl;

  fVoiceLastSegment->
    bringSegmentToMeasurePosition (
      inputLineNumber, measurePosition);
}

void msrVoice::appendTransposeToVoice (S_msrTranspose transpose)
{
  if (gGeneralOptions->fTraceVoices || gMsrOptions->fTraceMsr)
    cerr << idtr <<
      "Appending transpose '" <<
      transpose->transposeAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNeeded (
    transpose->getInputLineNumber ());

  fVoiceLastSegment->
    appendTransposeToSegment (transpose);
}

void msrVoice::appendStaffDetailsToVoice (
  S_msrStaffDetails staffDetails)
{
  if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceVoices)
    cerr << idtr <<
      "Appending staff details '" <<
      staffDetails->staffDetailsAsShortString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

/* JMI
  // register staff details in voice
  fCurrentVoicefStaffDetails = staffDetails;
  */
  
 // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNeeded (
    staffDetails->getInputLineNumber ());

  fVoiceLastSegment->
    appendStaffDetailsToSegment (staffDetails);
}

/* JMI
void msrVoice::appendWordsToVoice (S_msrWords words)
{
  if (gGeneralOptions->fTraceWords)
    cerr << idtr <<
      "Appending words '" << words->wordsAsString () <<
      "' to voice " << getVoiceName () << endl;

  S_msrElement w = words;
  fVoiceLastSegment->
    appendWordsToSegment (w);
}
*/

void msrVoice::appendTempoToVoice (S_msrTempo tempo)
{
  if (gMsrOptions->fTraceMsr)
    cerr << idtr <<
      "Appending tempo '" << tempo->tempoAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNeeded (
    tempo->getInputLineNumber ());

  fVoiceLastSegment->
    appendTempoToSegment (tempo);
}

void msrVoice::appendOctaveShiftToVoice (S_msrOctaveShift octaveShift)
{
  if (gMsrOptions->fTraceMsr)
    cerr << idtr <<
      "Appending octave shift '" <<
      octaveShift->octaveShiftKindAsString () <<
      "', size: " << octaveShift->getOctaveShiftSize () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNeeded (
    octaveShift->getInputLineNumber ());

  fVoiceLastSegment->
    appendOctaveShiftToSegment (octaveShift);
}

void msrVoice::appendRehearsalToVoice (S_msrRehearsal rehearsal)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Appending rehearsal '" << rehearsal->getRehearsalText () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNeeded (
    rehearsal->getInputLineNumber ());

  fVoiceLastSegment->
    appendRehearsalToSegment (rehearsal);
}

void msrVoice::appendNoteToVoice (S_msrNote note) {
  if (gGeneralOptions->fTraceNotes) {
    cerr << idtr <<
      "Appending note:" <<
      endl;

    idtr++;
    
    cerr <<
      idtr <<
        note <<
      idtr <<
        "to voice \"" << getVoiceName () << "\"" <<
        endl;

    idtr--;
  }

  // register actual note
  fVoiceActualNotesCounter++;

  // register whether music (i.e. not just skips)
  // has been inserted into the voice
  switch (note->getNoteKind ()) {
    case msrNote::kRestNote:
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kSkipNote:
      // don't account skips as music
      break;
      
    case msrNote::kStandaloneNote:
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kDoubleTremoloMemberNote:
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kGraceNote:
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kChordMemberNote:
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kTupletMemberNote:
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNote::k_NoNoteKind:
      break;
  } // switch
  
  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNeeded (
    note->getInputLineNumber ());

  // append the note to the last segment
  fVoiceLastSegment->
    appendNoteToSegment (note);
  
  // add a skip syllable of the same duration to the master stanza
  int
    noteSoundingDivisions =
      note->getNoteSoundingDivisions ();

  if (note->getNoteIsARest ())
    fVoiceMasterStanza->
      appendRestSyllableToStanza (
        note->getInputLineNumber (),
        noteSoundingDivisions);
  else
    fVoiceMasterStanza->
      appendSkipSyllableToStanza (
        note->getInputLineNumber (),
        noteSoundingDivisions);
}

void msrVoice::appendNoteToVoiceClone (S_msrNote note) {
  if (gGeneralOptions->fTraceNotes) {
    cerr << idtr <<
      "Appending note:" <<
      endl;

    idtr++;
    
    cerr <<
      idtr <<
        note <<
      idtr <<
        "to voice clone \"" << getVoiceName () << "\"" <<
        endl;

    idtr--;
  }

  // register actual note
  fVoiceActualNotesCounter++;

  // register whether music (i.e. not just skips)
  // has been inserted into the voice
  switch (note->getNoteKind ()) {
    case msrNote::kRestNote:
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kSkipNote:
      // don't account skips as music
      break;
      
    case msrNote::kStandaloneNote:
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kDoubleTremoloMemberNote:
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kGraceNote:
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kChordMemberNote:
      fMusicHasBeenInsertedInVoice = true;
      break;
      
    case msrNote::kTupletMemberNote:
      fMusicHasBeenInsertedInVoice = true;
      break;

    case msrNote::k_NoNoteKind:
      break;
  } // switch
  
  // append the note to the last segment
  fVoiceLastSegment->
    appendNoteToSegmentClone (note);
  
  // add a skip syllable of the same duration to the master stanza
  int
    noteSoundingDivisions =
      note->getNoteSoundingDivisions ();

  if (note->getNoteIsARest ())
    fVoiceMasterStanza->
      appendRestSyllableToStanza (
        note->getInputLineNumber (),
        noteSoundingDivisions);
  else
    fVoiceMasterStanza->
      appendSkipSyllableToStanza (
        note->getInputLineNumber (),
        noteSoundingDivisions);
}

void msrVoice::appendDoubleTremoloToVoice (
  S_msrDoubleTremolo doubleTremolo)

{
  if (gGeneralOptions->fTraceTremolos) {
    cerr << idtr <<
      "Appending double tremolo '" <<
      doubleTremolo->doubleTremoloAsShortString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNeeded (
    doubleTremolo->getInputLineNumber ());

  fVoiceLastSegment->
    appendDoubleTremoloToSegment (doubleTremolo);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendChordToVoice (S_msrChord chord)
{
  if (gGeneralOptions->fTraceChords) {
    cerr << idtr <<
      "Appending chord '" << chord->chordAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNeeded (
    chord->getInputLineNumber ());

  fVoiceLastSegment->
    appendChordToSegment (chord);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendTupletToVoice (S_msrTuplet tuplet)
{
  if (gGeneralOptions->fTraceTuplets) {
    cerr << idtr <<
      "Appending tuplet '" << tuplet->tupletAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNeeded (
    tuplet->getInputLineNumber ());

  fVoiceLastSegment->
    appendTupletToSegment (tuplet);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendOtherElementToVoice (S_msrElement elem) {
  if (gGeneralOptions->fTraceVoices) {
    cerr << idtr <<
      "Appending other element '" << elem <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNeeded (
    elem->getInputLineNumber ());

  fVoiceLastSegment->
    appendOtherElementToSegment (elem);
}

void msrVoice::appendGraceNotesToVoice (S_msrGraceNotes graceNotes)
{
  if (gGeneralOptions->fTraceGraceNotes) {
    cerr << idtr <<
      "Appending grace notes " << // JMI graceNotes <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNeeded (
    graceNotes->getInputLineNumber ());

  fVoiceLastSegment->
    appendGraceNotesToSegment (graceNotes);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::prependGraceNotesToVoice (S_msrGraceNotes graceNotes)
{
  if (gGeneralOptions->fTraceGraceNotes) {
    cerr << idtr <<
      "Prepending grace notes " << // JMI graceNotes <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNeeded (
    graceNotes->getInputLineNumber ());

  fVoiceFirstSegment->
    prependGraceNotesToSegment (graceNotes);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendAfterGraceNotesToVoice (
  S_msrAfterGraceNotes afterGraceNotes)
{
  if (gGeneralOptions->fTraceGraceNotes) {
    cerr << idtr <<
      "Appending after grace notes " << // JMI AfterGraceNotes <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNeeded (
    afterGraceNotes->getInputLineNumber ());

  fVoiceLastSegment->
    appendAfterGraceNotesToSegment (afterGraceNotes);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::prependAfterGraceNotesToVoice (
  S_msrAfterGraceNotes afterGraceNotes)
{
  if (gGeneralOptions->fTraceGraceNotes) {
    cerr << idtr <<
      "Prepending after grace notes " << // JMI AfterGraceNotes <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNeeded (
    afterGraceNotes->getInputLineNumber ());

  fVoiceFirstSegment->
    prependAfterGraceNotesToSegment (afterGraceNotes);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendSyllableToVoice (
  int           inputLineNumber,
  int           stanzaNumber,
  S_msrSyllable syllable)
{
  // append syllable to this voice
  if (gGeneralOptions->fTraceLyrics) {
    cerr << idtr <<
      "Appending syllable '" <<
      syllable->syllableAsString () <<
      "' to voice \"" << fVoiceName << "\"" <<
      endl;
  }

  // fetch stanzaNumber in this voice
  S_msrStanza
    stanza =
      createStanzaInVoiceIfNeeded (
        inputLineNumber, stanzaNumber);

  // add the syllable to the stanza
  stanza->
    appendSyllableToStanza (
      syllable);
}

void msrVoice::appendBarCheckToVoice (S_msrBarCheck barCheck)
{
  if (gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Appending bar check '" << barCheck->barCheckAsString () <<
      "' to voice \"" << getVoiceName () <<  "\"" <<
      endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNeeded (
    barCheck->getInputLineNumber ());

  fVoiceLastSegment->
    appendBarCheckToSegment (barCheck);

  // add bar check syllable to the voice master stanza
  fVoiceMasterStanza->
    appendBarcheckSyllableToStanza (
      barCheck->getInputLineNumber (),  // [passer barCheck directement? JMI
      barCheck->getNextBarNumber ());
}

void msrVoice::appendBarNumberCheckToVoice (
  S_msrBarNumberCheck barNumberCheck)
{
  if (gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Appending barnumber check '" <<
      barNumberCheck->barNumberCheckAsString () <<
      "' to voice \"" << getVoiceName () <<  "\"" <<
      endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNeeded (
    barNumberCheck->getInputLineNumber ());

  fVoiceLastSegment->
    appendBarNumberCheckToSegment (barNumberCheck);

  // add barnumber check syllable to the voice master stanza
  fVoiceMasterStanza->
    appendBarNumberCheckSyllableToStanza (
      barNumberCheck->getInputLineNumber (),  // [passer barNumberCheck directement? JMI
      barNumberCheck->getNextBarNumber ());
}

void msrVoice::appendBreakToVoice (S_msrBreak break_)
{
  if (gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Appending break '" << break_->breakAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  // create the voice last segment and first measure if needed
  appendAFirstMeasureToVoiceIfNeeded (
    break_->getInputLineNumber ());

  fVoiceLastSegment->
    appendBreakToSegment (break_);

  // add break syllable to the voice master stanza
  fVoiceMasterStanza->
    appendBreakSyllableToStanza (
      break_->getInputLineNumber (),
      break_->getNextBarNumber ());
}

void msrVoice::createAndAppendRepeatToVoice (int inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
      {
        // create a repeat
        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "Creating a repeat in voice \"" << getVoiceName () <<  "\"" <<
            ", line " << inputLineNumber <<
            endl;
      
        S_msrRepeat
          repeat =
            msrRepeat::create (
              inputLineNumber,
              this);
      
        // set current last segment as the repeat common segment
        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "Setting current last segment as repeat common segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
      
        repeat->
          setRepeatCommonSegment (
            fVoiceLastSegment);
          
        // register repeat as the (new) current repeat
        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "Registering repeat as the new current repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
                
        fVoiceCurrentRepeat =
          repeat;
      
        // append it to the list of repeats and segments
        fVoiceInitialRepeatsAndSegments.push_back (
          repeat);
      
        // create a new segment for the voice
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          cerr << idtr <<
            "Creating a new last segment for voice \"" <<
            fVoiceName << "\"" <<
            endl;
            
        createNewLastSegmentForVoice (
          inputLineNumber);
      }
      break;
            
    case msrVoice::kMasterVoice:
      break;
  } // switch
}

void msrVoice::createMeasureRepeatFromItsFirstMeasureInVoice (
  int inputLineNumber,
  int measureRepeatMeasuresNumber,
  int measureRepeatSlashes)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
      {
        // create a measure repeat
        if (gGeneralOptions->fTraceRepeats) {
          cerr << idtr <<
            "Creating a measure repeat from it's first measure in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }
      
        // grab the just created last measure from the voice,
        // (i.e. the one containing:
        //   <measure-repeat ... type="start">2</measure-repeat>)
        // which is the first replica measure
        S_msrMeasure
          firstReplicaMeasure =
            removeLastMeasureFromVoice (
              inputLineNumber);

        // grab the (new current) last measure from the voice,
        // which is the repeated measure
        S_msrMeasure
          repeatedMeasure =
            removeLastMeasureFromVoice (
              inputLineNumber);

        // create the measure repeat repeated segment
        S_msrSegment
          repeatedSegment =
            msrSegment::create (
              inputLineNumber,
              fVoiceDirectPartUplink,
              this);

        // append the repeated measure to the repeated segment
        repeatedSegment->
          appendMeasureToSegment (
            repeatedMeasure);
            
        // create the measure pattern
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          cerr << idtr <<
            "Creating a measure pattern for voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

        S_msrMeasureRepeatPattern
          measureRepeatPattern =
            msrMeasureRepeatPattern::create (
              inputLineNumber,
              this);


        // set the repeated segment as the measure repeat pattern segment
        measureRepeatPattern->
          setMeasureRepeatPatternSegment (
            repeatedSegment);
            
        // create the measure repeat
        if (fVoicePendingMeasureRepeat) {
          stringstream s;

          s <<
            "attempting to create a measure repeat while another one is pending";

          msrInternalError (
            inputLineNumber, s.str());
        }
        
        fVoicePendingMeasureRepeat =
          msrMeasureRepeat::create (
            inputLineNumber,
            measureRepeatMeasuresNumber,
            measureRepeatSlashes,
            this);

        // set the measure repeat pattern
        fVoicePendingMeasureRepeat->
          setMeasureRepeatPattern (
            measureRepeatPattern);
        
        // create a new last segment to collect the measure repeat replicas,
        // containing the first, yet incomplete, replica
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          cerr << idtr <<
            "Creating a new last segment with the first replica for voice \"" <<
            fVoiceName << "\"" <<
            endl;
            
        createNewLastSegmentWithFirstMeasureForVoice (
          inputLineNumber,
          firstReplicaMeasure);

        // print resulting voice contents
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          cerr << idtr <<
            "The resulting voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

        idtr++;
        print (cerr);
        idtr--;

        // keep the measure repeat pending
      }
      break;
            
    case msrVoice::kMasterVoice:
      break;
  } // switch
}

void msrVoice::appendPendingMeasureRepeatToVoice (
  int inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
      {
        /* JMI
        // print current voice contents
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          cerr << idtr <<
            "==================> The current voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl <<
            idtr;

        idtr++;
        print (cerr);
        idtr--;
*/

        // does the pending measure repeat exist?
        if (! fVoicePendingMeasureRepeat) {
          stringstream s;

          s <<
            "attempting to append a pending measure repeat which doesn't exist";

          msrInternalError (
            inputLineNumber, s.str());
        }

        // fetch the last segment's measure list
        list<S_msrMeasure>&
          voiceLastSegmentMeasureList =
            fVoiceLastSegment->
              getSegmentMeasuresListToModify ();
       
        // grab the just created last measure in the last segment's measure list,
        // (i.e. the one containing:
        //   <measure-repeat type="stop"/>)
        // which is the next measure after the measure repeat
        if (! voiceLastSegmentMeasureList.size ()) {
          stringstream s;

          s <<
            "attempting to grab first measure of voice last segment, that contains none";

          msrInternalError (
            inputLineNumber, s.str());
        }

        S_msrMeasure
          nextMeasureAfterMeasureRepeat =
            voiceLastSegmentMeasureList.back ();

            /* JMI
        cerr <<
          endl <<
          "==========> nextMeasureAfterMeasureRepeat:" <<
          endl;
          
        nextMeasureAfterMeasureRepeat->print (cerr);
          
        cerr <<
          endl;
*/

        // remove the next measure from the last segment's measure list
        voiceLastSegmentMeasureList.pop_back ();

         // create the measure repeat replicas contents
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          cerr << idtr <<
            "Creating a measure repeat replicas contents for voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

        S_msrMeasureRepeatReplicas
          measureRepeatReplicas =
            msrMeasureRepeatReplicas::create (
              inputLineNumber,
              this);

        // set the voice last segment as the measure repeat replicas segment
        measureRepeatReplicas->
          setMeasureRepeatReplicasSegment (
            fVoiceLastSegment);

        // set the measure repeat replicas contents
        fVoicePendingMeasureRepeat->
          setMeasureRepeatReplicas (
            measureRepeatReplicas);
        

/* JMI BOF
       // set last segment as the measure repeat replicas segment
        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "Setting current last segment as measure repeat replicas segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
      
        fVoicePendingMeasureRepeat->
          setMeasureRepeatReplicas (
            fVoiceLastSegment);

        // append pending measure repeat to the list of repeats and segments
        fVoiceInitialRepeatsAndSegments.push_back (
          fVoicePendingMeasureRepeat);

        // set multipleRestContents as the multiple rest contents
        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "Setting current last segment as multiple rest segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
      
        fVoicePendingMultipleRest->
          setMultipleRestContents (
            multipleRestContents);
*/





/* JMI
        // print current voice contents
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          cerr << idtr <<
            "==================> The current voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl <<
            idtr;

        idtr++;
        print (cerr);
        idtr--;
        */

        // create a new last segment to collect the remainder of the voice,
        // containing the next, yet incomplete, measure
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          cerr << idtr <<
            "Creating a new last segment with the measure repeat next measure for voice \"" <<
            fVoiceName << "\"" <<
            endl;
            
        createNewLastSegmentWithFirstMeasureForVoice (
          inputLineNumber,
          nextMeasureAfterMeasureRepeat);

        // print resulting voice contents
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          cerr << idtr <<
            "The resulting voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

        idtr++;
        print (cerr);
        idtr--;

        // forget about this pending measure repeat
        fVoicePendingMeasureRepeat = 0;
      }
      break;
            
    case msrVoice::kMasterVoice:
      break;
  } // switch
}

void msrVoice::createMultipleRestInVoice (
  int inputLineNumber,
  int multipleRestMeasuresNumber)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
      {
        // create a multiple rest
        if (gGeneralOptions->fTraceRepeats) {
          cerr << idtr <<
            "Creating a multiple rest in voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            ", " <<
            singularOrPlural (
              multipleRestMeasuresNumber, "measure", "measures") <<
            endl;
        }
      
        // grab the just created last measure from the voice,
        // (i.e. the one containing:
        //   <multiple-rest ... type="start">2</multiple-rest>)
        // which is the first rest measure
        S_msrMeasure
          firstRestMeasure =
            removeLastMeasureFromVoice (
              inputLineNumber);

/* JMI
        // create the multiple rest rests segment
        S_msrSegment
          restsSegment =
            msrSegment::create (
              inputLineNumber,
              fVoiceDirectPartUplink,
              this);
*/

        // create the multiple rest
        if (fVoicePendingMultipleRest) {
          stringstream s;

          s <<
            "attempting to create a multiple rest while another one is pending";

          msrInternalError (
            inputLineNumber, s.str());
        }
        
        fVoicePendingMultipleRest =
          msrMultipleRest::create (
            inputLineNumber,
            multipleRestMeasuresNumber,
            this);

        // create a new segment to collect the multiple rest measures,
        // containing the first, rest measure
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          cerr << idtr <<
            "Creating a new last segment containing the first rest measure in voice \"" <<
            fVoiceName << "\"" <<
            endl;
            
        createNewLastSegmentWithFirstMeasureForVoice (
          inputLineNumber,
          firstRestMeasure);

        // append the first rest measure to the new last segment
        fVoiceLastSegment->
          appendMeasureToSegment (
            firstRestMeasure);
            
        // print resulting voice contents
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          cerr << idtr <<
            "The resulting voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

        idtr++;
        print (cerr);
        idtr--;

        // keep the multiple rest pending
      }
      break;
      
    case msrVoice::kMasterVoice:
      break;
  } // switch
}

void msrVoice::appendPendingMultipleRestToVoice (
  int inputLineNumber)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
      {
        if (gGeneralOptions->fTraceRepeats) {
          cerr << idtr <<
            "Appending the pending multiple rest to voice \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }

/*
        // print current voice contents
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          cerr << idtr <<
            "==================> The current voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl <<
            idtr;

        idtr++;
        print (cerr);
        idtr--;
*/

        // does the pending multiple rest exist?
        if (! fVoicePendingMultipleRest) {
          stringstream s;

          s <<
            "attempting to append a pending multiple rest which doesn't exist";

          msrInternalError (
            inputLineNumber, s.str());
        }

        // fetch the last segment's measure list
        list<S_msrMeasure>&
          voiceLastSegmentMeasureList =
            fVoiceLastSegment->
              getSegmentMeasuresListToModify ();
       
        // grab the just created last measure in the last segment's measure list,
        // which is the next measure after the multiple rest
        if (! voiceLastSegmentMeasureList.size ()) {
          stringstream s;

          s <<
            "attempting to grab first measure of voice last segment, that contains none";

          msrInternalError (
            inputLineNumber, s.str());
        }

        S_msrMeasure
          nextMeasureAfterMultipleRest =
            voiceLastSegmentMeasureList.back ();

        /* JMI
        cerr <<
          endl <<
          "==========> nextMeasureAfterMultipleRest:" <<
          endl;

          
        nextMeasureAfterMultipleRest->
          print (cerr);
          
        cerr <<
          endl;
          */
          
        // remove the next measure from the last segment's measure list
        voiceLastSegmentMeasureList.pop_back ();

        // create the multiple rest contents
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          cerr << idtr <<
            "Creating a measure pattern for voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

        S_msrMultipleRestContents
          multipleRestContents =
            msrMultipleRestContents::create (
              inputLineNumber,
              this);

        // set voice last segment as the multiple rest segment
        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "Setting current last segment as multiple rest segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
      
        multipleRestContents->
          setMultipleRestContentsSegment (
            fVoiceLastSegment);

        // set multipleRestContents as the multiple rest contents
        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "Setting current last segment as multiple rest segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
      
        fVoicePendingMultipleRest->
          setMultipleRestContents (
            multipleRestContents);

        // append pending multiple rest to the list of repeats and segments
        fVoiceInitialRepeatsAndSegments.push_back (
          fVoicePendingMultipleRest);

/* JMI
        // print current voice contents
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          cerr << idtr <<
            "==================> The current voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl <<
            idtr;

        idtr++;
        print (cerr);
        idtr--;
*/

        // create a new segment to collect the remainder of the voice,
        // containing the next, yet incomplete, measure
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          cerr << idtr <<
            "Creating a new last segment with the next measure for voice \"" <<
            fVoiceName << "\"" <<
            endl;
            
        createNewLastSegmentWithFirstMeasureForVoice (
          inputLineNumber,
          nextMeasureAfterMultipleRest);

        // append the next measure after the multiple rest
        // to the new last segment
        fVoiceLastSegment->
          appendMeasureToSegment (
            nextMeasureAfterMultipleRest);

        // print resulting voice contents
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          cerr << idtr <<
            "The resulting voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

        idtr++;
        print (cerr);
        idtr--;

        // forget about this pending multiple rest
        fVoicePendingMultipleRest = 0;
      }
      break;
      
    case msrVoice::kMasterVoice:
      break;
  } // switch
}

void msrVoice::appendMultipleRestCloneToVoice (
  int               inputLineNumber,
  S_msrMultipleRest multipleRest)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
      {
        if (gGeneralOptions->fTraceRepeats) {
          cerr << idtr <<
            "Appending multiple rest '" <<
            multipleRest->multipleRestAsString () <<
            " to voice clone \"" <<
            getVoiceName () <<
            "\"" <<
            ", line " << inputLineNumber <<
            endl;
        }

/*
        // print current voice contents
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          cerr << idtr <<
            "==================> The current voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl <<
            idtr;

        idtr++;
        print (cerr);
        idtr--;
*/

        // append pending multiple rest to the list of repeats and segments
        fVoiceInitialRepeatsAndSegments.push_back (
          multipleRest);

        // print resulting voice contents
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          cerr << idtr <<
            "The resulting voice contents of voice \"" <<
            fVoiceName << "\" is:" <<
            endl;

        idtr++;
        print (cerr);
        idtr--;
      }
      break;
      
    case msrVoice::kMasterVoice:
      break;
  } // switch
}

void msrVoice::appendRepeatCloneToVoice (
  int         inputLineNumber,
  S_msrRepeat repeatCLone)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
      {
        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "Appending repeat clone to voice clone \"" <<
            getVoiceName () <<  "\"" <<
            endl;
      
        // set current last segment as the repeat common segment
        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "Segging current last segment as repeat common segment in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
      
        repeatCLone->
          setRepeatCommonSegment (
            fVoiceLastSegment);
          
        // register repeat clone as the (new) current repeat
        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "Registering repeat as the new current repeat in voice \"" <<
            getVoiceName () <<
            "\"" <<
            endl;
      
        msrAssert(
          repeatCLone != 0,
          "repeatCLone is null");
      
        fVoiceCurrentRepeat =
          repeatCLone;
      
        // append it to the list of repeats and segments
        fVoiceInitialRepeatsAndSegments.push_back (
          repeatCLone);
      
        // create a new segment for the voice
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          cerr << idtr <<
            "Creating a new last segment for voice BOF JMI \"" <<
            fVoiceName << "\"" <<
            endl;
            
        createNewLastSegmentForVoice (
          inputLineNumber);
        }
      break;
      
    case msrVoice::kMasterVoice:
      break;
  } // switch
}
    
void msrVoice::appendRepeatEndingToVoice (
  int       inputLineNumber,
  string    repeatEndingNumber, // may be "1, 2"
  msrRepeatEnding::msrRepeatEndingKind
            repeatEndingKind)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
      {
        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "Appending a repeat ending to voice \"" <<
            getVoiceName () <<  "\"" <<
            ", line " << inputLineNumber <<
            endl;
      
        S_msrRepeatEnding
          repeatEnding =
            msrRepeatEnding::create (
              inputLineNumber,
              repeatEndingNumber,
              repeatEndingKind,
              fVoiceLastSegment,
              fVoiceCurrentRepeat);
      
        // add the repeat ending it to the voice current repeat
        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "Appending repeat ending to current repeat in voice \"" <<
            fVoiceName <<
            "\"" <<
            endl;
            
        fVoiceCurrentRepeat->
          addRepeatEnding (repeatEnding);
      
        // create a new segment for the voice
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          cerr << idtr <<
            "Creating a new last segment for voice \"" <<
            fVoiceName <<
            "\"" <<
            endl;
            
        createNewLastSegmentForVoice (
          inputLineNumber);
      }
      break;
      
    case msrVoice::kMasterVoice:
      break;
  } // switch
}

void msrVoice:: appendRepeatEndingCloneToVoice ( // JMI
  S_msrRepeatEnding repeatEndingClone)
{
  switch (fVoiceKind) {
    case msrVoice::kRegularVoice:
    case msrVoice::kHarmonyVoice:
      {
        // add the repeat ending it to the voice current repeat
        if (gGeneralOptions->fTraceRepeats)
          cerr << idtr <<
            "Appending repeat ending clone to current repeat in voice \"" <<
            fVoiceName <<
            "\"" <<
            endl;
            
        msrAssert(
          fVoiceCurrentRepeat != 0,
          "fVoiceCurrentRepeat is null");
          
        fVoiceCurrentRepeat->
          addRepeatEnding (repeatEndingClone);
      
        // create a new segment for the voice
        if (gGeneralOptions->fTraceSegments || gGeneralOptions->fTraceVoices)
          cerr << idtr <<
            "Creating a new last segment for voice \"" <<
            fVoiceName <<
            "\"" <<
            endl;
            
        createNewLastSegmentForVoice (
          repeatEndingClone->getInputLineNumber ());
      }
      break;
            
    case msrVoice::kMasterVoice:
      break;
  } // switch
}

void msrVoice::prependBarlineToVoice (S_msrBarline barline)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Prepending barline '" <<
      barline->barlineAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      ":" << endl;
      
  fVoiceLastSegment->
    prependBarlineToSegment (barline);
}

void msrVoice::appendBarlineToVoice (S_msrBarline barline)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Appending barline '" <<
      barline->barlineAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      ":" << endl;
      
  fVoiceLastSegment->
    appendBarlineToSegment (barline);
}

void msrVoice::appendSegnoToVoice (S_msrSegno segno)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Appending a segno to voice \"" << getVoiceName () << "\"" <<
      endl;

  fVoiceLastSegment->
    appendSegnoToSegment (segno);
}

void msrVoice::appendCodaToVoice (S_msrCoda coda)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Appending a coda to voice \"" << getVoiceName () << "\"" <<
      ":" << endl;

  fVoiceLastSegment->
    appendCodaToSegment (coda);
}

void msrVoice::appendEyeGlassesToVoice (S_msrEyeGlasses eyeGlasses)
{
  if (gGeneralOptions->fTraceNotes)
    cerr << idtr <<
      "Appending a eyeGlasses to voice \"" << getVoiceName () << "\"" <<
      endl;

  fVoiceLastSegment->
    appendEyeGlassesToSegment (eyeGlasses);
}

void msrVoice::appendPedalToVoice (S_msrPedal pedal)
{
  if (gGeneralOptions->fTraceNotes)
    cerr << idtr <<
      "Appending a pedal to voice \"" << getVoiceName () << "\"" <<
      endl;

  fVoiceLastSegment->
    appendPedalToSegment (pedal);
}

/*
S_msrElement msrVoice::removeLastElementFromVoice (  // JMI
  int inputLineNumber)
{
  if (gGeneralOptions->fTraceGeneral)
    cerr << idtr <<
      "Removing last note" <<
      " from voice " << getVoiceName () <<
      endl;

  return
    fVoiceLastSegment->
      removeLastElementFromSegment (inputLineNumber);
}
*/

void msrVoice::removeNoteFromVoice (
  int       inputLineNumber,
  S_msrNote note)
{
  if (gGeneralOptions->fTraceChords) {
    cerr << idtr <<
      "Removing first chord note '" <<
      note->noteAsShortString () <<
      "' from voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  fVoiceLastSegment->
    removeNoteFromSegment (
      inputLineNumber,
      note);
}

S_msrMeasure msrVoice::removeLastMeasureFromVoice (
  int inputLineNumber)
{
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceVoices)
    cerr << idtr <<
      "Removing last measure from voice \"" <<
      getVoiceName () <<
      "\"" <<
      endl;

  // remove (new) last measure and return it
  return
    fVoiceLastSegment->
      removeLastMeasureFromSegment (inputLineNumber);
}

/*
void msrVoice::finalizeUltimateMeasuresInVoice (int inputLineNumber)
{
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceVoices) {
    cerr << idtr <<
      "Finalizing last measure in voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
  
  fVoiceLastSegment->
    finalizeUltimateMeasureInSegment (inputLineNumber);
}
*/

void msrVoice::finalizeCurrentMeasureInVoice (int inputLineNumber)
{
  if (gGeneralOptions->fTraceVoices) {
    cerr << idtr <<
      "Finalizing voice \"" <<
      getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }

  fVoiceLastSegment->
    finalizeCurrentMeasureInSegment (
      inputLineNumber);

  if (! gMsrOptions->fKeepMasterStanzas) { // JMI
// JMI    delete (fVoiceMasterStanza);
 // JMI   fVoiceMasterStanza = 0;
  }
}

void msrVoice::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrVoice::acceptIn()" <<
      endl;
      
  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrVoice::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrVoice::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrVoice::acceptOut()" <<
      endl;

  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrVoice::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrVoice::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrVoice::browseData()" <<
      endl;

  // browse the voice initial repeats and segments
  if (fVoiceInitialRepeatsAndSegments.size ()) {
    for (
      list<S_msrElement>::const_iterator i = fVoiceInitialRepeatsAndSegments.begin();
      i != fVoiceInitialRepeatsAndSegments.end();
      i++) {
      // browse the element
      msrBrowser<msrElement> browser (v);
      browser.browse (*(*i));
    } // for
  }

  // browse the segment
  msrBrowser<msrSegment> browser (v);
  browser.browse (*fVoiceLastSegment);

  // browse the voice stanzas
  if (fVoiceStanzasMap.size ()) {
    for (
      map<int, S_msrStanza>::const_iterator i = fVoiceStanzasMap.begin();
      i != fVoiceStanzasMap.end();
      i++) {
      S_msrStanza stanza = (*i).second;

      if (stanza->getStanzaTextPresent ()) {
        // browse the stanza
        msrBrowser<msrStanza> browser (v);
        browser.browse (*(stanza));
      }
    } // for
  }

  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% <== msrVoice::browseData()" <<
      endl;
}

string msrVoice::voiceKindAsString (
  msrVoiceKind voiceKind)
{
  string result;
  
  switch (voiceKind) {
    case msrVoice::kRegularVoice:
      result = "regular";
      break;
    case msrVoice::kHarmonyVoice:
      result = "harmony";
      break;
    case msrVoice::kMasterVoice:
      result = "marster";
      break;
  } // switch

  return result;
}

string msrVoice::voiceKindAsString () const
{
  return voiceKindAsString (fVoiceKind);
}
      
ostream& operator<< (ostream& os, const S_msrVoice& elt)
{
  elt->print (os);
  return os;
}

void msrVoice::print (ostream& os)
{
  os <<
    "Voice \"" << getVoiceName () << "\", " <<
    voiceKindAsString (fVoiceKind) <<
    " (" <<
    singularOrPlural (
      fVoiceActualHarmoniesCounter, "harmony", "harmonies") <<
     ", " <<
    singularOrPlural (
      fVoiceActualNotesCounter, "actual note", "actual notes") <<
     ", " <<
    singularOrPlural (
      fVoiceStanzasMap.size(), "stanza", "stanzas") <<
    ")" <<
    endl;

  idtr++;

  const int fieldWidth = 28;

  os <<
    idtr <<
      setw(fieldWidth) << "MusicHasBeenInsertedInVoice" << " : " <<
      booleanAsString (fMusicHasBeenInsertedInVoice) <<
      endl;

  os << endl;
  
  // print the voice initial repeats and segments
  int initialRepeatsAndSegmentsNumber =
    fVoiceInitialRepeatsAndSegments.size ();
    
  os << idtr <<
    "Initial repeats and segments: ";
  if (initialRepeatsAndSegmentsNumber)
    os << "(" << initialRepeatsAndSegmentsNumber << ")";
  else
    os << "none";
  os <<
    endl;
    
  if (initialRepeatsAndSegmentsNumber) {
    idtr++;

    list<S_msrElement>::const_iterator
      iBegin = fVoiceInitialRepeatsAndSegments.begin(),
      iEnd   = fVoiceInitialRepeatsAndSegments.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      // print the element
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    idtr--;
  }
  os << endl;
  
  // print the last segment
  msrAssert (
    fVoiceLastSegment != 0,
    "fVoiceLastSegment is null");
    
  os << idtr <<
    "Last segment:" <<
    endl;
    
  idtr++;
  os <<
    fVoiceLastSegment <<
    endl;
  idtr--;
  
  if (gGeneralOptions->fTraceLyrics) {
    // print the master stanza
    os << idtr <<
      fVoiceMasterStanza <<
      endl;    
  }
  
  if (gMsrOptions->fShowMsrStanzas) {
    // print the voice master stanza
    os << idtr <<
      fVoiceMasterStanza;
    
    // print the stanzas
    if (fVoiceStanzasMap.size()) {
      map<int, S_msrStanza>::const_iterator
        iBegin = fVoiceStanzasMap.begin(),
        iEnd   = fVoiceStanzasMap.end(),
        i      = iBegin;
        
      for ( ; ; ) {
        os << idtr << (*i).second;
        if (++i == iEnd) break;
        os << endl;
      } // for
    }
  }
  
  idtr--;
}

//______________________________________________________________________________
S_msrStaffLinesNumber msrStaffLinesNumber::create (
  int inputLineNumber,
  int linesNumber)
{
  msrStaffLinesNumber* o =
    new msrStaffLinesNumber (
      inputLineNumber, linesNumber);
  assert(o!=0);
  return o;
}

msrStaffLinesNumber::msrStaffLinesNumber (
  int inputLineNumber,
  int linesNumber)
    : msrElement (inputLineNumber)
{
  fLinesNumber = linesNumber;
}

msrStaffLinesNumber::~msrStaffLinesNumber() {}

S_msrStaffLinesNumber msrStaffLinesNumber::createStaffLinesNumberShallowClone ()
{
 if (gGeneralOptions->fTraceStaffTuning) {
    cerr << idtr <<
      "Creating a shallow clone of staff lines number '" <<
      staffLinesNumberAsString () <<
      "'" <<
      endl;
  }

 S_msrStaffLinesNumber
    clone =
      msrStaffLinesNumber::create (
        fInputLineNumber,
        fLinesNumber);
  
  return clone;
}

void msrStaffLinesNumber::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrStaffLinesNumber::acceptIn()" <<
      endl;
      
  if (visitor<S_msrStaffLinesNumber>*
    p =
      dynamic_cast<visitor<S_msrStaffLinesNumber>*> (v)) {
        S_msrStaffLinesNumber elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrStaffLinesNumber::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrStaffLinesNumber::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrStaffLinesNumber::acceptOut()" <<
      endl;

  if (visitor<S_msrStaffLinesNumber>*
    p =
      dynamic_cast<visitor<S_msrStaffLinesNumber>*> (v)) {
        S_msrStaffLinesNumber elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrStaffLinesNumber::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrStaffLinesNumber::browseData (basevisitor* v)
{}

string msrStaffLinesNumber::staffLinesNumberAsString () const
{
  stringstream s;

  s <<
    "StaffLinesNumber" <<
    ", line " << fInputLineNumber <<
    ", " <<
    "linesNumber: " << fLinesNumber;
    
  return s.str();
}

ostream& operator<< (ostream& os, const S_msrStaffLinesNumber& elt)
{
  elt->print (os);
  return os;
}

void msrStaffLinesNumber::print (ostream& os)
{
  os <<
    staffLinesNumberAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrStaffTuning msrStaffTuning::create (
  int                  inputLineNumber,
  int                  staffTuningLineNumber,
  msrQuartertonesPitch quartertonesPitch,
  int                  staffTuningOctave)
{
  msrStaffTuning* o =
    new msrStaffTuning (
      inputLineNumber,
      staffTuningLineNumber,
      quartertonesPitch,
      staffTuningOctave);
  assert(o!=0);
  return o;
}

msrStaffTuning::msrStaffTuning (
  int                  inputLineNumber,
  int                  staffTuningLineNumber,
  msrQuartertonesPitch quartertonesPitch,
  int                  staffTuningOctave)
    : msrElement (inputLineNumber)
{
  fStaffTuningLineNumber        = staffTuningLineNumber;
  fStaffTuningQuartertonesPitch = quartertonesPitch;
  fStaffTuningOctave            = staffTuningOctave;
}

msrStaffTuning::~ msrStaffTuning ()
{}

S_msrStaffTuning msrStaffTuning::createStaffTuningShallowClone ()
{
  if (gGeneralOptions->fTraceStaffTuning) {
    cerr << idtr <<
      "Creating a shallow clone of staff tuning '" <<
      staffTuningAsString () <<
      "'" <<
      endl;
  }

 S_msrStaffTuning
    clone =
      msrStaffTuning::create (
        fInputLineNumber,
        fStaffTuningLineNumber,
        fStaffTuningQuartertonesPitch,
        fStaffTuningOctave);
  
  return clone;
}

void msrStaffTuning::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    cerr << idtr <<
      "% ==> msrStaffTuning::acceptIn()" <<
      endl;
  }
      
  if (visitor<S_msrStaffTuning>*
    p =
      dynamic_cast<visitor<S_msrStaffTuning>*> (v)) {
        S_msrStaffTuning elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrStaffTuning::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrStaffTuning::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrStaffTuning::acceptOut()" <<
      endl;

  if (visitor<S_msrStaffTuning>*
    p =
      dynamic_cast<visitor<S_msrStaffTuning>*> (v)) {
        S_msrStaffTuning elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrStaffTuning::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrStaffTuning::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrStaffTuning& elt)
{
  elt->print (os);
  return os;
}

string msrStaffTuning::staffTuningAsString () const
{
  stringstream s;

  s <<
    "StaffTuning" <<
    ", line " << fStaffTuningLineNumber <<
    ", " <<
    msrQuartertonesPitchAsString (
      gMsrOptions->fMsrQuatertonesPitchesLanguage,
      fStaffTuningQuartertonesPitch) <<
    ", octave " << fStaffTuningOctave;
    
  return s.str();
}

void msrStaffTuning::print (ostream& os)
{
  os <<
    "StaffTuning" <<
    ", line " << fStaffTuningLineNumber <<
    ", " <<
    endl;

  idtr++;

  const int fieldWidth = 29;

  os << left <<  
    idtr <<
      setw(fieldWidth) <<
      "StaffTuningLineNumber" << " = " <<
      fStaffTuningLineNumber <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fStaffTuningQuartertonesPitch" << " = " <<
      msrQuartertonesPitchAsString (
        gMsrOptions->fMsrQuatertonesPitchesLanguage,
        fStaffTuningQuartertonesPitch) <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "StaffTuningOctave" << " = " <<
      fStaffTuningOctave <<
      endl;

  idtr--;
}

//______________________________________________________________________________
S_msrStaffDetails msrStaffDetails::create (
  int                   inputLineNumber,
  msrStaffTypeKind      staffTypeKind,
  S_msrStaffLinesNumber staffLinesNumber,
  S_msrStaffTuning      staffTuning,
  msrShowFretsKind      showFretsKind,
  msrPrintObjectKind    printObjectKind,
  msrPrintSpacingKind   printSpacingKind)
{
  msrStaffDetails* o =
    new msrStaffDetails (
      inputLineNumber,
      staffTypeKind,
      staffLinesNumber,
      staffTuning,
      showFretsKind,
      printObjectKind,
      printSpacingKind);
  assert(o!=0);
  return o;
}

msrStaffDetails::msrStaffDetails (
  int                   inputLineNumber,
  msrStaffTypeKind      staffTypeKind,
  S_msrStaffLinesNumber staffLinesNumber,
  S_msrStaffTuning      staffTuning,
  msrShowFretsKind      showFretsKind,
  msrPrintObjectKind    printObjectKind,
  msrPrintSpacingKind   printSpacingKind)
    : msrElement (inputLineNumber)
{
  fStaffTypeKind    = staffTypeKind;
  fStaffLinesNumber = staffLinesNumber;
  fStaffTuning      = staffTuning;
  fShowFretsKind    = showFretsKind;
  fPrintObjectKind  = printObjectKind;
  fPrintSpacingKind = printSpacingKind;
}

msrStaffDetails::~msrStaffDetails() {}

S_msrStaffDetails msrStaffDetails::createStaffDetailsShallowClone (
  S_msrStaffLinesNumber staffLinesNumberClone,
  S_msrStaffTuning      staffTuningClone)
{
  if (gGeneralOptions->fTraceStaves) {
    cerr << idtr <<
      "Creating a shallow clone of staff details \"" <<
      staffDetailsAsShortString () <<
      "\"" <<
      endl;
  }
      
  S_msrStaffDetails
    clone =
      msrStaffDetails::create (
        fInputLineNumber,
        fStaffTypeKind,
        staffLinesNumberClone,
        staffTuningClone,
        fShowFretsKind,
        fPrintObjectKind,
        fPrintSpacingKind);

  return clone;
}

void msrStaffDetails::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrStaffDetails::acceptIn()" <<
      endl;
      
  if (visitor<S_msrStaffDetails>*
    p =
      dynamic_cast<visitor<S_msrStaffDetails>*> (v)) {
        S_msrStaffDetails elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrStaffDetails::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrStaffDetails::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrStaffDetails::acceptOut()" <<
      endl;

  if (visitor<S_msrStaffDetails>*
    p =
      dynamic_cast<visitor<S_msrStaffDetails>*> (v)) {
        S_msrStaffDetails elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrStaffDetails::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrStaffDetails::browseData (basevisitor* v)
{
  if (fStaffLinesNumber) {
    // browse the staff lines number
    msrBrowser<msrStaffLinesNumber> browser (v);
    browser.browse (*fStaffLinesNumber);
  }

  if (fStaffTuning) {
    // browse the staff tuning
    msrBrowser<msrStaffTuning> browser (v);
    browser.browse (*fStaffTuning);
  }
}

string msrStaffDetails::staffTypeKindAsString (
  msrStaffTypeKind staffTypeKind)
{
  string result;
  
  switch (staffTypeKind) {
    case msrStaffDetails::kRegularStaffType:
      result = "regular";
      break;
    case msrStaffDetails::kOssiaStaffType:
      result = "ossia";
      break;
    case msrStaffDetails::kCueStaffType:
      result = "cue";
      break;
    case msrStaffDetails::kEditorialStaffType:
      result = "editorial";
      break;
    case msrStaffDetails::kAlternateStaffType:
      result = "alternate";
      break;
  } // switch

  return result;
}

string msrStaffDetails::showFretsKindAsString (
  msrShowFretsKind showFretsKind)
{
  string result;
  
  switch (showFretsKind) {
    case msrStaffDetails::kShowFretsNumbers:
      result = "numbers";
      break;
    case msrStaffDetails::kShowFretsLetters:
      result = "letters";
      break;
  } // switch

  return result;
}
      
string msrStaffDetails::printObjectKindKindAsString (
  msrPrintObjectKind printObjectKind)
{
  string result;
  
  switch (printObjectKind) {
    case msrStaffDetails::kPrintObjectYes:
      result = "print object yes";
      break;
    case msrStaffDetails::kPrintObjectNo:
      result = "print object no";
      break;
  } // switch

  return result;
}
      
string msrStaffDetails::printSpacingKindKindAsString (
  msrPrintSpacingKind printSpacingKind)
{
  string result;
  
  switch (printSpacingKind) {
    case msrStaffDetails::kPrintSpacingYes:
      result = "print spacing yes";
      break;
    case msrStaffDetails::kPrintSpacingNo:
      result = "print spacing no";
      break;
  } // switch

  return result;
}
      
ostream& operator<< (ostream& os, const S_msrStaffDetails& elt)
{
  elt->print (os);
  return os;
}

string msrStaffDetails::staffDetailsAsShortString ()
{
  stringstream s;

  s <<
    "StaffDetails" <<
    ", line " << fInputLineNumber;

  // print the staff lines number if any
  s << ", StaffLinesNumber: ";
  if (fStaffLinesNumber)
    s <<
      fStaffLinesNumber->getLinesNumber ();
  else
    s <<
      "none";

  // print the staff tuning if any
  s << ", StaffTuning: ";
  if (fStaffTuning)
    s <<
      "present";
  else
    s <<
      "none";

  return s.str ();
}

void msrStaffDetails::print (ostream& os)
{
  os <<
    "StaffDetails:" <<
    endl;

  idtr++;

  // print the staff lines number if any
  if (fStaffLinesNumber)
    os << idtr <<
      fStaffLinesNumber;
  else
    os <<
      "StaffLinesNumber: none" <<
      endl;

  // print the staff tuning if any
  if (fStaffTuning)
    os << idtr <<
      fStaffTuning;
  else
    os << idtr <<
      "StaffTuning: none" <<
      endl;

  idtr--;
}

//______________________________________________________________________________
int msrStaff::gMaxStaffVoices = 4;

S_msrStaff msrStaff::create (
  int          inputLineNumber,
  S_msrPart    staffDirectPartUplink,
  msrStaffKind staffKind,
  int          staffNumber)
{
  msrStaff* o =
    new msrStaff (
      inputLineNumber,
      staffDirectPartUplink,
      staffKind,
      staffNumber);
  assert(o!=0);

  return o;
}

msrStaff::msrStaff (
  int          inputLineNumber,
  S_msrPart    staffDirectPartUplink,
  msrStaffKind staffKind,
  int          staffNumber)
    : msrElement (inputLineNumber)
{
  // set staff part uplink
  msrAssert(
    staffDirectPartUplink != 0,
    "staffDirectPartUplink is null");

  fStaffDirectPartUplink =
    staffDirectPartUplink;
    
  fStaffKind = staffKind;

  fStaffNumber = staffNumber;

  initializeStaff ();
}

void msrStaff::initializeStaff ()
{
  fRegisteredVoicesCounter = 0;

  // set staff name
  switch (fStaffKind) {
    case msrStaff::kRegularStaff:
      fStaffName =
        fStaffDirectPartUplink->getPartMsrName () +
        "_Staff_" +
        int2EnglishWord (fStaffNumber);
      break;
      
    case msrStaff::kTablatureStaff:
        fStaffDirectPartUplink->getPartMsrName () +
        "_Tablature_" +
        int2EnglishWord (fStaffNumber);
      break;
      
    case msrStaff::kPercussionStaff:
        fStaffDirectPartUplink->getPartMsrName () +
        "_Percussion_" +
        int2EnglishWord (fStaffNumber);
      break;
      
    case msrStaff::kHarmonyStaff:
      fStaffName =
        fStaffDirectPartUplink->getPartMsrName () +
        "_HarmonyStaff";
      break;
  } // switch

  if (gGeneralOptions->fTraceStaves)
    cerr << idtr <<
      "Creating staff \"" << fStaffName <<
      "\" in part " <<
      fStaffDirectPartUplink->getPartCombinedName () <<
      endl;

  // check the staff number
  switch (fStaffKind) {
    case msrStaff::kRegularStaff:
      // the staff number should not be negative
      // (-99 is used for hidden staff containing the staves master voices)
      if (fStaffNumber < 0) {
        stringstream s;
    
        s <<
          "regular staff number " << fStaffNumber <<
          " is not positive";
          
        msrAssert (false, s.str());
      }
      break;
      
    case msrStaff::kTablatureStaff:
      // JMI
      break;
      
    case msrStaff::kPercussionStaff:
      // JMI
      break;
      
    case msrStaff::kHarmonyStaff:
      if (fStaffNumber != -1) {
        stringstream s;
    
        s <<
          "harmony staff number " << fStaffNumber <<
          " is not equal to -1";
          
        msrInternalError (
          fInputLineNumber, s.str());
      }
      break;
  } // switch


  // create the staff master voice with relative number -99
  createStaffMasterVoice (
    fInputLineNumber);

/* JMI BOF
  // create all 'gMaxStaffVoices' voices for this staff
  // those that remain without music will be removed later
  // in removeStaffEmptyVoices()
  for (int i = 1; i <= gMaxStaffVoices; i++) {
    addVoiceToStaffByItsRelativeNumber (
      fInputLineNumber,
      msrVoice::kRegularVoice,
      i);
  } // for
*/

  // get the initial staff details from the part if any
  S_msrStaffDetails
    partStaffDetails =
      fStaffDirectPartUplink->
        getCurrentPartStaffDetails ();

  if (partStaffDetails) {
    // append it to the staff
    appendStaffDetailsToStaff (partStaffDetails);
  }
    
  // get the initial clef from the part if any
  {
    S_msrClef
      clef =
        fStaffDirectPartUplink->
          getPartCurrentClef ();
  
    if (clef) {
      if (gGeneralOptions->fTraceStaves)
        cerr << idtr <<
          "Setting staff clef '" << clef->clefAsString () <<
          "' in staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffDirectPartUplink->getPartCombinedName () <<
          endl;

      setStaffCurrentClef (clef);
    }
    
    else {
      if (gGeneralOptions->fTraceStaves)
        cerr << idtr <<
          "Setting default treble clef " <<
          " in staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffDirectPartUplink->getPartCombinedName () <<
          endl;

      // create the implicit initial G line 2 clef
      setStaffCurrentClef (
        msrClef::create (
          fInputLineNumber,
          msrClef::kTrebleClef));
    }
  }
    
  // get the initial key from the part if any
  {
    S_msrKey
      key =
        fStaffDirectPartUplink->
          getPartCurrentKey ();
  
    if (key) {
      if (gGeneralOptions->fTraceStaves)
        cerr << idtr <<
          "Setting staff key '" << key->keyAsString () <<
          "' in staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffDirectPartUplink->getPartCombinedName () <<
          endl;

      setStaffCurrentKey (key);
      }
    else {
      if (gGeneralOptions->fTraceStaves)
        cerr << idtr <<
          "Setting default C major key " <<
          " in staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffDirectPartUplink->getPartCombinedName () <<
          endl;
          
      // create the implicit initial C major key
      setStaffCurrentKey (
        msrKey::createTraditional (
          fInputLineNumber,
          k_cNatural, msrKey::kMajorMode, 0));
    }
  }
  
  // get the initial time from the part if any
  {
    S_msrTime
      time =
        fStaffDirectPartUplink->
          getPartCurrentTime ();

    if (time) {
      if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceTimes)
        cerr << idtr <<
          "Setting staff time '" << time->timeAsString () <<
          "' in staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffDirectPartUplink->getPartCombinedName () <<
          endl;

      setStaffCurrentTime (time);
    }
    else {
      if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceTimes)
        cerr << idtr <<
          "Setting default 4/4 time " <<
          " in staff \"" <<
          getStaffName () <<
          "\" in part " <<
          fStaffDirectPartUplink->getPartCombinedName () <<
          endl;
          
      // create the implicit initial 4/4 time signature
      setStaffCurrentTime (
        msrTime::create (
          fInputLineNumber,
          4, 4));
    }
  }
  
  // get the initial transpose from the part if any
  {
    S_msrTranspose
      transpose =
        fStaffDirectPartUplink->getPartTranspose ();
        
    if (transpose) {
      fStaffTranspose = transpose;
      appendTransposeToAllStaffVoices (transpose);
    }
  }
}

msrStaff::~msrStaff()
{}

void msrStaff::createStaffMasterVoice (
  int inputLineNumber)
{
  // create the part harmony voice
  const int staffMasterVoiceNumber = -99; // JMI static?
  
  if (gGeneralOptions->fTraceHarmonies)
    cerr << idtr <<
      "Creating the master voice for staff \"" <<
      getStaffName () <<
      "\", line " << inputLineNumber <<
      endl;

  fStaffMasterVoice =
    msrVoice::create (
      inputLineNumber,
      fStaffDirectPartUplink,
      msrVoice::kMasterVoice,
      staffMasterVoiceNumber,
      this);

/*
  registerVoiceInStaff (
    inputLineNumber,
    fStaffMasterVoice );
    */
  // register is by its relative number
  fStaffAllVoicesMap [fRegisteredVoicesCounter] =
    fStaffMasterVoice;

/* JMI
  // register it by its external number
  fStaffVoiceRelativeNumberToVoiceMap [voice->getExternalVoiceNumber ()] =
    fStaffMasterVoice;
    */ 
}

S_msrStaff msrStaff::createStaffShallowClone (S_msrPart partClone)
{
  if (gGeneralOptions->fTraceStaves) {
    cerr << idtr <<
      "Creating a shallow clone of staff \"" <<
      fStaffName <<
      "\"" <<
      endl;
  }
  
  msrAssert(
    partClone != 0,
    "partClone is null");
    
  S_msrStaff
    clone =
      msrStaff::create (
        fInputLineNumber,
        partClone,
        fStaffKind,
        fStaffNumber);

  clone->fStaffName =
    fStaffName;
    
  return clone;
}

string msrStaff::getStaffName () const
  {
    return fStaffName;

    /*
  return
    fStaffNumber == 0
      ?
        fStaffDirectPartUplink->getPartMsrName () +
        "_S_" +
        "(MASTER)"
      :
        fStaffDirectPartUplink->getPartMsrName () +
        "_S_" +
        int2EnglishWord (fStaffNumber);
        */
  }

const int msrStaff::getStaffNumberOfMusicVoices () const
{
  int result = 0;

  for (
    map<int, S_msrVoice>::const_iterator i =
      fStaffVoiceRelativeNumberToVoiceMap.begin();
    i != fStaffVoiceRelativeNumberToVoiceMap.end();
    i++) {
      S_msrVoice
        voice =
          (*i).second;

      switch (voice->getVoiceKind ()) {
        case msrVoice::kRegularVoice:
          if (voice->getMusicHasBeenInsertedInVoice ())
            result++;
          break;
          
        case msrVoice::kHarmonyVoice: // JMI
          break;
          
        case msrVoice::kMasterVoice:
          break;
      } // switch
      
  } // for

  return result;
}

void msrStaff::createAndAppendMeasureToStaff (
  int    inputLineNumber,
  string measureNumber)
{
  if (gGeneralOptions->fTraceStaves)
    cerr << idtr <<
      "Append a new measure, " <<
      ", measureNumber = " << measureNumber <<
      ", line " << inputLineNumber <<
      ", in staff \"" << getStaffName () << "\"" <<
      endl;

  // set staff measure location
  fStaffMeasureNumber = measureNumber;

  // propagate it to all staves
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      createAndAppendMeasureToVoice (
        inputLineNumber, measureNumber);
  } // for
}

/* JMI
S_msrVoice msrStaff::addVoiceMasterToStaff (
  int inputLineNumber)
{
  // create the voice
  S_msrVoice
    voice =
      msrVoice::createMasterVoice (
        inputLineNumber,
        this);

  // add it to this staff as element '0'
  fStaffAllVoicesMap [0] = voice;
  
  // return the voice
  return voice;
}
*/

/* JMI BOF
S_msrVoice msrStaff::createVoiceInStaffByItsExternalNumber (
  int inputLineNumber,
  int externalVoiceNumber)
{
  // create the voice
  S_msrVoice
    voice =
      msrVoice::create (
        inputLineNumber,
        fStaffDirectPartUplink,
        msrVoice::kRegularVoice,
        externalVoiceNumber,
        this);

  // register the voice by its relative number
  fStaffAllVoicesMap [externalVoiceNumber] = voice;
  
  // return the voice
  return voice;
}
*/

S_msrVoice msrStaff::createVoiceInStaffByItsExternalNumber (
  int inputLineNumber,
  int externalVoiceNumber)
{
  // take this new voice into account
  fRegisteredVoicesCounter++;

  if (gGeneralOptions->fTraceVoices)
    cerr << idtr <<
      "Registering voice " << externalVoiceNumber <<
       " as relative voice " << fRegisteredVoicesCounter <<
     " of staff \"" << getStaffName () <<
      "\", line " << inputLineNumber <<
 // JMI     " in part " << fStaffDirectPartUplink->getPartCombinedName () <<
      endl;

  // are there too many voices in this staff? 
  if (fRegisteredVoicesCounter > msrStaff::gMaxStaffVoices) {
    stringstream s;
    
    s <<
      "staff \"" << getStaffName () <<
      "\" is already filled up with " <<
      msrStaff::gMaxStaffVoices << " voices" <<
      endl <<
      "the voice with external number " << externalVoiceNumber << " overflows it" <<
      endl <<
      ", fRegisteredVoicesCounter = " << fRegisteredVoicesCounter <<
      ", msrStaff::gMaxStaffVoices = " << msrStaff::gMaxStaffVoices <<
      endl;
      
    msrMusicXMLError (
// JMI    msrMusicXMLWarning ( JMI
      inputLineNumber,
      s.str());
  }

  // create the voice
  S_msrVoice
    voice =
      msrVoice::create (
        inputLineNumber,
        fStaffDirectPartUplink,
        msrVoice::kRegularVoice,
        externalVoiceNumber,
        this);
          
  // register the voice by its relative number
  if (gGeneralOptions->fTraceVoices)
    cerr << idtr <<
      "Voice " << externalVoiceNumber <<
      " in staff " << getStaffName () <<
      " gets staff relative number " << fRegisteredVoicesCounter <<
      endl;
    
  fStaffVoiceRelativeNumberToVoiceMap [fRegisteredVoicesCounter] =
    voice;

  // register is by its external number
  fStaffAllVoicesMap [fRegisteredVoicesCounter] =
    voice;

  return voice;
}

S_msrVoice msrStaff::fetchVoiceFromStaffByItsExternalNumber (
  int inputLineNumber,
  int externalVoiceNumber)
{
  S_msrVoice result; // JMI avoid repetivite messages!

  if (gGeneralOptions->fTraceVoices && gGeneralOptions->fTraceStaves)
    cerr << idtr <<
      "Fetching external voice number " << externalVoiceNumber <<
     " in staff \"" << getStaffName () <<
      "\", line " << inputLineNumber <<
      " in part " <<
      fStaffDirectPartUplink->getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i =
      fStaffVoiceRelativeNumberToVoiceMap.begin();
    i != fStaffVoiceRelativeNumberToVoiceMap.end();
    i++) {
    if (
      (*i).second->getExternalVoiceNumber ()
        ==
      externalVoiceNumber) {
      if (gGeneralOptions->fTraceVoices) {
        cerr << idtr <<
          "Voice " << externalVoiceNumber <<
          " in staff \"" << getStaffName () << "\"" <<
          " has staff relative number " << (*i).first <<
          endl;
      }
        
      result = (*i).second;
      break;
    }
  } // for

  return result;
}

void msrStaff::registerVoiceInStaff (
  int inputLineNumber, S_msrVoice voice)
{
  // take this new voice into account
  fRegisteredVoicesCounter++;

  // are there too many voices in this staff? 
  if (fRegisteredVoicesCounter > msrStaff::gMaxStaffVoices) {
    stringstream s;
    
    s <<
      "staff \"" << getStaffName () <<
      "\" is already filled up with " <<
      msrStaff::gMaxStaffVoices << " voices," <<
      endl <<
      "the voice named \"" << voice->getVoiceName () << "\" overflows it" <<
      endl <<
      ", fRegisteredVoicesCounter = " << fRegisteredVoicesCounter <<
      ", msrStaff::gMaxStaffVoices = " << msrStaff::gMaxStaffVoices <<
      endl;
      
    msrMusicXMLError (
// JMI    msrMusicXMLWarning ( JMI
      inputLineNumber,
      s.str());
  }

  // register voice in this staff
  if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceVoices)
    cerr << idtr <<
      "Registering voice \"" << voice->getVoiceName () <<
      "\" as relative voice " << fRegisteredVoicesCounter <<
      " of staff \"" << getStaffName () <<
      "\", line " << inputLineNumber <<
// JMI       " in part " << fStaffDirectPartUplink->getPartCombinedName () <<
      endl;

  // register is by its relative number
  fStaffAllVoicesMap [fRegisteredVoicesCounter] =
    voice;

  // register it by its external number
  fStaffVoiceRelativeNumberToVoiceMap [voice->getExternalVoiceNumber ()] =
    voice;
}

void msrStaff::setStaffCurrentClef (S_msrClef clef)
{
  if (gGeneralOptions->fTraceStaves)
    cerr << idtr <<
      "Setting staff clef '" << clef->clefAsString () <<
      "' in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffDirectPartUplink->getPartCombinedName () <<
      endl;

  // set staff clef
  fStaffCurrentClef = clef;

  // is this a tablature or percussion staff?
  switch (fStaffKind) {
    case msrStaff::kRegularStaff: // JMI
      if (clef->clefIsATablatureClef ())
        fStaffKind = kTablatureStaff;
      else if (clef->clefIsAPercussionClef ())
        fStaffKind = kPercussionStaff;
      break;
      
    case msrStaff::kTablatureStaff:
      break;
      
    case msrStaff::kPercussionStaff:
      break;
      
    case msrStaff::kHarmonyStaff:
      break;
  } // switch
  
  // propagate clef to all voices
  appendClefToAllStaffVoices (clef);
}

void msrStaff::setStaffCurrentKey  (S_msrKey  key)
{
  if (gGeneralOptions->fTraceKeys || gGeneralOptions->fTraceStaves)
    cerr << idtr <<
      "Setting key '" << key->keyAsString () <<
      "' in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffDirectPartUplink->getPartCombinedName () <<
      endl;

  // set staff key
  fStaffCurrentKey = key;

  // propagate it to all voices
  appendKeyToAllStaffVoices (key);
}

void msrStaff::setStaffCurrentTime (S_msrTime time)
{
  if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceTimes)
    cerr << idtr <<
      "Setting time to '" << time->timeAsString () <<
      "' in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffDirectPartUplink->getPartCombinedName () <<
      endl;

  // set staff time
  fStaffCurrentTime = time;

  // propagate it to all voices
  appendTimeToAllStaffVoices (time);
}    

void msrStaff::createAndAppendRepeatToStaff (int inputLineNumber)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Appending repeat to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffDirectPartUplink->getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      createAndAppendRepeatToVoice (inputLineNumber);
  } // for
}

void msrStaff::appendRepeatEndingToStaff (
  int       inputLineNumber,
  string    repeatEndingNumber, // may be "1, 2"
  msrRepeatEnding::msrRepeatEndingKind
            repeatEndingKind)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Appending repeat ending to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffDirectPartUplink->getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      appendRepeatEndingToVoice (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind);
  } // for
}

void msrStaff::createMeasureRepeatFromItsFirstMeasureInStaff (
  int inputLineNumber,
  int measureRepeatMeasuresNumber,
  int measureRepeatSlashes)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Creating a measure repeat from it's first measure in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffDirectPartUplink->getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      createMeasureRepeatFromItsFirstMeasureInVoice (
        inputLineNumber,
        measureRepeatMeasuresNumber,
        measureRepeatSlashes);
  } // for
}

void msrStaff::appendPendingMeasureRepeatToStaff (
  int inputLineNumber)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Appending the pending measure repeat to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffDirectPartUplink->getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      appendPendingMeasureRepeatToVoice (
        inputLineNumber);
  } // for
}

void msrStaff::createMultipleRestInStaff (
  int inputLineNumber,
  int multipleRestMeasuresNumber)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Creating a multiple rest in staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffDirectPartUplink->getPartCombinedName () <<
      ", " <<
      singularOrPlural (
        multipleRestMeasuresNumber, "measure", "measures") <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      createMultipleRestInVoice (
        inputLineNumber,
        multipleRestMeasuresNumber);
  } // for
}

void msrStaff::appendPendingMultipleRestToStaff (
  int inputLineNumber)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Appending the pending multiple rest to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffDirectPartUplink->getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      appendPendingMultipleRestToVoice (
        inputLineNumber);
  } // for
}

void msrStaff::appendMultipleRestCloneToStaff (
  int               inputLineNumber,
  S_msrMultipleRest multipleRest)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Appending multiple rest '" <<
      multipleRest->multipleRestAsString () <<
      "' to staff clone \"" <<
      getStaffName () <<
      "\"" <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      appendMultipleRestCloneToVoice (
        inputLineNumber,
        multipleRest);
  } // for
}

void msrStaff::appendRepeatCloneToStaff (
  int         inputLineNumber,
  S_msrRepeat repeatCLone)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Appending repeat clone to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffDirectPartUplink->getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->appendRepeatCloneToVoice (
      inputLineNumber, repeatCLone);
  } // for
}

void msrStaff::appendRepeatEndingCloneToStaff (
  S_msrRepeatEnding repeatEndingClone)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Appending repeat ending clone to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffDirectPartUplink->getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      appendRepeatEndingCloneToVoice (repeatEndingClone);
  } // for
}

void msrStaff::appendBarlineToStaff (S_msrBarline barline)
{
  if (gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Appending barline '" << barline->barlineAsString () <<
      "' to staff " <<
      getStaffName () <<
      "\" in part " <<
      fStaffDirectPartUplink->getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      appendBarlineToVoice (barline);
  } // for
}

void msrStaff::setStaffTranspose (S_msrTranspose transpose)
{
  if (gGeneralOptions->fTraceStaves)
    cerr << idtr <<
      "Setting transpose '" <<
      transpose->transposeAsString () <<
      "' in staff " <<
      getStaffName () <<
      "\" in part " <<
      fStaffDirectPartUplink->getPartCombinedName () <<
      endl;

  // set staff transpose
  fStaffTranspose = transpose;

  // propagate it to all voices
  appendTransposeToAllStaffVoices (transpose);
}

void msrStaff::appendStaffDetailsToStaff (
  S_msrStaffDetails staffDetails)
{
  if (gGeneralOptions->fTraceStaves)
    cerr << idtr <<
      "Appending staff details '" <<
      staffDetails->staffDetailsAsShortString () <<
      "' to staff \"" <<
      getStaffName () <<
      "\" in part " <<
      fStaffDirectPartUplink->getPartCombinedName () <<
      endl;

  // register staff details in staff
  fCurrentStaffStaffDetails = staffDetails;
  
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      appendStaffDetailsToVoice (staffDetails);
  } // for
}

void msrStaff::appendClefToAllStaffVoices (S_msrClef clef)
{
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      appendClefToVoice (clef);
  } // for
}

void msrStaff::appendKeyToAllStaffVoices (S_msrKey  key)
{
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      appendKeyToVoice (key);
  } // for
}

void msrStaff::appendTimeToAllStaffVoices (S_msrTime time)
{
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      appendTimeToVoice (time);
  } // for
}

/*
void msrStaff::appendHarmonyToStaff (S_msrHarmony harmony) // JMI
{
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->appendHarmonyToVoice (harmony);
  } // for
}
*/

void msrStaff::appendTransposeToAllStaffVoices (S_msrTranspose transpose)
{
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      appendTransposeToVoice (transpose);
  } // for
}

void msrStaff::finalizeCurrentMeasureInStaff (int inputLineNumber)
{  
  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
 // JMI   if (! (*i).second->getVoiceActualNotesCounter ()) {
    S_msrVoice voice = (*i).second;

    switch (voice->getVoiceKind ()) {
      case msrVoice::kRegularVoice:
      case msrVoice::kHarmonyVoice:
        if (! voice->getMusicHasBeenInsertedInVoice ()) {
          if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceVoices)
            cerr << idtr <<
              "Erasing regular master voice \"" <<
              voice->getVoiceName () <<
              "\" in staff " <<
              getStaffName () <<
              ", line " << inputLineNumber <<
              endl;
              
     // JMI BOF     fStaffAllVoicesMap.erase (i);
        }
        else {
          voice->
            finalizeCurrentMeasureInVoice (inputLineNumber);
        }
        break;
                
      case msrVoice::kMasterVoice:
        if (! gMsrOptions->fKeepMasterVoices) {
          if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceVoices)
            cerr << idtr <<
              "Erasing staff master voice \"" <<
              voice->getVoiceName () <<
              "\" in staff " <<
              getStaffName () <<
              ", line " << inputLineNumber <<
              endl;

    // JMI BOF      fStaffAllVoicesMap.erase (i);
        }
        break;
    } // switch
  } // for
}

/*
void msrStaff::finalizeUltimateMeasuresInStaff (int inputLineNumber)
{
  if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Finalizing last measure in staff " <<
      getStaffName () <<
      ", line " << inputLineNumber <<
      endl;

  for (
    map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
    i != fStaffAllVoicesMap.end();
    i++) {
    (*i).second->
      finalizeUltimateMeasuresInVoice (inputLineNumber);
  } // for
}
*/

void msrStaff::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrStaff::acceptIn()" <<
      endl;
      
  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrStaff::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrStaff::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrStaff::acceptOut()" <<
      endl;

  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrStaff::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrStaff::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrStaff::browseData()" <<
      endl;

  /*
    fPartCurrentClef, fPartCurrentKey and fPartCurrentTime are used
    to populate newly created voices, not to create music proper:
    they're thus not browsed
  */

  /*
    fCurrentPartStaffDetails is used
    to populate newly created voices, not to create music proper:
    it is thus not browsed
  */

/*
  if (fStaffTuningsList.size ()) {
    for (
      list<S_msrStaffTuning>::const_iterator i = fStaffTuningsList.begin();
      i != fStaffTuningsList.end();
      i++) {
      // browse the voice
      msrBrowser<msrStaffTuning> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }
*/

  if (fStaffAllVoicesMap.size ()) {
    for (
      map<int, S_msrVoice>::const_iterator i = fStaffAllVoicesMap.begin();
      i != fStaffAllVoicesMap.end();
      i++) {
 // JMI     if ((*i).second->getStaffRelativeVoiceNumber () != 0) {
        // browse the voice, but not if it is the master voice JMI
        msrBrowser<msrVoice> browser (v);
        browser.browse (*((*i).second));
  //    }
    } // for
  }

  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% <== msrStaff::browseData()" <<
      endl;
}

ostream& operator<< (ostream& os, const S_msrStaff& elt)
{
  elt->print (os);
  return os;
}


string msrStaff::staffKindAsString (
  msrStaffKind staffKind)
{
  string result;
  
  switch (staffKind) {
    case msrStaff::kRegularStaff:
      result = "regular";
      break;
    case msrStaff::kTablatureStaff:
      result = "tablature";
      break;
    case msrStaff::kPercussionStaff:
      result = "percussion";
      break;
    case msrStaff::kHarmonyStaff:
      result = "harmony";
      break;
  } // switch

  return result;
}

string msrStaff::staffKindAsString () const
{
  return staffKindAsString (fStaffKind);
}

void msrStaff::print (ostream& os)
{
  os <<
    "Staff" " " << getStaffName () <<
    ", " << staffKindAsString () <<
    ", (" <<
    singularOrPlural (
      fStaffVoiceRelativeNumberToVoiceMap.size(), "voice", "voices") <<
    ")" <<
    endl;

  idtr++;

/* JMI
  os <<
    idtr << "StaffInstrumentName: \"" <<
    fStaffInstrumentName << "\"" << endl;
*/

/*
  // print the staff staff datails if any
  os << idtr <<
    "Staff details: ";
  if (fCurrentStaffStaffDetails) {
    os << fCurrentStaffStaffDetails;
  }
  else
    os << "none";
  os << endl;
*/

  os << endl;

  /* JMI
  if (fStaffTuningsList.size ()) {
    os <<
      idtr << "Staff tunings:" <<
      endl;
      
    list<S_msrStaffTuning>::const_iterator
      iBegin = fStaffTuningsList.begin(),
      iEnd   = fStaffTuningsList.end(),
      i      = iBegin;
      
    idtr++;
    for ( ; ; ) {
      os << idtr <<
        (*i)->staffTuningAsString ();
      if (++i == iEnd) break;
      os << endl;
    } // for
    os << endl;
    idtr--;
  }

*/

  // print the voices
  if (fStaffVoiceRelativeNumberToVoiceMap.size ()) {
    map<int, S_msrVoice>::const_iterator
      iBegin = fStaffAllVoicesMap.begin(),
      iEnd   = fStaffAllVoicesMap.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      S_msrVoice voice = (*i).second;

      switch (voice->getVoiceKind ()) {
        case msrVoice::kRegularVoice:
          os << idtr << voice;
          break;

        case msrVoice::kHarmonyVoice:
          if (
            gMsrOptions->fShowHarmonyVoices
              ||
            voice->getMusicHasBeenInsertedInVoice ())
            os << idtr << voice;
          break;
          
        case msrVoice::kMasterVoice:
          if (gMsrOptions->fShowMasterVoices)
            os << idtr << voice;
          break;
      } // switch

      if (++i == iEnd) break;

      switch (voice->getVoiceKind ()) {
        case msrVoice::kRegularVoice:
          os << endl;
          break;

        case msrVoice::kHarmonyVoice:
          if (
            gMsrOptions->fShowHarmonyVoices
              ||
            voice->getMusicHasBeenInsertedInVoice ())
            os << endl;
          break;
          
        case msrVoice::kMasterVoice:
          if (gMsrOptions->fShowMasterVoices)
            os << endl;
          break;
      } // switch

    } // for
  }

  idtr--;
}

void msrStaff::printStructure (ostream& os)
{
  os <<
    "Staff" " " << getStaffName () <<
    ", " << staffKindAsString () <<
    " (" <<
    singularOrPlural (
      fStaffVoiceRelativeNumberToVoiceMap.size(), "voice", "voices") <<
    ")" <<
    endl;

  idtr++;

/* JMI
  os <<
    idtr << "StaffInstrumentName: \"" <<
    fStaffInstrumentName << "\"" << endl;
*/

/* JMI
  if (fStaffTuningsList.size ()) {
    os <<
      idtr << "Staff tunings:" <<
      endl;
      
    list<S_msrStaffTuning>::const_iterator
      iBegin = fStaffTuningsList.begin(),
      iEnd   = fStaffTuningsList.end(),
      i      = iBegin;
      
    idtr++;
    for ( ; ; ) {
      os << idtr << (*i)->staffTuningAsString ();
      if (++i == iEnd) break;
      os << endl;
    } // for
    os << endl;
    idtr--;
  }
*/

  // print the registered voices names
  if (fStaffVoiceRelativeNumberToVoiceMap.size ()) {
    os << idtr <<
      "Voices:" <<
      endl;
  
    idtr++;
    
    map<int, S_msrVoice>::const_iterator
      iBegin = fStaffVoiceRelativeNumberToVoiceMap.begin(),
      iEnd   = fStaffVoiceRelativeNumberToVoiceMap.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      S_msrVoice
        voice =
          (*i).second;
          
      os <<
        idtr << left <<
          voice->getVoiceName () <<
          " (" <<
          singularOrPlural (
            voice->getVoiceActualNotesCounter (),
            "actual note",
            "actual notes") <<
          ", " <<
          singularOrPlural (
            voice->getVoiceStanzasMap ().size(),
            "stanza",
            "stanzas") <<
          ")";
      if (++i == iEnd) break;
      os << endl;
    } // for

    idtr --;
  }

  idtr--;

  os <<
    endl;
}

//______________________________________________________________________________ 
S_msrPart msrPart::create (
  int            inputLineNumber,
  string         partID,
  S_msrPartGroup partPartGroupUplink)
{
  msrPart* o =
    new msrPart (
      inputLineNumber,
      partID,
      partPartGroupUplink);
  assert(o!=0);
  return o;
}

msrPart::msrPart (
  int            inputLineNumber,
  string         partID,
  S_msrPartGroup partPartGroupUplink)
    : msrElement (inputLineNumber)
{
  // replace spaces in part ID to set fPartID
  for_each (
    partID.begin(),
    partID.end(),
    stringSpaceReplacer (fPartID, '_'));

  // set part's part group uplink
  msrAssert(
    partPartGroupUplink != 0,
    "partPartGroupUplink is null");
    
  fPartPartGroupUplink = partPartGroupUplink;

  initializePart ();
}

void msrPart::initializePart ()
{
  // is this part name in the part renaming map?
  map<string, string>::const_iterator
    it =
      gMsrOptions->fPartsRenaming.find (fPartID);
        
  if (it != gMsrOptions->fPartsRenaming.end ()) {
    // yes, rename the part accordinglingly
    fPartMsrName = (*it).second;
  }
  else {
    // coin the name from the argument
    fPartMsrName =
      "P_"+stringNumbersToEnglishWords (fPartID);
  }
    
  if (gGeneralOptions->fTraceParts)
    cerr << idtr <<
      "Creating part " << getPartCombinedName () << endl;

  fPartDivisionsPerQuarterNote = -417; // JMI
  
  setPartMeasurePositionHighTide (
    fInputLineNumber, 1);

  // set part current time to the default 4/4 time signature
  fPartCurrentTime =
    msrTime::create (
      fInputLineNumber,
      4, 4);

  // the part harmony staff and voice will be created later
  // in setPartDivisionsPerQuarterNote()
}

msrPart::~msrPart() {}

S_msrPart msrPart::createPartShallowClone (S_msrPartGroup partGroupClone)
{
  if (gGeneralOptions->fTraceParts) {
    cerr << idtr <<
      "Creating a shallow clone of part " <<
      getPartCombinedName () <<
      endl;
  }

  msrAssert(
    partGroupClone != 0,
    "partGroupClone is null");
    
  S_msrPart
    clone =
      msrPart::create (
        fInputLineNumber,
        fPartID,
        partGroupClone);

  // it is not enough just to set fPartDivisionsPerQuarterNote,
  // since the part harmony staff and voice should be created
  clone->
    setPartDivisionsPerQuarterNote (
      fPartDivisionsPerQuarterNote);
    
  clone->fPartName =
    fPartName;
  clone->fPartAbbreviation =
    fPartAbbreviation;
  
  clone->fPartInstrumentName =
    fPartInstrumentName;
  clone->fPartInstrumentAbbreviation =
    fPartInstrumentAbbreviation;

  return clone;
}

void msrPart::setupDurationsDivisions (int divisionPerQuarterNote)
{
  if (gGeneralOptions->fTraceDivisions) {
  cerr <<
    "Setting durations divisions for part \"" <<
    "\"" << fPartName <<
    ", divisionPerQuarterNote = " << divisionPerQuarterNote <<
    endl;
  }
  
  // erase fPartDurationsToDivisions's contents
  fPartDurationsToDivisions.clear ();
  
  // positive powers of 2 of a quarter note
  int bigDivisions = divisionPerQuarterNote;
  for (int i = kQuarter; i >= kMaxima; i--) {
    /*
    cerr <<
      msrDurationAsString (msrDuration (i)) <<
      " -> " <<
      bigDivisions <<
      endl;
    */
    
    fPartDurationsToDivisions.push_front (
      make_pair (msrDuration (i), bigDivisions));
      
    bigDivisions *= 2;
  } // for

  if (divisionPerQuarterNote > 1) {
    // negative powers of 2 of a quarter note
    int
      smallDivisions =
        divisionPerQuarterNote / 2;
    msrDuration
      currentDuration =
        kEighth;
    
    while (smallDivisions >= 1) {
      /*
      cerr <<
        msrDurationAsString (currentDuration) <<
        " % --> " <<
        smallDivisions <<
        endl;
      */

      fPartDurationsToDivisions.push_back (
        make_pair (currentDuration, smallDivisions));
        
      currentDuration = msrDuration (currentDuration + 1);
      smallDivisions /= 2;
    } // while
  }

  if (gGeneralOptions->fTraceDivisions) {
    printDurationsDivisions (cerr);
  }
}

int msrPart::durationAsDivisions (
  int         inputLineNumber,
  msrDuration duration)
{
  for (
    list<pair<msrDuration, int> >::const_iterator i =
      fPartDurationsToDivisions.begin();
    i != fPartDurationsToDivisions.end ();
    i++) {
    if ((*i).first == duration)
      return (*i).second;
  } // for

  stringstream s;

  s <<
    "duration " << duration <<
    " cannot be converted to divisions with " <<
    fPartDivisionsPerQuarterNote << " dpqn" <<
    endl;

  printDurationsDivisions (s);
  
  msrInternalError (
    inputLineNumber,
    s.str ())
}

void msrPart::printDurationsDivisions (ostream& os)
{
  os << idtr <<
    "The mapping of durations to divisions with " <<
    fPartDivisionsPerQuarterNote << " divisions per quarter note" <<
    " is:" <<
    endl;

  idtr++;
  
  if (fPartDurationsToDivisions.size ()) {
    list<pair<msrDuration, int> >::const_iterator
      iBegin = fPartDurationsToDivisions.begin(),
      iEnd   = fPartDurationsToDivisions.end(),
      i      = iBegin;
          
    for ( ; ; ) {
      os << idtr <<
        setw(6) << left <<
        msrDurationAsString (msrDuration((*i).first)) <<
        ": " <<
        setw(4) << right <<
        (*i).second;

      if (++i == iEnd) break;
      
      os << endl;
    } // for

/* JMI

    for (
      list<pair<msrDuration, int> >::const_iterator i =
        fPartDurationsToDivisions.begin();
      i != fPartDurationsToDivisions.end ();
      i++) {
      os << idtr <<
        setw(6) << left <<
        msrDurationAsString (msrDuration((*i).first)) <<
        ": " <<
        setw(4) << right <<
        (*i).second <<
        endl;
    } // for
*/
  }
  
  else
    os << idtr <<
      "an empty list";

  os << endl;

  idtr--;
}

string msrPart::divisionsAsMsrString (
  int  inputLineNumber,
  int  divisions,
  int& numberOfDotsNeeded)
{  
  string result;

  // the result is a base duration, followed by a suffix made of
  // either a sequence of dots or a multiplication factor
  
  if (gGeneralOptions->fTraceDivisions) {
    cerr <<
      endl <<
      "--> divisionsAsMsrString ():" <<
      endl <<
      "inputLineNumber        = " << inputLineNumber <<
      endl <<
      "divisions              = " << divisions <<
      endl << endl;

      printDurationsDivisions (cerr);
  }
    
  msrDuration baseDuration          = k1024th;
  int         baseDurationDivisions = -1;
  
  // search fPartDurationsToDivisions in longer to shortest order
  list<pair<msrDuration, int> >::const_iterator
    iBegin = fPartDurationsToDivisions.begin(),
    iEnd   = fPartDurationsToDivisions.end(),
    i      = iBegin;
  for ( ; ; ) {
    if (i == iEnd) {
      stringstream s;

      s <<
        "divisions " << divisions <<
        " could not be handled by divisionsAsMsrString () with:" <<
        endl;

      printDurationsDivisions (cerr);

      msrInternalError (
        inputLineNumber, s.str());
      break;
    }

    if ((*i).second <= divisions) {
      // found base duration in list
      baseDuration          = (*i).first;
      baseDurationDivisions = (*i).second;

      result =
        msrDurationAsString (baseDuration);
      
      if (false && gGeneralOptions->fTraceDivisions) {
        cerr <<
          "divisions              = " << divisions <<
          endl <<
          "baseDuration           = " << msrDurationAsString (baseDuration) <<
          endl <<
          "baseDurationDivisions  = " << baseDurationDivisions <<
          endl <<
          "result                 = " << result <<
          endl << endl;
      }

      break;
    }
        
    // next please!
    i++;
  } // for

  int         dotsNumber = 0;

  if (divisions > baseDurationDivisions) {
    // divisions is not a power of 2 of a quarter note
    
    // the next element in the list is half as long as (*i)
    int remainingDivisions =
      divisions - baseDurationDivisions;
    int nextDivisionsInList =
      baseDurationDivisions / 2;

    if (false && gGeneralOptions->fTraceDivisions) {
      cerr <<
        "divisions              = " << divisions <<
        endl <<
        "baseDurationDivisions  = " << baseDurationDivisions <<
        endl <<
        "nextDivisionsInList    = " << nextDivisionsInList <<
        endl <<
        "remainingDivisions     = " << remainingDivisions <<
        endl << endl;
    }

    if (remainingDivisions < nextDivisionsInList) {
      // the suffix is a multiplication factor
      rational r (
        divisions,
        baseDurationDivisions);
      r.rationalise ();

      if (false && gGeneralOptions->fTraceDivisions) {
        cerr <<
          "divisions              = " << divisions <<
          endl <<
          "baseDurationDivisions  = " << baseDurationDivisions <<
          endl <<
          "r.getNumerator ()      = " << r.getNumerator () <<
          endl <<
          "r.getDenominator ()    = " << r.getDenominator () <<
          endl << endl;
      }
      
      result +=
        "*" +
        to_string (r.getNumerator ()) +
        "/" +
        to_string (r.getDenominator ());
    }

    else {
      dotsNumber = 1; // account for next element in the list
      
      while (remainingDivisions > nextDivisionsInList) {
        dotsNumber++;
        remainingDivisions -= nextDivisionsInList;
        nextDivisionsInList /= 2;
  
        if (false && gGeneralOptions->fTraceDivisions) {
          cerr <<
            "divisions              = " << divisions <<
            endl <<
            "baseDurationDivisions  = " << baseDurationDivisions <<
            endl <<
            "nextDivisionsInList    = " << nextDivisionsInList <<
            endl <<
            "remainingDivisions     = " << remainingDivisions <<
            endl <<
            "dotsNumber             = " << dotsNumber <<
            endl << endl;
        }
          
        if (dotsNumber > 5 )
          break; // JMI
      } // while
  
      if (false && gGeneralOptions->fTraceDivisions) {
        cerr <<
          "divisions              = " << divisions <<
          endl <<
          "baseDurationDivisions  = " << baseDurationDivisions <<
          endl <<
          "nextDivisionsInList    = " << nextDivisionsInList <<
          endl <<
          "remainingDivisions     = " << remainingDivisions <<
          endl <<
          "dotsNumber             = " << dotsNumber <<
          endl << endl;
      }
          
      if (remainingDivisions - nextDivisionsInList == 0) {
        // the suffix is composed of dots
      for (int k = 0; k < dotsNumber; k++)
        result += ".";
      }
    }
  }

  numberOfDotsNeeded = dotsNumber;

  if (false && gGeneralOptions->fTraceDivisions) {
    cerr <<
      endl <<
      "<-- divisionsAsMsrString (): returns " << result <<
      endl;
  }
  
  return result;
}

string msrPart::divisionsAsMsrString (
  int  inputLineNumber,
  int  divisions)
{
  int numberOfDots; // to be ignored

  return
    divisionsAsMsrString (
      inputLineNumber,
      divisions,
      numberOfDots);
}

string msrPart::tupletDivisionsAsMsrString (
  int inputLineNumber,
  int divisions,
  int actualNotes,
  int normalNotes)
{
  return
    divisionsAsMsrString (
      inputLineNumber,
      divisions * actualNotes / normalNotes);
}

void msrPart::testDivisionsAndDurations ()
{
  int divisionsPerQuarterNote = 8;
  cerr <<
    "divisionsPerQuarterNote = " << divisionsPerQuarterNote <<
    endl <<
    endl;
    
  setupDurationsDivisions (divisionsPerQuarterNote);

  int k;

  k = 32;
  cerr <<
    "divisionsAsMsrString (" << k << ") = " <<
    divisionsAsMsrString (
      133, k) <<
    endl <<
    endl;
    
  k = 16;
  cerr <<
    "divisionsAsMsrString (" << k << ") = " <<
    divisionsAsMsrString (
      133, k) <<
    endl <<
    endl;
    
  k = 24;
  cerr <<
    "divisionsAsMsrString (" << k << ") = " <<
    divisionsAsMsrString (
      133, k) <<
    endl <<
    endl;
    
  k = 28;
  cerr <<
    "divisionsAsMsrString (" << k << ") = " <<
    divisionsAsMsrString (
      133, k) <<
    endl <<
    endl;
    
  k = 20;
  cerr <<
    "divisionsAsMsrString (" << k << ") = " <<
    divisionsAsMsrString (
      133, k) <<
    endl <<
    endl;
    
  k = 40;
  cerr <<
    "divisionsAsMsrString (" << k << ") = " <<
    divisionsAsMsrString (
      133, k) <<
    endl <<
    endl;
    
  exit (0);
}

void msrPart::testTupletSoundingDivisionsAndDurations ()
{
  int divisionsPerQuarterNote = 30;
  cerr <<
    "divisionsPerQuarterNote = " << divisionsPerQuarterNote <<
    endl <<
    endl;
    
  setupDurationsDivisions (divisionsPerQuarterNote);

  int k, actual, normal;

  k = 12;
  actual = 5;
  normal = 4;

  cerr <<
    "tupletDivisionsAsMsrString (" << k << // JMI
    ", " << actual <<
    ", " << normal <<
    ") = " <<
    tupletDivisionsAsMsrString (
      133,
      k,
      actual,
      normal) <<
    endl <<
    endl;

  k = 20;
  actual = 3;
  normal = 2;

  cerr <<
    "tupletDivisionsAsMsrString (" << k <<
    ", " << actual <<
    ", " << normal <<
    ") = " <<
    tupletDivisionsAsMsrString (
      133,
      k,
      actual,
      normal) <<
    endl <<
    endl;

  exit (0);
}

void msrPart::createPartHarmonyStaffAndVoice (
  int inputLineNumber)
{
  // create the part harmony staff
  const int partHarmonyStaffNumber = -1;

  if (gGeneralOptions->fTraceHarmonies)
    cerr << idtr <<
      "Creating the harmony staff" <<
      " with number " << partHarmonyStaffNumber <<
      " for part " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;

  fPartHarmonyStaff =
    addStaffToPartByItsNumber (
      inputLineNumber,
      msrStaff::kHarmonyStaff,
      partHarmonyStaffNumber);
    
  // create the part harmony voice
  const int partHarmonyVoiceNumber = -1;
  
  if (gGeneralOptions->fTraceHarmonies)
    cerr << idtr <<
      "Creating the harmony voice for part " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;

  fPartHarmonyVoice =
    msrVoice::create (
      inputLineNumber,
      this,
      msrVoice::kHarmonyVoice,
      partHarmonyVoiceNumber, // JMI
      fPartHarmonyStaff);

  fPartHarmonyStaff->
    registerVoiceInStaff (
      inputLineNumber,
      fPartHarmonyVoice );
}

void msrPart::setPartMeasurePositionHighTide (
  int inputLineNumber,
  int measurePosition)
{
  if (gGeneralOptions->fTraceDivisions || gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Setting measure position high tide for part \"" <<
      getPartCombinedName () <<
      "\" to " << measurePosition <<
      ", line " << inputLineNumber <<
      endl;

  fPartMeasurePositionHighTide = measurePosition;
}

void msrPart::updatePartMeasurePositionHighTide (
  int inputLineNumber,
  int measurePosition)
{
  if (measurePosition > fPartMeasurePositionHighTide) {
    if (gGeneralOptions->fTraceDivisions || gGeneralOptions->fTraceMeasures)
        cerr << idtr <<
          "Updating measure position high tide for part \"" <<
          getPartCombinedName () <<
          "\" to " << measurePosition <<
          ", line " << inputLineNumber <<
          endl;

    fPartMeasurePositionHighTide = measurePosition;
  }
}

void msrPart::setPartMsrName (string partMsrName)
{
  // is this part name in the part renaming map?
  map<string, string>::const_iterator
    it =
      gMsrOptions->fPartsRenaming.find (fPartMsrName);
        
  if (it != gMsrOptions->fPartsRenaming.end ()) {
    // yes, rename the part accordinglingly
    fPartMsrName = (*it).second;

    if (gGeneralOptions->fTraceParts)
      cerr << idtr <<
        "Setting part name of " << getPartCombinedName () <<
        " to \"" << fPartMsrName << "\"" <<
         endl;
  }
  else {
    // use the argument
    fPartMsrName = partMsrName;

    if (gGeneralOptions->fTraceParts)
      cerr << idtr <<
        "Keeping partID \"" << partMsrName <<
        "\" as part name  for " << getPartCombinedName () <<
      endl;
  }
}

string msrPart::getPartCombinedName () const
{
  stringstream s;
  
  s <<
    "\"" << fPartMsrName << "\"" <<
    " (" << fPartID;

  if (fPartName.size ())
    s <<
      ", \"" << fPartName << "\"";

  s <<
    ")";

  return s.str();
}

void msrPart::setPartDivisionsPerQuarterNote (
  int divisionsPerQuarterNote)
{
  msrAssert(
    divisionsPerQuarterNote != 0,
    "divisionsPerQuarterNote == 0");
  
  if (gGeneralOptions->fTraceDivisions)
    cerr << idtr <<
      "Setting part divisions per quarter note to " <<
      divisionsPerQuarterNote <<
      " in part " << getPartCombinedName () <<
      endl;

  // register divisions per quarter note
  fPartDivisionsPerQuarterNote =
    divisionsPerQuarterNote;

  // initialize fPartDurationsToDivisions
  setupDurationsDivisions (
    fPartDivisionsPerQuarterNote);

  // create the part harmony staff and voice
  // only now because we need the divisions per quarter notes
  createPartHarmonyStaffAndVoice (
    fInputLineNumber);
}

void msrPart::createAndAppendMeasureToPart (
  int    inputLineNumber,
  string measureNumber)
{
  if (gGeneralOptions->fTraceMeasures)
    cerr <<
      idtr <<
        "Setting part measure number to " << measureNumber <<
        ", line " << inputLineNumber <<
        ", in part " << getPartCombinedName () <<
        endl;

  // set part measure location
  fPartMeasureNumber = measureNumber;

  // propagate it to all staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    S_msrStaff
      staff = (*i).second;
      
    staff->
      createAndAppendMeasureToStaff (
        inputLineNumber, measureNumber);
  } // for
}

void msrPart::appendStaffDetailsToPart (
  S_msrStaffDetails staffDetails)
{
  if (gGeneralOptions->fTraceStaves || gGeneralOptions->fTraceParts)
    cerr << idtr <<
      "Appending staff details\"" <<
      staffDetails->staffDetailsAsShortString () <<
      "\" to part " << getPartCombinedName () <<
    endl;

  // register staff details in part
  fCurrentPartStaffDetails = staffDetails;
  
  // propagate it to all staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    S_msrStaff
      staff = (*i).second;

    staff->
      appendStaffDetailsToStaff (staffDetails);
  } // for
}

void msrPart::setPartCurrentClef (S_msrClef clef)
{
  if (gGeneralOptions->fTraceParts)
    cerr << idtr <<
      "Setting part current clef \"" <<
      clef->clefAsString () <<
      "\" in part " << getPartCombinedName () <<
    endl;

  // set part clef
  fPartCurrentClef = clef; // JMI

  // propagate it to all staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->
      setStaffCurrentClef (clef);
  } // for
}

void msrPart::setPartCurrentKey  (S_msrKey  key)
{
  if (gGeneralOptions->fTraceParts || gGeneralOptions->fTraceKeys)
    cerr << idtr <<
      "Setting part current key to \"" <<
      key->keyAsString () <<
      "\" in part " << getPartCombinedName () <<
    endl;

  // set part key
  fPartCurrentKey = key;

  // propagate it to all staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    S_msrStaff
      staff = (*i).second;

    staff->
      setStaffCurrentKey (key);
  } // for
}

void msrPart::setPartCurrentTime (S_msrTime time)
{
  if (gGeneralOptions->fTraceParts || gGeneralOptions->fTraceTimes)
    cerr << idtr <<
      "Setting part current time to \"" <<
      time->timeAsString () <<
      "\" in part " << getPartCombinedName () <<
    endl;

  // set part time
  fPartCurrentTime = time;

  // propagate it to all staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    S_msrStaff
      staff = (*i).second;

    staff->
      setStaffCurrentTime (time);
  } // for
}

void msrPart::setPartTranspose (S_msrTranspose transpose)
{
  if (gGeneralOptions->fTraceParts)
    cerr << idtr <<
      "Setting part transpose \"" <<
      transpose->transposeAsString () <<
      "\" in part " << getPartCombinedName () <<
    endl;

  // set part transpose
  fPartTranspose = transpose;

  // propagate it to all staves
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    S_msrStaff
      staff = (*i).second;

    staff->
      setStaffTranspose (transpose);
  } // for
}


void msrPart::createAndAppendRepeatToPart (int inputLineNumber)
{
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->
      createAndAppendRepeatToStaff (inputLineNumber);
  } // for
}

void msrPart::appendRepeatEndingToPart (
  int       inputLineNumber,
  string    repeatEndingNumber, // may be "1, 2"
  msrRepeatEnding::msrRepeatEndingKind
            repeatEndingKind)
{
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->
      appendRepeatEndingToStaff (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind);
  } // for
}

void msrPart::appendRepeatCloneToPart (
  int         inputLineNumber,
  S_msrRepeat repeatCLone)
{
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    S_msrStaff
      staff =
        (*i).second;

    switch (staff->getStaffKind ()) {
      case msrStaff::kRegularStaff:
        staff->appendRepeatCloneToStaff (
          inputLineNumber, repeatCLone);
        break;
        
      case msrStaff::kTablatureStaff:
        break;
        
      case msrStaff::kPercussionStaff:
        break;
        
      case msrStaff::kHarmonyStaff:
        break;
    } // switch
  } // for
}

void msrPart::appendRepeatEndingCloneToPart (
  S_msrRepeatEnding repeatEndingCLone)
{
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->
      appendRepeatEndingCloneToStaff (repeatEndingCLone);
  } // for
}

void msrPart::createMeasureRepeatFromItsFirstMeasureInPart (
  int inputLineNumber,
  int measureRepeatMeasuresNumber,
  int measureRepeatSlashes)
{
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->
      createMeasureRepeatFromItsFirstMeasureInStaff (
        inputLineNumber,
        measureRepeatMeasuresNumber,
        measureRepeatSlashes);
  } // for
}

void msrPart::appendPendingMeasureRepeatToPart (
  int inputLineNumber)
{
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->
      appendPendingMeasureRepeatToStaff (
        inputLineNumber);
  } // for
}

void msrPart::createMultipleRestInPart (
  int inputLineNumber,
  int multipleRestMeasuresNumber)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Creating a multiple rest in part " <<
      getPartCombinedName () <<
      ", " <<
      singularOrPlural (
        multipleRestMeasuresNumber, "measure", "measures") <<
      endl;

  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->
      createMultipleRestInStaff (
        inputLineNumber,
        multipleRestMeasuresNumber);
  } // for
}

void msrPart::appendPendingMultipleRestToPart (
  int inputLineNumber)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Appending the pending multiple rest to part " <<
      getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->
      appendPendingMultipleRestToStaff (
        inputLineNumber);
  } // for
}

void msrPart::appendMultipleRestCloneToPart (
  int               inputLineNumber,
  S_msrMultipleRest multipleRest)
{
  if (gGeneralOptions->fTraceRepeats)
    cerr << idtr <<
      "Appending multiple rest '" <<
      multipleRest->multipleRestAsString () <<
      "' to part clone " <<
      getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->
      appendMultipleRestCloneToStaff (
        inputLineNumber,
        multipleRest);
  } // for
}

void msrPart::appendBarlineToPart (S_msrBarline barline)
{
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->
      appendBarlineToStaff (barline);
  } // for
}
     
S_msrStaff msrPart::addStaffToPartByItsNumber (
  int                    inputLineNumber,
  msrStaff::msrStaffKind staffKind,
  int                    staffNumber)
{
  if (fPartStavesMap.count (staffNumber)) {
    cerr << idtr <<
      "### Internal error: staffNumber " << staffNumber <<
      " already exists in part " << getPartCombinedName () <<
      endl;

    return fPartStavesMap [staffNumber];
  }

  if (gGeneralOptions->fTraceParts) {
    cerr << idtr <<
      "Adding " <<
      msrStaff::staffKindAsString (staffKind) <<
      " staff " << staffNumber <<
      " to part " << getPartCombinedName () <<
      endl;
  }
  
  // create the staff
  S_msrStaff
    staff =
      msrStaff::create (
        inputLineNumber,
        this,
        staffKind,
        staffNumber);

  // register staff in this part
  fPartStavesMap [staffNumber] = staff;

  // return staff
  return staff;
}

void msrPart::addStaffToPartCloneByItsNumber (S_msrStaff staff)
{
  if (gGeneralOptions->fTraceParts)
    cerr << idtr <<
      "Adding staff \"" << staff->getStaffName () <<
      "\" to part clone " << getPartCombinedName () <<
      endl;

  // register staff in this part
  fPartStavesMap [staff->getStaffNumber ()] = staff;
}

S_msrStaff msrPart::fetchStaffFromPart (
  int staffNumber)
{
  S_msrStaff result;
  
  if (fPartStavesMap.count (staffNumber)) {
    result = fPartStavesMap [staffNumber];
  }

  return result;
}

void msrPart::setPartHarmoniesSupplierVoice (
  int        inputLineNumber,
  S_msrVoice partHarmoniesSupplierVoice)
{
  switch (partHarmoniesSupplierVoice->getVoiceKind ()) {
    case msrVoice::kRegularVoice:
      if (! fPartHarmoniesSupplierVoice) {
        // first harmonies supplier voice met in this part,
        // set part harmonies supplier voice accordingly
        if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceParts)
          cerr << idtr <<
            "Setting voice \"" <<
            partHarmoniesSupplierVoice->
              getVoiceName () <<
            "\" as harmonies supplier for part " <<
            getPartCombinedName () <<
            ", line " << inputLineNumber <<
            endl;
    
        fPartHarmoniesSupplierVoice =
          partHarmoniesSupplierVoice;
      }
    
      else if (
        fPartHarmoniesSupplierVoice != partHarmoniesSupplierVoice) {
        stringstream s;
    
        s <<
          "harmonies are supplied both by:" <<
          endl <<
          msrVoice::voiceKindAsString (
            fPartHarmoniesSupplierVoice->getVoiceKind ()) <<
          " \"" << fPartHarmoniesSupplierVoice->getVoiceName () << "\"" <<
          endl <<
          "and:" <<
          endl <<
          msrVoice::voiceKindAsString (
            partHarmoniesSupplierVoice->getVoiceKind ()) <<
          " \"" << partHarmoniesSupplierVoice->getVoiceName () << "\"";
    
        msrMusicXMLError (
          inputLineNumber,
          s.str());
      }
      break;
      
    case msrVoice::kHarmonyVoice:
    case msrVoice::kMasterVoice:
      {
        stringstream s;
    
        s <<
          "harmonies cannot by supplied by " <<
          msrVoice::voiceKindAsString (
            partHarmoniesSupplierVoice->getVoiceKind ()) <<
          " voice \"" <<
           partHarmoniesSupplierVoice->getVoiceName () <<
           "\"";
    
        msrMusicXMLError (
          inputLineNumber,
          s.str());
      }
      break;
  } // switch
}
                              
void msrPart::appendHarmonyToPart (
  S_msrVoice   harmoniesSupplierVoice,
  S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();

  switch (harmoniesSupplierVoice->getVoiceKind ()) {
    case msrVoice::kRegularVoice:
      // register this voice as the part harmonies supplier voice
      setPartHarmoniesSupplierVoice (
        inputLineNumber,
        harmoniesSupplierVoice);
    
      // append the harmony to the part harmony voice
      if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceParts)
        cerr << idtr <<
          "Appending harmony '" <<
          harmony->harmonyAsString () <<
          "' to part " <<
          getPartCombinedName () <<
          ", line " << inputLineNumber <<
          endl;
    
      fPartHarmonyVoice->
        appendHarmonyToVoice (harmony);
      break;
      
    case msrVoice::kHarmonyVoice:
    case msrVoice::kMasterVoice:
      {
        stringstream s;
    
        s <<
          "harmonies cannot by supplied to part by " <<
          msrVoice::voiceKindAsString (
            harmoniesSupplierVoice->getVoiceKind ()) <<
          " voice \" " <<
           harmoniesSupplierVoice->getVoiceName () <<
           "\"";
    
        msrInternalError (
          inputLineNumber,
          s.str());
      }
      break;
  } // switch
}

void msrPart::appendHarmonyToPartClone (
  S_msrVoice   harmoniesSupplierVoice,
  S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();

  switch (harmoniesSupplierVoice->getVoiceKind ()) {
    case msrVoice::kHarmonyVoice:
      // append the harmony to the part harmony voice
      if (gGeneralOptions->fTraceHarmonies || gGeneralOptions->fTraceParts)
        cerr << idtr <<
          "Appending harmony '" <<
          harmony->harmonyAsString () <<
          "' to part clone " <<
          getPartCombinedName () <<
          ", line " << inputLineNumber <<
          endl;
    
      fPartHarmonyVoice->
        appendHarmonyToVoiceClone (harmony);
      break;
      
    case msrVoice::kRegularVoice:
    case msrVoice::kMasterVoice:
      {
        stringstream s;
    
        s <<
          "harmonies cannot by supplied to part clone by " <<
          msrVoice::voiceKindAsString (
            harmoniesSupplierVoice->getVoiceKind ()) <<
          " voice \" " <<
           harmoniesSupplierVoice->getVoiceName () <<
           "\"";
    
        msrInternalError (
          inputLineNumber,
          s.str());
      }
      break;
  } // switch
}

void msrPart:: handleBackup (int divisions)
{
 // JMI 
}

/*
void msrPart::finalizeUltimateMeasuresInPart (int inputLineNumber)
{
  if (gGeneralOptions->fTraceMeasures)
    cerr << idtr <<
      "Finalizing last measure in part " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;

  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->
      finalizeUltimateMeasuresInStaff (inputLineNumber);
  } // for
}
*/

void msrPart::finalizeCurrentMeasureInPart (int inputLineNumber)
{
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->
      finalizeCurrentMeasureInStaff (inputLineNumber);
  } // for
}

void msrPart::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrPart::acceptIn()" <<
      endl;
      
  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrPart::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrPart::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrPart::acceptOut()" <<
      endl;

  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrPart::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrPart::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrPart::browseData()" <<
      endl;

/* JMI
  // browse the harmony staff
  msrBrowser<msrStaff> browser (v);
  browser.browse (*fPartHarmonyStaff);
  */
  
  for (
    map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    // browse the staff
    msrBrowser<msrStaff> browser (v);
    browser.browse (*((*i).second));
  } // for

  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% <== msrPart::browseData()" <<
      endl;
}

ostream& operator<< (ostream& os, const S_msrPart& elt)
{
  elt->print (os);
  return os;
}

void msrPart::print (ostream& os)
{
  os <<
    "Part" << " " << getPartCombinedName () <<
    " (" <<
    singularOrPlural (
      fPartStavesMap.size(), "staff", "staves") <<
    ", measures " <<
    ", position high tide " << fPartMeasurePositionHighTide <<
    ")" <<
    endl;
    
  idtr++;
  
  const int fieldWidth = 29;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "PartDivisionsPerQuarterNote" << ": " <<
      fPartDivisionsPerQuarterNote << " ***" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "PartMsrName" << ": \"" <<
      fPartMsrName << "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "PartName" << ": \"" <<
      fPartName << "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "PartAbbrevation" << ": \"" <<
      fPartAbbreviation << "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
       "PartInstrumentName" << ": \"" <<
      fPartInstrumentName << "\"" <<
      endl;

/*
  // print the harmony staff
  if (fPartHarmonyStaff) {
    os <<
      endl <<
      idtr <<
        "Harmony staff:" <<
      endl;
            
    idtr++;
    os << idtr << fPartHarmonyStaff;
    idtr--;
  }
*/

  // print the staves
  if (fPartStavesMap.size()) {
    os << endl;
    for (
      map<int, S_msrStaff>::const_iterator i = fPartStavesMap.begin();
      i != fPartStavesMap.end();
      i++) {
      S_msrStaff
        staff =
          (*i).second;
          
      msrStaff::msrStaffKind
        staffKind =
          staff->getStaffKind ();

      switch (staffKind) {
        case msrStaff::kRegularStaff:
          os << idtr <<
            staff;
          break;
          
        case msrStaff::kTablatureStaff:
          os << idtr <<
            staff;
          break;
          
        case msrStaff::kPercussionStaff:
          os << idtr <<
            staff;
          break;
          
        case msrStaff::kHarmonyStaff:
          if (gMsrOptions->fShowHarmonyVoices)
          os << idtr <<
            staff;
          break;
      } // switch

      if (i != fPartStavesMap.end())
        switch (staffKind) {
          case msrStaff::kRegularStaff:
            os <<
              idtr <<
              endl;
            break;
            
          case msrStaff::kTablatureStaff:
            os <<
              idtr <<
              endl;
            break;
            
          case msrStaff::kPercussionStaff:
            os <<
              idtr <<
              endl;
            break;
            
          case msrStaff::kHarmonyStaff:
            if (gMsrOptions->fShowHarmonyVoices)
              os <<
                idtr <<
                endl;
            break;
        } // switch
    } // for
  }

  idtr--;
}

void msrPart::printStructure (ostream& os)
{
  os <<
    "Part" << " " << getPartCombinedName () <<
    " (" <<
    singularOrPlural (
      fPartStavesMap.size(), "staff", "staves") <<
    ", position high tide " << fPartMeasurePositionHighTide <<
    ")" <<
    endl;
    
  idtr++;
  
  const int fieldWidth = 27;

  os << left <<
    idtr <<
      setw(fieldWidth) <<
      "PartDivisionsPerQuarterNote" << ": " <<
      fPartDivisionsPerQuarterNote <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "PartMsrName" << ": \"" <<
      fPartMsrName << "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "PartName" << ": \"" <<
      fPartName << "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "PartAbbrevation" << ": \"" <<
      fPartAbbreviation << "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "PartInstrumentName" << ": \"" <<
      fPartInstrumentName << "\"" <<
      endl <<
    endl;

/* JMI
  // print the harmony staff
  if (fPartHarmonyStaff) {
    os <<
      endl <<
      idtr <<
        "Harmony staff" <<
      endl;
            
    idtr++;
    os << idtr << fPartHarmonyStaff;
    idtr--;
  }
*/

  // print the staves
  if (fPartStavesMap.size()) {
    map<int, S_msrStaff>::const_iterator
      iBegin = fPartStavesMap.begin(),
      iEnd   = fPartStavesMap.end(),
      i      = iBegin;
          
    for ( ; ; ) {
      os <<
        idtr;
      (*i).second->printStructure (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  idtr--;
}

//______________________________________________________________________________
int msrPartGroup::gPartGroupsCounter = 0;

S_msrPartGroup msrPartGroup::create (
  int                    inputLineNumber,
  int                    partGroupNumber,
  string                 partGroupName,
  string                 partGroupDisplayText,
  string                 partGroupAccidentalText,
  string                 partGroupAbbreviation,
  msrPartGroupSymbolKind partGroupSymbolKind,
  int                    partGroupSymbolDefaultX,
  bool                   partGroupBarline,
  S_msrPartGroup         partGroupPartGroupUplink,
  S_msrScore             partGroupScoreUplink)
{
  msrPartGroup* o =
    new msrPartGroup (
      inputLineNumber,
      partGroupNumber,
      partGroupName,
      partGroupDisplayText,
      partGroupAccidentalText,
      partGroupAbbreviation,
      partGroupSymbolKind,
      partGroupSymbolDefaultX,
      partGroupBarline,
      partGroupPartGroupUplink,
      partGroupScoreUplink);
  assert(o!=0);
  return o;
}

msrPartGroup::msrPartGroup (
  int                    inputLineNumber,
  int                    partGroupNumber,
  string                 partGroupName,
  string                 partGroupDisplayText,
  string                 partGroupAccidentalText,
  string                 partGroupAbbreviation,
  msrPartGroupSymbolKind partGroupSymbolKind,
  int                    partGroupSymbolDefaultX,
  bool                   partGroupBarline,
  S_msrPartGroup         partGroupPartGroupUplink,
  S_msrScore             partGroupScoreUplink)
    : msrElement (inputLineNumber)
{
  // generate a new partGroup absolute number
  fPartGroupAbsoluteNumber = ++gPartGroupsCounter;
  
  fPartGroupNumber = partGroupNumber;
        
  fPartGroupName = partGroupName;

  fPartGroupDisplayText    = partGroupDisplayText;
  fPartGroupAccidentalText = partGroupAccidentalText;
  
  fPartGroupAbbreviation = partGroupAbbreviation;

  fPartGroupSymbolKind     = partGroupSymbolKind;
  fPartGroupSymbolDefaultX = partGroupSymbolDefaultX;

  fPartGroupBarline = partGroupBarline;

  fPartGroupPartGroupUplink = partGroupPartGroupUplink;

  fPartGroupScoreUplink = partGroupScoreUplink;
  
  if (gGeneralOptions->fTracePartGroups)
    cerr <<
      idtr <<
      "--------------------------------------------" <<
      endl <<
      idtr <<
      "Creating part group " << fPartGroupNumber <<
      endl;
}

msrPartGroup::~msrPartGroup()
{}

S_msrPartGroup msrPartGroup::createPartGroupShallowClone (
  S_msrPartGroup partGroupClone,
  S_msrScore     scoreClone)
{
  if (gGeneralOptions->fTracePartGroups)
    cerr <<
      idtr <<
      "--------------------------------------------" <<
      endl <<
      idtr <<
      "Creating a shallow clone part group " <<
      getPartGroupCombinedName () <<
      endl;

  // don't check against 0, since the partGroup stack
  // that it comes from may be empty
/* JMI
  msrAssert(
    partGroupClone != 0,
    "partGroupClone is null");
    */
    
  S_msrPartGroup
    clone =
      msrPartGroup::create (
        fInputLineNumber,
        fPartGroupNumber,
        fPartGroupName,
        fPartGroupDisplayText,
        fPartGroupAccidentalText,
        fPartGroupAbbreviation,
        fPartGroupSymbolKind,
        fPartGroupSymbolDefaultX,
        fPartGroupBarline,
        partGroupClone,
        scoreClone);

  // avoid part group clone to keep its (new) absolute number
  clone->fPartGroupAbsoluteNumber =
    fPartGroupAbsoluteNumber;
  
  return clone;
}

string msrPartGroup::getPartGroupCombinedName () const
{
  stringstream s;

  s <<
    "PG_" << fPartGroupAbsoluteNumber <<
    " (" << fPartGroupNumber << ")";

  return s.str();
}

S_msrPart msrPartGroup::addPartToPartGroupByItsID (
  int    inputLineNumber,
  string partID)
{
  if (fPartGroupPartsMap.count (partID)) {
    stringstream s;

    s <<
      "partID \"" << partID <<
      "\" already exists in part group " <<
      getPartGroupCombinedName ();

    msrMusicXMLWarning ( // JMI
      inputLineNumber,
      s.str ());

    return fPartGroupPartsMap [partID];
  }

  // create the part
  S_msrPart
    part =
      msrPart::create (
        inputLineNumber,
        partID,
        this);

  // register it in this part group
  if (gGeneralOptions->fTracePartGroups) {
    cerr << idtr <<
      "Adding part " <<
      part->getPartCombinedName () <<
      " to part group " << fPartGroupNumber <<
      endl;
  }
  
  fPartGroupPartsMap [partID] = part;
  fPartGroupElements.push_back (part);

  if (gGeneralOptions->fTracePartGroups) {
    cerr <<
      endl <<
      idtr <<
        "After addPartToPartGroupByItsID, fPartGroupPartsMap contains:" <<
        endl;
        
    idtr++;
    
    for (
        map<string, S_msrPart>::const_iterator i = fPartGroupPartsMap.begin();
        i != fPartGroupPartsMap.end();
        i++) {
      cerr << idtr <<
        "\"" << (*i).first << "\" --% --> " <<
        (*i).second->getPartCombinedName() <<
        endl;
    } // for
    
    idtr--;
    
    cerr << idtr <<
 // JMI     "% ==> addPartToPartGroup" <<
      endl << endl <<

    idtr <<
      "After addPartToPartGroupByItsID, fPartGroupPartsList contains:" <<
      endl;
      
    if (fPartGroupElements.size()) {
      list<S_msrElement>::const_iterator
        iBegin = fPartGroupElements.begin(),
        iEnd   = fPartGroupElements.end(),
        i      = iBegin;
        
      idtr++;
      for ( ; ; ) {
        cerr << idtr << (*i);
        if (++i == iEnd) break;
        cerr << endl;
      } // for
      idtr--;
    }
    
    cerr <<
  // JMI    idtr << "% ==> addPartToPartGroupByItsID" <<
      endl << endl;
  }
  
  // return the part
  return part;
}

void msrPartGroup::addPartToPartGroup (S_msrPart part)
{
  // register part in this part group
  if (gGeneralOptions->fTracePartGroups) {
    cerr << idtr <<
      "Adding part " <<
      part->getPartCombinedName () <<
      " to part group " << fPartGroupNumber <<
      endl;
  }
  
  fPartGroupPartsMap [part->getPartID ()] = part;
  fPartGroupElements.push_back (part);
}

void msrPartGroup::prependSubPartGroupToPartGroup (
  S_msrPartGroup partGroup)
{
  if (gGeneralOptions->fTracePartGroups)
    cerr << idtr <<
      "Prepending (sub-)part group " << partGroup->getPartGroupNumber () <<
      " to part group " << getPartGroupNumber ()  << endl;

  // register it in this part group
  fPartGroupElements.push_front (partGroup);
}

void msrPartGroup::appendSubPartGroupToPartGroup (
  S_msrPartGroup partGroup)
{
  if (gGeneralOptions->fTracePartGroups)
    cerr << idtr <<
      "Appending (sub-)part group " << partGroup->getPartGroupNumber () <<
      " to part group " << getPartGroupNumber ()  << endl;

  // register it in this part group
  fPartGroupElements.push_back (partGroup);
}

S_msrPart msrPartGroup::fetchPartFromPartGroup (
  string partID)
{
  if (gGeneralOptions->fTracePartGroups) {
    cerr << idtr <<
      "fetchPartFromPartGroup(), fPartGroupPartsMap contains:" << endl;
    for (
        map<string, S_msrPart>::const_iterator i = fPartGroupPartsMap.begin();
        i != fPartGroupPartsMap.end();
        i++) {
      cerr << idtr <<
        (*i).first << " --% --> " <<
        (*i).second->getPartCombinedName() <<
        endl;
    } // for
    cerr << idtr <<
  // JMI    "% ==> fetchPartFromPartGroup" <<
      endl;
  }
  
  S_msrPart result;
  
  if (fPartGroupPartsMap.count (partID)) {
    result = fPartGroupPartsMap [partID];
  }

  return result;
}

void msrPartGroup::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrPartGroup::acceptIn()" <<
      endl;
      
  if (visitor<S_msrPartGroup>*
    p =
      dynamic_cast<visitor<S_msrPartGroup>*> (v)) {
        S_msrPartGroup elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrPartGroup::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrPartGroup::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrPartGroup::acceptOut()" <<
      endl;

  if (visitor<S_msrPartGroup>*
    p =
      dynamic_cast<visitor<S_msrPartGroup>*> (v)) {
        S_msrPartGroup elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrPartGroup::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrPartGroup::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrPartGroup::browseData()" <<
      endl;
  
  for (
    list<S_msrElement>::const_iterator i = fPartGroupElements.begin();
    i != fPartGroupElements.end();
    i++) {
    // browse the part element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% <== msrPartGroup::browseData()" <<
      endl;
}

ostream& operator<< (ostream& os, const S_msrPartGroup& elt)
{
  elt->print (os);
  return os;
}

string msrPartGroup::partGroupSymbolKindAsString (
  msrPartGroupSymbolKind partGroupSymbolKind)
{
  string result;
  
  switch (partGroupSymbolKind) {
    case k_NoPartGroupSymbol:
      result = "none";
      break;
    case kBracePartGroupSymbol:
      result = "brace";
      break;
    case kBracketPartGroupSymbol:
      result = "bracket";
      break;
    case kLinePartGroupSymbol:
      result = "line";
      break;
    case kSquarePartGroupSymbol:
      result = "square";
      break;
  } // switch

  return result;
}

void msrPartGroup::print (ostream& os)
{
  os <<
    "PartGroup" " \"" << getPartGroupCombinedName () <<
    "\" (" <<
    singularOrPlural (
      fPartGroupPartsMap.size(), "part", "parts") <<
    ")" <<
    endl;
    
  idtr++;

  const int fieldWidth = 23;
  
  os << left <<
    idtr <<
      setw(fieldWidth) << "PartGroupName" << " : \"" <<
      fPartGroupName <<
      "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) << "PartGroupDisplayText" << " : \"" <<
      fPartGroupDisplayText <<
      "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) << "PartGroupAccidentalText" << " : \"" <<
      fPartGroupAccidentalText <<
      "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) << "PartGroupAbbrevation" << " : \"" <<
      fPartGroupAbbreviation <<
      "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) << "PartGroupSymbolDefaultX" << " : " <<
      fPartGroupSymbolDefaultX <<
        endl <<
    idtr <<
      setw(fieldWidth) << "PartGroupSymbolKind" << " : " <<
      partGroupSymbolKindAsString (fPartGroupSymbolKind) <<
      endl;
    
  os <<
    idtr <<
    setw(fieldWidth) << "PartGroupBarline" << " : ";
  if (fPartGroupBarline)
    os << "true";
  else
    os << "false";
  os << endl;

  if (fPartGroupElements.size()) {
    os << endl;
    list<S_msrElement>::const_iterator
      iBegin = fPartGroupElements.begin(),
      iEnd   = fPartGroupElements.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  
  idtr--;
}

void msrPartGroup::printStructure (ostream& os)
{
  os <<
    "PartGroup" " \"" << getPartGroupCombinedName () <<
    "\" (" <<
    singularOrPlural (
      fPartGroupPartsMap.size(), "part", "parts") <<
    ")" <<
    endl;
    
  idtr++;

  const int fieldWidth = 24;
  
  os << left <<
    idtr <<
      setw(fieldWidth) << "PartGroupName" << " : \"" <<
      fPartGroupName <<
      "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) << "PartGroupAbbrevation" << " : \"" <<
      fPartGroupAbbreviation <<
      "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) << "fPartGroupSymbolDefaultX" << " : " <<
      fPartGroupSymbolDefaultX <<
        endl <<
    idtr <<
      setw(fieldWidth) << "fPartGroupSymbolKind" << " : \"" <<
      partGroupSymbolKindAsString (fPartGroupSymbolKind) <<
      "\"" <<
      endl;
    
  os <<
    idtr << "PartGroupBarline         : ";
  if (fPartGroupBarline)
    os << "true";
  else
    os << "false";
  os << endl;

  if (fPartGroupElements.size()) {
    os << endl;
    list<S_msrElement>::const_iterator
      iBegin = fPartGroupElements.begin(),
      iEnd   = fPartGroupElements.end(),
      i      = iBegin;
      
    idtr++;
    for ( ; ; ) {
      os << idtr;
      (*i)->printStructure (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
    idtr--;
  }
  
  idtr--;
}

//______________________________________________________________________________
S_msrIdentification msrIdentification::create (
  int inputLineNumber)
{
  msrIdentification* o =
    new msrIdentification (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrIdentification::msrIdentification (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrIdentification::~msrIdentification() {}

void msrIdentification::setWorkNumber (
  int    inputLineNumber,
  string val)
  {
  fWorkNumber =
    msrVarValAssoc::create (
      inputLineNumber,
      "work-number", val);
  }

void msrIdentification::setWorkTitle (
  int    inputLineNumber,
  string val)
  {
  fWorkTitle =
    msrVarValAssoc::create (
      inputLineNumber,
      "work-title", val);
  }

void msrIdentification::setMovementNumber (
  int    inputLineNumber,
  string val)
  {
  fMovementNumber =
    msrVarValAssoc::create (
      inputLineNumber,
      "movement-number", val);
  }

void msrIdentification::setMovementTitle (
  int    inputLineNumber,
  string val)
{
  fMovementTitle =
    msrVarValAssoc::create (
      inputLineNumber,
      "movement-title", val);
}

void msrIdentification::addComposer (
  int    inputLineNumber,
  string type,
  string val)
{
  fComposers.push_back(
    msrVarValAssoc::create (
      inputLineNumber,
      type, val)
  );
}

void msrIdentification::addArranger (
  int    inputLineNumber,
  string type,
  string val)
{
  fArrangers.push_back(
    msrVarValAssoc::create (
      inputLineNumber,
      type, val)
  );
}

void msrIdentification::addPoet (
  int    inputLineNumber,
  string type,
  string val)
{
  fPoets.push_back(
    msrVarValAssoc::create (
      inputLineNumber,
      type, val)
  );
}

void msrIdentification::addLyricist (
  int    inputLineNumber,
  string type,
  string val)
{
  fLyricists.push_back(
    msrVarValAssoc::create (
      inputLineNumber,
      type, val)
  );
}

void msrIdentification::setRights (
  int    inputLineNumber,
  string val)
  {
  fRights =
    msrVarValAssoc::create (
      inputLineNumber,
      "rights", val);
  }

void msrIdentification::addSoftware (
  int    inputLineNumber,
  string val)
{
  fSoftwares.push_back(
    msrVarValAssoc::create (
      inputLineNumber,
      "software", val)
  );
}

void msrIdentification::setEncodingDate (
  int    inputLineNumber,
  string val)
{
  fEncodingDate =
    msrVarValAssoc::create (
      inputLineNumber,
      "encoding-date", val);
}

void msrIdentification::setScoreInstrumentAssoc (
  int    inputLineNumber,
  string val)
{
  fScoreInstrumentAssoc =
    msrVarValAssoc::create (
      inputLineNumber,
      "score-instrument", val);
}

void msrIdentification::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrIdentification::acceptIn()" <<
      endl;
      
  if (visitor<S_msrIdentification>*
    p =
      dynamic_cast<visitor<S_msrIdentification>*> (v)) {
        S_msrIdentification elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrIdentification::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrIdentification::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrIdentification::acceptOut()" <<
      endl;

  if (visitor<S_msrIdentification>*
    p =
      dynamic_cast<visitor<S_msrIdentification>*> (v)) {
        S_msrIdentification elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrIdentification::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrIdentification::browseData (basevisitor* v)
{
  if (fWorkNumber) {
    // browse fWorkNumber
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fWorkNumber);
  }

  if (fWorkTitle) {
    // browse fWorkTitle
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fWorkTitle);
  }

  if (fMovementNumber) {
    // browse fMovementNumber
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fMovementNumber);
  }

  if (fMovementTitle) {
    // fMovementTitle fMovementTitle
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fMovementTitle);
  }

  if (!fComposers.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i;
    for (i=fComposers.begin(); i!=fComposers.end(); i++) {
      // browse creator
      msrBrowser<msrVarValAssoc> browser (v);
      browser.browse (*(*i));
    } // for
  }
    
  if (!fArrangers.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i;
    for (i=fArrangers.begin(); i!=fArrangers.end(); i++) {
      // browse creator
      msrBrowser<msrVarValAssoc> browser (v);
      browser.browse (*(*i));
    } // for
  }
    
  if (!fPoets.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i;
    for (i=fPoets.begin(); i!=fPoets.end(); i++) {
      // browse creator
      msrBrowser<msrVarValAssoc> browser (v);
      browser.browse (*(*i));
    } // for
  }
    
  if (!fLyricists.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i;
    for (i=fLyricists.begin(); i!=fLyricists.end(); i++) {
      // browse creator
      msrBrowser<msrVarValAssoc> browser (v);
      browser.browse (*(*i));
    } // for
  }
    
  if (fRights) {
    // browse rights
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fRights);
  }

  if (!fSoftwares.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i;
    for (i=fSoftwares.begin(); i!=fSoftwares.end(); i++) {
      // browse software
      msrBrowser<msrVarValAssoc> browser (v);
      browser.browse (*(*i));
    } // for
  }

  if (fEncodingDate) {
    // browse encoding date
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fEncodingDate);
  }

  if (fScoreInstrumentAssoc) {
    // browse score instrument
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fScoreInstrumentAssoc);
  }
}

ostream& operator<< (ostream& os, const S_msrIdentification& elt)
{
  elt->print (os);
  return os;
}

void msrIdentification::print (ostream& os)
{
  os <<
    "Identification" <<
    endl;

  bool emptyIdentification = true;

  idtr++;
  
  if (fWorkNumber) {
    os << idtr << fWorkNumber;
    emptyIdentification = false;
  }
  
  if (fWorkTitle) {
    os << idtr << fWorkTitle;
    emptyIdentification = false;
  }
    
  if (fMovementNumber) {
    os << idtr << fMovementNumber;
    emptyIdentification = false;
  }
    
  if (fMovementTitle) {
    os << idtr << fMovementTitle;
    emptyIdentification = false;
  }

  if (! fComposers.empty()) {
    vector<S_msrVarValAssoc>::const_iterator
      iBegin = fComposers.begin(),
      iEnd   = fComposers.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    emptyIdentification = false;
  }
    
  if (! fArrangers.empty()) {
    vector<S_msrVarValAssoc>::const_iterator
      iBegin = fArrangers.begin(),
      iEnd   = fArrangers.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    emptyIdentification = false;
  }
    
  if (! fPoets.empty()) {
    vector<S_msrVarValAssoc>::const_iterator
      iBegin = fPoets.begin(),
      iEnd   = fPoets.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    emptyIdentification = false;
  }
    
  if (! fLyricists.empty()) {
    vector<S_msrVarValAssoc>::const_iterator
      iBegin = fLyricists.begin(),
      iEnd   = fLyricists.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    emptyIdentification = false;
  }
    
  if (fRights) {
    os << idtr << fRights;
    emptyIdentification = false;
  }
    
  if (!fSoftwares.empty()) {
    vector<S_msrVarValAssoc>::const_iterator
      iBegin = fSoftwares.begin(),
      iEnd   = fSoftwares.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    emptyIdentification = false;
  }
    
  if (fEncodingDate) {
    os << idtr << fEncodingDate;
    emptyIdentification = false;
  }

  if (emptyIdentification)
    os << idtr <<
      " " << "nothing specified" << endl;

// JMI  os << endl;
  
  idtr--;
}

//______________________________________________________________________________
S_msrScore msrScore::create (
  int inputLineNumber)
{
  msrScore* o =
    new msrScore (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrScore::msrScore (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  // create the identification
  fIdentification =
    msrIdentification::create (
      inputLineNumber);

  // create the page geometry
  fPageGeometry =
    msrPageGeometry::create (
      inputLineNumber);

  // measure repeats replicas should be browsed by default
  fInhibitMeasureRepeatReplicasBrowsing = false;

  // multiple rest measures should be browsed by default
  fInhibitMultipleRestMeasuresBrowsing = false;
}

msrScore::~msrScore() {}

S_msrScore msrScore::createScoreShallowClone ()
{
  if (gGeneralOptions->fTraceScore) {
    cerr << idtr <<
      "Creating a shallow clone of a score" <<
      endl;
  }

  S_msrScore
    clone =
      msrScore::create (
        fInputLineNumber);

  clone->fInhibitMeasureRepeatReplicasBrowsing =
    fInhibitMeasureRepeatReplicasBrowsing;
    
  clone->fInhibitMultipleRestMeasuresBrowsing =
    fInhibitMultipleRestMeasuresBrowsing;
    
  return clone;
}

void msrScore::addPartGroupToScore (S_msrPartGroup partGroup)
{
  /* JMI
  if (fScorePartGroupsMap.count (partGroupNumber)) {
    cerr << idtr <<
      "### Internal error: part group " << partGroupNumber <<
      " already exists in this score" << endl;

    return fScorePartGroupsMap [partGroupNumber];
  }
*/

  // register it in this score
  fPartGroupsList.push_back (partGroup);
}

/*
S_msrPartGroup msrScore::fetchScorePartGroup (
  int partGroupNumber)
{
  S_msrPartGroup result;
  
  if (fScorePartGroupsMap.count (partGroupNumber)) {
    result = fScorePartGroupsMap [partGroupNumber];
  }

  return result;
}
*/

void msrScore::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrScore::acceptIn()" <<
      endl;
      
  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrScore::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrScore::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrScore::acceptOut()" <<
      endl;

  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrScore::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrScore::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrScore::browseData()" <<
      endl;
    
  if (fIdentification) {
    // browse identification
    msrBrowser<msrIdentification> browser (v);
    browser.browse (*fIdentification);
  }

  if (fPageGeometry) {
    // browse page geometry
    msrBrowser<msrPageGeometry> browser (v);
    browser.browse (*fPageGeometry);
  }

  if (fCredit) {
    // browse page credit
    msrBrowser<msrCredit> browser (v);
    browser.browse (*fCredit);
  }

  for (
    list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin();
    i != fPartGroupsList.end();
    i++) {
    // browse the part group
    msrBrowser<msrPartGroup> browser (v);
    browser.browse (*(*i));
  } // for

  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% <== msrScore::browseData()" <<
      endl;
  }

ostream& operator<< (ostream& os, const S_msrScore& elt)
{
  elt->print (os);
  return os;
}

void msrScore::print (ostream& os)
{
  os <<
    "MSR Score" <<
    " (" <<
    singularOrPlural (
      fPartGroupsList.size(),
      "part group",
      "part groups") <<
    ")" <<
    endl << endl;

  idtr++;
  
  // print the identification if any
  if (fIdentification) {
    os <<
      idtr <<
      fIdentification;
  }
  
  // print the geometry if any
  if (fPageGeometry) {
    os <<
      idtr <<
      fPageGeometry;
  }

  // print the credit if any
  if (fCredit) {
    os <<
      idtr <<
      fCredit;
  }

  // print the part groups
  list<S_msrPartGroup>::const_iterator
    iBegin = fPartGroupsList.begin(),
    iEnd   = fPartGroupsList.end(),
    i      = iBegin;
  for ( ; ; ) {
    os << idtr << (*i);
    if (++i == iEnd) break;
    os << endl;
  } // for
  
  idtr--;
}

void msrScore::printStructure (ostream& os)
{
  os <<
    "MSR component" <<
    " (" <<
    singularOrPlural (
      fPartGroupsList.size(),
      "part group",
      "part groups") <<
    ")" <<
    ", parts and staves not shown" <<
    endl << endl;

  idtr++;
  
  if (fIdentification) {
    os << idtr << fIdentification;
  }
  
  if (fPageGeometry) {
    os << idtr << fPageGeometry;
  }
  
  if (fCredit) {
    os << idtr << fCredit;
  }
  
  for (
    list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin();
    i != fPartGroupsList.end();
    i++) {
    os << idtr;
    (*i)->
      printStructure (os);
  } // for
  
  idtr--;
}

//______________________________________________________________________________
S_msrMidi msrMidi::create (
  int    inputLineNumber,
  string midiTempoDuration,
  int    midiTempoPerSecond)
{
  msrMidi* o =
    new msrMidi (
      inputLineNumber,
      midiTempoDuration,
      midiTempoPerSecond);
  assert(o!=0);
  return o;
}

msrMidi::msrMidi (
  int    inputLineNumber,
  string midiTempoDuration,
  int    midiTempoPerSecond)
    : msrElement (inputLineNumber)
{
  fMidiTempoDuration = midiTempoDuration;
  fMidiTempoPerSecond = midiTempoPerSecond;
}

msrMidi::~msrMidi() {}

void msrMidi::acceptIn (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrMidi::acceptIn()" <<
      endl;
      
  if (visitor<S_msrMidi>*
    p =
      dynamic_cast<visitor<S_msrMidi>*> (v)) {
        S_msrMidi elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrMidi::visitStart()" <<
             endl;
        p->visitStart (elem);
  }
}

void msrMidi::acceptOut (basevisitor* v) {
  if (gMsrOptions->fTraceMsrVisitors)
    cerr << idtr <<
      "% ==> msrMidi::acceptOut()" <<
      endl;

  if (visitor<S_msrMidi>*
    p =
      dynamic_cast<visitor<S_msrMidi>*> (v)) {
        S_msrMidi elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors)
          cerr << idtr <<
            "% ==> Launching msrMidi::visitEnd()" <<
            endl;
        p->visitEnd (elem);
  }
}

void msrMidi::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrMidi& elt)
{
  elt->print (os);
  return os;
}

void msrMidi::print (ostream& os)
{
  os << "Midi" << endl;

  idtr++;

  const int fieldWidth = 18;

  os <<
    idtr <<
      setw(fieldWidth) <<
      "MidiTempoDuration" << " = " << fMidiTempoDuration <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "MidiTempoPerSecond" << " = " << fMidiTempoPerSecond <<
      endl <<
    endl;
      
  idtr--;
}


}
    
