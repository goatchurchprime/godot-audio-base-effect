#!/usr/bin/env python

from SCons.Script import SConscript
from SCons.Script.SConscript import SConsEnvironment
from SCons.Script import ARGLIST, ARGUMENTS, BUILD_TARGETS, COMMAND_LINE_TARGETS, DEFAULT_TARGETS

import SCons, SCons.Script
import sys, os, platform
import lib_utils, lib_utils_external

# Fixing the encoding of the console
if platform.system() == "Windows":
    os.system("chcp 65001")

EnsureSConsVersion(4, 0)  # type: ignore
EnsurePythonVersion(3, 8)  # type: ignore

# Project config
project_name = "BaseEffect"
lib_name = "baseffect"
default_output_dir = os.path.join("addons", "baseeffect", "libs")
src_folder = "src"

patches_to_apply_godot = [
    "patches/godot_cpp_exclude_unused_classes.patch", # Removes unused godot-cpp classes from the build process
    "patches/unity_build.patch", # Speeds up the build by merging the source files. It can increase the size of assemblies.
]

print(
    f"If you add new source files (e.g. .cpp, .c), do not forget to specify them in '{src_folder}/default_sources.json'."
    + f"\n\tOr add them to 'setup_defines_and_flags' inside 'SConstruct'."
)
print("To apply git patches, use 'scons apply_patches'.")

# Additional console arguments
def setup_options(env: SConsEnvironment, arguments):
    from SCons.Variables import Variables, BoolVariable, EnumVariable, PathVariable

    opts = Variables([], arguments)

    # It must be here for lib_utils.py
    opts.Add(PathVariable("addon_output_dir", "Path to the output directory", default_output_dir, PathVariable.PathIsDirCreate))

    opts.Update(env)
    env.Help(opts.GenerateHelpText(env))


# Additional compilation flags
def setup_defines_and_flags(env: SConsEnvironment, src_out: list):
    # Add more sources to `src_out` if needed

    if env.get("lto") != "none":
        if env.get("is_msvc", False):
            env.AppendUnique(CCFLAGS=["/GL"],
                             ARFLAGS=["/LTCG"],
                             LINKFLAGS=["/LTCG"],)
        else:
            env.AppendUnique(CCFLAGS=["-flto"],
                             LINKFLAGS=["-flto"],)
    else:
        if env.get("is_msvc", False):
            env.AppendUnique(LINKFLAGS=["/incremental:no"])


    if env.get("is_msvc", False):
        env.Append(CCFLAGS=["/GF"])  # Eliminate Duplicate Strings
        env.Append(LINKFLAGS=["/WX:NO"])

    if env["platform"] in ["linux"]: # , "android"?
        env.Append(
            LINKFLAGS=[
                "-static-libgcc",
                "-static-libstdc++",
            ]
        )
    if env["platform"] == "android":
        env.Append(
            LIBS=[
                "log",
            ]
        )
    print()


def apply_patches(target, source, env: SConsEnvironment):
    return lib_utils_external.apply_git_patches(env, patches_to_apply_godot, "godot-cpp")

def get_android_toolchain() -> str:
    sys.path.insert(0, "godot-cpp/tools")
    import android  # type: ignore

    sys.path.pop(0)
    return os.path.join(android.get_android_ndk_root(env), "build/cmake/android.toolchain.cmake")

env: SConsEnvironment = SConscript("godot-cpp/SConstruct")
env = env.Clone()

args = ARGUMENTS
additional_src = []
setup_options(env, args)
setup_defines_and_flags(env, additional_src)

extra_tags = ""

output_path = env["addon_output_dir"]

lib_utils.get_library_object(env, project_name, lib_name, extra_tags, output_path, src_folder, additional_src)

# Register console commands
env.Command("apply_patches", [], apply_patches)
