//
// Created by lany on 24-5-6.
//

#ifndef PLAT_H
#define PLAT_H

bool blive_init(int x, int y, int width, int height, bool fullscreen);
void blive_clean();

typedef void (*plat_main_loop_func)();
void blive_main_loop(plat_main_loop_func func);

// void _blive_frame_before();
// void _blive_frame_begin();
// void _blive_frame_end(const ImVec4 &clear_color);
// void _blive_frame_after();

void blive_stop();

#endif // PLAT_H
