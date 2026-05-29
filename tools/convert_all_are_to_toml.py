#!/usr/bin/env python3
"""
Area Converter Script: Convert all `.are` files in a source directory to `.toml` files.
This script uses `are_converter.py` to perform the decompilation and does not delete the original files.
"""

import sys
import os

def read_file_with_encoding(file_path):
    """
    Attempt to read a file with multiple encodings to handle both legacy (Big5, GBK, CP950) 
    and UTF-8 converted area files.
    """
    encodings = ["utf-8", "big5", "gbk", "cp950", "latin-1"]
    for enc in encodings:
        try:
            with open(file_path, "r", encoding=enc) as f:
                content = f.read()
            return content, enc
        except UnicodeDecodeError:
            continue
    # Fallback to UTF-8 with character replacement
    with open(file_path, "r", encoding="utf-8", errors="replace") as f:
        return f.read(), "utf-8 (with replacement)"

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 convert_all_are_to_toml.py <source_folder_path>")
        sys.exit(1)
        
    source_folder = os.path.abspath(sys.argv[1])
    if not os.path.isdir(source_folder):
        print(f"Error: '{source_folder}' is not a valid directory.")
        sys.exit(1)
        
    # Locate tools directory relative to this script
    script_dir = os.path.dirname(os.path.abspath(__file__))
    sys.path.append(script_dir)
    
    try:
        from are_converter import decompile_area, dump_toml_string
    except ImportError as e:
        print(f"Error: Could not import 'are_converter' from the tools directory ({script_dir}).")
        print(f"Details: {e}")
        sys.exit(1)
        
    # Find all .are files (excluding compiled ones to avoid clutter)
    area_files = [
        f for f in os.listdir(source_folder)
        if f.endswith(".are") and not f.endswith(".compiled.are")
    ]
    
    if not area_files:
        print(f"No '.are' files found in '{source_folder}'.")
        sys.exit(0)
        
    area_files.sort()
    
    print("==============================================================")
    print(f"CONVERTING {len(area_files)} AREA FILES TO TOML")
    print(f"Source Folder: {source_folder}")
    print("==============================================================")
    
    success_count = 0
    fail_count = 0
    
    for idx, filename in enumerate(area_files, start=1):
        are_path = os.path.join(source_folder, filename)
        base_name, _ = os.path.splitext(filename)
        toml_filename = f"{base_name}.toml"
        toml_path = os.path.join(source_folder, toml_filename)
        
        print(f"[{idx}/{len(area_files)}] Converting {filename:<30} ... ", end="", flush=True)
        
        try:
            content, detected_encoding = read_file_with_encoding(are_path)
            
            # Decompile area content to dictionary
            data = decompile_area(content)
            
            # Dump to TOML format string
            toml_content = dump_toml_string(data)
            
            # Write out to TOML file
            with open(toml_path, "w", encoding="utf-8") as f:
                f.write(toml_content)
                
            print(f"[ PASSED ] (detected encoding: {detected_encoding})")
            success_count += 1
            
        except Exception as e:
            print("[ FAILED ]")
            print(f"  Error: {e}")
            fail_count += 1
            
    print("==============================================================")
    print("CONVERSION SUMMARY")
    print("==============================================================")
    print(f"Total Files Scanned : {len(area_files)}")
    print(f"Passed              : {success_count}")
    print(f"Failed              : {fail_count}")
    print(f"Original '.are' files have NOT been deleted.")
    print("==============================================================")
    
    if fail_count > 0:
        sys.exit(1)
    else:
        sys.exit(0)

if __name__ == "__main__":
    main()
