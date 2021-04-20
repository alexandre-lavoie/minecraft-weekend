#ifndef CFG_H
#define CFG_H

struct CFG {
    float mouse_sensitivity;
    int render_distance;
};

/**
 * Returns a default CFG struct.
 */
struct CFG get_default_config();

/**
 * Reads the `game.cfg` file and returns the contents.
 * 
 * If cannot find file, calls `save_config` with default values.
 */
struct CFG read_config();

/**
 * Sets the contents of `game.cfg`. 
 */
void save_config(struct CFG);

#endif