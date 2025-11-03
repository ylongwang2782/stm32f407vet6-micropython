#!/usr/bin/env python3
"""
Wrapper script to run GCC preprocessor with output redirection.
This handles the cross-platform issue where '>' redirection doesn't work well in CMake on Windows.
"""

import sys
import subprocess


def main():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <output_file> <compiler> [args...]")
        sys.exit(1)

    output_file = sys.argv[1]
    compiler_args = sys.argv[2:]

    # Find the input file (last argument that doesn't start with -)
    input_file = None
    for arg in reversed(compiler_args):
        if not arg.startswith("-"):
            input_file = arg
            break

    if not input_file:
        sys.stderr.write("Error: No input file found\n")
        sys.exit(1)

    # Read the input file
    try:
        with open(input_file, "r", encoding="utf-8") as f:
            input_data = f.read()
    except Exception as e:
        sys.stderr.write(f"Error reading input file: {e}\n")
        sys.exit(1)

    # Remove the input file from compiler args and use stdin instead
    compiler_args_modified = [arg for arg in compiler_args if arg != input_file]
    compiler_args_modified.append("-")  # Use stdin

    try:
        # Run the compiler/preprocessor with stdin input
        result = subprocess.run(
            compiler_args_modified,
            input=input_data,
            capture_output=True,
            text=True,
            check=False,  # Don't raise exception on non-zero return code
        )

        # Write the output to file
        with open(output_file, "w", encoding="utf-8") as f:
            f.write(result.stdout)

        # Print any errors to stderr (but ignore the "linker input" warning)
        if result.stderr:
            lines = result.stderr.split("\n")
            for line in lines:
                if "linker input file unused" not in line and line.strip():
                    sys.stderr.write(line + "\n")

        # Return the compiler's exit code
        sys.exit(result.returncode)

    except Exception as e:
        sys.stderr.write(f"Error: {e}\n")
        sys.exit(1)


if __name__ == "__main__":
    main()
