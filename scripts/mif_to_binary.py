import sys

import mif

input_file = sys.argv[-2]
output_file = sys.argv[-1]


def main():
    with open(input_file) as f:
        binary_content = mif.load(f)
    binary_content.astype("bool").tofile(output_file)


if __name__ == "__main__":
    main()
