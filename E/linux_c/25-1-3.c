#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct data {
  // 路径
  char *path;
  // 键数组
  char **key;
  // 值数组
  char **value;
  // 键值对数目
  int n;
} data;

int valid_path(const char *path) {
  // 不允许空路径
  if (!path || !strlen(path))
    return 0;

  // 路径中仅允许出现字母、数字、'.'、':'和'/'
  for (int i = 0; i < strlen(path); i++) {
    if ((path[i] >= 'a' && path[i] <= 'z') ||
        (path[i] >= 'A' && path[i] <= 'Z') ||
        (path[i] >= '0' && path[i] <= '9') || path[i] == '.' || path[i] == ':' ||
        path[i] == '/') {
      continue;
    }
    return 0;
  }
  return 1;
}

void free_data(data *data) {
  // 由于path就是malloc块的开始，所以释放path会释放整个url字符串
  if (data->path) {
    free(data->path);
    data->path = NULL;
  }
  if (data->key) {
    free(data->key);
    data->key = NULL;
  }
  if (data->value) {
    free(data->value);
    data->value = NULL;
  }
  data->n = 0;
}

int parse_data(char *url, data *data) {
  free_data(data);

  // 检查头
  if (strncmp(url, "http://", 7) && strncmp(url, "https://", 8))
    return -1;

  // 复制url到path，包含了malloc操作
  data->path = strdup(url);

  // 查找最后一个?
  char *q_mark = strrchr(data->path, '?');
  // 如果查到，分割路径和参数
  if (q_mark) {
    *q_mark = '\0';
  }
  // 检查路径合法性
  if (!valid_path(data->path)) {
    free_data(data);
    return -1;
  }

  // 若不存在参数，直接返回
  if (!q_mark || !strlen(q_mark + 1)) {
    return 0;
  }

  // 查询键值对个数，为"&"数量+1
  data->n++;
  for (char *c = q_mark+1; *c != '\0'; c++) {
    if (*c == '&') {
      data->n++;
    }
  }

  // 为key和value分配空间
  data->key = malloc(data->n * sizeof(char *));
  data->value = malloc(data->n * sizeof(char *));

  // 开始分割
  char *c = q_mark;
  int i = 0;
  // 找不到下一个"&"，退出循环
  while (c) {
    *c = '\0';
    c++;
    data->key[i] = c;
    c = strchr(c, '=');
    // 没有找到"="，不合法！
    if (!c) {
      free_data(data);
      return -1;
    }
    *c = '\0';
    c++;
    data->value[i] = c;
    i++;
    // 寻找下一个"&"
    c = strchr(c, '&');
  }
  
  // 检查是否找到了足够多的键值对
  if (i != data->n) {
    free_data(data);
    return -1;
  }
  return 0;
}

int main(int argc, char *argv[])
{
  char str[] = "http://www.google.cn/search?complete=1&hl=zh-CN&ie=GB2312&q=linux&meta=";
  // char str[] = "http://www.baidu.com/s?wd=linux&cl=3";
  data *data = malloc(sizeof(struct data));
  parse_data(str, data);
  printf("path: %s\n", data->path);
  for (int i = 0; i < data->n; i++) {
    printf("key: %s, value: %s\n", data->key[i], data->value[i]);
  }
  free_data(data);
  free(data);
  return EXIT_SUCCESS;
}
