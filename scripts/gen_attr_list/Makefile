SAI_PATH=<your path to SAI>
APP_NAME=gen_sai_attrs

build:
	g++ -std=c++14 -Wall -I$(SAI_PATH)/inc -I$(SAI_PATH)/meta  generate_attrs.cpp -o $(APP_NAME)

clean:
	rm -f *.o
	rm $(APP_NAME)
