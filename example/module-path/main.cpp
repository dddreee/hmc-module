#include <hmc_path_util.h>
#include <iostream>
#include <windows.h>
#include <string>
using namespace std;

int main(int argc, char *argv)
{
    cout << "[ test ]  ########hmc_path_util::join########  " << "\n";


    auto join_rsturn0 = hmc_path_util::join("/foo", "bar", "baz/asdf", "quux", "..");
    // returns "/foo/bar/baz/asdf"
    cout << "join_rsturn0 [\"/foo/bar/baz/asdf\"] -> "<< join_rsturn0 << "           [ " << boolalpha << (join_rsturn0 == "\\foo\\bar\\baz\\asdf") << " ] " << endl;



    auto resolve_rsturn0 = hmc_path_util::resolve("D:\\\\\\", "/foo", "bar", "baz/asdf", "quux", "..");
    // returns "/foo/bar/baz/asdf"
    cout << "resolve_rsturn0 [\"D:/foo/bar/baz/asdf\"] -> " << resolve_rsturn0 << "           [ " << boolalpha << (resolve_rsturn0 == "D:\\foo\\bar\\baz\\asdf") << " ] " << endl;

    cout << "[ endl ]  ########hmc_path_util::join########  " << "\n";
    
    return 0;
}
