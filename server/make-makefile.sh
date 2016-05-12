#!/bin/bash

############################# visual studio project converter ############################
###
### Convert visual studio c++ project file(.vcxproj) to makefile
###
### Supported version:
###     visual studio 2010
###     visual studio 2012
###     visual studio 2013
###
### Tested:
###     visual studio 2013
###
### Usage:
###     Call convert_vcxproj_to_makefile function with visual studio c++ project filename.
###
###     convert_vcxproj_to_makefile $vcx_project_filename
###
###     Example:
###         convert_vcxproj_to_makefile sample.vcxproj > sample.makefile
###         This will create sample.makefile according to sample.vcxproj.
############################# visual studio project converter ############################

function get_unix_filename()
{
    path=$1
    echo "$path" | awk -F"[/.]" '{print $(NF-1)}'
}

function convert_vcxproj_to_makefile()
{
    project=$1

    project_type=$(grep ConfigurationType $project | awk -F"[<>]" '{if (NR == 1) print $3; }')

    preprocessor_option=$(grep PreprocessorDefinitions $project | sed -e 's/WIN32//' | sed -e 's/PreprocessorDefinitions//g' | sed -e "s/[;%<>()/\t\r]/ /g"  | awk '{ for (i = 1; i <= NF; i++) {printf "-D " $i " "} printf "\n"}')

    debug_option=$(echo "$preprocessor_option" | egrep "\<_DEBUG\>|\<DEBUG\>")
    release_option=$(echo "$preprocessor_option" | egrep "\<NDEBUG\>")
    
    ####################################### header #######################################
    gccVersion=$(gcc --version | awk '{if (NR == 1) print $3}' | awk -F"." '{print $1*10000+$2*100+$3}')

    echo "# Compiler flags..."
    if [ $gccVersion -gt "40802" ]; then
        echo "CPP_COMPILER = g++ -g"
    else
        echo "CPP_COMPILER = g++"
    fi
    echo "C_COMPILER = gcc"
    echo ""
    echo "# Library paths..."
    echo "Debug_Library_Path=\$(LIBDIR)"
    echo "Release_Library_Path=\$(LIBDIR)"
    echo ""
    echo "# Additional libraries..."
    echo "Debug_Libraries=\$(EXT_LIBS)"
    echo "Release_Libraries=\$(EXT_LIBS)"
    echo ""
    echo "# Preprocessor definitions..."
    echo "Debug_Preprocessor_Definitions=$debug_option"
    echo "Release_Preprocessor_Definitions=$release_option"
    echo ""
    echo "# Compiler flags..."
    echo "Debug_Compiler_Flags=-fPIC -Wall -ggdb3"
    echo "Release_Compiler_Flags=-fPIC -Wall -ggdb3"
    echo ""
    echo "InDir=./release"
    echo ""
    echo "# Builds all configurations for this project..."
    echo ".PHONY: build_all_configurations"
    echo "build_all_configurations: Release"
    echo ""
    echo "# Builds the Release configuration..."
    echo ".PHONY: Release"
    
    ######################################## main ########################################
    file_list=$(cat $project | grep "ClCompile Include" | awk -F"\"" '{print $2}' | sed -e 's/\\/\//g')
    
    ### final target
    object_file_list=$(echo "$file_list" | awk -F"/" '{print $NF}' | awk -F"." '{printf "$(InDir)/" $1 ".o ";}')

    target=$(get_unix_filename $project)
    
    echo "Release: $object_file_list"
    ###printf "\t@echo \"================ make $target ================\"\n"

    case "$project_type" in
        Application)
            printf "\t@\$(CPP_COMPILER) $object_file_list\$(Release_Library_Path) \$(Release_Libraries) -Wl,-rpath,./ -o ./bin/$target\n"
        ;;
        StaticLibrary)
            printf "\t@ar rcs ../server/lib$target.a $object_file_list\$(Release_Implicitly_Linked_Objects)\n"
        ;;
        DynamicLibrary)
            printf "\t@\$(CPP_COMPILER) $object_file_list\$(Release_Library_Path) \$(Release_Libraries) -Wl,-rpath,./ -shared -o ./server/lib$target.so\n"
        ;;
        *)
            echo "ERROR: unknown project type=$project_type, project=$project !!!!!!!!!!!!!!!!!!!"
    esac
    echo ""
    
    ### object target
    for origin_filename in $file_list; do 
        filename=$(get_unix_filename "$origin_filename")
        depend_filename=$filename".d"
        object_filename=$filename".o"
    
        echo "\$(InDir)/$object_filename: $origin_filename"
        printf "\t@\$(CPP_COMPILER) \$(Release_Preprocessor_Definitions) \$(Release_Compiler_Flags) -c $origin_filename \$(Release_Include_Path) -o \$(InDir)/$object_filename\n"
        printf "\t@echo \"Compile $origin_filename\"\n"
        echo ""
        
    done
    
    ######################################## clean ########################################
    echo "# Cleans intermediate and output files (objects, libraries, executables)..."
    echo ".PHONY: clean"
    echo "clean:"
    printf "\trm -f ./release/*.o\n"
    printf "\trm -f ./release/*.d\n"
    printf "\trm -f ./server/lib$target.a\n"
    printf "\trm -f ./server/lib$target.so\n"
    printf "\trm -f ./bin/*$target*\n"
    printf "\trm -f ./bin/*.exe\n"
    echo ""
}


########################### JUST DO IT FOR lovedance ########################### 

lovedance_project="socket/Socket datainfor/DataInfor libserverframe/LibServerFrame server/Agent server/AccountServer server/GameServer server/GroupServer server/GMServer"

time for proj in $lovedance_project ; do
    echo "converting $proj.vcxproj ..."
    convert_vcxproj_to_makefile "$proj.vcxproj" >   "$proj.makefile"
done

echo ""
echo "done!!!"
echo ""

