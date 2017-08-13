#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct option
{
  option (
    string optionShortName,
    string optionLongName,
    string optionDescription)
    : fOptionShortName (optionShortName),
      fOptionLongName (optionLongName),
      fOptionDescription (optionDescription)
    {}
  
  string operator() () const
    { return fOptionDescription; }

  void print (ostream& os) const
    {
      os <<
        "fOptionShortName:" << fOptionShortName << endl <<
        "fOptionLongName:" << fOptionLongName << endl <<
        "fOptionDescription:" << fOptionDescription << endl;
    }

  private:
    string fOptionShortName;
    string fOptionLongName;
    string fOptionDescription;
};

ostream& operator<< (ostream& os, const option& elt)
{
  elt.print (os);
  return os;
}

int main()
{
  vector<option> vec {
    option ("1short", "1long", "descr1"),
    option ("2short", "1long", "descr2")
  };

  int counter = 0;
  cout <<
    "The contents of 'vec' is:" << endl << endl;
  for (option i : vec)
  {
    cout <<
      "Element " << counter << ":" << endl <<
      i << endl;
    counter++;
  }

  cout << "Which element should be printed? ";
  
  int n;
  cin >> n;

  cout << endl;
  
  if (n < vec.size ())
    cout <<
      "Element " << n << " constains:" << endl <<
      endl <<
      vec [n] << endl;
  else
    cout <<
      "Sorry, only elements from 0 to " << vec.size () - 1 << " exist" << endl;
}
