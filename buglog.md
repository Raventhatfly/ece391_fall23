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


**Author**: Zhang Yucheng<br>
**Description**: Display one character and a space when press a keyboard <br>
**Difficulty**: Five minutes, realize if immediately<br>
**Fixing Method**: I forget to consider the scancode when we release a key on the keyboard. I fix it by ignore the scancode larger than 0x58.

**Author**: Luo Hanjun<br>
**Description**: Can not display info mem. <br>
**Difficulty**: Five minutes, realize it immediately after gdb<br>
**Fixing Method**: Wrongly add exception-handle sentence in the tests.c. Actually all code works fine.

**Author**: Liu Xuecheng<br>
**Description**: RTC does not send interrrupt signal. <br>
**Difficulty**: ten minutes<br>
**Fixing Method**: Add enable_irq when initializing.

**Author**: Liu Xuecheng<br>
**Description**: Synchonization fails some times. <br>
**Difficulty**: 20 minutes<br>
**Fixing Method**: Change the place of STI.
