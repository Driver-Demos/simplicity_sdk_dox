/***************************************************************************//**
 * @file
 * @brief See @ref random for detailed documentation.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup legacyhal
 * @{
 ******************************************************************************/

/** @addtogroup random Random Numbers
 * @brief Functions that provide access to random numbers
 *
 * These functions may be hardware accelerated, though often are not.
 *
 * See random.h for source code.
 *@{
 */

#ifndef __RANDOM_H__
#define __RANDOM_H__

/***************************************************************************//**
 * @brief This function initializes the pseudorandom number generator used by
 * the stack, ensuring that subsequent calls to generate random numbers
 * produce different sequences. It should be called during the stack
 * initialization process, typically with a seed value obtained from the
 * radio. The function ensures that the seed values are non-zero by
 * substituting default values if necessary, which helps maintain
 * randomness in the generated sequences.
 *
 * @param seed A 32-bit unsigned integer used to seed the pseudorandom number
 * generator. The lower 16 bits and the upper 16 bits are used
 * separately. If either part is zero, a default non-zero value is
 * substituted to ensure proper seeding.
 * @return None
 ******************************************************************************/
void halStackSeedRandom(uint32_t seed);

/***************************************************************************//**
 * @brief This function provides a 16-bit pseudorandom number, which can be used
 * in various applications requiring randomization, such as selecting
 * random backoff slots in network protocols. It is designed to be called
 * frequently and should return quickly to avoid performance bottlenecks.
 * The function relies on an internal state that must be seeded using
 * `halStackSeedRandom` before use to ensure randomness. It is suitable
 * for applications where hardware acceleration is not available or
 * necessary.
 *
 * @return Returns a 16-bit unsigned integer representing a pseudorandom number.
 ******************************************************************************/
uint16_t halCommonGetRandom(void);

/** @} (end addtogroup random) */
/** @} (end addtogroup legacyhal) */

#endif //__RANDOM_H__
