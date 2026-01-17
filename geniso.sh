mkdir -p iso/boot/grub
cp kernel.bin iso/boot/kernel.bin
cp terminal.so iso/boot/terminal.so
cp fs.bin iso/boot/fs.bin

grub-mkrescue -o test.iso iso
