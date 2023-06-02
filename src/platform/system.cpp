#include <platform/system.hpp>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __unix__
#include <time.h>
#include <errno.h>
#endif

namespace artha {

void Sleep(unsigned ms) {

#ifdef _WIN32
	::Sleep(ms);

#elif __unix__
	struct timespec ts;
	int res;

	if (ms < 0) {
		errno = EINVAL;
		return;
	}

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000;

	do {
		res = nanosleep(&ts, &ts);
	} while (res && errno == EINTR);

#else
#error Sleep function is not implemented for your system!	

#endif

}

}
