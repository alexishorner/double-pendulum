import math
import random
import subprocess
import tempfile
import os
import sys
sys.path.append("../Poincare_map")
from Poincare_map import *

def P(ang1, ang2, radius, mass, g):
    return (3 - 2*math.cos(ang1) - math.cos(ang2))*mass*g*radius


def K(ang1, ang2, angVel1, angVel2, radius, mass):
    return 1.0/2.0*mass*radius**2*(2*angVel1**2 + angVel2**2 + 2*angVel1*angVel2*math.cos(ang1-ang2))


def totE(ang1, ang2, angVel1, angVel2, radius, mass, g):
    return P(ang1, ang2, radius, mass, g) + K(ang1, ang2, angVel1, angVel2, radius, mass)


def maxAng1(radius, mass, g, Energy):
    a = 1-Energy/(2*mass*g*radius)
    if a >= -1 and a <= 1:
        return math.acos(a)
    else:
        return math.pi


def minAng1(radius, mass, g, Energy):
    if ((3-Energy/(mass*g*radius))/2.0) <= 1:
        return math.acos((3-Energy/(mass*g*radius))/2.0)
    else:
        return 0


def maxAng2(ang1, radius, mass, g, Energy):
    a = 3-Energy/(mass*radius*g)-2*math.cos(ang1)
    if a >= -1 and a <= 1:
        return math.acos(a)
    else:
        return math.pi


def maxAngVel1(ang1, ang2, radius, mass, g, Energy):
    vel = math.sqrt((Energy - P(ang1, ang2, radius, mass, g))/(mass*radius**2))
    return vel, -vel


def angVel2(ang1, ang2, angVel1, radius, mass, g, Energy):
    a = 1.0/2.0*mass*radius**2
    b = mass*radius**2*angVel1*math.cos(ang1-ang2)
    c = mass*radius**2*angVel1**2 + mass*g*radius*(3 - 2*math.cos(ang1) - math.cos(ang2)) - Energy
    sqrtDelta = math.sqrt(b**2 - 4*a*c)
    return(-b + sqrtDelta)/(2.0*a), (-b - sqrtDelta)/(2.0*a)


def generatePendula(radius, mass, g, Energy, number=20):
    parametersList = []
    for i in range(number):
        parameters = []
        parameters.append("pendulum " + str(i+1))
        parameters.append((2*random.random()-1)*maxAng1(radius, mass, g, Energy))
        parameters.append((2*random.random()-1)*maxAng2(parameters[1], radius, mass, g, Energy))
        parameters.append((2*random.random()-1)*maxAngVel1(parameters[1], parameters[2], radius, mass, g, Energy)[round(random.random())])
        parameters.append(angVel2(parameters[1], parameters[2], parameters[3], radius, mass, g, Energy)[round(random.random())])
        parameters.append(radius)
        parameters.append(radius)
        parameters.append(mass)
        parameters.append(mass)
        parameters.append(g)
        parametersList.append(parameters)
    return parametersList


def toString(list):
    for i in range(len(list)):
        list[i] = str(list[i])
    return list

def isNumber(string):
    try:
        float(string)
        return True
    except:
        return False


def getParametersFromUser():
    parameters = []
    inp = input("Please enter the following parameters separated by a comma and a space each in the correct order:\n"
                "samples per second, dt, time, number of pendula, radius, mass, g, Energy\n"
                ">>> ")
    inp = inp.split(", ")
    for item in inp:
        parameters.extend(item.split(","))

    if len(parameters) != 8:
        print("Error: 8 parameters expected (got " + str(len(parameters)) +")")
        return parameters, False

    for number, item in enumerate(parameters):
        if not isNumber(item):
            print("Error: parameter " + str(number+1) + ": number expected (got string)")
            return parameters, False
    return parameters, True


def interact():
    currentDir = os.path.dirname(os.path.realpath(__file__))
    simulationRelativePath = "../simulation_command-line/simulation_command-line.exe"

    dataPath, newPath = get_paths()

    passedParameters = []
    passedParameters.append(dataPath)
    parameters, success = getParametersFromUser()
    if success:
        passedParameters.append(parameters[0])
        passedParameters.append(parameters[1])
        passedParameters.append(parameters[2])
        numberOfPendula = int(parameters[3])
        radius = float(parameters[4])
        mass = float(parameters[5])
        g = float(parameters[6])
        Energy = float(parameters[7])
        pendula = generatePendula(radius, mass, g, Energy, numberOfPendula)

        for pendulum in pendula:
            passedParameters.extend(pendulum)
        passedParameters = toString(passedParameters)


        args = [currentDir + "/" + simulationRelativePath]
        file = tempfile.NamedTemporaryFile(delete=False)
        try:
            args.append(file.name)

            for number, argument in enumerate(passedParameters):
                file.write(argument.encode("utf-8"))
                if number < len(passedParameters)-1:
                    file.write(",".encode("utf-8"))
            file.close()

            simulation = subprocess.Popen(args).wait()
        except IOError:
            print("An error occurred when writing to a temporary file")
        finally:
            if not file.closed:
                file.close()
            os.unlink(file.name)

        draw_poincare_map(dataPath, newPath)

if __name__ == "__main__":
    interact()
