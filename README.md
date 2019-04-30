# MiniShell
Author: Kinben Sin   
E-mail: sinkinben@{qq, outlook, gmail}.com  

# Progress
* Finish buildin-command: cd, exit, pwd
* Finish external command by execvp
* Support more than one commands like `cat /etc/passwd | wc -l | wc | wc -l | grep 1 >> log`

# TODO List
* External commands: achieve by execvp
* Multiple progress achieve by token '&'
* Load external commands like `myecho`, `mycp`, `mytree`, etc.

# How to build&run MiniShell
Just type this command in dir `MiniShell`.
```
make run
```

# How to use MiniShell
You can use commands in the following formats.  
`item` is necessary, `[item]` is alternative.  
When using `>`, `>>`, `<`, `<<`, `|`, you must use spaces to separate the left and right part.(Random numbers of spaces are permitted.)  
Maybe I will update to use with or without spaces. 
Now is still in the design phase.:)
```
>> command [argv]
>> command [argv] >  file
>> command [argv] >> file
>> command [argv] <  file
>> command [argv] << file
>> command [argv] | command1 [argv1] | ... | commandn [argvn]
>> command [argv] [< inputfile] [> outputfile] [| command2 ...]
```