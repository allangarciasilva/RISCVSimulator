import sys
from typing import List

elf_file = sys.argv[-2]
mif_file = sys.argv[-1]


def chunk(iterable, stride: int):
    for i in range(0, len(iterable), stride):
        value = iterable[i : i + stride]
        if len(value) < stride:
            value += "0" * (stride - len(value))
        yield value


def to_little_endian(hex: str):
    bytes = reversed(list(chunk(hex, 2)))
    return "".join(bytes)


def first_idx_of_zeroes(hex_values: List[str]):
    for i in range(len(hex_values) - 1, 0, -1):
        if hex_values[i] != "00000000":
            return i + 1
    return len(hex_values)


def remove_trailing_zeroes(hex_values: List[str]):
    return hex_values[: first_idx_of_zeroes(hex_values)]


def render_hex_values_as_mif(hex_values: List[str], total_words: int):
    lines = [f"{index} : {value}" for index, value in enumerate(hex_values)]
    if len(hex_values) < total_words:
        last_line = f"[{len(hex_values)}..{total_words - 1}] : 00000000"
        lines.append(last_line)

    rendered_lines = "\n".join("\t" + line + ";" for line in lines)
    file_content = f"""\
WIDTH=32;
DEPTH={total_words};

ADDRESS_RADIX=UNS;
DATA_RADIX=HEX;

CONTENT BEGIN
{rendered_lines}
END;
"""
    return file_content


def main():
    with open(elf_file, "rb") as f:
        content = f.read().hex()

    hex_values = list(map(to_little_endian, chunk(content, 8)))
    hex_values = remove_trailing_zeroes(hex_values)

    total_words = 2**16
    mif_content = render_hex_values_as_mif(hex_values, total_words)
    with open(mif_file, "w") as f:
        f.write(mif_content)


if __name__ == "__main__":
    main()
