import os
import re

def to_pascal_case(name):
    # Remove file extension
    base_name = os.path.splitext(name)[0]
    # Split by special characters and numbers
    words = re.findall('[A-Z][a-z]*|[a-z]+|[0-9]+', base_name)
    # Capitalize first letter of each word
    pascal = ''.join(word.capitalize() for word in words)
    # Add back file extension
    return pascal + os.path.splitext(name)[1]

def rename_files_to_pascal_case(directory):
    # Get all JavaScript files in the directory
    files = [f for f in os.listdir(directory) if f.endswith('.js')]
    
    # Create a mapping of old names to new names
    name_mapping = {}
    for file_name in files:
        new_name = to_pascal_case(file_name)
        if new_name != file_name:
            name_mapping[file_name] = new_name
    
    # Print preview of changes
    if name_mapping:
        print('\nThe following files will be renamed:')
        for old_name, new_name in name_mapping.items():
            print(f'{old_name} -> {new_name}')
        
        # Confirm with user
        response = input('\nProceed with renaming? (y/n): ')
        if response.lower() != 'y':
            print('Operation cancelled.')
            return
        
        # Perform the renaming
        for old_name, new_name in name_mapping.items():
            old_path = os.path.join(directory, old_name)
            new_path = os.path.join(directory, new_name)
            os.rename(old_path, new_path)
            print(f'Renamed: {old_name} -> {new_name}')
    else:
        print('No files need to be renamed.')

if __name__ == '__main__':
    display_dir = os.path.join('ui', 'js', 'time')
    if os.path.exists(display_dir):
        print(f'Processing directory: {display_dir}')
        rename_files_to_pascal_case(display_dir)
    else:
        print(f'Directory not found: {display_dir}')