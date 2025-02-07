'''
Write your solution for the class CheeseShop here.
This is your answer for Question 8.3.

Author:
SID:
Unikey:
'''

from hunter import Hunter


class CheeseShop:
    def __init__(self):
        self.cheeses = {"Cheddar": 10, "Marble": 50, "Swiss": 100}
        self.menu = {1: "Buy cheese", 2: "View inventory", 3: "Leave shop"}
    
    def get_cheeses(self):
        cheeses = (list(self.cheeses))
        prices = list((self.cheeses).values())
        i = 0
        cheese_menu = ""
        while i < len(self.cheeses):
            if i == len(self.cheeses) - 1:
                cheese_menu = cheese_menu + (f"{cheeses[i]} - {prices[i]} gold") 
            else:
                cheese_menu = cheese_menu + (f"{cheeses[i]} - {prices[i]} gold\n")
            i += 1
        return cheese_menu

    def get_menu(self):
        numbers = list(self.menu)
        options = list((self.menu).values())
        menu = f'''1. {options[0]}
2. {options[1]}
3. {options[2]}'''
        return menu

    def greet(self):
        greeting = ('Welcome to The Cheese Shop!\nCheddar - 10 gold\nMarble - 50 gold\nSwiss - 100 gold')
        return greeting

    def buy_cheese(self, gold) -> tuple:
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
        return (gold, cheese_bought)

    def move_to(self, player: Hunter):
        print("How can I help ye?")
        print(CheeseShop.get_menu(self))
        choice = input()
        while choice != "1" and choice != "2" and choice != "3":
            try:
                int(choice)
                if int(choice) == 0 or int(choice) > 3:
                    exit()
            except ValueError:
                print("I did not understand.")
            print("\nHow can I help ye?")
            print(CheeseShop.get_menu(self))
            choice = input()
        while choice != '3':
            if choice == '1':
                inital_gold = player.get_gold()
                gold, cheese_bought = CheeseShop.buy_cheese(self, player.get_gold())
                gold = player.set_gold(gold)
                cheese = player.set_cheese(player.update_cheese(cheese_bought))
            elif choice == '2':
                print(player.display_inventory())
            print("\nHow can I help ye?")
            print(CheeseShop.get_menu(self))
            choice = input()
        if choice == '3':
            exit



cheese = CheeseShop()
player = Hunter()
# player.set_gold(300)
# cheese.move_to(player)