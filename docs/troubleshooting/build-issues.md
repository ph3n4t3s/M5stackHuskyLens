# Build System Issues

## Current Status

The project is currently experiencing several issues related to the build system and code structure. This document outlines the problems and proposed solutions.

## Major Issues

### 1. ESP Async WebServer Dependency

#### Problem
- Missing header files (ESPAsyncWebSrv.h)
- Conflicts between different versions of AsyncWebServer
- Version mismatch with AsyncTCP

#### Current Workaround
```cpp
// WebServer.h adapter
#ifdef ARDUINO_ARCH_ESP32
#if defined(ESP_ASYNC_WEBSERVER)
#include <ESPAsyncWebServer.h>
#else 
#include <AsyncWebServer_ESPAsyncWebServer.h>
#endif
#else
#include <ESPAsyncWebServer.h>
#endif
```

#### Proposed Solution
1. Use consistent ESPAsyncWebServer-esphome across the project
2. Update platformio.ini dependencies
3. Add proper version constraints

### 2. Code Structure Issues

#### Problem
- SensorData type not recognized
- Inconsistent namespace usage
- Header organization needs improvement

#### Current State
```cpp
// Before: Direct namespace access
Constants::SCREEN_WIDTH

// After: Proper namespace organization
Constants::Display::SCREEN_WIDTH

// Alternative: Using declaration
using Constants::Display::SCREEN_WIDTH;
```

#### Proposed Solution
1. Reorganize header files
2. Implement consistent namespace usage
3. Add proper forward declarations
4. Document header dependencies

### 3. Resource Usage

#### Problem
- High CPU usage during compilation
- Excessive memory consumption
- Long build times

#### Current Status
- Build process can timeout
- System becomes unresponsive

#### Proposed Solution
1. Optimize include dependencies
2. Implement precompiled headers
3. Review and cleanup template usage
4. Consider using ccache

## Next Steps

### Immediate Actions
1. Fix ESPAsyncWebServer dependency
2. Reorganize header files
3. Implement namespace fixes

### Short-term Goals
1. Optimize build process
2. Add dependency caching
3. Improve documentation

### Long-term Goals
1. Implement precompiled headers
2. Setup CI/CD pipeline
3. Add automated testing

## Contributing

If you want to help fix these issues:

1. Pick an issue from GitHub
2. Create a new branch
3. Make your changes
4. Submit a Pull Request

## Resources

- [ESPAsyncWebServer Documentation](https://github.com/me-no-dev/ESPAsyncWebServer)
- [PlatformIO Build System](https://docs.platformio.org/en/latest/integration/ide/vscode.html)
- [C++ Best Practices](https://github.com/isocpp/CppCoreGuidelines)