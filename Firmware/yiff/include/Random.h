/*
 * Random.h
 *
 *  Created on: Nov 11, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_RANDOM_H_
#define INCLUDE_RANDOM_H_

#include <stdint.h>
#include <stdbool.h>

#define RAND_MODULUS 2147483648U

#define RAND_MULTIPLIER 1103515245U

#define RAND_INCREMENT 12345

/**
 * Current value of random generator
 */
static uint32_t Rand_CurrentValue;

/**
 * If not initialized, then Rand_GetRandom() will block until initialization
 */
static bool Rand_IsInitialized = false;

/**
 * Getting enthropy from battery voltage
 */
void Rand_GatherEnthropy(void);

/**
 * Sets random generator seed.
 */
void Rand_SetSeed(uint32_t seed);

/**
 * Returns 32-bit random number
 */
uint32_t Rand_GetRandom(void);

#endif /* INCLUDE_RANDOM_H_ */
