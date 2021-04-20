#include "cfg.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

struct CFG get_default_config() {
    #ifdef EMSCRIPTEN
    struct CFG cfg = {
        .mouse_sensitivity = 3.0f,
        .render_distance = 4
    };
    #else
    struct CFG cfg = {
        .mouse_sensitivity = 3.0f,
        .render_distance = 16
    };
    #endif

    return cfg;
}

struct CFG read_config() {
    FILE *file = fopen("game.cfg", "r");

    struct CFG cfg = get_default_config();

    if(file == NULL) {
        save_config(cfg);

        return cfg;
    }

    char tag[128];
    float val;

    while((fscanf(file, "%s %f\n", tag, &val)) != EOF) {
        if(strcmp(tag, "mouse_sensitivity") == 0) {
            cfg.mouse_sensitivity = val;
        } else if(strcmp(tag, "render_distance") == 0) {
            cfg.render_distance = (int)val;
        }
    }

    fclose(file);

    return cfg;
}

void save_config(struct CFG cfg) {
    FILE *file = fopen("game.cfg", "w");

    fprintf(file, "mouse_sensitivity %f\n", cfg.mouse_sensitivity);
    fprintf(file, "render_distance %f\n", (float)cfg.render_distance);

    fclose(file);
}