'''
Answer for Question 7 - PIAT: Improved Full Game.

Author:
SID:
Unikey:
'''
import game
import shop
import train
import mouse
import q4
import setup
import time
import setup

gold = 125
points = 0
cheese = [["Cheddar", 0], ["Marble", 0], ["Swiss", 0]]
enchant = False

def hunters_name():
    import name
    hunter = (input("What's ye name, Hunter? "))
    valid_name = False
    count = 0
    while valid_name == False:
        if name.is_valid_name(hunter) and (name.is_profanity(hunter)) == False:
            hunter_name = hunter
            valid_name = True
        else:
            valid_name = False
            print(f'''That's not nice!
I'll give ye 3 attempts to get it right or I'll name ye!
Let's try again...''')
            hunter = input("What's ye name, Hunter? ")
            while valid_name == False:
                if name.is_valid_name(hunter) and (name.is_profanity(hunter)) == False:
                    hunter_name = hunter
                    valid_name == True
                    return hunter_name
                else:
                    count += 1
                    print(f"Nice try. Strike {count}!")
                    hunter = input("What's ye name, Hunter? ")
                    if count == 2:
                        if name.is_valid_name(hunter) and name.is_profanity(hunter) == False:
                            hunter_name = hunter
                            valid_name = True
                            return hunter_name
                        else:
                            print(f"Nice try. Strike {count+1}!")
                            hunter_name = name.generate_name(hunter)
                            print("I told ye to be nice!!!")
                            return hunter_name
    return hunter_name


def training(hunter_name):
    print(f"Welcome to the Kingdom, Hunter {hunter_name}!")
    print("Before we begin, let's train you up!")
    training = input('Press "Enter" to start training or "skip" to Start Game: ')
    if training == "skip":
        trap = "Cardboard and Hook Trap"
    else:
        print("")
        trap = train.main()
    if trap != "Cardboard and Hook Trap":
        trap = "One-time Enchanted " + trap
        
    cheddar_lost = 0
    return trap


def game_menu(trap, gold, points, cheese, hunter_name, game_actions):
    print(f"\nWhat do ye want to do now, Hunter {hunter_name}?")
    print(game_actions)
    trap_cheese = None
    print("Enter a number between 1 and 4: ", end = '')
    choice = input()
    while choice != "1" and choice != "2" and choice != "3" and choice != "4":
        try:
            choice = int(choice)
        except ValueError:
            print("Invalid input.")
            print("Enter a number between 1 and 4: ", end = '')
            choice = input()
        if int(choice) == 0 or int(choice) > 4:
            print("Must be between 1 and 4.")
            print("Enter a number between 1 and 4: ", end = '')
            choice = input()
    while choice != '1':
        if choice == "2":
            if trap[0] == "O":
                enchant = True
            else:
                enchant = False
            gold, points = hunt(gold, cheese, trap_cheese, points, enchant)
            if trap[0] == "O":
                trap = trap[19:]
            enchant = False
        if choice == "3":
            print("")
            gold, cheese = shop.welcome(gold, cheese, trap)
        if choice == "4":
            print("")
            if trap[0] == "O":
                e_flag = True
            else:
                e_flag = False
            trap_status, trap_cheese = change_cheese(hunter_name, trap, cheese, e_flag)
        print(f"\nWhat do ye want to do now, Hunter {hunter_name}?")
        print(game_actions)
        print("Enter a number between 1 and 4: ", end = '')
        choice = input()
    if choice == "1":
        exit

def get_benefit(cheese):
    cheddar_benefit = "+25 points drop by next Brown mouse."
    marble_benefit = "+25 gold drop by next Brown mouse."
    swiss_benefit = "+0.25 attraction to Tiny mouse."
    return (cheddar_benefit, marble_benefit, swiss_benefit)
    return(f'''Cheddar: {cheddar_benefit}
Marble: {marble_benefit}
Swiss: {swiss_benefit}''')


def change_cheese(hunter_name: str, trap: str, cheese: list, e_flag: bool) -> tuple:
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
    print(f'''Hunter {hunter_name}, you currently have:
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
            print(f'''\nHunter {hunter_name}, you currently have:
Cheddar - {cheese[0][1]}
Marble - {cheese[1][1]}
Swiss - {cheese[2][1]}\n''')
        elif (arm_cheese == "Cheddar") or (arm_cheese == "Marble") or (arm_cheese == "Swiss"):
            if has_cheese(arm_cheese, cheese) == 0:
                print("Out of cheese!")
                print("")
                print(f'''Hunter {hunter_name}, you currently have:
Cheddar - {cheese[0][1]}
Marble - {cheese[1][1]}
Swiss - {cheese[2][1]}\n''')
            else:
                armed_status = True
                if e_flag == True:
                    if arm_cheese == "Cheddar":
                        benefit = get_benefit(cheese)[0]
                    elif arm_cheese == "Marble":
                        benefit = get_benefit(cheese)[1]
                    elif arm_cheese == "Swiss":
                        benefit = get_benefit(cheese)[2]
                    print(f"Your {trap} has a one-time enchantment granting {benefit}")
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
                    print(f'''Hunter {hunter_name}, you currently have:
Cheddar - {cheese[0][1]}
Marble - {cheese[1][1]}
Swiss - {cheese[2][1]}\n''')
        arm_cheese = input("Type cheese name to arm trap: ").capitalize().strip()


def consume_cheese(to_eat: str, my_cheese: list) -> tuple:
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


def has_cheese(to_check, my_cheese: list) -> int:
    if to_check== "Cheddar":
        return my_cheese[0][1]
    if to_check == "Marble":
        return my_cheese[1][1]
    if to_check == "Swiss":
        return my_cheese[2][1]

def hunt(gold: int, cheese: list, trap_cheese: str | None, points: int, enchant) -> tuple:
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
    # pdb.set_trace()
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
            mouse_type = (mouse.generate_mouse(trap_cheese, enchant))
            # mouse_type = "Brown"
            if mouse_type != None:
                print(f"Caught a {mouse_type} mouse!")
                print(mouse.generate_coat(mouse_type))
                gold_earnt, new_points = mouse.loot_lut(mouse_type)
                cheddar_consumed = True
                new_fail = 0
                if mouse_type == "Brown" and enchant == True:
                    if trap_cheese == "Cheddar":
                        new_points = new_points + 25
                    elif trap_cheese == "Marble":
                        gold_earnt = gold_earnt + 25
            else:
                print("Nothing happens.")
                new_fail = 1
                cheddar_consumed = True
                gold_earnt = 0
                new_points = 0
        elif not(cheese_available): 
            print("Nothing happens. You are out of cheese!")
            new_fail = 1
            cheddar_consumed = False
            gold_earnt = 0
            new_points = 0
        if cheddar_consumed:
            consume_cheese(trap_cheese, cheese)
        points = game.add_points(new_points, points)
        gold = game.add_gold(gold_earnt, gold)
        if new_fail == 0:
            fails = 0
        else:
            fails = game.add_fail(new_fail, fails)
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
        enchant = False
    return(gold, points) 

def run_setup():
    split_time = (time.asctime()).split(' ')
    date = split_time[2]
    month = split_time[1]
    current_time = split_time[3]
    year = split_time[4]
    timestamp = f"{date} {month} {year} {current_time}"
    verification_list = setup.verification("/home/game_master/", timestamp)
    tampered = True
    final_verification_list = verification_list[-1]
    if final_verification_list == f"{timestamp}  Verification complete.":
        tampered = False
    else:
        choice = input("Do you want to repair the game? ").strip().lower()
        if choice == "yes":
            installation_list = setup.installation("/home/game_master/", timestamp)
            tampered = False
        else:
            choice = input('''Game may malfunction and personalization will be locked.
Are you sure you want to proceed? ''')
            if choice == "yes".lower().strip():
                print("You have been warned!!!")
                tampered = True
            else:
                exit()
    return tampered

def personalization(tamper_flag):
    print('''Launching game...
.
.
.''')
    import q1
    print(" and Hayley Jane Wakenshaw\n")
    if tamper_flag == False:
        hunter_name = hunters_name()
    else:
        hunter_name = "Bob"
    return hunter_name

    

import pdb

def main():
    '''
    Implement your code here.
    '''
    tamper_flag = run_setup()
    hunter_name = personalization(tamper_flag)
    trap = training(hunter_name)
    game_actions = game.get_game_menu()
    game_menu(trap, gold, points, cheese, hunter_name, game_actions)


if __name__ == '__main__':
    main()
