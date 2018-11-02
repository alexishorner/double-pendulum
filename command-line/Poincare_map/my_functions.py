import colorsys
import os

def clean_path_string(string):
    if len(string) > 0:
        string = list(string)
        while string[-1] == " ":
            string.pop()
        if string[0] == "\"" and string[-1] == "\"" or string[0] == "\'" and string[-1] == "\'":
            string[0] = ""
            string[-1] = ""
        return "".join(string)
    else:
        return string


def find_pendulum_number(string):
    pendulum_number = ""
    pendulum = "\"pendulum "
    char_list = list(string)
    pos = -1
    for i in range(len(char_list)):
        tmp_string = ""
        for j in range(len(pendulum)):
            tmp_string += char_list[i+j]
        if tmp_string == pendulum:
            pos = i + len(pendulum)
            break
    if pos != -1:
        current_char = string[pos]
        while current_char.isdigit():
            pendulum_number += current_char
            pos += 1
            current_char = string[pos]
    return int(pendulum_number)


def find_number_of_pendula(file):
    pos = file.tell()
    file.seek(0)
    number_of_pendula = 0
    for line_number, line in enumerate(file):
        if line_number != 0:
            tmp_number = find_pendulum_number(line)
            if tmp_number > number_of_pendula:
                number_of_pendula = tmp_number
            else:
                file.seek(pos)
                return number_of_pendula
    file.seek(pos)
    return number_of_pendula


def changed_sign(prev_value, value):
    return prev_value < 0 and value >= 0 or prev_value >= 0 and value < 0


def close_files(files):
    for number, file in enumerate(files):
        print("\r\tclosing files...\t(" + str(number+1) + "/" + str(len(files)) + ")", end="")
        file.close()
    print("")


def generate_colors(number_of_colors):
    colors = []
    for i in range(number_of_colors):
        hue = 1.0/number_of_colors*i
        saturation = 1.0
        lightness = 0.35
        colors.append(colorsys.hls_to_rgb(hue, lightness, saturation))
    return colors
