#include <iostream>
#include <cxxtools/file.h>
#include <cxxtools/directory.h>
using namespace std;
using namespace cxxtools;

/**
 * Directory iterator, dirname should end with '/'
 */
void dir_it(string dirname)
{
    try
    {
        Directory dir(dirname);
        DirectoryIterator it;
        bool skip_hiden = true;

        for (it = dir.begin(skip_hiden); it != dir.end(); ++it)
        {
            string path = dir.path() + *it;
            FileInfo fileinfo(path);

            if (fileinfo.isDirectory())
            {
                cout << path << "*" << endl;
                dir_it(path + "/");
            }
            else
            {
                File file(fileinfo);
                cout << path << " " << file.size() << "|"
                << file.baseName() << "." << file.extension() << endl;
            }
        }
    }
    catch (const exception& e)
    {
        cerr << "Err: " << e.what() << endl;
    }
}

int main()
{
    dir_it("/usr/games/");
    return 0;
}
