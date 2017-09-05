#include <iostream>
#include <sstream>

#include <vector>
#include <list>
#include <map>

#include <algorithm>

#include <iomanip>      // setw, set::precision, ...


using namespace std;

#define EXP

//______________________________________________________________________________
class EXP msrOptionsElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

  protected:

    msrOptionsElement (
      string optionsElementShortName,
      string optionsElementLongName,
      string optionsElementDescription);

    virtual ~msrOptionsElement();

  public:

    // set and get
    // ------------------------------------------------------

    string                getOptionsElementShortName () const
                              { return fOptionsElementShortName; }

    string                getOptionsElementLongName () const
                              { return fOptionsElementLongName; }

    string                getOptionsElementDescription () const
                              { return fOptionsElementDescription; }

    // services
    // ------------------------------------------------------

    string                operator() () const
                              { return fOptionsElementDescription; }
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) = 0;
    
  protected:
     
    string                fOptionsElementShortName;
    string                fOptionsElementLongName;
    string                fOptionsElementDescription;
};
//typedef SMARTP<msrOptionsElement> S_msrOptionsElement;
typedef msrOptionsElement* S_msrOptionsElement;
EXP ostream& operator<< (ostream& os, const S_msrOptionsElement& elt);


msrOptionsElement::msrOptionsElement (
  string optionsElementShortName,
  string optionsElementLongName,
  string optionsElementDescription)
{
  fOptionsElementShortName   = optionsElementShortName;  
  fOptionsElementLongName    = optionsElementLongName;  
  fOptionsElementDescription = optionsElementDescription;  
}

msrOptionsElement::~msrOptionsElement()
{}

ostream& operator<< (ostream& os, const S_msrOptionsElement& elt)
{
  elt->print (os);
  return os;
}

void msrOptionsElement::print (ostream& os)
{
  os << "??? msrOptionsElement ???" << endl;
}

//______________________________________________________________________________
template <typename T> class EXP msrOptionsItem : public msrOptionsElement
{
  public:
  
    msrOptionsItem (
      string optionsItemShortName,
      string optionsItemLongName,
      string optionsItemDescription,
      T&     optionsItemVariable)
      : msrOptionsElement (
          optionsItemShortName,
          optionsItemLongName,
          optionsItemDescription),
        fOptionsItemVariable (optionsItemVariable)
      {
        fOptionsItemSelected = 0;
      }
    
    bool                  optionsItemSelected () const
                              { return fOptionsItemSelected; }
  
    void                  print (ostream& os) const
      {
        const int fieldWidth = 19;
        
        os <<
          setw(fieldWidth) <<
          "fOptionsElementShortName" << " : " << fOptionsElementShortName <<
          endl <<
          setw(fieldWidth) <<
          "fOptionsElementLongName" << " : " << fOptionsElementLongName <<
          endl <<
          setw(fieldWidth) <<
          "fOptionsElementDescription" << " : " << fOptionsElementDescription <<
          endl <<
          setw(fieldWidth) <<
          "fOptionsItemSelected" << " : " << fOptionsItemSelected <<
          endl <<
          setw(fieldWidth) <<
          "fOptionsItemVariable" << " : " << fOptionsItemVariable <<
          endl;
      }

  private:
  
    T&                    fOptionsItemVariable;
    
    int                   fOptionsItemSelected;
};
typedef msrOptionsItem<int>* S_msrOptionsIntItem;
typedef msrOptionsItem<string>* S_msrOptionsStringItem;

ostream& operator<< (ostream& os, const msrOptionsItem<int>& elt)
{
  elt.print (os);
  return os;
}

ostream& operator<< (ostream& os, const msrOptionsItem<string>& elt)
{
  elt.print (os);
  return os;
}

//_______________________________________________________________________________
class EXP msrOptionsSubGroup : public msrOptionsElement
{
  public:
  
    msrOptionsSubGroup (
      string optionsSubGroupShortName,
      string optionsSubGroupLongName,
      string optionsSubGroupDescription)
      : msrOptionsElement (
          optionsItemShortName,
          optionsItemLongName,
          optionsItemDescription)
      {}
    
    void                  appendOptionsIntItem (
                            S_msrOptionsIntItem optionsIntItem)
      {
        fOptionsSubGroupItemsList.push_back (
          optionsIntItem);
      }

    void                  appendOptionsStringItem (
                            S_msrOptionsStringItem optionsStringItem)
      {
        fOptionsSubGroupItemsList.push_back (
          optionsStringItem);
      }
        
    void                  print (ostream& os) const
      {
        const int fieldWidth = 19;
        
        os <<
          setw(fieldWidth) <<
          "fOptionsElementShortName" << " : " << fOptionsElementShortName <<
          endl <<
          setw(fieldWidth) <<
          "fOptionsElementLongName" << " : " << fOptionsElementLongName <<
          endl <<
          setw(fieldWidth) <<
          "fOptionsElementDescription" << " : " << fOptionsElementDescription <<
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
  
    list<S_msrOptionsItem>
                          fOptionsSubGroupItemsList;
};
typedef msrOptionsSubGroup* S_msrOptionsSubGroup;

ostream& operator<< (ostream& os, const msrOptionsSubGroup& elt)
{
  elt.print (os);
  return os;
}

//_______________________________________________________________________________
class EXP msrOptionsGroup : public msrOptionsElement
{
  public:
  
    msrOptionsGroup (
      string optionGroupShortName,
      string optionGroupLongName,
      string optionGroupDescription)
      : msrOptionsElement (
          optionsItemShortName,
          optionsItemLongName,
          optionsItemDescription)
      {}
      
    void                  appendOptionsSubGroup (
                            S_msrOptionsSubGroup optionsSubGroup)
      {
        fOptionsGroupSubGroupsList.push_back (
          optionsSubGroup);
      }
        
    void                  print (ostream& os) const
      {
        const int fieldWidth = 19;
        
        os <<
          setw(fieldWidth) <<
          "fOptionsElementShortName" << " : " << fOptionsElementShortName <<
          endl <<
          setw(fieldWidth) <<
          "fOptionsElementLongName" << " : " << fOptionsElementLongName <<
          endl <<
          setw(fieldWidth) <<
          "fOptionsElementDescription" << " : " << fOptionsElementDescription <<
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

  optionsGroup->
    appendOptionsSubGroup (optionsSubGroup);

  int intVariable;
  S_msrOptionsItem<int> optionsItem1 =
    new msrOptionsItem (
      "is", "il", "intVariable", intVariable);
  optionsSubGroup->
    appendOptionsItem (optionsItem1);
      
  string stringVariable;
  S_msrOptionsItem<string> optionsItem2 =
    new msrOptionsItem (
      "ss", "sl", "stringVariable", stringVariable);
  optionsSubGroup->
    appendOptionsItem (optionsItem2);

  cerr <<
    "optionsGroup:" << endl <<
    optionsGroup <<
    endl;
    
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
/*
  vector<msrOptionsItem> vec {
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
