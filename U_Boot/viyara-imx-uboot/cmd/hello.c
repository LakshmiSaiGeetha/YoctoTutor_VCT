#include <common.h>
#include <command.h>

static int do_hello(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
    printf("Hello World");
    return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
    hello,
    1,
    1,
    do_hello,
    "Print Hello World",
    ""
);
