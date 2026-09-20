// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once
#include <SPI1.h>
using SPIClass = SPI1Class;
using SPISettings = SPI1Settings;
// One object and one bus state even when SPI.h and SPI1.h are both included.
#define SPI SPI1
