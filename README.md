# MiniShell
Author: Kinben Sin  
E-mail: sinkinben@{qq, outlook, gmail}.com  

#Progress
* Only support single command in a line

# TODO List
* `Tab` to auto-complete the command
* Parse command strings
* Build in commands: cd, pwd, exit
* External commands: achieve by execvp
* Multiple progress
* MUltiple threads

# How to build&run MiniShell
Just run this command in dir `MiniShell`.
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