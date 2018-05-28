#!/bin/bash
# Startup test: Start clicon daemon in the (four) different startup modes
# and the dbs and files are setup as follows:
# - The example reset_state callback adds "lo" interface
# - An extra xml configuration file starts with an "extra" interface
# - running db starts with a "run" interface
# - startup db starts with a "start" interface
APPNAME=example
# include err() and new() functions and creates $dir
. ./lib.sh
cfg=$dir/conf_startup.xml

cat <<EOF > $cfg
<config>
  <CLICON_CONFIGFILE>$cfg</CLICON_CONFIGFILE>
  <CLICON_YANG_DIR>/usr/local/share/$APPNAME/yang</CLICON_YANG_DIR>
  <CLICON_YANG_MODULE_MAIN>example</CLICON_YANG_MODULE_MAIN>
  <CLICON_CLI_MODE>$APPNAME</CLICON_CLI_MODE>
  <CLICON_BACKEND_DIR>/usr/local/lib/$APPNAME/backend</CLICON_BACKEND_DIR>
  <CLICON_NETCONF_DIR>/usr/local/lib/$APPNAME/netconf</CLICON_NETCONF_DIR>
  <CLICON_RESTCONF_DIR>/usr/local/lib/$APPNAME/restconf</CLICON_RESTCONF_DIR>
  <CLICON_CLI_DIR>/usr/local/lib/$APPNAME/cli</CLICON_CLI_DIR>
  <CLICON_CLISPEC_DIR>/usr/local/lib/$APPNAME/clispec</CLICON_CLISPEC_DIR>
  <CLICON_SOCK>/usr/local/var/$APPNAME/$APPNAME.sock</CLICON_SOCK>
  <CLICON_BACKEND_PIDFILE>/usr/local/var/$APPNAME/$APPNAME.pidfile</CLICON_BACKEND_PIDFILE>
  <CLICON_CLI_GENMODEL_COMPLETION>1</CLICON_CLI_GENMODEL_COMPLETION>
  <CLICON_XMLDB_DIR>/usr/local/var/$APPNAME</CLICON_XMLDB_DIR>
  <CLICON_XMLDB_PLUGIN>/usr/local/lib/xmldb/text.so</CLICON_XMLDB_PLUGIN>
  <CLICON_CLI_LINESCROLLING>0</CLICON_CLI_LINESCROLLING>
  <CLICON_STARTUP_MODE>init</CLICON_STARTUP_MODE>
  <CLICON_XML_SORT>true</CLICON_XML_SORT>
</config>

EOF

run(){
    mode=$1
    expect=$2

    dbdir=$dir/db
    cat <<EOF > $dbdir
<config>
   <interfaces>
      <interface>
         <name>run</name>
         <type>eth</type>
      </interface>
    </interfaces>
</config>
EOF
    sudo mv $dbdir /usr/local/var/$APPNAME/running_db

    cat <<EOF > $dbdir
<config>
   <interfaces>
      <interface>
         <name>startup</name>
         <type>eth</type>
      </interface>
    </interfaces>
</config>
EOF
    sudo mv $dbdir /usr/local/var/$APPNAME/startup_db

    cat <<EOF > $dir/config
<config>
   <interfaces>
      <interface>
         <name>extra</name>
         <type>eth</type>
      </interface>
    </interfaces>
</config>
EOF

    # kill old backend (if any)
    new "kill old backend"
    sudo clixon_backend -zf $cfg
    if [ $? -ne 0 ]; then
	err
    fi

    new "start backend  -f $cfg -s $mode -c $dir/config"
    sudo clixon_backend -f $cfg -s $mode -c $dir/config
    if [ $? -ne 0 ]; then
	err
    fi

    new "Check $mode"
    expecteof "$clixon_netconf -qf $cfg" '<rpc><get-config><source><running/></source></get-config></rpc>]]>]]>' "^<rpc-reply>$expect</rpc-reply>]]>]]>$"

    new "Kill backend"
    # Check if still alive
    pid=`pgrep clixon_backend`
    if [ -z "$pid" ]; then
	err "backend already dead"
    fi
    # kill backend
    sudo clixon_backend -zf $cfg
    if [ $? -ne 0 ]; then
	err "kill backend"
    fi
}

run init    '<data/>'
run none    '<data><interfaces><interface><name>run</name><type>eth</type><enabled>true</enabled></interface></interfaces></data>'
run running '<data><interfaces><interface><name>extra</name><type>eth</type><enabled>true</enabled></interface><interface><name>lo</name><type>local</type><enabled>true</enabled></interface><interface><name>run</name><type>eth</type><enabled>true</enabled></interface></interfaces></data>'
run startup '<data><interfaces><interface><name>extra</name><type>eth</type><enabled>true</enabled></interface><interface><name>lo</name><type>local</type><enabled>true</enabled></interface><interface><name>startup</name><type>eth</type><enabled>true</enabled></interface></interfaces></data>'

rm -rf $dir
