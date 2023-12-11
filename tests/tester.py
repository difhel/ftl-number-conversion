import random


tests = [
    (
        "From N to 10 radix",
        "g++ tests/test_to_10ss.cpp -Isrc -o build/test1 -g",
        "./build/test1",
        "python3 tests/test_to_10ss.py", lambda: (
            base := random.randint(2, 36),
            alphabet := "0123456789abcdefghijklmnopqrstuvwxyz".upper()[:base],
            num := "".join([random.choice(alphabet) for _ in range(random.randint(1, 20))]),
            num + "_" + str(base)
        )[-1], 1000),
    (
        "From 10 to N radix",
        "g++ tests/test_from_10ss.cpp -Isrc -o build/test2 -g",
        "./build/test2",
        "python3 tests/test_from_10ss.py", lambda: (
            num := random.randint(1, 2 * 10**9),
            base := random.randint(2, 36),
            str(num) + "\n" + str(base)
        )[-1], 1000),
]

import os

def run_cpp(run_cmd, input_data):
    with open("/tmp/input.txt", "w") as f:
        f.write(input_data)
    os.system(f"{run_cmd} < /tmp/input.txt > /tmp/output.txt")
    
    with open("/tmp/output.txt") as f:
        return f.read().strip()

def run_python(run_cmd, input_data):
    # print("Running...")
    with open("/tmp/input.txt", "w") as f:
        f.write(input_data)
    os.system(f"{run_cmd} < /tmp/input.txt > /tmp/output.txt")
    
    with open("/tmp/output.txt") as f:
        return f.read().strip()

def run_test(test, input_data):
    _, _, exec_cmd, check_cmd, _, _ = test
    
    
    cpp_output = run_cpp(exec_cmd, input_data)
    python_output = run_python(check_cmd, input_data)
    if cpp_output != python_output:
        print(f"Input: {input_data}")
        print(f"Expected: {python_output}")
        print(f"Got: {cpp_output}")
        raise Exception("Test failed")

for test in tests:
    print("===")
    print("Started testing test", test[0])
    print("Compiling...")
    os.system(test[1])
    for i in range(test[5]):
        run_test(test, str(test[4]()))
        if i % 20 == 0:
            print(f"Test {i} passed")