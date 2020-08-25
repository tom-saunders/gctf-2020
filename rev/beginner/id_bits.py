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
        for j in range(0,32):
            x = affects.get(79 - i, {}).get(j, None)
        for j in range(112, 128):
            x = affects.get(79 - i, {}).get(j, None)


        for j in range(0, 128):
            x = affects.get(79 - i, {}).get(j, None)
            if x:
                sys.stdout.write('x')
            else:
                sys.stdout.write(' ')
            if not (j % 8):
                sys.stdout.write('|')
        sys.stdout.write('|\n')


def edit_dist(left, right):
    if len (left) != len(right):
#        print (f'{left.bytes}')
#        print (f'{right.bytes}')
        return 129
    xord = left ^ right
    set_bits = xord.findall(bitstring.Bits(bin = '1'))
    dist = 0
    for b in set_bits:
        dist += 1
    return dist

def calc_score(to_score):
    new_env = os.environ
    new_env['LD_PRELOAD'] = './intercept.so'
    proc = subprocess.Popen(
            ['./a.out'],
            stdin = subprocess.PIPE,
            stdout = subprocess.PIPE,
            env = new_env)
    (out, err) = proc.communicate(to_score.bytes)

    hex_bytes = out.split(b'\n')[1]
    hex = str(hex_bytes, 'utf-8')
    result = bitstring.Bits(hex=hex)
    score = edit_dist(to_score[0:24], result[0:24]) * 10
    score += edit_dist(to_score[24:112], result[24:112])
    score += edit_dist(to_score[112:120], result[112:120]) * 10
    score += edit_dist(to_score[120:128], result[120:128]) * 100
    return score

def optimal_bytes(fixed_left, fixed_right, options):
    if len(fixed_left) + len(fixed_right) != 120:
        print (f'{fixed_left} {len(fixed_left)} _ {fixed_right} {len(fixed_right)}')
    scores = {}
    for option in options:
        value = fixed_left + option + fixed_right
        score = calc_score(value)
        current = scores.get(score, [])
        current.append((str(option.bytes, 'utf-8'), str(value.bytes, 'utf-8')))
        scores[score] = current
    rscore = None
    i = 0
    while not scores.get(i, None):
        i += 1
    return (scores[i], i)


if __name__ == "__main__":
    exp_chars = []
    for i in range(32, 127):
        exp_chars.append(bitstring.Bits(int = i, length = 8))

    fixed_left = bitstring.Bits(bytes = b'CTF{', length = 32)
    fixed_right = bitstring.Bits(bytes = b'}\x00', length = 16)
    mid = bitstring.Bits(bytes = b'S1MDf0rM3!', length = 80)

    optimal = {}
    for i in range(0, 10):
        padded_left = fixed_left + mid[0:i * 8]
        padded_right = mid[(i + 1) * 8:80] + fixed_right

        optimal[i] = optimal_bytes(padded_left, padded_right, exp_chars)
    for i in range(0, 10):
        print(optimal[i])

