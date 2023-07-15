#ifndef DF_GSRC_GRAPHICS_DEFINITIONS
#define DF_GSRC_GRAPHICS_DEFINITIONS

//************************** WIDE SCREENS
#define MAX_GRID_X 768
#define MAX_GRID_Y 256

#define MIN_GRID_X 114
#define MIN_GRID_Y 46

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef ARRSZ
#define ARRSZ(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif
#ifndef CLAMP
#define CLAMP(x, a, b) MIN(MAX((x), (a)), (b))
#endif

// GL error macro
extern int glerrorcount;

#ifdef DEBUG
#define printGLError()                                                         \
  do {                                                                         \
    GLenum err;                                                                \
    do {                                                                       \
      err = glGetError();                                                      \
      if (err && glerrorcount < 40) {                                          \
        printf("GL error: 0x%x in %s:%d\n", err, __FILE__, __LINE__);          \
        glerrorcount++;                                                        \
      }                                                                        \
    } while (err);                                                             \
  } while (0);
#define deputs(str) puts(str)
#else
#define printGLError()
#define deputs(str)
#endif

#endif // DF_GSRC_GRAPHICS_DEFINITIONS
