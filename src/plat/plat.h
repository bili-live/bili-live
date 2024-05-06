//
// Created by lany on 24-5-6.
//

#ifndef PLAT_H
#define PLAT_H

bool plat_init(int x, int y, int width, int height, bool fullscreen);
void plat_clean();

typedef void (*plat_main_loop_func)();
void plat_main_loop(plat_main_loop_func func);

#endif // PLAT_H
