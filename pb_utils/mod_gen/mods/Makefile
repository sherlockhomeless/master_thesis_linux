obj-m += plan_same_v.o
obj-m += random_v.o

all:
	make -C /home/vagrant/kernel_src/master_thesis_linux/ M=$(PWD) modules

clean:
	make -C /home/vagrant/kernel_src/master_thesis_linux/ M=$(PWD) clean
