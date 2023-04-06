#include "dfhooks.h"

#include <string>

#if _WIN32
#   include <windows.h>
#else
#   include <dlfcn.h>
#   if __APPLE__
const std::string lib_ext = ".dylib";
#   else
const std::string lib_ext = ".so";
#   endif
#endif

const std::string g_lib_name = "dfhooks";
void* g_lib_handle = nullptr;

typedef void (*dfhooks_init_fn)();
dfhooks_init_fn g_dfhooks_init = nullptr;

typedef void (*dfhooks_shutdown_fn)();
dfhooks_shutdown_fn g_dfhooks_shutdown = nullptr;

typedef void (*dfhooks_update_fn)();
dfhooks_update_fn g_dfhooks_update = nullptr;

typedef void (*dfhooks_prerender_fn)();
dfhooks_prerender_fn g_dfhooks_prerender = nullptr;

typedef bool (*dfhooks_sdl_event_fn)(void* event);
dfhooks_sdl_event_fn g_dfhooks_sdl_event = nullptr;

typedef bool (*dfhooks_ncurses_key_fn)(int key);
dfhooks_ncurses_key_fn g_dfhooks_ncurses_key = nullptr;

static void* open_library(const std::string& lib_name) {
#if _WIN32
    return LoadLibrary((lib_name + ".dll").c_str());
#else
    return dlopen(("lib" + lib_name + lib_ext).c_str(), RTLD_LAZY);
#endif
}

static void close_lib(void* handle) {
    if (!handle) {
        return;
    }
#if _WIN32
    FreeLibrary((HMODULE)handle);
#else
    dlclose(handle);
#endif
}

static void* load_sym(void* handle, const char* sym) {
    if (!handle) {
        return nullptr;
    }
#if _WIN32
    return (void*)GetProcAddress((HMODULE)handle, sym);
#else
    return dlsym(handle, sym);
#endif
}

static void init_fns(void* handle) {
    g_dfhooks_init = (dfhooks_init_fn)load_sym(handle, "dfhooks_init");
    g_dfhooks_shutdown = (dfhooks_shutdown_fn)load_sym(handle, "dfhooks_shutdown");
    g_dfhooks_update = (dfhooks_update_fn)load_sym(handle, "dfhooks_update");
    g_dfhooks_prerender = (dfhooks_prerender_fn)load_sym(handle, "dfhooks_prerender");
    g_dfhooks_sdl_event = (dfhooks_sdl_event_fn)load_sym(handle, "dfhooks_sdl_event");
    g_dfhooks_ncurses_key = (dfhooks_ncurses_key_fn)load_sym(handle, "dfhooks_ncurses_key");
}

void hooks_init() {
    g_lib_handle = open_library(g_lib_name);
    init_fns(g_lib_handle);

    if (g_dfhooks_init)
        g_dfhooks_init();
}

void hooks_shutdown() {
    if (g_dfhooks_shutdown)
        g_dfhooks_shutdown();

    init_fns(nullptr);
    close_lib(g_lib_handle);
    g_lib_handle = nullptr;
}

void hooks_update() {
    if (g_dfhooks_update)
        g_dfhooks_update();
}

void hooks_prerender() {
    if (g_dfhooks_prerender)
        g_dfhooks_prerender();
}

bool hooks_sdl_event(void* event) {
    if (g_dfhooks_sdl_event)
        return g_dfhooks_sdl_event(event);
    return false;
}

bool hooks_ncurses_key(int key) {
    if (g_dfhooks_ncurses_key)
        return g_dfhooks_ncurses_key(key);
    return false;
}