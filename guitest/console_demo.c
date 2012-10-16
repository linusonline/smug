#include <common.h>
#include <utils/stdout_console.h>
#include <utils/log.h>

int main()
{
    Console* console = StdoutConsole_new();
    Log_init(console);
    ERROR("This is an ERROR message.");
    WARNING("This is a WARNING message.");
    Log_indent();
    LOG(LOG_DEFAULT, "This is an indented log line.");
    Log_dedent();
    Log_setIndentationString("----");
    Log_indent();
    LOG(LOG_DEFAULT, "This is a log line indented with a different indentation string.");
    Log_dedent();

    Log_terminate();
    StdoutConsole_delete(console);
    return 0;
}
