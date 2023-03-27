set(incs)
set(srcs)




list(APPEND incs
    cs2/osi
    cs2/
    )

    if (CONFIG_CS2_P2P_SERVER OR CONFIG_CS2_P2P_CLIENT)
        list(APPEND srcs
            cs2/osi/osi_system.c
            cs2/osi/osi_cs2_not_impl.c
            cs2/cli_cs2_p2p.c
            cs2/cs2_p2p_main.c
    )


    if (CONFIG_CS2_P2P_TEST)
        list(APPEND srcs
            cs2/ListenTester.c
            cs2/ConnectionTester.c
            cs2/ReadWriteTester.c
            cs2/picserver_tester.c
            cs2/test.c
            )
    endif()


endif()
armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" REQUIRES bk_log bk_thirdparty P2P PRIV_REQUIRES bk_common)
