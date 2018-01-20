/*
 * character.h
 *
 * Created: 20-Jan-2018
 * Author: Vuong Le
 */

#ifndef CHARACTER_H
#define CHARACTER_H

#include "define.h"

uint8_t checkBufferIndex(char character);
void GetBufferFromCharacter(char character, uint8_t* returnValue);
void GetBufferFromCharacterSpin(char character, uint8_t* returnValue);

#endif