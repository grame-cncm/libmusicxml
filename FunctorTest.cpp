#include <iostream>
#include <vector>
#include <algorithm>

struct add_x
{
  add_x(int x) : x(x) {}
  
  int operator() (int y) const { return x + y; }
  
  private:
    int x;
};

int main()
{
  std::vector<int> vec {2, 7, 5, 10};
  
  for (int i : vec)
  {
    std::cout << i << std::endl;
  }

  std::cout << "Add how much should be added to the elements? ";
  
  int n;
  std::cin >> n;
  
  std::transform(vec.begin(), vec.end(), vec.begin(), add_x(n));
  
  for (int i : vec)
  {
    std::cout << i << std::endl;
  }
}
