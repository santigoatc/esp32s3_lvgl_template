SET(SOURCES screens/ui_StartScreen.c
    screens/ui_MenuScreen.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c)

add_library(ui ${SOURCES})
