#!/bin/bash
# Test7: Yang specifics: leafref
APPNAME=example
# include err() and new() functions and creates $dir
. ./lib.sh
cfg=$dir/conf_yang.xml
fyang=$dir/leafref.yang

cat <<EOF > $cfg
<config>
  <CLICON_CONFIGFILE>$cfg</CLICON_CONFIGFILE>
  <CLICON_YANG_DIR>/usr/local/share/$APPNAME/yang</CLICON_YANG_DIR>
  <CLICON_YANG_MODULE_MAIN>example</CLICON_YANG_MODULE_MAIN>
  <CLICON_CLISPEC_DIR>/usr/local/lib/$APPNAME/clispec</CLICON_CLISPEC_DIR>
  <CLICON_CLI_DIR>/usr/local/lib/$APPNAME/cli</CLICON_CLI_DIR>
  <CLICON_CLI_MODE>$APPNAME</CLICON_CLI_MODE>
  <CLICON_SOCK>/usr/local/var/$APPNAME/$APPNAME.sock</CLICON_SOCK>
  <CLICON_BACKEND_PIDFILE>/usr/local/var/$APPNAME/$APPNAME.pidfile</CLICON_BACKEND_PIDFILE>
  <CLICON_CLI_GENMODEL_COMPLETION>1</CLICON_CLI_GENMODEL_COMPLETION>
  <CLICON_XMLDB_DIR>/usr/local/var/$APPNAME</CLICON_XMLDB_DIR>
  <CLICON_XMLDB_PLUGIN>/usr/local/lib/xmldb/text.so</CLICON_XMLDB_PLUGIN>
</config>
EOF

cat <<EOF > $fyang
module example{
    import ietf-ip {
      prefix ip;
    }
    container default-address {
         leaf absname {
             type leafref {
                 path "/ip:interfaces/ip:interface/ip:name";
             }
         }
         leaf relname {
             type leafref {
                 path "../../interfaces/interface/name";
             }
         }
         leaf address {
             type leafref {
                 path "../../interfaces/interface[name = current()/../relname]"
                    + "/ipv4/address/ip";
            }
         }
    }
    list sender{
        key name;
        leaf name{
            type string;
        }
        leaf template{
            type leafref{
                path "/sender/name";
            }
        }
    }
}
EOF

# kill old backend (if any)
new "kill old backend"
sudo clixon_backend -zf $cfg -y $fyang
if [ $? -ne 0 ]; then
    err
fi

new "start backend  -s init -f $cfg -y $fyang"
# start new backend
sudo clixon_backend -s init -f $cfg -y $fyang
if [ $? -ne 0 ]; then
    err
fi

new "leafref base config"
expecteof "$clixon_netconf -qf $cfg -y $fyang" "<rpc><edit-config><target><candidate/></target><config><interfaces>
<interface><name>eth0</name>  <type>eth</type>  <ipv4><address><ip>192.0.2.1</ip></address><address><ip>192.0.2.2</ip></address></ipv4></interface>
<interface><name>lo</name><type>lo</type><ipv4><address><ip>127.0.0.1</ip></address></ipv4></interface>
</interfaces></config></edit-config></rpc>]]>]]>" "^<rpc-reply><ok/></rpc-reply>]]>]]>$"

new "leafref get config"
expecteof "$clixon_netconf -qf $cfg" '<rpc><get-config><source><candidate/></source></get-config></rpc>]]>]]>' '^<rpc-reply><data><interfaces><interface><name>eth0</name>'

new "leafref base commit"
expecteof "$clixon_netconf -qf $cfg -y $fyang" "<rpc><commit/></rpc>]]>]]>" "^<rpc-reply><ok/></rpc-reply>]]>]]>$"

new "leafref add wrong ref"
expecteof "$clixon_netconf -qf $cfg -y $fyang" "<rpc><edit-config><target><candidate/></target><config><default-address><absname>eth3</absname><address>10.0.4.6</address></default-address></config></edit-config></rpc>]]>]]>" "^<rpc-reply><ok/></rpc-reply>]]>]]>$"

new "leafref validate XXX shouldnt really be operation-failed, more work in validate code"
expecteof "$clixon_netconf -qf $cfg -y $fyang" "<rpc><validate><source><candidate/></source></validate></rpc>]]>]]>" "^<rpc-reply><rpc-error><error-tag>operation-failed</error-tag>"

new "leafref discard-changes"
expecteof "$clixon_netconf -qf $cfg" "<rpc><discard-changes/></rpc>]]>]]>" "^<rpc-reply><ok/></rpc-reply>]]>]]>$"

new "leafref add correct absref"
expecteof "$clixon_netconf -qf $cfg -y $fyang" "<rpc><edit-config><target><candidate/></target><config><default-address><absname>eth0</absname></default-address></config></edit-config></rpc>]]>]]>" "^<rpc-reply><ok/></rpc-reply>]]>]]>$"

new "leafref add correct relref"
expecteof "$clixon_netconf -qf $cfg -y $fyang" "<rpc><edit-config><target><candidate/></target><config><default-address><relname>eth0</relname></default-address></config></edit-config></rpc>]]>]]>" "^<rpc-reply><ok/></rpc-reply>]]>]]>$"

# XXX add address

new "leafref validate (ok)"
expecteof "$clixon_netconf -qf $cfg -y $fyang" "<rpc><validate><source><candidate/></source></validate></rpc>]]>]]>" "^<rpc-reply><ok/></rpc-reply>"

new "leafref delete leaf"
expecteof "$clixon_netconf -qf $cfg -y $fyang" "<rpc><edit-config><target><candidate/></target><config><interfaces><interface operation=\"delete\"><name>eth0</name></interface></interfaces></config></edit-config></rpc>]]>]]>" "^<rpc-reply><ok/></rpc-reply>"

new "leafref validate (should fail)"
expecteof "$clixon_netconf -qf $cfg -y $fyang" "<rpc><validate><source><candidate/></source></validate></rpc>]]>]]>"  "^<rpc-reply><rpc-error><error-tag>operation-failed</error-tag>"

new "leafref discard-changes"
expecteof "$clixon_netconf -qf $cfg" "<rpc><discard-changes/></rpc>]]>]]>" "^<rpc-reply><ok/></rpc-reply>]]>]]>$"

new "cli leafref lo"
expectfn "$clixon_cli -1f $cfg -y $fyang -l o set default-address absname lo" "^$"

new "cli leafref validate"
expectfn "$clixon_cli -1f $cfg -y $fyang -l o validate" "^$"

new "cli sender"
expectfn "$clixon_cli -1f $cfg -y $fyang -l o set sender a" "^$"

new "cli sender template"
expectfn "$clixon_cli -1f $cfg -y $fyang -l o set sender b template a" "^$"

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

rm -rf $dir
