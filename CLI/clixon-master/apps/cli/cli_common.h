/*
 *
  ***** BEGIN LICENSE BLOCK *****
 
  Copyright (C) 2009-2018 Olof Hagsand and Benny Holmgren

  This file is part of CLIXON.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  Alternatively, the contents of this file may be used under the terms of
  the GNU General Public License Version 3 or later (the "GPL"),
  in which case the provisions of the GPL are applicable instead
  of those above. If you wish to allow use of your version of this file only
  under the terms of the GPL, and not to allow others to
  use your version of this file under the terms of Apache License version 2, 
  indicate your decision by deleting the provisions above and replace them with
  the  notice and other provisions required by the GPL. If you do not delete
  the provisions above, a recipient may use your version of this file under
  the terms of any one of the Apache License version 2 or the GPL.

  ***** END LICENSE BLOCK *****

 *
 */

#ifndef _CLI_COMMON_H_
#define _CLI_COMMON_H_

/*! macro to create a single-argument callback from multiple */
#define cb_single_arg(fn) \
int fn(clicon_handle h, cvec *cvv, cg_var *arg)      \
{						     \
    int   retval=-1;				     \
    cvec *argv = NULL;				     \
						     \
    if (arg && (argv = cvec_from_var(arg)) == NULL){ \
	clicon_err(OE_UNIX, errno, "cvec_from_var"); \
	goto done;				     \
    }						     \
    retval = fn##v(h, cvv, argv);		     \
 done:						     \
    if (argv) cvec_free(argv);			     \
      return retval;				     \
} 


void cli_signal_block(clicon_handle h);
void cli_signal_unblock(clicon_handle h);

/* If you do not find a function here it may be in clicon_cli_api.h which is 
   the external API */

#endif /* _CLI_COMMON_H_ */
