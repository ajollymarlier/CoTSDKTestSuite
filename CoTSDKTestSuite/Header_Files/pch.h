// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#ifndef PCH_H
#define PCH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <vector>
#include <direct.h>
#include "shlobj.h"
#include <string>

#include "CoTSdk.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <direct.h>
#define GetCurrentDir _getcwd
//No Non-Windows Solution

using namespace std;
using namespace CoTSdk;
/*using namespace NUser;
using namespace NCompany;*/

#endif //PCH_H
