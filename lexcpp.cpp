#include <iostream>
#include <string>
#include <set>
using namespace std;

int main()
{
       string b;
       int n;
       cin >> n >> b;
       set <string> s;
       for (int z = 1; z <= n; z++)
       {     
             int c = z;
             string v = "";
             while (c > 0) { v = char(c % 10 + '0') + v; c = c / 10; }
             s.insert(v);
       }
       cout << distance(s.begin(),s.find(b))+1;
       return 0;
}
