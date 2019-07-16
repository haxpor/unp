/*
====================
error.h should not be included individually.
Just include unp.h instead.
====================
*/

#ifndef __ERROR_H_
#define __ERROR_H_

#include "unp_wrapped.h"
#include <cstdarg>       /* ANSI C header file */
#include <syslog.h>       /* for syslog() */

namespace unp {
    /* set nonzero by daemon_init() in chapter 13*/
    extern              int daemonProc;

    /**
     * Log with fmt text, then return.
     *
     * \param fmt string to print into stderr
     * \param ... variadic parameters
     */
    void                ErrorRet( const char *fmt, ... );

    /**
     * Log with fmt text, then call exit(1).
     *
     * \param fmt string to print into stderr
     * \param ... variadic parameters
     */
    void                ErrorSys( const char *fmt, ... );

    /**
     * Log with fmt text, then call abort().
     * Thus this means it will do core dump, then terminate.
     *
     * \param fmt string to print into stderr
     * \param ... variadic paremeters
     */
    void                ErrorDump( const char *fmt, ... );

    /**
     * Log with fmt text for error unrelated to system call, then return.
     *
     * \param fmt string to print into stderr
     * \param ... variadic parameters
     */
    void                ErrorMsg( const char *fmt, ... );

    /**
     * Log with fmt text for error unrelated to system call, then call exit(1).
     *
     * \param fmt string to print into stderr
     * \param ... variadic parameters
     */
    void                ErrorQuit( const char *fmt, ... );

    /**
     * Work function for error handling.
     *
     * Technical Note:
     *  Make this function `static` to make it thread-safe. It will be available on each translation unit.
     *
     * \param errnoFlag set to 1 to indicate it related to system call, otherwise non-related system call
     * \param level error log level
     * \param fmt text to log via syslog()
     * \param ap variable list
     */
    static void         ErrorDoIt( int errnoFlag, int level, const char *fmt, va_list ap );
};

inline void unp::ErrorRet( const char *fmt, ... ) {
    va_list ap;

    va_start( ap, fmt );
    unp::ErrorDoIt( 1, LOG_ERR, fmt, ap );
    va_end( ap );
}

inline void unp::ErrorSys( const char *fmt, ... ) {
    va_list ap;

    va_start( ap, fmt );
    unp::ErrorDoIt( 1, LOG_ERR, fmt, ap );
    va_end(ap);
    exit( 1 );
}

inline void unp::ErrorDump( const char *fmt, ... ) {
    va_list ap;

    va_start( ap, fmt );
    unp::ErrorDoIt( 1, LOG_ERR, fmt, ap );
    va_end( ap );
    abort();            // dump core and terminate
    exit(1);            // shouldn't get here
}

inline void unp::ErrorMsg( const char *fmt, ... ) {
    va_list ap;

    va_start( ap, fmt );
    unp::ErrorDoIt( 0, LOG_ERR, fmt, ap );
    va_end( ap );
}

inline void unp::ErrorQuit( const char *fmt, ... ) {
    va_list ap;

    va_start( ap, fmt );
    unp::ErrorDoIt( 0, LOG_ERR, fmt, ap );
    va_end( ap );
    exit( 1 );
}

inline void unp::ErrorDoIt( int errnoFlag, int level, const char *fmt, va_list ap ) {
    int errnoSave, n;
    char buf[MAXLINE + 1];

    errnoSave = errno;
    std::vsnprintf( buf, MAXLINE, fmt, ap );     // don't make sense to still check if snprintf is available

    n = strlen( buf );
    // get error string if it related to system call error
    if ( errnoFlag ) {
        std::snprintf( buf + n, MAXLINE - n + 1, ": %s", strerror( errnoSave ) );
    }
    std::strncat( buf, "\n", 1 );

    if ( daemonProc ) {
        vsyslog( level, buf, ap );
    } else {
        std::fflush( stdout );
        std::fputs( buf, stderr );
        std::fflush( stderr );
    }
}

#endif
