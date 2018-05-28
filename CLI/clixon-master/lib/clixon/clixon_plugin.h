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
/*
 * Internal prototypes, not accessed by plugin client code
 */

#ifndef _CLIXON_PLUGIN_H_
#define _CLIXON_PLUGIN_H_

/*
 * Constants
 */
/* Hardcoded plugin symbol. Must exist in all plugins to kickstart */
#define CLIXON_PLUGIN_INIT     "clixon_plugin_init" 

/*
 * Types
 */
/* Dynamicically loadable plugin object handle. @see return value of dlopen(3) */
typedef void *plghndl_t;

/*! Registered RPC callback function 
 * @param[in]  h       Clicon handle 
 * @param[in]  xn      Request: <rpc><xn></rpc> 
 * @param[out] cbret   Return xml tree, eg <rpc-reply>..., <rpc-error.. 
 * @param[in]  arg     Domain specific arg, ec client-entry or FCGX_Request 
 * @param[in]  regarg  User argument given at rpc_callback_register() 
 */
typedef int (*clicon_rpc_cb)(
    clicon_handle h,       
    cxobj        *xn,      
    cbuf         *cbret,   
    void         *arg,     
    void         *regarg   
);  

/*
 * Prototypes
 */
/* Common plugin function names, function types and signatures. 
 * This plugin code is exytended by backend, cli, netconf, restconf plugins
 *   Cli     see cli_plugin.c
 *   Backend see config_plugin.c
 */

/* Called when backend started with cmd-line arguments from daemon call. 
 * Call plugin start functions with argc/argv multiple arguments.
 * Typically the argc/argv are the ones appearing after "--", eg
 * clicon_cli -f /etc/clicon.xml -- -a myopt
 */
typedef int (plgstart_t)(clicon_handle, int, char **); /* Plugin start */

/* Called just before plugin unloaded. 
 */
typedef int (plgexit_t)(clicon_handle);		       /* Plugin exit */

/*! Called by restconf to check credentials and return username
 */

/* Plugin authorization. Set username option (or not)
 * @param[in]  Clicon handle
 * @param[in]  void*, eg Fastcgihandle request restconf
 * @retval   0 if credentials OK
 * @retval  -1 credentials not OK
 */
typedef int (plgauth_t)(clicon_handle, void *);

typedef int (plgreset_t)(clicon_handle h, const char *db); /* Reset system status */
typedef int (plgstatedata_t)(clicon_handle h, char *xpath, cxobj *xtop);

typedef void *transaction_data;

/* Transaction callbacks */
typedef int (trans_cb_t)(clicon_handle h, transaction_data td); 

/* Hook to override default prompt with explicit function
 * Format prompt before each getline 
 * @param[in] h      Clicon handle
 * @param[in] mode   Cligen syntax mode
 * @retval    prompt Prompt to prepend all CLigen command lines
 */
typedef char *(cli_prompthook_t)(clicon_handle, char *mode);

/* plugin init struct for the api 
 * Note: Implicit init function
 */
struct clixon_plugin_api;
typedef struct clixon_plugin_api* (plginit2_t)(clicon_handle);    /* Clixon plugin Init */

struct clixon_plugin_api{
    /*--- Common fields.  ---*/
    char              ca_name[PATH_MAX]; /* Name of plugin (given by plugin) */
    plginit2_t       *ca_init;           /* Clixon plugin Init (implicit) */
    plgstart_t       *ca_start;          /* Plugin start */
    plgexit_t        *ca_exit;	         /* Plugin exit */
    union {
	struct {
	    cli_prompthook_t *ci_prompt;         /* Prompt hook */
	    cligen_susp_cb_t *ci_suspend;        /* Ctrl-Z hook, see cligen getline */
	    cligen_interrupt_cb_t *ci_interrupt; /* Ctrl-C, see cligen getline */
	} cau_cli;
	struct {
	    plgauth_t        *cr_auth;           /* Auth credentials */
	} cau_restconf;
	struct {
	} cau_netconf;
	struct {
	    plgreset_t       *cb_reset;          /* Reset system status (backend only) */
	    plgstatedata_t   *cb_statedata;      /* Get state data from plugin (backend only) */
	    trans_cb_t       *cb_trans_begin;	 /* Transaction start */
	    trans_cb_t       *cb_trans_validate; /* Transaction validation */
	    trans_cb_t       *cb_trans_complete; /* Transaction validation complete */
	    trans_cb_t       *cb_trans_commit;   /* Transaction commit */
	    trans_cb_t       *cb_trans_end;	 /* Transaction completed  */
	    trans_cb_t       *cb_trans_abort;	 /* Transaction aborted */    
	} cau_backend;

    } u;
};
/* Access fields */
#define ca_prompt         u.cau_cli.ci_prompt
#define ca_suspend        u.cau_cli.ci_suspend
#define ca_interrupt      u.cau_cli.ci_interrupt
#define ca_auth           u.cau_restconf.cr_auth
#define ca_reset          u.cau_backend.cb_reset
#define ca_statedata      u.cau_backend.cb_statedata
#define ca_trans_begin    u.cau_backend.cb_trans_begin
#define ca_trans_validate u.cau_backend.cb_trans_validate
#define ca_trans_complete u.cau_backend.cb_trans_complete
#define ca_trans_commit   u.cau_backend.cb_trans_commit
#define ca_trans_end      u.cau_backend.cb_trans_end
#define ca_trans_abort    u.cau_backend.cb_trans_abort

typedef struct clixon_plugin_api clixon_plugin_api;

/* Internal plugin structure with dlopen() handle and plugin_api
 */
struct clixon_plugin{
    char              cp_name[PATH_MAX]; /* Plugin filename. Note api ca_name is given by plugin itself */
    plghndl_t         cp_handle;  /* Handle to plugin using dlopen(3) */
    clixon_plugin_api cp_api;
};
typedef struct clixon_plugin clixon_plugin;

/*
 * Prototypes
 */

/*! Plugin initialization function. Must appear in all plugins
 * @param[in]  h    Clixon handle
 * @retval     api  Pointer to API struct
 * @see CLIXON_PLUGIN_INIT  default symbol 
 */
clixon_plugin_api *clixon_plugin_init(clicon_handle h);

clixon_plugin *clixon_plugin_each(clicon_handle h, clixon_plugin *cpprev);

clixon_plugin *clixon_plugin_each_revert(clicon_handle h, clixon_plugin *cpprev, int nr);

clixon_plugin *clixon_plugin_find(clicon_handle h, char *name);

int clixon_plugins_load(clicon_handle h, char *function, char *dir, char *regexp);

int clixon_plugin_start(clicon_handle h, int argc, char **argv);

int clixon_plugin_exit(clicon_handle h);

int clixon_plugin_auth(clicon_handle h, void *arg);

/* rpc callback API */
int rpc_callback_register(clicon_handle h, clicon_rpc_cb cb, void *arg, char *tag);

int rpc_callback_delete_all(void);

int rpc_callback_call(clicon_handle h, cxobj *xe, cbuf *cbret, void *arg);

#endif  /* _CLIXON_PLUGIN_H_ */
