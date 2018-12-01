#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;

int main() {
    int numLines = 2000;
    string outputs[4] = {"0 0 0", "0 1 1", "1 0 1", "1 1 0"}; 

    ofstream s;
    string filename = "xor.txt";
    s.open(filename);
    if(s.is_open()) {
        for (int i = 0; i < numLines; ++i) {
            s << outputs[rand() % 4] << "\n";    
        }

        s.close();
    }
    else {
        cout << "pinga" << endl;
    }

    return 0;
}

// 0 0 0
// 0 1 1
// 1 0 1
// 1 1 0