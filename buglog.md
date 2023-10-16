## Team2 RazelOS development Bug Log

-----
### Checkpoint 1

**Author**: Wu Feiyang<br>
**Description**: While passing the default IDT test, the exception handler never executes.<br>
**Difficulty**: Takes 20 minutes, changing only one line of code.<br>
**Fixing Method**: Although I setup the full IDT interface, I failed to laod the IDT table and 
it took me quite a long time to realize the fact. This can be solved by calling the `lidt` function
in `lib.c` to load the IDT table into the IDTR register.

**Author**: Wu Feiyang<br>
**Description**: Make error showing undefined reference.<br>
**Difficulty**: Takes half an hour, changing only one line of code.<br>
**Fixing Method**: The functions are not decalred in the assembly. 
I fix the problem by adding `.global` and declare the function I implemented in 
the assembly code `handler_dispatcher.S`, so that other programs are able to call
the assembly functions.




