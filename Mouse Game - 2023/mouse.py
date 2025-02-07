'''
Write solutions to 3. New Mouse Release here.

Author:
SID:
Unikey:
'''

'''
Keep this line!
'''
import random

TYPE_OF_MOUSE = (None, "Brown", "Field", "Grey", "White", "Tiny")


def generate_probabilities(cheese_type, enchant = False) -> tuple:
    if cheese_type == "Cheddar":
        attraction_rate = 0.5
        brown_attraction = 0.1
        field_attraction = 0.15
        grey_attraction = 0.1
        white_attraction = 0.1
        tiny_attraction = 0.05
    elif cheese_type == "Marble":
        attraction_rate = 0.4
        brown_attraction = 0.05
        field_attraction = 0.2
        grey_attraction = 0.05
        white_attraction = 0.02
        tiny_attraction = 0.08
    elif cheese_type == "Swiss":
        if enchant == True:
            attraction_rate = 0.55
            tiny_attraction = 0.4
        else:
            attraction_rate = 0.3
            tiny_attraction = 0.15
        brown_attraction = 0.01
        field_attraction = 0.05
        grey_attraction = 0.05
        white_attraction = 0.04
    
    return(round(1-attraction_rate, 2), brown_attraction, field_attraction, grey_attraction, white_attraction, tiny_attraction)


def generate_mouse(cheese="Cheddar", enchant=False) -> str | None:
    '''
    Spawn a random mouse during a hunt depending on cheese type
    Hint: You should be using TYPE_OF_MOUSE in this function.
    Returns:
        spawn_mouse: str | None, type of mouse
    '''
    probability = random.random()
    if cheese == "Cheddar":
        if probability < 0.5:
            spawn_mouse = TYPE_OF_MOUSE[0]
        elif 0.5 <= probability < 0.6:
            spawn_mouse = TYPE_OF_MOUSE[1]
        elif 0.6 <= probability < 0.75:
            spawn_mouse = TYPE_OF_MOUSE[2]
        elif 0.75 <= probability < 0.85:
            spawn_mouse= TYPE_OF_MOUSE[3]
        elif 0.85 <= probability < 0.95:
            spawn_mouse = TYPE_OF_MOUSE[4]
        else:
            spawn_mouse = TYPE_OF_MOUSE[5]
    elif cheese == "Marble":
        if probability < 0.6:
            spawn_mouse = TYPE_OF_MOUSE[0]
        elif 0.6 <= probability < 0.65:
            spawn_mouse = TYPE_OF_MOUSE[1]
        elif 0.65 <= probability < 0.85:
            spawn_mouse = TYPE_OF_MOUSE[2]
        elif 0.85 <= probability < 0.9:
            spawn_mouse= TYPE_OF_MOUSE[3]
        elif 0.9 <= probability < 0.92:
            spawn_mouse = TYPE_OF_MOUSE[4]
        else:
            spawn_mouse = TYPE_OF_MOUSE[5]
    elif cheese == "Swiss" and enchant == False:
        if probability < 0.7:
            spawn_mouse = TYPE_OF_MOUSE[0]
        elif 0.7 <= probability < 0.71:
            spawn_mouse = TYPE_OF_MOUSE[1]
        elif 0.71 <= probability < 0.76:
            spawn_mouse = TYPE_OF_MOUSE[2]
        elif 0.76 <= probability < 0.81:
            spawn_mouse= TYPE_OF_MOUSE[3]
        elif 0.81 <= probability < 0.85:
            spawn_mouse = TYPE_OF_MOUSE[4]
        else:
            spawn_mouse = TYPE_OF_MOUSE[5]
    elif cheese == "Swiss" and enchant == True:
        if probability < 0.45:
            spawn_mouse = TYPE_OF_MOUSE[0]
        elif 0.45 <= probability < 0.46:
            spawn_mouse = TYPE_OF_MOUSE[1]
        elif 0.46 <= probability < 0.51:
            spawn_mouse = TYPE_OF_MOUSE[2]
        elif 0.51 <= probability < 0.56:
            spawn_mouse= TYPE_OF_MOUSE[3]
        elif 0.56 <= probability < 0.6:
            spawn_mouse = TYPE_OF_MOUSE[4]
        else:
            spawn_mouse = TYPE_OF_MOUSE[5]

    return spawn_mouse


def generate_coat(mouse_type: str) -> str:
    if mouse_type == "Brown":
        coat = """

       ____()()
      /      @@
`~~~~~\_;m__m._>o

"""
    elif mouse_type == "Field":
        coat = """

        (`-()_.-=-.
       /66  ,  ,  \\
     =(o_/=//_(   /======`
         ~"` ~"~~`

"""
    elif mouse_type == "Grey":
        coat = """

 _  _
(o)(o)--. 
 \../ (  ) 
 m\/m--m'`--

"""
    elif mouse_type == "White":
        coat = """

            _
           (c).-.
        .-" , _( )____)

"""
    elif mouse_type == "Tiny":
        coat = """
                    _
                 .-.(c)
          (_____( )  , "-.
                 `~  ~""`

"""
    elif mouse_type == None:
        coat = ''
    return coat



def loot_lut(mouse_type: str | None) -> tuple:
    '''
    Look-up-table for gold and points for different types of mouse
    Parameter:
        mouse_type: str | None, type of mouse
    Returns:
        gold:       int, amount of gold reward for mouse
        points:     int, amount of points given for mouse
    '''
    if mouse_type == "Brown":
        gold = 125
        points = 115
    elif mouse_type == "Field":
        gold = 200
        points = 200
    elif mouse_type == "Grey":
        gold = 125
        points = 90
    elif mouse_type == "White":
        gold = 100
        points = 70
    elif mouse_type == "Tiny":
        gold = 900
        points = 200
    else:
        gold = 0
        points = 0
        
    return (gold, points)



class Mouse:
    def __init__(self):
        self.name = generate_mouse(cheese="Cheddar", enchant=False)
        self.gold, self.points = loot_lut(self.name)
        self.coat = generate_coat(self.name)

    def get_name(self) -> str:
        return self.name

    def get_gold(self) -> int:
        return self.gold
    
    def get_points(self) -> int:
        return self.points
    
    def get_coat(self) -> str:
        return self.coat
    
    def __str__(self) -> str:
        return str(self.name)
