#pragma once

#include "core.h"
#include "utils.h"

#define AL_LIBTYPE_STATIC // must define this before including the audio library (openal), or else face the linking errors
#include <AL/al.h>
#include <AL/alc.h>
#include <AudioFile.h>

#if defined(SOUND_LIB_DEBUGGER)

// al debugger

void clear_al_error();
const char* get_al_error_enum_readable(ALenum error_code);
bool al_error(const char* conditon_name, const char* file, uint32_t line);

// alc debugger

void clear_alc_error(ALCdevice* device);
const char* get_alc_error_enum_readable(ALCenum error_code);
bool alc_error(ALCdevice* device, const char* conditon_name, const char* file, uint32_t line);

#define alCall(alFunction) clear_al_error(); alFunction; if (al_error(#alFunction, util::get_file_name_from_path(__FILE__), __LINE__)) { DEBUG_BREAK(); }
//#define alcCall(device, alcFunction) clear_alc_error(device); alcFunction; if(alc_error(device, #alcFunction, __FILE__, __LINE__)) { DEBUG_BREAK(); }

#else
#define alCall(alFunction) alFunction
#endif