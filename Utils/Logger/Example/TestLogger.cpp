#include "../inc/Logger.h"

void testPrint()
{
    printLog_E("print log with message in line here with char: %s", "Test Char");
}

int main() {
    testPrint();

    printLog_I("print log with message in line here with %g and double: %X and int: %d", 0.000001, 10, 10);
    printLog_A("This is an alert message: %s", "Alert!");
    printLog_D("This is a debug message");
    return 0;
}

/*after run file TestLogger*/
// source code will print the log file in syslog (link path: '/var/log/syslog' with permission sudo)
// format the log file is printed 
// -> eCall[21074]: [ERR][TestLogger.cpp:testPrint():5] print log with message in line here with char: Test Char
// -> eCall[21074]: [INFO][TestLogger.cpp:main():11] print log with message in line here with 1e-06 and double: A and int: 10