#pragma once

/**
* @brief OS‚Ì’è‹`
*/
#if defined(_WIN64)// || defined(_WIN32)
#define PLATFORM_WINDOWS 1
#else
#error "No support platform."
#endif

#ifndef PLATFORM_WINDOWS
#define PLATFORM_WINDOWS 0
#endif