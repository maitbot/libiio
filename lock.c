// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * libiio - Library for interfacing industrial I/O (IIO) devices
 *
 * Copyright (C) 2015 Analog Devices, Inc.
 * Author: Paul Cercueil <paul.cercueil@analog.com>
 */

#include "iio-config.h"
#include "iio-lock.h"

#ifndef NO_THREADS
#include <pthread.h>
#endif

#include <stdlib.h>

struct iio_mutex {
#ifdef NO_THREADS
	int foo; /* avoid complaints about empty structure */
#else
	pthread_mutex_t lock;
#endif
#endif
};

struct iio_mutex * iio_mutex_create(void)
{
	struct iio_mutex *lock = malloc(sizeof(*lock));

	if (!lock)
		return NULL;

#ifndef NO_THREADS
	pthread_mutex_init(&lock->lock, NULL);
#endif
	return lock;
}

void iio_mutex_destroy(struct iio_mutex *lock)
{
#ifndef NO_THREADS
	pthread_mutex_destroy(&lock->lock);
#endif
	free(lock);
}

void iio_mutex_lock(struct iio_mutex *lock)
{
#ifndef NO_THREADS
	pthread_mutex_lock(&lock->lock);
#endif
}

void iio_mutex_unlock(struct iio_mutex *lock)
{
#ifndef NO_THREADS
	pthread_mutex_unlock(&lock->lock);
#endif
}
