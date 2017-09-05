#include <iostream>
#include <sstream>

#include <vector>
#include <list>
#include <map>

#include <algorithm>

#include <iomanip>      // setw, set::precision, ...


using namespace std;


class msrOptionsItem
{
public:

  msrOptionsItem (
    string optionsItemShortName,
    string optionsItemLongName,
    string optionsItemDescription)
    : fOptionsItemShortName (optionsItemShortName),
      fOptionsItemLongName (optionsItemLongName),
      fOptionsItemDescription (optionsItemDescription)
    {
      fOptionsItemSelected = 0;
    }
  
  string operator() () const
    { return fOptionsItemDescription; }

  void print (ostream& os) const
    {
      const int fieldWidth = 19;
      
      os <<
        setw(fieldWidth) <<
        "fOptionsItemShortName" << " : " << fOptionsItemShortName <<
        endl <<
        setw(fieldWidth) <<
        "fOptionsItemLongName" << " : " << fOptionsItemLongName <<
        endl <<
        setw(fieldWidth) <<
        "fOptionsItemDescription" << " : " << fOptionsItemDescription <<
        endl <<
        setw(fieldWidth) <<
        "fOptionsItemSelected" << " : " << fOptionsItemSelected <<
        endl;
    }

  private:
    string                fOptionsItemShortName;
    string                fOptionsItemLongName;
    string                fOptionsItemDescription;

    int                   fOptionsItemSelected;
};
typedef msrOptionsItem* S_msrOptionsItem;

ostream& operator<< (ostream& os, const msrOptionsItem& elt)
{
  elt.print (os);
  return os;
}

//_______________________________________________________________________________
class msrOptionsSubGroup
{
public:

  msrOptionsSubGroup (
    string optionsSubGroupShortName,
    string optionsSubGroupLongName,
    string optionsSubGroupDescription)
    : fOptionsSubGroupShortName (optionsSubGroupShortName),
      fOptionsSubGroupLongName (optionsSubGroupLongName),
      fOptionsSubGroupDescription (optionsSubGroupDescription)
    {
      fOptionsSubGroupSelected = 0;
    }
  
  string operator() () const
    { return fOptionsSubGroupDescription; }

  void appendOptionsItem (S_msrOptionsItem optionsItem)
    {
      fOptionsSubGroupItemsList.push_back (
        optionsItem);
    }
      
  void print (ostream& os) const
    {
      const int fieldWidth = 19;
      
      os <<
        setw(fieldWidth) <<
        "fOptionsSubGroupShortName" << " : " << fOptionsSubGroupShortName <<
        endl <<
        setw(fieldWidth) <<
        "fOptionsSubGroupLongName" << " : " << fOptionsSubGroupLongName <<
        endl <<
        setw(fieldWidth) <<
        "fOptionsSubGroupDescription" << " : " << fOptionsSubGroupDescription <<
        endl <<
        setw(fieldWidth) <<
        "fOptionsSubGroupSelected" << " : " << fOptionsSubGroupSelected <<
        endl;

      for (
        list<S_msrOptionsItem>::const_iterator
          i = fOptionsSubGroupItemsList.begin();
        i != fOptionsSubGroupItemsList.end();
        i++) {
        // print the element
        os << (*i);
      } // for
    }

  private:
    string                fOptionsSubGroupShortName;
    string                fOptionsSubGroupLongName;
    string                fOptionsSubGroupDescription;

    int                   fOptionsSubGroupSelected;

    list<S_msrOptionsItem>   fOptionsSubGroupItemsList;
};
typedef msrOptionsSubGroup* S_msrOptionsSubGroup;

ostream& operator<< (ostream& os, const msrOptionsSubGroup& elt)
{
  elt.print (os);
  return os;
}

//_______________________________________________________________________________
class msrOptionsGroup
{
public:

  msrOptionsGroup (
    string optionGroupShortName,
    string optionGroupLongName,
    string optionGroupDescription)
    : fOptionGroupShortName (optionGroupShortName),
      fOptionGroupLongName (optionGroupLongName),
      fOptionGroupDescription (optionGroupDescription)
    {
      fOptionGroupSelected = 0;
    }
  
  string operator() () const
    { return fOptionGroupDescription; }

  void appendOptionsSubGroup (S_msrOptionsSubGroup optionsSubGroup)
    {
      fOptionsGroupSubGroupsList.push_back (
        optionsSubGroup);
    }
      
  void print (ostream& os) const
    {
      const int fieldWidth = 19;
      
      os <<
        setw(fieldWidth) <<
        "fOptionGroupShortName" << " : " << fOptionGroupShortName <<
        endl <<
        setw(fieldWidth) <<
        "fOptionGroupLongName" << " : " << fOptionGroupLongName <<
        endl <<
        setw(fieldWidth) <<
        "fOptionGroupDescription" << " : " << fOptionGroupDescription <<
        endl <<
        setw(fieldWidth) <<
        "fOptionGroupSelected" << " : " << fOptionGroupSelected <<
        endl;

      for (
        list<S_msrOptionsSubGroup>::const_iterator
          i = fOptionsGroupSubGroupsList.begin();
        i != fOptionsGroupSubGroupsList.end();
        i++) {
        // print the element
        os << (*i);
      } // for
    }

  private:
    string                fOptionGroupShortName;
    string                fOptionGroupLongName;
    string                fOptionGroupDescription;

    int                   fOptionGroupSelected;

    list<S_msrOptionsSubGroup>
                          fOptionsGroupSubGroupsList;
};
typedef msrOptionsGroup* S_msrOptionsGroup;

ostream& operator<< (ostream& os, const msrOptionsGroup& elt)
{
  elt.print (os);
  return os;
}

//______________________________________________________________________________
void optionError (string errorMessage)
{
  cerr <<
    endl <<
    endl <<
    "### ERROR in the options:" <<
    endl <<
    errorMessage <<
    endl <<
    endl;
    
  exit(99);
}

//_______________________________________________________________________________
// a private variable
map<string, string> pOptionShortNames;

void checkOptionUniqueness (
  string optionLongName, string optionShortName)
{
  if (optionShortName == optionLongName) {
    stringstream s;

    s <<
      "option long name '" << optionLongName << "'" <<
      " is also used as short name";
      
    optionError (s.str());
  }
  
  for (
    map<string, string>::iterator i = pOptionShortNames.begin();
    i != pOptionShortNames.end();
    i++) {
      
    // is optionLongName in the options names map?
    if ((*i).first == optionLongName) {
      stringstream s;
  
      s <<
        "option long name '" << optionLongName << "'" <<
        " is specified more that once";
        
      optionError (s.str());
    }

    // is optionShortName in the options names map?
    if ((*i).second == optionShortName) {
      if (optionShortName.size ()) {
        stringstream s;
    
        s <<
          "option short name '" << optionShortName << "'" <<
          " for option long name '" << optionLongName << "'" <<
          " is specified more that once";
          
        optionError (s.str());
      }
    }
  } // for

  // everything OK, register the option names
  pOptionShortNames [optionLongName] = optionShortName;
}

//_______________________________________________________________________________
void analyzeOptions (
  int            argc,
  char*          argv[])
{
  int   n = 0;

  S_msrOptionsGroup optionsGroup =
    new msrOptionsGroup (
    "hog", "helpOptionsGroup", " help for OptionsGroup");

  S_msrOptionsSubGroup optionsSubGroup =
    new msrOptionsSubGroup (
    "hosg", "helpOptionsSubGroup", " help for OptionsSubGroup");

  optionsGroup->appendOptionsSubGroup (optionsSubGroup);

  S_msrOptionsItem optionsItem =
    new msrOptionsItem (
      "1short", "1long", "descr1");
  optionsGroup->appendOptionsItem (optionsItem);
      
  S_msrOptionsItem optionsItem =
    new msrOptionsItem (
      "2short", "1long", "descr2")
  optionsGroup->appendOptionsItem (optionsItem);

  while (true) { 
    if (argv [n] == 0)
      break;

    string currentElement = string (argv [n]);
    
    // print current element
    cout <<
      n << ":" << currentElement <<
      endl;

    // handle current element
    if (currentElement [0] == '-') {
      // this is an option
      string elementTrailer =
        currentElement.substr (1, string::npos);

      /* JMI
      cout <<
        "elementTrailer '" << elementTrailer << "' is preceded by a dash" <<
        endl;
      */

      if (elementTrailer.size ()) {
        if (elementTrailer [0] == '-') {
          // it is a double-dashed option
          string currentDoubleDashedOption =
            elementTrailer.substr (1, string::npos);
          
          cout <<
            "'" << currentDoubleDashedOption << "' is a double-dashed option" <<
            endl;
        }
        else {
          // it is a single-dashed option
          string currentSingleDashedOption =
            elementTrailer; //.substr (1, string::npos);
          
          cout <<
            "'" << currentSingleDashedOption << "' is a single-dashed option" <<
            endl;
        }
      }
      
      else {
        cout <<
          "'-' is the minimal single-dashed option" <<
          endl;
      }
    }

    // next please
    n++;
  } // while
}

int main (int argc, char *argv[])
{
  vector<msrOptionItem> vec {
    msrOptionItem ("1short", "1long", "descr1"),
    msrOptionItem ("2short", "1long", "descr2")
  };

  int counter = 0;
  cout <<
    "The contents of 'vec' is:" <<
    endl << endl;
  for (msrOptionItem i : vec)
  {
    cout <<
      "Element " << counter << ":" <<
      endl <<
      i <<
      endl;
    counter++;
  }

/*
struct option
{
  const char *name;
  // has_arg can't be an enum because some compilers complain about
  // type mismatches in all the code that assumes it is an int.
  int has_arg;
  int *flag;
  int val;
};
*/

/*
  vector<struct option> myLongOptions {
    option ("1short", no_argument, &vec [0].fOptionSelected, 1),
    option ("1long", no_argument, &vec [0].fOptionSelected, 1),
    
    option ("2short", required_argument, &vec [1].fOptionSelected, 1),
    option ("2long", required_argument, &vec [1].fOptionSelected, 1),
    
    option (0, 0, 0, 0) // option trailer
  };
*/


/*
  cout << "Which element should be printed? ";
  
  int n;
  cin >> n;

  cout << endl;
  
  if (n < vec.size ())
    cout <<
      "Element " << n << " constains:" <<
      endl <<
      endl <<
      vec [n] <<
      endl;
  else
    cout <<
      "Sorry, only elements from 0 to " << vec.size () - 1 << " exist" <<
      endl;
*/

  analyzeOptions (
    argc, argv);

}
