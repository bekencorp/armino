/**
 ****************************************************************************************
 *
 * @file gnuarm/compiler.h
 *
 * @brief Definitions of compiler specific directives.
 *
 * Copyright (C) RivieraWaves 2011-2016
 *
 ****************************************************************************************
 */

#ifndef _COMPILER_H_
#define _COMPILER_H_

/// define the force inlining attribute for this compiler  gcc: __attribute__((always_inline))
#define __INLINE                     static inline

/// function returns struct in registers (4 words max, var with gnuarm)
#if !defined(__VIR)
#define __VIR                        __value_in_regs
#endif

/// function has no side effect and return depends only on arguments
#if !defined(__PURE)
#define __PURE                       __pure
#endif

/// Align instantiated lvalue or struct member on 4 bytes
#if !defined(__ALIGN4)
#define __ALIGN4                     __attribute__((aligned(4)))
#endif

#define __MODULE__                   __BASE_FILE__

/// define the BLE IRQ handler attribute for this compiler
#define __BLEIRQ

/// define size of an empty array (used to declare structure with an array size not defined)
#define __ARRAY_EMPTY

/// define the static keyword for this compiler
#define __STATIC static

/// Pack a structure field
#define __PACKED16                   __attribute__( ( packed ) )
#if !defined(__PACKED)
#define __PACKED                     __attribute__( ( packed ) )
#endif
#ifndef __packed
#define __packed                     __attribute__((packed))
#endif

#if !defined(__SECTION)
#define __SECTION(x)                 __attribute__((section(x)))
#endif

#ifndef likely
#define likely(x)                    __builtin_expect(!!(x), 1)
#endif
#ifndef unlikely
#define unlikely(x)                  __builtin_expect(!!(x), 0)
#endif

/* XXX: move to right place */
#if (CONFIG_FULLY_HOSTED)
/* host packed */
#undef __hpacked
#define __hpacked                    __attribute__((packed))
#else
#define __hpacked
#endif

#ifndef __deprecated
#define __deprecated                 __attribute__((deprecated))
#endif
#define __weak                       __attribute__((weak))
#define __must_check                 __attribute__((warn_unused_result))

#define __SHAREDRAM
#define __MIB

#define __MAYBE_UNUSED               __attribute__((unused))

#ifndef BUILD_BUG_ON
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))
#endif

/* Force a compilation error if condition is true, but also produce a
   result (of value 0 and type size_t), so the expression can be used
   e.g. in a structure initializer (or where-ever else comma expressions
   aren't permitted). */
#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))
#define BUILD_BUG_ON_NULL(e) ((void *)sizeof(struct { int:-!!(e); }))


/* Are two types/vars the same type (ignoring qualifiers)? */
#ifndef __same_type
# define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))
#endif

/* &a[0] degrades to a pointer: a different type from an array */
#define __must_be_array(a) BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))


#endif // _COMPILER_H_

