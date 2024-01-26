#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "curl/curl.h"
#include "cjson/cJSON.h"
#include "cjson/cJSON.c"

#include <SDL2/SDL.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GLES2_IMPLEMENTATION

#include "nuklear/nuklear.h"
#include "nuklear/nuklear_sdl_gles2.h"
#include "nuklear/style.h"
//100mb
#define MAX 10240
#define MBX 10240
#define MCX 10240
CURL *curl;

int running = nk_true;
SDL_Window *win;
int w, h;

// 加载音效
void playSound(const char *filePath);
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
char myapi_key[] = "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";
//6
//char myapi_key[] = "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";

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
void my_insert_newline(char *text, int line_length) {

    setlocale(LC_ALL, "");  // 设置本地化，以正确处理多字节字符
    size_t len2 = strlen(text);
    wchar_t wtext[len2+1];
    mbstowcs(wtext, text, len2);  // 将多字节字符串转换为宽字符字符串
    wtext[len2] = L'\0';  // 确保字符串以null字符结尾

    int count = 0;
    for (int i = 0; wtext[i] != L'\0'; i++) {
        count++;
        if (count == line_length) {
            count = 0;
            wtext[i] = L'\n';
        }
    }

    wcstombs(text, wtext, len2+1);  // 将宽字符字符串转换回多字节字符串
}

//向后插入字符
int appendToBuffer( char *text)
{
	
	int text_length = strlen(text);
	if (len1 + text_length < MBX - 1)
	{
        my_insert_newline(text,MAX_LINE_LENGTH);
		len1 += snprintf(buf_sys + len1, MBX - len1, "User:\n%s\nAI: %s\n", buf_user, text);
	}

	len = 0;
	memset(buf_user, 0, sizeof(buf_user));

	const char *soundFilePath = "resources/ball.wav";
	playSound(soundFilePath);
	return len1;
}

// 线程函数
int thread_function_gpt_x(void *data)
{
	const char *endpoint = "https://api.openai.com/v1/chat/completions";

	const char *model = "gpt-4";

	//buf_user[strcspn(buf_user, "\n")] = '\0';

	makeRequestb_b(curl, endpoint, myapi_key, buf_user, model);
	return 0;
}

// 线程函数
int thread_function_gpt_sdw(void *data)
{
	const char *endpoint = "https://api.openai.com/v1/chat/completions";

	const char *model = "gpt-3.5-turbo";

	//buf_user[strcspn(buf_user, "\n")] = '\0';

	makeRequestb_b(curl, endpoint, myapi_key, buf_user, model);
	return 0;
}

// 线程函数
int thread_function_gpt_s(void *data)
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
	cJSON *stop_array = cJSON_CreateArray();
	cJSON_AddItemToArray(stop_array, cJSON_CreateString("##STOP##"));
	cJSON_AddItemToArray(stop_array, cJSON_CreateString("##END##"));
	cJSON_AddItemToObject(json, "stop", stop_array);

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
	cJSON_AddStringToObject(root, "model", "text-davinci-003");

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
		// printf("Asst: %s\n,",chunk.memory);
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
static void
MainLoop(void *loopArg)
{
	struct nk_context *ctx = (struct nk_context *)loopArg;
	/* Input */
	SDL_Event evt;
	nk_input_begin(ctx);
	while (SDL_PollEvent(&evt))
	{
		if (evt.type == SDL_QUIT)
			running = nk_false;
		nk_sdl_handle_event(&evt);
	}
	nk_input_end(ctx);

	int RH = h / 10;

	if (nk_begin(ctx, "JNCKGUI_Android Chat Gpt", nk_rect(0, 0, w, h),
				 NK_WINDOW_CLOSABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_MINIMIZABLE |
					 NK_WINDOW_TITLE | NK_WINDOW_SCROLL_AUTO_HIDE | NK_WINDOW_NO_SCROLLBAR))
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

				nk_layout_row_dynamic(ctx, RH / 3, 2);
				nk_label(ctx, "max_tokens ", NK_TEXT_LEFT);
				nk_flags event = nk_edit_string_zero_terminated(ctx, flags, cymax_tokens, sizeof(cymax_tokens), nk_filter_float);
				if (event & NK_EDIT_ACTIVATED)
				{
					SDL_StartTextInput();
				}

				if (event & NK_EDIT_DEACTIVATED)
				{
					SDL_StopTextInput();
				}

				nk_layout_row_dynamic(ctx, RH / 3, 2);
				nk_label(ctx, "temperature ", NK_TEXT_LEFT);
				nk_flags eventa = nk_edit_string_zero_terminated(ctx, flags, cytemperature, sizeof(cytemperature), nk_filter_float);
				if (eventa & NK_EDIT_ACTIVATED)
				{
					SDL_StartTextInput();
				}

				if (eventa & NK_EDIT_DEACTIVATED)
				{
					SDL_StopTextInput();
				}

				nk_layout_row_dynamic(ctx, RH / 3, 2);
				nk_label(ctx, "presence_penalty ", NK_TEXT_LEFT);
				nk_flags eventb = nk_edit_string_zero_terminated(ctx, flags, cypresence_penalty, sizeof(cypresence_penalty), nk_filter_float);
				if (eventb & NK_EDIT_ACTIVATED)
				{
					SDL_StartTextInput();
				}

				if (eventb & NK_EDIT_DEACTIVATED)
				{
					SDL_StopTextInput();
				}

				nk_layout_row_dynamic(ctx, RH / 3, 2);
				nk_label(ctx, "frequency_penalty ", NK_TEXT_LEFT);
				nk_flags eventc = nk_edit_string_zero_terminated(ctx, flags, cyfrequency_penalty, sizeof(cyfrequency_penalty), nk_filter_float);
				if (eventc & NK_EDIT_ACTIVATED)
				{
					SDL_StartTextInput();
				}

				if (eventc & NK_EDIT_DEACTIVATED)
				{
					SDL_StopTextInput();
				}

				nk_layout_row_dynamic(ctx, RH / 3, 2);
				nk_label(ctx, "OPENAI API_KEY ", NK_TEXT_LEFT);
				nk_flags eventd = nk_edit_string(ctx, flags, myapi_key, &myapi_len, sizeof(myapi_key), nk_filter_default);
				if (eventd & NK_EDIT_ACTIVATED)
				{
					SDL_StartTextInput();
				}

				if (eventd & NK_EDIT_DEACTIVATED)
				{
					SDL_StopTextInput();
				}

				nk_layout_row_dynamic(ctx, RH / 3, 2);
				nk_label(ctx, "SOFT_KEY ", NK_TEXT_LEFT);
				nk_flags evente = nk_edit_string(ctx, flags, mysoft_key, &mysoft_len, sizeof(mysoft_key), nk_filter_default);
				if (evente & NK_EDIT_ACTIVATED)
				{
					SDL_StartTextInput();
				}

				if (evente & NK_EDIT_DEACTIVATED)
				{
					SDL_StopTextInput();
				}

				nk_layout_row_dynamic(ctx, RH / 3, 2);
				nk_label(ctx, "PROXY_IP ", NK_TEXT_LEFT);
				nk_flags eventf = nk_edit_string(ctx, flags, my_ip, &myip_len, sizeof(my_ip), nk_filter_default);
				if (eventf & NK_EDIT_ACTIVATED)
				{
					SDL_StartTextInput();
				}

				if (eventf & NK_EDIT_DEACTIVATED)
				{
					SDL_StopTextInput();
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
			// 复制文本到剪贴板
			SDL_SetClipboardText(buf_sys);
		}

		if (nk_button_label(ctx, "粘贴"))
		{
			// 从剪贴板粘贴文本
			const char *clipboard_text = SDL_GetClipboardText();
			if (clipboard_text)
			{
				len = snprintf(buf_user, sizeof(buf_user), "%s", clipboard_text);

				clipboard_text = 0;
				// strncpy(buf_user, clipboard_text, MAX);
				//	SDL_free(clipboard_text);
			}
		}

		/* 函数：调整控件宽度NK布局行动态 */
		nk_layout_row_dynamic(ctx, RH, 1);

		/*创建可编辑的文本输入框*/
		nk_flags input_text =
			nk_edit_string(ctx, NK_EDIT_BOX, buf_user, &len, sizeof(buf_user), nk_filter_default);

		if (input_text & NK_EDIT_ACTIVATED)
		{
			SDL_StartTextInput();
		}

		if (input_text & NK_EDIT_DEACTIVATED)
		{
			SDL_StopTextInput();
		}

		/*下拉框模型*/
		static int selected_item = 2;
		static const char *items[] = {"OPENAI GPT-4 (VIP)", "OPENAI GPT-3.5", "OPENAI GPT-3"};
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

		static const char *items1[] = {"GPT-3单次对话", "GPT-3理解上下文", "GPT-3理解全文"};

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
		nk_layout_row_dynamic(ctx, RH * 6.8, 1);

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
				SDL_Thread *thread = SDL_CreateThread(thread_function_gpt_x, "My Thread", NULL);
				if (!thread)
				{
					fprintf(stderr, "SDL_CreateThread Error: %s\n", SDL_GetError());
				}
				else
				{
					// 等待线程结束
					int thread_result;
					SDL_WaitThread(thread, &thread_result);
				}
			}
			if (selected_item == 1) //gpt3.5
			{
				// 创建线程，不需要传递参数
				SDL_Thread *thread = SDL_CreateThread(thread_function_gpt_sdw, "My Thread", NULL);
				if (!thread)
				{
					fprintf(stderr, "SDL_CreateThread Error: %s\n", SDL_GetError());
				}
				else
				{
					// 等待线程结束
					int thread_result;
					SDL_WaitThread(thread, &thread_result);
				}
			}

			if (selected_item == 2) //gpt3
			{
				// 创建线程，不需要传递参数
				SDL_Thread *thread1 = SDL_CreateThread(thread_function_gpt_s, "My Thread", NULL);
				if (!thread1)
				{
					fprintf(stderr, "SDL_CreateThread Error: %s\n", SDL_GetError());
				}
				else
				{
					// 等待线程结束
					int thread_result;
					SDL_WaitThread(thread1, &thread_result);
				}
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

	float bg[4];
	nk_color_fv(bg, nk_rgb(225, 225, 225)); // 设置背景颜色

	glViewport(0, 0, w, h);					  // 设置OpenGL视口
	glClear(GL_COLOR_BUFFER_BIT);			  // 清除颜色缓冲区
	glClearColor(bg[0], bg[1], bg[2], bg[3]); // 设置清除颜色

	nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY); // 渲染GUI
	SDL_GL_SwapWindow(win);													   // 交换窗口缓冲区
}

int main(int argc, char *argv[])
{
	curl = curl_easy_init();
	if (!curl)
	{
		//printf("curl_easy_init() failed\n");
		return 0;
	}

	curl_global_init(CURL_GLOBAL_DEFAULT);

	struct nk_context *ctx; // 创建nk_context对象

	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8); // 设置alpha位深度为8

	win = SDL_CreateWindow("Demo",
						   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
						   WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

	// 创建OpenGL上下文
	SDL_GLContext glContext = SDL_GL_CreateContext(win);
	if (!glContext)
	{
		// 处理OpenGL上下文创建失败的情况
	}

	float dpi, bpi, cpi;
	SDL_GetWindowSize(win, &w, &h);						  // 获取窗口的宽度和高度
	int index = SDL_GetWindowDisplayIndex(win);			  // 获取窗口所在的显示器的索引
	int ret = SDL_GetDisplayDPI(index, &bpi, &dpi, &cpi); // 获取显示器的DPI值
	float mult;
	if (ret < 0)
	{
		// 获取DPI值失败，设置一个合理的默认值，比如1.0
		mult = 1.0f;
	}
	else
	{
		// 计算像素大小的倍数
		mult = bpi / 100;
	}

	// 前景图片显示
	SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // 开启硬件加速
	SDL_Texture *texBack = IMG_LoadTexture(renderer, "resources/image.png");
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texBack, NULL, NULL);
	SDL_RenderPresent(renderer);
	sleep(1);

	/* OpenGL setup */
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	ctx = nk_sdl_init(win);
	{
		struct nk_font_atlas *atlas;
		int font_pixel_height = 28;
		// 初始化字体图集
		nk_sdl_font_stash_begin(&atlas);

		// 加载定义字库
		struct nk_font_config config = nk_font_config(font_pixel_height);
		config.merge_mode = nk_false;
		config.oversample_h = 1;
		config.oversample_v = 1;
		config.range = nk_font_chinese_glyph_ranges(); // 使用自带编码支持中文

		// 从文件中添加字体到字体图集
		struct nk_font *font =
			nk_font_atlas_add_from_file(atlas, "/system/fonts/DroidSansFallback.ttf", font_pixel_height,
										&config);

		// 结束字体图集的构建，并将修改后的字体纹理保存到font->texture中
		nk_sdl_font_stash_end();
		// 将字体高度乘以2，放大字体
		font->handle.height *= 1.5f;

		// 设置Nuklear上下文中的字体为修改后的字体
		nk_style_set_font(ctx, &font->handle);

		// 加载光标
		nk_style_load_all_cursors(ctx, atlas->cursors);
		nk_font_atlas_end(atlas, font->texture, NULL);
	}

	/* style.h主题 */
	// ///////////////////
	set_style(ctx, 4);
	// //////////////////

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
	emscripten_set_main_loop_arg(MainLoop, (void *)ctx, 0, nk_true);
#else
	while (running)

		MainLoop((void *)ctx);

#endif

	nk_sdl_shutdown(); // 关闭nk_sdl插件

	curl_easy_cleanup(curl);		 //断开全局
	curl_global_cleanup();			 // 清理CURL库的全局
	SDL_GL_DeleteContext(glContext); //删除OpenGL上下文
	SDL_DestroyTexture(texBack);	 // 销毁纹理对象
	SDL_DestroyWindow(win);			 // 销毁窗口对象
	SDL_Quit();						 // 退出SDL库
	return 0;						 // 退出程序
}

/*加载音效*/
void playSound(const char *filePath)
{
	// 初始化SDL_mixer
	Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_Chunk *sound = 0;
	// 加载音频文件
	sound = Mix_LoadWAV(filePath);
	if (sound == NULL)
	{
		appendToBuffer(Mix_GetError());

		return;
	}

	// 播放音频
	Mix_PlayChannel(-1, sound, 0);

	// 等待音频播放完成
	while (Mix_Playing(-1) != 0)
	{
		SDL_Delay(10);
	}

	// 释放音频资源
	Mix_FreeChunk(sound);

	// 关闭SDL_mixer
	Mix_CloseAudio();
	Mix_Quit();
}