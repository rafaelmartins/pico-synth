#include <hardware/gpio.h>
#include <hardware/timer.h>
#include <ec11.h>


void
ec11_init(ec11_t *p)
{
    hard_assert(p);

    gpio_set_dir(p->button, false);
    gpio_set_dir(p->a, false);
    gpio_set_dir(p->b, false);
    gpio_pull_up(p->button);
    gpio_pull_up(p->a);
    gpio_pull_up(p->b);

    mutex_init(&p->mtx);
    p->cb = NULL;
    p->button_hit = 0;
    p->rotate_hit = 0;
    p->rotate_state = EC11_STATE_INITIAL;
    p->a_value = true;
    p->b_value = true;
}


void
ec11_set_callback(ec11_t *p, ec11_callback_t cb)
{
    hard_assert(p);

    mutex_enter_blocking(&p->mtx);
    p->cb = cb;
    mutex_exit(&p->mtx);
}


static inline void
call_callback(ec11_t *p, ec11_callback_type_t type)
{
    mutex_enter_blocking(&p->mtx);
    if (p->cb != NULL)
        p->cb(type);
    mutex_exit(&p->mtx);
}


static inline void
button_task(ec11_t *p)
{
    bool button = gpio_get(p->button);
    if (button)
        return;

    uint64_t t = time_us_64();
    if (t - p->button_hit <= p->button_debounce_us)
        return;
    p->button_hit = t;

    call_callback(p, EC11_CALLBACK_BUTTON);
}


static inline void
rotate_task(ec11_t *p)
{
    bool a = gpio_get(p->a);
    bool b = gpio_get(p->b);
    if (a == p->a_value && b == p->b_value)
        return;

    uint64_t t = time_us_64();
    if (t - p->rotate_hit <= p->rotate_debounce_us)
        return;
    p->rotate_hit = t;

    p->a_value = a;
    p->b_value = b;

    switch (p->rotate_state) {
        case EC11_STATE_INITIAL:
            break;

        case EC11_STATE_FIRST_CW:
        case EC11_STATE_FIRST_CCW:
        case EC11_STATE_SECOND_CW:
        case EC11_STATE_SECOND_CCW:
            if (a && b)
                p->rotate_state = EC11_STATE_INITIAL;
            break;

        case EC11_STATE_CW:
            if (!b)
                p->rotate_state = EC11_STATE_INITIAL;
            break;

        case EC11_STATE_CCW:
            if (!a)
                p->rotate_state = EC11_STATE_INITIAL;
            break;
    }

    switch (p->rotate_state) {
        case EC11_STATE_INITIAL:
            if (!a)
                p->rotate_state = EC11_STATE_FIRST_CW;
            else if (!b)
                p->rotate_state = EC11_STATE_FIRST_CCW;
            break;

        case EC11_STATE_FIRST_CW:
            if ((!a) && (!b))
                p->rotate_state = EC11_STATE_SECOND_CW;
            break;

        case EC11_STATE_FIRST_CCW:
            if ((!a) && (!b))
                p->rotate_state = EC11_STATE_SECOND_CCW;
            break;

        case EC11_STATE_SECOND_CW:
            if (!b)
                p->rotate_state = EC11_STATE_CW;
            break;

        case EC11_STATE_SECOND_CCW:
            if (!a)
                p->rotate_state = EC11_STATE_CCW;
            break;

        case EC11_STATE_CW:
            if (a && b) {
                call_callback(p, EC11_CALLBACK_ROTATE_CW);
                p->rotate_state = EC11_STATE_INITIAL;
            }
            break;

        case EC11_STATE_CCW:
            if (a && b) {
                call_callback(p, EC11_CALLBACK_ROTATE_CCW);
                p->rotate_state = EC11_STATE_INITIAL;
            }
            break;
    }
}


void
ec11_task(ec11_t *p)
{
    hard_assert(p);

    button_task(p);
    rotate_task(p);
}
