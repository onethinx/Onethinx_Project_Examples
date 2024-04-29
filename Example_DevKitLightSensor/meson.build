project('meson_test', 'c',
	version : '0.1',
	default_options : ['warning_level=3']
)

[host_machine]
system     = 'none'
cpu_family = 'arm'
cpu        = 'cortex-m4'
endian     = 'little'

#================================================================================================================================#
# check Creator_PostBuild version and set various variables for possible use

# Creator_PostBuild_Minumum_Version: '2.10' - used for checking Creator_PostBuild minimum version, do not remove

# Creator_PostBuild_Version_Line - automatic insert of Creator_PostBuild version. Do not edit the line below
creatorPostBuildVersion = '2.11'

# Creator_PostBuild_DateTime_Line - automatic insert of Creator Generated DateTime. Do not edit the line below
creatorGeneratedDateTime = '2024-01-01 00:00:00'

# Creator_PostBuild_devicePart_Line - automatic insert of device part by Creator_PostBuild. Do not edit the line below
devicePart = 'CY8C6347BZI-BLD53'

# Creator_PostBuild_linkerFile_Line - automatic insert of linker file by Creator_PostBuild. Do not edit the line below
linkerFile = './cy8c6xx7_cm4_dual.ld'

# Creator_PostBuild_SVDfile_Line - automatic insert of SVD file by Creator_PostBuild. Do not edit the line below
SVDfile = 'Onethinx_Creator.svd'

# Creator_PostBuild_prePostBuild_Lines - automatic insert of pre- and postbuild commands by Creator_PostBuild. Do not edit the two line below
preBuildCommands = ''
postBuildCommands = ''

# set directories
# Creator_PostBuild_Directory_Line - automatic insert of Creator Project main directory. Do not edit the line below
creatorDirectory = 'Onethinx_Creator.cydsn'

# OTX_Extension_print(PROJECTNAME = '${workspaceFolderBasename}')
PROJECTNAME = 'BLE-example'

# OTX_Extension_print(ONETHINX_PACK_LOC = '${env:ONETHINX_PACK_LOC}')
ONETHINX_PACK_LOC = '/Applications/OTX-Maestro'

CONFIG_DIR 			= ONETHINX_PACK_LOC + '/config'
GCC_DIR 			= ONETHINX_PACK_LOC + '/gcc-arm/bin'
BIN_DIR 			= ONETHINX_PACK_LOC + '/bin'
SCRIPTS_DIR 		= ONETHINX_PACK_LOC + '/config/scripts'

MESON_SOURCE_LOC 	= meson.current_source_dir()
CREATOR_DIR 		= MESON_SOURCE_LOC + '/' + creatorDirectory

#================================================================================================================================#
# import tools

objcopy  		= find_program(GCC_DIR + '/arm-none-eabi-objcopy').path()
objdump  		= find_program(GCC_DIR + '/arm-none-eabi-objdump').path()
size     		= find_program(GCC_DIR + '/arm-none-eabi-size').path()
gdb      		= find_program(GCC_DIR + '/arm-none-eabi-gdb').path()
cymcuelftool 	= find_program(BIN_DIR + '/cymcuelftool').path()
readelf  		= find_program(GCC_DIR + '/arm-none-eabi-readelf').path()
memcalc 		= find_program(BIN_DIR + '/memcalc').path()

#================================================================================================================================#
# setup compile & link arguments

assembler_args = [
# Creator_PostBuild_AssemblerOptions_Start - automatic insert of assembler options by Creator_PostBuild. Do not edit below this line
# Creator_PostBuild_AssemblerOptions_End - automatic insert of assembler options by Creator_PostBuild. Do not edit above this line
]

compiler_args     = [
# Creator_PostBuild_CompilerOptions_Start - automatic insert of compiler options by Creator_PostBuild. Do not edit below this line
# Creator_PostBuild_CompilerOptions_End - automatic insert of compiler options by Creator_PostBuild. Do not edit above this line
	'-O0'
]

linker_args     = [
	'-T'+SCRIPTS_DIR+'/linker/OnethinxCore_18.ld', 		# manual insert of OTX18 linkerfile
# Creator_PostBuild_LinkerOptions_Start - automatic insert of linker options by Creator_PostBuild. Do not edit below this line
# Creator_PostBuild_LinkerOptions_End - automatic insert of linker options by Creator_PostBuild. Do not edit above this line
]

include_dirs       = [
# OTX_Extension_HeaderFiles_Start - automatic insert of source files by the OTX VSCode extension. Do not edit below this line
# OTX_Extension_HeaderFiles_End - automatic insert of source files by the OTX VSCode extension. Do not edit above this line
# Creator_PostBuild_IncludeFolders_Start - automatic insert of include header folders by Creator_PostBuild. Do not edit below this line
# Creator_PostBuild_IncludeFolders_End - automatic insert of include header folders by Creator_PostBuild. Do not edit above this line
]

sourcefiles       = [
	# manual insert of Creator_PostBuild stripped files
	CREATOR_DIR + '/gcc/startup_psoc6_01_cm4.S',
	CREATOR_DIR + '/system_psoc6_cm4.c',
	CREATOR_DIR + '/Generated_Source/PSoC6/cyfitter_cfg_otx.c',
# OTX_Extension_SourceFiles_Start - automatic insert of source files by the OTX VSCode extension. Do not edit below this line
# OTX_Extension_SourceFiles_End - automatic insert of source files by the OTX VSCode extension. Do not edit above this line
# Creator_PostBuild_SourceFiles_Start - automatic insert of source files by Creator_PostBuild. Do not edit below this line
# Creator_PostBuild_SourceFiles_End - automatic insert of source files by Creator_PostBuild. Do not edit above this line
]

#================================================================================================================================#
# include libraries

cc = meson.get_compiler('c')

# example: include library.a from directory
#link_deps = declare_dependency( dependencies : cc.find_library('library', dirs : ['directory/libdir']) )
link_deps = [
# Creator_PostBuild_LibSources_Start - automatic insert of include source files by Creator_PostBuild. Do not edit below this line
# Creator_PostBuild_LibSources_End - automatic insert of include source files by Creator_PostBuild. Do not edit above this line
]

# example: include object.o from directory
#link_deps += declare_dependency( link_args : ['directory/libdir/object.o'] )

#================================================================================================================================#
# run prebuild script 

#prebuild = custom_target('prebuild', output : 'buildversion.h', command : [MESON_SOURCE_LOC+'/config/prebuild.bash']) #use prebuild.bash / prebuild.bat file
#link_deps += declare_dependency( sources : [prebuild])

#================================================================================================================================#
# build executable

main = executable(
            PROJECTNAME,          [sourcefiles],
            name_suffix         : 'elf',
            c_args              : [compiler_args],
            link_args           : [linker_args],
            dependencies        : link_deps,
            include_directories : [include_dirs] )

#================================================================================================================================#
# run post build

#uncomment for a signed build output
#signed = custom_target(
#                        PROJECTNAME + '_signed.elf',
#    output           : [PROJECTNAME + '_signed.elf'],
#    build_by_default : true,
#    command          : [cymcuelftool, '--sign',  PROJECTNAME + '.elf', '--output', PROJECTNAME + '_signed.elf', '--hex', PROJECTNAME + '.hex'],
#    depends          : [main])

mainout = custom_target(
                        PROJECTNAME + '.memcalc.txt',
    output           :  [PROJECTNAME + '.memcalc.txt'],
    build_by_default : true,
	console			 : true,
    command          : [memcalc, readelf, PROJECTNAME + '.elf', '0x0e0000', '0x38000', '0x10000000', '0x08000000', PROJECTNAME + '.memcalc.txt'],  #Flash 0x0e0000   SRAM 0x38000   Flash start 0x10000000   SRAM start 0x08000000
    depends          : [main])
