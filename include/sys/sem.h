/*
 * Copyright (c) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 *
 * @brief public sys_sem APIs.
 */

#ifndef ZEPHYR_INCLUDE_SYS_SEM_H_
#define ZEPHYR_INCLUDE_SYS_SEM_H_

/*
 * sys_sem exists in user memory working as counter semaphore for
 * user mode thread when user mode enabled. When user mode isn't
 * enabled, sys_sem behaves like k_sem.
 */

#include <kernel.h>
#include <sys/atomic.h>
#include <zephyr/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * sys_sem structure
 */
struct sys_sem {
#ifdef CONFIG_USERSPACE
	struct k_futex futex;
	int limit;
#else
	struct k_sem kernel_sem;
#endif
};

/**
 * @brief Initialize a semaphore.
 *
 * This routine initializes a semaphore instance, prior to its first use.
 *
 * @param sem Address of the semaphore.
 * @param initial_count Initial semaphore count.
 * @param limit Maximum permitted semaphore count.
 *
 * @retval 0 Initial success.
 * @retval -EINVAL Bad parameters, the value of limit should be located in
 *         (0, INT_MAX] and initial_count shouldn't be greater than limit.
 */
int sys_sem_init(struct sys_sem *sem, unsigned int initial_count,
		unsigned int limit);

/**
 * @brief Give a semaphore.
 *
 * This routine gives @a sem, unless the semaphore is already at its
 * maximum permitted count.
 *
 * @param sem Address of the semaphore.
 *
 * @retval 0 Semaphore given.
 * @retval -EINVAL Parameter address not recognized.
 * @retval -EACCES Caller does not have enough access.
 * @retval -EAGAIN Count reached Maximum permitted count and try again.
 */
int sys_sem_give(struct sys_sem *sem);

/**
 * @brief Take a sys_sem.
 *
 * This routine takes @a sem.
 *
 * @param sem Address of the sys_sem.
 * @param timeout Waiting period to take the sys_sem (in milliseconds),
 *                or one of the special values K_NO_WAIT and K_FOREVER.
 *
 * @retval 0 sys_sem taken.
 * @retval -EINVAL Parameter address not recognized.
 * @retval -ETIMEDOUT Waiting period timed out.
 * @retval -EACCES Caller does not have enough access.
 */
int sys_sem_take(struct sys_sem *sem, s32_t timeout);

/**
 * @brief Get sys_sem's value
 *
 * This routine returns the current value of @a sem.
 *
 * @param sem Address of the sys_sem.
 *
 * @return Current value of sys_sem.
 */
unsigned int sys_sem_count_get(struct sys_sem *sem);

#ifdef __cplusplus
}
#endif

#endif
