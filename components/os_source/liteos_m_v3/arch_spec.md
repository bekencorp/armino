```
.
├── components                                                  --- Components available for porting and header files exposed externally
│   ├── backtrace                                               --- Backtrace
│   ├── cppsupport                                              --- C++
│   ├── cpup                                                    --- CPUP
│   ├── dynlink                                                 --- Dynamic loader & linker
│   ├── exchook                                                 --- Exception hook
│   ├── fs                                                      --- File System
│   └── net                                                     --- Network
├── kal                                                         --- Kernel Abstraction Layer, APIs exposed externally, including CMSIS APIs and part of POSIX APIs
│   ├── cmsis                                                   --- CMSIS
│   └── posix                                                   --- POSIX
├── kernel                                                      --- Code for defining the minimum kernel function set
│   ├── arch                                                    --- Code of the kernel instruction architecture layer
│   │   ├── arm                                                 --- ARM32 architecture
│   │   │   ├── cortex-m3                                       --- Cortex-m3 architecture
│   │   │   │   └── keil                                        --- Implementation of the Keil toolchain
│   │   │   ├── cortex-m33                                      --- Cortex-m33 architecture
│   │   │   │   │── gcc                                         --- Implementation of the GCC toolchain
│   │   │   │   └── iar                                         --- Implementation of the IAR toolchain
│   │   │   └── cortex-m4                                       --- Cortex-m4 architecture
│   │   │   │   │── gcc                                         --- Implementation of the GCC toolchain
│   │   │   │   └── iar                                         --- Implementation of the IAR toolchain
│   │   │   └── cortex-m7                                       --- Cortex-m7 architecture
│   │   │   │   │── gcc                                         --- Implementation of the GCC toolchain
│   │   │   │   └── iar                                         --- Implementation of the IAR toolchain
│   │   ├── risc-v                                              --- Risc-v architecture
│   │   │   ├── nuclei                                          --- Nuclei architecture
│   │   │   │   └── gcc                                         --- Implementation of the GCC toolchain
│   │   │   └── riscv32                                         --- Riscv32 architecture
│   │   │   │   └── gcc                                         --- Implementation of the GCC toolchain
│   │   └── include
│   │       ├── los_arch.h                                      --- Arch initialization
│   │       ├── los_atomic.h                                    --- Atomic operations
│   │       ├── los_context.h                                   --- Context switch
│   │       ├── los_interrupt.h                                 --- Interrupts
│   │       ├── los_mpu.h                                       --- Memory protection unit operations
│   │       └── los_timer.h                                     --- Timer operations
│   ├── include
│   │   ├── los_config.h                                        --- Configuration parameters
│   │   ├── los_event.h                                         --- Events management
│   │   ├── los_membox.h                                        --- Membox management
│   │   ├── los_memory.h                                        --- Heap memory management
│   │   ├── los_mux.h                                           --- Mutex
│   │   ├── los_queue.h                                         --- Queue
│   │   ├── los_sched.h                                         --- Scheduler
│   │   ├── los_sem.h                                           --- Semaphores
│   │   ├── los_sortlink.h                                      --- Sort link
│   │   ├── los_swtmr.h                                         --- Timer
│   │   ├── los_task.h                                          --- Tasks
│   │   └── los_tick.h                                          --- Tick
│   └── src
├── targets
│   └── targets
│       └── riscv_nuclei_demo_soc_gcc
│       │   ├── GCC                                            --- Compilation config
│       │   ├── OS_CONFIG                                      --- Board config
│       │   ├── SoC                                            --- SOC codes
│       │   └── Src                                            --- Application codes
│       └── riscv_nuclei_gd32vf103_soc_gcc
│       └── riscv_sifive_fe310_gcc
└── utils
    ├── internal
    ├── BUILD.gn                                               --- Gn build config file
    ├── los_compiler.h                                         --- Compiler configuration
    ├── los_debug.c                                            --- Debugging facilities
    ├── los_debug.h
    ├── los_error.c                                            --- Errors codes and definitions
    ├── los_error.h
    ├── los_hook.c                                             --- Hook function facilities
    ├── los_hook.h
    ├── los_list.h                                             --- Doubly linked list
    └── los_reg.h                                              --- Register macros
    └── src
```

