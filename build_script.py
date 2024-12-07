Import("env")

env.Append(
    CPPDEFINES=[
        ("ESP32", "1"),
        ("M5STACK_CORES3", "1"),
        ("USE_TFLITE", "1"),
    ],
    CPPPATH=[
        "lib/tensorflow_lite",
        "lib/tensorflow_lite/third_party/flatbuffers/include",
        "lib/tensorflow_lite/third_party/gemmlowp",
        "lib/tensorflow_lite/third_party/ruy",
    ],
    CXXFLAGS=[
        "-Wno-sign-compare",
        "-Wno-unused-parameter",
        "-Wno-missing-field-initializers",
        "-Wno-write-strings",
        "-fno-threadsafe-statics",
        "-fno-rtti",
        "-fno-exceptions",
        "-ffunction-sections",
        "-fdata-sections",
        "-std=gnu++17",
        "-O3",
    ],
)

# Créer la partition huge_app.csv si elle n'existe pas
def create_huge_app_csv():
    content = """# Name,   Type, SubType, Offset,  Size, Flags
nvs,      data, nvs,     0x9000,  0x5000,
otadata,  data, ota,     0xe000,  0x2000,
app0,     app,  ota_0,   0x10000, 0x300000,
app1,     app,  ota_1,   0x310000,0x300000,
spiffs,   data, spiffs,  0x610000,0x9F0000,"""
    
    with open("huge_app.csv", "w") as f:
        f.write(content)

create_huge_app_csv()