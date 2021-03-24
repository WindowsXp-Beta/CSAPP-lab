# Labs of CSAPP
> Also projects of SE-118 SE-119 SE-120 @SJTU-SE

> A big reason I build this repo is to use github to exchange codes between my PC and my virtual machine.

## SVN tutorial
- `svn co svn://ipads.se.sjtu.edu.cn/ics-se19/ics519021910888 icslabs --username=ics519021910888`

co: checkout get the file(path is the set by the url) from svn server into your local directory `icslabs`

- `svn status`
> The symbol 'M' means the file has been modified, and not be commited
> The symbol '?' means the file doesn't exist in svn server
> The symbol 'A' means the file is new, and not be commited

- `svn add filename`
similar to `git add filename` add the file to svn server

- `svn ci filename --message="..."`
similar to `git commmit -m "..."`