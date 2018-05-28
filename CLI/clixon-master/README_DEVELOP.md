# README for Clixon developers

  * [Code documentation](#documentation)
  * [How to work in git (branching)](#branching)
  * [How the meta-configure stuff works](#meta-configure)
  * [How to debug](#debug)

## Documentation
How to document the code

```
/*! This is a small comment on one line
 *
 * This is a detailed description
 * spanning several lines.
 *
 * Example usage:
 * @code
 *   fn(a, &b);
 * @endcode
 *
 * @param[in] src         This is a description of the first parameter
 * @param[in,out] dest    This is a description of the second parameter
 * @retval TRUE           This is a description of the return value
 * @retval FALSE          This is a description of another return value
 * @see                   See also this function
 */
```

## Branching
How to work in git (branching)

Basically follows: http://nvie.com/posts/a-successful-git-branching-model/
only somewhat simplified:

Do commits in develop branch. When done, merge with master.

## How the meta-configure stuff works
```
configure.ac --.
                    |   .------> autoconf* -----> configure
     [aclocal.m4] --+---+
                    |   `-----> [autoheader*] --> [config.h.in]
     [acsite.m4] ---'

                           .-------------> [config.cache]
     configure* ------------+-------------> config.log
                            |
     [config.h.in] -.       v            .-> [config.h] -.
                    +--> config.status* -+               +--> make*
     Makefile.in ---'                    `-> Makefile ---'
```

## Debug
How to debug

### Configure in debug mode
```
   CFLAGS="-g -Wall" INSTALLFLAGS="" ./configure
```

### Make your own simplified yang and configuration file.
```
cat <<EOF > /tmp/my.yang
module mymodule{
   container x {
    list y {
      key "a";
      leaf a {
        type string;
      }
    }
  }
}
EOF
cat <<EOF > /tmp/myconf.xml
<config>
  <CLICON_CONFIGFILE>/tmp/myconf.xml</CLICON_CONFIGFILE>
  <CLICON_YANG_DIR>/usr/local/share/example/yang</CLICON_YANG_DIR>
  <CLICON_YANG_MODULE_MAIN>example</CLICON_YANG_MODULE_MAIN>
  <CLICON_SOCK>/usr/local/var/example/example.sock</CLICON_SOCK>
  <CLICON_BACKEND_PIDFILE>/usr/local/var/example/example.pidfile</CLICON_BACKEND_PIDFILE>
  <CLICON_XMLDB_DIR>/usr/local/var/example</CLICON_XMLDB_DIR>
  <CLICON_XMLDB_PLUGIN>/usr/local/lib/xmldb/text.so</CLICON_XMLDB_PLUGIN>
</config>
EOF
 sudo clixon_backend -F -s init -f /tmp/myconf.xml -y /tmp/my.yang
 ```

### Run valgrind and callgrind
 ```
  valgrind --leak-check=full --show-leak-kinds=all clixon_netconf -qf /tmp/myconf.xml -y /tmp/my.yang
  valgrind --tool=callgrind clixon_netconf -qf /tmp/myconf.xml -y /tmp/my.yang
  sudo kcachegrind
 ```