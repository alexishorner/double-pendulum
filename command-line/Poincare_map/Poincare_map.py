from my_functions import *
import subprocess
from math import cos, sqrt
import tempfile
import time


def get_paths():
    data_path = clean_path_string(input("Enter the path of the data file\n"
                                        ">>> "))
    processed_data_dir = clean_path_string(input("Enter the path of the directory where the processed data will be stored\n"
                                                 ">>> "))
    return data_path, processed_data_dir


def draw_poincare_map(data_path, processed_data_dir):
    THETA_1 = "\u03b8" + "\u2081"
    theta_1_index = None
    THETA_2 = "\u03b8" + "\u2082"
    theta_2_index = None
    OMEGA_1 = "\u03c9" + "\u2081"
    omega_1_index = None
    OMEGA_2 = "\u03c9" + "\u2082"
    omega_2_index = None
    X_1 = "x" + "\u2081"
    x_1_index = None
    Y_1 = "y" + "\u2081"
    y_1_index = None
    X_2 = "x" + "\u2082"
    x_2_index = None
    Y_2 = "y" + "\u2082"
    y_2_index = None
    ENERGY = "Total energy"
    energy_index = None
    energy = None

    number_of_pendula = 0
    processed_data_files = []
    error = False
    try:
        print("\tOpening \"" + data_path + "\"...", end="")
        with open(data_path, "r", encoding="utf-8") as data_file:
            print("\tdone")
            print("\tExtracting data from \"" + data_path + "\"...", end="")
            number_of_pendula = find_number_of_pendula(data_file)

            if len(processed_data_dir) > 0:
                if list(processed_data_dir)[-1] != "\\":
                    processed_data_dir += "\\"
            for i in range(number_of_pendula):
                processed_data_files.append(open(processed_data_dir + "pendulum" + str(i+1) + ".dat", "wb+"))

            prev_theta_2 = [None]*number_of_pendula # creates an empty list of size number_of_pendula
            theta_2 = [None]*number_of_pendula
            for line_number, line in enumerate(data_file):
                parameters = line.split(",")
                if line_number == 0:
                    theta_1_index = parameters.index(THETA_1)
                    theta_2_index = parameters.index(THETA_2)
                    omega_1_index = parameters.index(OMEGA_1)
                    omega_2_index = parameters.index(OMEGA_2)
                    x_1_index = parameters.index(X_1)
                    y_1_index = parameters.index(Y_1)
                    x_2_index = parameters.index(X_2)
                    y_2_index = parameters.index(Y_2)
                    energy_index = parameters.index(ENERGY)
                else:
                    if line_number == 1:
                        energy = parameters[energy_index]

                    pendulum_index = find_pendulum_number(line)-1
                    theta_1 = parameters[theta_1_index]
                    theta_2[pendulum_index] = parameters[theta_2_index]
                    omega_1 = parameters[omega_1_index]
                    omega_2 = parameters[omega_2_index]
                    x_1 = float(parameters[x_1_index])
                    y_1 = float(parameters[y_1_index])
                    x_2 = float(parameters[x_2_index])
                    y_2 = float(parameters[y_2_index])
                    r_1 = sqrt(float(parameters[x_1_index])**2 + float(parameters[y_1_index])**2)
                    r_2 = sqrt((x_2-x_1)**2 + (y_2-y_1)**2)
                    if prev_theta_2[pendulum_index] != None:
                        if changed_sign(float(prev_theta_2[pendulum_index]), float(theta_2[pendulum_index]))\
                        and float(omega_2) + r_1/r_2*float(omega_1)*cos(float(theta_1)) > 0:
                            line_to_write = theta_1 + "," + omega_1 + "\n"
                            processed_data_files[pendulum_index].write(line_to_write.encode("utf-8"))
                    prev_theta_2[pendulum_index] = theta_2[pendulum_index]

            print("\tdone")
        close_files(processed_data_files)
    except IOError:
        print("\nError: the specified file cannot be read. It might not exist.")
        error = True

    if not error:
        colors = generate_colors(number_of_pendula)
        gnuplot = subprocess.Popen(["gnuplot", "--persist"], stdin=subprocess.PIPE)
        gnuplot_commands = ["rgb(r, g, b) = 65536 * int(r*255) + 256 * int(g*255) + int(b*255)",
                            "set datafile separator ','",
                            "set autoscale xy",
                            "set size square",
                            "set xlabel '" + THETA_1 + "'",
                            "set ylabel '" + OMEGA_1 + "'",
                            "set title 'Section de Poincaré du pendule double, E = " + str(round(float(energy))) + "[J]'",
                            "set key off"]
        command = "plot "
        for number, file in enumerate(processed_data_files):
            command += "'" + file.name + "' pt 7 ps 0.15 lc rgbcolor rgb" + str(colors[number])
            if number < len(processed_data_files) - 1:
                command += ",\\"
            gnuplot_commands.append(command)
            command = ""

        for command in gnuplot_commands:
            gnuplot.stdin.write((command + "\n").encode("utf-8"))
            gnuplot.stdin.flush()
        print("\tploting data...", end="")


if __name__ == "__main__":
    data_path, processed_data_dir = get_paths()
    draw_poincare_map(data_path, processed_data_dir)
