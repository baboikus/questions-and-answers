#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
    cout << argv[0] << " " << argv[1] << endl;

    ofstream freqFile(argv[2]);
    freqFile << "123" << endl;
    freqFile.close();

    return 0;
}
