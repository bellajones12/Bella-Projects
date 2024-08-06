'''
Write your answer for the full OO version of the game here.

Author:
SID:
Unikey:
'''

import game_final
from interface import Interface
interface1 = Interface()

def main():
    '''
    Implement your code here.
    '''
    # pdb.set_trace()
    print(game_final.run_setup())
    name = interface1.player.set_name(game_final.hunters_name())
    trap = interface1.trap.set_trap_name(game_final.training(name))
    print("How can I help ye?")
    print(interface1.get_menu())
    choice = input()    
    interface1.move_to(choice)


if __name__ == '__main__':
    main()

