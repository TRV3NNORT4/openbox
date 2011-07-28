#include "openbox/action.h"
#include "openbox/keyboard.h"

static gboolean run_func(ObActionData *data, gpointer options);

void action_breakchroot_startup(void)
{
    action_register("BreakChroot",
                    NULL, NULL,
                    run_func);
}

/* Always return FALSE because its not interactive */
static gboolean run_func(ObActionData *data, gpointer options)
{
    /* break out of one chroot */
    keyboard_reset_chains(1);

    return FALSE;
}
