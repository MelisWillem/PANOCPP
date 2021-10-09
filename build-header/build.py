from os import path
from os import getcwd
import re

current_path = getcwd()
repo_path = path.dirname(current_path)
include_path = path.join(repo_path, 'include')

ignore_headers = [
    'vector',
    'memory',
    'math.h',
    'limits',
    'cassert',
    'iostream',
    'string',
    'stdlib.h',
    'utility']
dont_touch_headers = [
    'vector',
    'memory',
    'math.h',
    'limits',
    'cassert',
    'iostream',
    'string',
    'cstdlib',
    'utility',
    'array',
    'functional',
    'type_traits',
    'initializer_list',
    'sstream',
    'algorithm'
]

start_head_file = path.join(include_path, 'panoc/panoc.hpp')


def getPathFromRepo(file_path):
    base = path.basename(file_path)
    out_path = base
    while(base != 'include'):
        base = path.pardir(file_path)
        out_path = path.join(base, out_path)

    return out_path


def FileNameFromInclude(include_string: str):
    return include_string.split('<')[1].split('>')[0]


def parse(file_path, out):
    """
    parse the header file:
    - recursively call includes if not parsed yet
    - write the rest of the file to the output file
    """
    print("parsing: " + file_path)
    with open(file_path) as f:
        for line in f:
            include_match = re.search('#include[\s]*<[a-zA-Z./_]*>', line)
            pragma_once = re.search('#pragma once', line)
            if (include_match is not None) and (pragma_once is None):
                rel_path = FileNameFromInclude(include_match.group())
                if (rel_path not in ignore_headers) and (rel_path not in dont_touch_headers):
                    header_path = path.join(include_path, rel_path)
                    parse(header_path, out)
                    ignore_headers.append(rel_path)
            elif pragma_once is None:
                out.write(line)


def GenerateHeader():
    with open('panocpplib.hpp', 'w') as out:
        for inc in dont_touch_headers:
            out.write('#include<'+inc+'>\n')
        parse(start_head_file, out)
GenerateHeader()
