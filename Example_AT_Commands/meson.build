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
creatorPostBuildVersion = '2.10'

# Creator_PostBuild_DateTime_Line - automatic insert of Creator Generated DateTime. Do not edit the line below
creatorGeneratedDateTime = '2023-05-16 16:13:00'

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
PROJECTNAME = 'Example_AT_Commands'

# OTX_Extension_print(ONETHINX_PACK_LOC = '${env:ONETHINX_PACK_LOC}')
ONETHINX_PACK_LOC = '/Volumes/Data/VScode/GIT/VSCode_OnethinxPack_macOS'

# OTX_Extension_print(ONETHINX_TOOLS_LOC = '${env:ONETHINX_TOOLS_LOC}')
ONETHINX_TOOLS_LOC = '/Volumes/Data/VScode/GIT/VSCode_OnethinxPack_macOS/tools_3.0'

CONFIG_DIR = ONETHINX_PACK_LOC + '/config'
MESON_SOURCE_LOC = meson.current_source_dir()

CREATOR_DIR = MESON_SOURCE_LOC + '/' + creatorDirectory

#================================================================================================================================#
# import tools

objcopy  		= find_program('arm-none-eabi-objcopy').path()
objdump  		= find_program('arm-none-eabi-objdump').path()
size     		= find_program('arm-none-eabi-size').path()
gdb      		= find_program('arm-none-eabi-gdb').path()
cymcuelftool 	= find_program(ONETHINX_TOOLS_LOC + '/cymcuelftool-1.0/bin/cymcuelftool').path()
readelf  		= find_program('arm-none-eabi-readelf').path()
memcalc 		= find_program(CONFIG_DIR + '/scripts/memcalc').path()

#================================================================================================================================#
# setup compile & link arguments

assembler_args = [
# Creator_PostBuild_AssemblerOptions_Start - automatic insert of assembler options by Creator_PostBuild. Do not edit below this line
	'-mcpu=cortex-m4',
	'-mfloat-abi=softfp',
	'-mfpu=fpv4-sp-d16',
	'-mthumb',
	'-g',
	'-W',
# Creator_PostBuild_AssemblerOptions_End - automatic insert of assembler options by Creator_PostBuild. Do not edit above this line
]

compiler_args     = [
# Creator_PostBuild_CompilerOptions_Start - automatic insert of compiler options by Creator_PostBuild. Do not edit below this line
	'-mcpu=cortex-m4',
	'-mfloat-abi=softfp',
	'-mfpu=fpv4-sp-d16',
	'-mthumb',
	'-g',
	'-DDEBUG',
	'-DCY_CORE_ID=0',
	'-DCY_PSOC_CREATOR_USED=1',
	'-DCY8C6347BZI_BLD53',
	'-Wall',
	'-ffunction-sections',
	'-ffat-lto-objects',
	'-Og',
# Creator_PostBuild_CompilerOptions_End - automatic insert of compiler options by Creator_PostBuild. Do not edit above this line
]

linker_args     = [
	'-T'+ONETHINX_PACK_LOC+'/linker/OnethinxCore_18.ld', 		# manual insert of OTX18 linkerfile
# Creator_PostBuild_LinkerOptions_Start - automatic insert of linker options by Creator_PostBuild. Do not edit below this line
	'-mcpu=cortex-m4',
	'-mfloat-abi=softfp',
	'-mfpu=fpv4-sp-d16',
	'-mthumb',
	'-specs=nano.specs',
	'-Wl,--gc-sections',
	'-g',
	'-ffunction-sections',
	'-Og',
	'-ffat-lto-objects',
# Creator_PostBuild_LinkerOptions_End - automatic insert of linker options by Creator_PostBuild. Do not edit above this line
]

include_dirs       = [
# OTX_Extension_HeaderFiles_Start - automatic insert of source files by the OTX VSCode extension. Do not edit below this line
	'source',
	'source/OnethinxCore',
# OTX_Extension_HeaderFiles_End - automatic insert of source files by the OTX VSCode extension. Do not edit above this line
# Creator_PostBuild_IncludeFolders_Start - automatic insert of include header folders by Creator_PostBuild. Do not edit below this line
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/cmsis/include',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/efuse',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/flash',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/gpio',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/ipc',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/lvd',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/profile',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/prot',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/sysanalog',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/sysclk',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/sysint',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/syslib',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/syspm',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/systick',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/trigmux',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/wdt',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/device',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/scb',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/middleware/ble',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/devices/psoc6/include',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/devices/psoc6/include/ip',
	'Onethinx_Creator.cydsn/',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/middleware',
# Creator_PostBuild_IncludeFolders_End - automatic insert of include header folders by Creator_PostBuild. Do not edit above this line
]

sourcefiles       = [
	# manual insert of Creator_PostBuild stripped files
	CREATOR_DIR + '/gcc/startup_psoc6_01_cm4.S',
	CREATOR_DIR + '/system_psoc6_cm4.c',
	CREATOR_DIR + '/Generated_Source/PSoC6/cyfitter_cfg_otx.c',
# OTX_Extension_SourceFiles_Start - automatic insert of source files by the OTX VSCode extension. Do not edit below this line
	'source/ATinterface.c',
	'source/OnethinxCore/OnethinxCore01.c',
	'source/OnethinxCore/OnethinxExt01.c',
	'source/main.c',
# OTX_Extension_SourceFiles_End - automatic insert of source files by the OTX VSCode extension. Do not edit above this line
# Creator_PostBuild_SourceFiles_Start - automatic insert of source files by Creator_PostBuild. Do not edit below this line
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/cyfitter_sysint_cfg.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/cymetadata.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/UART.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/efuse/cy_efuse.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/flash/cy_flash.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/gpio/cy_gpio.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/ipc/cy_ipc_drv.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/ipc/cy_ipc_sema.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/ipc/cy_ipc_pipe.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/lvd/cy_lvd.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/profile/cy_profile.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/prot/cy_prot.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/sysanalog/cy_sysanalog.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/sysclk/cy_sysclk.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/sysint/cy_sysint.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/syslib/cy_syslib.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/syslib/gcc/cy_syslib_gcc.S',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/syspm/cy_syspm.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/systick/cy_systick.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/trigmux/cy_trigmux.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/wdt/cy_wdt.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/device/cy_device.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/scb/cy_scb_common.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/scb/cy_scb_i2c.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/scb/cy_scb_ezi2c.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/scb/cy_scb_spi.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/drivers/peripheral/scb/cy_scb_uart.c',
	'Onethinx_Creator.cydsn/Generated_Source/PSoC6/pdl/middleware/ble/cy_ble_clk.c',
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

signed = custom_target(
                        PROJECTNAME + '_signed.elf',
    output           : [PROJECTNAME + '_signed.elf'],
    build_by_default : true,
    command          : [cymcuelftool, '--sign',  PROJECTNAME + '.elf', '--output', PROJECTNAME + '_signed.elf', '--hex', PROJECTNAME + '.hex'],
    depends          : [main])
	
mainout = custom_target(
                        PROJECTNAME + '.memcalc.txt',
    output           :  [PROJECTNAME + '.memcalc.txt'],
    build_by_default : true,
	console			 : true,
    command          : [memcalc, readelf, PROJECTNAME + '.elf', '0x0e0000', '0x38000', '0x10000000', '0x08000000', PROJECTNAME + '.memcalc.txt'],  #Flash 0x0e0000   SRAM 0x38000   Flash start 0x10000000   SRAM start 0x08000000
   # command          : [memcalc, PROJECTNAME + '.readelf', '0x0e0000', '0x38000', '0x10000000', '0x08000000', PROJECTNAME + '.memcalc2.txt'],  #Flash 0x0e0000   SRAM 0x38000   Flash start 0x10000000   SRAM start 0x08000000
    depends          : [main])
