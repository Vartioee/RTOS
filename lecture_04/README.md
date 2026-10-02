# Lecture 04: FreeRTOS Task Memory Allocation

## Answers

### a. Did the free heap change when you increased the task stack size?

Yes. The `8192_stack.png` run ends with 340,484 bytes free, while the
`4096_stack.png` run ends with 348,932 bytes free. That is 8,448 fewer freebytes with the larger stack.

### b. What happened to the free heap?

The free heap went down when a task was created. In the `8192_stack.png`
run, it fell by 9,080 bytes after Task A and by another 8,824 bytes after
Task B. In the `4096_stack.png` run, it fell by 4,728 bytes after Task A and
by another 4,728 bytes after Task B.

| Screenshot | Before tasks | After Task A | After Task B |
| --- | ---: | ---: | ---: |
| `8192_stack.png` | 358,388 bytes | 349,308 bytes | 340,484 bytes |
| `4096_stack.png` | 358,388 bytes | 353,660 bytes | 348,932 bytes |

Overall, the free heap fell by 17,904 bytes in the 8192-stack run and by 9,456
bytes in the 4096-stack run. Larger stack show more heap consumed.

### c. Why does a task need stack memory?

For the RTOS to be able to pause and resume a task, it needs to allocate memory for temporary data, 
like local variables, function call infromation and task execution context. When resumed it can use these datapoints to continue where it left off


### d. In your own words, why does creating a FreeRTOS task use RAM?

From available heap, FreeRTOS reserves memory to a task for it to manange and run it. There it stores tasks stack and control block where information like state and priority is contained. Bigger task = more RAM taken from available heap. 
