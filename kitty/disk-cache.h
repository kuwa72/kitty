/*
 * Copyright (C) 2020 Kovid Goyal <kovid at kovidgoyal.net>
 *
 * Distributed under terms of the GPL3 license.
 */

#pragma once

#include "data-types.h"

PyObject* create_disk_cache(void);
bool add_to_disk_cache(PyObject *self, const void *key, size_t key_sz, const void *data, size_t data_sz);
bool remove_from_disk_cache(PyObject *self_, const void *key, size_t key_sz);
void* read_from_disk_cache(PyObject *self_, const void *key, size_t key_sz, void*(allocator)(void*, size_t), void*);
static inline void* disk_cache_malloc_allocator(void *x UNUSED, size_t sz) { return malloc(sz); }
static inline void* read_from_disk_cache_simple(PyObject *self_, const void *key, size_t key_sz) { return read_from_disk_cache(self_, key, key_sz, disk_cache_malloc_allocator, NULL); }
