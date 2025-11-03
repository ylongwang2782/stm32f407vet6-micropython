#!/usr/bin/env python3
"""
Helper script for preprocessing qstr definitions.
This replaces the sed+cat pipeline with a cross-platform Python solution.
"""

import sys
import re

def process_qstr_step1(input_file, output_file):
    """Add quotes around Q(...) definitions"""
    with open(input_file, 'r', encoding='utf-8') as f:
        data = f.read()
    
    # Replace Q(...) with "Q(...)"
    data = re.sub(r'^Q\((.*)\)$', r'"Q(\1)"', data, flags=re.MULTILINE)
    
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write(data)

def process_qstr_step2(input_file, output_file):
    """Remove quotes from preprocessed Q(...) definitions"""
    with open(input_file, 'r', encoding='utf-8') as f:
        data = f.read()
    
    # Remove quotes around Q(...) after preprocessing
    data = re.sub(r'^"(Q\(.*\))"$', r'\1', data, flags=re.MULTILINE)
    
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write(data)

if __name__ == '__main__':
    if len(sys.argv) < 4:
        print(f"Usage: {sys.argv[0]} <step1|step2> <input_file> <output_file>")
        sys.exit(1)
    
    step = sys.argv[1]
    input_file = sys.argv[2]
    output_file = sys.argv[3]
    
    if step == 'step1':
        process_qstr_step1(input_file, output_file)
    elif step == 'step2':
        process_qstr_step2(input_file, output_file)
    else:
        print(f"Unknown step: {step}")
        sys.exit(1)

