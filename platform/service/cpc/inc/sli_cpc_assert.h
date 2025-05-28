#ifndef SLI_CPC_ASSERT_H
#define SLI_CPC_ASSERT_H

#include "cmsis_compiler.h"

#if !defined(DEBUG_EFM) && !defined(DEBUG_EFM_USER)

#define SLI_CPC_ASSERT(condition) (void)(condition)

#else

#define SLI_CPC_ASSERT(condition) sli_cpc_assert(condition, __FILE__, __LINE__)

/***************************************************************************//**
 * @brief This function is used to handle assertion failures by halting the
 * program execution. It is typically called when a condition that should
 * never occur is detected, indicating a serious error in the program.
 * The function takes the file name and line number where the assertion
 * failed, which can be used for debugging purposes. It is intended to be
 * used in debug builds where assertions are enabled. Once called, the
 * function enters an infinite loop, effectively stopping further
 * execution of the program.
 *
 * @param file A string representing the name of the source file where the
 * assertion failed. It must not be null.
 * @param line An integer representing the line number in the source file where
 * the assertion failed. It should be a positive number.
 * @return None
 ******************************************************************************/
__NO_RETURN void sli_cpc_assert_fail(const char *file, int line);

/***************************************************************************//**
 * @brief The `sli_cpc_assert` function checks a condition and triggers a
 * failure handler if the condition is false.
 *
 * @param condition An integer representing the condition to be checked;
 * typically, this is a boolean value where non-zero is true
 * and zero is false.
 * @param file A string representing the name of the source file where the
 * assertion is being checked.
 * @param line An integer representing the line number in the source file where
 * the assertion is being checked.
 * @return The function does not return a value; it either does nothing if the
 * condition is true or calls a failure handler if the condition is
 * false.
 ******************************************************************************/
__STATIC_FORCEINLINE void sli_cpc_assert(int condition, const char *file, int line)
{
  if (!condition) {
    sli_cpc_assert_fail(file, line);
  }
}

#endif

#endif /* SLI_CPC_ASSERT_H */
