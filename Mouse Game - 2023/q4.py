'''
Answer for Question 4 - The Training

Name:
SID:
unikey:

'''

# Phase 1
def intro():
    print('''Larry: I'm Larry. I'll be your hunting instructor.
Larry: Let's go to the Meadow to begin your training!''')
    '''
    Prints the introduction by Larry.
    Only line to print:     "Larry: I'm Larry. I'll be your hunting instructor."
    '''

def travel_to_camp():
    travel = input('Press Enter to travel to the Meadow...')
    if travel == "\x1b":
        exit()
    print('''Travelling to the Meadow...
Larry: This is your camp. Here you'll set up your mouse trap.''')
    '''
    Prints the game conversation of travelling and reaching the camp.
    First line to print:    "Larry: Let's go to the Meadow to begin your training!"
    Last line to print:     "Larry: This is your camp. Here you'll set up your mouse trap."
    Input from the user should be taken in once in this function.
    '''


#Phase 2
def setup_trap() -> int:
    print("Larry: Let's get your first trap...")
    view_traps = input("Press Enter to view traps that Larry is holding...")
    if view_traps == "\x1b":
        exit()
    print('''Larry is holding...
Left: High Strain Steel Trap
Right: Hot Tub Trap''')
    choose_trap = (input('Select a trap by typing "left" or "right": ')).lower().strip()
    if choose_trap == "\x1b":
        exit()
    if choose_trap == 'left':
        trap = "High Strain Steel Trap"
    elif choose_trap == 'right':
        trap = "Hot Tub Trap"
    else:
        trap = "Cardboad and Hook Trap"
    if choose_trap == 'right' or choose_trap == 'left':
        cheddar = 1
        print(f'''Larry: Excellent choice.
Your "{trap}" is now set!
Larry: You need cheese to attract a mouse.
Larry places one cheddar on the trap!''')
    else:
        print('''Invalid command! No trap selected.
Larry: Odds are slim with no trap!''')
        cheddar = 0
        trap = "Cardboard and Hook Trap"
    return (trap, cheddar)
    
    '''
    Prints the game conversation of getting your first trap and setting it.
    First line to print:    "Larry: Let's get your first trap..."
    Last line to print:     Either "Larry places one cheddar on the trap!", or
                            "Larry: Odds are slim with no trap!" depending on if you 
                            chose a trap or not. 
    Input from the user should be taken in twice this function.
    Returns:
        A tuple containing 2 elements:
        1. trap,            str
            - If a trap was chosen, the value is the name of the trap
              e.g. 'High Strain Steel Trap'
            - If no trap was chosen, the value is 'Cardboard and Hook Trap'
              (this will be useful in later questions)
        2. cheddar,         int
            - If a cheese was placed, value is 1
            - If no cheese was placed, value is 0
    '''

#Phase 3
def sound_horn() -> str:
    print('''Sound the horn to call for the mouse...''')
    horn_input = (input('Sound the horn by typing "yes": ')).lower().strip()
    if horn_input == "\x1b":
        exit()
        
    return horn_input
    '''
    Prints the game conversation to sound horn
    First line to print:    "Sound the horn to call for the mouse..."
    Last line to print:     'Sound the horn by typing "yes": '
    Input from the user should be taken in once this function.
    Returns:
        horn input:     str, the input entered by user for sounding horn
        e.g. 'yes'
        e.g. 'asdhasjkhdsa'
    '''

def basic_hunt(cheddar: int, horn_input: str) -> bool:
    if horn_input == "yes" and cheddar == 1:
        hunt_status = True
        print('''Caught a Brown mouse!
Congratulations. Ye have completed the training.''')
    elif horn_input == "yes" or cheddar == 1:
        hunt_status = False
        print('''Nothing happens.
To catch a mouse, you need both trap and cheese!''')
    else:
        hunt_status = False
        print("Nothing happens.")
    return hunt_status
    '''
    Prints the hunt and Larry's feedback of hunt.
    The outcome of hunt is determined by the number of cheddar and horn input.
    First line to print:    Varies depending on the hunt. Could be "Caught a Brown
                            Mouse!" or "Nothing happens." or "To catch a mouse, you
                            need both trap and cheese!"
    Last line to print:     Varies depending on the hunt like above.
    Parameters:
        cheddar:        int, the number of cheddar
        horn_input:     str, the input entered by user for sounding horn
    Returns:
        hunt status:    bool, whether the hunt succeeded or not
    '''

def end(hunt_status: bool):
    if hunt_status == True:
        print('Good luck~')
    else:
        pass
    '''
    Prints the 'Good luck~' message if hunt was successful
    Parameters:
        hunt_status:    bool, whether the hunt succeeded or not
    '''

def main():
    intro()
    travel_to_camp()
    trap, cheddar = setup_trap()
    horn_input = sound_horn()
    hunt_status = basic_hunt(cheddar, horn_input)
    end(hunt_status)
    return trap
    '''
    Call your functions here.
    Apart from good design, this is so if you import this file in train.py 
    (question 5), it will not run this code. Because this code's __name__ 
    will not be '__main__', but it will instead be 'q4', allowing you to
    import this file to use your functions without running unwanted calling code.
    ''' 

'''
This statement is true if you run this script.
This statement is false if this file is to be imported from another script. 
'''
if __name__ == '__main__':
    main()
