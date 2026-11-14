#include <iostream>
#include <GSIO/GSIO.h>

int main()
{
    std::cout << "Game Stand I/O Version " GSIO_Version_String " // Copyright Game Stand I/O - All rights reserved." << std::endl;

    GSIO gsio;
    GSIO_response init = GSIO_Init(&gsio);
    if (!init.success)
    {
        std::cout << init.message << std::endl;
        return 1;
    }

    GSIO_response application = GSIO_Application(&gsio);
    std::cout << application.message << std::endl;
    if (!application.success) return 1;

    GSIO_response shutdown = GSIO_Shutdown(&gsio);
    if (!shutdown.success)
    {
        std::cout << shutdown.message << std::endl;
        return 1;
    }

    return 0;
}