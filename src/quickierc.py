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

    return current_dir, os.path.join(current_dir, QUICKIERC_FILE)

def read_quickierc():
    quickierc_dir, quickierc_path = find_quickierc()
    config = configparser.ConfigParser()
    config.read(quickierc_path)
    
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

    computed_paths = []
    for path in paths:
        if os.path.isabs(path):
            computed_paths.append(path)
        else:
            computed_paths.append(os.path.abspath(os.path.join(quickierc_dir, path)))

    # Automatically add path of quickierc file
    computed_paths.append(quickierc_dir)

    return (module, computed_paths, quickierc_path)

if __name__ == '__main__':
    print(read_quickierc())

# The last character of this file will be replaced by a \0
# Please do not add anything after this
