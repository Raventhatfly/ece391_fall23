## Team2 RazelOS development Bug Log

-----
### Checkpoint 1

**Author**: Wu Feiyang<br>
**Description**: While passing the default IDT test, the exception handler never executes.<br>
**Difficulty**: Takes 20 minutes, changing only one line of code.<br>
**Fixing Method**: Although I setup the full IDT interface, I failed to laod the IDT table and 
it took me quite a long time to realize the fact. This can be solved by calling the `lidt` function
in `lib.c` to load the IDT table into the IDTR register.<br>

**Author**: Wu Feiyang<br>
**Description**: Make error showing undefined reference.<br>
**Difficulty**: Takes half an hour, changing only one line of code.<br>
**Fixing Method**: The functions are not decalred in the assembly. 
I fix the problem by adding `.global` and declare the function I implemented in 
the assembly code `handler_dispatcher.S`, so that other programs are able to call
the assembly functions.<br>


**Author**: Zhang Yucheng<br>
**Description**: Display one character and a space when press a keyboard <br>
**Difficulty**: Five minutes, realize if immediately<br>
**Fixing Method**: I forget to consider the scancode when we release a key on the keyboard. I fix it by ignore the scancode larger than 0x58.<br>

**Author**: Luo Hanjun<br>
**Description**: Can not display info mem. <br>
**Difficulty**: Five minutes, realize it immediately after gdb<br>
**Fixing Method**: Wrongly add exception-handle sentence in the tests.c. Actually all code works fine.<br>

**Author**: Liu Xuecheng<br>
**Description**: RTC does not send interrrupt signal. <br>
**Difficulty**: ten minutes<br>
**Fixing Method**: Add enable_irq when initializing.<br>

**Author**: Liu Xuecheng<br>
**Description**: Synchonization fails some times. <br>
**Difficulty**: 20 minutes<br>
**Fixing Method**: Change the place of STI.<br>

### Checkpoint 2

**Author**: Luo Hanjun<br>
**Description**: Can not delete when we input more than 128 chars. We can only delete to the char that the overflow starts. <br>
**Difficulty**: 30 minutes<br>
**Fixing Method**: Delete a wrongly-added sentence about end the delete when iterator=0 and add a mod-128 for the overflow. <br>

**Author**: Liu Xuecheng<br>
**Description**: Can not input more than 128 chars in one time. <br>
**Difficulty**: 10 minutes<br>
**Fixing Method**: Add a mod-128 for the overflow. When the overflow starts, the iterator will come back to 0. <br>

**Author**: Liu Xuecheng<br>
**Description**: When we delete characters and start typing again, the cursor is not displayed until the cursor reaches the position of the first deleted character. <br>
**Difficulty**: 25 minutes<br>
**Fixing Method**: Add a sentence that give the value of ATTRIB to the last screen place in the delete. <br>

**Author**: Wu Feiyang<br>
**Description**: The characters appear on the screen are random rubbish characters.<br>
**Difficulty**: 15 minutes <br>
**Fixing Method**: Using array of `uint32_t` instead of `uint8_t` when printing the characters. <br>

### Checkpoint 3

**Author**: Zhang Yucheng<br>
**Description**: ls only shows part of the filename in the directory <br>
**Difficulty**: Half an hour<br>
**Fixing Method**: I find that I add the number of bytes into the file_pos, while we should only add 1 for ls because we read directory one by one. <br>

**Author**: Luo Hanjun<br>
**Description**: when we use ls, the cursor didn't change immediately <br>
**Difficulty**: 5 minutes<br>
**Fixing Method**: Add a draw_cursor function in the terminal write <br>

**Author**: Liu Xuecheng<br>
**Description**: halt cannot return to the previous process <br>
**Difficulty**: 30 minutes<br>
**Fixing Method**: save previous esp and ebp in the pdb and reset them when halt ends <br>

**Author**: Wu Feiyang<br>
**Description**: Page fault after halting the user program<br>
**Difficulty**: 1 day, changing one line of code<br>
**Fixing Method**: The address of filled in the page table is not correct.

### Checkpoint 4
**Author**: Luo Hanjun<br>
**Description**: The RTC didn't work<br>
**Difficulty**: 10 MINUTES, add one line of code<br>
**Fixing Method**: Add a line of code to initialize RTC

**Author**: Wu Feiyang<br>
**Description**: When executing shell strange characters will be printed<br>
**Difficulty**: 5 MINUTES, add one line of code<br>
**Fixing Method**: Just initialize the buffer and set them to '\0' the problem would be fixed.<br>

### Checkpoint 5
**Author** Wu Feiyang <br>
**Description**: Terminal losing control after switching to terminals other than the first terminal <br>
**Difficulty**: 5 hours, chaning several lines of code<br>
**Fixing Method**: When executing the `pithandler` function, there are two functions, namely `process_swith` and `sendedo`.
It seems that  `process_switch` will not return immediately and send EOI to the interrupt handler. So PIC got stuck, unable to 
respond any interrupts. <br>

**Author** Wu Feiyang <br>
**Description**: Program enters page fault executing the first terminal<br>
**Difficulty**: 6 hours, chaning several lines of code<br>
**Fixing Method**: The stack is faulty. I update the current esp and ebp registers in the pcb struct, however when the 
stack was torn down these values are not updated. I decide to add a new variable to store the ebp and esp in case of 
scheduling to avoid the problem.<br>

**Author** Zhang Yucheng <br>
**Description**: The backspace can't function sometimes<br>
**Difficulty**: Half an hour<br>
**Fixing Method**: I use two function to putc, while the one for keyboard responce don't lock because I already lock in irq handler. Double lock lead to this bug.<br>

**Author** Zhang Yucheng <br>
**Description**: Buddy system can't merge the pages after free<br>
**Difficulty**: Half an hour<br>
**Fixing Method**: I forget to judge the condition that the pages are on the head of the list, I fix it by considering it.<br>

### Extra Credit
**Author** Wu Feiyang <br>
**Description**: Program always enter general protection fault after page fault.<br>
**Difficulty**: 10 hours, chaning more than twenty lines of code<br>
**Fixing Method**: I forget that the processor pushes an error code on the stack. The program enters the general protection for loading the wrong segment after `iret`.<br>

**Author** Wu Feiyang <br>
**Description**: Program always page faults and never stop in sigtest.<br>
**Difficulty**: 12 hours, chaning more than a hundred lines of assembly code<br>
**Fixing Method**: The user program `sigtest` require rigorous stack frame that is listed on the document of MP3. At first I didn't know this and used my own stack instead. But after insecting the program of the usercoded encoded in assembly I found the page fault occurred in effort to dereference `eax` and the user program updated eax from the hardware context at a given offset, which is 7, avoid dereferencing NULL, so eax must be at the 7th location from the top of the hardware context stack frame. I modified all of the functions in the assembly to comply with the stack.<br>

**Author** Wu Feiyang <br>
**Description**: Serial driver associated IRQ4 in loopback mode never executes.<br>
**Difficulty**: 10 minutes, chaning one line of code<br>
**Fixing Method**: The example code copied from osdev.com is faulty. It initialzie the wrong port. I looked up the reference table and found the error.<br>