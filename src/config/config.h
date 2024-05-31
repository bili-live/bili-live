//
// Created by lany on 24-5-19.
//
#pragma once

void config_init(const char *config_path = nullptr);
void config_save();

void set_window_pos(const int x, const int y);
void set_window_size(const int width, const int height);
void set_window_title(const char *title);

void get_window_pos(int *x, int *y);
void get_window_size(int *width, int *height);
void get_window_title(char *title, int max_length);

void set_fullscreen(bool fullscreen);
bool get_fullscreen();

void set_frame_rate(int frame_rate);
int get_frame_rate();

void set_cookie(const char *cookie);
void get_cookie(char *cookie, int max_length);
