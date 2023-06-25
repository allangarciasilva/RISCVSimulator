import sys
from typing import List

import mif
import numpy as np

input_file = sys.argv[-2]
output_file = sys.argv[-1]


def remove_trailing_zeroes(values: List[int]):
    last_idx = len(values) - 1
    while values[last_idx] == 0:
        last_idx -= 1
    return values[: last_idx + 1]


def np_to_values(array: np.ndarray):
    values = []
    for line in array:
        binary_str = "".join(map(str, line))[::-1]
        values.append(int(binary_str, 2))
    return remove_trailing_zeroes(values)


def to_c_array_element(value: int):
    if value != 0:
        return hex(value)
    return "0"


def values_to_c(values: List[int]):
    array = "{" + ", ".join(map(to_c_array_element, values)) + "}"

    return f"""\
#include "common/ICMC/icmc_ram.h"

uint16_t icmc_ram[ICMCProcessor::ROM_SIZE] = {array};    
"""


def main():
    with open(input_file) as f:
        binary_content = mif.load(f)

    file_content = values_to_c(np_to_values(binary_content))
    with open(output_file, "w") as f:
        f.write(file_content)


if __name__ == "__main__":
    main()
