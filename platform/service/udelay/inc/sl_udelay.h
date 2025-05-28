/***************************************************************************//**
 * @file
 * @brief Microsecond delay.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#ifndef UDELAY_H
#define UDELAY_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup udelay Microsecond Delay
 * @brief Microsecond delay function
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @brief This function introduces a delay in code execution for a specified
 * number of microseconds by using a busy loop, which relies on the core
 * clock frequency for timing. It is suitable for short delays where high
 * precision is not critical, as the delay may be affected by interrupts
 * and context switching. The function does not return earlier than the
 * specified delay time, but there is some overhead associated with the
 * function call itself. For delays longer than 1 millisecond, it is
 * recommended to use a hardware-based timer for better accuracy.
 *
 * @param us The number of microseconds to delay execution. Valid values range
 * from 0 to 100,000 microseconds (100 milliseconds). The function
 * will not return earlier than the specified delay time. For delays
 * longer than 1 millisecond, using a hardware timer is recommended
 * for improved accuracy.
 * @return None
 ******************************************************************************/
void sl_udelay_wait(unsigned us);

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup udelay) */

#endif
