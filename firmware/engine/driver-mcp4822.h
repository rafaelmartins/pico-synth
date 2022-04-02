#pragma once

#include <stdint.h>
#include <pico-synth/engine.h>

void mcp4822_init(ps_engine_t *t);
void mcp4822_task(ps_engine_t *t);
uint32_t mcp4822_callback(ps_engine_t *t) __attribute__((weak));
