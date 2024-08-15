src = []
src += '../main.cpp'
src += 'GeometryTest.cpp'

src += '../../src/Geometry/shapes.c'
src += '../../src/glad.c'

inc = []
inc += include_directories('../../include')
inc += include_directories('../../include/Geometry')
inc += include_directories('../../include/glad')
inc += include_directories('../../include/GLFW')
inc += include_directories('../../include/KHR')

dep = []
dep += dependency('gtest')
dep += dependency('gmock')

app = executable('GeometryTest', src, include_directories : inc, dependencies : dep)

test('GeometryTest', app)