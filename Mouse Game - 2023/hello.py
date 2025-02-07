name = "Bella"
def is_valid_length(name: str) -> bool:
    result = len(name) < 10
    return result
    '''
    Checks if name has length between 1 and 9 (inclusive)
    Parameters:
        name:   str, a name
    Returns:
        Whether the length of the name is valid or not
    '''
print(is_valid_length(name))  

def is_valid_start(name: str) -> bool:
    result = (name[0] >='a' and name[0] <='z') or (name[0] >='A' and name[0] <='Z')
    return result
    '''
    Checks if name starts with an alphabet
    Parameters:
        name:   str, a name
    Returns:
        Whether the name starts with an alphabetical character or not
    '''
print(is_valid_start(name))   

def is_one_word(name: str) -> bool:
    result = name.count(' ') == 0
    return result
    '''
    Checks if name is a single word
    Parameters:
        name:   str, a name
    Returns:
        Whether the name is one word or not
    '''
print(is_one_word(name))  

def is_valid_name(name: str) -> bool:
    result = (is_valid_length(name)==True and is_valid_start(name) == True and is_one_word(name) == True)
    return result
print(is_valid_name(name))