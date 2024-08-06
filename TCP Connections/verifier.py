"""
Write code for your verifier here.

You may import library modules allowed by the specs, as well as your own other modules.
"""
from sys import argv

from pathlib import Path


def main(args: list[str]) -> None:
    master_path = args[0]
    directory_path = args[1]

    # checking if master config is valid
    master_result = is_valid_config(master_path)
    if master_result[0] == False:
        print("invalid master")
        exit()

    
    # getting data from master file
    f = open(master_path)
    contents = f.readlines()
    master_second_port = int(contents[0].strip())
    master_domain = (contents[1].split(","))[0]
    master_port = int((contents[1].split(","))[1])
    f.close()

    # checking directory exists and can be written to
    directory = Path(directory_path)
    if not(directory.exists() and directory.is_dir()):
        print("singles io error")
        exit()
    
    directory_of_files = [file for file in directory.iterdir() if file.is_file()]


    for single_file in directory_of_files:
        f = open(single_file)
        contents = f.readlines()
        if is_valid_config(contents)[0] == False:
            print("invalid single")
            exit()

    # getting data of single files
    # files have already been checked and are valid, so just collating info now
    directory_of_files = [file for file in directory.iterdir() if file.is_file()]
    if is_valid_single_files(directory) == False:
        print("neq")
    else:
        print("eq")




  
# checking if file formats are valid
def is_valid_config(filepath) -> tuple:
    try:
        f = open(master_path)
    except FileNotFoundError:
        return (False, "")

    contents = f.readlines()
    try:
        port = int(contents[0].strip())
    except (TypeError, ValueError) as e:
        return (False, (""))
    list = []

    if not(port >= 1024 and port <= 65535):
        f.close()
        f.close()
        return (False, "")
    
    domains = contents[1:]

    for domain in domains:
        port2 = domain.strip().split(',')[1]
        domain = domain.strip().split(',')[0]
        if not(port2.isnumeric() and (port >= 1024 and port <= 65535)):
            f.close()
            return (False, (""))

        if not(domain.replace('-', '').replace('.', '').isalnum()):
            return (False, (""))
        if '.' in domain:
            parts = domain.split('.')
            if len(parts) == 3:
                C, B, A = parts
            elif len(parts) == 2:
                B, A = parts
                f.close()
                return (False, (""))
            elif len(parts) == 1:
                A = parts
                f.close()
                return (False, (""))
            else:
                A = parts[-1]
                B = parts[-2]
                C = '.'.join(parts[:-2])
            
            # checking if a and b are alphanumeric + hyphen
            if not(A.replace('-', '').isalnum() or B.replace('-', '').isalnum()):
                f.close()
                return (False, (""))
            elif len(parts) >= 3 and  (C.replace('.', '').replace('-', '').isalnum()):
                if(C[0] != '.' and C[-1] != '.'):
                    f.close()
                    list.append((A,f"{B}.{A}",f"{C}.{B}.{A}", port2))
            else: 
                f.close()
                return (False, (""))
                

        else:
            f.close()
            return (False, (""))
    # print("A: " + A)
    # print("B: " + B)
    # print("C: " + C)
    return(True, (list))

# CHECKING SINGLE FILES ARE VALID

def is_valid_single_files(directory):

    list_of_port_numbers = []
    dictionary_of_files = {}
    sorted_dictionary = {}

    directory_of_files = [file for file in directory.iterdir() if file.is_file()]

    # initial dictionary
    root_dictionary = {}
    tld_dictionary = {}
    auth_dictionary = {}

    for single_file in directory_of_files:
        f = open(single_file)
        contents = f.readlines()
        single_file = {}
        first_port = int(contents[0].strip())

        for record in contents[1:]:
            if record.count(".") == 0: # add to root
                record_split = record.strip().split(',')
                root_dictionary[(record_split[0], record_split[1])] = first_port
                
            elif record.count(".") == 1: # add to tld
                record_split = record.strip().split(',')
                tld_dictionary[(record_split[0], record_split[1])] = first_port
            
            elif record.count(".") >= 2: # add to auth
                record_split = record.strip().split(',')
                auth_dictionary[(record_split[0], record_split[1])] = first_port

    for key in root_dictionary:
        domain = key[0]
        domain_port = int(key[1].strip())
        for key in tld_dictionary:
            value = tld_dictionary[key]
            if domain in key[0]:
                if domain_port != value:
                    return False

    
    for key in tld_dictionary:
        domain = key[0]
        domain_port = int(key[1].strip())

        for key in auth_dictionary:
            value = auth_dictionary[key]
            if domain in key[0]:
                if domain_port != value:
                    return False

    f = open(master_path)
    contents = f.readlines()
        
    hostname_list = []
    for hostname in contents[1:]:
        hostname_list.append(hostname.strip())

    for key in auth_dictionary:
        domain_port = int(key[1].strip())
        domain = f"{key[0]},{domain_port}"
        if domain not in hostname_list:
            return False
    
    return True


if __name__ == "__main__":
    try:
        master_path = argv[1]
        directory_path = argv[2]
    except IndexError:
        print("invalid arguments")
        exit()
    try:
        arg3 = argv[3]
    except IndexError:
        arg3 = None
    if arg3 != None:
        print("invalid arguments")
        exit()
    main(argv[1:])