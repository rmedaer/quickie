import os
import configparser

QUICKIERC_FILE = '.quickierc'

def find_quickierc():
    current_dir = os.getcwd()

    while not os.path.isfile(os.path.join(current_dir, QUICKIERC_FILE)):
        parent_dir = os.path.dirname(current_dir)
        
        if current_dir == parent_dir:
            raise Exception('could not find quickierc file')
        
        current_dir = parent_dir    

    return os.path.join(current_dir, QUICKIERC_FILE) 

def read_quickierc():
    config = configparser.ConfigParser()
    config.read(find_quickierc())
    
    try:
        main_section = config['main'] 
    except configparser.NoSectionError:
        raise Exception('could not find section \'main\' in quickierc file')
    
    try:
        module = main_section['module']
    except KeyError:
        raise Exception('could not find module keyword in quickierc file')

    paths = main_section.get('path', [])
    if not isinstance(paths, list):
        paths = [paths]

    return (module, paths)

if __name__ == '__main__':
    read_quickierc()
