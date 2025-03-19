import logging

logging.basicConfig(level=logging.DEBUG)

def step(number):

    logging.debug("This is greet function")
    return get_type_map(number)

if __name__ == "__main__":
    print("Script loaded successfully!")
