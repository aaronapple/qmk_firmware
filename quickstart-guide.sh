cd ~/projects/qmk_firmware/
qmk setup
qmk setup --help
cd qmk_firmware
qmk config user.qmk_home=/home/aaron/projects/qmk_firmware
qmk config user.keyboard=lily58/rev1 user.keymap=aa_lily58
git submodule update --init --recursive
qmk doctor
qmk compile -kb lily58/rev1 -km aa_lily58
qmk flash -kb lily58/rev1 -km aa_lily58
