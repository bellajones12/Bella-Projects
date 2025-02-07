'''
Write your solution for the class Trap here.
This is your answer for Question 8.1.

Author:
SID:
Unikey:
'''

import pdb

class Trap:
    def __init__(self):
        self.trap_name = ""
        self.trap_cheese = None
        self.arm_status = False
        self.one_time_enchantment = False
    
    def set_trap_name(self, name):
        if (name == "Cardboard and Hook Trap") or (name == "High Strain Steel Trap") or (name == "Hot Tub Trap"):
            if self.get_one_time_enchantment() and name != "Cardboard and Hook Trap":
                self.trap_name = "One-time Enchanted " + name
            else:
                self.trap_name = name


    def set_trap_cheese(self, cheese):
        if (cheese != "Cheddar") and (cheese != "Marble") and (cheese != "Swiss"):
            self.trap_cheese = None
        else:
            self.trap_cheese = cheese
        return self.trap_cheese
    
    def set_arm_status(self):
        self.arm_status = True
        if self.trap_cheese == None or self.trap_name == "":
            self.arm_status = False
        return self.arm_status

    def set_one_time_enchantment(self, enchanted = False):
        if self.trap_name == "High Strain Steel Trap" or self.trap_name == "Hot Tub Trap" and enchanted == True:
            self.one_time_enchantment = True
        else:
            self.one_time_enchantment = False
        return self.one_time_enchantment

    def get_trap_name(self):
        if Trap().get_one_time_enchantment() == True:
            self.trap_name = "One-Time Enchanted " + self.trap_name
        return self.trap_name

    def get_trap_cheese(self):
        return self.trap_cheese

    def get_arm_status(self):
        return self.arm_status

    def get_one_time_enchantment(self):
        return self.one_time_enchantment

    def get_benefit(cheese:str):
        if cheese == "Cheddar":
            benefit = "+25 points drop by next Brown mouse"
        elif cheese == "Marble":
            benefit = "+25 gold drop by next Brown mouse"
        elif cheese == "Swiss":
            benefit = "+0.25 attraction to Tiny mouse"
        else:
            benefit = None
        return benefit
        
    
    def __str__(self):
        if self.one_time_enchantment == True:
            return (f"One-time Enchanted {self.trap_name}")
        else:
            exit()

# trap1 = Trap()
# trap1.set_trap_name("High Strain Steel Trap")
# print(trap1.get_trap_name())
# print(trap1.get_one_time_enchantment())
# print(trap1.set_one_time_enchantment(enchanted=True))
# print(trap1.get_trap_name())
# print(trap1.get_one_time_enchantment())
# print(trap1.get_trap_name())
# print(trap1)

# print(trap1.set_arm_status())
# print(trap1.set_trap_cheese("Cheddar"))
# print(trap1.set_arm_status())

