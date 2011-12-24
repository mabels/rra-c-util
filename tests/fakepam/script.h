/*
 * PAM interaction script API.
 *
 * Provides an interface that loads a PAM interaction script from a file and
 * runs through that script, calling the internal PAM module functions and
 * checking their results.  This allows automation of PAM testing through
 * external data files instead of coding everything in C.
 *
 * Written by Russ Allbery <rra@stanford.edu>
 * Copyright 2011
 *     The Board of Trustees of the Leland Stanford Junior University
 *
 * See LICENSE for licensing terms.
 */

#ifndef TESTS_MODULE_SCRIPT_H
#define TESTS_MODULE_SCRIPT_H 1

#include <portable/pam.h>

#include <tests/tap/basic.h>

/* A test callback called after PAM functions are run but before pam_end. */
struct script_config;
typedef void (*script_callback)(pam_handle_t *, const struct script_config *,
                                void *);

/* Configuration for the PAM interaction script API. */
struct script_config {
    const char *user;           /* Username to pass into pam_start (%u). */
    const char *password;       /* Substituted for %p in prompts. */
    const char *newpass;        /* Substituted for %n in prompts. */
    const char *extra[10];      /* Substituted for %0-%9 in logging. */
    script_callback callback;   /* Called after PAM, before pam_end. */
    void *data;                 /* Passed to the callback function. */
};

BEGIN_DECLS

/*
 * Given the file name of an interaction script (which may be a full path or
 * relative to SOURCE or BUILD) and configuration containing other parameters
 * such as the user, run that script, reporting the results via the TAP
 * format.
 */
void run_script(const char *file, const struct script_config *)
    __attribute__((__nonnull__));

/*
 * The same as run_script, but run every script found in the given directory,
 * skipping file names that contain characters other than alphanumerics and -.
 */
void run_script_dir(const char *dir, const struct script_config *)
    __attribute__((__nonnull__));

END_DECLS

#endif /* !TESTS_MODULE_SCRIPT_H */
