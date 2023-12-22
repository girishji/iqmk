# iQMK

iQMK is a lean reliable keyboard firmare adapted from [qmk](https://github.com/qmk/qmk_firmware) and ported to pico, xxx. It is built using modern cmake after removing qmk's notorious middleware and other gunk.


It designed to be memory-safe with no dynamic allocation and thread-safe with all interrupt events being deferred and then handled in the non-ISR task function.

This WIP.

