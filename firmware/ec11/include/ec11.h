#pragma once

#include <pico/mutex.h>
#include <pico/stdlib.h>

typedef enum {
    EC11_CALLBACK_BUTTON = 1,
    EC11_CALLBACK_ROTATE_CW,
    EC11_CALLBACK_ROTATE_CCW,
} ec11_callback_type_t;

typedef void (*ec11_callback_t) (ec11_callback_type_t type);

typedef enum {
    EC11_STATE_INITIAL = 1,
    EC11_STATE_FIRST_CW,
    EC11_STATE_FIRST_CCW,
    EC11_STATE_SECOND_CW,
    EC11_STATE_SECOND_CCW,
    EC11_STATE_CW,
    EC11_STATE_CCW,
} ec11_state_t;

typedef struct {
    uint button;
    uint a;
    uint b;
    uint64_t button_debounce_us;
    uint64_t rotate_debounce_us;

    // private
    mutex_t mtx;
    ec11_callback_t cb;
    uint64_t button_hit;
    uint64_t rotate_hit;
    ec11_state_t rotate_state;
    bool a_value;
    bool b_value;
} ec11_t;


void ec11_init(ec11_t *p);
void ec11_set_callback(ec11_t *p, ec11_callback_t cb);
void ec11_task(ec11_t *p);
