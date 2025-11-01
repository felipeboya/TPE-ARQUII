#include <library.h>
#include <utils.h>

uint64_t read(uint64_t fd, char * buffer, uint64_t count){
    return sys_read(fd, buffer, count);   // guarda los valores de los parametros en los registros correspondientes
}

uint64_t write(uint64_t fd, const char * s){
    uint64_t len = strlen(s);
    return sys_write(fd, s, len);   // guarda los valores de los parametros en los registros correspondientes
}

uint64_t snapshot(CpuSnapshotPtr snapshot){
    return sys_registers_snapshot(snapshot);
}

uint64_t setFontSize(uint64_t size){
    return sys_set_font_size(size);
}

uint64_t clearScreen(){
    return sys_clear_screen();
}

uint64_t putPixel(uint64_t x, uint64_t y, Color color){
    return sys_put_pixel(x, y, color);
}

uint64_t drawRectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint64_t color){
    return sys_draw_rectangle(x, y, width, height, color);
}

uint64_t drawFont(uint64_t x, uint64_t y, uint64_t ch, uint64_t color, uint64_t size){
    return sys_draw_font(x, y, ch, color, size);
}

uint64_t setMode(uint64_t mode, uint64_t color){
    return sys_set_mode(mode, color);
}

uint64_t sleep(uint64_t sleepTicks){
    return sys_sleep(sleepTicks);
}

uint64_t playSound(uint64_t frequency, uint64_t time){
    return sys_play_sound(frequency, time);
}

uint64_t getTime(timeStructPtr time){
    return sys_get_time(time);
}

uint64_t getScreenInfo(screenInfoPtr screenInformation){
    return sys_get_screen_info(screenInformation);
}
