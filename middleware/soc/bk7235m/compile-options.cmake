set(OVERRIDE_COMPILE_OPTIONS
    "-mstrict-align"
    "-fstack-protector"
    "-Wl,--defsym,memcpy=memcpy_ss"
    "-matomic"
    "-fno-builtin-printf"
    "-Os"
)

set(OVERRIDE_LINK_OPTIONS
    "-Wl,--defsym,memcpy=memcpy_ss"
    "-fno-builtin-printf"
    "-Os"
)