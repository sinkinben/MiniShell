# MiniShell
Author: Kinben Sin   
E-mail: sinkinben@{qq, outlook, gmail}.com  

# Progress
* Finish buildin-commands
* Finish external command by execvp
* Support more than one commands like `cat /etc/passwd | wc -l | wc | wc -l | grep 1 >> log`

# TODO List
* Multiple progress achieve by token '&'
* Load external commands like `myecho`, `mycp`, `mytree`, etc  
* To support the token `<<`

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

# Test Cases
```
cd
pwd
exit
tree
help
echo a b c
cat /etc/passwd
echo hello >log
cat /etc/passwd | wc -l
cat <input.txt | sort | uniq | cat >output.txt
```


