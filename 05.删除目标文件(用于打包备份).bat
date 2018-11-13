del /Q project\MDK-ARM\*.bak
del /Q project\MDK-ARM\*.dep
del /Q project\MDK-ARM\JLink*
del /Q project\MDK-ARM\project.uvgui.*
del /Q project\MDK-ARM\Flash\Obj\*.htm
rd /Q /S project\MDK-ARM\Flash
rd /Q /S project\MDK-ARM\CpuRAM
rd /Q /S project\MDK-ARM\ExtSRAM


del /Q project\EWARMv6\Project.dep
del /Q project\EWARMv6\Flash
del /Q project\EWARMv6\CpuRAM
del /Q project\EWARMv6\settings
rd  /Q /S project\EWARMv6\Flash
rd /Q /S project\EWARMv6\CpuRAM
rd /Q /S project\EWARMv6\settings

del /Q /S *.tags
del /Q /S *.tags_sorted_by_file
