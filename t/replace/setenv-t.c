/* $Id$ */
/* setenv test suite. */

#include "config.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "librutil.h"

int test_setenv(const char *name, const char *value, int overwrite);

static void
ok(int n, int success)
{
    printf("%sok %d\n", success ? "" : "not ", n);
}

static const char test_var[] = "SETENV_TEST";
static const char test_value1[] = "Do not taunt Happy Fun Ball.";
static const char test_value2[] = "Do not use Happy Fun Ball on concrete.";

int
main(void)
{
    char *value;
    int status;

    if (getenv(test_var)) die("%s already in the environment!", test_var);

    puts("7");

    status = test_setenv(test_var, test_value1, 0);
    ok(1, (status == 0) && !strcmp(getenv(test_var), test_value1));
    status = test_setenv(test_var, test_value2, 0);
    ok(2, (status == 0) && !strcmp(getenv(test_var), test_value1));
    status = test_setenv(test_var, test_value2, 1);
    ok(3, (status == 0) && !strcmp(getenv(test_var), test_value2));
    status = test_setenv(test_var, "", 1);
    value = getenv(test_var);
    ok(4, (status == 0) && value && *value == 0);

    /* We're run by a shell script wrapper that sets resource limits such
       that we can allocate one string of this size but not two. */
    value = xmalloc(30 * 1024);
    memset(value, 'A', 30 * 1024 - 1);
    value[30 * 1024 - 1] = 0;
    status = test_setenv(test_var, value, 0);
    ok(5, (status == 0) && !strcmp(getenv(test_var), ""));
    status = test_setenv(test_var, value, 1);
    ok(6, (status == -1) && (errno == ENOMEM));
    ok(7, !strcmp(getenv(test_var), ""));

    return 0;
}