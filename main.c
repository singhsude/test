/*	$NetBSD: strlcpy.c,v 1.3 2007/06/04 18:19:27 christos Exp $	*/
/*	$OpenBSD: strlcpy.c,v 1.7 2003/04/12 21:56:39 millert Exp $	*/

/*
 * Copyright (c) 1998 Todd C. Miller <Todd.Miller@courtesan.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND TODD C. MILLER DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL TODD C. MILLER BE LIABLE
 * FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#if !defined(_KERNEL) && !defined(_STANDALONE)
#if HAVE_NBTOOL_CONFIG_H
#include "nbtool_config.h"
#endif

#include <sys/cdefs.h>
#if defined(LIBC_SCCS) && !defined(lint)
__RCSID("$NetBSD: strlcpy.c,v 1.3 2007/06/04 18:19:27 christos Exp $");
#endif /* LIBC_SCCS and not lint */

#ifdef _LIBC
#include "namespace.h"
#endif
#include <sys/types.h>
#include <assert.h>
#include <string.h>

#ifdef _LIBC
# ifdef __weak_alias
__weak_alias(strlcpy, _strlcpy)
# endif
#endif
#else
#include <lib/libkern/libkern.h>
#endif /* !_KERNEL && !_STANDALONE */


#if !HAVE_STRLCPY
/*
 * Copy src to string dst of size siz.  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 */
size_t
strlcpy(char *dst, const char *src, size_t siz)
{
	char *d = dst;
	const char *s = src;
	size_t n = siz;

	_DIAGASSERT(dst != NULL);
	_DIAGASSERT(src != NULL);

	/* Copy as many bytes as will fit */
	if (n != 0 && --n != 0) {
		do {
			if ((*d++ = *s++) == 0)
				break;
		} while (--n != 0);
	}

	/* Not enough room in dst, add NUL and traverse rest of src */
	if (n == 0) {
		if (siz != 0)
			*d = '\0';		/* NUL-terminate dst */
		while (*s++)
			;
	}

	return(s - src - 1);	/* count does not include NUL */
}
#endif


#include <sys/cdefs.h>
#include <lib.h>
#include "namespace.h"
/*#include "extern.h"*/
#include <string.h>

/*
 * The sysctl(2) system call, handled by the MIB service.
 */
int
__sysctl(const int * name, unsigned int namelen, void * oldp, size_t * oldlenp,
	const void * newp, size_t newlen)
{
	message m;
	int r;

	memset(&m, 0, sizeof(m));
	m.m_lc_mib_sysctl.oldp = (vir_bytes)oldp;
	m.m_lc_mib_sysctl.oldlen = (oldlenp != NULL) ? *oldlenp : 0;
	m.m_lc_mib_sysctl.newp = (vir_bytes)newp;
	m.m_lc_mib_sysctl.newlen = newlen;
	m.m_lc_mib_sysctl.namelen = namelen;
	m.m_lc_mib_sysctl.namep = (vir_bytes)name;
	if (namelen <= CTL_SHORTNAME)
		memcpy(m.m_lc_mib_sysctl.name, name, sizeof(*name) * namelen);

	r = _syscall(MIB_PROC_NR, MIB_SYSCTL, &m);

	/*
	 * We copy the NetBSD behavior of replying with the old length also if
	 * the call failed, typically with ENOMEM.  This is undocumented
	 * behavior, but unfortunately relied on by sysctl(8) and other NetBSD
	 * userland code.  If the call failed at the IPC level, the resulting
	 * value will be garbage, but it should then not be used anyway.
	 */
	if (oldlenp != NULL)
		*oldlenp = m.m_mib_lc_sysctl.oldlen;

	return r;
}


/*	$NetBSD: main.c,v 1.233 2015/09/10 17:15:11 sjg Exp $	*/

/*
 * Copyright (c) 1988, 1989, 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Adam de Boor.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Copyright (c) 1989 by Berkeley Softworks
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Adam de Boor.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef MAKE_NATIVE
static char rcsid[] = "$NetBSD: main.c,v 1.233 2015/09/10 17:15:11 sjg Exp $";
#else
#include <sys/cdefs.h>
#ifndef lint
__COPYRIGHT("@(#) Copyright (c) 1988, 1989, 1990, 1993\
 The Regents of the University of California.  All rights reserved.");
#endif /* not lint */

#ifndef lint
#if 0
static char sccsid[] = "@(#)main.c	8.3 (Berkeley) 3/19/94";
#else
__RCSID("$NetBSD: main.c,v 1.233 2015/09/10 17:15:11 sjg Exp $");
#endif
#endif /* not lint */
#endif

/*-
 * main.c --
 *	The main file for this entire program. Exit routines etc
 *	reside here.
 *
 * Utility functions defined in this file:
 *	Main_ParseArgLine	Takes a line of arguments, breaks them and
 *				treats them as if they were given when first
 *				invoked. Used by the parse module to implement
 *				the .MFLAGS target.
 *
 *	Error			Print a tagged error message. The global
 *				MAKE variable must have been defined. This
 *				takes a format string and optional arguments
 *				for it.
 *
 *	Fatal			Print an error message and exit. Also takes
 *				a format string and arguments for it.
 *
 *	Punt			Aborts all jobs and exits with a message. Also
 *				takes a format string and arguments for it.
 *
 *	Finish			Finish things up by printing the number of
 *				errors which occurred, as passed to it, and
 *				exiting.
 */

#include <sys/cdefs.h>
#if defined(LIBC_SCCS) && !defined(lint)
#if 0
static char sccsid[] = "@(#)sysctl.c	8.2 (Berkeley) 1/4/94";
#else
__RCSID("$NetBSD: sysctl.c,v 1.35 2015/02/05 16:05:20 christos Exp $");
#endif
#endif /* LIBC_SCCS and not lint */

#include "namespace.h"
#include <sys/param.h>
#define __COMPAT_SYSCTL
#include <sys/sysctl.h>

#include <assert.h>
#include <errno.h>
#include <paths.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
/*#include "extern.h"*/

#ifdef __weak_alias
__weak_alias(sysctl,_sysctl)
#endif

/*
 * handles requests off the user subtree
 */
static int user_sysctl(const int *, u_int, void *, size_t *,
			const void *, size_t);

/*
 * copies out individual nodes taking target version into account
 */
static size_t __cvt_node_out(uint, const struct sysctlnode *, void **,
			     size_t *);

#include <stdlib.h>

int
sysctl(const int *name, unsigned int namelen,
	void *oldp, size_t *oldlenp,
	const void *newp, size_t newlen)
{
	size_t oldlen, savelen;
	int error;

	if (name[0] != CTL_USER)
		return (__sysctl(name, namelen, oldp, oldlenp,
				 newp, newlen));

	oldlen = (oldlenp == NULL) ? 0 : *oldlenp;
	savelen = oldlen;
	error = user_sysctl(name + 1, namelen - 1, oldp, &oldlen, newp, newlen);

	if (error != 0) {
		errno = error;
		return (-1);
	}

	if (oldlenp != NULL) {
		*oldlenp = oldlen;
		if (oldp != NULL && oldlen > savelen) {
			errno = ENOMEM;
			return (-1);
		}
	}

	return (0);
}

static int
user_sysctl(const int *name, unsigned int namelen,
	void *oldp, size_t *oldlenp,
	const void *newp, size_t newlen)
{
#define _INT(s, n, v, d) {					\
	.sysctl_flags = CTLFLAG_IMMEDIATE|CTLFLAG_PERMANENT|	\
			CTLTYPE_INT|SYSCTL_VERSION,		\
	.sysctl_size = sizeof(int),				\
	.sysctl_name = (s),					\
	.sysctl_num = (n),					\
	.sysctl_un.scu_idata = (v),				\
	.sysctl_desc = (d),					\
	}

	/*
	 * the nodes under the "user" node
	 */
	static const struct sysctlnode sysctl_usermib[] = {
#if defined(lint)
		/*
		 * lint doesn't like my initializers
		 */
		0
#else /* !lint */
		{
			.sysctl_flags = SYSCTL_VERSION|CTLFLAG_PERMANENT|
				CTLTYPE_STRING,
			.sysctl_size = sizeof(_PATH_STDPATH),
			.sysctl_name = "cs_path",
			.sysctl_num = USER_CS_PATH,
			.sysctl_data = __UNCONST(_PATH_STDPATH),
			.sysctl_desc = __UNCONST(
				"A value for the PATH environment variable "
				"that finds all the standard utilities"),
		},
		_INT("bc_base_max", USER_BC_BASE_MAX, BC_BASE_MAX,
		     "The maximum ibase/obase values in the bc(1) utility"),
		_INT("bc_dim_max", USER_BC_DIM_MAX, BC_DIM_MAX,
		     "The maximum array size in the bc(1) utility"),
		_INT("bc_scale_max", USER_BC_SCALE_MAX, BC_SCALE_MAX,
		     "The maximum scale value in the bc(1) utility"),
		_INT("bc_string_max", USER_BC_STRING_MAX, BC_STRING_MAX,
		     "The maximum string length in the bc(1) utility"),
		_INT("coll_weights_max", USER_COLL_WEIGHTS_MAX,
		     COLL_WEIGHTS_MAX, "The maximum number of weights that can "
		     "be assigned to any entry of the LC_COLLATE order keyword "
		     "in the locale definition file"),
		_INT("expr_nest_max", USER_EXPR_NEST_MAX, EXPR_NEST_MAX,
		     "The maximum number of expressions that can be nested "
		     "within parenthesis by the expr(1) utility"),
		_INT("line_max", USER_LINE_MAX, LINE_MAX, "The maximum length "
		     "in bytes of a text-processing utility's input line"),
		_INT("re_dup_max", USER_RE_DUP_MAX, RE_DUP_MAX, "The maximum "
		     "number of repeated occurrences of a regular expression "
		     "permitted when using interval notation"),
		_INT("posix2_version", USER_POSIX2_VERSION, _POSIX2_VERSION,
		     "The version of POSIX 1003.2 with which the system "
		     "attempts to comply"),
#ifdef _POSIX2_C_BIND
		_INT("posix2_c_bind", USER_POSIX2_C_BIND, 1,
		     "Whether the system's C-language development facilities "
		     "support the C-Language Bindings Option"),
#else
		_INT("posix2_c_bind", USER_POSIX2_C_BIND, 0,
		     "Whether the system's C-language development facilities "
		     "support the C-Language Bindings Option"),
#endif
#ifdef POSIX2_C_DEV
		_INT("posix2_c_dev", USER_POSIX2_C_DEV, 1,
		     "Whether the system supports the C-Language Development "
		     "Utilities Option"),
#else
		_INT("posix2_c_dev", USER_POSIX2_C_DEV, 0,
		     "Whether the system supports the C-Language Development "
		     "Utilities Option"),
#endif
#ifdef POSIX2_CHAR_TERM
		_INT("posix2_char_term", USER_POSIX2_CHAR_TERM, 1,
		     "Whether the system supports at least one terminal type "
		     "capable of all operations described in POSIX 1003.2"),
#else
		_INT("posix2_char_term", USER_POSIX2_CHAR_TERM, 0,
		     "Whether the system supports at least one terminal type "
		     "capable of all operations described in POSIX 1003.2"),
#endif
#ifdef POSIX2_FORT_DEV
		_INT("posix2_fort_dev", USER_POSIX2_FORT_DEV, 1,
		     "Whether the system supports the FORTRAN Development "
		     "Utilities Option"),
#else
		_INT("posix2_fort_dev", USER_POSIX2_FORT_DEV, 0,
		     "Whether the system supports the FORTRAN Development "
		     "Utilities Option"),
#endif
#ifdef POSIX2_FORT_RUN
		_INT("posix2_fort_run", USER_POSIX2_FORT_RUN, 1,
		     "Whether the system supports the FORTRAN Runtime "
		     "Utilities Option"),
#else
		_INT("posix2_fort_run", USER_POSIX2_FORT_RUN, 0,
		     "Whether the system supports the FORTRAN Runtime "
		     "Utilities Option"),
#endif
#ifdef POSIX2_LOCALEDEF
		_INT("posix2_localedef", USER_POSIX2_LOCALEDEF, 1,
		     "Whether the system supports the creation of locales"),
#else
		_INT("posix2_localedef", USER_POSIX2_LOCALEDEF, 0,
		     "Whether the system supports the creation of locales"),
#endif
#ifdef POSIX2_SW_DEV
		_INT("posix2_sw_dev", USER_POSIX2_SW_DEV, 1,
		     "Whether the system supports the Software Development "
		     "Utilities Option"),
#else
		_INT("posix2_sw_dev", USER_POSIX2_SW_DEV, 0,
		     "Whether the system supports the Software Development "
		     "Utilities Option"),
#endif
#ifdef POSIX2_UPE
		_INT("posix2_upe", USER_POSIX2_UPE, 1,
		     "Whether the system supports the User Portability "
		     "Utilities Option"),
#else
		_INT("posix2_upe", USER_POSIX2_UPE, 0,
		     "Whether the system supports the User Portability "
		     "Utilities Option"),
#endif
		_INT("stream_max", USER_STREAM_MAX, FOPEN_MAX,
		     "The minimum maximum number of streams that a process "
		     "may have open at any one time"),
		_INT("tzname_max", USER_TZNAME_MAX, NAME_MAX,
		     "The minimum maximum number of types supported for the "
		     "name of a timezone"),
		_INT("atexit_max", USER_ATEXIT_MAX, -1,
		     "The maximum number of functions that may be registered "
		     "with atexit(3)"),
#endif /* !lint */
	};
#undef _INT

	static const int clen = sizeof(sysctl_usermib) /
		sizeof(sysctl_usermib[0]);

	const struct sysctlnode *node;
	int ni;
	size_t l, sz;

	/*
	 * none of these nodes are writable and they're all terminal (for now)
	 */
	if (namelen != 1)
		return (EINVAL);

	l = *oldlenp;
	if (name[0] == CTL_QUERY) {
		uint v;
		node = newp;
		if (node == NULL)
			return (EINVAL);
		else if (SYSCTL_VERS(node->sysctl_flags) == SYSCTL_VERS_1 &&
			 newlen == sizeof(struct sysctlnode))
			v = SYSCTL_VERS_1;
		else
			return (EINVAL);

		sz = 0;
		for (ni = 0; ni < clen; ni++)
			sz += __cvt_node_out(v, &sysctl_usermib[ni], &oldp, &l);
		*oldlenp = sz;
		return (0);
	}

	if (name[0] == CTL_DESCRIBE) {
		/*
		 * XXX make sure this is larger than the largest
		 * "user" description
		 */
		char buf[192];
		struct sysctldesc *d1 = (void *)&buf[0], *d2 = oldp;
		size_t d;

		node = newp;
		if (node != NULL &&
		    (SYSCTL_VERS(node->sysctl_flags) < SYSCTL_VERS_1 ||
		     newlen != sizeof(struct sysctlnode)))
			return (EINVAL);

		sz = 0;
		for (ni = 0; ni < clen; ni++) {
			memset(&buf[0], 0, sizeof(buf));
			if (node != NULL &&
			    node->sysctl_num != sysctl_usermib[ni].sysctl_num)
				continue;
			d1->descr_num = sysctl_usermib[ni].sysctl_num;
			d1->descr_ver = sysctl_usermib[ni].sysctl_ver;
			if (sysctl_usermib[ni].sysctl_desc == NULL)
				d1->descr_len = 1;
			else {
				size_t dlen;
				(void)strlcpy(d1->descr_str,
					sysctl_usermib[ni].sysctl_desc,
					sizeof(buf) - sizeof(*d1));
				dlen = strlen(d1->descr_str) + 1;
				_DIAGASSERT(__type_fit(uint32_t, dlen));
				d1->descr_len = (uint32_t)dlen;
			}
			d = (size_t)__sysc_desc_adv(NULL, d1->descr_len);
			if (d2 != NULL)
				memcpy(d2, d1, d);
			sz += d;
			d2 = (struct sysctldesc *)(void *)((char *)d2 + d);
			if (node != NULL)
				break;
		}
		*oldlenp = sz;
		if (sz == 0 && node != NULL)
			return (ENOENT);
		return (0);

	}

	/*
	 * none of these nodes are writable
	 */
	if (newp != NULL || newlen != 0)
		return (EPERM);
	
	node = &sysctl_usermib[0];
	for (ni = 0; ni	< clen; ni++)
		if (name[0] == node[ni].sysctl_num)
			break;
	if (ni == clen)
		return (EOPNOTSUPP);

	node = &node[ni];
	if (node->sysctl_flags & CTLFLAG_IMMEDIATE) {
		switch (SYSCTL_TYPE(node->sysctl_flags)) {
		case CTLTYPE_INT:
			newp = &node->sysctl_idata;
			break;
		case CTLTYPE_QUAD:
			newp = &node->sysctl_qdata;
			break;
		default:
			return (EINVAL);
		}
	}
	else
		newp = node->sysctl_data;

	l = MIN(l, node->sysctl_size);
	if (oldp != NULL)
		memcpy(oldp, newp, l);
	*oldlenp = node->sysctl_size;

	return (0);
}

static size_t
__cvt_node_out(uint v, const struct sysctlnode *n, void **o, size_t *l)
{
	const void *src = n;
	size_t sz;

	switch (v) {
#if (SYSCTL_VERSION != SYSCTL_VERS_1)
#error __cvt_node_out: no support for SYSCTL_VERSION
#endif /* (SYSCTL_VERSION != SYSCTL_VERS_1) */

	case SYSCTL_VERSION:
		sz = sizeof(struct sysctlnode);
		break;

	default:
		sz = 0;
		break;
	}

	if (sz > 0 && *o != NULL && *l >= sz) {
		memcpy(*o, src, sz);
		*o = sz + (caddr_t)*o;
		*l -= sz;
	}

	return(sz);
}






#include <sys/types.h>
#include <sys/time.h>
#include <sys/param.h>
#include <sys/resource.h>
#include <sys/stat.h>
#ifdef MAKE_NATIVE
#include <sys/sysctl.h>
#endif
#include <sys/utsname.h>
#include <sys/wait.h>

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "make.h"
#include "hash.h"
#include "dir.h"
#include "job.h"
#include "pathnames.h"
#include "trace.h"

#ifdef USE_IOVEC
#include <sys/uio.h>
#endif


#include "namespace.h"
#include <sys/param.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "reentrant.h"
#include "local.h"
/*#include "glue.h"*/

#include "namespace.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include "reentrant.h"


#include "namespace.h"
#include <sys/param.h>
#include <sys/stat.h>

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ssp/ssp.h>


/*
 * char *realpath(const char *path, char *resolved);
 *
 * Find the real name of path, by removing all ".", ".." and symlink
 * components.  Returns (resolved) on success, or (NULL) on failure,
 * in which case the path which caused trouble is left in (resolved).
 */
char *
realpath(const char * __restrict path, char * __restrict resolved)
{
	struct stat sb;
	int idx = 0, nlnk = 0;
	const char *q;
	char *p, wbuf[2][MAXPATHLEN], *fres;
	size_t len;
	ssize_t n;

	/* POSIX sez we must test for this */
	if (path == NULL) {
		errno = EINVAL;
		return NULL;
	}

	if (resolved == NULL) {
		fres = resolved = malloc(MAXPATHLEN);
		if (resolved == NULL)
			return NULL;
	} else
		fres = NULL;


	/*
	 * Build real path one by one with paying an attention to .,
	 * .. and symbolic link.
	 */

	/*
	 * `p' is where we'll put a new component with prepending
	 * a delimiter.
	 */
	p = resolved;

	if (*path == '\0') {
		*p = '\0';
		errno = ENOENT;
		goto out;
	}

	/* If relative path, start from current working directory. */
	if (*path != '/') {
		/* check for resolved pointer to appease coverity */
		if (resolved && getcwd(resolved, MAXPATHLEN) == NULL) {
			p[0] = '.';
			p[1] = '\0';
			goto out;
		}
		len = strlen(resolved);
		if (len > 1)
			p += len;
	}

loop:
	/* Skip any slash. */
	while (*path == '/')
		path++;

	if (*path == '\0') {
		if (p == resolved)
			*p++ = '/';
		*p = '\0';
		return resolved;
	}

	/* Find the end of this component. */
	q = path;
	do
		q++;
	while (*q != '/' && *q != '\0');

	/* Test . or .. */
	if (path[0] == '.') {
		if (q - path == 1) {
			path = q;
			goto loop;
		}
		if (path[1] == '.' && q - path == 2) {
			/* Trim the last component. */
			if (p != resolved)
				while (*--p != '/')
					continue;
			path = q;
			goto loop;
		}
	}

	/* Append this component. */
	if (p - resolved + 1 + q - path + 1 > MAXPATHLEN) {
		errno = ENAMETOOLONG;
		if (p == resolved)
			*p++ = '/';
		*p = '\0';
		goto out;
	}
	p[0] = '/';
	memcpy(&p[1], path,
	    /* LINTED We know q > path. */
	    q - path);
	p[1 + q - path] = '\0';

	/*
	 * If this component is a symlink, toss it and prepend link
	 * target to unresolved path.
	 */
	if (lstat(resolved, &sb) == -1)
		goto out;

	if (S_ISLNK(sb.st_mode)) {
		if (nlnk++ >= MAXSYMLINKS) {
			errno = ELOOP;
			goto out;
		}
		/*n = readlink(resolved, wbuf[idx], sizeof(wbuf[0]) - 1);*/
		if (n < 0)
			goto out;
		if (n == 0) {
			errno = ENOENT;
			goto out;
		}

		/* Append unresolved path to link target and switch to it. */
		if (n + (len = strlen(q)) + 1 > sizeof(wbuf[0])) {
			errno = ENAMETOOLONG;
			goto out;
		}
		memcpy(&wbuf[idx][n], q, len + 1);
		path = wbuf[idx];
		idx ^= 1;

		/* If absolute symlink, start from root. */
		if (*path == '/')
			p = resolved;
		goto loop;
	}
	if (*q == '/' && !S_ISDIR(sb.st_mode)) {
		errno = ENOTDIR;
		goto out;
	}

	/* Advance both resolved and unresolved path. */
	p += 1 + q - path;
	path = q;
	goto loop;
out:
	free(fres);
	return NULL;
}

/*
int
uname(struct utsname *name)
{
	return (0);
}*/


#include "namespace.h"
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/utsname.h>

#include <assert.h>
#include <errno.h>
/*
#ifdef __weak_alias
__weak_alias(uname,_uname)
#endif
*/
int
uname(struct utsname *name)
{
	int mib[2];
	size_t len;
	char *p;

	_DIAGASSERT(name != NULL);

	mib[0] = CTL_KERN;
	mib[1] = KERN_OSTYPE;
	len = sizeof(name->sysname);
	if (sysctl(mib, 2, &name->sysname, &len, NULL, 0) == -1)
		goto error;

	mib[0] = CTL_KERN;
	mib[1] = KERN_HOSTNAME;
	len = sizeof(name->nodename);
	if (sysctl(mib, 2, &name->nodename, &len, NULL, 0) == -1)
		goto error;

	mib[0] = CTL_KERN;
	mib[1] = KERN_OSRELEASE;
	len = sizeof(name->release);
	if (sysctl(mib, 2, &name->release, &len, NULL, 0) == -1)
		goto error;

	mib[0] = CTL_KERN;
	mib[1] = KERN_VERSION;
	len = sizeof(name->version);
	if (sysctl(mib, 2, &name->version, &len, NULL, 0) == -1) {
		if (errno == ENOMEM) {
			/*
			 * string is too long for {struct utsname}.version.
			 * Just use the truncated string.
			 * XXX: We could mark the truncation with "..."
			 */
			name->version[sizeof(name->version) - 1] = '\0';
		}
		else goto error;
	}

	/* The version may have newlines in it, turn them into spaces. */
	for (p = name->version; len--; ++p) {
		if (*p == '\n' || *p == '\t') {
			if (len > 1)
				*p = ' ';
			else
				*p = '\0';
		}
	}

	mib[0] = CTL_HW;
	mib[1] = HW_MACHINE;
	len = sizeof(name->machine);
	if (sysctl(mib, 2, &name->machine, &len, NULL, 0) == -1)
		goto error;
	return (0);

error:
	return (-1);
}


#ifdef __weak_alias
__weak_alias(initstate,_initstate)
__weak_alias(random,_random)
__weak_alias(setstate,_setstate)
__weak_alias(srandom,_srandom)
#endif


static void srandom_unlocked(unsigned int);
static long random_unlocked(void);

/*
#define mutex_lock(a)	(void)0
#define mutex_unlock(a) (void)0
*/

/*
 * For each of the currently supported random number generators, we have a
 * break value on the amount of state information (you need at least this
 * many bytes of state info to support this random number generator), a degree
 * for the polynomial (actually a trinomial) that the R.N.G. is based on, and
 * the separation between the two lower order coefficients of the trinomial.
 */
#define	TYPE_0		0		/* linear congruential */
#define	BREAK_0		8
#define	DEG_0		0
#define	SEP_0		0

#define	TYPE_1		1		/* x**7 + x**3 + 1 */
#define	BREAK_1		32
#define	DEG_1		7
#define	SEP_1		3

#define	TYPE_2		2		/* x**15 + x + 1 */
#define	BREAK_2		64
#define	DEG_2		15
#define	SEP_2		1

#define	TYPE_3		3		/* x**31 + x**3 + 1 */
#define	BREAK_3		128
#define	DEG_3		31
#define	SEP_3		3

#define	TYPE_4		4		/* x**63 + x + 1 */
#define	BREAK_4		256
#define	DEG_4		63
#define	SEP_4		1

/*
 * Array versions of the above information to make code run faster --
 * relies on fact that TYPE_i == i.
 */
#define	MAX_TYPES	5		/* max number of types above */

static const int degrees[MAX_TYPES] =	{ DEG_0, DEG_1, DEG_2, DEG_3, DEG_4 };
static const int seps[MAX_TYPES] =	{ SEP_0, SEP_1, SEP_2, SEP_3, SEP_4 };

/*
 * Initially, everything is set up as if from:
 *
 *	initstate(1, &randtbl, 128);
 *
 * Note that this initialization takes advantage of the fact that srandom()
 * advances the front and rear pointers 10*rand_deg times, and hence the
 * rear pointer which starts at 0 will also end up at zero; thus the zeroeth
 * element of the state information, which contains info about the current
 * position of the rear pointer is just
 *
 *	MAX_TYPES * (rptr - state) + TYPE_3 == TYPE_3.
 */

/* LINTED */
static int randtbl[DEG_3 + 1] = {
	TYPE_3,
#ifdef USE_BETTER_RANDOM
	0x991539b1, 0x16a5bce3, 0x6774a4cd,
	0x3e01511e, 0x4e508aaa, 0x61048c05,
	0xf5500617, 0x846b7115, 0x6a19892c,
	0x896a97af, 0xdb48f936, 0x14898454,
	0x37ffd106, 0xb58bff9c, 0x59e17104,
	0xcf918a49, 0x09378c83, 0x52c7a471,
	0x8d293ea9, 0x1f4fc301, 0xc3db71be,
	0x39b44e1c, 0xf8a44ef9, 0x4c8b80b1,
	0x19edc328, 0x87bf4bdd, 0xc9b240e5,
	0xe9ee4b1b, 0x4382aee7, 0x535b6b41,
	0xf3bec5da,
#else
	0x9a319039, 0x32d9c024, 0x9b663182,
	0x5da1f342, 0xde3b81e0, 0xdf0a6fb5,
	0xf103bc02, 0x48f340fb, 0x7449e56b,
	0xbeb1dbb0, 0xab5c5918, 0x946554fd,
	0x8c2e680f, 0xeb3d799f, 0xb11ee0b7,
	0x2d436b86, 0xda672e2a, 0x1588ca88,
	0xe369735d, 0x904f35f7, 0xd7158fd6,
	0x6fa6f051, 0x616e6b96, 0xac94efdc,
	0x36413f93, 0xc622c298, 0xf5a42ab8,
	0x8a88d77b, 0xf5ad9d0e, 0x8999220b,
	0x27fb47b9,
#endif /* USE_BETTER_RANDOM */
};

/*
 * fptr and rptr are two pointers into the state info, a front and a rear
 * pointer.  These two pointers are always rand_sep places aparts, as they
 * cycle cyclically through the state information.  (Yes, this does mean we
 * could get away with just one pointer, but the code for random() is more
 * efficient this way).  The pointers are left positioned as they would be
 * from the call
 *
 *	initstate(1, randtbl, 128);
 *
 * (The position of the rear pointer, rptr, is really 0 (as explained above
 * in the initialization of randtbl) because the state table pointer is set
 * to point to randtbl[1] (as explained below).
 */
static int *fptr = &randtbl[SEP_3 + 1];
static int *rptr = &randtbl[1];

/*
 * The following things are the pointer to the state information table, the
 * type of the current generator, the degree of the current polynomial being
 * used, and the separation between the two pointers.  Note that for efficiency
 * of random(), we remember the first location of the state information, not
 * the zeroeth.  Hence it is valid to access state[-1], which is used to
 * store the type of the R.N.G.  Also, we remember the last location, since
 * this is more efficient than indexing every time to find the address of
 * the last element to see if the front and rear pointers have wrapped.
 */
static int *state = &randtbl[1];
static int rand_type = TYPE_3;
static int rand_deg = DEG_3;
static int rand_sep = SEP_3;
static int *end_ptr = &randtbl[DEG_3 + 1];

/*
 * srandom:
 *
 * Initialize the random number generator based on the given seed.  If the
 * type is the trivial no-state-information type, just remember the seed.
 * Otherwise, initializes state[] based on the given "seed" via a linear
 * congruential generator.  Then, the pointers are set to known locations
 * that are exactly rand_sep places apart.  Lastly, it cycles the state
 * information a given number of times to get rid of any initial dependencies
 * introduced by the L.C.R.N.G.  Note that the initialization of randtbl[]
 * for default usage relies on values produced by this routine.
 */

static void
srandom_unlocked(unsigned int x)
{
	int i;

	if (rand_type == TYPE_0)
		state[0] = x;
	else {
		state[0] = x;
		for (i = 1; i < rand_deg; i++) {
#ifdef USE_BETTER_RANDOM
			int x1, hi, lo, t;

			/*
			 * Compute x[n + 1] = (7^5 * x[n]) mod (2^31 - 1).
			 * From "Random number generators: good ones are hard
			 * to find", Park and Miller, Communications of the ACM,
			 * vol. 31, no. 10,
			 * October 1988, p. 1195.
			 */
			x1 = state[i - 1];
			hi = x1 / 127773;
			lo = x1 % 127773;
			t = 16807 * lo - 2836 * hi;
			if (t <= 0)
				t += 0x7fffffff;
			state[i] = t;
#else
			state[i] = 1103515245 * state[i - 1] + 12345;
#endif /* USE_BETTER_RANDOM */
		}
		fptr = &state[rand_sep];
		rptr = &state[0];
		for (i = 0; i < 10 * rand_deg; i++)
			(void)random_unlocked();
	}
}

void
srandom(unsigned int x)
{

	mutex_lock(&random_mutex);
	srandom_unlocked(x);
	mutex_unlock(&random_mutex);
}


static long
random_unlocked(void)
{
	int i;
	int *f, *r;

	if (rand_type == TYPE_0) {
		i = state[0];
		state[0] = i = (i * 1103515245 + 12345) & 0x7fffffff;
	} else {
		/*
		 * Use local variables rather than static variables for speed.
		 */
		f = fptr; r = rptr;
		*f += *r;
		/* chucking least random bit */
		i = ((unsigned int)*f >> 1) & 0x7fffffff;
		if (++f >= end_ptr) {
			f = state;
			++r;
		}
		else if (++r >= end_ptr) {
			r = state;
		}

		fptr = f; rptr = r;
	}
	return(i);
}

#if !defined(_LIBMINC) && !defined(__kernel__) && defined(__minix)

#define	std(flags, file) { \
	._p = NULL, \
	._r = 0, \
	._w = 0, \
	._flags = (flags), \
	._file = (file),  \
	._bf = { ._base = NULL, ._size = 0 }, \
	._lbfsize = 0,  \
	._cookie = __sF + (file), \
	._close = __sclose, \
	._read = __sread, \
	._seek = __sseek, \
	._write = __swrite, \
	._ext = { ._base = (void *)(__sFext + (file)), ._size = 0 }, \
	._up = NULL, \
        ._ur = 0, \
	._ubuf = { [0] = '\0', [1] = '\0', [2] = '\0' }, \
	._nbuf = { [0] = '\0' }, \
	._flush = NULL, \
	._lb_unused = { '\0' }, \
	._blksize = 0, \
	._offset = (off_t)0, \
}

#else

#define	std(flags, file) { \
	._p = NULL, \
	._r = 0, \
	._w = 0, \
	._flags = (flags), \
	._file = (file),  \
	._bf = { ._base = NULL, ._size = 0 }, \
	._lbfsize = 0,  \
	._cookie = __sF + (file), \
	._close = NULL, \
	._read = NULL, \
	._seek = NULL, \
	._write = NULL, \
	._ext = { ._base = (void *)(__sFext + (file)), ._size = 0 }, \
	._up = NULL, \
        ._ur = 0, \
	._ubuf = { [0] = '\0', [1] = '\0', [2] = '\0' }, \
	._nbuf = { [0] = '\0' }, \
	._flush = NULL, \
	._lb_unused = { '\0' }, \
	._blksize = 0, \
	._offset = (off_t)0, \
}

#endif /* !defined(_LIBMINC) && !defined(__kernel__) */

#if defined(_REENTRANT) && !defined(__lint__) /* XXX lint is busted */
#define	STDEXT { ._lock = MUTEX_INITIALIZER, ._lockcond = COND_INITIALIZER }
struct __sfileext __sFext[3] = { STDEXT,
				 STDEXT,
				 STDEXT};
#else
struct __sfileext __sFext[3];
#endif


FILE __sF[3] = {
	std(__SRD, STDIN_FILENO),		/* stdin */
	std(__SWR, STDOUT_FILENO),		/* stdout */
	std(__SWR|__SNBF, STDERR_FILENO)	/* stderr */
};


#ifndef	DEFMAXLOCAL
#define	DEFMAXLOCAL DEFMAXJOBS
#endif	/* DEFMAXLOCAL */

Lst			create;		/* Targets to be made */
time_t			now;		/* Time at start of make */
GNode			*DEFAULT;	/* .DEFAULT node */
Boolean			allPrecious;	/* .PRECIOUS given on line by itself */

static Boolean		noBuiltins;	/* -r flag */
static Lst		makefiles;	/* ordered list of makefiles to read */
static Boolean		printVars;	/* print value of one or more vars */
static Lst		variables;	/* list of variables to print */
int			maxJobs;	/* -j argument */
static int		maxJobTokens;	/* -j argument */
Boolean			compatMake;	/* -B argument */
int			debug;		/* -d argument */
Boolean			debugVflag;	/* -dV */
Boolean			noExecute;	/* -n flag */
Boolean			noRecursiveExecute;	/* -N flag */
Boolean			keepgoing;	/* -k flag */
Boolean			queryFlag;	/* -q flag */
Boolean			touchFlag;	/* -t flag */
Boolean			enterFlag;	/* -w flag */
Boolean			enterFlagObj;	/* -w and objdir != srcdir */
Boolean			ignoreErrors;	/* -i flag */
Boolean			beSilent;	/* -s flag */
Boolean			oldVars;	/* variable substitution style */
Boolean			checkEnvFirst;	/* -e flag */
Boolean			parseWarnFatal;	/* -W flag */
Boolean			jobServer; 	/* -J flag */
static int jp_0 = -1, jp_1 = -1;	/* ends of parent job pipe */
Boolean			varNoExportEnv;	/* -X flag */
Boolean			doing_depend;	/* Set while reading .depend */
static Boolean		jobsRunning;	/* TRUE if the jobs might be running */
static const char *	tracefile;
static void		MainParseArgs(int, char **);
static int		ReadMakefile(const void *, const void *);
static void		usage(void) MAKE_ATTR_DEAD;

static Boolean		ignorePWD;	/* if we use -C, PWD is meaningless */
static char objdir[MAXPATHLEN + 1];	/* where we chdir'ed to */
char curdir[MAXPATHLEN + 1];		/* Startup directory */
char *progname;				/* the program name */
char *makeDependfile;
pid_t myPid;
int makelevel;

Boolean forceJobs = FALSE;

extern Lst parseIncPath;

/*
 * For compatibility with the POSIX version of MAKEFLAGS that includes
 * all the options with out -, convert flags to -f -l -a -g -s.
 */
static char *
explode(const char *flags)
{
    size_t len;
    char *nf, *st;
    const char *f;

    if (flags == NULL)
	return NULL;

    for (f = flags; *f; f++)
	if (!isalpha((unsigned char)*f))
	    break;

    if (*f)
	return bmake_strdup(flags);

    len = strlen(flags);
    st = nf = bmake_malloc(len * 3 + 1);
    while (*flags) {
	*nf++ = '-';
	*nf++ = *flags++;
	*nf++ = ' ';
    }
    *nf = '\0';
    return st;
}
	    
static void
parse_debug_options(const char *argvalue)
{
	const char *modules;
	const char *mode;
	char *fname;
	int len;

	for (modules = argvalue; *modules; ++modules) {
		switch (*modules) {
		case 'A':
			debug = ~0;
			break;
		case 'a':
			debug |= DEBUG_ARCH;
			break;
		case 'C':
			debug |= DEBUG_CWD;
			break;
		case 'c':
			debug |= DEBUG_COND;
			break;
		case 'd':
			debug |= DEBUG_DIR;
			break;
		case 'e':
			debug |= DEBUG_ERROR;
			break;
		case 'f':
			debug |= DEBUG_FOR;
			break;
		case 'g':
			if (modules[1] == '1') {
				debug |= DEBUG_GRAPH1;
				++modules;
			}
			else if (modules[1] == '2') {
				debug |= DEBUG_GRAPH2;
				++modules;
			}
			else if (modules[1] == '3') {
				debug |= DEBUG_GRAPH3;
				++modules;
			}
			break;
		case 'j':
			debug |= DEBUG_JOB;
			break;
		case 'l':
			debug |= DEBUG_LOUD;
			break;
		case 'M':
			debug |= DEBUG_META;
			break;
		case 'm':
			debug |= DEBUG_MAKE;
			break;
		case 'n':
			debug |= DEBUG_SCRIPT;
			break;
		case 'p':
			debug |= DEBUG_PARSE;
			break;
		case 's':
			debug |= DEBUG_SUFF;
			break;
		case 't':
			debug |= DEBUG_TARG;
			break;
		case 'V':
			debugVflag = TRUE;
			break;
		case 'v':
			debug |= DEBUG_VAR;
			break;
		case 'x':
			debug |= DEBUG_SHELL;
			break;
		case 'F':
			if (debug_file != stdout && debug_file != stderr)
				fclose(debug_file);
			if (*++modules == '+') {
				modules++;
				mode = "a";
			} else
				mode = "w";
			if (strcmp(modules, "stdout") == 0) {
				debug_file = stdout;
				goto debug_setbuf;
			}
			if (strcmp(modules, "stderr") == 0) {
				debug_file = stderr;
				goto debug_setbuf;
			}
			len = strlen(modules);
			fname = malloc(len + 20);
			memcpy(fname, modules, len + 1);
			/* Let the filename be modified by the pid */
			if (strcmp(fname + len - 3, ".%d") == 0)
				snprintf(fname + len - 2, 20, "%d", getpid());
			debug_file = fopen(fname, mode);
			if (!debug_file) {
				fprintf(stderr, "Cannot open debug file %s\n",
				    fname);
				usage();
			}
			free(fname);
			goto debug_setbuf;
		default:
			(void)fprintf(stderr,
			    "%s: illegal argument to d option -- %c\n",
			    progname, *modules);
			usage();
		}
	}
debug_setbuf:
	/*
	 * Make the debug_file unbuffered, and make
	 * stdout line buffered (unless debugfile == stdout).
	 */
	setvbuf(debug_file, NULL, _IONBF, 0);
	if (debug_file != stdout) {
		setvbuf(stdout, NULL, _IOLBF, 0);
	}
}

/*-
 * MainParseArgs --
 *	Parse a given argument vector. Called from main() and from
 *	Main_ParseArgLine() when the .MAKEFLAGS target is used.
 *
 *	XXX: Deal with command line overriding .MAKEFLAGS in makefile
 *
 * Results:
 *	None
 *
 * Side Effects:
 *	Various global and local flags will be set depending on the flags
 *	given
 */
static void
MainParseArgs(int argc, char **argv)
{
	char *p;
	int c = '?';
	int arginc;
	char *argvalue;
	const char *getopt_def;
	char *optscan;
	Boolean inOption, dashDash = FALSE;
	char found_path[MAXPATHLEN + 1];	/* for searching for sys.mk */

#define OPTFLAGS "BC:D:I:J:NST:V:WXd:ef:ij:km:nqrstw"
/* Can't actually use getopt(3) because rescanning is not portable */

	getopt_def = OPTFLAGS;
rearg:	
	inOption = FALSE;
	optscan = NULL;
	while(argc > 1) {
		char *getopt_spec;
		if(!inOption)
			optscan = argv[1];
		c = *optscan++;
		arginc = 0;
		if(inOption) {
			if(c == '\0') {
				++argv;
				--argc;
				inOption = FALSE;
				continue;
			}
		} else {
			if (c != '-' || dashDash)
				break;
			inOption = TRUE;
			c = *optscan++;
		}
		/* '-' found at some earlier point */
		getopt_spec = strchr(getopt_def, c);
		if(c != '\0' && getopt_spec != NULL && getopt_spec[1] == ':') {
			/* -<something> found, and <something> should have an arg */
			inOption = FALSE;
			arginc = 1;
			argvalue = optscan;
			if(*argvalue == '\0') {
				if (argc < 3)
					goto noarg;
				argvalue = argv[2];
				arginc = 2;
			}
		} else {
			argvalue = NULL; 
		}
		switch(c) {
		case '\0':
			arginc = 1;
			inOption = FALSE;
			break;
		case 'B':
			compatMake = TRUE;
			Var_Append(MAKEFLAGS, "-B", VAR_GLOBAL);
			Var_Set(MAKE_MODE, "compat", VAR_GLOBAL, 0);
			break;
		case 'C':
			if (chdir(argvalue) == -1) {
				(void)fprintf(stderr,
					      "%s: chdir %s: %s\n",
					      progname, argvalue,
					      strerror(errno));
				exit(1);
			}
			if (getcwd(curdir, MAXPATHLEN) == NULL) {
				(void)fprintf(stderr, "%s: %s.\n", progname, strerror(errno));
				exit(2);
			}
			ignorePWD = TRUE;
			break;
		case 'D':
			if (argvalue == NULL || argvalue[0] == 0) goto noarg;
			Var_Set(argvalue, "1", VAR_GLOBAL, 0);
			Var_Append(MAKEFLAGS, "-D", VAR_GLOBAL);
			Var_Append(MAKEFLAGS, argvalue, VAR_GLOBAL);
			break;
		case 'I':
			if (argvalue == NULL) goto noarg;
			Parse_AddIncludeDir(argvalue);
			Var_Append(MAKEFLAGS, "-I", VAR_GLOBAL);
			Var_Append(MAKEFLAGS, argvalue, VAR_GLOBAL);
			break;
		case 'J':
			if (argvalue == NULL) goto noarg;
			if (sscanf(argvalue, "%d,%d", &jp_0, &jp_1) != 2) {
			    (void)fprintf(stderr,
				"%s: internal error -- J option malformed (%s)\n",
				progname, argvalue);
				usage();
			}
			if ((fcntl(jp_0, F_GETFD, 0) < 0) ||
			    (fcntl(jp_1, F_GETFD, 0) < 0)) {
#if 0
			    (void)fprintf(stderr,
				"%s: ###### warning -- J descriptors were closed!\n",
				progname);
			    exit(2);
#endif
			    jp_0 = -1;
			    jp_1 = -1;
			    compatMake = TRUE;
			} else {
			    Var_Append(MAKEFLAGS, "-J", VAR_GLOBAL);
			    Var_Append(MAKEFLAGS, argvalue, VAR_GLOBAL);
			    jobServer = TRUE;
			}
			break;
		case 'N':
			noExecute = TRUE;
			noRecursiveExecute = TRUE;
			Var_Append(MAKEFLAGS, "-N", VAR_GLOBAL);
			break;
		case 'S':
			keepgoing = FALSE;
			Var_Append(MAKEFLAGS, "-S", VAR_GLOBAL);
			break;
		case 'T':
			if (argvalue == NULL) goto noarg;
			tracefile = bmake_strdup(argvalue);
			Var_Append(MAKEFLAGS, "-T", VAR_GLOBAL);
			Var_Append(MAKEFLAGS, argvalue, VAR_GLOBAL);
			break;
		case 'V':
			if (argvalue == NULL) goto noarg;
			printVars = TRUE;
			(void)Lst_AtEnd(variables, argvalue);
			Var_Append(MAKEFLAGS, "-V", VAR_GLOBAL);
			Var_Append(MAKEFLAGS, argvalue, VAR_GLOBAL);
			break;
		case 'W':
			parseWarnFatal = TRUE;
			break;
		case 'X':
			varNoExportEnv = TRUE;
			Var_Append(MAKEFLAGS, "-X", VAR_GLOBAL);
			break;
		case 'd':
			if (argvalue == NULL) goto noarg;
			/* If '-d-opts' don't pass to children */
			if (argvalue[0] == '-')
			    argvalue++;
			else {
			    Var_Append(MAKEFLAGS, "-d", VAR_GLOBAL);
			    Var_Append(MAKEFLAGS, argvalue, VAR_GLOBAL);
			}
			parse_debug_options(argvalue);
			break;
		case 'e':
			checkEnvFirst = TRUE;
			Var_Append(MAKEFLAGS, "-e", VAR_GLOBAL);
			break;
		case 'f':
			if (argvalue == NULL) goto noarg;
			(void)Lst_AtEnd(makefiles, argvalue);
			break;
		case 'i':
			ignoreErrors = TRUE;
			Var_Append(MAKEFLAGS, "-i", VAR_GLOBAL);
			break;
		case 'j':
			if (argvalue == NULL) goto noarg;
			forceJobs = TRUE;
			maxJobs = strtol(argvalue, &p, 0);
			if (*p != '\0' || maxJobs < 1) {
				(void)fprintf(stderr, "%s: illegal argument to -j -- must be positive integer!\n",
				    progname);
				exit(1);
			}
			Var_Append(MAKEFLAGS, "-j", VAR_GLOBAL);
			Var_Append(MAKEFLAGS, argvalue, VAR_GLOBAL);
			Var_Set(".MAKE.JOBS", argvalue, VAR_GLOBAL, 0);
			maxJobTokens = maxJobs;
			break;
		case 'k':
			keepgoing = TRUE;
			Var_Append(MAKEFLAGS, "-k", VAR_GLOBAL);
			break;
		case 'm':
			if (argvalue == NULL) goto noarg;
			/* look for magic parent directory search string */
			if (strncmp(".../", argvalue, 4) == 0) {
				if (!Dir_FindHereOrAbove(curdir, argvalue+4,
				    found_path, sizeof(found_path)))
					break;		/* nothing doing */
				(void)Dir_AddDir(sysIncPath, found_path);
			} else {
				(void)Dir_AddDir(sysIncPath, argvalue);
			}
			Var_Append(MAKEFLAGS, "-m", VAR_GLOBAL);
			Var_Append(MAKEFLAGS, argvalue, VAR_GLOBAL);
			break;
		case 'n':
			noExecute = TRUE;
			Var_Append(MAKEFLAGS, "-n", VAR_GLOBAL);
			break;
		case 'q':
			queryFlag = TRUE;
			/* Kind of nonsensical, wot? */
			Var_Append(MAKEFLAGS, "-q", VAR_GLOBAL);
			break;
		case 'r':
			noBuiltins = TRUE;
			Var_Append(MAKEFLAGS, "-r", VAR_GLOBAL);
			break;
		case 's':
			beSilent = TRUE;
			Var_Append(MAKEFLAGS, "-s", VAR_GLOBAL);
			break;
		case 't':
			touchFlag = TRUE;
			Var_Append(MAKEFLAGS, "-t", VAR_GLOBAL);
			break;
		case 'w':
			enterFlag = TRUE;
			Var_Append(MAKEFLAGS, "-w", VAR_GLOBAL);
			break;
		case '-':
			dashDash = TRUE;
			break;
		default:
		case '?':
			usage();
		}
		argv += arginc;
		argc -= arginc;
	}

	oldVars = TRUE;

	/*
	 * See if the rest of the arguments are variable assignments and
	 * perform them if so. Else take them to be targets and stuff them
	 * on the end of the "create" list.
	 */
	for (; argc > 1; ++argv, --argc)
		if (Parse_IsVar(argv[1])) {
			Parse_DoVar(argv[1], VAR_CMD);
		} else {
			if (!*argv[1])
				Punt("illegal (null) argument.");
			if (*argv[1] == '-' && !dashDash)
				goto rearg;
			(void)Lst_AtEnd(create, bmake_strdup(argv[1]));
		}

	return;
noarg:
	(void)fprintf(stderr, "%s: option requires an argument -- %c\n",
	    progname, c);
	usage();
}

/*-
 * Main_ParseArgLine --
 *  	Used by the parse module when a .MFLAGS or .MAKEFLAGS target
 *	is encountered and by main() when reading the .MAKEFLAGS envariable.
 *	Takes a line of arguments and breaks it into its
 * 	component words and passes those words and the number of them to the
 *	MainParseArgs function.
 *	The line should have all its leading whitespace removed.
 *
 * Input:
 *	line		Line to fracture
 *
 * Results:
 *	None
 *
 * Side Effects:
 *	Only those that come from the various arguments.
 */
void
Main_ParseArgLine(const char *line)
{
	char **argv;			/* Manufactured argument vector */
	int argc;			/* Number of arguments in argv */
	char *args;			/* Space used by the args */
	char *buf, *p1;
	char *argv0 = Var_Value(".MAKE", VAR_GLOBAL, &p1);
	size_t len;

	if (line == NULL)
		return;
	for (; *line == ' '; ++line)
		continue;
	if (!*line)
		return;

	buf = bmake_malloc(len = strlen(line) + strlen(argv0) + 2);
	(void)snprintf(buf, len, "%s %s", argv0, line);
	if (p1)
		free(p1);

	argv = brk_string(buf, &argc, TRUE, &args);
	if (argv == NULL) {
		Error("Unterminated quoted string [%s]", buf);
		free(buf);
		return;
	}
	free(buf);
	MainParseArgs(argc, argv);

	free(args);
	free(argv);
}

Boolean
Main_SetObjdir(const char *path)
{
	struct stat sb;
	char *p = NULL;
	char buf[MAXPATHLEN + 1];
	Boolean rc = FALSE;

	/* expand variable substitutions */
	if (strchr(path, '$') != 0) {
		snprintf(buf, MAXPATHLEN, "%s", path);
		path = p = Var_Subst(NULL, buf, VAR_GLOBAL, 0);
	}

	if (path[0] != '/') {
		snprintf(buf, MAXPATHLEN, "%s/%s", curdir, path);
		path = buf;
	}

	/* look for the directory and try to chdir there */
	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
		if (chdir(path)) {
			(void)fprintf(stderr, "make warning: %s: %s.\n",
				      path, strerror(errno));
		} else {
			strncpy(objdir, path, MAXPATHLEN);
			Var_Set(".OBJDIR", objdir, VAR_GLOBAL, 0);
			setenv("PWD", objdir, 1);
			Dir_InitDot();
			rc = TRUE;
			if (enterFlag && strcmp(objdir, curdir) != 0)
				enterFlagObj = TRUE;
		}
	}

	if (p)
		free(p);
	return rc;
}

/*-
 * ReadAllMakefiles --
 *	wrapper around ReadMakefile() to read all.
 *
 * Results:
 *	TRUE if ok, FALSE on error
 */
static int
ReadAllMakefiles(const void *p, const void *q)
{
	return (ReadMakefile(p, q) == 0);
}

int
str2Lst_Append(Lst lp, char *str, const char *sep)
{
    char *cp;
    int n;

    if (!sep)
	sep = " \t";

    for (n = 0, cp = strtok(str, sep); cp; cp = strtok(NULL, sep)) {
	(void)Lst_AtEnd(lp, cp);
	n++;
    }
    return (n);
}

#ifdef SIGINFO
/*ARGSUSED*/
static void
siginfo(int signo MAKE_ATTR_UNUSED)
{
	char dir[MAXPATHLEN];
	char str[2 * MAXPATHLEN];
	int len;
	if (getcwd(dir, sizeof(dir)) == NULL)
		return;
	len = snprintf(str, sizeof(str), "%s: Working in: %s\n", progname, dir);
	if (len > 0)
		(void)write(STDERR_FILENO, str, (size_t)len);
}
#endif

/*
 * Allow makefiles some control over the mode we run in.
 */
void
MakeMode(const char *mode)
{
    char *mp = NULL;

    if (!mode)
	mode = mp = Var_Subst(NULL, "${" MAKE_MODE ":tl}", VAR_GLOBAL, 0);

    if (mode && *mode) {
	if (strstr(mode, "compat")) {
	    compatMake = TRUE;
	    forceJobs = FALSE;
	}
#if USE_META
	if (strstr(mode, "meta"))
	    meta_mode_init(mode);
#endif
    }
    if (mp)
	free(mp);
}

/*-
 * main --
 *	The main function, for obvious reasons. Initializes variables
 *	and a few modules, then parses the arguments give it in the
 *	environment and on the command line. Reads the system makefile
 *	followed by either Makefile, makefile or the file given by the
 *	-f argument. Sets the .MAKEFLAGS PMake variable based on all the
 *	flags it has received by then uses either the Make or the Compat
 *	module to create the initial list of targets.
 *
 * Results:
 *	If -q was given, exits -1 if anything was out-of-date. Else it exits
 *	0.
 *
 * Side Effects:
 *	The program exits when done. Targets are created. etc. etc. etc.
 */
int
main(int argc, char **argv)
{
	Lst targs;	/* target nodes to create -- passed to Make_Init */
	Boolean outOfDate = FALSE; 	/* FALSE if all targets up to date */
	struct stat sb, sa;
	char *p1, *path;
	char mdpath[MAXPATHLEN];
    	const char *machine = getenv("MACHINE");
	const char *machine_arch = getenv("MACHINE_ARCH");
	char *syspath = getenv("MAKESYSPATH");
	Lst sysMkPath;			/* Path of sys.mk */
	char *cp = NULL, *start;
					/* avoid faults on read-only strings */
	static char defsyspath[] = _PATH_DEFSYSPATH;
	char found_path[MAXPATHLEN + 1];	/* for searching for sys.mk */
	struct timeval rightnow;		/* to initialize random seed */
	struct utsname utsname;

	/* default to writing debug to stderr */
	debug_file = stderr;

#ifdef SIGINFO
	(void)bmake_signal(SIGINFO, siginfo);
#endif
	/*
	 * Set the seed to produce a different random sequence
	 * on each program execution.
	 */
	gettimeofday(&rightnow, NULL);
	srandom(rightnow.tv_sec + rightnow.tv_usec);
	
	if ((progname = strrchr(argv[0], '/')) != NULL)
		progname++;
	else
		progname = argv[0];
#if defined(MAKE_NATIVE) || (defined(HAVE_SETRLIMIT) && defined(RLIMIT_NOFILE))
	/*
	 * get rid of resource limit on file descriptors
	 */
	{
		struct rlimit rl;
		if (getrlimit(RLIMIT_NOFILE, &rl) != -1 &&
		    rl.rlim_cur != rl.rlim_max) {
			rl.rlim_cur = rl.rlim_max;
			(void)setrlimit(RLIMIT_NOFILE, &rl);
		}
	}
#endif

	if (uname(&utsname) == -1) {
	    (void)fprintf(stderr, "%s: uname failed (%s).\n", progname,
		strerror(errno));
	    exit(2);
	}

	/*
	 * Get the name of this type of MACHINE from utsname
	 * so we can share an executable for similar machines.
	 * (i.e. m68k: amiga hp300, mac68k, sun3, ...)
	 *
	 * Note that both MACHINE and MACHINE_ARCH are decided at
	 * run-time.
	 */
	if (!machine) {
#ifdef MAKE_NATIVE
	    machine = utsname.machine;
#else
#ifdef MAKE_MACHINE
	    machine = MAKE_MACHINE;
#else
	    machine = "unknown";
#endif
#endif
	}

	if (!machine_arch) {
#if defined(MAKE_NATIVE)
	    static char machine_arch_buf[sizeof(utsname.machine)];
	    const int mib[2] = { CTL_HW, HW_MACHINE_ARCH };
	    size_t len = sizeof(machine_arch_buf);
                
	    if (sysctl(mib, __arraycount(mib), machine_arch_buf,
		    &len, NULL, 0) < 0) {
		(void)fprintf(stderr, "%s: sysctl failed (%s).\n", progname,
		    strerror(errno));
		exit(2);
	    }

	    machine_arch = machine_arch_buf;
#else
#ifndef MACHINE_ARCH
#ifdef MAKE_MACHINE_ARCH
            machine_arch = MAKE_MACHINE_ARCH;
#else
	    machine_arch = "unknown";
#endif
#else
	    machine_arch = MACHINE_ARCH;
#endif
#endif
	}

	myPid = getpid();		/* remember this for vFork() */

	/*
	 * Just in case MAKEOBJDIR wants us to do something tricky.
	 */
	Var_Init();		/* Initialize the lists of variables for
				 * parsing arguments */
	Var_Set(".MAKE.OS", utsname.sysname, VAR_GLOBAL, 0);
	Var_Set("MACHINE", machine, VAR_GLOBAL, 0);
	Var_Set("MACHINE_ARCH", machine_arch, VAR_GLOBAL, 0);
#ifdef MAKE_VERSION
	Var_Set("MAKE_VERSION", MAKE_VERSION, VAR_GLOBAL, 0);
#endif
	Var_Set(".newline", "\n", VAR_GLOBAL, 0); /* handy for :@ loops */
	/*
	 * This is the traditional preference for makefiles.
	 */
#ifndef MAKEFILE_PREFERENCE_LIST
# define MAKEFILE_PREFERENCE_LIST "makefile Makefile"
#endif
	Var_Set(MAKEFILE_PREFERENCE, MAKEFILE_PREFERENCE_LIST,
		VAR_GLOBAL, 0);
	Var_Set(MAKE_DEPENDFILE, ".depend", VAR_GLOBAL, 0);

	create = Lst_Init(FALSE);
	makefiles = Lst_Init(FALSE);
	printVars = FALSE;
	debugVflag = FALSE;
	variables = Lst_Init(FALSE);
	beSilent = FALSE;		/* Print commands as executed */
	ignoreErrors = FALSE;		/* Pay attention to non-zero returns */
	noExecute = FALSE;		/* Execute all commands */
	noRecursiveExecute = FALSE;	/* Execute all .MAKE targets */
	keepgoing = FALSE;		/* Stop on error */
	allPrecious = FALSE;		/* Remove targets when interrupted */
	queryFlag = FALSE;		/* This is not just a check-run */
	noBuiltins = FALSE;		/* Read the built-in rules */
	touchFlag = FALSE;		/* Actually update targets */
	debug = 0;			/* No debug verbosity, please. */
	jobsRunning = FALSE;

	maxJobs = DEFMAXLOCAL;		/* Set default local max concurrency */
	maxJobTokens = maxJobs;
	compatMake = FALSE;		/* No compat mode */
	ignorePWD = FALSE;

	/*
	 * Initialize the parsing, directory and variable modules to prepare
	 * for the reading of inclusion paths and variable settings on the
	 * command line
	 */

	/*
	 * Initialize various variables.
	 *	MAKE also gets this name, for compatibility
	 *	.MAKEFLAGS gets set to the empty string just in case.
	 *	MFLAGS also gets initialized empty, for compatibility.
	 */
	Parse_Init();
	if (argv[0][0] == '/' || strchr(argv[0], '/') == NULL) {
	    /*
	     * Leave alone if it is an absolute path, or if it does
	     * not contain a '/' in which case we need to find it in
	     * the path, like execvp(3) and the shells do.
	     */
	    p1 = argv[0];
	} else {
	    /*
	     * A relative path, canonicalize it.
	     */
	    p1 = realpath(argv[0], mdpath);
	    if (!p1 || *p1 != '/' || stat(p1, &sb) < 0) {
		p1 = argv[0];		/* realpath failed */
	    }
	}
	Var_Set("MAKE", p1, VAR_GLOBAL, 0);
	Var_Set(".MAKE", p1, VAR_GLOBAL, 0);
	Var_Set(MAKEFLAGS, "", VAR_GLOBAL, 0);
	Var_Set(MAKEOVERRIDES, "", VAR_GLOBAL, 0);
	Var_Set("MFLAGS", "", VAR_GLOBAL, 0);
	Var_Set(".ALLTARGETS", "", VAR_GLOBAL, 0);
	/* some makefiles need to know this */
	Var_Set(MAKE_LEVEL ".ENV", MAKE_LEVEL_ENV, VAR_CMD, 0);

	/*
	 * Set some other useful macros
	 */
	{
	    char tmp[64], *ep;

	    makelevel = ((ep = getenv(MAKE_LEVEL_ENV)) && *ep) ? atoi(ep) : 0;
	    if (makelevel < 0)
		makelevel = 0;
	    snprintf(tmp, sizeof(tmp), "%d", makelevel);
	    Var_Set(MAKE_LEVEL, tmp, VAR_GLOBAL, 0);
	    snprintf(tmp, sizeof(tmp), "%u", myPid);
	    Var_Set(".MAKE.PID", tmp, VAR_GLOBAL, 0);
	    snprintf(tmp, sizeof(tmp), "%u", getppid());
	    Var_Set(".MAKE.PPID", tmp, VAR_GLOBAL, 0);
	}
	if (makelevel > 0) {
		char pn[1024];
		snprintf(pn, sizeof(pn), "%s[%d]", progname, makelevel);
		progname = bmake_strdup(pn);
	}

#ifdef USE_META
	meta_init();
#endif
	/*
	 * First snag any flags out of the MAKE environment variable.
	 * (Note this is *not* MAKEFLAGS since /bin/make uses that and it's
	 * in a different format).
	 */
#ifdef POSIX
	p1 = explode(getenv("MAKEFLAGS"));
	Main_ParseArgLine(p1);
	free(p1);
#else
	Main_ParseArgLine(getenv("MAKE"));
#endif

	/*
	 * Find where we are (now).
	 * We take care of PWD for the automounter below...
	 */
	if (getcwd(curdir, MAXPATHLEN) == NULL) {
		(void)fprintf(stderr, "%s: getcwd: %s.\n",
		    progname, strerror(errno));
		exit(2);
	}

	MainParseArgs(argc, argv);

	if (enterFlag)
		printf("%s: Entering directory `%s'\n", progname, curdir);

	/*
	 * Verify that cwd is sane.
	 */
	if (stat(curdir, &sa) == -1) {
	    (void)fprintf(stderr, "%s: %s: %s.\n",
		 progname, curdir, strerror(errno));
	    exit(2);
	}

	/*
	 * All this code is so that we know where we are when we start up
	 * on a different machine with pmake.
	 * Overriding getcwd() with $PWD totally breaks MAKEOBJDIRPREFIX
	 * since the value of curdir can vary depending on how we got
	 * here.  Ie sitting at a shell prompt (shell that provides $PWD)
	 * or via subdir.mk in which case its likely a shell which does
	 * not provide it.
	 * So, to stop it breaking this case only, we ignore PWD if
	 * MAKEOBJDIRPREFIX is set or MAKEOBJDIR contains a transform.
	 */
#ifndef NO_PWD_OVERRIDE
	if (!ignorePWD) {
		char *pwd, *ptmp1 = NULL, *ptmp2 = NULL;

		if ((pwd = getenv("PWD")) != NULL &&
		    Var_Value("MAKEOBJDIRPREFIX", VAR_CMD, &ptmp1) == NULL) {
			const char *makeobjdir = Var_Value("MAKEOBJDIR",
			    VAR_CMD, &ptmp2);

			if (makeobjdir == NULL || !strchr(makeobjdir, '$')) {
				if (stat(pwd, &sb) == 0 &&
				    sa.st_ino == sb.st_ino &&
				    sa.st_dev == sb.st_dev)
					(void)strncpy(curdir, pwd, MAXPATHLEN);
			}
		}
		free(ptmp1);
		free(ptmp2);
	}
#endif
	Var_Set(".CURDIR", curdir, VAR_GLOBAL, 0);

	/*
	 * Find the .OBJDIR.  If MAKEOBJDIRPREFIX, or failing that,
	 * MAKEOBJDIR is set in the environment, try only that value
	 * and fall back to .CURDIR if it does not exist.
	 *
	 * Otherwise, try _PATH_OBJDIR.MACHINE, _PATH_OBJDIR, and
	 * finally _PATH_OBJDIRPREFIX`pwd`, in that order.  If none
	 * of these paths exist, just use .CURDIR.
	 */
	Dir_Init(curdir);
	(void)Main_SetObjdir(curdir);

	if ((path = Var_Value("MAKEOBJDIRPREFIX", VAR_CMD, &p1)) != NULL) {
		(void)snprintf(mdpath, MAXPATHLEN, "%s%s", path, curdir);
		(void)Main_SetObjdir(mdpath);
		free(p1);
	} else if ((path = Var_Value("MAKEOBJDIR", VAR_CMD, &p1)) != NULL) {
		(void)Main_SetObjdir(path);
		free(p1);
	} else {
		(void)snprintf(mdpath, MAXPATHLEN, "%s.%s", _PATH_OBJDIR, machine);
		if (!Main_SetObjdir(mdpath) && !Main_SetObjdir(_PATH_OBJDIR)) {
			(void)snprintf(mdpath, MAXPATHLEN, "%s%s", 
					_PATH_OBJDIRPREFIX, curdir);
			(void)Main_SetObjdir(mdpath);
		}
	}

	/*
	 * Be compatible if user did not specify -j and did not explicitly
	 * turned compatibility on
	 */
	if (!compatMake && !forceJobs) {
		compatMake = TRUE;
	}
	
	/*
	 * Initialize archive, target and suffix modules in preparation for
	 * parsing the makefile(s)
	 */
	Arch_Init();
	Targ_Init();
	Suff_Init();
	Trace_Init(tracefile);

	DEFAULT = NULL;
	(void)time(&now);

	Trace_Log(MAKESTART, NULL);
	
	/*
	 * Set up the .TARGETS variable to contain the list of targets to be
	 * created. If none specified, make the variable empty -- the parser
	 * will fill the thing in with the default or .MAIN target.
	 */
	if (!Lst_IsEmpty(create)) {
		LstNode ln;

		for (ln = Lst_First(create); ln != NULL;
		    ln = Lst_Succ(ln)) {
			char *name = (char *)Lst_Datum(ln);

			Var_Append(".TARGETS", name, VAR_GLOBAL);
		}
	} else
		Var_Set(".TARGETS", "", VAR_GLOBAL, 0);


	/*
	 * If no user-supplied system path was given (through the -m option)
	 * add the directories from the DEFSYSPATH (more than one may be given
	 * as dir1:...:dirn) to the system include path.
	 */
	if (syspath == NULL || *syspath == '\0')
		syspath = defsyspath;
	else
		syspath = bmake_strdup(syspath);

	for (start = syspath; *start != '\0'; start = cp) {
		for (cp = start; *cp != '\0' && *cp != ':'; cp++)
			continue;
		if (*cp == ':') {
			*cp++ = '\0';
		}
		/* look for magic parent directory search string */
		if (strncmp(".../", start, 4) != 0) {
			(void)Dir_AddDir(defIncPath, start);
		} else {
			if (Dir_FindHereOrAbove(curdir, start+4, 
			    found_path, sizeof(found_path))) {
				(void)Dir_AddDir(defIncPath, found_path);
			}
		}
	}
	if (syspath != defsyspath)
		free(syspath);

	/*
	 * Read in the built-in rules first, followed by the specified
	 * makefile, if it was (makefile != NULL), or the default
	 * makefile and Makefile, in that order, if it wasn't.
	 */
	if (!noBuiltins) {
		LstNode ln;

		sysMkPath = Lst_Init(FALSE);
		Dir_Expand(_PATH_DEFSYSMK,
			   Lst_IsEmpty(sysIncPath) ? defIncPath : sysIncPath,
			   sysMkPath);
		if (Lst_IsEmpty(sysMkPath))
			Fatal("%s: no system rules (%s).", progname,
			    _PATH_DEFSYSMK);
		ln = Lst_Find(sysMkPath, NULL, ReadMakefile);
		if (ln == NULL)
			Fatal("%s: cannot open %s.", progname,
			    (char *)Lst_Datum(ln));
	}

	if (!Lst_IsEmpty(makefiles)) {
		LstNode ln;

		ln = Lst_Find(makefiles, NULL, ReadAllMakefiles);
		if (ln != NULL)
			Fatal("%s: cannot open %s.", progname, 
			    (char *)Lst_Datum(ln));
	} else {
	    p1 = Var_Subst(NULL, "${" MAKEFILE_PREFERENCE "}",
		VAR_CMD, 0);
	    if (p1) {
		(void)str2Lst_Append(makefiles, p1, NULL);
		(void)Lst_Find(makefiles, NULL, ReadMakefile);
		free(p1);
	    }
	}

	/* In particular suppress .depend for '-r -V .OBJDIR -f /dev/null' */
	if (!noBuiltins || !printVars) {
	    makeDependfile = Var_Subst(NULL, "${.MAKE.DEPENDFILE:T}",
		VAR_CMD, 0);
	    doing_depend = TRUE;
	    (void)ReadMakefile(makeDependfile, NULL);
	    doing_depend = FALSE;
	}

	if (enterFlagObj)
		printf("%s: Entering directory `%s'\n", progname, objdir);
	
	MakeMode(NULL);

	Var_Append("MFLAGS", Var_Value(MAKEFLAGS, VAR_GLOBAL, &p1), VAR_GLOBAL);
	if (p1)
	    free(p1);

	if (!compatMake)
	    Job_ServerStart(maxJobTokens, jp_0, jp_1);
	if (DEBUG(JOB))
	    fprintf(debug_file, "job_pipe %d %d, maxjobs %d, tokens %d, compat %d\n",
		jp_0, jp_1, maxJobs, maxJobTokens, compatMake);

	Main_ExportMAKEFLAGS(TRUE);	/* initial export */
	

	/*
	 * For compatibility, look at the directories in the VPATH variable
	 * and add them to the search path, if the variable is defined. The
	 * variable's value is in the same format as the PATH envariable, i.e.
	 * <directory>:<directory>:<directory>...
	 */
	if (Var_Exists("VPATH", VAR_CMD)) {
		char *vpath, savec;
		/*
		 * GCC stores string constants in read-only memory, but
		 * Var_Subst will want to write this thing, so store it
		 * in an array
		 */
		static char VPATH[] = "${VPATH}";

		vpath = Var_Subst(NULL, VPATH, VAR_CMD, FALSE);
		path = vpath;
		do {
			/* skip to end of directory */
			for (cp = path; *cp != ':' && *cp != '\0'; cp++)
				continue;
			/* Save terminator character so know when to stop */
			savec = *cp;
			*cp = '\0';
			/* Add directory to search path */
			(void)Dir_AddDir(dirSearchPath, path);
			*cp = savec;
			path = cp + 1;
		} while (savec == ':');
		free(vpath);
	}

	/*
	 * Now that all search paths have been read for suffixes et al, it's
	 * time to add the default search path to their lists...
	 */
	Suff_DoPaths();

	/*
	 * Propagate attributes through :: dependency lists.
	 */
	Targ_Propagate();

	/* print the initial graph, if the user requested it */
	if (DEBUG(GRAPH1))
		Targ_PrintGraph(1);

	/* print the values of any variables requested by the user */
	if (printVars) {
		LstNode ln;
		Boolean expandVars;

		if (debugVflag)
			expandVars = FALSE;
		else
			expandVars = getBoolean(".MAKE.EXPAND_VARIABLES", FALSE);
		for (ln = Lst_First(variables); ln != NULL;
		    ln = Lst_Succ(ln)) {
			char *var = (char *)Lst_Datum(ln);
			char *value;
			
			if (strchr(var, '$')) {
				value = p1 = Var_Subst(NULL, var, VAR_GLOBAL, 0);
			} else if (expandVars) {
				char tmp[128];
								
				if (snprintf(tmp, sizeof(tmp), "${%s}", var) >= (int)(sizeof(tmp)))
					Fatal("%s: variable name too big: %s",
					      progname, var);
				value = p1 = Var_Subst(NULL, tmp, VAR_GLOBAL, 0);
			} else {
				value = Var_Value(var, VAR_GLOBAL, &p1);
			}
			printf("%s\n", value ? value : "");
			if (p1)
				free(p1);
		}
	} else {
		/*
		 * Have now read the entire graph and need to make a list of
		 * targets to create. If none was given on the command line,
		 * we consult the parsing module to find the main target(s)
		 * to create.
		 */
		if (Lst_IsEmpty(create))
			targs = Parse_MainName();
		else
			targs = Targ_FindList(create, TARG_CREATE);

		if (!compatMake) {
			/*
			 * Initialize job module before traversing the graph
			 * now that any .BEGIN and .END targets have been read.
			 * This is done only if the -q flag wasn't given
			 * (to prevent the .BEGIN from being executed should
			 * it exist).
			 */
			if (!queryFlag) {
				Job_Init();
				jobsRunning = TRUE;
			}

			/* Traverse the graph, checking on all the targets */
			outOfDate = Make_Run(targs);
		} else {
			/*
			 * Compat_Init will take care of creating all the
			 * targets as well as initializing the module.
			 */
			Compat_Run(targs);
		}
	}

#ifdef CLEANUP
	Lst_Destroy(targs, NULL);
	Lst_Destroy(variables, NULL);
	Lst_Destroy(makefiles, NULL);
	Lst_Destroy(create, (FreeProc *)free);
#endif

	/* print the graph now it's been processed if the user requested it */
	if (DEBUG(GRAPH2))
		Targ_PrintGraph(2);

	Trace_Log(MAKEEND, 0);

	if (enterFlagObj)
		printf("%s: Leaving directory `%s'\n", progname, objdir);
	if (enterFlag)
		printf("%s: Leaving directory `%s'\n", progname, curdir);

	Suff_End();
        Targ_End();
	Arch_End();
	Var_End();
	Parse_End();
	Dir_End();
	Job_End();
	Trace_End();

	return outOfDate ? 1 : 0;
}

/*-
 * ReadMakefile  --
 *	Open and parse the given makefile.
 *
 * Results:
 *	0 if ok. -1 if couldn't open file.
 *
 * Side Effects:
 *	lots
 */
static int
ReadMakefile(const void *p, const void *q MAKE_ATTR_UNUSED)
{
	const char *fname = p;		/* makefile to read */
	int fd;
	size_t len = MAXPATHLEN;
	char *name, *path = bmake_malloc(len);

	if (!strcmp(fname, "-")) {
		Parse_File(NULL /*stdin*/, -1);
		Var_Set("MAKEFILE", "", VAR_INTERNAL, 0);
	} else {
		/* if we've chdir'd, rebuild the path name */
		if (strcmp(curdir, objdir) && *fname != '/') {
			size_t plen = strlen(curdir) + strlen(fname) + 2;
			if (len < plen)
				path = bmake_realloc(path, len = 2 * plen);
			
			(void)snprintf(path, len, "%s/%s", curdir, fname);
			fd = open(path, O_RDONLY);
			if (fd != -1) {
				fname = path;
				goto found;
			}
			
			/* If curdir failed, try objdir (ala .depend) */
			plen = strlen(objdir) + strlen(fname) + 2;
			if (len < plen)
				path = bmake_realloc(path, len = 2 * plen);
			(void)snprintf(path, len, "%s/%s", objdir, fname);
			fd = open(path, O_RDONLY);
			if (fd != -1) {
				fname = path;
				goto found;
			}
		} else {
			fd = open(fname, O_RDONLY);
			if (fd != -1)
				goto found;
		}
		/* look in -I and system include directories. */
		name = Dir_FindFile(fname, parseIncPath);
		if (!name)
			name = Dir_FindFile(fname,
				Lst_IsEmpty(sysIncPath) ? defIncPath : sysIncPath);
		if (!name || (fd = open(name, O_RDONLY)) == -1) {
			if (name)
				free(name);
			free(path);
			return(-1);
		}
		fname = name;
		/*
		 * set the MAKEFILE variable desired by System V fans -- the
		 * placement of the setting here means it gets set to the last
		 * makefile specified, as it is set by SysV make.
		 */
found:
		if (!doing_depend)
			Var_Set("MAKEFILE", fname, VAR_INTERNAL, 0);
		Parse_File(fname, fd);
	}
	free(path);
	return(0);
}



/*-
 * Cmd_Exec --
 *	Execute the command in cmd, and return the output of that command
 *	in a string.
 *
 * Results:
 *	A string containing the output of the command, or the empty string
 *	If errnum is not NULL, it contains the reason for the command failure
 *
 * Side Effects:
 *	The string must be freed by the caller.
 */
char *
Cmd_Exec(const char *cmd, const char **errnum)
{
    const char	*args[4];   	/* Args for invoking the shell */
    int 	fds[2];	    	/* Pipe streams */
    int 	cpid;	    	/* Child PID */
    int 	pid;	    	/* PID from wait() */
    char	*res;		/* result */
    int		status;		/* command exit status */
    Buffer	buf;		/* buffer to store the result */
    char	*cp;
    int		cc;		/* bytes read, or -1 */
    int		savederr;	/* saved errno */


    *errnum = NULL;

    if (!shellName)
	Shell_Init();
    /*
     * Set up arguments for shell
     */
    args[0] = shellName;
    args[1] = "-c";
    args[2] = cmd;
    args[3] = NULL;

    /*
     * Open a pipe for fetching its output
     */
    if (pipe(fds) == -1) {
	*errnum = "Couldn't create pipe for \"%s\"";
	goto bad;
    }

    /*
     * Fork
     */
    switch (cpid = vFork()) {
    case 0:
	/*
	 * Close input side of pipe
	 */
	(void)close(fds[0]);

	/*
	 * Duplicate the output stream to the shell's output, then
	 * shut the extra thing down. Note we don't fetch the error
	 * stream...why not? Why?
	 */
	(void)dup2(fds[1], 1);
	(void)close(fds[1]);

	Var_ExportVars();

	(void)execv(shellPath, UNCONST(args));
	_exit(1);
	/*NOTREACHED*/

    case -1:
	*errnum = "Couldn't exec \"%s\"";
	goto bad;

    default:
	/*
	 * No need for the writing half
	 */
	(void)close(fds[1]);

	savederr = 0;
	Buf_Init(&buf, 0);

	do {
	    char   result[BUFSIZ];
	    cc = read(fds[0], result, sizeof(result));
	    if (cc > 0)
		Buf_AddBytes(&buf, cc, result);
	}
	while (cc > 0 || (cc == -1 && errno == EINTR));
	if (cc == -1)
	    savederr = errno;

	/*
	 * Close the input side of the pipe.
	 */
	(void)close(fds[0]);

	/*
	 * Wait for the process to exit.
	 */
	while(((pid = waitpid(cpid, &status, 0)) != cpid) && (pid >= 0)) {
	    JobReapChild(pid, status, FALSE);
	    continue;
	}
	cc = Buf_Size(&buf);
	res = Buf_Destroy(&buf, FALSE);

	if (savederr != 0)
	    *errnum = "Couldn't read shell's output for \"%s\"";

	if (WIFSIGNALED(status))
	    *errnum = "\"%s\" exited on a signal";
	else if (WEXITSTATUS(status) != 0)
	    *errnum = "\"%s\" returned non-zero status";

	/*
	 * Null-terminate the result, convert newlines to spaces and
	 * install it in the variable.
	 */
	res[cc] = '\0';
	cp = &res[cc];

	if (cc > 0 && *--cp == '\n') {
	    /*
	     * A final newline is just stripped
	     */
	    *cp-- = '\0';
	}
	while (cp >= res) {
	    if (*cp == '\n') {
		*cp = ' ';
	    }
	    cp--;
	}
	break;
    }
    return res;
bad:
    res = bmake_malloc(1);
    *res = '\0';
    return res;
}

/*-
 * Error --
 *	Print an error message given its format.
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *	The message is printed.
 */
/* VARARGS */
void
Error(const char *fmt, ...)
{
	va_list ap;
	FILE *err_file;

	err_file = debug_file;
	if (err_file == stdout)
		err_file = stderr;
	(void)fflush(stdout);
	for (;;) {
		va_start(ap, fmt);
		fprintf(err_file, "%s: ", progname);
		(void)vfprintf(err_file, fmt, ap);
		va_end(ap);
		(void)fprintf(err_file, "\n");
		(void)fflush(err_file);
		if (err_file == stderr)
			break;
		err_file = stderr;
	}
}

/*-
 * Fatal --
 *	Produce a Fatal error message. If jobs are running, waits for them
 *	to finish.
 *
 * Results:
 *	None
 *
 * Side Effects:
 *	The program exits
 */
/* VARARGS */
void
Fatal(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	if (jobsRunning)
		Job_Wait();

	(void)fflush(stdout);
	(void)vfprintf(stderr, fmt, ap);
	va_end(ap);
	(void)fprintf(stderr, "\n");
	(void)fflush(stderr);

	PrintOnError(NULL, NULL);

	if (DEBUG(GRAPH2) || DEBUG(GRAPH3))
		Targ_PrintGraph(2);
	Trace_Log(MAKEERROR, 0);
	exit(2);		/* Not 1 so -q can distinguish error */
}

/*
 * Punt --
 *	Major exception once jobs are being created. Kills all jobs, prints
 *	a message and exits.
 *
 * Results:
 *	None
 *
 * Side Effects:
 *	All children are killed indiscriminately and the program Lib_Exits
 */
/* VARARGS */
void
Punt(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	(void)fflush(stdout);
	(void)fprintf(stderr, "%s: ", progname);
	(void)vfprintf(stderr, fmt, ap);
	va_end(ap);
	(void)fprintf(stderr, "\n");
	(void)fflush(stderr);

	PrintOnError(NULL, NULL);

	DieHorribly();
}

/*-
 * DieHorribly --
 *	Exit without giving a message.
 *
 * Results:
 *	None
 *
 * Side Effects:
 *	A big one...
 */
void
DieHorribly(void)
{
	if (jobsRunning)
		Job_AbortAll();
	if (DEBUG(GRAPH2))
		Targ_PrintGraph(2);
	Trace_Log(MAKEERROR, 0);
	exit(2);		/* Not 1, so -q can distinguish error */
}

/*
 * Finish --
 *	Called when aborting due to errors in child shell to signal
 *	abnormal exit.
 *
 * Results:
 *	None
 *
 * Side Effects:
 *	The program exits
 */
void
Finish(int errors)
	           	/* number of errors encountered in Make_Make */
{
	Fatal("%d error%s", errors, errors == 1 ? "" : "s");
}

/*
 * eunlink --
 *	Remove a file carefully, avoiding directories.
 */
int
eunlink(const char *file)
{
	struct stat st;

	if (lstat(file, &st) == -1)
		return -1;

	if (S_ISDIR(st.st_mode)) {
		errno = EISDIR;
		return -1;
	}
	return unlink(file);
}

/*
 * execError --
 *	Print why exec failed, avoiding stdio.
 */
void
execError(const char *af, const char *av)
{
#ifdef USE_IOVEC
	int i = 0;
	struct iovec iov[8];
#define IOADD(s) \
	(void)(iov[i].iov_base = UNCONST(s), \
	    iov[i].iov_len = strlen(iov[i].iov_base), \
	    i++)
#else
#define	IOADD(void)write(2, s, strlen(s))
#endif

	IOADD(progname);
	IOADD(": ");
	IOADD(af);
	IOADD("(");
	IOADD(av);
	IOADD(") failed (");
	IOADD(strerror(errno));
	IOADD(")\n");

#ifdef USE_IOVEC
	while (writev(2, iov, 8) == -1 && errno == EAGAIN)
	    continue;
#endif
}

/*
 * usage --
 *	exit with usage message
 */
static void
usage(void)
{
	char *p;
	if ((p = strchr(progname, '[')) != NULL)
	    *p = '\0';

	(void)fprintf(stderr,
"usage: %s [-BeikNnqrstWwX] \n\
            [-C directory] [-D variable] [-d flags] [-f makefile]\n\
            [-I directory] [-J private] [-j max_jobs] [-m directory] [-T file]\n\
            [-V variable] [variable=value] [target ...]\n", progname);
	exit(2);
}


int
PrintAddr(void *a, void *b)
{
    printf("%lx ", (unsigned long) a);
    return b ? 0 : 0;
}



void
PrintOnError(GNode *gn, const char *s)
{
    static GNode *en = NULL;
    char tmp[64];
    char *cp;

    if (s)
	printf("%s", s);
	
    printf("\n%s: stopped in %s\n", progname, curdir);

    if (en)
	return;				/* we've been here! */
    if (gn) {
	/*
	 * We can print this even if there is no .ERROR target.
	 */
	Var_Set(".ERROR_TARGET", gn->name, VAR_GLOBAL, 0);
    }
    strncpy(tmp, "${MAKE_PRINT_VAR_ON_ERROR:@v@$v='${$v}'\n@}",
	    sizeof(tmp) - 1);
    cp = Var_Subst(NULL, tmp, VAR_GLOBAL, 0);
    if (cp) {
	if (*cp)
	    printf("%s", cp);
	free(cp);
    }
    /*
     * Finally, see if there is a .ERROR target, and run it if so.
     */
    en = Targ_FindNode(".ERROR", TARG_NOCREATE);
    if (en) {
	en->type |= OP_SPECIAL;
	Compat_Make(en, en);
    }
}

void
Main_ExportMAKEFLAGS(Boolean first)
{
    static int once = 1;
    char tmp[64];
    char *s;

    if (once != first)
	return;
    once = 0;
    
    strncpy(tmp, "${.MAKEFLAGS} ${.MAKEOVERRIDES:O:u:@v@$v=${$v:Q}@}",
	    sizeof(tmp));
    s = Var_Subst(NULL, tmp, VAR_CMD, 0);
    if (s && *s) {
#ifdef POSIX
	setenv("MAKEFLAGS", s, 1);
#else
	setenv("MAKE", s, 1);
#endif
    }
}

char *
getTmpdir(void)
{
    static char *tmpdir = NULL;

    if (!tmpdir) {
	struct stat st;

	/*
	 * Honor $TMPDIR but only if it is valid.
	 * Ensure it ends with /.
	 */
	tmpdir = Var_Subst(NULL, "${TMPDIR:tA:U" _PATH_TMP "}/", VAR_GLOBAL, 0);
	if (stat(tmpdir, &st) < 0 || !S_ISDIR(st.st_mode)) {
	    free(tmpdir);
	    tmpdir = bmake_strdup(_PATH_TMP);
	}
    }
    return tmpdir;
}

/*
 * Create and open a temp file using "pattern".
 * If "fnamep" is provided set it to a copy of the filename created.
 * Otherwise unlink the file once open.
 */
int
mkTempFile(const char *pattern, char **fnamep)
{
    static char *tmpdir = NULL;
    char tfile[MAXPATHLEN];
    int fd;
    
    if (!pattern)
	pattern = TMPPAT;
    if (!tmpdir)
	tmpdir = getTmpdir();
    if (pattern[0] == '/') {
	snprintf(tfile, sizeof(tfile), "%s", pattern);
    } else {
	snprintf(tfile, sizeof(tfile), "%s%s", tmpdir, pattern);
    }
    if ((fd = mkstemp(tfile)) < 0)
	Punt("Could not create temporary file %s: %s", tfile, strerror(errno));
    if (fnamep) {
	*fnamep = bmake_strdup(tfile);
    } else {
	unlink(tfile);			/* we just want the descriptor */
    }
    return fd;
}


/*
 * Return a Boolean based on setting of a knob.
 *
 * If the knob is not set, the supplied default is the return value.
 * If set, anything that looks or smells like "No", "False", "Off", "0" etc,
 * is FALSE, otherwise TRUE.
 */
Boolean
getBoolean(const char *name, Boolean bf)
{
    char tmp[64];
    char *cp;

    if (snprintf(tmp, sizeof(tmp), "${%s:tl}", name) < (int)(sizeof(tmp))) {
	cp = Var_Subst(NULL, tmp, VAR_GLOBAL, 0);

	if (cp) {
	    switch(*cp) {
	    case '\0':			/* not set - the default wins */
		break;
	    case '0':
	    case 'f':
	    case 'n':
		bf = FALSE;
		break;
	    case 'o':
		switch (cp[1]) {
		case 'f':
		    bf = FALSE;
		    break;
		default:
		    bf = TRUE;
		    break;
		}
		break;
	    default:
		bf = TRUE;
		break;
	    }
	    free(cp);
	}
    }
    return (bf);
}

