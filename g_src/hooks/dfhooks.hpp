#ifndef DF_GSRC_HOOKS_DFHOOKS_HPP
#define DF_GSRC_HOOKS_DFHOOKS_HPP

namespace SDL {
    // so we don't need to actually include SDL_events.h
    union Event;
}

// called before main event loop starts
void hooks_init();

// called after main event loops exits
void hooks_shutdown();

// called in the main event loop
void hooks_update();

// called just before adding the macro recording/playback overlay
void hooks_prerender();

// called for each SDL event, if true is returned, then the event has been
// consumed and further processing shouldn't happen
bool hooks_sdl_event(void* event);

// called for each utf-8 char read from the ncurses input
// key is positive for ncurses keys and negative for everything else.
// If true is returned then the key has been consumed and further
// processing shouldn't happen
bool hooks_ncurses_key(int key);

#endif // DF_GSRC_HOOKS_DFHOOKS_HPP