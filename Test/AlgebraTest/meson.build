src = []
src += '../main.cpp'
src += 'AlgebraTest.cpp'

src += '../../src/LinearAlgebra/lin_alg.c'
src += '../../src/Geometry/shapes.c'

inc = []
inc += include_directories('../../include')

dep = []
dep += dependency('gtest')
dep += dependency('gmock')

app = executable('AlgebraTest', src, include_directories : inc, dependencies : dep)

test('AlgebraTest', app)