#include <common.h>
#include <utils/stdout_console.h>
#include <utils/log.h>

int main()
{
    Console* console = StdoutConsole_new();
    Log_init(console);
    Log_setLevel(LOG_DEBUG);
    DEBUG("This is a DEBUG level message. It only shows up when the level is set to LOG_DEBUG.");
    ERROR("This is an ERROR level message. It shows up for any debug level except LOG_NONE.");
    WARNING("This is a WARNING level message.");
    NOTIFY("This is a NOTIFICATION level message.");
    Log_indent();
    DEBUG("This is an indented log line.");
    Log_dedent();
    Log_setIndentationString("----");
    Log_indent();
    DEBUG("This is a log line indented with a different indentation string.");
    Log_dedent();

    Log_terminate();
    StdoutConsole_delete(console);
    return 0;
}
