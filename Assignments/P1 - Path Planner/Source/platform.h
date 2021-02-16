#pragma once

// Used to export DLL functions
#ifdef _WIN64
#define DLLEXPORT __declspec(dllexport)
#endif
#ifndef _WIN64
#define DLLEXPORT
#endif
