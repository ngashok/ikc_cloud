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

 */
#ifndef _CLIXON_JSON_PARSE_H_
#define _CLIXON_JSON_PARSE_H_

/*
 * Types
 */

struct clicon_json_yacc_arg{ /* XXX: mostly unrelevant */
    const char           *jy_name;         /* Name of syntax (for error string) */
    int                   jy_linenum;      /* Number of \n in parsed buffer */
    char                 *jy_parse_string; /* original (copy of) parse string */
    void                 *jy_lexbuf;       /* internal parse buffer from lex */
    cxobj                *jy_current;
};

/*
 * Variables
 */
extern char *clixon_json_parsetext;

/*
 * Prototypes
 */
int json_scan_init(struct clicon_json_yacc_arg *jy);
int json_scan_exit(struct clicon_json_yacc_arg *jy);

int json_parse_init(struct clicon_json_yacc_arg *jy);
int json_parse_exit(struct clicon_json_yacc_arg *jy);

int clixon_json_parselex(void *);
int clixon_json_parseparse(void *);
void clixon_json_parseerror(void *, char*);

#endif	/* _CLIXON_JSON_PARSE_H_ */
