import os.path
import re

testSrcDir        = './src/'
droneLibsSrcDir   = '../src/'
simavrLibDir      = '/usr/include/simavr'
targetAVRplatform = '-mmcu=atmega2560'
arduinoInstallDir = '/opt/arduino'
variant           = 'mega'

sim = Environment(CCFLAGS = '-std=gnu++11')
sim.Append(CPPPATH = [simavrLibDir])
sim.Append(LIBS = ['simavr'])
sim.Program('testRunner', 'simulator/testRunner.cpp')

avr = DefaultEnvironment(CC="avr-g++",CXX="avr-g++")
# avr = DefaultEnvironment(CC = arduinoInstallDir+"/hardware/tools/avr/bin/avr-g++",
#                          CXX = arduinoInstallDir+"/hardware/tools/avr/bin/avr-g++")
#Map drone/test src onto build dir so keep *.o and *.d files out of the way
avr.VariantDir('build/droneSrc', droneLibsSrcDir, duplicate=0)
avr.VariantDir('build/testSrc', testSrcDir, duplicate=0)
avr.VariantDir('build/avrLib', './avrLibs', duplicate=0)
avr.Append(CPPPATH  = ['simulator'])
avr.Append(CPPPATH  = ['build/droneSrc', 'build/avrLib'])
avr.Append(CPPPATH  = ['build/arduino'])
avr.Append(CPPFLAGS = [targetAVRplatform, '-DF_CPU=16000000UL'])
avr.Append(CPPFLAGS = ['-std=gnu++11', '-g', '-Os', '-MMD'])
avr.Append(CPPFLAGS = ['-fno-exceptions', '-ffunction-sections',
                        '-fdata-sections', '-fno-threadsafe-statics'])
avr.Append(LIBS = 'printf_flt')
avr.Append(LINKFLAGS = ['-Wl,-u,vfprintf'])
avr.Append(LINKFLAGS = [targetAVRplatform])

if(ARGUMENTS.get('asm', False)):
    avr.Append(CPPFLAGS = ['-fverbose-asm', '-S'])

#The specific files from an arduino AVR core that we want compiled for our tests
arduinoCoreDir = arduinoInstallDir+'/hardware/arduino/avr/cores/arduino/'
arduinoFiles = ['Printable.h', 'Print.h', 'Stream.h', 'WString.h',
                'Print.cpp', 'Stream.cpp', 'WString.cpp',
                'wiring_digital.c', 'wiring_analog.c', 'wiring_private.h' ]
for file in arduinoFiles:
    filepath = arduinoCoreDir+file
    avr.Command('build/arduino/'+file, filepath, Copy("$TARGET", "$SOURCE"))

#compile the arduino mega variant
avr.Command(
    'build/arduino/pins_arduino.h',
    '/opt/arduino/hardware/arduino/avr/variants/'+variant+'/pins_arduino.h',
    Copy("$TARGET", "$SOURCE"))

avrLibSrc = Glob('build/avrLib/*.cpp') + Glob('build/arduino/*.cpp') \
            + Glob('build/arduino/*.c')

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
