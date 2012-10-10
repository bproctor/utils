
#define VERSION		"0.1"

typedef struct {
	char *option;
	void (*func)();
} Option;

void version     (void);
void usage		 (char *);

int  command_cat  (int, char**);
int  command_head (int, char**);
int  command_od   (int, char**);
int  command_tac  (int, char**);
int  command_tail (int, char**);
int  command_yes  (int, char**);
