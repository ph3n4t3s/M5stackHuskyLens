#pragma once

// Base Arduino et M5Stack
#include <M5CoreS3.h>

// Core système
#include "core/Constants.h"
#include "core/Types.h"
#include "core/Config.h"

// Interface utilisateur
#include "display/DisplayManager.h"
#include "display/QRCode.h"

// Système
#include "system/BootManager.h"
#include "system/ConfigManager.h"

// Vision
#include "vision/HuskyLensPlus.h"