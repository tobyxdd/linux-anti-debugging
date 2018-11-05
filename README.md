# Linux Anti-debugging Demo

This is a very simple demo of an anti-debugging idea that randomly popped into my mind.

Classic anti-debugging techniques on Linux are usually either checking `TracerPID` in `/proc/pid/status` or using `ptrace` to attach to the process and do nothing. These techniques are very easy to bypass, and in many cases can't even waste the reverse any time.

So instead of attaching to the process but doing nothing, what about we actually do *something*?

In this demo, the program has a custom system call (it's just `write` with a different system call number and argument order in this case) that cannot be recognized by any standard system. It must rely on its own `ptrace` code to dynamically replace those calls with standard `write` calls. So whenever someone detaches it and attaches his own debugger, the program automatically loses its ability to run normally.

This demo itself is still quite easy to bypass, since reversers can easily analyze the syscall mapping and implement it into their own debuggers. To make it more effective against (non-retarded) reversers, consider doing the following:

 - Build a customized `libc` with all syscall numbers modified
 - Or even dynamically randomize the syscall mapping each time the program starts
 - Further mess up the argument (register) order
 - Do some weird things to register values (XOR, rotations etc.)
 - Compile your `ptrace` code with `OLLVM` or use other obfuscators
