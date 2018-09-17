#!/usr/bin/env python3

## Creates a gamma-corrected lookup table
import math

def gamma(nsteps, gamma):
    gammaedUp = [math.pow(x, gamma) for x in range(nsteps)]
    return [x/max(gammaedUp) for x in gammaedUp]

def rounder(topValue, gammas):
    return [min(topValue, round(x*topValue)) for x in gammas]

if __name__ == "__main__":
    myGammaR = 2.2
    myGammaG = 2.2
    myGammaB = 2.2
    steps = 256
    output = open("gamma.h", "w")
    output.write("/* %d-step brightness table: gamma = %s */ \n\n" % (steps, myGammaR))
    output.write("const uint8_t gammaR[%d] = {\n" % steps)
    for value in rounder(255, gamma(steps, myGammaR)):
        output.write("\t %d,\n" % value)
    output.write("};\n")

    output.write("/* %d-step brightness table: gamma = %s */ \n\n" % (steps, myGammaG))
    output.write("const uint8_t gammaG[%d] = {\n" % steps)
    for value in rounder(255, gamma(steps, myGammaG)):
        output.write("\t %d,\n" % value)
    output.write("};\n")

    output.write("/* %d-step brightness table: gamma = %s */ \n\n" % (steps, myGammaB))
    output.write("const uint8_t gammaB[%d] = {\n" % steps)
    for value in rounder(255, gamma(steps, myGammaB)):
        output.write("\t %d,\n" % value)
    output.write("};\n")
    output.close()

