#!/usr/bin/env python
from random import randint
from itertools import chain

pulses = [0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200]
codes = {
        0:  [1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1],
        1:  [1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1],
        7:  [1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0],
        8:  [1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0],
        12: [1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0],
        22: [1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1],
        24: [1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1],
        27: [1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1] }
ids = list(codes.keys())

def get_pulses(k):
    return [pulses[j] for j in range(len(pulses)) if codes[k][j]]

def pulses_csv(pulses):
    return ','.join(map(str, pulses))

def construct_miles(args):
    pulses = [get_pulses(c) + [c] for c in args.codes]
    return '\n'.join(pulses_csv(p) for p in pulses)

def construct_seq(args):
    pulses = [get_pulses(c) for c in args.codes]
    for i in range(1, len(pulses)):
        for j in range(len(pulses[i])):
            pulses[i][j] += pulses[i-1][-1] + (pulses[i-1][1] - pulses[i-1][0])
    return ','.join(map(str, [ p+args.shift for p in chain(*pulses) ]))

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(title='subcommands', help='sub-command help')

    miles = subparsers.add_parser('m', help='construct code file')
    miles.set_defaults(func=construct_miles)
    miles.add_argument('codes', help='Code IDs to include',
            type=int, nargs='*', choices=ids)

    seq = subparsers.add_parser('s', help='construct sequence file',
            formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    seq.set_defaults(func=construct_seq)
    seq.add_argument('-s', '--shift', type=int, help='Amount to shift', default=0)
    seq.add_argument('codes', help='Code IDs to include',
            type=int, nargs='*', choices=ids)

    args = parser.parse_args()
    print(args.func(args))
