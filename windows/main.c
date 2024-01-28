#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <locale.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#define NK_GDI_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_gdi.h"
#include "../style.c"
#include "../overview.c"
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <cjson/cJSON.c>
//#include "ui.c"
CURL *curl;
 int width, height;

// 获取窗口大小的函数
void GetWindowSize(HWND hwnd, int *width, int *height) {
    RECT rect;
    GetWindowRect(hwnd, &rect);

    *width = rect.right - rect.left;
    *height = rect.bottom - rect.top;
}

static void MainLoop(HWND hwnd,struct nk_context *ctx);

int is_maximized = 0;
int is_minimized = 0;

            static int selected_item = 0;
            char username[50];
            int username_len = 0;
            char password[50];
            int password_len = 0;

struct nk_colorf bg = {0.0f, 0.0f, 0.0f, 0.0f}; // 背景颜色

void draw_window(struct nk_context *ctx, HWND hwnd ) {

        // GUI 绘制
        if (nk_begin(ctx, "演示", nk_rect(0.8, 0.8, WINDOW_WIDTH+1, WINDOW_HEIGHT+1),
        NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_BORDER))
         {  
                nk_layout_row_template_begin(ctx, 30);
                nk_layout_row_template_push_dynamic(ctx);
                nk_layout_row_template_push_static(ctx, 30);
                nk_layout_row_template_push_static(ctx, 30);
                nk_layout_row_template_push_static(ctx, 30);
                nk_layout_row_template_end(ctx);
               // 在这里添加您的 GUI 元素
            
             nk_label(ctx, "登入窗口：", NK_TEXT_LEFT);
               if (nk_button_symbol(ctx, NK_SYMBOL_MINUS))
                  {
                    ShowWindow(hwnd, SW_SHOWMINIMIZED);
                  }
                                static int inactive = 1;
               if (inactive) {
                    if(nk_button_symbol(ctx, NK_SYMBOL_RECT_OUTLINE)){
                   
                       ShowWindow(hwnd, SW_SHOWMAXIMIZED); 

                    inactive = 0;
                    }
               }else if(nk_button_symbol(ctx, NK_SYMBOL_RECT_SOLID))
                  {
                    ShowWindow(hwnd, SW_SHOWNORMAL);//正常窗口
                    inactive = 1;
                  }   
                if (nk_button_symbol(ctx, NK_SYMBOL_X))
                  {
                    DestroyWindow(hwnd);//关闭窗口
                  }
    


//   // 在窗口中绘制一条水平直线
//         int y = 30;  // 你想要绘制直线的像素位置
//         struct nk_vec2 line_start = nk_vec2(0, y);//
//         struct nk_vec2 line_end = nk_vec2(320, y);
//         struct nk_color line_color = nk_rgb(0, 0, 0);
//         nk_stroke_line(&ctx->current->buffer, line_start.x, line_start.y, line_end.x, line_end.y, 2, line_color);

//enum theme {THEME_BLACK, THEME_WHITE, THEME_RED, THEME_BLUE, THEME_DARK, THEME_DARK_BLUE, THEME_GREEN, THEME_BROWN, THEME_PURPLE, THEME_DRACULA};


      if (selected_item == 0) { set_style(ctx, THEME_BLACK); }
      if (selected_item == 1) { set_style(ctx, THEME_WHITE);}
      if (selected_item == 2) { set_style(ctx, THEME_RED); }
      if (selected_item == 3) { set_style(ctx, THEME_BLUE); }
      if (selected_item == 4) { set_style(ctx, THEME_DARK); }
      if (selected_item == 5) { set_style(ctx, THEME_DARK_BLUE); }
      if (selected_item == 6) { set_style(ctx, THEME_GREEN); }
      if (selected_item == 7) { set_style(ctx, THEME_BROWN); }
      if (selected_item == 8) { set_style(ctx, THEME_PURPLE);}
      if (selected_item == 9) { set_style(ctx, THEME_DRACULA);}


    static const char *items[] = {"black","white","red","blue","dark","green","brown","purple","dracula","default"};
    enum { aa, bb, cc, dd,ee,ff,gg,hh };

    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "选择主题", NK_TEXT_LEFT);
    if (nk_combo_begin_label(
            ctx, items[selected_item], nk_vec2(nk_widget_width(ctx), 200))) {

      nk_layout_row_dynamic(ctx, 30, 1);
      for (int i = 0; i < 10; ++i)

      {
        if (nk_combo_item_symbol_label(ctx, gg, items[i], NK_TEXT_LEFT)) 
        {

          //    if (nk_combo_item_label(ctx, items[i], NK_TEXT_CENTERED))
          selected_item = i;
        }
      }
      nk_combo_end(ctx);
    }


           if (nk_tree_push(ctx, NK_TREE_NODE, "Inactive", NK_MINIMIZED))
            {
                static int inactive = 1;
                nk_layout_row_dynamic(ctx, 30, 1);
                nk_checkbox_label(ctx, "Inactive", &inactive);

                nk_layout_row_static(ctx, 30, 80, 1);
                if (inactive) {
                    struct nk_style_button button;
                    button = ctx->style.button;
                    ctx->style.button.normal = nk_style_item_color(nk_rgb(40,40,40));
                    ctx->style.button.hover = nk_style_item_color(nk_rgb(40,40,40));
                    ctx->style.button.active = nk_style_item_color(nk_rgb(40,40,40));
                    ctx->style.button.border_color = nk_rgb(60,60,60);
                    ctx->style.button.text_background = nk_rgb(60,60,60);
                    ctx->style.button.text_normal = nk_rgb(60,60,60);
                    ctx->style.button.text_hover = nk_rgb(60,60,60);
                    ctx->style.button.text_active = nk_rgb(60,60,60);
                    nk_button_label(ctx, "button");
                    ctx->style.button = button;
                } else if (nk_button_label(ctx, "button"))
                    fprintf(stdout, "button pressed\n");
                nk_tree_pop(ctx);
            }

            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label(ctx, "用户名:", NK_TEXT_LEFT);

            nk_layout_row_dynamic(ctx, 30, 1);
            nk_flags event = nk_edit_string(ctx, NK_EDIT_FIELD, username, &username_len, 64, nk_filter_default);

            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label(ctx, "密码:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(ctx, 30, 1);
            nk_edit_string(ctx, NK_EDIT_FIELD, password, &password_len, 64, nk_filter_default);

            nk_layout_row_dynamic(ctx, 30, 4);
            if (nk_button_label(ctx, "登录"))
                fprintf(stdout, "登录按钮被点击\n");

            if (nk_button_label(ctx, "注册"))
                fprintf(stdout, "注册按钮被点击\n");

            if (nk_button_label(ctx, "购卡"))
                fprintf(stdout, "购卡按钮被点击\n");

            if (nk_button_label(ctx, "充值积分")){
                fprintf(stdout, "充值积分按钮被点击\n");
                  }

    // // 计算两个矩形的位置和大小
    //     struct nk_rect rect1 = nk_rect(50, 50, 100, 100);
    //     struct nk_rect rect2 = nk_rect(80, 80, 100, 100);

    //     // 绘制矩形边框
    //     float rounding = 0.0f;  // 无圆角
    //     float line_thickness = 2.0f;  // 边框线宽度
    //     struct nk_color rect_color = nk_rgb(255, 0, 0);
    //     nk_stroke_rect(&ctx->current->buffer, rect1, rounding, line_thickness, rect_color);
    //     nk_stroke_rect(&ctx->current->buffer, rect2, rounding, line_thickness, rect_color);

         /* 判断窗口是否折叠 */
        if (nk_window_is_collapsed(ctx, "演示")) {
            ShowWindow(hwnd, SW_SHOWMINIMIZED);
        }

        }
        nk_end(ctx);

    }

// 窗口过程回调函数
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // 处理 Nuklear 事件
    if (nk_gdi_handle_event(hwnd, uMsg, wParam, lParam))
        return 0;

    switch (uMsg) {
        case WM_DESTROY:
            // 当窗口被销毁时发送退出消息
            PostQuitMessage(0);
            return 0;

        case WM_PAINT: {
            // 处理 WM_PAINT 消息
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
            return 0;
        }

        // case WM_NCHITTEST: {
        //     // 处理 WM_NCHITTEST 消息以通过标题栏拖动窗口
        //     LRESULT result = DefWindowProcW(hwnd, uMsg, wParam, lParam);
        //     if (result == HTCLIENT) {
        //         POINT cursorPos;
        //         GetCursorPos(&cursorPos);
        //         ScreenToClient(hwnd, &cursorPos);
        //         RECT windowRect;
        //         GetClientRect(hwnd, &windowRect);
        //         if (cursorPos.y < 30 && cursorPos.x < 200)  {  // 假设标题栏高度为30像素 
        //             return HTCAPTION;
        //         }
        //     }
        //     return result;
        // }

        // case WM_NCLBUTTONDOWN: { //非客户区左键按下消息
        //     if (wParam == HTCLOSE) { 
        //         DestroyWindow(hwnd);
        //         return 0;
        //     } else if (wParam == HTMINBUTTON) {
        //         is_minimized = 1;
        //         return 0;
        //     } else if (wParam == HTMAXBUTTON) {
        //         if (is_maximized) {
        //             ShowWindow(hwnd, SW_SHOWMINIMIZED);
        //             is_maximized = 0;
        //         } else {
        //             ShowWindow(hwnd, SW_MAXIMIZE);
        //             is_maximized = 1;
        //         }
        //         return 0;
        //     }
        //     break;
        // }

        // case WM_GETMINMAXINFO: {
        //     // 处理 WM_GETMINMAXINFO 消息以设置窗口的最小尺寸
        //     MINMAXINFO* minmax = (MINMAXINFO*)lParam;
        //     minmax->ptMinTrackSize.x = 200;
        //     minmax->ptMinTrackSize.y = 100;
        //     return 0;
        // }

        // case WM_SYSCOMMAND: {
        //     // 处理 WM_SYSCOMMAND 消息以防止按下 Alt 键时弹出系统菜单
        //     if (wParam == SC_KEYMENU && (lParam >> 16) <= 0) {
        //         return 0;
        //     }
        //     break;
        // }

        // case WM_SIZE: {
        //     // 处理 WM_SIZE 消息以跟踪窗口的大小变化
        //     int width = LOWORD(lParam);
        //     int height = HIWORD(lParam);

        //     // 设置 GDI 窗口的大小
        //     SetWindowPos(hwnd, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);

        //     // 继续处理其他消息
        //     break;
        // // case WM_SIZE: {
        // //     // 处理 WM_SIZE 消息以跟踪窗口的最大化状态
        // //     if (wParam == SIZE_MAXIMIZED) {
        // //         is_maximized = 1;
        // //     } else if (wParam == SIZE_RESTORED) {
        // //         is_maximized = 0;
        // //     }
        // //     break;
        // }

    }

    // 未处理消息的默认窗口过程
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

// 主函数
int main(void) {

			curl = curl_easy_init();
			if (!curl)
			{
				printf("curl_easy_init() failed\n");
				return 0;
			}

			curl_global_init(CURL_GLOBAL_DEFAULT);

    GdiFont* font;
    struct nk_context *ctx;
    // 注册窗口类
    WNDCLASSW wc = {0};
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandleW(0);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"NuklearWindowClass";
    int running = 1;
    int needs_refresh = 1;

    if (!RegisterClassW(&wc)) {
        MessageBoxW(NULL, L"窗口类注册失败", L"错误", MB_OK | MB_ICONERROR);
        return 1;
    }

    // 获取屏幕宽度和高度
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // 计算窗口位置以居中显示在屏幕上
    int x = (screenWidth - WINDOW_WIDTH) / 2;
    int y = (screenHeight - WINDOW_HEIGHT) / 2;

        // 创建窗口
        HWND hwnd  = CreateWindowW(
        L"NuklearWindowClass", L"xxx",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,//WS_POPUP | WS_VISIBLE,  // 使用 WS_POPUP 创建无边框窗口
        x, y, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, wc.hInstance, NULL);

    if (!hwnd) {
        MessageBoxW(NULL, L"窗口创建失败", L"错误", MB_OK | MB_ICONERROR);
        return 1;
    }
    HDC dc = GetDC(hwnd);

    // GUI 初始化
    font = nk_gdifont_create("Arial", 14);
    ctx = nk_gdi_init(font, dc, WINDOW_WIDTH, WINDOW_HEIGHT);
        RECT rect;
    GetWindowRect(hwnd, &rect);

GetWindowSize(hwnd, &width, &height);


    /*设置窗口主题*/
    // set_style(ctx, THEME_BLACK);
    // set_style(ctx, THEME_WHITE);
    // set_style(ctx, THEME_RED);
       set_style(ctx, THEME_BLUE);
    // set_style(ctx, THEME_DARK);
    // set_style(ctx, THEME_DARK_BLUE);

    // 消息循环
    while (running) {
        // 输入处理
        MSG msg;
        nk_input_begin(ctx);
        if (needs_refresh == 0) {
            if (GetMessageW(&msg, NULL, 0, 0) <= 0)
                running = 0;
            else {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
            needs_refresh = 1;
        } else
            needs_refresh = 0;

        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                running = 0;
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
            needs_refresh = 1;
        }
      // 在绘制之后调用输入结束
         nk_input_end(ctx);
GetWindowSize(hwnd, &width, &height);
         // 绘制
       // draw_window(ctx,hwnd);
MainLoop(hwnd,ctx);
        //overview(ctx);

        // 渲染B
        nk_gdi_render(nk_rgba(255, 255, 255, 128));
    }

    // 清理资源
    nk_gdifont_del(font);
    ReleaseDC(hwnd, dc);
    UnregisterClassW(wc.lpszClassName, wc.hInstance);
    return 0;
}




//100mb
#define MAX 10240
#define MBX 10240
#define MCX 10240


int running = nk_true;
int w, h;

//4
int makeRequestb_a(CURL *curl, const char *endpoint, const char *api_key, const char *input, const char *model);

//3.5
int makeRequestb_b(CURL *curl, const char *endpoint, const char *api_key, const char *input, const char *model);
//3
int makeRequest(CURL *curl, const char *endpoint, const char *api_key, const char *input);

char *reply = "我是助手";
char conversation[MCX] = {0};
char buf_sys[MBX] = {0};
char buf_user[MAX] = {0};
int len = 0;
int len1 = 0;

/*下拉框控制gpt3对话*/
static int selected_item1 = 1;

char cymax_tokens[] = "1024";
char cytemperature[] = "0.9";
char cypresence_penalty[] = "0.0";
char cyfrequency_penalty[] = "0.0";
int myapi_len = 0;
int mysoft_len = 0;
char my_ip[] = "28.0.4.83:80";
int myip_len = 0;
char myapi_key[] = "sk-";
char mysoft_key[] = "jnck-T3BlbkFJ6oyQkabIwAqV7YJWyZh3";

//在指定字符实现换行
char **split_string(char *input, const char *delimiter) {
    char **result = NULL;
    int count = 0;

    char *token = strtok(input, delimiter);
    while (token != NULL) {
        result = realloc(result, sizeof(char *) * (count + 1));
        
        result[count] = malloc(strlen(token) + 1);
        
        strcpy(result[count], token);
        count++;
        token = strtok(NULL, delimiter);
        
    }

    result = realloc(result, sizeof(char *) * (count + 1));
    result[count] = NULL;

    return result;
}

#include <locale.h>

#define MAX_LINE_LENGTH 27

//指定长度插入换行符
/*
void my_insert_newline(char *text, int line_length) {
    setlocale(LC_ALL, "");  // 设置本地化，以正确处理多字节字符
    size_t len2 = strlen(text);
    
    // 为宽字符字符串分配内存
    wchar_t *wtext = (wchar_t*)malloc((len2 + 1) * sizeof(wchar_t));
    if (wtext == NULL) {
        // 处理内存分配失败
        perror("malloc");
        return;
    }

    // 将多字节字符串转换为宽字符字符串
    if (mbstowcs(wtext, text, len2) == (size_t)-1) {
        // 处理转换错误
        perror("mbstowcs");
        free(wtext);
        return;
    }
    
    // 确保宽字符字符串以null字符结尾
    wtext[len2] = L'\0';

    int count = 0;
    for (int i = 0; wtext[i] != L'\0'; i++) {
        count++;
        if (count == line_length) {
            count = 0;
            wtext[i] = L'\n';
        }
    }

    // 将宽字符字符串转换回多字节字符串
    size_t result = wcstombs(text, wtext, len2 + 1);

    if (result == (size_t)-1) {
        // 处理转换错误
        perror("wcstombs");
    }

    // 释放动态分配的内存
    free(wtext);
}
*/


//向后插入字符
int appendToBuffer( char *text)
{
	
	int text_length = strlen(text);
	if (len1 + text_length < MBX - 1)
	{
       // my_insert_newline(text,MAX_LINE_LENGTH);
		len1 += snprintf(buf_sys + len1, MBX - len1, "User:\n%s\nAI: %s\n", buf_user, text);
	}

	len = 0;
	memset(buf_user, 0, sizeof(buf_user));

	return len1;
}



// 线程函数
int thread_function_gpt_x()
{
	const char *endpoint = "https://api.openai.com/v1/chat/completions";

	const char *model = "gpt-4";

	//buf_user[strcspn(buf_user, "\n")] = '\0';

	makeRequestb_b(curl, endpoint, myapi_key, buf_user, model);
	return 0;
}

// 线程函数
int thread_function_gpt_sdw()
{
	const char *endpoint = "https://api.openai.com/v1/chat/completions";

	const char *model = "gpt-3.5-turbo";

	//buf_user[strcspn(buf_user, "\n")] = '\0';

	makeRequestb_b(curl, endpoint, myapi_key, buf_user, model);
	return 0;
}

// 线程函数
int thread_function_gpt_s()
{
	char endpoint[] = "https://api.openai.com/v1/completions";

	//strcat(buf_user, "\n");
	//	buf_user[strcspn(buf_user, "\n")] = '\0';

	if (selected_item1 == 0)
	{
		strcat(conversation, buf_user);
		strcat(conversation, "\n");
		makeRequest(curl, endpoint, myapi_key, buf_user);
	}
	if (selected_item1 == 1)
	{
		strcat(conversation, buf_user);
		strcat(conversation, "\n");
		makeRequest(curl, endpoint, myapi_key, conversation);
	}
	if (selected_item1 == 2)
	{
		strcat(conversation, buf_user);
		strcat(conversation, "\n");
		makeRequest(curl, endpoint, myapi_key, conversation);
	}

	return 0;
}

char *systemout;

struct MemoryStruct
{
	char *memory;
	size_t size;
};

void initMemoryStruct(struct MemoryStruct *chunk)
{
	chunk->memory = malloc(1);
	chunk->size = 0;
}

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, struct MemoryStruct *chunk)
{
	size_t realSize = size * nmemb;
	chunk->memory = realloc(chunk->memory, chunk->size + realSize + 1);
	if (chunk->memory == NULL)
	{
		printf("Error allocating memory\n");
		return 0;
	}
	memcpy(&(chunk->memory[chunk->size]), contents, realSize);
	chunk->size += realSize;
	chunk->memory[chunk->size] = '\0';
	return realSize;
}

// 4
int makeRequestb_a(CURL *curl, const char *endpoint, const char *api_key, const char *input, const char *model)
{
	float mymax_tokens = atof(cymax_tokens);
	float mytemperature = atof(cytemperature);
	float mypresence_penalty = atof(cypresence_penalty);
	float myfrequency_penalty = atof(cyfrequency_penalty);

	struct MemoryStruct chunk;
	initMemoryStruct(&chunk);

	struct curl_slist *headers = NULL;
	char auth_header[MCX];
	headers = curl_slist_append(headers, "Content-Type: application/json");
	snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", api_key);
	headers = curl_slist_append(headers, auth_header);

	// 准备请求数据
	cJSON *json = cJSON_CreateObject();
	if (json == NULL)
	{
		printf("Error creating JSON object\n");
		return 0;
	}
	cJSON_AddStringToObject(json, "model", model);
	cJSON_AddArrayToObject(json, "messages");

	/*	// 添加系统角色消息
	cJSON *system_message = cJSON_CreateObject();

	if (system_message == NULL)
	{
		printf("Error creating system_message object\n");
		return;
	}
	cJSON_AddStringToObject(system_message, "role", "system");
	cJSON_AddStringToObject(system_message, "content", reply);
	cJSON_AddItemToArray(cJSON_GetObjectItem(json, "messages"), system_message);
*/
	// 添加助手角色消息
	cJSON *assistant_message = cJSON_CreateObject();
	if (assistant_message == NULL)
	{
		appendToBuffer("创建 assistant_message 对象时出错\n");

		return 0;
	}
	cJSON_AddStringToObject(assistant_message, "role", "assistant");
	cJSON_AddStringToObject(assistant_message, "content", reply);
	cJSON_AddItemToArray(cJSON_GetObjectItem(json, "messages"), assistant_message);

	// 用户角色消息
	cJSON *user_message = cJSON_CreateObject();
	if (user_message == NULL)
	{
		appendToBuffer("创建 user_message 对象时出错\n");

		return 0;
	}
	cJSON_AddStringToObject(user_message, "role", "user");
	cJSON_AddStringToObject(user_message, "content", input);
	cJSON_AddItemToArray(cJSON_GetObjectItem(json, "messages"), user_message);
	cJSON_AddNumberToObject(json, "max_tokens", mymax_tokens);
	cJSON_AddNumberToObject(json, "temperature", mytemperature);
	cJSON_AddNumberToObject(json, "n", 1);
	cJSON_AddNumberToObject(json, "top_p", 1);
	cJSON_AddNumberToObject(json, "presence_penalty", mypresence_penalty);
	cJSON_AddNumberToObject(json, "frequency_penalty", myfrequency_penalty);
	// 添加数组类型的参数
	cJSON *stop_array = cJSON_CreateArray();
	cJSON_AddItemToArray(stop_array, cJSON_CreateString("##STOP##"));
	cJSON_AddItemToArray(stop_array, cJSON_CreateString("##END##"));
	cJSON_AddItemToObject(json, "stop", stop_array);

	cJSON_Delete(json);

	// 将 JSON 对象转换为字符串best_of
	char *request_data = cJSON_Print(json);
	if (request_data == NULL)
	{
		appendToBuffer("Error converting JSON to string\n");

		return 0;
	}

	// Send HTTP POST request
	curl_easy_setopt(curl, CURLOPT_URL, endpoint);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_data);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		appendToBuffer(curl_easy_strerror(res));
	}
	else
	{
		// 解析JSON响应
		cJSON *root = cJSON_Parse(chunk.memory);
		if (json)
		{
			cJSON *choices = cJSON_GetObjectItem(root, "choices");

			if (cJSON_IsArray(choices))
			{
				cJSON *message = cJSON_GetObjectItem(cJSON_GetArrayItem(choices, 0), "message");

				cJSON *role = cJSON_GetObjectItem(message, "role");
				if (cJSON_IsString(role))
				{
					cJSON *content = cJSON_GetObjectItem(message, "content");

					if (cJSON_IsString(content))
					{
						reply = content->valuestring;		  //上下文
						appendToBuffer(content->valuestring); //打印文本
					}
				}
			}
		}
		else
		{
			appendToBuffer(chunk.memory);
		}
		cJSON_Delete(root);
	}

	free(chunk.memory);
	curl_slist_free_all(headers);
	free(request_data);

	return 0;
}

// 3.5
int makeRequestb_b(CURL *curl, const char *endpoint, const char *api_key, const char *input, const char *model)
{
	float mymax_tokens = atof(cymax_tokens);
	float mytemperature = atof(cytemperature);
	float mypresence_penalty = atof(cypresence_penalty);
	float myfrequency_penalty = atof(cyfrequency_penalty);

	struct MemoryStruct chunk;
	initMemoryStruct(&chunk);

	struct curl_slist *headers = NULL;
	char auth_header[MCX];
	headers = curl_slist_append(headers, "Content-Type: application/json");
	snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", api_key);
	headers = curl_slist_append(headers, auth_header);

	// 准备请求数据
	cJSON *json = cJSON_CreateObject();
	if (json == NULL)
	{
		printf("Error creating JSON object\n");
		return 0;
	}
	cJSON_AddStringToObject(json, "model", model);
	cJSON_AddArrayToObject(json, "messages");

	/*	// 添加系统角色消息
	cJSON *system_message = cJSON_CreateObject();

	if (system_message == NULL)
	{
		printf("Error creating system_message object\n");
		return;
	}
	cJSON_AddStringToObject(system_message, "role", "system");
	cJSON_AddStringToObject(system_message, "content", reply);
	cJSON_AddItemToArray(cJSON_GetObjectItem(json, "messages"), system_message);
*/
	// 添加助手角色消息
	cJSON *assistant_message = cJSON_CreateObject();
	if (assistant_message == NULL)
	{
		appendToBuffer("创建 assistant_message 对象时出错\n");

		return 0;
	}
	cJSON_AddStringToObject(assistant_message, "role", "assistant");
	cJSON_AddStringToObject(assistant_message, "content", reply);
	cJSON_AddItemToArray(cJSON_GetObjectItem(json, "messages"), assistant_message);

	// 用户角色消息
	cJSON *user_message = cJSON_CreateObject();
	if (user_message == NULL)
	{
		appendToBuffer("创建 user_message 对象时出错\n");

		return 0;
	}
	cJSON_AddStringToObject(user_message, "role", "user");
	cJSON_AddStringToObject(user_message, "content", input);
	cJSON_AddItemToArray(cJSON_GetObjectItem(json, "messages"), user_message);
	cJSON_AddNumberToObject(json, "max_tokens", mymax_tokens);
	cJSON_AddNumberToObject(json, "temperature", mytemperature);
	cJSON_AddNumberToObject(json, "n", 1);
	cJSON_AddNumberToObject(json, "top_p", 1);
	cJSON_AddNumberToObject(json, "presence_penalty", mypresence_penalty);
	cJSON_AddNumberToObject(json, "frequency_penalty", myfrequency_penalty);
	cJSON_AddStringToObject(json, "stream", 0 /* false */);
	cJSON_AddStringToObject(json, "logprobs", 0);
	//	cJSON_AddStringToObject(json, "stop", "END");
	// 添加数组类型的参数
	// cJSON *stop_array = cJSON_CreateArray();
	// cJSON_AddItemToArray(stop_array, cJSON_CreateString("##STOP##"));
	// cJSON_AddItemToArray(stop_array, cJSON_CreateString("##END##"));
	// cJSON_AddItemToObject(json, "stop", stop_array);

	// 将 JSON 对象转换为字符串best_of
	char *request_data = cJSON_Print(json);
	if (request_data == NULL)
	{
		appendToBuffer("Error converting JSON to string\n");

		return 0;
	}

	// Send HTTP POST request
	curl_easy_setopt(curl, CURLOPT_URL, endpoint);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_data);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);
	//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		appendToBuffer(curl_easy_strerror(res));
	}
	else
	{
		// 解析JSON响应
		cJSON *root = cJSON_Parse(chunk.memory);
		if (json)
		{
			cJSON *choices = cJSON_GetObjectItem(root, "choices");

			if (cJSON_IsArray(choices))
			{
				cJSON *message = cJSON_GetObjectItem(cJSON_GetArrayItem(choices, 0), "message");

				cJSON *role = cJSON_GetObjectItem(message, "role");
				if (cJSON_IsString(role))
				{
					cJSON *content = cJSON_GetObjectItem(message, "content");

					if (cJSON_IsString(content))
					{
						reply = content->valuestring;		  //上下文
						appendToBuffer(content->valuestring); //打印文本
					}
				}
			}
			else
			{
				appendToBuffer(chunk.memory);
			}
			cJSON_Delete(root);
			cJSON_Delete(json);
		}
	}

	free(chunk.memory);
	curl_slist_free_all(headers);
	free(request_data);

	return 0;
}

int makeRequest(CURL *curl, const char *endpoint, const char *api_key, const char *input)
{
	float mymax_tokens = atof(cymax_tokens);
	float mytemperature = atof(cytemperature);
	float mypresence_penalty = atof(cypresence_penalty);
	float myfrequency_penalty = atof(cyfrequency_penalty);

	struct MemoryStruct chunk;
	initMemoryStruct(&chunk);

	char auth_header[MCX];
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");
	snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", api_key);
	headers = curl_slist_append(headers, auth_header);

	// Create JSON object
	cJSON *root = cJSON_CreateObject();
	if (root == NULL)
	{
		appendToBuffer("创建 JSON 对象出错\n");

		return 0;
	}

	// Add request parameters
	cJSON_AddStringToObject(root, "model", "gpt-3.5-turbo-instruct");

	cJSON *user_message = cJSON_CreateArray();
	cJSON_AddStringToObject(user_message, "content", conversation);
	//	cJSON_AddStringToObject(user_message, "content", input);

	cJSON_AddItemToObject(root, "prompt", user_message);

	//cJSON_AddStringToObject(root, "prompt", input);
	cJSON_AddNumberToObject(root, "max_tokens", mymax_tokens);
	cJSON_AddNumberToObject(root, "temperature", mytemperature);
	cJSON_AddNumberToObject(root, "n", 1);
	cJSON_AddNumberToObject(root, "top_p", 1);
	cJSON_AddNumberToObject(root, "presence_penalty", mypresence_penalty);
	cJSON_AddNumberToObject(root, "frequency_penalty", myfrequency_penalty);
	cJSON_AddStringToObject(root, "stream", 0 /* false */);
	cJSON_AddStringToObject(root, "logprobs", 0);
	//	cJSON_AddStringToObject(root, "stop", "");
	// 添加数组类型的参数
	cJSON *stop_array = cJSON_CreateArray();
	cJSON_AddItemToArray(stop_array, cJSON_CreateString("##STOP##"));
	cJSON_AddItemToArray(stop_array, cJSON_CreateString("##END##"));
	cJSON_AddItemToObject(root, "stop", stop_array);

	// Convert JSON object to string
	char *request_data = cJSON_Print(root);
	if (request_data == NULL)
	{
		appendToBuffer("将 JSON 转换为 string\n 时出错");

		return 0;
	}

	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_URL, endpoint);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_data);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookies.txt");
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies.txt");

	// 设置http代理服务器：
	// curl_easy_setopt(curl, CURLOPT_PROXY, my_ip);
	// 设置http的代理类型：
	//curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_HTTPS);
	// 若是https请求，设置通道：
	//	curl_easy_setopt(curl, CURLOPT_HTTPPROXYTUNNEL, 1L);
	// 若需要代理用户密码：
	// curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, "user:password");

	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		appendToBuffer(curl_easy_strerror(res));
	}
	else
	{
		cJSON *json = cJSON_Parse(chunk.memory);
		 printf("Asst: %s\n,",chunk.memory);
		if (json)
		{
			cJSON *choices = cJSON_GetObjectItemCaseSensitive(json, "choices");
			if (cJSON_IsArray(choices))
			{
				cJSON *choice = cJSON_GetArrayItem(choices, 0);
				cJSON *text = cJSON_GetObjectItemCaseSensitive(choice, "text");
				if (cJSON_IsString(text))
				{
					if (selected_item1 == 0)
					{ //方便理解写上
					}
					if (selected_item1 == 1)
					{
						memset(conversation, 0, sizeof(conversation));
						strcat(conversation, text->valuestring);
						strcat(conversation, "\n");
					}
					if (selected_item1 == 2)
					{
						strcat(conversation, text->valuestring);
						strcat(conversation, "\n");
					}

					appendToBuffer(text->valuestring);
				}
			}
			else
			{
				appendToBuffer(chunk.memory);
			}
			cJSON_Delete(json);
		}
	}
	cJSON_Delete(root);
	free(chunk.memory);
	curl_slist_free_all(headers);
	free(request_data);

	return 0;
}

/* ――――――渲染GUI―――――― */
static void MainLoop(HWND hwnd, struct nk_context *ctx)
{

GetWindowSize(hwnd, &width, &height);
	int RH = height / 10;

    if (nk_begin(ctx, "JNCKGUI_Android Chat Gpt", nk_rect(0, 0, width-15, height),
        NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_BORDER))
	{
		static int lop = 5;
		static int popup_active;
        
		nk_layout_row_dynamic(ctx, RH / 3, 4);
		nk_flags flags = NK_EDIT_SIMPLE;
		if (nk_button_symbol_text(ctx, -1, "设置", 6, NK_TEXT_ALIGN_CENTERED))

			popup_active = 1;

		if (popup_active)
		{
			static struct nk_rect s = {80, 400, 910, 700};
			if (nk_popup_begin(ctx, NK_POPUP_DYNAMIC, "root", 0, s))
			{
				nk_layout_row_dynamic(ctx, RH / 2, 5);

				// style
				if (nk_option_label(ctx, "白色", lop == 0))
				{
					lop = 0;
					set_style(ctx, 1);
				}
				if (nk_option_label(ctx, "红色", lop == 1))
				{
					lop = 1;
					set_style(ctx, 2);
				}
				if (nk_option_label(ctx, "蓝色", lop == 2))
				{
					lop = 2;
					set_style(ctx, 3);
				}
				if (nk_option_label(ctx, "灰色", lop == 3))
				{
					lop = 3;
					set_style(ctx, 5);
				}
				if (nk_option_label(ctx, "全灰", lop == 4))
				{
					lop = 4;
					set_style(ctx, 4);
				}

				nk_layout_row_dynamic(ctx, RH / 3, 1);
				nk_label(ctx, "max_tokens ", NK_TEXT_LEFT);
				nk_flags event = nk_edit_string_zero_terminated(ctx, flags, cymax_tokens, sizeof(cymax_tokens), nk_filter_float);
				if (event & NK_EDIT_ACTIVATED)
				{

				}

				if (event & NK_EDIT_DEACTIVATED)
				{
					
				}

				nk_layout_row_dynamic(ctx, RH / 3, 1);
				nk_label(ctx, "temperature ", NK_TEXT_LEFT);
				nk_flags eventa = nk_edit_string_zero_terminated(ctx, flags, cytemperature, sizeof(cytemperature), nk_filter_float);
				if (eventa & NK_EDIT_ACTIVATED)
				{
					
				}

				if (eventa & NK_EDIT_DEACTIVATED)
				{
					
				}

				nk_layout_row_dynamic(ctx, RH / 3, 1);
				nk_label(ctx, "presence_penalty ", NK_TEXT_LEFT);
				nk_flags eventb = nk_edit_string_zero_terminated(ctx, flags, cypresence_penalty, sizeof(cypresence_penalty), nk_filter_float);
				if (eventb & NK_EDIT_ACTIVATED)
				{
					
				}

				if (eventb & NK_EDIT_DEACTIVATED)
				{
					
				}

				nk_layout_row_dynamic(ctx, RH / 3, 1);
				nk_label(ctx, "frequency_penalty ", NK_TEXT_LEFT);
				nk_flags eventc = nk_edit_string_zero_terminated(ctx, flags, cyfrequency_penalty, sizeof(cyfrequency_penalty), nk_filter_float);
				if (eventc & NK_EDIT_ACTIVATED)
				{
					
				}

				if (eventc & NK_EDIT_DEACTIVATED)
				{
				
				}

				nk_layout_row_dynamic(ctx, RH / 3, 1);
				nk_label(ctx, "OPENAI API_KEY ", NK_TEXT_LEFT);
				nk_flags eventd = nk_edit_string(ctx, flags, myapi_key, &myapi_len, sizeof(myapi_key), nk_filter_default);
				if (eventd & NK_EDIT_ACTIVATED)
				{
				
				}

				if (eventd & NK_EDIT_DEACTIVATED)
				{
				
				}

				nk_layout_row_dynamic(ctx, RH / 3, 1);
				nk_label(ctx, "SOFT_KEY ", NK_TEXT_LEFT);
				nk_flags evente = nk_edit_string(ctx, flags, mysoft_key, &mysoft_len, sizeof(mysoft_key), nk_filter_default);
				if (evente & NK_EDIT_ACTIVATED)
				{
				
				}

				if (evente & NK_EDIT_DEACTIVATED)
				{
				
				}

				nk_layout_row_dynamic(ctx, RH / 3, 1);
				nk_label(ctx, "PROXY_IP ", NK_TEXT_LEFT);
				nk_flags eventf = nk_edit_string(ctx, flags, my_ip, &myip_len, sizeof(my_ip), nk_filter_default);
				if (eventf & NK_EDIT_ACTIVATED)
				{
				
				}

				if (eventf & NK_EDIT_DEACTIVATED)
				{
				
				}
				nk_layout_row_dynamic(ctx, RH / 2, 1);
				if (nk_button_label(ctx, ""))
				{
					popup_active = 0;
					nk_popup_close(ctx);
				}
				/* if (nk_button_label(ctx, "取消")) {          
				       popup_active = 0;
				   nk_popup_close(ctx); } */
				nk_popup_end(ctx);
			}
			else
				popup_active = nk_false;
		}

		if (nk_button_symbol_text(ctx, -1, "清空", 6, NK_TEXT_ALIGN_CENTERED))
		{
			// 清空数组

			nk_memset(buf_user, 0, MAX);
			nk_memset(buf_sys, 0, MBX);
			len = 0, len1 = 0;
		}
		//复制粘贴
		if (nk_button_label(ctx, "复制"))
		{
		
		}

		if (nk_button_label(ctx, "粘贴"))
		{

		}

		/* 函数：调整控件宽度NK布局行动态 */
		nk_layout_row_dynamic(ctx, RH, 1);

		/*创建可编辑的文本输入框*/
		nk_flags input_text =
			nk_edit_string(ctx, NK_EDIT_BOX, buf_user, &len, sizeof(buf_user), nk_filter_default);

		if (input_text & NK_EDIT_ACTIVATED)
		{
			
		}

		if (input_text & NK_EDIT_DEACTIVATED)
		{
			
		}

		/*下拉框模型*/
		static int selected_item = 2;
		static const char *items[] = {"OPENAI GPT-4 (VIP)", "OPENAI GPT-3.5", "gpt-3.5-turbo-instruct"};
		nk_layout_row_dynamic(ctx, RH / 3, 2);
		//nk_label(ctx, "选择模型：{模型}", NK_TEXT_LEFT);
		if (nk_combo_begin_label(ctx, items[selected_item], nk_vec2(nk_widget_width(ctx), RH * 3)))
		{
			nk_layout_row_dynamic(ctx, RH / 2, 1);
			for (int i = 0; i < 3; ++i)
			{
				/* 检查当前选项是否为选中状态 */
				if (nk_combo_item_symbol_label(ctx, selected_item == i ? 7 : 5, items[i], NK_TEXT_RIGHT))
				{
					selected_item = i;
				}
			}
			nk_combo_end(ctx);
		}

		static const char *items1[] = {"gpt-3.5-turbo-instruct单次对话", "gpt-3.5-turbo-instruct理解上下文", "gpt-3.5-turbo-instruct理解全文"};

		//nk_label(ctx, "选择模型：{模型}", NK_TEXT_LEFT);
		if (nk_combo_begin_label(ctx, items1[selected_item1], nk_vec2(nk_widget_width(ctx), RH * 3)))
		{
			nk_layout_row_dynamic(ctx, RH / 2, 1);
			for (int i = 0; i < 3; ++i)
			{
				/* 检查当前选项是否为选中状态 */
				if (nk_combo_item_symbol_label(ctx, selected_item1 == i ? 7 : 5, items1[i], NK_TEXT_RIGHT))
				{
					selected_item1 = i;
				}
			}
			nk_combo_end(ctx);
		}

		/*创建一个文本编辑框*/
		nk_layout_row_dynamic(ctx, RH*6, 1);

		// nk_labelf_colored_wrap(ctx, nk_rgb(255, 255, 255), "Long Text: %.10s\n", buf_sys);
  //指定字符实现换行
  //  char **strs = split_string(buf_sys, "\n");
    
    // 使用 strs[0] 来作为输入字符串
    nk_edit_string(ctx, NK_EDIT_BOX|NK_EDIT_MULTILINE, buf_sys, &len1, sizeof(buf_sys), nk_filter_default);

		// 绘制进度条
		nk_layout_row_dynamic(ctx, RH / 15, 1);
		
		nk_progress(ctx, &len1, MBX, NK_FIXED);

		/*设置窗口*/

		nk_layout_row_dynamic(ctx, RH / 1.5, 1);
		if (nk_button_symbol_text(ctx, 10, "发送", 6, NK_TEXT_ALIGN_CENTERED))
		{
			if (selected_item == 0) //gpt4
			{
				// 创建线程，不需要传递参数
				thread_function_gpt_x();
			}
			if (selected_item == 1) //gpt3.5
			{
                thread_function_gpt_sdw();
			}

			if (selected_item == 2) //gpt3
			{
	           thread_function_gpt_s();
		}
        }
		// 绘制进度条
		nk_layout_row_dynamic(ctx, RH / 15, 1);
		nk_progress(ctx, &len1, MBX, NK_FIXED);

		// 显示当前进度值
		char progress_text[16];
		sprintf(progress_text, "%d%%", len1);
		nk_label(ctx, progress_text, NK_TEXT_ALIGN_CENTERED);
	}

	nk_end(ctx);

}
