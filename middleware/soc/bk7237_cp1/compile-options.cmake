set(OVERRIDE_COMPILE_OPTIONS
    "-mstrict-align" 
    "-Wl,--defsym,memcpy=memcpy_ss"
    "-matomic"
)

set(OVERRIDE_LINK_OPTIONS
    "-Wl,--defsym,memcpy=memcpy_ss"
)