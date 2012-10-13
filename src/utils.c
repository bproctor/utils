
/*  utils.c
 *  Copyright (c) 2012 Brad Proctor. <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <libgen.h>
#include <ctype.h>
#include "utils.h"

typedef struct {
	const char *name;
	int (*func)(int, char **);
} Command;

Command a_cmds[] = {{NULL, NULL}};

Command b_cmds[] = {
	{"basename", command_basename},
	{NULL, NULL},
};

Command c_cmds[] = {
	{"cal", command_cal},
	{"cat", command_cat},
	{"chgrp", command_chgrp},
	{"chmod", command_chmod},
	{"chown", command_chown},
	{"chroot", command_chroot},
	{"cksum", command_cksum},
	{"cmp", command_cmp},
	{"colon", command_colon},
	{"comm", command_comm},
	{"cut", command_cut},
	{NULL, NULL},
};

Command d_cmds[] = {
	{"date", command_date},
	{"dirname", command_dirname},
	{"domainname", command_domainname},
	{NULL, NULL},
};

Command e_cmds[] = {
	{"echo", command_echo},
	{"expand", command_expand},
	{NULL, NULL},
};

Command f_cmds[] = {
	{"false", command_false},
	{"fold", command_fold},
	{NULL, NULL},
};

Command h_cmds[] = {
	{"head", command_head},
	{"hostname", command_hostname},
	{NULL, NULL},
};

Command k_cmds[] = {
	{"kill", command_kill},
	{NULL, NULL},
};

Command l_cmds[] = {
	{"link", command_link},
	{"ln", command_ln},
	{"logger", command_logger},
	{"logname", command_logname},
	{"ls", command_ls},
	{NULL, NULL},
};

Command m_cmds[] = {
	{"md5sum", command_md5sum},
	{"mesg", command_mesg},
	{"mkdir", command_mkdir},
	{"mkfifo", command_mkfifo},
	{"mknod", command_mknod},
	{"more", command_more},
	{NULL, NULL},
};

Command n_cmds[] = {
	{"newgrp", command_newgrp},
	{"nl", command_nl},
	{"no", command_no},
	{NULL, NULL},
};

Command o_cmds[] = {
	{"od", command_od},
	{NULL, NULL},
};

Command p_cmds[] = {
	{"paste", command_paste},
	{"prezip", command_prezip},
	{"printf", command_printf},
	{"pwd", command_pwd},
	{NULL, NULL},
};

Command r_cmds[] = {
	{"rm", command_rm},
	{"rmdir", command_rmdir},
	{"rot13", command_rot13},
	{NULL, NULL},
};

Command s_cmds[] = {
	{"seq", command_seq},
	{"sleep", command_sleep},
	{"sort", command_sort},
	{"split", command_split},
	{"strings", command_strings},
	{"sum", command_sum},
	{NULL, NULL},
};

Command t_cmds[] = {
	{"tabs", command_tabs},
	{"tac", command_tac},
	{"tail", command_tail},
	{NULL, NULL},
};

Command u_cmds[] = {
	{"uname", command_uname},
	{"unexpand", command_unexpand},
	{"uniq", command_uniq},
	{"unlink", command_unlink},
	{"uptime", command_uptime},
	{"uudecode", command_uudecode},
	{"uuencode", command_uuencode},
	{NULL, NULL},
};

Command w_cmds[] = {
	{"wc", command_wc},
	{"who", command_who},
	{"whoami", command_whoami},
	{"write", command_write},
	{NULL, NULL},
};

Command x_cmds[] = {
	{"xargs", command_xargs},
	{NULL, NULL},
};

Command y_cmds[] = {
	{"yes", command_yes},
	{NULL, NULL},
};


Command *commands[] = {
	a_cmds, b_cmds, c_cmds, d_cmds, e_cmds, f_cmds, 0,      h_cmds,
	0,      0,      k_cmds, l_cmds, m_cmds, n_cmds, o_cmds, p_cmds,
	0,      r_cmds, s_cmds, t_cmds, u_cmds, 0,      w_cmds, x_cmds,
	y_cmds, 0
};

int main (int argc, char **argv)
{
	Command *cptr;
	int found = 0;
	int ret = EXIT_FAILURE;

	progname = basename(argv[0]);
	
	// Search through all known commands
	if (islower(*progname)) {
		cptr = commands[*progname - 'a'];
		while (cptr) {
			if (strcmp(cptr->name, progname) == 0) {
				found = 1;
				ret = cptr->func(argc, argv);
			}
			++cptr;
		}
	}

	// If we didn't find a matching command
	if (found == 0) {
		puts("invalid command");
	}

	return ret;
}
