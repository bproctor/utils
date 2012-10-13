
/*
 *  yesno.c
 *  Copyright (c) 2002-2012 Brad Proctor
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */


#include "utils.h"

/*
 * Get input from stdin to answer a yes/no question.
 * Return 1 for 'y' or 'Y'
 * Return 0 for 'n' or 'N'
 * If neither is given, return the default specified by 'which'
 */

int yesno (int which)
{
   int c = getchar ();

   if (c == 'y' || c == 'Y')
      return (1);
   else if (c == 'n' || c == 'N')
      return (0);
   else
      return (which ? 1 : 0);
}
