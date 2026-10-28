# Producer vs Consumer

1. What does the producer send, and what does the consumer do with it?
   - The producer sends an integer value into the queue. Each time a send succeeds, the value increases by 1.
   - The consumer reads the oldest queued value from the front of the queue and prints it.
   - Important: in a FIFO queue, the newest item is at the end of the queue, while the oldest item is at the front.

2. What happens to a queued item after a successful receive?
   - When `xQueueReceive()` succeeds, the item is removed from the queue.
   - The remaining items shift forward, so the queue keeps its FIFO order.

3. Why does the queue fill when the producer interval is 100 ms?
   - The producer adds items faster than the consumer removes them.
   - With a queue length of 5, the queue eventually fills up and `xQueueSend()` fails when no space is available.
   - In that case, the program prints `Queue full: value not sent`, and the value is dropped.

4. Why do gaps appear in the received numbers? Does this break FIFO?
   - Gaps appear because some values are never sent when the queue is full, so they are lost.
   - The producer keeps incrementing `value` even when a send fails, so the next successful send continues from a higher number.
   - This does not break FIFO for the values that are actually received, but it does break the continuity because some values are skipped = data lost.

5. Would increasing the queue length permanently solve this speed difference? Explain briefly.
   - No. Increasing the queue length only delays the overflow.
   - The real issue is the mismatch in timing. The producer is faster than the consumer, so the queue will still fill if that continues.