'''
This file should borrow code from your Assignment 1.
However, it will require some modifications for this assignment.

Author:
SID:
Unikey:
'''

'''
Keep this line!
'''
import random

'''
We recommend you import your 'name', 'train' and 'shop' modules to complete this 
question. It will save trouble in needing to copy and paste code from previous 
questions. However if you wish not to, you are free to remove the imports below.
Feel free to import other modules that you have written.
'''
import name
import train
import shop
import q4

# you can make more functions or global read-only variables here if you please!

#initialising global variables
gold = 125
points = 0
cheese = [["Cheddar", 0], ["Marble", 0], ["Swiss", 0]]


# defining game menu
def get_game_menu():
    '''
    Returns a string displaying all possible actions at the game menu.
    '''
    game_actions = (f'''1. Exit game
2. Join the Hunt
3. The Cheese Shop
4. Change Cheese''')
    return game_actions

def game_menu(trap, gold, points, cheese, name, game_actions):
    print(game_actions)
    choice = input()
    trap_cheese = None
    while choice != '1':
        if choice == "2":
            gold, points = hunt(gold, cheese, trap_cheese, points)
        if choice == "3":
            gold, cheese = shop.welcome(gold, cheese, trap)
        if choice == "4":
            e_flag = False
            trap_status, trap_cheese = change_cheese(name, trap, cheese)
        choice = input(f'''\nWhat do ye want to do now, Hunter {name}?
1. Exit game
2. Join the Hunt
3. The Cheese Shop
4. Change Cheese
''')
    if choice == "1":
        exit

# change cheese to arm trap
def change_cheese(name: str, trap: str, cheese: list) -> tuple:
    '''
    Handles the inputs and ouputs of the change cheese feature.
    Parameters:
        name:        str,        the name of the player.
        trap:        str,        the trap name.
        cheese:      list,       all the cheese and its quantities the player 
                                 currently possesses.
        e_flag:      bool,       if the trap is enchanted, this will be True. 
                                 default value is False.
    Returns:
        trap_status: str,        True if armed and False otherwise.
        trap_cheese: str | None, the type of cheese in the trap. if player 
                                 exits the function without without arming 
                                 trap succesfully, this value is None.
    '''
    if cheese == []:
        return (False, None)
    print(f'''Hunter {name}, you currently have:
Cheddar - {cheese[0][1]}
Marble - {cheese[1][1]}
Swiss - {cheese[2][1]}\n''')
    arm_status = False
    trap_status = False
    trap_cheese = None
    arm_cheese = input("Type cheese name to arm trap: ").capitalize().strip()
    while arm_status == False:
        if arm_cheese == "Back":
            return(trap_status, trap_cheese)
        elif arm_cheese != "Cheddar" and arm_cheese != "Marble" and arm_cheese != "Swiss":
            print("No such cheese!")
            print(f'''\nHunter {name}, you currently have:
Cheddar - {cheese[0][1]}
Marble - {cheese[1][1]}
Swiss - {cheese[2][1]}\n''')
        elif (arm_cheese == "Cheddar") or (arm_cheese == "Marble") or (arm_cheese == "Swiss"):
            if (arm_cheese == "Cheddar" and cheese[0][1] == 0) or (arm_cheese == "Marble" and cheese[1][1] == 0) or (arm_cheese == "Swiss" and cheese[2][1] == 0):
                print("Out of cheese!")
                print("")
                print(f'''Hunter {name}, you currently have:
Cheddar - {cheese[0][1]}
Marble - {cheese[1][1]}
Swiss - {cheese[2][1]}\n''')
            else:
                armed_status = True
                confirm_trap_cheese = input(f"Do you want to arm your trap with {arm_cheese}? ").lower().strip()
                if confirm_trap_cheese == "back":
                    return(trap_status, trap_cheese)
                elif confirm_trap_cheese == "yes":
                    print(f"{trap} is now armed with {arm_cheese}!")
                    trap_status = True
                    trap_cheese = arm_cheese
                    return(trap_status, trap_cheese)
                else:
                    print("")
                    print(f'''Hunter {name}, you currently have:
Cheddar - {cheese[0][1]}
Marble - {cheese[1][1]}
Swiss - {cheese[2][1]}\n''')
        arm_cheese = input("Type cheese name to arm trap: ").capitalize().strip()

# changing cheese variables 
def consume_cheese(to_eat: str, cheese: str) -> tuple:
    '''
    Handles the consumption of cheese.
    Parameters:
        to_eat:    str,        the type of cheese to consume during the hunt.
        cheese:    str,        all the cheeses and quantities the player 
                               currently posseses.
    Returns:
        None
    Example:
    >>> consume_cheese('cheddar', [['cheddar', 2], ['marble', 0], ['swiss', 1]]
        
    >>> consume_cheese('marble', [['cheddar', 2], ['marble', 0], ['swiss', 1]] 
    '''
    cheddar = cheese[0][1]
    marble = cheese[1][1]
    swiss = cheese[2][1]
    if to_eat == "Cheddar":
        if cheddar > 0:
            cheddar = cheese[0][1] - 1
    elif to_eat == "Marble":
        if marble > 0:
            marble = cheese[1][1] - 1
    elif to_eat == "Swiss":
        if swiss > 0:
            swiss = cheese[2][1] - 1
    cheese[0][1] = cheddar
    cheese[1][1] = marble
    cheese[2][1] = swiss

# updating points, gold and fails

def add_points(new_points, points):
    return new_points + points

def add_gold(gold_earnt, gold):
    return gold_earnt + gold  

def add_fail(new_fail, fails):
    return new_fail + fails

# hunt 

def hunt(gold: int, cheese: list, trap_cheese: str | None, points: int) -> tuple:
    '''
    Handles the hunt mechanic.
    It includes the inputs and outputs of sounding the horn, the result of 
    the hunt, the gold and points earned, and whether users want to continue 
    after failing consecutively.
    Parameters:
        gold:        int,        the quantity of gold the player possesses.
        cheese:      list,       all the cheese and quantities the player 
                                 currently posseses.
        trap_cheese: str | None, the type of cheese that the trap is currently 
                                 armed with. if its not armed, value is None.
        points:      int,        the quantity of points that the player 
                                 currently posseses.
    Returns:
        gold:        int,        the updated quantity of gold after the hunt.   
        cheese:      tuple,      all the cheese and updated quantities after 
                                 the hunt.
        points:      int,        the updated quantity of points after the hunt.
    '''
    
    cheddar = cheese[0][1]
    marble = cheese[1][1]
    swiss = cheese[2][1]
    gold_earnt = 0
    new_points = 0
    fails = 0
    horn_input = q4.sound_horn()
    while horn_input != "stop hunt":
        if trap_cheese == "Cheddar" and cheddar > 0:
            cheese_available = True
        elif trap_cheese == "Marble" and marble > 0:
            cheese_available = True
        elif trap_cheese == "Swiss" and swiss > 0:
            cheese_available = True
        else:
            cheese_available = False
        if horn_input != "yes":
            print("Do nothing.")
            new_fail = 1
            cheddar_consumed = False
            gold_earnt = 0
            new_points = 0
        elif horn_input == "yes" and cheese_available:
            if random.random() > 0.5:
                print("Nothing happens.")
                new_fail = 1
                cheddar_consumed = True
                gold_earnt = 0
                new_points = 0
            else:
                gold_earnt = 125
                new_points = 115
                new_fail = 0
                cheddar_consumed = True
                print('''Caught a Brown mouse!''')
        elif not(cheese_available): 
            print("Nothing happens. You are out of cheese!")
            new_fail = 1
            cheddar_consumed = False
            gold_earnt = 0
            new_points = 0
        if cheddar_consumed:
            consume_cheese(trap_cheese, cheese)
        points = add_points(new_points, points)
        gold = add_gold(gold_earnt, gold)
        if new_fail == 0:
            fails = 0
        else:
            fails = add_fail(new_fail, fails)
        print(f"My gold: {gold}, My points: {points}")

        cheddar = cheese[0][1]
        marble = cheese[1][1]
        swiss = cheese[2][1]
        print("")
        if fails >=5:
            if fails % 5 == 0:
                continue_hunt = input("Do you want to continue to hunt? ")
                if continue_hunt == "no":      
                    return(gold, points)
        horn_input = q4.sound_horn()
        if horn_input == "stop hunt":
            return(gold, points)

# Game Introduction
def hunters_name():
    import q1
    print('')
    import name
    hunter = (input("\nWhat's ye name, Hunter?\n"))
    if name.is_valid_name(hunter) and (name.is_profanity(hunter)) == False:
        name =  hunter
    else:
        name = 'Bob'
    print(f"Welcome to the Kingdom, Hunter {name}!")
    return name

#To train or not to train

def training(name):
    print("Before we begin, let's train you up!")
    training = input('Press "Enter" to start training or "skip" to Start Game: ')
    if training == "skip":
        trap = "Cardboard and Hook Trap"
    else:
        print("")
        trap = train.main()
    cheddar_lost = 0
    return trap

def main():
    '''
    Implement your code here.
    '''
    name = hunters_name()
    trap = training(name)
    print(f"\nWhat do ye want to do now, Hunter {name}?")
    game_actions = get_game_menu()
    choice = 0
    game_menu(trap, gold, points, cheese, name, game_actions)


if __name__ == '__main__':
    main()

# testing change_cheese function

name = "Bella"
trap = "Trap1"
cheese = [["Cheddar", 2], ["Marble", 2], ["Swiss", 2]]
# assert(change_cheese(name, trap, cheese) == (True, 'Cheddar')), "Test failed"