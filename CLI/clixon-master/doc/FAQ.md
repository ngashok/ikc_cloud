i# Clixon FAQ

## What is Clixon?

Clixon is a configuration management tool including a generated CLI ,
Yang parser, netconf and restconf interface and an embedded databases.

## Why should I use Clixon?

If you want an easy-to-use configuration frontend based on yang with an
open-source license.  Typically for embedded devices requiring a
config interface such as routers and switches. 

## What license is available?
CLIXON is dual license. Either Apache License, Version 2.0 or GNU
General Public License Version 2. 

## Is Clixon extendible?
Yes. All application semantics is defined in plugins with well-defined
APIs. There are currently plugins for: CLI, Netconf,  Restconf, the datastore and the backend.

## Which programming language is used?
Clixon is written in C. The plugins are written in C. The CLI
specification uses cligen (http://cligen.se)

## How to best understand Clixon?
Run the Clixon example, in the example directory.

## How do you build and install Clixon (and the example)?
Clixon: 
```
	./configure; 
	make; 
	sudo make install; 
	sudo make install-include
```
The example: 
```
	 cd example; 
	 make; 
	 sudo make install
```

## Do I need to setup anything?

The config demon requires a valid group to create a server UNIX socket.
Define a valid CLICON_SOCK_GROUP in the config file or via the -g option
or create the group and add the user to it. The default group is 'clicon'.
On linux:
  sudo groupadd clicon
  sudo usermod -a -G clicon user

## What about reference documentation?
Clixon uses Doxygen for reference documentation.
Build using 'make doc' and aim your browser at doc/html/index.html or
use the web resource: http://clicon.org/ref/index.html

## How do you run the example?
- Start a backend server: 'clixon_backend -Ff /usr/local/etc/example.xml'
- Start a cli session: clixon_cli -f /usr/local/etc/example.xml
- Start a netconf session: clixon_netconf -f /usr/local/etc/example.xml

## How is configuration data stored?
Configuration data is stored in an XML datastore. The default is a
text-based datastore. In the example the datastore are regular files found in
/usr/local/var/example/.

## What is validate and commit?
Clixon follows netconf in its validate and commit semantics.
In short, you edit a 'candidate' configuration, which is first
'validated' for consistency and then 'committed' to the 'running' 
configuration.

A clixon developer writes commit functions to incrementaly upgrade a
system state based on configuration changes. Writing commit callbacks
is the core functionality of a clixon system.

## What is a Clixon configuration file?

Clixon options are stored in an XML configuration file. The default
configuration file is /usr/local/etc/clixon.xml. The example
configuration file is installed at /usr/local/etc/example.xml. The
YANG specification for the configuration file is clixon-config.yang.

You can change where CLixon looks for the configuration FILE as follows:
  - Provide -f FILE option when starting a program (eg clixon_backend -f FILE)
  - Provide --with-configfile=FILE when configuring
  - Provide --with-sysconfig=<dir> when configuring, then FILE is <dir>/clixon.xml
  - Provide --sysconfig=<dir> when configuring then FILE is <dir>/etc/clixon.xml
  - FILE is /usr/local/etc/clixon.xml

## Can I run Clixon as docker containers?
Yes, the example works as docker containers as well. backend and cli needs a 
common file-system so they need to run as a composed pair.
```
	cd example/docker
	make docker # Prepares /data as shared file-system mount
	run.sh      # Starts an example backend and a cli
```
The containers are by default downloaded from dockerhib, but you may
build the containers locally: 
```
	cd docker
	make docker
```
You may also push the containers with 'make push' but you may then consider changing the image name in the makefile.

## How do I use netconf?

As an alternative to cli configuration, you can use netconf. Easiest is to just pipe netconf commands to the clixon_netconf application.
Example:
	echo "<rpc><get-config><source><candidate/></source><configuration/></get-config></rpc>]]>]]>" | clixon_netconf -f /usr/local/etc/example.xml

However, more useful is to run clixon_netconf as an SSH
subsystem. Register the subsystem in /etc/sshd_config:
```
	Subsystem netconf /usr/local/bin/clixon_netconf -f /usr/local/etc/example.xml
```
and then invoke it from a client using
```
	ssh -s <host> netconf
```

## How do I use restconf?

You can access clixon via REST API using restconf, such as using
curl. GET, PUT, POST are supported.

You need a web-server, such as nginx, and start a restconf fcgi
daemon, clixon_restconf. Read more in the restconf docs.

Example:
```
   curl -G http://127.0.0.1/restconf/data/interfaces/interface/name=eth9/type
   [
     {
       "type": "eth" 
     }
   ]
```

## How do I use notifications?

The example has a prebuilt notification stream called "ROUTING" that triggers every 10s.
You enable the notification either via the cli:
```
cli> notify 
cli>
```
or via netconf:
```
clixon_netconf -qf /usr/local/etc/example.xml 
<rpc><create-subscription><stream>ROUTING</stream></create-subscription></rpc>]]>]]>
<rpc-reply><ok/></rpc-reply>]]>]]>
<notification><event>Routing notification</event></notification>]]>]]>
<notification><event>Routing notification</event></notification>]]>]]>
...
```

## How should I start the backend daemon?

There are four different backend startup modes. There is differences in running state treatment, ie what state the machine is when you startthe daemon and how loading the configuration affects it:
- none - Do not touch running state. Typically after crash when running state and db are synched.
- init - Initialize running state. Start with a completely clean running state.
- running - Commit running db configuration into running state. Typically after reboot if a persistent running db exists.
- startup - Commit startup configuration into running state. After reboot when no persistent running db exists.

You use the -s to select the mode:
```
clixon_backend ... -s running
```

You may also add a default method in the configuration file:
```
<config>
   ...
   <CLICON_STARTUP_MODE>init</CLICON_STARTUP_MODE
</config>
```

## How can I add extra XML?

There are two ways to add extra XML to running database  after start. Note that this XML is not "committed" into running.

The first way is via a file. Assume you want to add this xml (the config tag is a necessary top-level tag):
```
<config>
   <x>extra</x>
</config>
```
You add this via the -c option:
```
clixon_backend ... -c extra.xml
```

The second way is by programming the plugin_reset() in the backend
plugin. The example code contains an example on how to do this (see plugin_reset() in example_backend.c).

## I want to program. How do I extend the example?
See [../apps/example] 
- example.xml - Change the configuration file
- The yang specifications - This is the central part. It changes the XML, database and the config cli.
- example_cli.cli - Change the fixed part of the CLI commands 
- example_cli.c - Cli C-commands are placed here.
- example_backend.c - Commit and validate functions.
- example_netconf.c - Netconf plugin
- example_restconf.c - Add restconf authentication, etc.

## How is a plugin initiated?
Each plugin is initiated with an API struct followed by a plugin init function as follows:
```
   static clixon_plugin_api api = {
      "example",          /* name */
      clixon_plugin_init, 
      plugin_start,
      ... /* more functions here */
   }
   clixon_plugin_api *
   clixon_plugin_init(clicon_handle h)
   {
      ...
      return &api; /* Return NULL on error */
   }
```
For more info see [../example/README.md]


## How do I write a commit function?
In the example, you write a commit function in example_backend.c.
Every time a commit is made, transaction_commit() is called in the
backend.  It has a 'transaction_data td' argument which is used to fetch
information on added, deleted and changed entries. You access this
information using access functions as defined in clixon_backend_transaction.h

## How do I check what has changed on commit?
You use XPATHs on the XML trees in the transaction commit callback.
Suppose you want to print all added interfaces:
```
	cxobj *target = transaction_target(td); # wanted XML tree
	vec = xpath_vec_flag(target, "//interface", &len, XML_FLAG_ADD); /* Get added i/fs */
	for (i=0; i<len; i++)             /* Loop over added i/fs */
	  clicon_xml2file(stdout, vec[i], 0, 1); /* Print the added interface */
```
You can look for added, deleted and changed entries in this way.

## How do I access the XML tree?
Using XPATH, find and iteration functions defined in the XML library. Example library functions:
```
      xml_child_each(), 
      xml_find(), 
      xml_body(), 
      xml_print(), 
      xml_apply()
```
More are found in the doxygen reference.

## How do I write a CLI callback function?

1. You add an entry in example_cli.cli
>   example("This is a comment") <var:int32>("This is a variable"), mycallback("myarg");
2. Then define a function in example_cli.c
>   mycallback(clicon_handle h, cvec *cvv, cvec *arv)
where 'cvv' contains the value of the variable 'var' and 'argv' contains the string "myarg".

The 'cvv' datatype is a 'CLIgen variable vector'.
They are documented in [CLIgen tutorial](https://github.com/olofhagsand/cligen/blob/master/cligen_tutorial.pdf)

## How do I write a validation function?
Similar to a commit function, but instead write the transaction_validate() function.
Check for inconsistencies in the XML trees and if they fail, make an clicon_err() call.
```
    clicon_err(OE_PLUGIN, 0, "Route %s lacks ipv4 addr", name);
    return -1;
```
The validation or commit will then be aborted.

## How do I write a state data callback function?

Netconf <get> and restconf GET also returns state data, in contrast to
config data. In YANG state data is specified with "config false;".

To return state data, you need to write a backend state data callback
with the name "plugin_statedata()" where you return an XML tree.

Please look at the example for an example on how to write a state data callback.

## How do I write an RPC function?

A YANG RPC is an application specific operation. Example:
```
   rpc fib-route {
      input {
         leaf inarg { type string; }
      }
      output {
         leaf outarg { type string; }
      }
   }
```
which defines the fib-route operation present in the example (the arguments have been changed).

Clixon automatically relays the RPC to the clixon backend. To
implement the RFC, you need to register an RPC callback in the backend plugin:
Example:
```
int
clixon_plugin_init(clicon_handle h)
{
...
   rpc_callback_register(h, fib_route, NULL, "fib-route");
...
}
```
And then define the callback itself:
```
static int 
fib_route(clicon_handle h,            /* Clicon handle */
	  cxobj        *xe,           /* Request: <rpc><xn></rpc> */
	  cbuf         *cbret,        /* Reply eg <rpc-reply>... */
	  void         *arg,          /* Client session */
	  void         *regarg)       /* Argument given at register */
{
    cprintf(cbret, "<rpc-reply><ok/></rpc-reply>");    
    return 0;
}
```
Here, the callback is over-simplified.

## How do I write an authentication callback?

A restconf call may need to be authenticated. 
You can specify an authentication callback for restconf as follows:
```
int
plugin_credentials(clicon_handle h,     
		   void         *arg)
{
    FCGX_Request *r = (FCGX_Request *)arg;
    ...
    clicon_username_set(h, user);
```

To authenticate, the callback needs to return the value 1 and supply a username.

See [../apps/example/example_restconf.c] plugin_credentials() for
an example of HTTP basic auth.
