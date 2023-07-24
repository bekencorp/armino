set(incs)
set(srcs)



set(incs include)
list(APPEND incs
    cs2p2p/osi
    cs2p2p/
    )

if (CONFIG_CS2_P2P_SERVER OR CONFIG_CS2_P2P_CLIENT)
    list(APPEND srcs
        cs2p2p/osi/osi_system.c
        cs2p2p/osi/osi_cs2_not_impl.c
        cs2p2p/cli_cs2_p2p.c
        cs2p2p/cs2_p2p_main.c
    )

    #message(FATAL_ERROR "asd")

    if (CONFIG_CS2_P2P_TEST)
        list(APPEND srcs
            cs2p2p/ListenTester.c
            cs2p2p/ConnectionTester.c
            cs2p2p/ReadWriteTester.c
            cs2p2p/picserver_tester.c
            cs2p2p/test.c
            )
    endif()
endif()


armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" REQUIRES bk_log cs2p2p PRIV_REQUIRES bk_common )
