# MiniShell
Author: Kinben Sin   

# TODO List
* To support the token `<<`

# How to build&run MiniShell
Just type this command in dir `MiniShell`.
```
make run
```

# How to use MiniShell
You can use commands in the following formats.  
`item` is necessary, `[item]` is alternative.  
```
>> command [argv]
>> command [argv] >  file
>> command [argv] >> file
>> command [argv] <  file
>> command [argv] << file
>> command [argv] | command1 [argv1] | ... | commandn [argvn]
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
./build/bin/mycp /etc/passwd ./log
./build/bin/myecho a b c
./build/bin/sleep & 
etc
```


