
#include "Application.h"

int main(int argc, const char *argv[])
{
    der::Application app;
    app.init("derelict");
    app.run();
    return 0;
}
