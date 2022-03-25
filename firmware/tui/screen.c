#include "driver-ec11.h"
#include "screen.h"

// FIXME: remove when the callback is implemented
#include <stdio.h>


void
ec11_callback(ps_tui_t *tui, ps_tui_encoder_action_t act)
{
    hard_assert(tui);

    switch (act) {
    case PS_TUI_ENCODER_ACTION_BUTTON:
        printf("button\n");
        break;

    case PS_TUI_ENCODER_ACTION_ROTATE_CCW:
        printf("ccw\n");
        break;

    case PS_TUI_ENCODER_ACTION_ROTATE_CW:
        printf("cw\n");
        break;
    }
}
