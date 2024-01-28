
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include "curl/curl.h"
#include "cjson/cJSON.h"
#include "cjson/cJSON.c"
char *reply = "我是助手";
// char *reply ="我是系统";
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

// 定义回调函数来处理CURL响应
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

// Function to handle CURL request
int makeRequest(CURL * curl, const char *endpoint, const char *api_key, const char *input,
				 const char *model)
{
	char *reply = "我是助手";
	struct MemoryStruct chunk;
	initMemoryStruct(&chunk);

	struct curl_slist *headers = NULL;
	char auth_header[2048];
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
		//appendToBuffer("创建 assistant_message 对象时出错\n");

		return 0;
	}
	cJSON_AddStringToObject(assistant_message, "role", "assistant");
	cJSON_AddStringToObject(assistant_message, "content", reply);
	cJSON_AddItemToArray(cJSON_GetObjectItem(json, "messages"), assistant_message);

	// 用户角色消息
	cJSON *user_message = cJSON_CreateObject();
	if (user_message == NULL)
	{
		//appendToBuffer("创建 user_message 对象时出错\n");

		return 0;
	}
	cJSON_AddStringToObject(user_message, "role", "user");
	cJSON_AddStringToObject(user_message, "content", input);
	cJSON_AddItemToArray(cJSON_GetObjectItem(json, "messages"), user_message);
	cJSON_AddNumberToObject(json, "max_tokens", 2048);
	cJSON_AddNumberToObject(json, "temperature", 0.8);
	cJSON_AddNumberToObject(json, "n", 1);
	cJSON_AddNumberToObject(json, "top_p", 1);
	cJSON_AddNumberToObject(json, "presence_penalty", 0);
	cJSON_AddNumberToObject(json, "frequency_penalty", 0);
	cJSON_AddStringToObject(json, "stream", 0 /* false */ );
	cJSON_AddStringToObject(json, "logprobs", 0);
  
	//	cJSON_AddStringToObject(json, "stop", "END");
	//添加数组类型的参数
	cJSON *stop_array = cJSON_CreateArray();
	cJSON_AddItemToArray(stop_array, cJSON_CreateString("##STOP##"));
	cJSON_AddItemToArray(stop_array, cJSON_CreateString("##END##"));
	cJSON_AddItemToObject(json, "stop", stop_array);

	// 将 JSON 对象转换为字符串best_of
	char *request_data = cJSON_Print(json);
	if (request_data == NULL)
	{
		//appendToBuffer("Error converting JSON to string\n");

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
		printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
	}
	else
	{
		// 解析JSON响应
		cJSON *root = cJSON_Parse(chunk.memory);
		if (json)
		{
									// 助手打印
						//printf("system:%s\n",chunk.memory);
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
						// 上下文
						reply = content->valuestring;

						size_t reply_length = strlen(reply);
						// 助手打印
						printf("system:\n");
						for (size_t i = 0; i < reply_length; i++)
						{
							putchar(reply[i]);
							fflush(stdout);
							_sleep(10);
						}
						printf("\n");
					}
				}
			}
		}
			else
			{
				//appendToBuffer(chunk.memory);
			}
			cJSON_Delete(root);
			cJSON_Delete(json);
	}

	free(chunk.memory);
	curl_slist_free_all(headers);
	free(request_data);
}

int main()
{
	system("clear");			// 若在Windows系统上编译运
	CURL *curl = curl_easy_init();
	if (!curl)
	{
		printf("Failed to initialize CURL\n");
		return 1;
	}
	// Initialize CURL
	curl_global_init(CURL_GLOBAL_DEFAULT);

	const char *endpoint = "https://api.openai.com/v1/chat/completions";
	const char api_key[] = "sk-r";
	const char *model = "gpt-3.5-turbo";	// "gpt-3-turbo";
	char input[4096];

	while (1)
	{

		printf("uesr:\n");
		fgets(input, sizeof(input), stdin);
		// input[strcspn(input, "\n")] = '\0';

		makeRequest(curl, endpoint, api_key, input, model);
	}

	curl_easy_cleanup(curl);
	curl_global_cleanup();

	return 0;
}
