#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string.h>

#include "cartridge.h"
#include "cpu.h"

using namespace std;

int main(int argc, char **argv) {
    if(argc != 1) {
        char* rompath = argv[1];

        FILE* file = fopen(rompath, "r");
        if(file == NULL) {
            cerr << "File not found "+(string) rompath << endl;
            return -1;
        }
        fseek(file, 0, SEEK_END);
        long fsize = ftell(file);
        rewind(file);
        char* buffer = (char *) malloc(fsize);

        int readsize = fread(buffer, 1, fsize, file);
        if(readsize == 0) {
            cerr << ("Could not read rom file ("+to_string(fsize)+")") << endl;
            free(buffer);
            fclose(file);
            return -1;
        }
        fclose(file);

        CartData* cart = cart_init(rompath, buffer, fsize);
        if(cart == NULL) {
            free(buffer);
            fclose(file);
            cerr << "Invalid cartridge header" << endl;
            return -1;
        }
        cout << "cpu init" << endl;
        CPU* cpu = cpu_init();
        cout << "ram init" << endl;
        RAM* ram = ram_init(buffer, fsize);
        dump(cart);

        cout << "instructions check" << endl;
        char* name = "UNKNOWN 255";
        float done = 0.0;
        for(int i = 0;i<256;i++) {
            exec(cpu, ram, (uint8_t) i, &name);

            string index = "FF";
            to_hex((uint8_t) i, &index);
            cout << "opcode(" << index << "): " << name << endl;
            if(!strstr(name, "UNKNOWN")) {
                done++;
            }
        }
        float max = 256.0;
        cout << to_string(done/max*100.0) << "% of opcodes implemented, that's " << to_string((int)done) << " out of " << to_string((int)max) << endl;
        cpu_reset(cpu, ram);

        if(!glfwInit()) {
            cerr << "Could not load GLFW ;(" << endl;
        }
        cout << "GLFW loaded correctly!" << endl;
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        int height = 640;
        int width = (int) (height * (16.0 / 9.0));

        GLFWwindow *window = glfwCreateWindow(width, height, rompath, nullptr, nullptr);
        const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        int screenw = vidmode->width;
        int screenh = vidmode->height;
        glfwSetWindowPos(window, screenw/2-width/2, screenh/2-height/2);
        glfwSwapInterval(1);
        glfwMakeContextCurrent(window);
        glfwShowWindow(window);

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            glClearColor(0.0, 0.0, 1.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);
            glfwSwapBuffers(window);
        }

        ram_dispose(ram);
        return 0;
    } else {
        cout << "Missing rom file, please launch CES with rom path as first argument" << endl;
        return -1;
    }
}