'''
Write your solution for 6. PIAT: Check Setup here.

Author:
SID:
Unikey:
'''

import os
import shutil
import sys
import time
import datetime
import pdb


def logging(logs: list, date: str, time: str) -> None:
    '''
    Logging function uses a list of strings to write previous output into a
    log file.
    Parameters:
        logs: list, output from verification/installation in the form of list of 
                    strings to write to logging file.
        date: str,  a string representing the date to generate the necessary 
                    directory date must be in the format YYYY-MM-DD as seen in 
                    the specs (ex: 2023-Mar-03 for March 3rd, 2023).
        time: str,  a string representing the time to generate the log file
                    time must be in the format HH_MM_SS as seen in the specs
                    (ex: 14_31_27 for 14:31:27).
    '''
    if os.path.isdir("/home/logs/"):
        if os.path.isdir(f"/home/logs/{date}/") == False:
            new_directory = os.mkdir(f"/home/logs/{date}/")
    else:
        os.mkdir("/home/logs/")
        new_directory = os.mkdir(f"/home/logs/{date}/")
    new_directory_path = os.path.join(os.getcwd(), f"/home/logs/{date}/")

    filename = (new_directory_path + time + ".txt")

    new_file = open(filename, 'w')

    i = 0
    while i < len(logs):
        current_line = logs[i]
        new_file.write(current_line + "\n")
        i += 1
    new_file.close()
    


def verification(master: str, timestamp: str) -> list:
    '''
    Verification makes sure all files and directories listed in the config file
    are present and match the contents of the master files. 
    Parameters:
        master:    str,  a string representing the absolute path to the master directory.
        timestamp: str,  a string representing the time to insert into the output.
    Returns:
        output:    list, a list of strings generated from the verification process.
    '''
    # 1. Extract absolute paths to directories from given configuration file.
    verification_list = []
    verification_list.append(f"{timestamp} Start verification process.")
    verification_list.append (f"{timestamp} Extracting paths in configuration file.")
    files_path = "/home/files/"
    samples_path = "/home/samples/"
    directories = []
    files = []
    files_files = []
    samples_files = []
    config = open("/home/master/config.txt", 'r')
    while True:
        config_line = config.readline()
        if config_line == '':
            break
        if config_line[0] == "/":
            directories.append(config_line)
            
        elif config_line[0] == ".":
             files.append(config_line)
    config.close()

    verification_list.append(f"Total directories to check: {len(directories)}")

    # 2. Check if directory exists. 
    verification_list.append(f"{timestamp} Checking if directories exists.")
    i = 0
    while i < len(directories): 
        current_directory = (directories[i]).strip()
        if os.path.isdir(current_directory):
            verification_list.append(f"{current_directory} is found!")
        else:
            verification_list.append(f"{current_directory} is not found.")
        i += 1

    # 3. Extract all absolute paths of all files from given configuration file.
    verification_list.append(f"{timestamp} Extracting files in configuration file.")

    files_config_path = directories[0].strip()
    files_master_path = master + "files/"
    samples_master_path = master + "samples/"
    samples_config_path = directories[1].strip()
    sample_files = sorted(os.listdir(samples_master_path))
    files_files = sorted(os.listdir(files_master_path))
    files = files_files + sample_files
    files_in_config = []
    samples_in_config = []

    config = open("/home/master/config.txt", 'r')

    i = 0
    while i < len(files):
        current_file = ("./" + files[i])
        j = True
        while j == True:
            content_line = config.readline().strip()
            if content_line == current_file:
                files_in_config.append(current_file)
                j = False
                break
            if content_line == "/home/samples/":
                a = True
                while a == True:
                    content_line = config.readline().strip() 
                    if content_line == "":
                        break
                    if content_line == current_file:
                        samples_in_config.append(current_file)
                        a = False
                        break
                    j = False
                    break
            if content_line == "":
                break        
        i += 1
    i = 0
    while i < len(files_in_config):
        current_file = (files_in_config[i])[2:]
        verification_list.append(f"File to check: {os.path.join(files_config_path, current_file)}")
        i += 1

    i = 0
    while i < len(samples_in_config):
        current_file = (samples_in_config[i])[2:]
        verification_list.append(f"File to check: {os.path.join(samples_config_path, current_file)}")
        i += 1

    verification_list.append(f"Total files to check: {len(files_in_config) + len(samples_in_config)}")

    # 4. Check if files exists.
    verification_list.append(f"{timestamp} Checking if files exists.")
    i = 0
    while i < len(files_in_config):
        current_file = (files_in_config[i])[2:]
        if os.path.exists(files_config_path + current_file):
            verification_list.append(f"{files_config_path + current_file} found!")
        i += 1

    i = 0
    while i < len(samples_in_config):
        current_file = (samples_in_config[i])[2:]
        if os.path.exists(samples_config_path + current_file):
            verification_list.append(f"{os.path.join(samples_config_path, current_file)} found!")
        i += 1

    # 5. Check contents of each file with those in the master folder.
    config.close()
    verification_list.append(f"{timestamp} Check contents with master copy.")
    i = 0
    while i < len(files_in_config):
        current_file = (files_in_config[i])[2:]
        target = files_config_path + current_file
        src = files_master_path + current_file
        t = open(target, 'r')
        s = open(src, 'r')
        t_line = t.read()
        s_line = s.read()
        if t_line == s_line:
            result = True
            verification_list.append(f"{target} is same as {src}: {result}")
        else:
            result = False
            j = 0
            t_contents = t_line.split('\n')
            s_contents = s_line.split('\n')
            biggest = max(len(t_contents), len(s_contents))
            while j<(biggest):
                line_t = t_contents[j]
                line_s = s_contents[j]
                if line_t == line_s:
                    verification_list.append(f"File name: {target}, {line_t}, {line_s}")
                elif line_t != line_s:
                    verification_list.append(f"File name: {target}, {line_t}, {line_s}")
                    break
                j += 1
            verification_list.append("Abnormalities detected...")
            return verification_list
        
        i += 1
    t.close()
    s.close()

    i = 0
    while i < len(samples_in_config):
        current_file = (samples_in_config[i])[2:]
        target = samples_config_path + current_file
        src = samples_master_path + current_file
        t = open(target, 'r')
        s = open(src, 'r')
        if t.read() == s.read():
            result = True
            verification_list.append(f"{target} is same as {src}: {result}")
        else:
            result = False
            while True:
                line_t = t.readline() 
                line_s = t.readline()
                contents = f"{line_t} {line_s}"
                if line_t == line_s:
                    verification_list.append(f"File name: {target}, {contents}")
                elif line_t != line_s:
                    verification_list.append(contents)
                    break
            verification_list.append("Abnormalities detected...")
        
        i += 1
    t.close()
    s.close()
    if verification_list.count("Abnormalities detected...") == 0:
        verification_list.append(f"{timestamp}  Verification complete.")

    return verification_list



def installation(master: str, timestamp: str) -> list:

    # 1. Extract absolute paths to directories from given configuration file.
    installation_list =  []
    installation_list.append(f"{timestamp} Start installation process.")

    installation_list.append (f"{timestamp} Extracting paths in configuration file.")

    directories = []
    files = []
    config = open("/home/master/config.txt", 'r')
    while True:
        config_line = config.readline()
        if config_line == '':
            break
        if config_line[0] == "/":
             directories.append(config_line)
        elif config_line[0] == ".":
             files.append(config_line)
    config.close()

    installation_list.append(f"Total directories to create: {len(directories)}")

    # 2. Create new directories.
    installation_list.append(f"{timestamp} Create new directories.")

    i = 0

    while i < len(directories): 
        current_directory = directories[i].strip()
        if os.path.isdir(current_directory):
            installation_list.append(f"{current_directory} exists. Skip directory creation.")
        else:
            new_directory = os.mkdir(current_directory)
            new_directory_path = os.path.join(os.getcwd(), current_directory)
            installation_list.append(f"{new_directory_path} is created successfully.")
        i += 1

    # 3. Extract all absolute paths of all files found in the `master` directory.
    installation_list.append(f"{timestamp} Extracting paths of all files in {master}.")
    
    files_config_path = directories[0].strip()
    samples_config_path = directories[1].strip()
    folders_in_master = sorted(next(os.walk(master))[1])
    files_master_path = master + folders_in_master[0] + "/"
    samples_master_path = master + folders_in_master[1] + "/"

    if os.path.isdir(files_master_path) == False:
        os.mkdir(files_master_path)
    if os.path.isdir(samples_master_path) == False:
        os.mkdir(samples_master_path)

    sample_files = sorted(os.listdir(samples_master_path))
    files_files = sorted(os.listdir(files_master_path))
    files_config_master_path = master + directories[0][6:].strip()
    samples_config_master_path = master + directories[1][6:].strip()

    files = files_files + sample_files
    i = 0
    while i < len(files_files):
        current_file = files_files[i]
        installation_list.append(f"Found: {os.path.join(files_master_path, current_file)}")
        i += 1

    i = 0
    while i < len(sample_files):
        current_file = sample_files[i]
        installation_list.append(f"Found: {os.path.join(samples_master_path, current_file)}")
        i += 1

    # 4. Create new files.
    installation_list.append(f"{timestamp}  Create new files.")

    i = 0
    config = open("/home/master/config.txt", 'r')
    files_in_config = []
    samples_in_config = []
    while i < len(files):
        current_file = ("./" + files[i])
        j = True
        while j == True:
            content_line = config.readline().strip()
            if content_line == current_file:
                files_in_config.append(current_file)
                j = False
                break
            if content_line == "/home/samples/":
                a = True
                while a == True:
                    content_line = config.readline().strip() 
                    if content_line == "":
                        break
                    if content_line == current_file:
                        samples_in_config.append(current_file)
                        a = False
                    j = False
                    break
            if content_line == "":
                break        
        i += 1

    config.close()

    i = 0
    while i < len(files_in_config):
        current_file = (files_in_config[i])[2:]
        file_path = files_config_path + current_file
        open(file_path, 'w')
        installation_list.append(f"Creating file: {file_path}")
        i += 1

    i = 0
    while i < len(samples_in_config):
        current_file = (samples_in_config[i])[2:]
        file_path = samples_config_path + current_file
        open(file_path, 'w')
        installation_list.append(f"Creating file: {file_path}")
        i += 1

    # 5. Copy files from `master` directory accordingly.
    installation_list.append(f"{timestamp} Copying files.")
    i = 0

    while i < len(files_in_config):
        current_file = (files_in_config[i])[2:]
        original_path = files_config_master_path + current_file
        destination_path = files_config_path + current_file
        installation_list.append(f"Locating: {current_file}")
        
        try:
            shutil.copy(original_path, destination_path)
        except FileNotFoundError:
            installation_list.append(f"Original path: {original_path} is not found.")
            installation_list.append(f"Installation error...")
            return installation_list
        
        installation_list.append(f"Original path: {original_path}")
        installation_list.append(f"Destination path: {destination_path}")
        i+=1

    i = 0
    while i < len(samples_in_config):
        current_file = (samples_in_config[i])[2:]
        original_path = samples_config_master_path + current_file
        destination_path = samples_config_path + current_file
        installation_list.append(f"Locating: {current_file}")
        
        try:
            shutil.copy(original_path, destination_path)
        except FileNotFoundError:
            installation_list.append(f"Original path: {original_path} is not found.")
            installation_list.append(f"Installation error...")
            return installation_list
        
        installation_list.append(f"Original path: {original_path}")
        installation_list.append(f"Destination path: {destination_path}")

        i+=1
    if installation_list.count("Installation error...") == 0:
        installation_list.append(f"{timestamp}  Installation complete.")

    return installation_list
    


def main(master: str, flag: str, timestamp: str):
    '''
    Ideally, all your print statements would be in this function. However, this is
    not a requirement.
    Parameters:
        master:    str, a string representing the absolute path to the master directory.
        flags:     str, a string representing the specified flags, if no flag is given
                        through the command line, flags will be an empty string.
        timestamp: str, a string representing the time to insert into the output.
                    in the format: DD MMM YYYY HH:MM:DD , ex: 10 Apr 2023 12:44:17
    '''
    if flag[0] != "-":
        sys.stderr.write("Invalid flag. Flag must start with '-'.\n")
        exit()
    elif flag == "-l":
        sys.stderr.write("Invalid flag. Log can only run with install or verify.\n")
        exit()
    elif flag == "-vi" or flag == "-iv":
        sys.stderr.write("Invalid flag. Choose verify or install process not both.\n")
        exit()
    elif flag == "-vv" or flag == "-ii":
        sys.stderr.write("Invalid flag. Each character must be unique.\n")
        exit()
    elif flag[1] != "i" and flag[1] != "v" and flag[1] != "l":
        sys.stderr.write("Invalid flag. Character must be a combination of 'v' or 'i' and 'l'.\n")
        exit()
    if len(flag) > 2:
        if flag[2] != "i" and flag[2] != "v" and flag[2] != "l":
            sys.stderr.write("Invalid flag. Character must be a combination of 'v' or 'i' and 'l'.\n")
            exit()
    log_date = (f"{timestamp[7:11]}-{timestamp[3:6]}-{timestamp[0:2]}")
    log_time = (f"{timestamp[12:14]}_{timestamp[15:17]}_{timestamp[18:20]}")

    if flag == "-i":
        i = 0
        installation_list = installation(master, timestamp)
        while i < len(installation_list):
            sys.stderr.write(installation_list[i] + '\n')
            i += 1
    elif flag == "-v":
        i = 0
        verification_list = verification(master, timestamp)
        while i < len(verification_list):
            sys.stderr.write(verification_list[i] + '\n')
            i += 1
    elif flag == "-il" or flag == "-li":
        i = 0
        installation_list = installation(master, timestamp)
        while i < len(installation_list):
            sys.stderr.write(installation_list[i] + '\n')
            i += 1
        logging(installation_list, log_date, log_time)
    elif flag == "-vl" or "-lv":
        i = 0
        verification_list = verification(master, timestamp)
        while i < len(verification_list):
            sys.stderr.write(verification_list[i] + '\n')
            i += 1
        logging(verification_list, log_date, log_time)



if __name__ == "__main__":
    #you will need to pass in some arguments here
    # we will leave this empty for you to handle the implementation
    split_time = (time.asctime()).split(' ')
    date = split_time[2]
    month = split_time[1]
    current_time = split_time[3]
    year = split_time[4]
    timestamp = f"{date} {month} {year} {current_time}"

    try:
        flag = sys.argv[2]
        master = sys.argv[1]
    except IndexError:
        sys.stderr.write("Insufficient arguments.")
        exit()

    if os.path.isdir(master) == False:
        sys.stderr.write("Invalid master directory.\n")
        exit()
    main(master, flag, timestamp)

