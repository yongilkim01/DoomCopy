#pragma once

#define USEDLL

#ifdef USEDLL
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API 
#endif