SET(SOURCES screens/ui_scrsplash.c
    screens/ui_scrhome.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c)

add_library(ui ${SOURCES})
