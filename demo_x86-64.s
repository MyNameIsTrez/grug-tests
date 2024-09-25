// Assemble: clang demo_x86-64.s -shared
// Assemble: gcc demo_x86-64.s -shared
// Assemble: as demo_x86-64.s
// Assemble: llvm-mc-14 --filetype=obj -o demo_x86-64.o demo_x86-64.s
