#!/usr/bin/env python

import subprocess
import os
import sys

import bitstring

def ident_bits():
    fixed_left = bitstring.Bits(bytes = b'CTF{', length = 32)
    fixed_right = bitstring.Bits(bytes = b'}\x00', length = 16)

    # b'P' == 0x50 which any bit flip does not create whitespace
    # as interpreted by scanf
    centre = bitstring.BitArray(bytes = b'PPPPPPPPPP', length = 80)
    inv_centre = bitstring.BitArray(bytes = b'\xaf' * 10, length = 80)

    base = fixed_left + centre + fixed_right
    new_env = os.environ
    new_env['LD_PRELOAD'] = './intercept.so'
    base_proc = subprocess.Popen(
            ['./a.out'],
            stdin = subprocess.PIPE,
            stdout = subprocess.PIPE,
            env = new_env)
    (base_out, err) = base_proc.communicate(base.bytes)

    base_hex_bytes = base_out.split(b'\n')[1]
    base_hex = str(base_hex_bytes, 'utf-8')
    base_result = bitstring.Bits(hex=base_hex)

    inv_base = fixed_left + inv_centre + fixed_right
    new_env = os.environ
    new_env['LD_PRELOAD'] = './intercept.so'
    inv_base_proc = subprocess.Popen(
            ['./a.out'],
            stdin = subprocess.PIPE,
            stdout = subprocess.PIPE,
            env = new_env)
    (inv_base_out, err) = inv_base_proc.communicate(inv_base.bytes)

    inv_base_hex_bytes = inv_base_out.split(b'\n')[1]
    inv_base_hex = str(inv_base_hex_bytes, 'utf-8')
    inv_base_result = bitstring.Bits(hex=inv_base_hex)

#    print(base)
#    print(base_result)
    tests = {}

    affects = {}
    affected = {}
    for i in range(0, 80):
        affects[i] = {}
        mask = bitstring.Bits(bin = ('0' * (80 - i - 1)) + '1' + ('0' * i))
        result = centre ^ mask
        test = fixed_left + result + fixed_right
        test_proc = subprocess.Popen(
                ['./a.out'],
                stdin = subprocess.PIPE,
                stdout = subprocess.PIPE,
                env = new_env)
        (test_out, err) = test_proc.communicate(test.bytes)

        test_hex_bytes = test_out.split(b'\n')[1]
        test_hex = str(test_hex_bytes, 'utf-8')
        test_result = bitstring.Bits(hex=test_hex)
        test_masked = base_result ^ test_result
        tests[i] = test_masked
        set_bits = test_masked.findall(bitstring.Bits(bin = '1'))
        for set_bit in set_bits:
            affects[i][set_bit] = True
            x = affected.get(set_bit, {})
            x[i] = True
            affected[set_bit] = x
    for i in range(0, 80):
        mask = bitstring.Bits(bin = ('0' * (80 - i - 1)) + '1' + ('0' * i))
        result = inv_centre ^ mask
        test = fixed_left + result + fixed_right
        test_proc = subprocess.Popen(
                ['./a.out'],
                stdin = subprocess.PIPE,
                stdout = subprocess.PIPE,
                env = new_env)
        (test_out, err) = test_proc.communicate(test.bytes)

        test_hex_bytes = test_out.split(b'\n')[1]
        test_hex = str(test_hex_bytes, 'utf-8')
        test_result = bitstring.Bits(hex=test_hex)
        test_masked = inv_base_result ^ test_result
        tests[i] = test_masked
        set_bits = test_masked.findall(bitstring.Bits(bin = '1'))
        for set_bit in set_bits:
            affects[i][79 - set_bit] = True
            x = affected.get(79 - set_bit, {})
            x[i] = True
            affected[79 - set_bit] = x

    for i in range(0,80):
        if not (i % 8):
            print('-' * 146)
        for j in range(0, 128):
            x = affects.get(79 - i, {}).get(j, None)
            if x:
                sys.stdout.write('x')
            else:
                sys.stdout.write(' ')
            if not (j % 8):
                sys.stdout.write('|')
        sys.stdout.write('|\n')

if __name__ == "__main__":
    ident_bits()

