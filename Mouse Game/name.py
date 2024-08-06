'''
Answer for Question 5. Kids' Friendly.

Author:
SID:
Unikey:
'''


'''
This part should be your solution from Assignment 1, 3. Functions.
'''


# testing valid name from A1

def is_valid_length(name: str) -> bool:
    result = len(name) < 10
    return result


def is_valid_start(name: str) -> bool:
    result = (name[0] >='a' and name[0] <='z') or (name[0] >='A' and name[0] <='Z')
    return result


def is_one_word(name: str) -> bool:
    result = name.count(' ') == 0
    return result


def is_valid_name(name: str) -> bool:
    result = (is_valid_length(name) and is_valid_start(name) and is_one_word(name) and not(is_profanity(name)))
    return result


# testing if word is in database of bad words

def is_profanity(word: str, database='/home/files/list.txt', records='/home/files/history.txt') -> bool:
        
    '''
    Checks if `word` is listed in the blacklist `database`.
    Parameters:
        word:     str,  word to check against database.
        database: str,  absolute directory to file containing list of bad words.
        records:  str,  absolute directory to file to record past offenses by player.
    Returns:
        result:   bool, status of check. 
    '''

    result = False
    try:
        f = open(database, 'r')
    except FileNotFoundError:
        print("Check directory of database!")
        return result

    contents = f.read()
    occurence = contents.find(word)
    if occurence == -1:
        result = False
    else:
        result = True
        # recording word on data base
        try:
            r = open(records, 'a')
        except FileNotFoundError:
            r = open(records, 'w')
        r.write(word + '\n')
        r.close()
    f.close()
    return result

# determining how many times word is in database

def count_occurrence(word: str, file_records="/home/files/history.txt", start_flag=True) -> int:
    count = 0
    if not(isinstance(word, str)):
        print("First argument must be a string object!")
        return count
    else:
        new_word = word.lower().strip()
    try:
        first_letter = new_word[0]
    except IndexError:
        print("Must have at least one character in the string!")
        return count

    try:
        f = open(file_records, 'r')
    except FileNotFoundError:
        print(f"File records not found!")
        return count
    
    if start_flag == False: 
        while True:
            line = f.readline().strip().lower()
            if line == (new_word):
                count += 1
            if line == "":
                break

    else:
        a = True
        while a == True:
            line = f.readline().strip().lower()
            if line == "":
                a = False
            elif line[0] == first_letter:
                count = count + 1
    f.close()
    return count
    '''
    Count the occurrences of `word` contained in file_records.
    Parameters:
        word:         str,  target word to count number of occurences.
        file_records: str,  absolute directory to file that contains past records.
        start_flag:   bool, set to False to count whole words. True to count words 
                            that start with.
    Returns:
        count:        int, total number of times `word` is found in the file.
    '''

# generating a new name from a given invalid name

def generate_name(word: str, src="/home/files/animals.txt", past="/home/files/names.txt") -> str:
    '''
    Select a word from file `src` in sequence depending on the number of times word occurs.
    Parameters:
        word:     str, word to swap
        src:      str, absolute directory to file that contains safe in-game names
        past:     str, absolute directory to file that contains past names 
                       auto-generated
    Returns:
        new_name: str, the generated name to replace word
    '''
    try:
        first_letter = word[0].lower().strip()
    except TypeError:
        print("First argument must be a string object!")
        return "Bob"
    except IndexError:
        print("Must have at least one character in the string!")
        return "Bob"
    
    try:
        f = open(src)
    except FileNotFoundError:
        print(f"Source file is not found!")
        return "Bob"

    n = open(past, 'r')
    file_read = n.read()

    # creating a list of potential names that have been used before
    potential_names = []
    while True:
        line = f.readline().lower().strip()
        if line == "":
            break
        elif first_letter == line[0]:
            count = (count_occurrence(line, past, start_flag = False))
            if count == 0:
                new_name = line
                n = open(past, 'a')
                n.write(new_name + '\n')
                f.close() 
                n.close()
                return new_name
            else:
                potential_names.append(line)
                continue
    n.close()

    # checking to see which potential name was last used
    n = open(past, 'r')
    lines = n.readlines()
    i = 1
    while i < len(lines):
        last_line = (lines[-i]).lower().strip()
        if potential_names.count(last_line) == 0:
            pass
        else:
            last_used_index = potential_names.index(last_line)
            new_name_index = last_used_index + 1
            # determinin
            try:
                new_name = potential_names[new_name_index]
            except IndexError:
                new_name = potential_names[0]
            n = open(past, 'a')
            n.write(new_name + '\n')
            f.close() 
            n.close()
            return new_name
        i += 1


def main():
    name = input("Check name: ")
    while name != 's'.lower():
        if is_valid_name(name) and (is_profanity(name) == False):
            name = name
            print(f"{name} is a valid name!")
        else:
            name = generate_name(name)
            print(f"Your new name is: {name}")
        name = input("Check name: ")

    


if __name__ == "__main__":
    main()