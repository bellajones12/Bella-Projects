"""
Write code for your recursor here.

You may import library modules allowed by the specs, as well as your own other modules.
"""
from sys import argv
import socket
import signal


def main(args: list[str]) -> None:
    port = int(args[0])
    time_out = float(args[1])
    time_out = int(time_out)

    signal.signal(signal.SIGALRM, timeout_handler)
    signal.alarm(time_out)

    while True:
        try:
            root_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            root_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            root_sock.settimeout(time_out)
            root_sock.connect(("", port))
        except TimeoutError:
            print("root timeout", flush=True)
            print("NXDOMAIN", flush=True)
            root_sock.close()
            continue
        except (socket.error):
            print("FAILED TO CONNECT TO ROOT", flush=True)
            exit()
        # user inputs domain
        try:
            domain = input()
        except EOFError:
            exit()
        while (is_valid(domain)[0] == False):
            try:
                print("INVALID", flush=True)
                domain = input()
            except EOFError:
                exit()
        
        A = is_valid(domain)[1][0]
        B = is_valid(domain)[1][1]

        # queries DNS root nameserver
        try:
            # print("root: " + A, flush=True)
            query_bytes = (A+"\n").encode('utf-8')
            root_sock.send(query_bytes)
            port1 = root_sock.recv(512).decode('utf-8')
            # print("port1: " + port1, flush=True)
            if port1.strip() == "NXDOMAIN":
                root_sock.close()
                print("NXDOMAIN", flush=True)
                continue
            root_sock.close()
        except TimeoutError:
            print("tld timeout", flush=True)
            print("NXDOMAIN", flush=True)
            root_sock.close()
            continue


        # second query

        try:
            tld_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            tld_sock.connect(("", int(port1)))
            # print(f"tld: {B}.{A}", flush=True)
            query_bytes = ((B+"."+A)+"\n").encode('utf-8')
            tld_sock.send(query_bytes)
            port2 = tld_sock.recv(512).decode('utf-8')
            # print("port2: " + port2, flush=True)
            if port2.strip() == "NXDOMAIN":
                tld_sock.close()
                print("NXDOMAIN", flush=True)
                continue
            tld_sock.close()
        except TimeoutError:
            print("auth timeout", flush=True)
            print("NXDOMAIN", flush=True)
            tld_sock.close()
            continue
        except socket.error:
            print("FAILED TO CONNECT TO TLD", flush=True)
            tld_sock.close()
            exit()


        try:
            auth_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            auth_sock.connect(("", int(port2)))
            query_bytes = (domain+"\n").encode('utf-8')
            # print("auth: " + domain, flush=True)
            auth_sock.send(query_bytes)
            port3 = auth_sock.recv(512).decode('utf-8').strip()
            print(port3, flush=True)
        except TimeoutError:
            print("NXDOMAIN", flush=True)
            auth_sock.close()
            continue
        except socket.error:
            print("FAILED TO CONNECT TO AUTH", flush=True)
            auth_sock.close()
            exit()
        
    


def is_valid(domain) -> tuple:
    if domain.strip() != domain:
        return (False, (""))

    if '.' in domain:
        parts = domain.split('.')
        if len(parts) == 3:
            C, B, A = parts
        elif len(parts) == 2:
            B, A = parts
            return (False, (""))
        elif len(parts) == 1:
            A = parts
            return (False, (""))
        else:
            A = parts[-1]
            B = parts[-2]
            C = '.'.join(parts[:-2])
        
        # checking if a and b are alphanumeric + hyphen
        if not(A.replace('-', '').isalnum() or B.replace('-', '').isalnum()):
            return (False, (""))
        elif len(parts) >= 3 and  (C.replace('.', '').replace('-', '').isalnum()):
            if(C[0] != '.' and C[-1] != '.'):
                return (True, (A, B, C))
        else: 
            return (False, (""))
            

    elif (domain.replace('-', '').isalnum()):
        return (False, (""))
    else:
        return (False, (""))

def timeout_handler(signum, frame):
    raise TimeoutError

if __name__ == "__main__":
    try:
        port = argv[1]
        time_out = argv[2]
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
    try:
        port = int(port)
        time_out = float(time_out)
    except TypeError:
        print("INVALID ARGUMENTS")
        exit()
    if not(port >= 1024 and port <= 65535):
        print("INVALID ARGUMENTS")
        exit()
    main(argv[1:])