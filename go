#!/bin/sh
#
#  Make and run, reloading resources.
#
make
(cd; xrdb -load .xresources)
xrdb -merge XBlackjack.ad
xblackjack
