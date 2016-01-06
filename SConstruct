VariantDir('build/test', './src', duplicate=0)
VariantDir('build/math', '../src/math', duplicate=0)
VariantDir('build/util', '../src/util', duplicate=0)
VariantDir('build/comms', '../src/comms', duplicate=0)
env = Environment()
env['CXXFLAGS'] = ["-g","-std=gnu++11"]
env['CPPDEFINES'] = {"STAND_ALONE_TEST": None}
env['CPPPATH'] = ["./src", "../src"]
env.Program(target = 'test',
            source = [Glob('build/test/*.cpp'),
                      Glob('build/math/*.cpp'),
                      Glob('build/util/*.cpp'),
                      'build/comms/Decoder.cpp' ] )
