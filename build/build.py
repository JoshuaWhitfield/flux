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
OUTPUT_DIR = "test"
MAIN_CLASS = "engine.driver.Interpreter"

def gather_sources():
    sources = []
    for d in SRC_DIRS:
        for f in os.listdir(d):
            if f.endswith(".java"):
                sources.append(os.path.join(d, f))
    return sources

def build():
    sources = gather_sources()
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    cmd = ["javac", f"--release=24", "-d", OUTPUT] + sources
    print("Compiling with command:")
    print(" ".join(cmd))
    subprocess.run(cmd, check=True)

def run():
    cmd = ["java", "--enable-preview", "-cp", OUTPUT_DIR, MAIN_CLASS]
    print("Running with command: ")
    print(" ".join(cmd))
    subprocess.run(cmd, check=True)

if __name__ == "__main__":
    build()
    run()
