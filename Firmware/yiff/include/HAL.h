/*
 * HAL.h
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_HAL_H_
#define INCLUDE_HAL_H_

/**
 * Init fox-specific hardware here
 */
void HAL_IntiHardware();

/**
 * Switch TX manipulator on of off
 */
void HAL_SwitchManipulator(bool isTxOn);


#endif /* INCLUDE_HAL_H_ */
