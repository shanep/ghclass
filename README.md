# ghclass

1. Clone this repo: ```git clone https://github.com/shanep/ghclass && cd ghclass```
2. mkdir build && cd build && cmake .. && make
3. Install ghclass into your local path and enjoy!
4. Run the exe to get the help message

## Overview

This is a command line tool to download your classroom.github.com repositories. It requires using
the ssh protocol (https is not supported).

Please read the [official
docs](https://docs.github.com/en/github/authenticating-to-github/connecting-to-github-with-ssh)
to setup your ssh keys.

After you have created your classroom on github and your students have all accepted their
first assignment you need to download the roster (classroom_roster.csv) and place it in the
directory that you want your repos downloaded to.

``` bash
$ ./ghc
-r <class roster>             :[required] File name of the class roster (ex. class_roster.csv)
-o <organization>             :[required] Name of your Github organization
-a <assignment name>          :[required] Name of the assignment
-h                            :Help
$ ./ghc -r classroom_roster.csv -o shanep-classroom -a p1
Cloning: git@github.com:shanep-classroom/p1-BSU-ShanePanter to: /Users/shane/repos/ghclass/data/BSU-ShanePanter

```

![img](img/usage.gif)

## Groups

To download groups make sure and download your roster from classroom.github.com with group
names. This tool should automatically detect the the roster has group names and will download the
assignment into a folder named after the team.

## Extra help

Extra help can be found [here](https://shanepanter.com/2021/01/11/github-classroom-setup.html)
