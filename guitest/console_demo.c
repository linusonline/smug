#include <common.h>
#include <utils/string_console.h>
#include <utils/log.h>

int main()
{
    Log_init(StringConsole_new(128));
    return 0;
}
