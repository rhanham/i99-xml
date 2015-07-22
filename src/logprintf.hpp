#pragma once

typedef void(*logprintf_t)(const char *format, ...);
extern logprintf_t logprintf;
