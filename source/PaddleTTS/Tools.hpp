#include <iostream>
#include <vector>
#include <fstream>
using namespace std;
#define fs_in "./cache/phoneids"

vector<int> readData2Phoneids()
{
    vector<int> phoneids;
    ifstream ifile;
    ifile.open(fs_in);
    if (!ifile.is_open())
    {
        cout << "打开文件失败" << endl;
    }
    int buf;
    while (ifile >> buf)
    {
        phoneids.push_back(buf);
    }
    ifile.close();
    return phoneids;
}