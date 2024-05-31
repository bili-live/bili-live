//
// Created by lany on 24-5-6.
//

#pragma once

#include <atomic>

bool gui_init(int x, int y, int width, int height, bool fullscreen);
void gui_clean();

typedef void (*frame_func)();
void gui_frame(frame_func func);

extern std::atomic_bool done;
