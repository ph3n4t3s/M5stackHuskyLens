---
name: Code Structure Issue
about: Report issues with code organization and type system
title: '[CODE] Type System and Namespace Organization Issues'
labels: 'bug, code-organization, critical'
assignees: ''
---

# Code Structure Issue

## Current Behavior
The codebase exhibits structural issues related to type visibility, namespace organization, and constant definitions.

## Expected Behavior
- Proper type visibility across compilation units
- Consistent namespace usage
- Clear and organized header structure
- Efficient compilation process

## Technical Details

### Type System Issues
- SensorData structure not recognized across files
- Multiple definition errors
- Include path resolution problems

### Namespace Inconsistencies
```cpp
// Current problematic usage
Constants::SCREEN_WIDTH vs Constants::Display::SCREEN_WIDTH

// Issues with namespace organization
namespace Constants {
    namespace Display {
        const int SCREEN_WIDTH = 320;
    }
}
```

### Header Organization
- Circular dependencies
- Missing include guards
- Inconsistent include paths

## Code Examples

### Current Implementation
```cpp
// In ImageProcessor.cpp
void processImage(SensorData& data) {
    if (point.x >= 0 && point.x < Constants::SCREEN_WIDTH &&
        point.y >= 0 && point.y < Constants::SCREEN_HEIGHT) {
        // ...
    }
}
```

### Proposed Implementation
```cpp
// In ImageProcessor.cpp
namespace {
    using Constants::Display::SCREEN_WIDTH;
    using Constants::Display::SCREEN_HEIGHT;
}

void processImage(SensorData& data) {
    if (point.x >= 0 && point.x < SCREEN_WIDTH &&
        point.y >= 0 && point.y < SCREEN_HEIGHT) {
        // ...
    }
}
```

## Steps to Reproduce
1. Clone the repository
2. Attempt to compile with `pio run`
3. Observe type-related compilation errors
4. Note namespace inconsistencies in error messages

## Error Messages
```cpp
error: 'SensorData' does not name a type
error: 'SCREEN_WIDTH' is not a member of 'Constants'
```

## Proposed Solutions
1. Reorganize header files
2. Implement consistent namespace usage
3. Fix include paths
4. Add proper forward declarations
5. Document header dependencies

## Environment
- OS: Ubuntu
- Compiler: GCC for ESP32
- C++ Standard: C++11
- Build System: PlatformIO

## Additional Context
These issues affect code maintainability and compile times significantly.

## Related Issues
- #1 Build System Issues