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

This program implements an alternative process tree output format to the pstree program included with most Linux distributions. By default, pstree outputs either a full tree of all running processes or just the given pid and its descendants, and shows only the process names and counts.

This program, on the other hand, outputs either a full tree of all running processes, or *the ancestors and descendants* of a given pid. It outputs the pid and entire command line and lists every process individually, emphasizing the process ID over the process name. This makes it more suitable for finding the right process to signal, trace, wait for, etc.

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

This program shows explicitly how each process was started and which processes were started by it or its children. (Note that newer versions of pstree can output the same info using options `-anps`, but they still emphasize process names over IDs.)

### Disclaimer

Copyright (C) 2012 by Javier Alvarado.

This program was written for personal use by the author. Permission is hereby granted to study, use, copy, and/or modify for non-commercial use.

***THE SOFTWARE IS PROVIDED "AS IS" AND WITHOUT WARRANTY OF ANY KIND. THE AUTHOR IS IN NO WAY LIABLE FOR ANY DAMAGES OR LOSS OF DATA.***
