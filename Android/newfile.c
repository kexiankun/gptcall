#include <SFML/Graphics.hpp>
#include "nuklear.h"

int main() {
    // 创建SFML窗口
    sfVideoMode mode = {800, 600, 32};
    sfRenderWindow* window = sfRenderWindow_create(mode, "SFML and nuklear", sfResize | sfClose, NULL);

    // 创建nuklear GUI上下文
    struct nk_context* ctx = nk_sfml_init();

    // 创建一个GUI窗口
    struct nk_color background;
    struct nk_color text;
    nk_color_fv(&background.r, 0.10f);
    nk_color_fv(&text.r, 0.90f);
    
    while (sfRenderWindow_isOpen(window)) {
        // 处理SFML事件
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            nk_sfml_handle_event(&event);
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
        }

        // 开始nuklear GUI的帧
        nk_sfml_update(ctx, window);

        // 更新和绘制nuklear界面
        if (nk_begin(ctx, "Hello World", nk_rect(50, 50, 200, 200), NK_WINDOW_BORDER)) {
            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label_colored(ctx, "Hello, World!", NK_TEXT_CENTERED, text);
        }
        nk_end(ctx);

        // 清除窗口并绘制nuklear界面
        sfRenderWindow_clear(window, sfBlack);
        nk_sfml_render(NK_ANTI_ALIASING_ON);
        sfRenderWindow_display(window);
    }

    // 释放资源
    nk_sfml_shutdown();
    sfRenderWindow_destroy(window);

    return 0;
}
