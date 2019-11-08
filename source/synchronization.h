#pragma once

#include "context.h"

void create_fence(Context& ctx);
void cleanup_fence(Context& ctx);
void create_semaphores(Context& ctx);
void cleanup_semaphores(Context& ctx);
