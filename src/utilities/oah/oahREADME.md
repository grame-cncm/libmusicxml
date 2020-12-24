/*
OAH basics:
  - OAH (Options And Help) is supposed to be pronounced something close to "whaaaah!"
    The intonation is left to the speaker, though...
    And as the saying goes: "OAH? oahy not!"

  - options handling is organized as a hierarchical, instrospective set of classes.
    An options and its corresponding help are grouped in a single object.

  - the options can be supplied thru:
      - the command line, in argv.
        This allows for mixed options and arguments in any order, à la GNU;
      - the API function such as musicxmlfile2lilypond(), in an options vector.

  - oahElement is the super-class EXP of all options types, including groups and subgroups.
    It contains a short name and a long name, as well as a decription.
    Short and long names can be used and mixed at will in the command line
    and in option vectors (API),
    as well as '-' and '--'.
    The short name is mandatory, but the long name may be empty
    if the short name is explicit enough.

  - prefixes such '-t=' and -help=' allow for a contracted form of options.
    For example, -t=meas,notes is short for '-t-meas, -tnotes'.
    A oahPrefix contains the prefix name, the ersatz by which to replace it,
    and a description.

  - a oahHandler contains a list of oahGroup's, each handled
    in a pair or .h/.cpp files such as msrOah.h and msrOahGroup.cpp,
    and a list of options prefixes.

  - a oahGroup contains a list of oahSubGroup's
    and an upLink to the containing oahHandler.

  - a oahSubGroup contains a list of oahAtom's
    and an upLink to the containing oahGroup.

  - each oahAtom contains an atomic option and the corresponding help,
    and an upLink to the containing oahSubGroup.

Features:
  - partial help can be obtained, i.e. help about any group, subgroup or atom,
    showing the path in the hierarchy down to the corresponding option.

  - there are various subclasses of oahAtom such as oahIntegerAtom, oahBooleanAtom
    and oahRationalAtom, to control options values of common types.

  - oahThreeBooleansAtom, for example, allows for three boolean settings
    to be controlled at once with a single option.

  - oahAtomWithValue describes options for which a value is supplied
    in the command line or in option vectors (API).

  - a class EXP such as lpsrPitchesLanguageAtom is used
    to supply a string value to be converted into an internal enumerated type.

  - a oahCombinedBooleansAtom contains a list of boolean atoms
    to manipulate several such atoms as a single one,
    see the 'cubase' combined booleans atom in mxmlTree2msrOah.cpp.

  - oahMultiplexBooleansAtom contains a list of boolean atoms
    sharing a common prefix to display such atoms in a compact manner,
    see the 'ignore-redundant-clefs' multiplex booleans atom in mxmlTree2msrOah.cpp.

  - storing options and the corresponding help in oahGroup's makes it easy to re-use them.
    For example, xml2ly and xml2lbr have their three first passes in common,
    (up to obtaining the MSR description of the score),
    as well as the corresponding options and help.

  - oahAtomsCollection.h/.cpp contains a bunch of general purpose options
    such as oahContactAtom, oahFloatAtom and oahLengthAtom.

  - a regular handler (used by default unless the '-insider' option is used),
    presents the options and help grouped by subject, such as voices and tuplets.
    It uses an insider handler, which groups them by internal representation
    and conversion pass.
    This is how options groups are re-used for various
    translators such as xml2ly, xml2brl and xml2xml.

Handling:
  - each option short name and non-empty long name must be unique in a given handler,
    to avoid ambiguities.

  - an executable main() calls applyOptionsAndArgumentsFromArgcAndArgv(), in which:
    - handleOptionName() handles the option names
    - handleOptionValueOrArgument() handle the values that may follow an atom name
      and the arguments to the executable.

  - contracted forms are expanded in handleOptionName() before the resulting,
    uncontracted options are handled.

  - options handling works in two passes:
      - the first one creates a list of 'elementUse' instances from
        argc/argv or an options vector;
      - the second one traverses this list to apply the options that are used.

  - the options are applied the the applyElement(), applyAtomWithValue() And
    applyAtomWithValueDefaultValue() methods.

  - handleOptionValueOrArgument() associatiates the value
    to the (preceding) fPendingAtomWithValue if not null,
    or appends it fHandlerArgumentsVector to otherwise.

  - fPendingArgvAtomWithValue is used in argv contents handling
    to associate an option name with it value, which is the next element in argv.
*/

