"""
Write code for your launcher here.

You may import library modules allowed by the specs, as well as your own other modules.
"""
from sys import argv

from pathlib import Path

import random


def main(args: list[str]) -> None:
    master_path = args[0]
    directory = args[1]

    result = is_valid_config(master_path)
    if result[0] == False:
        print("INVALID MASTER")
        exit()
    
    else:
        f = open(master_path)
        contents = f.readlines()
        master_port = int(contents[0].strip())
        f.close()
    

    directory = Path(directory)
    if not(directory.exists() and directory.is_dir()):
        print("NON-WRITABLE SINGLE DIR")
        exit()

    i = 0
    root_created = {}
    tld_created = {}
    auth_created = {}
    for record in result[1]:
        i += 1
        A_domain = record[0]
        B_domain = record[1]
        C_domain = record[2]
        port = int(record[3])

        if not root_created:
            A = open(f"{directory}/root.txt", 'w')
            A.write(f"{master_port}\n")
            random_number = random.sample(range(1024, 65535 + 1), 1)[0]
            # print(f"root: {random_number}, write")
            A.write(f"{A_domain},{random_number}")
            A.close()
            root_created[A_domain] = random_number

        elif not(A_domain in root_created):
            A = open(f"{directory}/root.txt", 'a')
            random_number = random.sample(range(1024, 65535 + 1), 1)[0]
            # print(f"root: {random_number}")
            A.write(f"\n{A_domain},{random_number}")
            A.close()
            root_created[A_domain] = random_number
        else:
            random_number = root_created[A_domain]


        if Path(f"{directory}/{A_domain}.tld.txt").exists():
            B = open(f"{directory}/{A_domain}.tld.txt", 'a')
        else:
            B = open(f"{directory}/{A_domain}.tld.txt", 'w')
            B.write(f"{random_number}")
        if not(B_domain in tld_created):
            random_number2 = random.sample(range(1024, 65535 + 1), 1)[0]
            B.write(f"\n{B_domain},{random_number2}")
            B.close()
            tld_created[B_domain] = random_number2
        else:
            random_number2 = tld_created[B_domain]
            B.close()


        if Path(f"{directory}/{B_domain}.auth.txt").exists():
            C = open(f"{directory}/{B_domain}.auth.txt", 'a')
        else:
            C = open(f"{directory}/{B_domain}.auth.txt", 'w')
            C.write(f"{random_number2}")
        C.write(f"\n{C_domain},{port}")
        C.close()
        auth_created[C_domain] = port

    

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
            return (False, "")

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


if __name__ == "__main__":
    try:
        master_path = argv[1]
        directory = argv[2]
    except IndexError:
        print("INVALID ARGUMENTS")
        exit()
    try:
        arg3 = argv[3]
    except IndexError:
        arg3 = None
    if arg3 != None:
        print("INVALID ARGUMENTS")
        exit()
    main(argv[1:])