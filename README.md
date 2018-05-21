# ptree

ptree is a quick hack to print a process tree in Linux. That is, it outputs running processes in a format that indicates who spawned each process. It can output all running processes or just the lineage for specific pids.

### Usage

    ptree [opt] [pid1 pid2 ...]

### Options

The options below are supported by the program.

* `-h`

  Output a summary of usage and options and exit.

* `-v`

  Output the version number, currently v1.0.0, and exit.    

* `-k`

  Output kernel threads as well (i.e. `[kthreadd]` and its descendants).

### Description

This program implements a different approach to the pstree program included with most Linux distributions.

pstree outputs either a full tree of all running processes or the descendants of a given pid. By default it shows only the process names and combines processes with the same name that have the same parent into a single entry.

This program, on the other hand, outputs either a full tree of all running processes, or *the ancestors and descendants* of a given pid. It outputs the pid and entire command line and lists every process individually.

For example, the tree for the current shell is output by pstree as:

```
$ pstree $$ | less -ESX
bash-+-less
     `-pstree

```

The same is output by this program as:

```
$ ptree $$ | less -ESX
1 /usr/lib/systemd/systemd --system --deserialize 97
  1581 /usr/lib/systemd/systemd --user
    3414 /usr/libexec/gnome-terminal-server
      27569 bash
        27703 ptree 27569
        27704 less -ESX
```

This program shows explicitly how each process was started and which processes were started by it or it children.

### Disclaimer

Copyright (C) 2012 by Javier Alvarado.

This program was written for personal use by the author. Permission is hereby granted to study, use, copy, and/or modify for non-commercial use.

***THE SOFTWARE IS PROVIDED "AS IS" AND WITHOUT WARRANTY OF ANY KIND. THE AUTHOR IS IN NO WAY LIABLE FOR ANY DAMAGES OR LOSS OF DATA.***
