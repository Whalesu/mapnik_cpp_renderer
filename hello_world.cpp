#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    vector<string> msg(6);
    msg = {"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};
    for (auto word = msg.begin(); word != msg.end(); word++)
    {
        cout << *word << " ";
        cout << *word->c_str() << endl;
    };
    cout << endl;
}
