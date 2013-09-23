/*
 *
 * (C) COPYRIGHT 2012-2013 ARM Limited. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
 *
 * A copy of the licence is included with the program, and can also be obtained from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */



#if !defined(_KBASE_TRACE_TIMELINE_H)
#define _KBASE_TRACE_TIMELINE_H

#ifdef CONFIG_MALI_TRACE_TIMELINE

typedef enum
{
	#define KBASE_TIMELINE_TRACE_CODE(enum_val, desc, format, format_desc) enum_val
	#include "mali_kbase_trace_timeline_defs.h"
	#undef KBASE_TIMELINE_TRACE_CODE
} kbase_trace_timeline_code;

extern ssize_t show_timeline_defs(struct device *dev, struct device_attribute *attr, char *buf);

/* mali_timeline.h defines kernel tracepoints used by the KBASE_TIMELINE 
   functions.
   Output is timestamped by either sched_clock() (default), local_clock(), or 
   cpu_clock(), depending on /sys/kernel/debug/tracing/trace_clock */
#include "mali_timeline.h"

/* Trace number of atoms in flight for kctx (atoms either not completed, or in 
   process of being returned to user */
#define KBASE_TIMELINE_ATOMS_IN_FLIGHT(kctx, count)                                 \
	do                                                                          \
	{                                                                           \
		struct timespec ts;                                                 \
		getnstimeofday(&ts);                                                \
		trace_mali_timeline_atoms_in_flight(ts.tv_sec, ts.tv_nsec,          \
                                                    (int)kctx->timeline.owner_tgid, \
                                                    count);                         \
	} while (0)

/* Trace number of atoms submitted to job slot js */
#define KBASE_TIMELINE_ATOMS_SUBMITTED(kctx, js, count)                             \
	do                                                                          \
	{                                                                           \
		struct timespec ts;                                                 \
		getnstimeofday(&ts);                                                \
		trace_mali_timeline_gpu_slot_active(ts.tv_sec, ts.tv_nsec,          \
		                                    SW_SET_GPU_SLOT_ACTIVE, \
		                                    (int)kctx->timeline.owner_tgid, \
		                                    js, count);                     \
	} while (0)


/* Trace atoms present in JSn_NEXT */
#define KBASE_TIMELINE_JOB_START_NEXT(kctx, js, count)                             \
	do                                                                          \
	{                                                                           \
		struct timespec ts;                                                 \
		getnstimeofday(&ts);                                                \
		trace_mali_timeline_gpu_slot_active(ts.tv_sec, ts.tv_nsec,          \
		                                    SW_SET_GPU_SLOT_NEXT, \
		                                    (int)kctx->timeline.owner_tgid, \
		                                    js, count);                     \
	} while (0)

/* Trace atoms present in JSn_HEAD */
#define KBASE_TIMELINE_JOB_START_HEAD(kctx, js, count)                             \
	do                                                                          \
	{                                                                           \
		struct timespec ts;                                                 \
		getnstimeofday(&ts);                                                \
		trace_mali_timeline_gpu_slot_active(ts.tv_sec, ts.tv_nsec,          \
		                                    SW_SET_GPU_SLOT_HEAD, \
		                                    (int)kctx->timeline.owner_tgid, \
		                                    js, count);                     \
	} while (0)

/* Trace that a soft stop/evict from next is being attempted on a slot */
#define KBASE_TIMELINE_TRY_SOFT_STOP(kctx, js, count) \
	do                                                                          \
	{                                                                           \
		struct timespec ts;                                                 \
		getnstimeofday(&ts);                                                \
		trace_mali_timeline_gpu_slot_active(ts.tv_sec, ts.tv_nsec,          \
		                                    SW_SET_GPU_SLOT_STOPPING, \
		                                    (kctx)?(int)kctx->timeline.owner_tgid:0, \
		                                    js, count);                     \
	} while (0)



/* Trace state of overall GPU power */
#define KBASE_TIMELINE_GPU_POWER(kbdev, active)                                    \
	do                                                                         \
	{                                                                          \
		struct timespec ts;                                                \
		getnstimeofday(&ts);                                               \
		trace_mali_timeline_gpu_power_active(ts.tv_sec, ts.tv_nsec,        \
		                                     SW_SET_GPU_POWER_ACTIVE, active); \
	} while (0)

/* Trace state of tiler power */
#define KBASE_TIMELINE_POWER_TILER(kbdev, bitmap)                               \
	do                                                                      \
	{                                                                       \
		struct timespec ts;                                             \
		getnstimeofday(&ts);                                            \
		trace_mali_timeline_gpu_power_active(ts.tv_sec, ts.tv_nsec,     \
		                                     SW_SET_GPU_POWER_TILER_ACTIVE, \
		                                     hweight64(bitmap));        \
	} while (0)

/* Trace number of shaders currently powered */
#define KBASE_TIMELINE_POWER_SHADER(kbdev, bitmap)                               \
	do                                                                       \
	{                                                                        \
		struct timespec ts;                                              \
		getnstimeofday(&ts);                                             \
		trace_mali_timeline_gpu_power_active(ts.tv_sec, ts.tv_nsec,      \
		                                     SW_SET_GPU_POWER_SHADER_ACTIVE, \
		                                     hweight64(bitmap));         \
	} while (0)

/* Trace atom_id starting in JSn_HEAD */
#define KBASE_TIMELINE_JOB_START(kctx, js, _consumerof_atom_number)     \
	do                                                                  \
	{                                                                   \
		struct timespec ts;                                             \
		getnstimeofday(&ts);                                            \
		trace_mali_timeline_slot_atom(ts.tv_sec, ts.tv_nsec,            \
		                              HW_START_GPU_JOB_CHAIN_SW_APPROX, \
		                              (int)kctx->timeline.owner_tgid,   \
		                              js, _consumerof_atom_number);     \
	} while (0)

/* Trace atom_id stopping on JSn_HEAD */
#define KBASE_TIMELINE_JOB_STOP(kctx, js, _producerof_atom_number_completed) \
	do                                                                  \
	{                                                                   \
		struct timespec ts;                                             \
		getnstimeofday(&ts);                                            \
		trace_mali_timeline_slot_atom(ts.tv_sec, ts.tv_nsec,            \
		                              HW_STOP_GPU_JOB_CHAIN_SW_APPROX,  \
		                              (int)kctx->timeline.owner_tgid,   \
		                              js, _producerof_atom_number_completed);     \
	} while (0)

/**
 * Trace that an atom is starting on a job slot
 *
 * The caller must be holding kbasep_js_device_data::runpool_irq::lock
 */
void kbase_timeline_job_slot_submit(kbase_device *kbdev, kbase_context *kctx,
                                    kbase_jd_atom *katom, int js);

/**
 * Trace that an atom has done on a job slot
 *
 * 'Done' in this sense can occur either because:
 * - the atom in JSn_HEAD finished
 * - the atom in JSn_NEXT was evicted
 *
 * Whether the atom finished or was evicted is passed in @a done_code
 *
 * It is assumed that the atom has already been removed from the submit slot,
 * with either:
 * - kbasep_jm_dequeue_submit_slot()
 * - kbasep_jm_dequeue_tail_submit_slot()
 *
 * The caller must be holding kbasep_js_device_data::runpool_irq::lock
 */
void kbase_timeline_job_slot_done(kbase_device *kbdev, kbase_context *kctx,
                                  kbase_jd_atom *katom, int js,
                                  kbasep_js_atom_done_code done_code);

#else

#define KBASE_TIMELINE_ATOMS_IN_FLIGHT(kctx, count) CSTD_NOP()

#define KBASE_TIMELINE_ATOMS_SUBMITTED(kctx, js, count) CSTD_NOP()

#define KBASE_TIMELINE_JOB_START_NEXT(kctx, js, count) CSTD_NOP()

#define KBASE_TIMELINE_JOB_START_HEAD(kctx, js, count) CSTD_NOP()

#define KBASE_TIMELINE_TRY_SOFT_STOP(kctx, js, count) CSTD_NOP()

#define KBASE_TIMELINE_GPU_POWER(kbdev, active) CSTD_NOP()

#define KBASE_TIMELINE_POWER_TILER(kbdev, bitmap) CSTD_NOP()

#define KBASE_TIMELINE_POWER_SHADER(kbdev, bitmap) CSTD_NOP()

#define KBASE_TIMELINE_JOB_START(kctx, js, _consumerof_atom_number) CSTD_NOP()

#define KBASE_TIMELINE_JOB_STOP(kctx, js, _producerof_atom_number_completed) CSTD_NOP()

static INLINE void kbase_timeline_job_slot_submit(kbase_device *kbdev, kbase_context *kctx,
                                    kbase_jd_atom *katom, int js)
{
	lockdep_assert_held(&kbdev->js_data.runpool_irq.lock);
}

static INLINE void kbase_timeline_job_slot_done(kbase_device *kbdev, kbase_context *kctx,
                                    kbase_jd_atom *katom, int js,
                                    kbasep_js_atom_done_code done_code)
{
	lockdep_assert_held(&kbdev->js_data.runpool_irq.lock);
}

#endif				/* CONFIG_MALI_TRACE_TIMELINE */

#endif				/* _KBASE_TRACE_TIMELINE_H */

