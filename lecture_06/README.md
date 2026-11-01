# Mutex Challenge

## Why can passing the address of a local variable to a task be unsafe?
Local variables are temporary and only exist while their specific function is running. If the setup function finishes before the new task has a chance to read the variable, the original data is destroyed and the task will read corrupted "garbage" data.

## When does blinkLED copy the parameter?
It copies the value immediately when the task starts, right after it locks the mutex and before it enters its infinite blinking loop.

## How does the mutex make setup() wait?
Think of the mutex as a single physical key. The new task takes the key first. The setup function also tries to take the key to continue, but because the task has it, the setup function is forced to wait in place. Once the task finishes copying the value and returns the key, the setup function can finally take it and finish.

## Why is this described as a demonstration hack? What would be a better design in a real application?
Mutexes are designed to protect shared data from being edited by two tasks at the same time, not to act as a one-time "wait" signal. In a real application, a better design would be to use a permanent global variable, allocate permanent memory for the value, or use a tool actually designed for signaling and waiting.