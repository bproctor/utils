
/*
 *  utils.h
 *  Copyright (c) 2002-2012 Brad Proctor. <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifndef  TRUE
#define  TRUE     1
#endif

#ifndef  FALSE
#define  FALSE    0
#endif

void               warning (const char *, ...);
void               error (const char *, ...);
void               terror (const char *, ...);
long               getnum_l (const char *);
long long          getnum_ll (const char *);
unsigned long      getnum_ul (const char *);
unsigned long long getnum_ull (const char *);
float              getnum_f (const char *);
double             getnum_d (const char *);
long double        getnum_ld (const char *);
long long          getnum_hex (const char *);
long long          getnum_octal (const char *);
int                yesno (int);

int                command_basename (int, char **);
int                command_cal (int, char **);
int                command_cat (int, char **);
int                command_chgrp (int, char **);
int                command_chmod (int, char **);
int                command_chown (int, char **);
int                command_chroot (int, char **);
int                command_cksum (int, char **);
int                command_cmp (int, char **);
int                command_colon (int, char **);
int                command_comm (int, char **);
int                command_cut (int, char **);
int                command_date (int, char **);
int                command_dirname (int, char **);
int                command_domainname (int, char **);
int                command_echo (int, char **);
int                command_expand (int, char **);
int                command_false (int, char **);
int                command_fold (int, char **);
int                command_head (int, char **);
int                command_hostname (int, char **);
int                command_kill (int, char **);
int                command_link (int, char **);
int                command_ln (int, char **);
int                command_logger (int, char **);
int                command_logname (int, char **);
int                command_ls (int, char **);
int                command_md5sum (int, char **);
int                command_mesg (int, char **);
int                command_mkdir (int, char **);
int                command_mkfifo (int, char **);
int                command_mknod (int, char **);
int                command_more (int, char **);
int                command_newgrp (int, char **);
int                command_nl (int, char **);
int                command_no (int, char **);
int                command_od (int, char **);
int                command_paste (int, char **);
int                command_prezip (int, char **);
int                command_printf (int, char **);
int                command_pwd (int, char **);
int                command_rm (int, char **);
int                command_rmdir (int, char **);
int                command_rot13 (int, char **);
int                command_seq (int, char **);
int                command_sleep (int, char **);
int                command_sort (int, char **);
int                command_split (int, char **);
int                command_strings (int, char **);
int                command_sum (int, char **);
int                command_tabs (int, char **);
int                command_tac (int, char **);
int                command_tail (int, char **);
int                command_tee (int, char **);
int                command_touch (int, char **);
int                command_true (int, char **);
int                command_tty (int, char **);
int                command_uname (int, char **);
int                command_unexpand (int, char **);
int                command_uniq (int, char **);
int                command_unlink (int, char **);
int                command_uptime (int, char **);
int                command_utils (int, char **);
int                command_uudecode (int, char **);
int                command_uuencode (int, char **);
int                command_wc (int, char **);
int                command_who (int, char **);
int                command_whoami (int, char **);
int                command_write (int, char **);
int                command_xargs (int, char **);
int                command_yes (int, char **);


extern char *progname;

static inline void *(xmalloc)(int n)
{
   void *x = malloc (n);
   if (x == NULL)
      error ("could not allocate memory");
   return (x);
}

static inline void *(xrealloc)(void *x, int n)
{
   void *y = realloc (x, n);
   if (y == NULL)
      error ("could not reallocate memory");
   return (x);
}

static inline void *(xcalloc)(int n)
{
   void *x = malloc (n);
   if (x == NULL)
      error ("could not allocate memory");
   return (x);
}

static inline FILE *(xfopen)(const char *name, const char *mode)
{
   FILE *f = fopen (name, mode);
   if (!f)
      error ("cannot open file `%s' %s: %s", name,
         *mode == 'r' ? "for reading" : "for writing", strerror (errno));
   return (f);
}

#endif /* __UTILS_H__ */
