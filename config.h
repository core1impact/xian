#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG 0
#define VERSION "1.0"

#define TRANSPORT CURL_TRANSPORT
#define CURL_TRANSPORT 1

#define START_SLEEP 10
#define RETRY_CONNECT 80 // 180

#define CURL_BLOCK_SIZE_MAX 1000000
#define CURL_BUFFER_SIZE_MAX 250000000

#define STDOUT_BLOCK_SIZE 1000000
#define STDOUT_MAX_BLOCK_SIZE 250000000

#if DEBUG == 1
#define G_STR(var) extern char *var
#else
#define G_STR(var) extern char var[]
#endif

#endif
