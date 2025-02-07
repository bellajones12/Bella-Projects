'''
Write your solution for the class Interface here.
This is your answer for Question 8.4.

Author:
SID:
Unikey:
'''

from hunter import Hunter
from cshop import CheeseShop
from trap import Trap
import game_final
import pdb
import q4
import mouse
import game

class Interface:
    def __init__(self):
        self.menu =  self.menu = {1: "Exit game", 2: "Join the Hunt", 3: "The Cheese Shop", 4: "Change Cheese"}
        self.player = Hunter()
        self.trap = self.player.trap

    # setters and getters

    def set_player(self, player):
        if isinstance(player, Hunter):
            self.player = player
        return self.player

    def get_menu(self):
        numbers = list(self.menu)
        options = list((self.menu).values())
        menu = f'''1. {options[0]}
2. {options[1]}
3. {options[2]}
4. {options[3]}'''
        return menu


    def change_cheese(self):
        cheese = [["Cheddar", self.player.have_cheese("Cheddar")], ["Marble", self.player.have_cheese("Marble")], ["Swiss", self.player.have_cheese("Swiss")]]
        if self.trap.get_one_time_enchantment():
            self.trap.set_trap_name(self.trap.get_trap_name())
        armed, trap_cheese = game_final.change_cheese(self.player.get_name(), self.trap.get_trap_name(), cheese, self.trap.get_one_time_enchantment())
        return (self.trap.set_trap_cheese(trap_cheese), self.trap.set_arm_status())


    def cheese_shop(self):
        cheese_shop_object = CheeseShop()
        cheese_shop_object.move_to(self.player)

    # def hunt(self):
    #     cheese = [["Cheddar", self.player.have_cheese("Cheddar")], ["Marble", self.player.have_cheese("Marble")], ["Swiss", self.player.have_cheese("Swiss")]]
    #     gold, points = game_final.hunt(self.player.get_gold(), cheese, self.trap.get_trap_cheese(), self.player.get_points(), self.trap.get_one_time_enchantment())
    #     return (self.player.set_gold(gold), self.player.set_points(points))

    # hunt 
    
    def hunt(self):
        cheese = [["Cheddar", self.player.have_cheese("Cheddar")], ["Marble", self.player.have_cheese("Marble")], ["Swiss", self.player.have_cheese("Swiss")]]
        gold_earnt = 0
        new_points = 0
        fails = 0
        horn_input = q4.sound_horn()
        gold = self.player.get_gold()
        points = self.player.get_points()
        while horn_input != "stop hunt":
            if self.trap.get_trap_cheese() == "Cheddar" and self.player.have_cheese() > 0:
                cheese_available = True
            elif self.trap.get_trap_cheese() == "Marble" and self.player.have_cheese("Marble") > 0:
                cheese_available = True
            elif self.trap.get_trap_cheese() == "Swiss" and self.player.have_cheese("Swiss") > 0:
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
                mouse_type = (mouse.generate_mouse(self.trap.get_trap_cheese(), self.trap.get_one_time_enchantment()))
                if mouse_type != None:
                    print(f"Caught a {mouse_type} mouse!")
                    print(mouse.generate_coat(mouse_type))
                    gold_earnt, new_points = mouse.loot_lut(mouse_type)
                    cheddar_consumed = True
                    new_fail = 0
                    if mouse_type == "Brown" and self.trap.get_one_time_enchantment():
                        if self.trap.get_trap_cheese() == "Cheddar":
                            new_points = new_points + 25
                        elif self.trap.get_trap_cheese() == "Marble":
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
                self.player.consume_cheese(self.trap.get_trap_cheese())
            self.player.update_points(new_points)
            self.player.update_gold(gold_earnt)
            gold = self.player.get_gold()
            points = self.player.get_points()
            if new_fail == 0:
                fails = 0
            else:
                fails = game.add_fail(new_fail, fails)
            print(f"My gold: {gold}, My points: {points}")
            print("")
            if fails >=5:
                if fails % 5 == 0:
                    continue_hunt = input("Do you want to continue to hunt? ")
                    if continue_hunt == "no":      
                        return(gold, points)
            horn_input = q4.sound_horn()
            self.trap.set_one_time_enchantment(False)
        return(gold, points)

    # game_menu

    def move_to(self, choice):
        try:
            choice = int(choice)
        except ValueError:
            print("Invalid input. Try again!")
            exit()
        if int(choice) == 0 or int(choice) > 4:
            print("Must be within 1 and 4.")
        if choice == 2:
            self.hunt()
        if choice == 3:
            print(CheeseShop().greet() + '\n')
            self.cheese_shop()
        elif choice == 4:
            self.change_cheese()
        if choice == '1':
            exit
