'''
Write your solution for the class Hunter here.
This is your answer for Question 8.2.

Author:
SID:
Unikey:
'''


import pdb

from name import is_valid_name
from name import is_profanity
from trap import Trap




class Hunter:
    def __init__(self):
        self.name = "Bob"
        self.cheese = [["Cheddar", 0], ["Marble", 0], ["Swiss", 0]]
        self.trap = Trap()
        self.gold = 125
        self.points = 0
    
    # setter and getter methods
    
    def set_name(self, name):
        if is_valid_name(name) == True and is_profanity(name) == False:
            self.name = name
        return self.name

    def set_cheese(self, cheese_value):
        if (isinstance(cheese_value, tuple)):
            self.cheese = [["Cheddar", cheese_value[0]], ["Marble", cheese_value[1]], ["Swiss", cheese_value[2]]]
        return self.cheese

    def set_gold(self, gold:int):
        if (isinstance(gold, int)):
            self.gold = gold
        return self.gold

    def set_points(self, points:int):
        if (isinstance(points, int)):
            self.points = points
        return self.points

    def get_name(self):
        return self.name

    def get_cheese(self):
        cheddar = self.cheese[0][1]
        marble = self.cheese[1][1]
        swiss = self.cheese[2][1]
        result = (f'''Cheddar - {cheddar}
Marble - {marble}
Swiss - {swiss}''')
        return result

    def get_gold(self):
        return int(self.gold)

    def get_points(self):
        return int(self.points)
    
    # updating methods

    def update_cheese(self, cheese_add):
        if (isinstance(cheese_add, tuple)):
            self.cheese[0][1] = self.cheese[0][1] + cheese_add[0]
            self.cheese[1][1] = self.cheese[1][1] + cheese_add[1]
            self.cheese[2][1] = self.cheese[2][1] + cheese_add[2]
        return self.cheese
    
    def consume_cheese(self, cheese_type):
        if cheese_type == "Cheddar" and self.cheese[0][1] > 0:
            self.cheese[0][1] -= 1
        elif cheese_type == "Marble" and self.cheese[1][1] > 0:
            self.cheese[1][1] -= 1
        elif cheese_type == "Swiss" and self.cheese[2][1] > 0:
            self.cheese[2][1] -= 1
        return self.cheese

    def have_cheese(self, cheese_type = "Cheddar"):
        if cheese_type == "Marble":
            return self.cheese[1][1]
        elif cheese_type == "Swiss":
            return self.cheese[2][1]
        elif cheese_type == "Cheddar":
            return self.cheese[0][1]
        elif not(isinstance(cheese_type, str)) or (cheese_type != "Cheddar" and cheese_type != "Marble" and cheese_type != "Swiss"):
            return 0

    
    def display_inventory(self):
        if self.trap.get_one_time_enchantment() == False:
            result = (f'''Gold - {self.gold}
Cheddar - {self.cheese[0][1]}
Marble - {self.cheese[1][1]}
Swiss - {self.cheese[2][1]}
Trap - {(self.trap).get_trap_name()}''')
        else:
            result = (f'''Gold - {self.gold}
Cheddar - {self.cheese[0][1]}
Marble - {self.cheese[1][1]}
Swiss - {self.cheese[2][1]}
Trap - One-time Enchanted {self.trap.get_trap_name()}''')
        return result

    def arm_trap(self, arm_cheese):
        if self.have_cheese(arm_cheese) != 0:
            self.trap.set_trap_cheese(arm_cheese)
        else:
            self.trap.set_trap_cheese(None)
        self.trap.set_arm_status()
    
    def update_gold(self, gold_earnt):
        if isinstance(gold_earnt, int):
            self.gold += gold_earnt
        return self.gold

    def update_points(self, new_points):
        if isinstance(new_points, int):
            self.points += new_points
        return self.points

    def __str__(self) -> str:
        str_out = (f'''Hunter {self.name}\n{self.display_inventory()}''')
        return str_out
