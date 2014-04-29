#ifndef DETECTION_UTILS
#define APP_DEBUG 1

#ifdef APP_DEBUG
#define DEBUGPRINT(...) \
    fprintf(stdout, "*** DEBUG *** "); \
    fprintf(stdout, __VA_ARGS__);
#define PRNTSTR(val,name) \
    printf("%s = %s\n", name, val);
#else /* !APP_DEBUG */
#define DEBUGPRINT(...)
#define PRNTSTR(...)
#endif /* !APP_DEBUG */
#endif /* !DETECTION_UTILS */
