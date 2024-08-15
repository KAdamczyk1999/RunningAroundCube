src = []
src += 'TemplateTest.cpp'
src += '../main.cpp'

src += '../../src/Example/example_file.c'
src += '../../src/glad.c'

inc = []
inc += include_directories('../../include')
inc += include_directories('../../include/Example')
inc += include_directories('../../include/glad')
inc += include_directories('../../include/GLFW')
inc += include_directories('../../include/KHR')

dep = []
dep += dependency('gtest')
dep += dependency('gmock')

app = executable('TemplateTest', src, include_directories : inc, dependencies : dep)

test('TemplateTest', app)