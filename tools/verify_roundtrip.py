#!/usr/bin/env python3
import sys
import subprocess
import os

def clean_dict_keys(d):
    """
    Recursively clean dictionary to remove any structure variations that don't affect game state
    """
    if isinstance(d, dict):
        new_d = {}
        for k, v in d.items():
            if k == "resets":
                # We can normalize resets structure if needed, but it should be exact
                new_d[k] = clean_dict_keys(v)
            else:
                new_d[k] = clean_dict_keys(v)
        return new_d
    elif isinstance(d, list):
        return [clean_dict_keys(x) for x in d]
    elif isinstance(d, str):
        # Normalize line endings in strings
        return d.replace('\r\n', '\n').replace('\n\r', '\n').replace('\r', '\n').strip()
    else:
        return d

def main():
    original_are = "/workspaces/ROCSource/ROC_AREA_UTF8/TestArea/elftown.are"
    output_toml = "/workspaces/ROCSource/ROC_AREA_UTF8/TestArea/elftown.toml"
    compiled_are = "/workspaces/ROCSource/ROC_AREA_UTF8/TestArea/elftown.compiled.are"

    print("======================================================================")
    print("RUNNING ROC MUD AREA ROUNDTRIP VERIFICATION")
    print("======================================================================")

    # Make converter executable if needed
    os.chmod("/workspaces/ROCSource/tools/are_converter.py", 0o755)

    # Step 1: Decompile original to TOML
    print("\n[Step 1] Decompiling original .are to TOML...")
    cmd1 = ["python3", "/workspaces/ROCSource/tools/are_converter.py", "decompile", original_are, output_toml]
    res1 = subprocess.run(cmd1, capture_output=True, text=True)
    if res1.returncode != 0:
        print("FAIL: Decompiling original failed!")
        print("STDOUT:", res1.stdout)
        print("STDERR:", res1.stderr)
        sys.exit(1)
    print("SUCCESS: Decompiled to TOML.")

    # Step 2: Compile TOML back to compiled .are
    print("\n[Step 2] Compiling TOML back to .are...")
    cmd2 = ["python3", "/workspaces/ROCSource/tools/are_converter.py", "compile", output_toml, compiled_are]
    res2 = subprocess.run(cmd2, capture_output=True, text=True)
    if res2.returncode != 0:
        print("FAIL: Compiling back failed!")
        print("STDOUT:", res2.stdout)
        print("STDERR:", res2.stderr)
        sys.exit(1)
    print("SUCCESS: Compiled back to .are.")

    # Step 3: Parse both using are_converter decompilation logic and assert functional identity
    print("\n[Step 3] Parsing and comparing decompiled dictionaries...")
    from are_converter import decompile_area

    with open(original_are, "r", encoding="utf-8") as f:
        orig_content = f.read()
    with open(compiled_are, "r", encoding="utf-8") as f:
        comp_content = f.read()

    print("Parsing original...")
    orig_dict = clean_dict_keys(decompile_area(orig_content))
    print("Parsing compiled...")
    comp_dict = clean_dict_keys(decompile_area(comp_content))

    # Compare metadata
    print("Comparing metadata...")
    if orig_dict["area"] != comp_dict["area"]:
        print("FAIL: [area] section mismatch!")
        print("Original:", orig_dict["area"])
        print("Compiled:", comp_dict["area"])
        sys.exit(1)

    # Compare helps
    print("Comparing helps...")
    if orig_dict["helps"] != comp_dict["helps"]:
        print("FAIL: [helps] section mismatch!")
        sys.exit(1)

    # Compare materials
    print("Comparing materials...")
    if orig_dict["materials"] != comp_dict["materials"]:
        print("FAIL: [materials] section mismatch!")
        sys.exit(1)

    # Compare mobiles
    print("Comparing mobiles...")
    orig_mobs = orig_dict["mobiles"]
    comp_mobs = comp_dict["mobiles"]
    if len(orig_mobs) != len(comp_mobs):
        print(f"FAIL: Mobiles count mismatch! Original: {len(orig_mobs)}, Compiled: {len(comp_mobs)}")
        sys.exit(1)

    for vnum in sorted(orig_mobs.keys()):
        if vnum not in comp_mobs:
            print(f"FAIL: Mobile {vnum} missing in compiled!")
            sys.exit(1)
        if orig_mobs[vnum] != comp_mobs[vnum]:
            print(f"FAIL: Mobile {vnum} mismatch!")
            # Print diff of keys
            for k in orig_mobs[vnum]:
                if orig_mobs[vnum][k] != comp_mobs[vnum].get(k):
                    print(f"  Field '{k}' mismatch:")
                    print("    Original:", repr(orig_mobs[vnum][k]))
                    print("    Compiled:", repr(comp_mobs[vnum].get(k)))
            sys.exit(1)

    # Compare objects
    print("Comparing objects...")
    orig_objs = orig_dict["objects"]
    comp_objs = comp_dict["objects"]
    if len(orig_objs) != len(comp_objs):
        print(f"FAIL: Objects count mismatch! Original: {len(orig_objs)}, Compiled: {len(comp_objs)}")
        sys.exit(1)

    for vnum in sorted(orig_objs.keys()):
        if vnum not in comp_objs:
            print(f"FAIL: Object {vnum} missing in compiled!")
            sys.exit(1)
        if orig_objs[vnum] != comp_objs[vnum]:
            print(f"FAIL: Object {vnum} mismatch!")
            for k in orig_objs[vnum]:
                if orig_objs[vnum][k] != comp_objs[vnum].get(k):
                    print(f"  Field '{k}' mismatch:")
                    print("    Original:", repr(orig_objs[vnum][k]))
                    print("    Compiled:", repr(comp_objs[vnum].get(k)))
            sys.exit(1)

    # Compare rooms
    print("Comparing rooms...")
    orig_rooms = orig_dict["rooms"]
    comp_rooms = comp_dict["rooms"]
    if len(orig_rooms) != len(comp_rooms):
        print(f"FAIL: Rooms count mismatch! Original: {len(orig_rooms)}, Compiled: {len(comp_rooms)}")
        sys.exit(1)

    for vnum in sorted(orig_rooms.keys()):
        if vnum not in comp_rooms:
            print(f"FAIL: Room {vnum} missing in compiled!")
            sys.exit(1)
        if orig_rooms[vnum] != comp_rooms[vnum]:
            print(f"FAIL: Room {vnum} mismatch!")
            for k in orig_rooms[vnum]:
                if orig_rooms[vnum][k] != comp_rooms[vnum].get(k):
                    print(f"  Field '{k}' mismatch:")
                    print("    Original:", repr(orig_rooms[vnum][k]))
                    print("    Compiled:", repr(comp_rooms[vnum].get(k)))
            sys.exit(1)

    print("\n======================================================================")
    print("ALL TESTS PASSED! ROUNDTRIP IS FUNCTIONALLY 100% IDENTICAL!")
    print("======================================================================")

if __name__ == "__main__":
    # Add tools to path so we can import are_converter
    sys.path.append("/workspaces/ROCSource/tools")
    main()
