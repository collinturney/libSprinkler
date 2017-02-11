#!/usr/bin/env python

from ctypes import cdll


class SprinklerError(Exception):
    pass


class Sprinkler(object):

    def __init__(self, libpath='./libSprinkler.so'):
        _oslib = cdll.LoadLibrary(libpath)

        self._oslib_init = _oslib.init
        self._oslib_set = _oslib.set
        self._oslib_reset = _oslib.reset
        self._oslib_reset_all = _oslib.resetAll
        self._oslib_is_on = _oslib.isOn
        self._oslib_is_off = _oslib.isOff

        if self._oslib_init() != 0:
            raise SprinklerError("Failed to initialize wiringPi!")

        self._init_called = True

    def __del__(self):
        if self._init_called:
            self._oslib_reset_all()

    def on(self, station=0):
        if not self._init_called:
            raise SprinklerError("wiringPi not initialized!")

        return self._oslib_set(station)

    def off(self, station=0):
        if not self._init_called:
            raise SprinklerError("wiringPi not initialized!")

        return self._oslib_reset(station)

    def all_off(self):
        if not self._init_called:
            raise SprinklerError("wiringPi not initialized!")

        return self._oslib_reset_all()

    def is_on(self, station=0):
        if not self._init_called:
            raise SprinklerError("wiringPi not initialized!")

        return self._oslib_is_on(station) == 1

    def is_off(self, station=0):
        if not self._init_called:
            raise SprinklerError("wiringPi not initialized!")

        return self._oslib_is_off(station) == 1
