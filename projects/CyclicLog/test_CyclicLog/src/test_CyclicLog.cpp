
#include "test_CyclicLogAppLayer.h"

int main(int, char *[])
{
    Test_CyclicLogAppLayer app;

    if (!app.run())
    {
        // log
        return -1;
    }

    return 0;
}
