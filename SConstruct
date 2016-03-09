import os.path
import re

testSrcDir        = './src/'
droneLibsSrcDir   = '../src/'
simavrLibDir      = '/usr/include/simavr'
targetAVRplatform = '-mmcu=atmega2560'

sim = Environment(CCFLAGS = '-std=gnu++11')
sim.Append(CPPPATH = [simavrLibDir])
sim.Append(LIBS = ['simavr'])
sim.Program('testRunner', 'simulator/testRunner.cpp')

avr = DefaultEnvironment(CC = "avr-gcc", CXX = "avr-g++")
#Map drone/test src onto build dir so keep *.o and *.d files out of the way
avr.VariantDir('build/droneSrc', droneLibsSrcDir, duplicate=0)
avr.VariantDir('build/testSrc', testSrcDir, duplicate=0)
avr.VariantDir('build/avrLib', './avrLibs', duplicate=0)
avr.Append(CPPPATH  = ['simulator'])
avr.Append(CPPPATH  = ['build/droneSrc', 'build/avrLib'])
avr.Append(CPPFLAGS = [targetAVRplatform, '-DF_CPU=16000000UL'])
avr.Append(CPPFLAGS = ['-std=gnu++11', '-g', '-Os', '-MMD'])
avr.Append(CPPFLAGS = ['-fno-exceptions', '-ffunction-sections',
                        '-fdata-sections', '-fno-threadsafe-statics'])
avr.Append(LIBS = 'printf_flt')
avr.Append(LINKFLAGS = ['-Wl,-u,vfprintf'])
avr.Append(LINKFLAGS = [targetAVRplatform])

avrLibSrc = Glob('build/avrLib/*.cpp')

def scanTestFile(name):
    sourceFile = open(name, "r")
    targets = [x for x in re.findall('//TESTING \"(.*)\"', sourceFile.read())]
    files = []
    for target in targets:
        search = 'build/droneSrc/'+target
        files = files + Glob(search)
    sourceFile.close()
    return files

for test in Glob('build/testSrc/*.cpp'):
    implUnderTest = scanTestFile(test.rstr())
    elf = avr.Program("testElfs/" + test.name + ".elf",
                      [test] + avrLibSrc + implUnderTest)
    avr.AddPostAction(elf, "./testRunner "+elf[0].abspath)
    Depends(elf, 'testRunner')

    runonly = ARGUMENTS.get('only', False)
    if (not runonly) or (runonly == test.name):
        avr.AlwaysBuild(elf)
