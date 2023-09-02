#include <sys/stat.h>
#include <signal.h>
#include <stdlib.h>
#include <math.h>

#include "fbg_fbdev.h"

#include "perlin.h"

int keep_running = 1;

void int_handler(int dummy) {
    keep_running = 0;
}

struct _fbg_img *texture;

struct _fragment_user_data {
    float pr;
    float xxm;
    float motion;
};

void *fragmentStart(struct _fbg *fbg) {
    struct _fragment_user_data *user_data = (struct _fragment_user_data *)calloc(1, sizeof(struct _fragment_user_data));

    user_data->xxm = 0;
    user_data->motion = 0;
    user_data->pr = 8.0f;

    return user_data;
}

void fragmentStop(struct _fbg *fbg, void *data) {
    struct _fragment_user_data *ud = (struct _fragment_user_data *)data;

    free(ud);
}

void fragment(struct _fbg *fbg, void *user_data) {
    struct _fragment_user_data *ud = (struct _fragment_user_data *)user_data;

    fbg_clear(fbg, 0);

    // what we do here is use the perlin noise function and draw it fullscreen
    // we also mix in a fullscreen texture (which can be repeated along x/y axis if needed)

    float perlin_freq = 0.03 + abs(sin(ud->motion * 8.)) * 8.;

    int x, y;
    for (y = fbg->task_id - 1; y < fbg->height; y += fbg->parallel_tasks) {
        float perlin_x = ud->xxm * fbg->height;

        int rep = 4;

        for (x = 0; x < fbg->width; x += 1) {
            float p = perlin2d(x + perlin_x, y, perlin_freq, 2);

            int r = p*255;
            int g = p*255;
            int b = p*255;

            int yy = fminf(fmaxf(y + ud->pr * p, 0), fbg->height - 1);
            int xx = x;

            int ytl = (((yy * rep - (int)(ud->motion * 4)) % texture->height) * fbg->components) * texture->width;
            int xtl = ytl + (((x * rep + (int)(ud->motion * 4)) % texture->width) * fbg->components);

            r = texture->data[xtl] * (1. - p);
            g = texture->data[xtl + 1] * (1. - p);
            b = texture->data[xtl + 2] * (1. - p);

            fbg_pixel(fbg, xx, yy, r, g, b);
        }
    }

    ud->xxm += 0.001;
    ud->motion += 0.001;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    struct _fbg *fbg = fbg_fbdevInit();
    if (fbg == NULL) {
        return 0;
    }

    signal(SIGINT, int_handler);

    texture = fbg_loadPNG(fbg, "texture.png");
    struct _fbg_img *bbimg = fbg_loadPNG(fbg, "bbmode1_8x8.png");

    struct _fbg_font *bbfont = fbg_createFont(fbg, bbimg, 8, 8, 33);

    fbg_createFragment(fbg, fragmentStart, fragment, fragmentStop, 3);

    do {
        fbg_clear(fbg, 0);

        //fbg_fadeDown(fbg, 2);

        fbg_draw(fbg, NULL);

        // we just draw texts from this thread
        fbg_write(fbg, "FBGraphics", 4, 2);

        fbg_write(fbg, "FPS", 4, 12+8);
        fbg_write(fbg, "#0 (Main): ", 4, 22+8);
        fbg_write(fbg, "#1: ", 4, 32+8);
        fbg_write(fbg, "#2: ", 4, 32+8+2+8);
        fbg_write(fbg, "#3: ", 4, 32+16+4+8);
        fbg_drawFramerate(fbg, NULL, 0, 4+32+48+8, 22+8, 255, 255, 255);
        fbg_drawFramerate(fbg, NULL, 1, 4+32, 32+8, 255, 255, 255);
        fbg_drawFramerate(fbg, NULL, 2, 4+32, 32+8+2+8, 255, 255, 255);
        fbg_drawFramerate(fbg, NULL, 3, 4+32, 32+16+4+8, 255, 255, 255);

        fbg_flip(fbg);
    } while (keep_running);

    fbg_close(fbg);

    fbg_freeImage(texture);
    fbg_freeImage(bbimg);
    fbg_freeFont(bbfont);

    return 0;
}
