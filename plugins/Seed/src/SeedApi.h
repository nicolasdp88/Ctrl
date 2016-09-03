#pragma once

#if SEED_AS_DLL
#define SEED_API __declspec(dllexport)
#else
#define SEED_API __declspec(dllimport)
#endif