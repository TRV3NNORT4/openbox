#include "openbox/action.h"
#include "openbox/action_value.h"
#include "openbox/screen.h"
#include <glib.h>

typedef struct {
    gboolean current;
    gboolean add;
} Options;

static gpointer setup_func(GHashTable *config);
static gpointer setup_add_func(GHashTable *config);
static gpointer setup_remove_func(GHashTable *config);
static void free_func(gpointer o);
static gboolean run_func(ObActionData *data, gpointer options);
/* 3.4-compatibility */
static gpointer setup_addcurrent_func(GHashTable *config);
static gpointer setup_addlast_func(GHashTable *config);
static gpointer setup_removecurrent_func(GHashTable *config);
static gpointer setup_removelast_func(GHashTable *config);

void action_addremovedesktop_startup(void)
{
    action_register("AddDesktop", setup_add_func, free_func, run_func);
    action_register("RemoveDesktop", setup_remove_func, free_func, run_func);

    /* 3.4-compatibility */
    action_register("AddDesktopLast", setup_addlast_func,
                    free_func, run_func);
    action_register("RemoveDesktopLast", setup_removelast_func,
                    free_func, run_func);
    action_register("AddDesktopCurrent", setup_addcurrent_func,
                    free_func, run_func);
    action_register("RemoveDesktopCurrent", setup_removecurrent_func,
                    free_func, run_func);
}

static gpointer setup_func(GHashTable *config)
{
    ObActionValue *v;
    Options *o;

    o = g_slice_new0(Options);

    v = g_hash_table_lookup(config, "where");
    if (v && action_value_is_string(v)) {
        const gchar *s = action_value_string(v);
        if (!g_ascii_strcasecmp(s, "last"))
            o->current = FALSE;
        else if (!g_ascii_strcasecmp(s, "current"))
            o->current = TRUE;
    }

    return o;
}

static gpointer setup_add_func(GHashTable *config)
{
    Options *o = setup_func(config);
    o->add = TRUE;
    return o;
}

static gpointer setup_remove_func(GHashTable *config)
{
    Options *o = setup_func(config);
    o->add = FALSE;
    return o;
}

static void free_func(gpointer o)
{
    g_slice_free(Options, o);
}

/* Always return FALSE because its not interactive */
static gboolean run_func(ObActionData *data, gpointer options)
{
    Options *o = options;

    action_client_move(data, TRUE);

    if (o->add)
        screen_add_desktop(o->current);
    else
        screen_remove_desktop(o->current);

    action_client_move(data, FALSE);

    return FALSE;
}

/* 3.4-compatibility */
static gpointer setup_addcurrent_func(GHashTable *config)
{
    Options *o = setup_add_func(config);
    o->current = TRUE;
    return o;
}

static gpointer setup_addlast_func(GHashTable *config)
{
    Options *o = setup_add_func(config);
    o->current = FALSE;
    return o;
}

static gpointer setup_removecurrent_func(GHashTable *config)
{
    Options *o = setup_remove_func(config);
    o->current = TRUE;
    return o;
}

static gpointer setup_removelast_func(GHashTable *config)
{
    Options *o = setup_remove_func(config);
    o->current = FALSE;
    return o;
}
