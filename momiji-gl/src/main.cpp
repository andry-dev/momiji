#include "Gui.h"

#include <asl/detect_features>

#ifdef ASL_WIN32
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif

    #include <windows.h>
#endif

int main()
{
    gui();
}

#ifdef ASL_WIN32
extern "C" int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    return main();
}
#endif
