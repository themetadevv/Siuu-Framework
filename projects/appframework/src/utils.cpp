
#include "afpch.h"

#include "utils.h"

namespace util {
	const char* get_file_name_from_path(const char* path) {
		const char* file = path;
		for (const char* p = path; *p; ++p) {
			if (*p == '/' || *p == '\\')
				file = p + 1;
		}
		return file;
	}
}