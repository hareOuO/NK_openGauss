/* -------------------------------------------------------------------------
 *
 * pgarch.h
 *	  Exports from postmaster/pgarch.c.
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/postmaster/pgarch.h
 *
 * -------------------------------------------------------------------------
 */
#ifndef _PGARCH_H
#define _PGARCH_H

/* ----------
 * Functions called from postmaster
 * ----------
 */
extern ThreadId pgarch_start(void);
extern Datum gs_get_archive_status(PG_FUNCTION_ARGS);
#ifdef EXEC_BACKEND
extern void PgArchiverMain();
#endif

#endif /* _PGARCH_H */
