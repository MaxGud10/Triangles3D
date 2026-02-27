import subprocess
from pathlib import Path
import sys


TEST_FILENAME = Path(__file__).with_name("e2e_tests.txt")
PATH_TO_EXECUTABLE = Path(__file__).parents[1] / "build" / "triag"
TIMEOUT_SEC = 5
SEP = "---"  

def parse_tests(path: Path):
    """
    Test file format:

    TEST_NUMBER: <int>
    INPUT:
    <multiline input>
    OUTPUT:
    <multiline output or <empty>>
    ---
    """
    text = path.read_text(encoding="utf-8")
    blocks = [b.strip() for b in text.split(SEP) if b.strip()]
    tests = []
    for bi, block in enumerate(blocks, 1):
        lines = [ln.rstrip("\n") for ln in block.splitlines()]

        if not lines or not lines[0].startswith("TEST_NUMBER:"):
            raise ValueError(f"[block {bi}] expected 'TEST_NUMBER:' on first line")
        try:
            number = int(lines[0].split(":", 1)[1].strip())
        except Exception as e:
            raise ValueError(f"[block {bi}] invalid TEST_NUMBER: {lines[0]}") from e


        try:
            i_input = lines.index("INPUT:")
            i_output = lines.index("OUTPUT:")
        except ValueError as e:
            raise ValueError(f"[block {bi}] must contain 'INPUT:' and 'OUTPUT:' lines") from e
        if i_output <= i_input:
            raise ValueError(f"[block {bi}] OUTPUT must go after INPUT")

        input_lines = lines[i_input + 1 : i_output]
        output_lines = lines[i_output + 1 : ]


        if len(output_lines) == 1 and output_lines[0].strip().lower() == "<empty>":
            output_lines = []

        tests.append((
            number,
            "\n".join(input_lines) + ("\n" if input_lines else ""),
            output_lines
        ))
    return tests

def run_one(exe: Path, number: int, prog_input: str, ref_lines: list[str]) -> tuple[bool, str]:
    try:
        res = subprocess.run(
            [str(exe)],
            input=prog_input,
            capture_output=True,
            text=True,
            timeout=TIMEOUT_SEC,
        )
    except FileNotFoundError:
        return False, f"Executable not found: {exe}"
    except subprocess.TimeoutExpired:
        return False, "Timeout expired"

    real_lines = [ln.rstrip() for ln in res.stdout.splitlines()]
    ref_norm   = [ln.rstrip() for ln in ref_lines]

    ok = (real_lines == ref_norm) and (res.returncode == 0)
    if ok:
        return True, ""
    else:
        msg = []
        if res.returncode != 0:
            msg.append(f"Exit code: {res.returncode}")
        if res.stderr.strip():
            msg.append(f"Stderr: {res.stderr.strip()}")
        msg.append("Expected:\n" + ("\n".join(ref_norm) if ref_norm else "<empty>"))
        msg.append("Got:\n" + ("\n".join(real_lines) if real_lines else "<empty>"))
        return False, "\n".join(msg)

def main():
    exe = PATH_TO_EXECUTABLE
    if len(sys.argv) >= 2:
        exe = Path(sys.argv[1])

    tests = parse_tests(TEST_FILENAME)
    passed = failed = 0
    for number, prog_input, ref_lines in tests:
        ok, info = run_one(exe, number, prog_input, ref_lines)
        if ok:
            print(f"Test {number}: PASSED")
            passed += 1
        else:
            print(f"Test {number}: FAILED")
            if info:
                print(info)
            failed += 1

    total = passed + failed
    print(f"\nPassed: {passed}")
    print(f"Failed: {failed}")
    print(f"Total:  {total}")
    sys.exit(0 if failed == 0 else 1)

if __name__ == "__main__":
    main()
