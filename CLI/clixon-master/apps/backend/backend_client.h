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

#ifndef _BACKEND_CLIENT_H_
#define _BACKEND_CLIENT_H_

/*
 * Types
 */ 
/*
 * Client entry.
 * Keep state about every connected client.
 */
struct client_entry{
    struct client_entry   *ce_next;  /* The clients linked list */
    struct sockaddr        ce_addr;  /* The clients (UNIX domain) address */
    int                    ce_s;     /* stream socket to client */
    int                    ce_nr;    /* Client number (for dbg/tracing) */
    int                    ce_stat_in; /* Nr of received msgs from client */
    int                    ce_stat_out;/* Nr of sent msgs to client */
    int                    ce_pid;   /* Process id */
    int                    ce_uid;   /* User id of calling process */
    clicon_handle          ce_handle; /* clicon config handle (all clients have same?) */
    struct client_subscription   *ce_subscription; /* notification subscriptions */
};

/* Notification subscription info 
 * @see subscription in config_handle.c
 */
struct client_subscription{
    struct client_subscription *su_next;
    int                  su_s; /* stream socket */
    enum format_enum     su_format; /* format of notification stream */
    char                *su_stream;
    char                *su_filter;
};

/*
 * Prototypes
 */ 
int backend_client_rm(clicon_handle h, struct client_entry *ce);
int from_client(int fd, void *arg);

#endif  /* _BACKEND_CLIENT_H_ */
