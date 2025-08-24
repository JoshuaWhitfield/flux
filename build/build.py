import os
import subprocess

# directories containing .cpp files
SRC_DIRS = [
    "syntax/types",
    "syntax/lexer",
    "syntax/parser",
    "syntax/tokens",
    "syntax",
    "engine/core",
    "engine/driver",
    "engine/std",
]

OUTPUT = "flux"  # name of the final executable

def gather_sources():
    sources = []
    for d in SRC_DIRS:
        for f in os.listdir(d):
            if f.endswith(".cpp"):
                sources.append(os.path.join(d, f))
    return sources

def build():
    sources = gather_sources()
    cmd = ["g++", "-std=c++20", "-Wall", "-o", OUTPUT] + sources
    print("Compiling with command:")
    print(" ".join(cmd))
    subprocess.run(cmd, check=True)

if __name__ == "__main__":
    build()
