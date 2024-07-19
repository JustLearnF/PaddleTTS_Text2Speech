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
        cout << "���ļ�ʧ��" << endl;
    }
    int buf;
    while (ifile >> buf)
    {
        phoneids.push_back(buf);
    }
    ifile.close();
    return phoneids;
}