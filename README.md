# XENOos - Simple Bare Metal Kernel

A lightweight operating system kernel written in C++ for 32-bit x86 architecture, running on QEMU.

## Features

### Core Components
- **VGA Text Output** - 80x25 character text mode display
- **Keyboard Input** - Full keyboard support with backspace handling
- **Multiboot** - Compatible with GRUB bootloader
- **Command Registry** - Efficient command handling system
- **Panic Handler** - Debug shell with system information on kernel panic

### Commands

**System Information:**
- `cpuinfo` - Display CPU architecture and mode information
- `meminfo` - Show memory layout and addresses
- `whoami` - Display kernel version and info
- `date` - Show current date
- `uptime` - Show kernel ticks since boot
- `ps` - List running processes (stub)
- `ls` - List filesystem structure (stub)

**Utilities:**
- `echo` - Echo text back to screen
- `clear` - Clear the display
- `help` - Show all available commands
- `version` - Show XENOos version

**Debug:**
- `kernelp` - Trigger kernel panic and enter debug shell

### Debug Shell (on panic)
When `kernelp` is executed, you enter the debug shell with:
- `mem` - Memory information with hex addresses
- `regs` - Simulated register dump
- `stack` - Stack pointer and base information
- `vga` - VGA buffer address and dimensions
- `halt` - Exit debug shell and halt system

## Files Structure

### Kernel Core
- `main.cpp` - Entry point with Multiboot header
- `commonkernel.h` - Main kernel loop and initialization
- `keyboard.h` - Keyboard input handler
- `vga.h` - VGA text mode display driver

### Command System
- `commands.h` - Command registry and handlers
- `system.h` - System information functions
- `help.h` - Help and command documentation

### Utilities
- `utils.h` - String handling and number conversion
- `panic.h` - Kernel panic and debug shell

### Configuration
- `common.h` - Type definitions
- `multiboot.h` - Multiboot header definitions
- `linker.ld` - Linker script for kernel layout

## Optimizations

1. **Command Registry** - Uses function pointers to dispatch commands efficiently
2. **Utility Functions** - Reusable string handling (strlen, itoa, strcmp)
3. **Memory Efficient** - Direct VGA buffer writing
4. **Clean Architecture** - Separate headers for each subsystem
5. **Backspace Support** - Proper VGA buffer handling for text editing

## Building

```bash
./build.sh
```

Creates `kernel.elf` - a 32-bit ELF executable.

## Running

```bash
qemu-system-x86_64 -kernel kernel.elf
```

The kernel will boot and display:
```
XENOos Kernel v1.0 - Starting...
Type 'help' for commands

> 
```

## Usage Examples

```
> help          # Show all commands
> cpuinfo       # Display CPU info
> meminfo       # Show memory layout
> echo hello    # Echo text
> clear         # Clear screen
> kernelp       # Enter debug mode
> exit          # Shutdown
```

In debug mode:
```
debug> mem      # Show memory info
debug> regs     # Show CPU registers
debug> halt     # Exit and shutdown
```

## Technical Details

- **Architecture** - x86 32-bit protected mode
- **Boot** - GRUB Multiboot protocol
- **Display** - VGA text mode (80x25)
- **Input** - PS/2 keyboard via port I/O
- **Language** - C++ with inline assembly

## Limitations

- Single core, no multitasking
- Basic console interface only
- Simulated process/filesystem info
- No real memory management
- No network or file I/O