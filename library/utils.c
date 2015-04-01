/*
twopence utility functions.

Copyright (C) 2014-2015 SUSE

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 2.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <string.h>
#include <sys/time.h>
#include "utils.h"

void
twopence_timeout_init(twopence_timeout_t *tmo)
{
	gettimeofday(&tmo->now, NULL);
	timerclear(&tmo->until);
}

bool
twopence_timeout_update(twopence_timeout_t *tmo, const struct timeval *deadline)
{
	struct timeval delta;

	if (deadline->tv_sec == 0)
		return true;

	if (timercmp(&tmo->now, deadline, >=))
		return false; /* expired */

	/* deadline is still in the future. Figure out how much longer we have */
	timersub(deadline, &tmo->now, &delta);
	if (!timerisset(&tmo->until) || timercmp(&delta, &tmo->until, <))
		tmo->until = delta;

	return true;
}

long
twopence_timeout_msec(const twopence_timeout_t *tmo)
{
	if (!timerisset(&tmo->until))
		return -1;
	return 1000 * tmo->until.tv_sec + tmo->until.tv_usec / 1000;
}
