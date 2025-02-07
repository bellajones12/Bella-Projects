'''
Write your solution to 1. Upgraded Cheese Shop here.
It should borrow code from Assignment 1.

Author:
SID:
Unikey:
'''

# initialising global variables
CHEESE_MENU = (("Cheddar", 10), ("Marble", 50), ("Swiss", 100))
cheddar_lost = 0
gold = 125
trap = 'Cardboard and Hook Trap'
cheese = [["Cheddar", 0], ["Marble", 0], ["Swiss", 0]]


def buy_cheese(gold: int) -> tuple:
    '''
    Feature for players to buy cheese from shop.
    Parameters:
        gold:           int,    amount of gold that player has
    Returns:
        gold_spent:     int,    amount of gold spent
        cheese_bought:  tuple,  amount of each type of cheese bought
    '''
    # print(gold)
    print(f"You have {gold} gold to spend.")
    cheese_wanted = input("State [cheese quantity]: ").lower()
    cheddar_bought = 0
    marble_bought = 0
    swiss_bought = 0
    gold_spent = 0
    current_gold_spent = 0
    cheese_price = 0
    while cheese_wanted != "back".lower():
        cheese_type = cheese_wanted.split(' ')[0].lower().strip()
        if len(cheese_wanted.split()) > 1:
            cheese_quantity = cheese_wanted.split(' ')[1]
            if cheese_quantity.isdigit():
                cheese_quantity = int(cheese_quantity)
        if cheese_type == "cheddar":
            cheese_price = 10
        elif cheese_type == "marble":
            cheese_price = 50
        elif cheese_type == "swiss":
            cheese_price = 100
        if cheese_type != "cheddar" and cheese_type != "marble" and cheese_type != "swiss":
            print(f"We don't sell {cheese_type}!")
            current_gold_spent = 0
        elif len(cheese_wanted.split()) < 2:
            print("Missing quantity.")
        elif not(isinstance(cheese_quantity, int)):
            print("Invalid quantity.")
        elif cheese_quantity <= 0:
            print("Must purchase positive amount of cheese.")
        elif cheese_price*cheese_quantity <= gold and cheese_quantity >= 0:
            if cheese_type == 'cheddar'.lower().strip():
                current_gold_spent = (cheese_quantity)*10                
                cheddar_bought = cheese_quantity + cheddar_bought
                print(f"Successfully purchase {cheese_quantity} {cheese_type}.")
            elif cheese_type == 'marble':
                current_gold_spent = (cheese_quantity)*50
                marble_bought = cheese_quantity + marble_bought
                print(f"Successfully purchase {cheese_quantity} {cheese_type}.")
            elif cheese_type == 'swiss':
                current_gold_spent = (cheese_quantity)*100
                swiss_bought = cheese_quantity + swiss_bought
                print(f"Successfully purchase {cheese_quantity} {cheese_type}.")
        else:
            print("Insufficient gold.")
            current_gold_spent = 0
        gold = gold - current_gold_spent
        print(f"You have {gold} gold to spend.")
        cheese_wanted = input("State [cheese quantity]: ")
        gold_spent = gold_spent + current_gold_spent
    cheese_bought = (cheddar_bought, marble_bought, swiss_bought)

    return (gold_spent, cheese_bought)


def display_inventory(gold: int, cheese: list, trap: str) -> None:
    '''
    Displays contents of inventory.
    Parameters:
        gold:   int,  amount of gold that player has
        cheese: list, amount of each type of cheese that player has
        trap:   str,  name of trap that player that player has
    '''
    print(f'''Gold - {gold}
Cheddar - {cheese[0][1]}
Marble - {cheese[1][1]}
Swiss - {cheese[2][1]}
Trap - {trap}''')



def welcome(gold, cheese, trap):
    print('''Welcome to The Cheese Shop!
Cheddar - 10 gold
Marble - 50 gold
Swiss - 100 gold''')
    option = input('''\nHow can I help ye?
1. Buy cheese
2. View inventory
3. Leave shop\n''')
    gold_spent = 0
    cheese_bought = [0, 0, 0]
    while option != '3':
        if option == "1":
            gold_spent, cheese_bought = buy_cheese(gold)
            gold = gold - gold_spent
            gold_spent = gold_spent + gold_spent
            cheese[2][1] = cheese[2][1] + cheese_bought[2]
            cheese[1][1] = cheese[1][1] + cheese_bought[1]
            cheese[0][1] = cheese[0][1] + cheese_bought[0]
        elif option == "2":
            display_inventory(gold, cheese, trap)
        else:
            print("I did not understand.")
        option = input('''\nHow can I help ye?
1. Buy cheese
2. View inventory
3. Leave shop\n''')
        if option == '3':
            return(gold, cheese)

# updating cheddar value

def add_cheddar(cheddar_lost: int, cheese: int) -> int:
    if cheese - cheddar_lost < 0:
        return 0
    else:
        return cheese - cheddar_lost


def main():
    welcome(gold, cheese, trap)


# if __name__ == "__main__":
#     main()


# buy_cheese testing
assert(buy_cheese(125) == (0, (0,0,0))), "Test failed"