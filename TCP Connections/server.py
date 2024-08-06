from sys import argv
import socket

def main(args: list[str]) -> None:
    dictionary = {}
    # try open config file
    try:
        f = open(args[0])
        lines = f.readlines()
    except FileNotFoundError:
        print("INVALID CONFIGURATION")
        exit()
    
    f.close()

    # check if config structure is valid
    if not(is_valid_config(args[0])[0]):
        print("INVALID CONFIGURATION")
        exit()

    for line in lines:
        split_line = line.split(',')
        if len(split_line) > 1:
            dictionary[split_line[0]] = split_line[1]

    server_port = int(lines[0].strip())

    if not(server_port >= 1024 and server_port <= 65535):
        print("INVALID CONFIGURATION")
        exit()

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind(('localhost', server_port))
    server_socket.listen(5)
    
    while True:
        client_socket, client_address = server_socket.accept()

        while True:
            data = client_socket.recv(1024).decode('utf-8')

            if data.strip()[:4] == "!ADD":
                hostname = data.strip().split()[1]
                port = f"{data.strip().split()[2]}\n"
                dictionary[hostname] = port
            
            elif data.strip()[:4] == "!DEL":
                hostname = data.strip().split()[1]
                dictionary.pop(hostname, None)
            
            elif (is_valid(data.strip())[0] == True):
                response = dictionary.get(data.strip())

                if response == None:
                    response = "NXDOMAIN\n"
                print(f"resolve {data.strip()} to {response.strip()}", flush=True)
                response = str(response).encode('utf-8')
                client_socket.send(response)

            elif not data or data.strip() == "!EXIT":
                client_socket.close()
                break

            else:
                print("INVALID CONFIGURATION")
    

def is_valid_config(filepath) -> tuple:
    try:
        f = open(filepath)
        contents = f.readlines()
    except FileNotFoundError:
        return (False, "")


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

        if is_valid(domain)[0] == False:
            f.close()
            return (False, "")

    return(True, (list))

    
def is_valid(domain) -> tuple:
    if ' ' in domain:
        return (False, "")
    if '.' in domain:
        parts = domain.split('.')
        if len(parts) == 3:
            C, B, A = parts
        elif len(parts) == 2:
            B, A = parts
        elif len(parts) == 1:
            A = parts
        else:
            A = parts[-1]
            B = parts[-2]
            C = '.'.join(parts[:-2])
        
        # checking if a and b are alphanumeric + hyphen
        if not(A.replace('-', '').isalnum() or B.replace('-', '').isalnum()):
            return (False, '')
        elif(len(parts) == 3 and  (C.replace('.', '').replace('-', '').isalnum())):
            if(C[0] != '.' and C[-1] != '.'):
                return (True, "hostname")
        else:
            return (True, 'partial')

    elif (domain.replace('-', '').isalnum()):
        return (True, "partial")
    else:
        return (False, '')
    

if __name__ == "__main__":
    try:
        config = argv[1]
    except IndexError:
        print("INVALID ARGUMENTS")
        exit()
    try:
        arg2 = argv[2]
    except IndexError:
        arg2 = None
    if arg2 != None:
        print("INVALID ARGUMENTS")
        exit()


    main(argv[1:])