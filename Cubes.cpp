#include <iostream>
using namespace std;

class Cubes
{
public:
 Cubes(int size=10): capacity(size)
 {   
   arr = new int[size];
   for (int i=0;i<capacity;i++)
     arr[i] = i*i*i;
 }

 int GetNthCube(int num) const
 {
    return(arr[num]);
 }
// ~Cubes();

private:
  int *arr;
  int capacity;
};
int main(void)
{
  Cubes a;

  cout << a.GetNthCube(5);

}


