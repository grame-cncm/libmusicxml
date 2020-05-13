/*
OAH basics:
  - OAH (Options And Help) is supposed to be pronouced something close to "whaaaah!"
    The intonation is left to the speaker, though...
    And as the saying goes: "OAH? oahy not!"

  - options handling is organized as a hierarchical, instrospective set of classes.
    Options and their corresponding help are grouped in a single object.

  - oahElement is the super-class of all options types, including groups and subgroups.
    It contains a short name and a long name, as well as a decription.
    Short and long names can be used and mixed at will in the command line,
    as well as '-' and '--'.
    The short name is mandatory, but the long name may be empty.

  - prefixes such '-t=' and -help=' allow for a contracted form of options.
    For example, -t=meas,notes is short for '-t-meas, -tnotes'.
    An oahPrefix contains the prefix name, the ersatz by which to replace it,
    and a description.

  - an oahHandler contains oahGroup's, each handled in a pair or .h/.cpp files,
    such as msrOah.h and msrOah.cpp, and a list of options prefixes.

  - an oahGroup contains oahSubGroup's and an upLink to the containing oahHandler.

  - an oahSubGroup contains oahAtom's and an upLink to the containing oahGroup.

  - each oahAtom is an atomic option to the executable and its corresponding help,
    and an upLink to the containing oahSubGroup.

Features:
  - partial help to be obtained, i.e. help about any group, subgroup or atom,
    showing the path in the hierarchy down to the corresponding option.

  - there are various subclasses of oahAtom such as oahIntegerAtom, oahBooleanAtom
    and oahRationalAtom to control options values of common types.

  - oahThreeBooleansAtom, for example, allows for three boolean settings
    to be controlled at once with a single option.

  - oahValuedAtom describes options for which a value is supplied in the command line.

  - a class such as optionsLpsrPitchesLanguageOption is used
    to supply a string value to be converted into an internal enumerated type.

  - oahCombinedBooleansAtom contains a list of boolean atoms to manipulate several such atoms as a single one,
    see the 'cubase' combined booleans atom in mxmlTreeOah.cpp.

  - oahMultiplexBooleansAtom contains a list of boolean atoms sharing a common prefix to display such atoms in a compact manner,
    see the 'cubase' combined booleans atom in mxmlTreeOah.cpp.

  - storing options and the corresponding help in oahGroup's makes it easy to re-use them.
    For example, xml2ly and xml2lbr have their three first passes in common,
    (up to obtaining the MSR description of the score), as well as the corresponding options and help.

Handling:
  - each optionOption must have unique short and long names, for consistency.

  - an executable main() calls applyOptionsAndArgumentsFromArgcAndArgv(), in which:
    - handleOptionName() handles the option names
    - handleOptionValueOrArgument() handle the values that may follow an atom name
      and the arguments to the executable.

  - contracted forms are expanded in handleOptionName() before the resulting,
    uncontracted options are handled.

  - handleOptionName() fetches the oahElement corresponding to the name from the map,
    determines the type of the latter,
    and delegates the handling to the corresponding object.

  - handleOptionValueOrArgument() associatiates the value
    to the (preceding) fPendingValuedAtom if not null,
    or appends it fHandlerArgumentsVector to otherwise.

  - the printOptionsSummary() methods are used when there are errors in the options used.

  - the printHelp() methods perform the actual help print work

  - options deciphering it done by the handleAtom() methods defined:
      - in oahBasicTypes.h/.cpp for the predefined ones;
      - in the various options groups for those specific to the latter.

  - the value following the option name, if any, is taken care of
    by the handle*AtomValue() methods, using fPendingValuedAtom
    to hold the valuedAtom until the corresponding value is found.
*/

