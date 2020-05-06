import os
import configparser

QUICKIE_CONFIG_FILE = '.quickieconfig'

def find_quickieconfig():
    current_dir = os.getcwd()

    while not os.path.isfile(os.path.join(current_dir, QUICKIE_CONFIG_FILE)):
        parent_dir = os.path.dirname(current_dir)
        
        if current_dir == parent_dir:
            raise Exception('could not find `.quickieconfig` file')
        
        current_dir = parent_dir    

    return current_dir, os.path.join(current_dir, QUICKIE_CONFIG_FILE)

def read_quickieconfig():
    quickieconfig_dir, quickieconfig_path = find_quickieconfig()
    config = configparser.ConfigParser(
        allow_no_value=True,
        default_section=None,
        strict=False,
    )
    config.read(quickieconfig_path)
    
    try:
        main_section = config['main'] 
    except configparser.NoSectionError:
        raise Exception('could not find section \'main\' in `.quickieconfig` file')
    
    try:
        module = main_section['module']
    except KeyError:
        raise Exception('could not find module keyword in `.quickieconfig` file')

    paths = list(filter(None, main_section.get('path', '').splitlines()))
    computed_paths = []
    for path in paths:
        if os.path.isabs(path):
            computed_paths.append(path)
        else:
            computed_paths.append(os.path.abspath(os.path.join(quickieconfig_dir, path)))

    # Automatically add path of quickieconfig file
    computed_paths.append(quickieconfig_dir)

    return (module, computed_paths, quickieconfig_path)

if __name__ == '__main__':
    print(read_quickieconfig())

# The last character of this file will be replaced by a \0
# Please do not add anything after this
