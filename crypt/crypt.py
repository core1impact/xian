#!/usr/bin/env python3

import os
import re
import sys


def add_xor(text, key):
    def repl(m):
        tab = m[1]
        var_name = m[3]
        val = m[4]

        val = re.sub('"\n\s+"', '', val)
        val_len = len(val) + 1
        val += '\0'

        val = val.encode().decode('unicode-escape')

        val = xor(val, key)

        hex_arr = []

        for i in val:
            hex_arr.append(hex(ord(i)))

        hex_arr = '{ ' + ', '.join(hex_arr) + ' };'

        return f'\n{tab}char {var_name}[{val_len}] = {hex_arr}\n' \
               f'{tab}xor(&{var_name}, {val_len});\n' \
               f'{tab}/* {m[2]} */'

    return re.sub('\n( *)(char \*(\w+) = "(.+?)";)', repl, text, flags=re.DOTALL)


def del_xor(text):
    def repl(m):
        tab = m[1]
        expr = m[2]

        return f'\n{tab}{expr}'

    return re.sub('\n( *)char \w+\[\d+\] = {.+?};\s+xor\(.+?\);\s+/\* (char \*\w+ = ".+?";) \*/', repl, text,
                  flags=re.DOTALL)


def add_xor_recur(dir, revert=False):
    exclude = ('.idea', 'cmake-build-debug', 'build', 'external', '.git')

    pattern_h = re.compile('.+?\.h$')
    pattern_c = re.compile('.+?\.c$')

    if not revert:
        key = os.urandom(128)
        key_bytes = []

        for i in key:
            key_bytes.append(hex(i))

        xor_key = 'char key[128] = { ' + ', '.join(key_bytes) + ' };'
        xor_key_size = 'int key_size = 128;'

        with open('common/xor.c') as f:
            data = f.read()

            data = data.replace('int key_size = 1;', xor_key_size)
            data = data.replace('char key[] = {1};', xor_key)

            with open('common/xor.c', 'w') as f2:
                f2.write(data)
    else:
        with open('common/xor.c') as f:
            data = f.read()

            data = re.sub('char key\[\d+\] = {.+?};', 'char key[] = {1};', data)
            data = re.sub('int key_size = \d+;', 'int key_size = 1;', data)

            with open('common/xor.c', 'w') as f2:
                f2.write(data)

    for root, dirs, files in os.walk(dir):
        dirs[:] = [d for d in dirs if d not in exclude]

        for file in files:
            if not pattern_h.match(file) and not pattern_c.match(file):
                continue

            file = f'{root}/{file}'

            print(file)

            with open(file) as f:
                data = f.read()

                if revert:
                    new_content = del_xor(data)
                else:
                    new_content = add_xor(data, key)

                with open(file, 'w') as f2:
                    f2.write(new_content)


def xor(data, key):
    output = ''

    for i, ch in enumerate(data):
        output += chr(ord(ch) ^ key[i % len(key)])

    return output


def main():
    if sys.argv[1] == 'encrypt':
        add_xor_recur('.')
        print('\nencrypt ok')
    elif sys.argv[1] == 'decrypt':
        add_xor_recur('.', True)
        print('\ndecrypt ok')


if __name__ == '__main__':
    main()
