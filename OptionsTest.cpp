#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>      // setw, set::precision, ...

#include <getopt.h>

using namespace std;

struct myOption
{
  myOption (
    string optionShortName,
    string optionLongName,
    string optionDescription)
    : fOptionShortName (optionShortName),
      fOptionLongName (optionLongName),
      fOptionDescription (optionDescription)
    {
      fOptionSelected = 0;
    }
  
  string operator() () const
    { return fOptionDescription; }

  void print (ostream& os) const
    {
      const int fieldWidth = 19;
      
      os <<
        setw(fieldWidth) <<
        "fOptionShortName" << " : " << fOptionShortName <<
        endl <<
        setw(fieldWidth) <<
        "fOptionLongName" << " : " << fOptionLongName <<
        endl <<
        setw(fieldWidth) <<
        "fOptionDescription" << " : " << fOptionDescription <<
        endl <<
        setw(fieldWidth) <<
        "fOptionSelected" << " : " << fOptionSelected <<
        endl;
    }

  private:
    string fOptionShortName;
    string fOptionLongName;
    string fOptionDescription;

    int    fOptionSelected;
};

ostream& operator<< (ostream& os, const myOption& elt)
{
  elt.print (os);
  return os;
}

//_______________________________________________________________________________
void analyzeOptions (
  int            argc,
  char*          argv[])
{
}

int main (int argc, char *argv[])
{
  vector<myOption> vec {
    myOption ("1short", "1long", "descr1"),
    myOption ("2short", "1long", "descr2")
  };

  int counter = 0;
  cout <<
    "The contents of 'vec' is:" <<
    endl << endl;
  for (myOption i : vec)
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

  vector<struct option> myLongOptions {
    option ("1short", no_argument, &vec [0].fOptionSelected, 1),
    option ("1long", no_argument, &vec [0].fOptionSelected, 1),
    
    option ("2short", required_argument, &vec [1].fOptionSelected, 1),
    option ("2long", required_argument, &vec [1].fOptionSelected, 1),
    
    option (0, 0, 0, 0) // option trailer
  };



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
