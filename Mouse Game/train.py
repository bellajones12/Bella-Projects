'''
Answer for Question 5 - The Training Again from Assignment 1.

Author:
SID:
unikey:
'''

# you can make more functions or global read-only variables here if you please!
import q4

def main() -> tuple:
    trap = q4.main()
    training_continued = (input('\nPress Enter to continue training and "no" to stop training: ').lower().strip())
    while training_continued != "no" and training_continued != "\x1b":
        trap, cheddar = q4.setup_trap()
        horn_input = q4.sound_horn()
        hunt_status = q4.basic_hunt(cheddar, horn_input)
        q4.end(hunt_status)
        training_continued = (input('\nPress Enter to continue training and "no" to stop training: ').lower().strip())
        if training_continued == "no" or training_continued == "\x1b":
            break
    return (trap)
    '''
    Implement your code here.
    ''' 

if __name__ == '__main__':
    main()