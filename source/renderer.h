#ifndef RENDERER_H
#define RENDERER_H

void renderer_init(void);
void renderer_shutdown(void);
void renderer_update(void);
void renderer_reset(void);

int renderer_set_background(const char *bg_name);

int renderer_show_left(const char *name, const char *expression);
int renderer_show_right(const char *name, const char *expression);
int renderer_show_center(const char *name, const char *expression);

void renderer_hide_left(void);
void renderer_hide_right(void);
void renderer_hide_center(void);

#endif