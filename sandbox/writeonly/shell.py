#!/usr/bin/env python

from pwn import *

context(arch = 'x86_64', os = 'linux')
r.remote('writeonly.2020.ctfcompetition.com', 1337)



