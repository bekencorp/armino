set(incs include include/bk_private include/bk_private/legacy)

armino_component_register(INCLUDE_DIRS "${incs}" 
    PRIV_REQUIRES bk_common
    REQUIRES bk_netif)
