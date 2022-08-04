# Linux ptrace anti-debugging demo

Simple proof-of-concept of a random idea.

Classic anti-debugging techniques on Linux include either checking `TracerPID` in `/proc/pid/status` or using `ptrace` to attach to the process and do nothing. These techniques are easy to bypass.

Instead of just attaching to the process and doing nothing, what about we make `ptrace` an integral part of the program that can't be simply removed?

In this demo, the program has a custom system call (it's just `write` with a different system call number and argument/register mapping in this case) that cannot be recognized by the standard Linux kernel. It must rely on its own `ptrace` code to dynamically replace the calls with standard `write` calls. If someone detaches it and attaches their own debugger, the program automatically loses its ability to run normally.

This demo itself is still relatively easy to bypass since a reverser can analyze the syscall mapping and implement it into their own debuggers. To make it more effective, consider doing the following:

 - Build a custom `libc` with all syscall modified
 - Dynamically randomize the syscall mapping each time the program starts
 - Further mess up the argument (register) order
 - Do some weird things to register values (XOR, rotations etc.)
 - Compile your `ptrace` code with `OLLVM` or use other obfuscators
