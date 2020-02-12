# /bin/sh
#
# Author:       Anton Scmied
# Date:         12.02.2020
# TODO:         - create a directory update structure (LPC/ BBB/ EPC)
#               - grab tools/utilites from the rootfs.tar and add these to the /utilites directory
#               - deploy rootfs.tar as bbb_update.tar in /BBB
#               - deploy bbb_update.tar on a BBBB with rsync
#               -


create_update_file_structure() {
    rm -rf /nonlinear-C15-update
    mkdir /nonlinear-C15-update
    mkdir /nonlinear-C15-update/BBB
    mkdir /nonlinear-C15-update/EPC
    mkdir /nonlinear-C15-update/LPC
    mkdir /nonlinear-C15-update/utilities
}

get_tools_from_root_fs() {




}






main() {
    create_update_file_structure




}
