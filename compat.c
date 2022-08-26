// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * libiio - Library for interfacing industrial I/O (IIO) devices
 *
 * Libiio 0.x to 1.x compat library
 *
 * Copyright (C) 2022 Analog Devices, Inc.
 * Author: Paul Cercueil <paul.cercueil@analog.com>
 */

#include "dynamic.h"
#include "iio-config.h"

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define snprintf sprintf_s
#endif

#if (defined(_WIN32) || defined(__MBED__))
#ifndef _SSIZE_T_DEFINED
typedef ptrdiff_t ssize_t;
#define _SSIZE_T_DEFINED
#endif
#else
#include <sys/types.h>
#endif

struct iio_block;
struct iio_buffer;
struct iio_channel;
struct iio_channels_mask;
struct iio_context;
struct iio_context_params;
struct iio_data_format;
struct iio_device;
struct iio_scan;
struct iio_stream;

struct compat {
	void *lib;

	/* Scan */
	struct iio_scan * (*iio_scan)(const struct iio_context_params *, const char *);
	void (*iio_scan_destroy)(struct iio_scan *);
	size_t (*iio_scan_get_results_count)(const struct iio_scan *);
	const char * (*iio_scan_get_description)(const struct iio_scan *, size_t);
	const char * (*iio_scan_get_uri)(const struct iio_scan *, size_t );

	/* Backends */
	bool (*iio_has_backend)(const char *);
	unsigned int (*iio_get_backends_count)(void);
	const char * (*iio_get_backend)(unsigned int);

	/* Context */
	struct iio_context * (*iio_create_context)(const struct iio_context_params *,
						   const char *);
	void (*iio_context_destroy)(struct iio_context *);

	void (*iio_context_set_data)(struct iio_context *, void *);
	void * (*iio_context_get_data)(const struct iio_context *);

	const char * (*iio_context_get_version_tag)(const struct iio_context *);
	unsigned int (*iio_context_get_version_major)(const struct iio_context *);
	unsigned int (*iio_context_get_version_minor)(const struct iio_context *);

	const char * (*iio_context_get_name)(const struct iio_context *);
	const char * (*iio_context_get_description)(const struct iio_context *);
	const char * (*iio_context_get_xml)(const struct iio_context *);
	const struct iio_context_params *
		(*iio_context_get_params)(const struct iio_context *);

	unsigned int (*iio_context_get_devices_count)(const struct iio_context *);
	struct iio_device * (*iio_context_get_device)(const struct iio_context *,
						      unsigned int);
	struct iio_device * (*iio_context_find_device)(const struct iio_context *,
						       const char *);

	int (*iio_context_set_timeout)(struct iio_context *, unsigned int);

	/* Context attributes */
	unsigned int (*iio_context_get_attrs_count)(const struct iio_context *);
	int (*iio_context_get_attr)(const struct iio_context *, unsigned int,
				    const char **, const char **);
	const char * (*iio_context_get_attr_value)(const struct iio_context *,
						   const char *);

	/* Devices */
	const struct iio_context * (*iio_device_get_context)(const struct iio_device *);
	const char * (*iio_device_get_id)(const struct iio_device *);
	const char * (*iio_device_get_name)(const struct iio_device *);
	const char * (*iio_device_get_label)(const struct iio_device *);

	void * (*iio_device_get_data)(const struct iio_device *);
	void (*iio_device_set_data)(struct iio_device *, void *);

	int (*iio_device_identify_filename)(const struct iio_device *dev,
					    const char *filename,
					    struct iio_channel **chn,
					    const char **attr);
	int (*iio_device_reg_read)(struct iio_device *, uint32_t, uint32_t *);
	int (*iio_device_reg_write)(struct iio_device *, uint32_t, uint32_t);

	unsigned int (*iio_device_get_channels_count)(const struct iio_device *);
	struct iio_channel * (*iio_device_get_channel)(const struct iio_device *,
						       unsigned int);
	struct iio_channel * (*iio_device_find_channel)(const struct iio_device *,
							const char *, bool);

	unsigned int (*iio_device_get_attrs_count)(const struct iio_device *);
	const char * (*iio_device_get_attr)(const struct iio_device *,
					    unsigned int);
	const char * (*iio_device_find_attr)(const struct iio_device *,
					     const char *);
	ssize_t (*iio_device_attr_read_raw)(const struct iio_device *,
					    const char *, char *, size_t);
	int (*iio_device_attr_read_bool)(const struct iio_device *,
					 const char *, bool *);
	int (*iio_device_attr_read_longlong)(const struct iio_device *,
					     const char *, long long *);
	int (*iio_device_attr_read_double)(const struct iio_device *,
					   const char *, double *);
	ssize_t (*iio_device_attr_write_raw)(const struct iio_device *,
					     const char *, const void *,
					     size_t);
	ssize_t (*iio_device_attr_write_string)(const struct iio_device *,
						const char *, const char *);
	int (*iio_device_attr_write_bool)(const struct iio_device *,
					  const char *, bool);
	int (*iio_device_attr_write_longlong)(const struct iio_device *,
					      const char *, long long);
	int (*iio_device_attr_write_double)(const struct iio_device *,
					    const char *, double);
	unsigned int (*iio_device_get_buffer_attrs_count)(const struct iio_device *);
	const char * (*iio_device_get_buffer_attr)(const struct iio_device *,
						   unsigned int);
	const char * (*iio_device_find_buffer_attr)(const struct iio_device *,
						    const char *);
	ssize_t (*iio_device_buffer_attr_read_raw)(const struct iio_device *,
						   const char *, char *, size_t);
	int (*iio_device_buffer_attr_read_bool)(const struct iio_device *,
						const char *, bool *);
	int (*iio_device_buffer_attr_read_longlong)(const struct iio_device *,
						    const char *, long long *);
	int (*iio_device_buffer_attr_read_double)(const struct iio_device *,
						  const char *, double *);
	ssize_t (*iio_device_buffer_attr_write_raw)(const struct iio_device *,
						    const char *, const void *,
						    size_t);
	ssize_t (*iio_device_buffer_attr_write_string)(const struct iio_device *,
						       const char *, const char *);
	int (*iio_device_buffer_attr_write_bool)(const struct iio_device *,
						 const char *, bool);
	int (*iio_device_buffer_attr_write_longlong)(const struct iio_device *,
						     const char *, long long);
	int (*iio_device_buffer_attr_write_double)(const struct iio_device *,
						   const char *, double);
	unsigned int (*iio_device_get_debug_attrs_count)(const struct iio_device *);
	const char * (*iio_device_get_debug_attr)(const struct iio_device *,
						  unsigned int);
	const char * (*iio_device_find_debug_attr)(const struct iio_device *,
						   const char *);
	ssize_t (*iio_device_debug_attr_read_raw)(const struct iio_device *,
						  const char *, char *, size_t);
	int (*iio_device_debug_attr_read_bool)(const struct iio_device *,
					       const char *, bool *);
	int (*iio_device_debug_attr_read_longlong)(const struct iio_device *,
						   const char *, long long *);
	int (*iio_device_debug_attr_read_double)(const struct iio_device *,
						 const char *, double *);
	ssize_t (*iio_device_debug_attr_write_raw)(const struct iio_device *,
						   const char *, const void *,
						   size_t);
	ssize_t (*iio_device_debug_attr_write_string)(const struct iio_device *,
						      const char *, const char *);
	int (*iio_device_debug_attr_write_bool)(const struct iio_device *,
						const char *, bool);
	int (*iio_device_debug_attr_write_longlong)(const struct iio_device *,
						    const char *, long long);
	int (*iio_device_debug_attr_write_double)(const struct iio_device *,
						  const char *, double);

	int (*iio_device_get_trigger)(const struct iio_device *,
				      const struct iio_device **);
	int (*iio_device_set_trigger)(const struct iio_device *,
				      const struct iio_device *);
	bool (*iio_device_is_trigger)(const struct iio_device *);

	ssize_t (*iio_device_get_sample_size)(const struct iio_device *,
					      const struct iio_channels_mask *);

	/* Channels */
	const struct iio_device * (*iio_channel_get_device)(const struct iio_channel *);
	const char * (*iio_channel_get_id)(const struct iio_channel *);
	const char * (*iio_channel_get_name)(const struct iio_channel *);
	bool (*iio_channel_is_output)(const struct iio_channel *);
	bool (*iio_channel_is_scan_element)(const struct iio_channel *);

	void (*iio_channel_set_data)(struct iio_channel *, void *);
	void * (*iio_channel_get_data)(const struct iio_channel *);

	unsigned int (*iio_channel_get_attrs_count)(const struct iio_channel *);
	const char * (*iio_channel_get_attr)(const struct iio_channel *,
					     unsigned int);
	const char * (*iio_channel_find_attr)(const struct iio_channel *,
					      const char *);
	const char * (*iio_channel_attr_get_filename)(const struct iio_channel *,
						      const char *);
	ssize_t (*iio_channel_attr_read_raw)(const struct iio_channel *,
					     const char *, char *, size_t);
	int (*iio_channel_attr_read_bool)(const struct iio_channel *,
					  const char *, bool *);
	int (*iio_channel_attr_read_longlong)(const struct iio_channel *,
					      const char *, long long *);
	int (*iio_channel_attr_read_double)(const struct iio_channel *,
					    const char *, double *);
	ssize_t (*iio_channel_attr_write_raw)(const struct iio_channel *,
					      const char *, const void *, size_t);
	ssize_t (*iio_channel_attr_write_string)(const struct iio_channel *,
						 const char *, const char *);
	int (*iio_channel_attr_write_bool)(const struct iio_channel *,
					   const char *, bool);
	int (*iio_channel_attr_write_longlong)(const struct iio_channel *,
					       const char *, long long);
	int (*iio_channel_attr_write_double)(const struct iio_channel *,
					     const char *, double);
	unsigned int (*iio_channel_get_type)(const struct iio_channel *);
	unsigned int (*iio_channel_get_modifier)(const struct iio_channel *);
	long (*iio_channel_get_index)(const struct iio_channel *);
	const struct iio_data_format *
		(*iio_channel_get_data_format)(const struct iio_channel *);

	void (*iio_channel_convert)(const struct iio_channel *,
				    void *, const void *);
	void (*iio_channel_convert_inverse)(const struct iio_channel *,
					    void *, const void *);

	void (*iio_channel_enable)(const struct iio_channel *,
				   struct iio_channels_mask *);
	void (*iio_channel_disable)(const struct iio_channel *,
				    struct iio_channels_mask *);
	bool (*iio_channel_is_enabled)(const struct iio_channel *,
				       const struct iio_channels_mask *);

	/* Channel masks */
	struct iio_channels_mask * (*iio_create_channels_mask)(unsigned int);
	void (*iio_channels_mask_destroy)(struct iio_channels_mask *);

	/* Buffers */
	struct iio_buffer * (*iio_device_create_buffer)(const struct iio_device *,
							unsigned int,
							const struct iio_channels_mask *);
	void (*iio_buffer_destroy)(struct iio_buffer *);
	void (*iio_buffer_cancel)(struct iio_buffer *);

	const struct iio_device * (*iio_buffer_get_device)(const struct iio_buffer *);
	const struct iio_channels_mask *
		(*iio_buffer_get_channels_mask)(const struct iio_buffer *);
	void (*iio_buffer_set_data)(struct iio_buffer *, void *);
	void * (*iio_buffer_get_data)(const struct iio_buffer *);

	/* Stream */
	struct iio_stream * (*iio_buffer_create_stream)(struct iio_buffer *,
							size_t, size_t);
	void (*iio_stream_destroy)(struct iio_stream *);
	const struct iio_block * (*iio_stream_get_next_block)(struct iio_stream *);

	/* Blocks */
	struct iio_buffer * (*iio_block_get_buffer)(const struct iio_block *);
	void * (*iio_block_start)(const struct iio_block *);
	void * (*iio_block_first)(const struct iio_block *,
				  const struct iio_channel *);
	void * (*iio_block_end)(const struct iio_block *);
	ssize_t (*iio_block_foreach_sample)(const struct iio_block *,
					    const struct iio_channels_mask *,
					    ssize_t (*callback)(const struct iio_channel *,
								void *, size_t, void *),
					    void *);

	/* Misc */
	size_t (*iio_strlcpy)(char * __restrict, const char * __restrict, size_t);
	char *(*iio_strdup)(const char *);
	void (*iio_strerror)(int, char *, size_t);
};

static struct compat *compat_lib_ptr;

#define IIO_CALL(fn) (*compat_lib_ptr->fn)

struct iio_context_info {
	char *description;
	char *uri;
};

struct iio_scan_block {
	struct iio_scan *ctx;
	struct iio_context_info **info;
	ssize_t ctx_cnt;
};

struct iio_channel_compat {
	void *userdata;
};

struct iio_device_compat {
	void *userdata;

	struct iio_channels_mask *mask;
	unsigned int nb_kernel_buffers;
	bool is_tx;

	unsigned int nb_channels;
};

struct iio_context_compat {
	void *userdata;

	unsigned int nb_devices;
};

struct iio_buffer_compat {
	void *userdata;

	struct iio_stream *stream;
	const struct iio_block *block;

	size_t size;
	bool cyclic, nonblock;
};

static inline int iio_err(const void *ptr)
{
	return (uintptr_t) ptr >= (uintptr_t) -4095 ? (int)(intptr_t) ptr : 0;
}

static bool iio_device_is_tx(const struct iio_device *dev)
{
	struct iio_channel *chn;
	unsigned int i, nb_channels;

	nb_channels = IIO_CALL(iio_device_get_channels_count)(dev);

	for (i = 0; i < nb_channels; i++) {
		chn = IIO_CALL(iio_device_get_channel)(dev, i);

		if (IIO_CALL(iio_channel_is_output)(chn)
		    && IIO_CALL(iio_channel_is_scan_element)(chn)) {
			return true;
		}
	}

	return false;
}

static int iio_init_context_compat(struct iio_context *ctx)
{
	struct iio_context_compat *compat;
	struct iio_device_compat *dev_compat;
	struct iio_channel_compat *chn_compat;
	struct iio_device *dev;
	struct iio_channel *chn;
	size_t size = sizeof(*compat);
	unsigned int i, j, nb_devices, nb_channels;
	int err;

	nb_devices = IIO_CALL(iio_context_get_devices_count)(ctx);

	for (i = 0; i < nb_devices; i++) {
		dev = IIO_CALL(iio_context_get_device)(ctx, i);
		nb_channels = IIO_CALL(iio_device_get_channels_count)(dev);

		size += sizeof(struct iio_device_compat)
			+ nb_channels * sizeof(struct iio_channel_compat);
	}

	compat = calloc(1, size);
	if (!compat)
		return -ENOMEM;

	compat->nb_devices = nb_devices;

	for (i = 0; i < nb_devices; i++) {
		dev = IIO_CALL(iio_context_get_device)(ctx, i);
		nb_channels = IIO_CALL(iio_device_get_channels_count)(dev);

		dev_compat = (void *)((uintptr_t)compat
				      + sizeof(*compat)
				      + i * sizeof(*dev_compat));
		dev_compat->nb_channels = nb_channels;
		dev_compat->is_tx = iio_device_is_tx(dev);

		IIO_CALL(iio_device_set_data)(dev, dev_compat);

		dev_compat->mask = IIO_CALL(iio_create_channels_mask)(nb_channels);
		if (!dev_compat->mask) {
			err = -ENOMEM;
			goto err_destroy_masks;
		}

		dev_compat->nb_kernel_buffers = 4;

		for (j = 0; j < nb_channels; j++) {
			chn = IIO_CALL(iio_device_get_channel)(dev, j);

			chn_compat = (void *)((uintptr_t)compat
					      + sizeof(*compat)
					      + nb_devices * sizeof(*dev_compat)
					      + j * sizeof(*chn_compat));
			IIO_CALL(iio_channel_set_data)(chn, chn_compat);
		}
	}

	return 0;

err_destroy_masks:
	for (; i > 0; i--) {
		dev = IIO_CALL(iio_context_get_device)(ctx, i - 1);
		dev_compat = IIO_CALL(iio_device_get_data)(dev);
		IIO_CALL(iio_channels_mask_destroy)(dev_compat->mask);
	}
	free(compat);
	return err;
}

struct iio_context * iio_create_context_from_uri(const char *uri)
{
	struct iio_context *ctx;
	int err;

	ctx = IIO_CALL(iio_create_context)(NULL, uri);
	err = iio_err(ctx);
	if (err)
		goto err_set_errno;

	err = iio_init_context_compat(ctx);
	if (err)
		goto err_destroy_context;

	return ctx;

err_destroy_context:
	IIO_CALL(iio_context_destroy)(ctx);
err_set_errno:
	errno = -err;
	return NULL;
}

struct iio_context * iio_context_clone(const struct iio_context *old_ctx)
{
	const struct iio_context_params *params;
	struct iio_context *ctx;
	const char *uri;
	int err = -ENOENT;

	uri = IIO_CALL(iio_context_get_attr_value)(old_ctx, "uri");
	if (!uri)
		goto err_set_errno;

	params = IIO_CALL(iio_context_get_params)(old_ctx);

	ctx = IIO_CALL(iio_create_context)(params, uri);
	err = iio_err(ctx);
	if (err)
		goto err_set_errno;

	err = iio_init_context_compat(ctx);
	if (err)
		goto err_destroy_context;

	return ctx;

err_destroy_context:
	IIO_CALL(iio_context_destroy)(ctx);
err_set_errno:
	errno = -err;
	return NULL;
}

void iio_context_destroy(struct iio_context *ctx)
{
	struct iio_context_compat *compat;

	compat = IIO_CALL(iio_context_get_data)(ctx);
	IIO_CALL(iio_context_destroy)(ctx);
	free(compat);
}

static struct iio_context *
create_context_with_arg(const char *prefix, const char *arg)
{
	char buf[256];

	snprintf(buf, sizeof(buf), "%s%s", prefix, arg);

	return iio_create_context_from_uri(buf);
}

struct iio_context * iio_create_xml_context(const char *xml_file)
{
	return create_context_with_arg("xml:", xml_file);
}

struct iio_context * iio_create_xml_context_mem(const char *xml, size_t len)
{
	size_t uri_len = sizeof("xml:") - 1;
	struct iio_context *ctx;
	char *uri;

	uri = malloc(len + uri_len + 1);
	memcpy(uri, "xml:", uri_len);
	memcpy(uri + uri_len, xml, len);
	uri[uri_len + len] = '\0';

	ctx = iio_create_context_from_uri(uri);
	free(uri);

	return ctx;
}

struct iio_context * iio_create_network_context(const char *hostname)
{
	return create_context_with_arg("ip:", hostname);
}

struct iio_context * iio_create_local_context(void)
{
	return iio_create_context_from_uri("local:");
}

struct iio_context * iio_create_default_context(void)
{
	return iio_create_context_from_uri(NULL);
}

void iio_context_info_list_free(struct iio_context_info **list)
{
	struct iio_context_info **it, *info;

	if (!list)
		return;

	for (it = list; *it; it++) {
		info = *it;

		free(info->description);
		free(info->uri);
	}

	free(list);
}

struct iio_scan *
iio_create_scan_context(const char *backends, unsigned int flags)
{
	struct iio_scan *ctx;
	char buf[256];
	char *ptr = NULL;
	int err;

	if (backends) {
		IIO_CALL(iio_strlcpy)(buf, backends, sizeof(buf));

		/* iio_scan() requires a comma-separated list of backends */
		for (ptr = buf; *ptr; ptr++) {
			if (*ptr == ':')
				*ptr = ',';
		}

		ptr = buf;
	}

	ctx = IIO_CALL(iio_scan)(NULL, ptr);
	err = iio_err(ctx);
	if (err) {
		errno = -err;
		return NULL;
	}

	return ctx;
}

void iio_scan_context_destroy(struct iio_scan *ctx)
{
	IIO_CALL(iio_scan_destroy)(ctx);
}

ssize_t iio_scan_context_get_info_list(struct iio_scan *ctx,
				       struct iio_context_info ***info)
{
	struct iio_context_info *results, **results_ptr;
	size_t nb = IIO_CALL(iio_scan_get_results_count)(ctx);
	unsigned int i;
	int ret = -ENOMEM;
	const char *ptr;
	char *dup;

	results_ptr = malloc((nb + 1) * sizeof(*results_ptr));
	if (!results_ptr)
		return -ENOMEM;

	results_ptr[nb] = NULL;

	results = malloc(nb * sizeof(*results));
	if (!results)
		goto out_free_results_ptr;

	for (i = 0; i < nb; i++) {
		ptr = IIO_CALL(iio_scan_get_description)(ctx, i);
		dup = IIO_CALL(iio_strdup)(ptr);
		if (!dup)
			goto out_free_results_list;

		results[i].description = dup;

		ptr = IIO_CALL(iio_scan_get_uri)(ctx, i);
		dup = IIO_CALL(iio_strdup)(ptr);
		if (!dup)
			goto out_free_results_list;

		results[i].uri = dup;

		results_ptr[i] = &results[i];
	}

	*info = results_ptr;

	return nb;

out_free_results_list:
	iio_context_info_list_free(results_ptr);
	free(results);
out_free_results_ptr:
	free(results_ptr);
	return ret;
}

const char *
iio_context_info_get_description(const struct iio_context_info *info)
{
	return info->description;
}

const char * iio_context_info_get_uri(const struct iio_context_info *info)
{
	return info->uri;
}

ssize_t iio_scan_block_scan(struct iio_scan_block *blk)
{
	iio_context_info_list_free(blk->info);
	blk->info = NULL;
	blk->ctx_cnt = iio_scan_context_get_info_list(blk->ctx, &blk->info);

	return blk->ctx_cnt;
}

struct iio_context_info *iio_scan_block_get_info(
		struct iio_scan_block *blk, unsigned int index)
{
	if (!blk->info || (ssize_t)index >= blk->ctx_cnt) {
		errno = EINVAL;
		return NULL;
	}

	return blk->info[index];
}

struct iio_scan_block *iio_create_scan_block(
		const char *backend, unsigned int flags)
{
	struct iio_scan_block *blk;

	blk = calloc(1, sizeof(*blk));
	if (!blk) {
		errno = ENOMEM;
		return NULL;
	}

	blk->ctx = iio_create_scan_context(backend, flags);
	if (!blk->ctx) {
		free(blk);
		return NULL;
	}

	return blk;
}

void iio_scan_block_destroy(struct iio_scan_block *blk)
{
	iio_context_info_list_free(blk->info);
	iio_scan_context_destroy(blk->ctx);
	free(blk);
}

int iio_context_get_version(const struct iio_context *ctx,
			    unsigned int *major, unsigned int *minor,
			    char git_tag[8])
{
	const char *tag = IIO_CALL(iio_context_get_version_tag)(ctx);

	if (git_tag)
		IIO_CALL(iio_strlcpy)(git_tag, tag, 8);
	if (major)
		*major = IIO_CALL(iio_context_get_version_major)(ctx);
	if (minor)
		*minor = IIO_CALL(iio_context_get_version_minor)(ctx);

	return 0;
}

const char * iio_context_get_name(const struct iio_context *ctx)
{
	return IIO_CALL(iio_context_get_name)(ctx);
}

const char * iio_context_get_description(const struct iio_context *ctx)
{
	return IIO_CALL(iio_context_get_description)(ctx);
}

const char * iio_context_get_xml(const struct iio_context *ctx)
{
	return IIO_CALL(iio_context_get_xml)(ctx);
}

unsigned int iio_context_get_devices_count(const struct iio_context *ctx)
{
	return IIO_CALL(iio_context_get_devices_count)(ctx);
}

struct iio_device * iio_context_get_device(const struct iio_context *ctx,
					   unsigned int index)
{
	return IIO_CALL(iio_context_get_device)(ctx, index);
}

struct iio_device *
iio_context_find_device(const struct iio_context *ctx, const char *name)
{
	return IIO_CALL(iio_context_find_device)(ctx, name);
}

int iio_context_set_timeout(struct iio_context *ctx, unsigned int timeout_ms)
{
	return IIO_CALL(iio_context_set_timeout)(ctx, timeout_ms);
}

unsigned int iio_context_get_attrs_count(const struct iio_context *ctx)
{
	return IIO_CALL(iio_context_get_attrs_count)(ctx);
}

int iio_context_get_attr(const struct iio_context *ctx, unsigned int index,
			 const char **name, const char **value)
{
	return IIO_CALL(iio_context_get_attr)(ctx, index, name, value);
}

const char * iio_context_get_attr_value(const struct iio_context *ctx,
					const char *name)
{
	return IIO_CALL(iio_context_get_attr_value)(ctx, name);
}

const struct iio_context * iio_device_get_context(const struct iio_device *dev)
{
	return IIO_CALL(iio_device_get_context)(dev);
}

const char * iio_device_get_id(const struct iio_device *dev)
{
	return IIO_CALL(iio_device_get_id)(dev);
}

const char * iio_device_get_name(const struct iio_device *dev)
{
	return IIO_CALL(iio_device_get_name)(dev);
}

const char * iio_device_get_label(const struct iio_device *dev)
{
	return IIO_CALL(iio_device_get_label)(dev);
}

void * iio_device_get_data(const struct iio_device *dev)
{
	struct iio_device_compat *compat = IIO_CALL(iio_device_get_data)(dev);

	return compat->userdata;
}

void iio_device_set_data(struct iio_device *dev, void *data)
{
	struct iio_device_compat *compat = IIO_CALL(iio_device_get_data)(dev);

	compat->userdata = data;
}

int iio_device_identify_filename(const struct iio_device *dev,
				 const char *filename, struct iio_channel **chn,
				 const char **attr)
{
	return IIO_CALL(iio_device_identify_filename)(dev, filename, chn, attr);
}

int iio_device_reg_read(struct iio_device *dev,
			uint32_t address, uint32_t *value)
{
	return IIO_CALL(iio_device_reg_read)(dev, address, value);
}

int iio_device_reg_write(struct iio_device *dev,
			 uint32_t address, uint32_t value)
{
	return IIO_CALL(iio_device_reg_write)(dev, address, value);
}

unsigned int iio_device_get_channels_count(const struct iio_device *dev)
{
	return IIO_CALL(iio_device_get_channels_count)(dev);
}

unsigned int iio_device_get_attrs_count(const struct iio_device *dev)
{
	return IIO_CALL(iio_device_get_attrs_count)(dev);
}

const char * iio_device_get_attr(const struct iio_device *dev,
				 unsigned int index)
{
	return IIO_CALL(iio_device_get_attr)(dev, index);
}

const char * iio_device_find_attr(const struct iio_device *dev,
				  const char *name)
{
	return IIO_CALL(iio_device_find_attr)(dev, name);
}

unsigned int iio_device_get_buffer_attrs_count(const struct iio_device *dev)
{
	return IIO_CALL(iio_device_get_buffer_attrs_count)(dev);
}

const char * iio_device_get_buffer_attr(const struct iio_device *dev,
					unsigned int index)
{
	return IIO_CALL(iio_device_get_buffer_attr)(dev, index);
}

const char * iio_device_find_buffer_attr(const struct iio_device *dev,
					 const char *name)
{
	return IIO_CALL(iio_device_find_buffer_attr)(dev, name);
}

unsigned int iio_device_get_debug_attrs_count(const struct iio_device *dev)
{
	return IIO_CALL(iio_device_get_debug_attrs_count)(dev);
}

const char * iio_device_get_debug_attr(const struct iio_device *dev,
				       unsigned int index)
{
	return IIO_CALL(iio_device_get_debug_attr)(dev, index);
}

const char * iio_device_find_debug_attr(const struct iio_device *dev,
					const char *name)
{
	return IIO_CALL(iio_device_find_debug_attr)(dev, name);
}

struct iio_channel * iio_device_get_channel(const struct iio_device *dev,
					    unsigned int index)
{
	return IIO_CALL(iio_device_get_channel)(dev, index);
}

struct iio_channel * iio_device_find_channel(const struct iio_device *dev,
					     const char *name, bool output)
{
	return IIO_CALL(iio_device_find_channel)(dev, name, output);
}

ssize_t iio_device_attr_read(const struct iio_device *dev,
			     const char *attr, char *dst, size_t len)
{
	return IIO_CALL(iio_device_attr_read_raw)(dev, attr, dst, len);
}

int iio_device_attr_read_bool(const struct iio_device *dev,
			      const char *attr, bool *val)
{
	return IIO_CALL(iio_device_attr_read_bool)(dev, attr, val);
}

int iio_device_attr_read_longlong(const struct iio_device *dev,
				  const char *attr, long long *val)
{
	return IIO_CALL(iio_device_attr_read_longlong)(dev, attr, val);
}

int iio_device_attr_read_double(const struct iio_device *dev,
				const char *attr, double *val)
{
	return IIO_CALL(iio_device_attr_read_double)(dev, attr, val);
}

int iio_device_attr_read_all(struct iio_device *dev,
			     int (*cb)(struct iio_device *dev, const char *attr,
				       const char *value, size_t len, void *d),
			     void *data)
{
	unsigned int i, nb_attrs;
	size_t len = 0x100000;
	const char *attr;
	char *buf;
	ssize_t count;
	int ret = 0;

	buf = malloc(len);
	if (!buf)
		return -ENOMEM;

	nb_attrs = IIO_CALL(iio_device_get_attrs_count)(dev);

	for (i = 0; i < nb_attrs; i++) {
		attr = IIO_CALL(iio_device_get_attr)(dev, i);
		count = IIO_CALL(iio_device_attr_read_raw)(dev, attr, buf, len);
		if (count < 0) {
			ret = (int) count;
			goto out_free_buffer;
		}

		ret = (*cb)(dev, attr, buf, (size_t) count, data);
		if (ret < 0)
			goto out_free_buffer;
	}

out_free_buffer:
	free(buf);
	return ret;
}

ssize_t iio_device_buffer_attr_read(const struct iio_device *dev,
				    const char *attr, char *dst, size_t len)
{
	return IIO_CALL(iio_device_buffer_attr_read_raw)(dev, attr, dst, len);
}

int iio_device_buffer_attr_read_bool(const struct iio_device *dev,
				     const char *attr, bool *val)
{
	return IIO_CALL(iio_device_buffer_attr_read_bool)(dev, attr, val);
}

int iio_device_buffer_attr_read_longlong(const struct iio_device *dev,
					 const char *attr, long long *val)
{
	return IIO_CALL(iio_device_buffer_attr_read_longlong)(dev, attr, val);
}

int iio_device_buffer_attr_read_double(const struct iio_device *dev,
				       const char *attr, double *val)
{
	return IIO_CALL(iio_device_buffer_attr_read_double)(dev, attr, val);
}

int iio_device_buffer_attr_read_all(struct iio_device *dev,
				    int (*cb)(struct iio_device *dev,
					      const char *attr,
					      const char *value,
					      size_t len, void *d),
			     void *data)
{
	unsigned int i, nb_attrs;
	size_t len = 0x100000;
	const char *attr;
	char *buf;
	ssize_t count;
	int ret = 0;

	buf = malloc(len);
	if (!buf)
		return -ENOMEM;

	nb_attrs = IIO_CALL(iio_device_get_buffer_attrs_count)(dev);

	for (i = 0; i < nb_attrs; i++) {
		attr = IIO_CALL(iio_device_get_buffer_attr)(dev, i);
		count = IIO_CALL(iio_device_buffer_attr_read_raw)(dev, attr,
								  buf, len);
		if (count < 0) {
			ret = (int) count;
			goto out_free_buffer;
		}

		ret = (*cb)(dev, attr, buf, (size_t) count, data);
		if (ret < 0)
			goto out_free_buffer;
	}

out_free_buffer:
	free(buf);
	return ret;
}

ssize_t iio_device_debug_attr_read(const struct iio_device *dev,
				   const char *attr, char *dst, size_t len)
{
	return IIO_CALL(iio_device_debug_attr_read_raw)(dev, attr, dst, len);
}

int iio_device_debug_attr_read_bool(const struct iio_device *dev,
				    const char *attr, bool *val)
{
	return IIO_CALL(iio_device_debug_attr_read_bool)(dev, attr, val);
}

int iio_device_debug_attr_read_longlong(const struct iio_device *dev,
					const char *attr, long long *val)
{
	return IIO_CALL(iio_device_debug_attr_read_longlong)(dev, attr, val);
}

int iio_device_debug_attr_read_double(const struct iio_device *dev,
				      const char *attr, double *val)
{
	return IIO_CALL(iio_device_debug_attr_read_double)(dev, attr, val);
}

int iio_device_debug_attr_read_all(struct iio_device *dev,
				   int (*cb)(struct iio_device *dev,
					     const char *attr,
					     const char *value,
					     size_t len, void *d),
			     void *data)
{
	unsigned int i, nb_attrs;
	size_t len = 0x100000;
	const char *attr;
	char *buf;
	ssize_t count;
	int ret = 0;

	buf = malloc(len);
	if (!buf)
		return -ENOMEM;

	nb_attrs = IIO_CALL(iio_device_get_debug_attrs_count)(dev);

	for (i = 0; i < nb_attrs; i++) {
		attr = IIO_CALL(iio_device_get_debug_attr)(dev, i);
		count = IIO_CALL(iio_device_debug_attr_read_raw)(dev, attr,
								 buf, len);
		if (count < 0) {
			ret = (int) count;
			goto out_free_buffer;
		}

		ret = (*cb)(dev, attr, buf, (size_t) count, data);
		if (ret < 0)
			goto out_free_buffer;
	}

out_free_buffer:
	free(buf);
	return ret;
}

ssize_t iio_device_attr_write_raw(const struct iio_device *dev,
				  const char *attr, const void *src, size_t len)
{
	return IIO_CALL(iio_device_attr_write_raw)(dev, attr, src, len);
}

ssize_t iio_device_attr_write(const struct iio_device *dev,
			      const char *attr, const char *src)
{
	return IIO_CALL(iio_device_attr_write_string)(dev, attr, src);
}

int iio_device_attr_write_bool(const struct iio_device *dev,
			       const char *attr, bool val)
{
	return IIO_CALL(iio_device_attr_write_bool)(dev, attr, val);
}

int iio_device_attr_write_longlong(const struct iio_device *dev,
				   const char *attr, long long val)
{
	return IIO_CALL(iio_device_attr_write_longlong)(dev, attr, val);
}

int iio_device_attr_write_double(const struct iio_device *dev,
				 const char *attr, double val)
{
	return IIO_CALL(iio_device_attr_write_double)(dev, attr, val);
}

int iio_device_attr_write_all(struct iio_device *dev,
			      ssize_t (*cb)(struct iio_device *dev,
					    const char *attr, void *buf,
					    size_t len, void *d),
			      void *data)
{
	unsigned int i, nb_attrs;
	size_t len = 0x100000;
	const char *attr;
	char *buf;
	ssize_t count;
	int ret = 0;

	buf = malloc(len);
	if (!buf)
		return -ENOMEM;

	nb_attrs = IIO_CALL(iio_device_get_attrs_count)(dev);

	for (i = 0; i < nb_attrs; i++) {
		attr = IIO_CALL(iio_device_get_attr)(dev, i);

		count = (*cb)(dev, attr, buf, len, data);
		if (count < 0) {
			ret = (int) count;
			goto out_free_buffer;
		}

		count = IIO_CALL(iio_device_attr_write_raw)(dev, attr,
							    buf, count);
		if (count < 0) {
			ret = (int) count;
			goto out_free_buffer;
		}
	}

out_free_buffer:
	free(buf);
	return ret;
}

ssize_t iio_device_buffer_attr_write_raw(const struct iio_device *dev,
					 const char *attr, const void *src,
					 size_t len)
{
	return IIO_CALL(iio_device_buffer_attr_write_raw)(dev, attr, src, len);
}

ssize_t iio_device_buffer_attr_write(const struct iio_device *dev,
				     const char *attr, const char *src)
{
	return IIO_CALL(iio_device_buffer_attr_write_string)(dev, attr, src);
}

int iio_device_buffer_attr_write_bool(const struct iio_device *dev,
				      const char *attr, bool val)
{
	return IIO_CALL(iio_device_buffer_attr_write_bool)(dev, attr, val);
}

int iio_device_buffer_attr_write_longlong(const struct iio_device *dev,
					  const char *attr, long long val)
{
	return IIO_CALL(iio_device_buffer_attr_write_longlong)(dev, attr, val);
}

int iio_device_buffer_attr_write_double(const struct iio_device *dev,
					const char *attr, double val)
{
	return IIO_CALL(iio_device_buffer_attr_write_double)(dev, attr, val);
}

int iio_device_buffer_attr_write_all(struct iio_device *dev,
				     ssize_t (*cb)(struct iio_device *dev,
						   const char *attr, void *buf,
						   size_t len, void *d),
				     void *data)
{
	unsigned int i, nb_attrs;
	size_t len = 0x100000;
	const char *attr;
	char *buf;
	ssize_t count;
	int ret = 0;

	buf = malloc(len);
	if (!buf)
		return -ENOMEM;

	nb_attrs = IIO_CALL(iio_device_get_buffer_attrs_count)(dev);

	for (i = 0; i < nb_attrs; i++) {
		attr = IIO_CALL(iio_device_get_buffer_attr)(dev, i);

		count = (*cb)(dev, attr, buf, len, data);
		if (count < 0) {
			ret = (int) count;
			goto out_free_buffer;
		}

		count = IIO_CALL(iio_device_buffer_attr_write_raw)(dev, attr,
								   buf, count);
		if (count < 0) {
			ret = (int) count;
			goto out_free_buffer;
		}
	}

out_free_buffer:
	free(buf);
	return ret;
}

ssize_t iio_device_debug_attr_write_raw(const struct iio_device *dev,
					const char *attr, const void *src,
					size_t len)
{
	return IIO_CALL(iio_device_debug_attr_write_raw)(dev, attr, src, len);
}

ssize_t iio_device_debug_attr_write(const struct iio_device *dev,
				    const char *attr, const char *src)
{
	return IIO_CALL(iio_device_debug_attr_write_string)(dev, attr, src);
}

int iio_device_debug_attr_write_bool(const struct iio_device *dev,
				     const char *attr, bool val)
{
	return IIO_CALL(iio_device_debug_attr_write_bool)(dev, attr, val);
}

int iio_device_debug_attr_write_longlong(const struct iio_device *dev,
					 const char *attr, long long val)
{
	return IIO_CALL(iio_device_debug_attr_write_longlong)(dev, attr, val);
}

int iio_device_debug_attr_write_double(const struct iio_device *dev,
				       const char *attr, double val)
{
	return IIO_CALL(iio_device_debug_attr_write_double)(dev, attr, val);
}

int iio_device_debug_attr_write_all(struct iio_device *dev,
				    ssize_t (*cb)(struct iio_device *dev,
						  const char *attr, void *buf,
						  size_t len, void *d),
				    void *data)
{
	unsigned int i, nb_attrs;
	size_t len = 0x100000;
	const char *attr;
	char *buf;
	ssize_t count;
	int ret = 0;

	buf = malloc(len);
	if (!buf)
		return -ENOMEM;

	nb_attrs = IIO_CALL(iio_device_get_debug_attrs_count)(dev);

	for (i = 0; i < nb_attrs; i++) {
		attr = IIO_CALL(iio_device_get_debug_attr)(dev, i);

		count = (*cb)(dev, attr, buf, len, data);
		if (count < 0) {
			ret = (int) count;
			goto out_free_buffer;
		}

		count = IIO_CALL(iio_device_debug_attr_write_raw)(dev, attr,
								  buf, count);
		if (count < 0) {
			ret = (int) count;
			goto out_free_buffer;
		}
	}

out_free_buffer:
	free(buf);
	return ret;
}

int iio_device_get_trigger(const struct iio_device *dev,
			   const struct iio_device **trigger)

{
	return IIO_CALL(iio_device_get_trigger)(dev, trigger);
}

int iio_device_set_trigger(const struct iio_device *dev,
			   const struct iio_device *trigger)

{
	return IIO_CALL(iio_device_set_trigger)(dev, trigger);
}

bool iio_device_is_trigger(const struct iio_device *dev)
{
	return IIO_CALL(iio_device_is_trigger)(dev);
}

int iio_device_set_kernel_buffers_count(const struct iio_device *dev,
					unsigned int nb_buffers)
{
	struct iio_device_compat *compat = IIO_CALL(iio_device_get_data)(dev);

	if (nb_buffers == 0)
		return -EINVAL;

	compat->nb_kernel_buffers = nb_buffers;

	return 0;
}

ssize_t iio_device_get_sample_size(const struct iio_device *dev)
{
	struct iio_device_compat *compat = IIO_CALL(iio_device_get_data)(dev);

	return IIO_CALL(iio_device_get_sample_size)(dev, compat->mask);
}

const struct iio_device * iio_channel_get_device(const struct iio_channel *chn)
{
	return IIO_CALL(iio_channel_get_device)(chn);
}

const char * iio_channel_get_id(const struct iio_channel *chn)
{
	return IIO_CALL(iio_channel_get_id)(chn);
}

const char * iio_channel_get_name(const struct iio_channel *chn)
{
	return IIO_CALL(iio_channel_get_name)(chn);
}

bool iio_channel_is_output(const struct iio_channel *chn)
{
	return IIO_CALL(iio_channel_is_output)(chn);
}

bool iio_channel_is_scan_element(const struct iio_channel *chn)
{
	return IIO_CALL(iio_channel_is_scan_element)(chn);
}

void * iio_channel_get_data(const struct iio_channel *chn)
{
	struct iio_channel_compat *compat = IIO_CALL(iio_channel_get_data)(chn);

	return compat->userdata;
}

void iio_channel_set_data(struct iio_channel *chn, void *data)
{
	struct iio_channel_compat *compat = IIO_CALL(iio_channel_get_data)(chn);

	compat->userdata = data;
}

unsigned int iio_channel_get_attrs_count(const struct iio_channel *chn)
{
	return IIO_CALL(iio_channel_get_attrs_count)(chn);
}

const char * iio_channel_get_attr(const struct iio_channel *chn,
				  unsigned int index)
{
	return IIO_CALL(iio_channel_get_attr)(chn, index);
}

const char * iio_channel_find_attr(const struct iio_channel *chn,
				   const char *name)
{
	return IIO_CALL(iio_channel_find_attr)(chn, name);
}

const char * iio_channel_attr_get_filename(const struct iio_channel *chn,
					   const char *name)
{
	return IIO_CALL(iio_channel_attr_get_filename)(chn, name);
}

ssize_t iio_channel_attr_read(const struct iio_channel *chn,
			      const char *attr, char *dst, size_t len)
{
	return IIO_CALL(iio_channel_attr_read_raw)(chn, attr, dst, len);
}

int iio_channel_attr_read_bool(const struct iio_channel *chn,
			       const char *attr, bool *val)
{
	return IIO_CALL(iio_channel_attr_read_bool)(chn, attr, val);
}

int iio_channel_attr_read_longlong(const struct iio_channel *chn,
				   const char *attr, long long *val)
{
	return IIO_CALL(iio_channel_attr_read_longlong)(chn, attr, val);
}

int iio_channel_attr_read_double(const struct iio_channel *chn,
				 const char *attr, double *val)
{
	return IIO_CALL(iio_channel_attr_read_double)(chn, attr, val);
}

int iio_channel_attr_read_all(struct iio_channel *chn,
			     int (*cb)(struct iio_channel *chn,
				       const char *attr,
				       const char *value, size_t len, void *d),
			     void *data)
{
	unsigned int i, nb_attrs;
	size_t len = 0x100000;
	const char *attr;
	char *buf;
	ssize_t count;
	int ret = 0;

	buf = malloc(len);
	if (!buf)
		return -ENOMEM;

	nb_attrs = IIO_CALL(iio_channel_get_attrs_count)(chn);

	for (i = 0; i < nb_attrs; i++) {
		attr = IIO_CALL(iio_channel_get_attr)(chn, i);
		count = IIO_CALL(iio_channel_attr_read_raw)(chn, attr,
							    buf, len);
		if (count < 0) {
			ret = (int) count;
			goto out_free_buffer;
		}

		ret = (*cb)(chn, attr, buf, (size_t) count, data);
		if (ret < 0)
			goto out_free_buffer;
	}

out_free_buffer:
	free(buf);
	return ret;
}

ssize_t iio_channel_attr_write_raw(const struct iio_channel *chn,
				   const char *attr, const void *src,
				   size_t len)
{
	return IIO_CALL(iio_channel_attr_write_raw)(chn, attr, src, len);
}

ssize_t iio_channel_attr_write(const struct iio_channel *chn,
			       const char *attr, const char *src)
{
	return IIO_CALL(iio_channel_attr_write_string)(chn, attr, src);
}

ssize_t iio_channel_attr_write_bool(const struct iio_channel *chn,
				    const char *attr, bool val)
{
	return IIO_CALL(iio_channel_attr_write_bool)(chn, attr, val);
}

ssize_t iio_channel_attr_write_longlong(const struct iio_channel *chn,
					const char *attr, long long val)
{
	return IIO_CALL(iio_channel_attr_write_longlong)(chn, attr, val);
}

ssize_t iio_channel_attr_write_double(const struct iio_channel *chn,
				      const char *attr, double val)
{
	return IIO_CALL(iio_channel_attr_write_double)(chn, attr, val);
}

int iio_channel_attr_write_all(struct iio_channel *chn,
			      ssize_t (*cb)(struct iio_channel *chn,
					    const char *attr, void *buf,
					    size_t len, void *d),
			      void *data)
{
	unsigned int i, nb_attrs;
	size_t len = 0x100000;
	const char *attr;
	char *buf;
	ssize_t count;
	int ret = 0;

	buf = malloc(len);
	if (!buf)
		return -ENOMEM;

	nb_attrs = IIO_CALL(iio_channel_get_attrs_count)(chn);

	for (i = 0; i < nb_attrs; i++) {
		attr = IIO_CALL(iio_channel_get_attr)(chn, i);

		count = (*cb)(chn, attr, buf, len, data);
		if (count < 0) {
			ret = (int) count;
			goto out_free_buffer;
		}

		count = IIO_CALL(iio_channel_attr_write_raw)(chn, attr,
							     buf, count);
		if (count < 0) {
			ret = (int) count;
			goto out_free_buffer;
		}
	}

out_free_buffer:
	free(buf);
	return ret;
}

unsigned int iio_channel_get_type(const struct iio_channel *chn)
{
	return IIO_CALL(iio_channel_get_type)(chn);
}

unsigned int iio_channel_get_modifier(const struct iio_channel *chn)
{
	return IIO_CALL(iio_channel_get_modifier)(chn);
}

long iio_channel_get_index(const struct iio_channel *chn)
{
	return IIO_CALL(iio_channel_get_index)(chn);
}

const struct iio_data_format *
iio_channel_get_data_format(const struct iio_channel *chn)
{
	return IIO_CALL(iio_channel_get_data_format)(chn);
}

void iio_channel_convert(const struct iio_channel *chn,
			    void *dst, const void *src)
{
	IIO_CALL(iio_channel_convert)(chn, dst, src);
}

void iio_channel_convert_inverse(const struct iio_channel *chn,
				 void *dst, const void *src)
{
	IIO_CALL(iio_channel_convert_inverse)(chn, dst, src);
}

void iio_channel_enable(struct iio_channel *chn)
{
	const struct iio_device *dev = IIO_CALL(iio_channel_get_device)(chn);
	struct iio_device_compat *compat = IIO_CALL(iio_device_get_data)(dev);

	IIO_CALL(iio_channel_enable)(chn, compat->mask);
}

void iio_channel_disable(struct iio_channel *chn)
{
	const struct iio_device *dev = IIO_CALL(iio_channel_get_device)(chn);
	struct iio_device_compat *compat = IIO_CALL(iio_device_get_data)(dev);

	IIO_CALL(iio_channel_disable)(chn, compat->mask);
}

bool iio_channel_is_enabled(const struct iio_channel *chn)
{
	const struct iio_device *dev = IIO_CALL(iio_channel_get_device)(chn);
	struct iio_device_compat *compat = IIO_CALL(iio_device_get_data)(dev);

	IIO_CALL(iio_channel_is_enabled)(chn, compat->mask);
}

void iio_library_get_version(unsigned int *major, unsigned int *minor,
			     char git_tag[8])
{
	iio_context_get_version(NULL, major, minor, git_tag);
}

bool iio_has_backend(const char *backend)
{
	return IIO_CALL(iio_has_backend)(backend);
}

unsigned int iio_get_backends_count(void)
{
	return IIO_CALL(iio_get_backends_count)();
}

const char * iio_get_backend(unsigned int index)
{
	return IIO_CALL(iio_get_backend)(index);
}

void iio_strerror(int err, char *dst, size_t len)
{
	IIO_CALL(iio_strerror)(err, dst, len);
}

struct iio_buffer * iio_device_create_buffer(const struct iio_device *dev,
					     size_t samples_count, bool cyclic)
{
	struct iio_device_compat *dev_compat;
	struct iio_buffer_compat *compat;
	struct iio_buffer *buf;
	int err = -ENOMEM;

	if (cyclic) {
		errno = ENOSYS; /* TODO */
		return NULL;
	}

	dev_compat = IIO_CALL(iio_device_get_data)(dev);

	compat = calloc(1, sizeof(*compat));
	if (!compat)
		goto err_set_errno;

	compat->cyclic = cyclic;
	compat->size = samples_count * iio_device_get_sample_size(dev);

	buf = IIO_CALL(iio_device_create_buffer)(dev, 0, dev_compat->mask);
	err = iio_err(buf);
	if (err)
		goto err_free_compat;

	IIO_CALL(iio_buffer_set_data)(buf, compat);

	compat->stream = IIO_CALL(iio_buffer_create_stream)(buf,
			dev_compat->nb_kernel_buffers, samples_count);
	err = iio_err(compat->stream);
	if (err)
		goto err_free_buf;

	compat->block = IIO_CALL(iio_stream_get_next_block)(compat->stream);
	err = iio_err(compat->block);
	if (err)
		goto err_free_stream;

	return buf;

err_free_stream:
	IIO_CALL(iio_stream_destroy)(compat->stream);
err_free_buf:
	IIO_CALL(iio_buffer_destroy)(buf);
err_free_compat:
	free(compat);
err_set_errno:
	errno = -err;
	return NULL;
}

void * iio_buffer_get_data(const struct iio_buffer *buf)
{
	struct iio_buffer_compat *compat = IIO_CALL(iio_buffer_get_data)(buf);

	return compat->userdata;
}

void iio_buffer_set_data(struct iio_buffer *buf, void *data)
{
	struct iio_buffer_compat *compat = IIO_CALL(iio_buffer_get_data)(buf);

	compat->userdata = data;
}

void iio_buffer_destroy(struct iio_buffer *buf)
{
	struct iio_buffer_compat *compat = IIO_CALL(iio_buffer_get_data)(buf);

	IIO_CALL(iio_stream_destroy)(compat->stream);
	IIO_CALL(iio_buffer_destroy)(buf);
	free(compat);
}

void iio_buffer_cancel(struct iio_buffer *buf)
{
	IIO_CALL(iio_buffer_cancel)(buf);
}

const struct iio_device * iio_buffer_get_device(const struct iio_buffer *buf)
{
	return IIO_CALL(iio_buffer_get_device)(buf);
}

int iio_buffer_get_poll_fd(struct iio_buffer *buf)
{
	return -ENOENT;
}

int iio_buffer_set_blocking_mode(struct iio_buffer *buf, bool blocking)
{
	struct iio_buffer_compat *compat = IIO_CALL(iio_buffer_get_data)(buf);

	compat->nonblock = !blocking;

	/* TODO: Support non-block mode */

	return -ENOSYS;
}

ssize_t iio_buffer_refill(struct iio_buffer *buf)
{
	struct iio_buffer_compat *compat = IIO_CALL(iio_buffer_get_data)(buf);
	int err;

	compat->block = IIO_CALL(iio_stream_get_next_block)(compat->stream);
	err = iio_err(compat->block);
	if (err)
		return (ssize_t) err;

	return compat->size;
}

ssize_t iio_buffer_push(struct iio_buffer *buf)
{
	struct iio_buffer_compat *compat = IIO_CALL(iio_buffer_get_data)(buf);
	int err;

	compat->block = IIO_CALL(iio_stream_get_next_block)(compat->stream);
	err = iio_err(compat->block);
	if (err)
		return (ssize_t) err;

	return compat->size;
}

ssize_t iio_buffer_push_partial(struct iio_buffer *buf, size_t samples_count)
{
	const struct iio_device *dev = IIO_CALL(iio_buffer_get_device)(buf);
	struct iio_buffer_compat *compat = IIO_CALL(iio_buffer_get_data)(buf);
	int err;

	compat->block = IIO_CALL(iio_stream_get_next_block)(compat->stream);
	err = iio_err(compat->block);
	if (err)
		return (ssize_t) err;

	return samples_count * iio_device_get_sample_size(dev);
}

void * iio_buffer_start(const struct iio_buffer *buf)
{
	struct iio_buffer_compat *compat = IIO_CALL(iio_buffer_get_data)(buf);

	return IIO_CALL(iio_block_start)(compat->block);
}

void * iio_buffer_first(const struct iio_buffer *buf,
			const struct iio_channel *chn)
{
	struct iio_buffer_compat *compat = IIO_CALL(iio_buffer_get_data)(buf);

	return IIO_CALL(iio_block_first)(compat->block, chn);
}

ptrdiff_t iio_buffer_step(const struct iio_buffer *buf)
{
	const struct iio_device *dev = IIO_CALL(iio_buffer_get_device)(buf);
	struct iio_buffer_compat *compat = IIO_CALL(iio_buffer_get_data)(buf);
	const struct iio_channels_mask *mask;

	mask = IIO_CALL(iio_buffer_get_channels_mask)(buf);

	return IIO_CALL(iio_device_get_sample_size)(dev, mask);
}

void * iio_buffer_end(const struct iio_buffer *buf)
{
	struct iio_buffer_compat *compat = IIO_CALL(iio_buffer_get_data)(buf);

	return IIO_CALL(iio_block_end)(compat->block);
}

ssize_t iio_buffer_foreach_sample(struct iio_buffer *buf,
				  ssize_t (*callback)(const struct iio_channel *chn,
						      void *src, size_t bytes,
						      void *d),
				  void *data)
{
	const struct iio_device *dev = IIO_CALL(iio_buffer_get_device)(buf);
	struct iio_buffer_compat *compat = IIO_CALL(iio_buffer_get_data)(buf);
	struct iio_device_compat *dev_compat = IIO_CALL(iio_device_get_data)(dev);

	return IIO_CALL(iio_block_foreach_sample)(compat->block,
						  dev_compat->mask,
						  callback, data);
}

static void compat_lib_init(void)
{
	struct compat *ctx;
	int err = -ENOMEM;

	ctx = malloc(sizeof(*ctx));
	if (!ctx) {
		fprintf(stderr, "Unable to alloc compat structure\n");
		goto err_set_errno;
	}

	ctx->lib = iio_dlopen(LIBIIO1_NAME);
	if (!ctx->lib) {
		fprintf(stderr, "Unable to dlopen " LIBIIO1_NAME "\n");
		err = -ENOENT;
		goto err_free_ctx;
	}

#define FIND_SYMBOL(lib, symbol) \
	do { \
		ctx->symbol = iio_dlsym(lib, #symbol); \
		if (!ctx->symbol) { \
			fprintf(stderr, "Unable to find symbol: %s\n", #symbol); \
			err = -ENOENT; \
			goto err_dlclose; \
		} \
	} while (0)

	FIND_SYMBOL(ctx->lib, iio_scan);
	FIND_SYMBOL(ctx->lib, iio_scan_destroy);
	FIND_SYMBOL(ctx->lib, iio_scan_get_results_count);
	FIND_SYMBOL(ctx->lib, iio_scan_get_description);
	FIND_SYMBOL(ctx->lib, iio_scan_get_uri);

	FIND_SYMBOL(ctx->lib, iio_has_backend);
	FIND_SYMBOL(ctx->lib, iio_get_backends_count);
	FIND_SYMBOL(ctx->lib, iio_get_backend);

	FIND_SYMBOL(ctx->lib, iio_create_context);
	FIND_SYMBOL(ctx->lib, iio_context_destroy);
	FIND_SYMBOL(ctx->lib, iio_context_get_data);
	FIND_SYMBOL(ctx->lib, iio_context_set_data);
	FIND_SYMBOL(ctx->lib, iio_context_get_version_major);
	FIND_SYMBOL(ctx->lib, iio_context_get_version_minor);
	FIND_SYMBOL(ctx->lib, iio_context_get_version_tag);
	FIND_SYMBOL(ctx->lib, iio_context_get_name);
	FIND_SYMBOL(ctx->lib, iio_context_get_description);
	FIND_SYMBOL(ctx->lib, iio_context_get_xml);
	FIND_SYMBOL(ctx->lib, iio_context_get_params);
	FIND_SYMBOL(ctx->lib, iio_context_get_devices_count);
	FIND_SYMBOL(ctx->lib, iio_context_get_device);
	FIND_SYMBOL(ctx->lib, iio_context_find_device);
	FIND_SYMBOL(ctx->lib, iio_context_set_timeout);

	FIND_SYMBOL(ctx->lib, iio_context_get_attrs_count);
	FIND_SYMBOL(ctx->lib, iio_context_get_attr);
	FIND_SYMBOL(ctx->lib, iio_context_get_attr_value);

	FIND_SYMBOL(ctx->lib, iio_device_get_context);
	FIND_SYMBOL(ctx->lib, iio_device_get_id);
	FIND_SYMBOL(ctx->lib, iio_device_get_name);
	FIND_SYMBOL(ctx->lib, iio_device_get_label);
	FIND_SYMBOL(ctx->lib, iio_device_get_data);
	FIND_SYMBOL(ctx->lib, iio_device_set_data);
	FIND_SYMBOL(ctx->lib, iio_device_identify_filename);
	FIND_SYMBOL(ctx->lib, iio_device_reg_read);
	FIND_SYMBOL(ctx->lib, iio_device_reg_write);
	FIND_SYMBOL(ctx->lib, iio_device_get_channels_count);
	FIND_SYMBOL(ctx->lib, iio_device_get_attrs_count);
	FIND_SYMBOL(ctx->lib, iio_device_get_attr);
	FIND_SYMBOL(ctx->lib, iio_device_find_attr);
	FIND_SYMBOL(ctx->lib, iio_device_get_buffer_attrs_count);
	FIND_SYMBOL(ctx->lib, iio_device_get_buffer_attr);
	FIND_SYMBOL(ctx->lib, iio_device_find_buffer_attr);
	FIND_SYMBOL(ctx->lib, iio_device_get_debug_attrs_count);
	FIND_SYMBOL(ctx->lib, iio_device_get_debug_attr);
	FIND_SYMBOL(ctx->lib, iio_device_find_debug_attr);
	FIND_SYMBOL(ctx->lib, iio_device_get_channel);
	FIND_SYMBOL(ctx->lib, iio_device_find_channel);
	FIND_SYMBOL(ctx->lib, iio_device_attr_read_raw);
	FIND_SYMBOL(ctx->lib, iio_device_attr_read_bool);
	FIND_SYMBOL(ctx->lib, iio_device_attr_read_longlong);
	FIND_SYMBOL(ctx->lib, iio_device_attr_read_double);
	FIND_SYMBOL(ctx->lib, iio_device_attr_write_raw);
	FIND_SYMBOL(ctx->lib, iio_device_attr_write_string);
	FIND_SYMBOL(ctx->lib, iio_device_attr_write_bool);
	FIND_SYMBOL(ctx->lib, iio_device_attr_write_longlong);
	FIND_SYMBOL(ctx->lib, iio_device_attr_write_double);
	FIND_SYMBOL(ctx->lib, iio_device_buffer_attr_read_raw);
	FIND_SYMBOL(ctx->lib, iio_device_buffer_attr_read_bool);
	FIND_SYMBOL(ctx->lib, iio_device_buffer_attr_read_longlong);
	FIND_SYMBOL(ctx->lib, iio_device_buffer_attr_read_double);
	FIND_SYMBOL(ctx->lib, iio_device_buffer_attr_write_raw);
	FIND_SYMBOL(ctx->lib, iio_device_buffer_attr_write_string);
	FIND_SYMBOL(ctx->lib, iio_device_buffer_attr_write_bool);
	FIND_SYMBOL(ctx->lib, iio_device_buffer_attr_write_longlong);
	FIND_SYMBOL(ctx->lib, iio_device_buffer_attr_write_double);
	FIND_SYMBOL(ctx->lib, iio_device_debug_attr_read_raw);
	FIND_SYMBOL(ctx->lib, iio_device_debug_attr_read_bool);
	FIND_SYMBOL(ctx->lib, iio_device_debug_attr_read_longlong);
	FIND_SYMBOL(ctx->lib, iio_device_debug_attr_read_double);
	FIND_SYMBOL(ctx->lib, iio_device_debug_attr_write_raw);
	FIND_SYMBOL(ctx->lib, iio_device_debug_attr_write_string);
	FIND_SYMBOL(ctx->lib, iio_device_debug_attr_write_bool);
	FIND_SYMBOL(ctx->lib, iio_device_debug_attr_write_longlong);
	FIND_SYMBOL(ctx->lib, iio_device_debug_attr_write_double);
	FIND_SYMBOL(ctx->lib, iio_device_get_trigger);
	FIND_SYMBOL(ctx->lib, iio_device_set_trigger);
	FIND_SYMBOL(ctx->lib, iio_device_is_trigger);
	FIND_SYMBOL(ctx->lib, iio_device_get_sample_size);

	FIND_SYMBOL(ctx->lib, iio_channel_get_device);
	FIND_SYMBOL(ctx->lib, iio_channel_get_id);
	FIND_SYMBOL(ctx->lib, iio_channel_get_name);
	FIND_SYMBOL(ctx->lib, iio_channel_is_output);
	FIND_SYMBOL(ctx->lib, iio_channel_is_scan_element);
	FIND_SYMBOL(ctx->lib, iio_channel_get_data);
	FIND_SYMBOL(ctx->lib, iio_channel_set_data);
	FIND_SYMBOL(ctx->lib, iio_channel_get_attrs_count);
	FIND_SYMBOL(ctx->lib, iio_channel_get_attr);
	FIND_SYMBOL(ctx->lib, iio_channel_find_attr);
	FIND_SYMBOL(ctx->lib, iio_channel_attr_get_filename);
	FIND_SYMBOL(ctx->lib, iio_channel_attr_read_raw);
	FIND_SYMBOL(ctx->lib, iio_channel_attr_read_bool);
	FIND_SYMBOL(ctx->lib, iio_channel_attr_read_longlong);
	FIND_SYMBOL(ctx->lib, iio_channel_attr_read_double);
	FIND_SYMBOL(ctx->lib, iio_channel_attr_write_raw);
	FIND_SYMBOL(ctx->lib, iio_channel_attr_write_string);
	FIND_SYMBOL(ctx->lib, iio_channel_attr_write_bool);
	FIND_SYMBOL(ctx->lib, iio_channel_attr_write_longlong);
	FIND_SYMBOL(ctx->lib, iio_channel_attr_write_double);
	FIND_SYMBOL(ctx->lib, iio_channel_get_type);
	FIND_SYMBOL(ctx->lib, iio_channel_get_modifier);
	FIND_SYMBOL(ctx->lib, iio_channel_get_index);
	FIND_SYMBOL(ctx->lib, iio_channel_get_data_format);
	FIND_SYMBOL(ctx->lib, iio_channel_convert);
	FIND_SYMBOL(ctx->lib, iio_channel_convert_inverse);
	FIND_SYMBOL(ctx->lib, iio_channel_enable);
	FIND_SYMBOL(ctx->lib, iio_channel_disable);
	FIND_SYMBOL(ctx->lib, iio_channel_is_enabled);

	FIND_SYMBOL(ctx->lib, iio_create_channels_mask);
	FIND_SYMBOL(ctx->lib, iio_channels_mask_destroy);

	FIND_SYMBOL(ctx->lib, iio_device_create_buffer);
	FIND_SYMBOL(ctx->lib, iio_buffer_destroy);
	FIND_SYMBOL(ctx->lib, iio_buffer_cancel);
	FIND_SYMBOL(ctx->lib, iio_buffer_get_device);
	FIND_SYMBOL(ctx->lib, iio_buffer_get_channels_mask);
	FIND_SYMBOL(ctx->lib, iio_buffer_get_data);
	FIND_SYMBOL(ctx->lib, iio_buffer_set_data);

	FIND_SYMBOL(ctx->lib, iio_buffer_create_stream);
	FIND_SYMBOL(ctx->lib, iio_stream_destroy);
	FIND_SYMBOL(ctx->lib, iio_stream_get_next_block);

	FIND_SYMBOL(ctx->lib, iio_block_get_buffer);
	FIND_SYMBOL(ctx->lib, iio_block_start);
	FIND_SYMBOL(ctx->lib, iio_block_first);
	FIND_SYMBOL(ctx->lib, iio_block_end);
	FIND_SYMBOL(ctx->lib, iio_block_foreach_sample);

	FIND_SYMBOL(ctx->lib, iio_strlcpy);
	FIND_SYMBOL(ctx->lib, iio_strdup);
	FIND_SYMBOL(ctx->lib, iio_strerror);

#undef FIND_SYMBOL

	compat_lib_ptr = ctx;

	return;

err_dlclose:
	iio_dlclose(ctx->lib);
err_free_ctx:
	free(ctx);
err_set_errno:
	errno = -err;
}

static void compat_lib_exit(void)
{
	iio_dlclose(compat_lib_ptr->lib);
	free(compat_lib_ptr);
}

#if defined(_MSC_BUILD)
#pragma section(".CRT$XCU", read)
#define __CONSTRUCTOR(f, p) \
  static void f(void); \
  __declspec(allocate(".CRT$XCU")) void (*f##_)(void) = f; \
  __pragma(comment(linker,"/include:" p #f "_")) \
  static void f(void)
#ifdef _WIN64
#define _CONSTRUCTOR(f) __CONSTRUCTOR(f, "")
#else
#define _CONSTRUCTOR(f) __CONSTRUCTOR(f, "_")
#endif
#elif defined(__GNUC__)
#define _CONSTRUCTOR(f) static void __attribute__((constructor)) f(void)
#else
#define _CONSTRUCTOR(f) static void f(void)
#endif

_CONSTRUCTOR(initialize)
{
	compat_lib_init();

	/*
	 * When the library loads, register our destructor.
	 * Do it here and not in the context creation function,
	 * as it could otherwise end up registering the destructor
	 * many times.
	 */
	atexit(compat_lib_exit);
}
#undef _CONSTRUCTOR
