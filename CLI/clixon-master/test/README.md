# Clixon tests

This directory contains testing code for clixon and the example
routing application. Assumes setup of http daemon as describe under apps/restonf
- clixon    A top-level script clones clixon in /tmp and starts all.sh. You can copy this file (review it first) and place as cron script
- all.sh    Run through all tests named 'test*.sh' in this directory. Therefore, if you place a test in this directory matching 'test*.sh' it will be run automatically. 
- test_auth.sh      Auth tests using internal NACM
- test_auth_ext.sh  Auth tests using external NACM
- test_cli.sh       CLI tests
- test_netconf.sh   Netconf tests
- test_restconf.sh  Restconf tests
- test_yang.sh      Yang tests for constructs not in the example.
- test_leafref.sh   Yang leafref tests
- test_datastore.sh Datastore tests

