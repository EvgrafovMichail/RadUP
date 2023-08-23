import glob
import os

import invoke


@invoke.task
def clean_up(_):
    _clean_up()


@invoke.task
def build(_):
    print(f'\n{"Start Building Program":-^80}\n')

    try:
        print('[INFO]: start compiling udp_socket...', end=' ')

        socket_lib_path = os.path.join('cpp_modules', 'udp_socket')
        _compile_module(socket_lib_path)

        print('success')
        print('[INFO]: start compiling worker_functions...', end=' ')

        worker_functions_path = os.path.join('cpp_modules', 'worker_functions')
        _compile_module(worker_functions_path)

        print('success')
        print('[INFO]: start compiling main...', end=' ')

        invoke.run(f'g++ -c main.cpp')

        print('success')
        print('[INFO]: start linking objects...', end=' ')

        object_files = glob.glob("*.o")

        invoke.run(
            f'g++ {" ".join(object_files)} -lws2_32 -o main'
        )
        _clean_up()

        print('success')
        print(f'\n{"Successfully build":-^80}\n')

    except Exception as error:
        print('fail\n')
        print(f'[ERROR]: {str(error)}')
        print(f'{"Building failed":-^80}')


def _compile_module(path_to_files: str) -> None:

    source_files = []

    if os.path.exists(path_to_files):
        source_files = [
            os.path.join('.', path_to_files, source_file)
            for source_file in os.listdir(path_to_files)
            if '.cpp' in source_file
        ]

    if not source_files:
        raise RuntimeError(f'{path_to_files} folder is empty!')
    
    invoke.run(f'g++ -w -c {" ".join(source_files)}')


def _clean_up() -> None:
    file_patterns = ['*.o', '*.json']

    for file_pattern in file_patterns:
        for file in glob.glob(file_pattern):
            os.remove(file)
