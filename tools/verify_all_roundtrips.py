#!/usr/bin/env python3
import sys
import os
import subprocess
import shutil
from are_converter import decompile_area

def clean_dict_keys(d):
    """
    Recursively clean dictionary to remove any structure variations that don't affect game state
    """
    if isinstance(d, dict):
        new_d = {}
        for k, v in d.items():
            new_d[k] = clean_dict_keys(v)
        return new_d
    elif isinstance(d, list):
        return [clean_dict_keys(x) for x in d]
    elif isinstance(d, str):
        # Normalize line endings in strings
        return d.replace('\r\n', '\n').replace('\n\r', '\n').replace('\r', '\n').strip()
    else:
        return d

def deep_compare(path_desc, orig, comp):
    """
    Deep compare two dictionaries/lists/primitives, returning a list of mismatches.
    """
    mismatches = []
    
    if type(orig) != type(comp):
        return [f"{path_desc}: Type mismatch! Original: {type(orig)}, Compiled: {type(comp)}"]
        
    if isinstance(orig, dict):
        # Check keys
        orig_keys = set(orig.keys())
        comp_keys = set(comp.keys())
        
        if orig_keys != comp_keys:
            mismatches.append(f"{path_desc}: Keys mismatch! Original keys: {orig_keys}, Compiled keys: {comp_keys}")
            
        for k in orig_keys.intersection(comp_keys):
            mismatches.extend(deep_compare(f"{path_desc}.{k}", orig[k], comp[k]))
            
    elif isinstance(orig, list):
        if len(orig) != len(comp):
            return [f"{path_desc}: List length mismatch! Original len: {len(orig)}, Compiled len: {len(comp)}"]
        for idx, (o, c) in enumerate(zip(orig, comp)):
            mismatches.extend(deep_compare(f"{path_desc}[{idx}]", o, c))
            
    else:
        if orig != comp:
            mismatches.append(f"{path_desc}: Value mismatch! Original: {repr(orig)}, Compiled: {repr(comp)}")
            
    return mismatches

def run_roundtrip(area_path, temp_dir):
    area_name = os.path.basename(area_path)
    base_name, _ = os.path.splitext(area_name)
    
    toml_path = os.path.join(temp_dir, f"{base_name}.toml")
    compiled_path = os.path.join(temp_dir, f"{base_name}.compiled.are")
    
    # Step 1: Decompile
    cmd1 = ["python3", "/workspaces/ROCSource/tools/are_converter.py", "decompile", area_path, toml_path]
    res1 = subprocess.run(cmd1, capture_output=True, text=True)
    if res1.returncode != 0:
        return False, f"Decompiling failed!\nSTDOUT: {res1.stdout}\nSTDERR: {res1.stderr}"
        
    # Step 2: Compile back
    cmd2 = ["python3", "/workspaces/ROCSource/tools/are_converter.py", "compile", toml_path, compiled_path]
    res2 = subprocess.run(cmd2, capture_output=True, text=True)
    if res2.returncode != 0:
        return False, f"Compiling back failed!\nSTDOUT: {res2.stdout}\nSTDERR: {res2.stderr}"
        
    # Step 3: Parse and compare
    try:
        with open(area_path, "r", encoding="utf-8") as f:
            orig_content = f.read()
        with open(compiled_path, "r", encoding="utf-8") as f:
            comp_content = f.read()
            
        orig_dict = clean_dict_keys(decompile_area(orig_content))
        comp_dict = clean_dict_keys(decompile_area(comp_content))
        
        mismatches = []
        for section in ["area", "helps", "materials", "mobiles", "objects", "rooms"]:
            if section in orig_dict or section in comp_dict:
                mismatches.extend(deep_compare(section, orig_dict.get(section, {}), comp_dict.get(section, {})))
                
        if mismatches:
            # Show up to 10 mismatches
            mismatch_str = "\n".join(mismatches[:10])
            if len(mismatches) > 10:
                mismatch_str += f"\n... and {len(mismatches) - 10} more mismatches."
            return False, mismatch_str
            
        return True, "SUCCESS"
    except Exception as e:
        import traceback
        return False, f"Exception during dictionary comparison: {e}\n{traceback.format_exc()}"

def main():
    if len(sys.argv) > 1:
        test_dir = os.path.abspath(sys.argv[1])
    else:
        test_dir = "/workspaces/ROCSource/ROC_AREA_UTF8/TestArea"
        
    temp_dir = os.path.join(test_dir, "roundtrip_temp")
    
    # Create temp directory
    os.makedirs(temp_dir, exist_ok=True)
    
    sys.path.append("/workspaces/ROCSource/tools")
    
    # Find all .are files (excluding compiled ones)
    area_files = [
        os.path.join(test_dir, f) for f in os.listdir(test_dir)
        if f.endswith(".are") and not f.endswith(".compiled.are")
    ]
    area_files.sort()
    
    print("======================================================================")
    print(f"RUNNING ROUNDTRIP VERIFICATION FOR {len(area_files)} AREA FILES")
    print("======================================================================")
    
    passed_count = 0
    failed_files = []
    
    for area_path in area_files:
        area_name = os.path.basename(area_path)
        print(f"Testing {area_name:<30} ... ", end="", flush=True)
        
        success, message = run_roundtrip(area_path, temp_dir)
        if success:
            print("[ PASSED ]")
            passed_count += 1
        else:
            print("[ FAILED ]")
            print(f"--- FAILURE DETAIL FOR {area_name} ---")
            print(message)
            print("-" * 60)
            failed_files.append((area_name, message))
            
    # Clean up temp directory
    try:
        shutil.rmtree(temp_dir)
    except Exception:
        pass
        
    print("\n======================================================================")
    print("VERIFICATION SUMMARY")
    print("======================================================================")
    print(f"Total Files Tested : {len(area_files)}")
    print(f"Passed             : {passed_count}")
    print(f"Failed             : {len(failed_files)}")
    
    if failed_files:
        print("\nFailed files:")
        for name, _ in failed_files:
            print(f"  - {name}")
        sys.exit(1)
    else:
        print("\nALL FILES PASSED ROUNDTRIP SUCCESSFULLY!")
        sys.exit(0)

if __name__ == "__main__":
    main()
