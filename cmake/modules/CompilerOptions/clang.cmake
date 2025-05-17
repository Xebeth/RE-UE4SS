set(DEFAULT_COMPILER_FLAGS "-g;-gcodeview;-fcolor-diagnostics;-Wno-unknown-pragmas;-Wno-unused-parameter;-fms-extensions;-Wignored-attributes;-Wno-deprecated-enum-enum-conversion;-Wno-enum-enum-conversion;-Wno-enum-float-conversion;-Wno-deprecated-enum-float-conversion;-Wno-deprecated-anon-enum-enum-conversion;-Wno-error=enum-enum-conversion;-Wno-error=enum-float-conversion;-Wno-error=deprecated-enum-enum-conversion;-Wno-error=deprecated-enum-float-conversion;-Wno-error=deprecated-anon-enum-enum-conversion" PARENT_SCOPE)

set(LINKER_FLAGS "-g" PARENT_SCOPE)
set(DEFAULT_SHARED_LINKER_FLAGS "${LINKER_FLAGS}" PARENT_SCOPE)
set(DEFAULT_EXE_LINKER_FLAGS "${LINKER_FLAGS}" PARENT_SCOPE)

set(Shipping_FLAGS "" PARENT_SCOPE)